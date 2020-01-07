// Kitty simulator using old pong game code shell
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#include<math.h>
#include<alloc.h>

#define timer_tickInterval 60
#define RenderInterval 0.0333f

#define _UP 1
#define _DOWN 2
#define _LEFT 3
#define _RIGHT 4
#define _FLAT 5
#define _PUSS 6

#define DEAD 0
#define ALIVE 1

#define bX 80
#define bY 25
#define resolution (bX*bY)
#define halfX (bX/2)
#define halfY (bY/2)

#define ESC 0x1B
#define UP 0x48
#define DOWN 0x50

#define BYTE unsigned char
#define NUM_SCAN_QUE 256

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

typedef struct {
	int x, y;
}vertex;

vertex vertexWith(int theX, int theY) {
	vertex retVal;
	retVal.x=theX;
	retVal.y=theY;
	return retVal;
}

typedef struct {
	float duration;
	int changeCount;
	int *fromValue[5];
	int toValue[5];
}Frame;

typedef struct {
	int status;
	int scratchPad[5];		// Remember variables, or w/e
	int frameCount;			// How many frames in the animation
	int currentFrame;
	clock_t frameTimer;
	Frame frame[5];			// Serious limiter, fix memory allocation to change this
}AnimationBlock;

AnimationBlock nullAnimationBlock() {
	AnimationBlock retVal;
	retVal.status = DEAD;
	return retVal;
}

void catCycle();
void renderCat();
void handleAnimations();

void renderFlatEye(vertex location, int size);
void renderUpEye(vertex location, int size, int offset);
void renderDownEye(vertex location, int size, int offset);
void renderLeftEye(vertex location, int size, int offset);
void renderRightEye(vertex location, int size, int offset);

void renderMouth(vertex location);

AnimationBlock RandomAnimation();
AnimationBlock SitStill(float duration);
AnimationBlock Blink();
void addAnimation(AnimationBlock animation);

void setupBoard();
void clearBoard();
void printBoard();

void boardSetXY(int x, int y, char value, char color);
void boardSet(vertex location, char value, char color);

char far *Screen_base = (char far*)0xB8000000;

volatile unsigned long timer_tick, slow_timer_tick;
static void interrupt(far *old_timer)();

static void interrupt new_timer();
void init_timer();
void deinit_timer();

char *board;

clock_t renderTimer;

vertex center;
vertex catLocation;

int leftEyeStatus, rightEyeStatus, mouthStatus;
vertex leftEyeLocation, rightEyeLocation, mouthLocation;
int leftEyeSize, rightEyeSize;
int leftEyeOffset, rightEyeOffset;

AnimationBlock AnimationStack[50];
int AnimationStackSize = 50;

AnimationBlock futureAnimation;
int futureAnimationDelay;
clock_t futureAnimationTimer;

void main() {
	seed();
	center = vertexWith(halfX, halfY);
	catLocation = center;

	leftEyeLocation = vertexWith(catLocation.x-15, catLocation.y-3);
	rightEyeLocation = vertexWith(catLocation.x+15, catLocation.y-3);
	mouthLocation = vertexWith(catLocation.x, catLocation.y+5);
	leftEyeSize = 2;
	rightEyeSize = 2;
	leftEyeOffset = 0;
	rightEyeOffset = 0;
	leftEyeStatus = _UP;
	rightEyeStatus = _UP;
	mouthStatus = _PUSS;

	init_timer();

	setupBoard();

	catCycle();

	deinit_timer();
}

void catCycle() {
	futureAnimationDelay = 2.0;
	futureAnimationTimer = timer_tick;
	while(!kbhit()) {
		if(timer_tick-renderTimer > RenderInterval*timer_tickInterval) {
			clearBoard();
			renderCat();
			printBoard();
			renderTimer = timer_tick;
		}
		handleAnimations();
		if(timer_tick-futureAnimationTimer > futureAnimationDelay*timer_tickInterval) {
			addAnimation(RandomAnimation());
			futureAnimationTimer = timer_tick;
		}
	}
}

void renderCat() {
	switch(leftEyeStatus) {
	case _UP:
		renderUpEye(leftEyeLocation, leftEyeSize, leftEyeOffset);
		break;
	case _DOWN:
		renderDownEye(leftEyeLocation, leftEyeSize, leftEyeOffset);
		break;
	case _LEFT:
		renderLeftEye(leftEyeLocation, leftEyeSize, leftEyeOffset);
		break;
	case _RIGHT:
		renderRightEye(leftEyeLocation, leftEyeSize, leftEyeOffset);
		break;
	case _FLAT:
		renderFlatEye(leftEyeLocation, leftEyeSize);
		break;
	default:
		break;
	}

	switch(rightEyeStatus) {
	case _UP:
		renderUpEye(rightEyeLocation, rightEyeSize, rightEyeOffset);
		break;
	case _DOWN:
		renderDownEye(rightEyeLocation, rightEyeSize, rightEyeOffset);
		break;
	case _LEFT:
		renderLeftEye(rightEyeLocation, rightEyeSize, rightEyeOffset);
		break;
	case _RIGHT:
		renderRightEye(rightEyeLocation, rightEyeSize, rightEyeOffset);
		break;
	case _FLAT:
		renderFlatEye(rightEyeLocation, leftEyeSize);
		break;
	default:
		break;
	}

	switch(mouthStatus) {
	case _PUSS:
		renderMouth(mouthLocation);
		break;
	default:
		break;
	}
}

void handleAnimations() {
	int currentAnimation;
	for(currentAnimation=0;currentAnimation<AnimationStackSize;currentAnimation++) {
		if(AnimationStack[currentAnimation].status == ALIVE) {
			int currentFrame, currentChange, currentChangeCount;
			currentFrame = AnimationStack[currentAnimation].currentFrame;
			currentChangeCount = AnimationStack[currentAnimation].frame[currentFrame].changeCount;
			for(currentChange=0;currentChange<currentChangeCount;currentChange++) {
				*AnimationStack[currentAnimation].frame[currentFrame].fromValue[currentChange] =
					AnimationStack[currentAnimation].frame[currentFrame].toValue[currentChange];
			}
			if(timer_tick-AnimationStack[currentAnimation].frameTimer >
			AnimationStack[currentAnimation].frame[currentFrame].duration*timer_tickInterval) {
				AnimationStack[currentAnimation].frameTimer = timer_tick;
				AnimationStack[currentAnimation].currentFrame++;
				if(AnimationStack[currentAnimation].frameCount < AnimationStack[currentAnimation].currentFrame) {
					AnimationStack[currentAnimation].status = DEAD;
				}
			}
		}
	}
}

void renderFlatEye(vertex location, int size) {
	int d;
	for(d=-size;d<size+1;d++)
		boardSetXY(location.x+d, location.y, 'X', WHITE);
}

void renderUpEye(vertex location, int size, int offset) {
	int d;
	for(d=0;d<size*2+1;d++) {
		boardSetXY(location.x-d, location.y+d-size-offset, 'X', WHITE);
		boardSetXY(location.x+d, location.y+d-size-offset, 'X', WHITE);
	}
}

void renderDownEye(vertex location, int size, int offset) {
	int d;
	for(d=0;d<size*2+1;d++) {
		boardSetXY(location.x-d, location.y-d+size+offset, 'X', WHITE);
		boardSetXY(location.x+d, location.y-d+size+offset, 'X', WHITE);
	}
}

void renderLeftEye(vertex location, int size, int offset) {
	int d;
	for(d=0;d<size*2+1;d++) {
		boardSetXY(location.x+d-size-offset, location.y-d, 'X', WHITE);
		boardSetXY(location.x+d-size-offset, location.y+d, 'X', WHITE);
	}
}

void renderRightEye(vertex location, int size, int offset) {
	int d;
	for(d=0;d<size*2+1;d++) {
		boardSetXY(location.x-d+size+offset, location.y-d, 'X', WHITE);
		boardSetXY(location.x-d+size+offset, location.y+d, 'X', WHITE);
	}
}

void renderMouth(vertex location) {
	// Left Side
	boardSetXY(location.x, location.y, 'X', WHITE);
	boardSetXY(location.x+1, location.y+1, 'X', WHITE);
	boardSetXY(location.x+2, location.y+2, 'X', WHITE);
	boardSetXY(location.x+3, location.y+2, 'X', WHITE);
	boardSetXY(location.x+4, location.y+2, 'X', WHITE);
	boardSetXY(location.x+5, location.y+2, 'X', WHITE);
	boardSetXY(location.x+6, location.y+1, 'X', WHITE);
	boardSetXY(location.x+6, location.y, 'X', WHITE);
	boardSetXY(location.x+5, location.y-1, 'X', WHITE);
	// Right Side
	boardSetXY(location.x-1, location.y+1, 'X', WHITE);
	boardSetXY(location.x-2, location.y+2, 'X', WHITE);
	boardSetXY(location.x-3, location.y+2, 'X', WHITE);
	boardSetXY(location.x-4, location.y+2, 'X', WHITE);
	boardSetXY(location.x-5, location.y+2, 'X', WHITE);
	boardSetXY(location.x-6, location.y+1, 'X', WHITE);
	boardSetXY(location.x-6, location.y, 'X', WHITE);
	boardSetXY(location.x-5, location.y-1, 'X', WHITE);
}

AnimationBlock RandomAnimation() {
	int d;
	float animationDuration;
	AnimationBlock retVal;

	retVal = Blink();
	animationDuration = 0;

	for(d=0;d<retVal.frameCount;d++)
		animationDuration += retVal.frame[d].duration;
	// ERROR HERE //
	// Problem seems to occur when animations have quick frames

	futureAnimationDelay = animationDuration+2.0;
	return retVal;
}

AnimationBlock SitStill(float duration) {
	AnimationBlock retVal;

	retVal.status = ALIVE;
	retVal.frameCount = 1;
	retVal.currentFrame = 0;
	retVal.frameTimer = timer_tick;

	retVal.frame[0].duration = duration;
	retVal.frame[0].changeCount = 0;

	return retVal;
}

AnimationBlock Blink() {
	AnimationBlock retVal;

	retVal.status = ALIVE;
	retVal.frameCount = 4;
	retVal.currentFrame = 0;
	retVal.frameTimer = timer_tick;

	retVal.scratchPad[0] = leftEyeStatus;
	retVal.scratchPad[1] = rightEyeStatus;

	retVal.frame[0].duration = 0.5;
	retVal.frame[0].changeCount = 2;
	retVal.frame[0].fromValue[0] = &leftEyeStatus;
	retVal.frame[0].toValue[0] = _FLAT;
	retVal.frame[0].fromValue[1] = &rightEyeStatus;
	retVal.frame[0].toValue[1] = _FLAT;

	retVal.frame[1].duration = 0.5;
	retVal.frame[1].changeCount = 2;
	retVal.frame[1].fromValue[0] = &leftEyeStatus;
	retVal.frame[1].toValue[0] = retVal.scratchPad[0];
	retVal.frame[1].fromValue[1] = &rightEyeStatus;
	retVal.frame[1].toValue[1] = retVal.scratchPad[1];

	retVal.frame[2].duration = 0.5;
	retVal.frame[2].changeCount = 2;
	retVal.frame[2].fromValue[0] = &leftEyeStatus;
	retVal.frame[2].toValue[0] = _FLAT;
	retVal.frame[2].fromValue[1] = &rightEyeStatus;
	retVal.frame[2].toValue[1] = _FLAT;

	retVal.frame[3].duration = 0.5;
	retVal.frame[3].changeCount = 2;
	retVal.frame[3].fromValue[0] = &leftEyeStatus;
	retVal.frame[3].toValue[0] = retVal.scratchPad[0];
	retVal.frame[3].fromValue[1] = &rightEyeStatus;
	retVal.frame[3].toValue[1] = retVal.scratchPad[1];

	return retVal;
}

void addAnimation(AnimationBlock animation) {
	int d;
	for(d=0;d<AnimationStackSize;d++) {
		if(AnimationStack[d].status == DEAD)
			AnimationStack[d] = animation;
	}
}

void setupBoard() {
	board = (char *)malloc(2*resolution*sizeof(char));
	clearBoard();
}

void clearBoard() {
	int d;
	for(d=0;d<resolution*2;d+=2) {
		board[d] = ' ';
		board[d+1] = BLACK;
	}
}

void printBoard() {
	int d;

	unsigned char far *target = Screen_base;
	for(d=0;d<2*resolution;d++) {
		*target = board[d];
		++target;
	}
}

void boardSetXY(int x, int y, char value, char color) {
	board[2*((y-1)*bX+(x-1))] = value;
	board[2*((y-1)*bX+(x-1))+1] = color;
}

void boardSet(vertex location, char value, char color) {
	boardSetXY(location.x, location.y, value, color);
}

static void interrupt new_timer() {
	asm cli
	timer_tick++;

	if(!(timer_tick & 3)) {
		old_timer();
		slow_timer_tick++;
	}
	else {
		asm {
			mov al, 20h
			out 20h, al
		}
	}
	asm sti
}

void init_timer() {
	unsigned int interval = 1193180/timer_tickInterval;

	timer_tick = slow_timer_tick = 01;
	old_timer = getvect(8);

	asm cli

	asm {
		mov bx, interval
		mov al, 00110110b
		out 43h, al
		mov al, bl
		out 40h, al
		mov al, bh
		out 40h, al
	}

	setvect(8, new_timer);

	asm sti
}

void deinit_timer() {
	asm cli

	asm {
		xor bx, bx
		mov al, 00110110b
		out 43h, al
		mov al, bl
		out 40h, al
		mov al, bh
		out 40h, al
	}

	setvect(8, old_timer);

	asm sti
}
