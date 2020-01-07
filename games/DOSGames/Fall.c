#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<dos.h>

#define timer_tickInterval 60
#define RenderInterval 0.0333f

#define bX 80
#define bY 25
#define resolution (bX*bY)
#define halfX (bX/2)
#define halfY (bY/2)

#define BYTE unsigned char
#define NUM_SCAN_QUE 256

#define ESC 0x1B

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define ESC_make 1
#define LEFT_make 75
#define LEFT_break 203
#define RIGHT_make 77
#define RIGHT_break 205

typedef struct {
	int x, y;
}vertex;

vertex vertexWith(int x, int y) {
	vertex retVal;
	retVal.x = x;retVal.y = y;
	return retVal;
}

typedef struct {
	char icon;
	int color;
	vertex pos;
}Player;

typedef struct {
	int size;
	vertex opening;
}wall;

wall wallWith(vertex opening, int size) {
	wall retVal;
	retVal.size = size;
	retVal.opening = opening;
	return retVal;
}

void gameLoop();
void render();
void renderPlayer();
void renderWall(wall toRender);
void renderWalls();
void processInput(int value);

void Fall();
void PlayerFall();

int VertexWallCollide(vertex theVertex, wall theWall);
int VertexWallsCollide(vertex theVertex);

void movePlayer(int direction);

void boardSet(vertex pos, char value, char color);

void setupGame();
void clearBoard();
void printBoard();

void interrupt get_scan();
void init_keyboard();
void deinit_keyboard();
void process_keyboard();

volatile unsigned long timer_tick, slow_timer_tick;
static void interrupt(far *oldkb)();

static void interrupt new_timer();
void init_timer();
void deinit_timer();

static void interrupt(far *old_timer)();

char far *Screen_base = (char far*)0xB8000000;

BYTE gb_scan;
BYTE gb_scan_q[NUM_SCAN_QUE];
BYTE gb_scan_head;
BYTE gb_scan_tail;
BYTE gb_status[256];

int wallCount;
wall walls[100];

char *board;
Player player;
clock_t renderTimer, fallTimer, playerFallTimer, playerMoveTimer;
float fallInterval, playerFallInterval;
int gameInPlay;
float gameAccelerator;

void main() {
	init_keyboard();
	init_timer();

	setupGame();

	gameLoop();

	deinit_keyboard();
	deinit_timer();
}

void gameLoop() {
	gameInPlay = 1;
	gameAccelerator = 1.0f;
	while(gameInPlay) {
		process_keyboard();
		if(timer_tick-renderTimer > RenderInterval*timer_tickInterval) {
			render();
			renderTimer = timer_tick;
		}
		if(timer_tick-fallTimer > fallInterval*timer_tickInterval) {
			Fall();
			fallTimer = timer_tick;
		}
		if(timer_tick-playerFallTimer > playerFallInterval*timer_tickInterval) {
			PlayerFall();
			playerFallTimer = timer_tick;
		}
	}
}

void render() {
	clearBoard();

	renderPlayer();
	renderWalls();

	printBoard();
}

void renderPlayer() {
	boardSet(player.pos, player.icon, player.color);
}

void renderWall(wall toRender) {
	int x, d;
	int *yOpenings;
	int half;
	yOpenings = (int *)malloc(toRender.size*sizeof(int));
	half = toRender.size/2;

	for(d=0;d<toRender.size;d++)
		yOpenings[d] = toRender.opening.x-half+d;

	for(x=0;x<bX;x++) {
		int draw = 1;
		for(d=0;d<toRender.size;d++) {
			if(yOpenings[d] == x)
				draw = 0;
		}
		if(draw)
			boardSet(vertexWith(x, toRender.opening.y), 177, WHITE);
	}
}

void renderWalls() {
	int d;
	for(d=0;d<wallCount;d++) {
		renderWall(walls[d]);
	}
}

void processInput(int value) {
	if(value == ESC)
		gameInPlay = 0;
	if(value == LEFT || value == RIGHT)
		movePlayer(value);
}

void Fall() {
	//
}

void PlayerFall() {
	movePlayer(DOWN);
}

int VertexWallCollide(vertex theVertex, wall theWall) {
	int x, d;
	int *yOpenings, *yClosings;
	int half;
	yOpenings = (int *)malloc(theWall.size*sizeof(int));
	yClosings = (int *)malloc(bX-theWall.size*sizeof(int));
	half = theWall.size/2;

	for(d=0;d<theWall.size;d++)
		yOpenings[d] = theWall.opening.x-half+d;

	for(x=0;x<bX;x++) {
		int block = 1;
		for(d=0;d<theWall.size;d++) {
			if(yOpenings[d] == x)
				block = 0;
		}
		yClosings[x] = block;
	}

	for(x=0;x<bX;x++) {
		if(yClosings[x] == 1 && theVertex.x == x && theVertex.y == theWall.opening.y)
			return 1;
	}
	return 0;
}

int VertexWallsCollide(vertex theVertex) {
	int d;
	for(d=0;d<wallCount;d++) {
		if(VertexWallCollide(theVertex, walls[d]) == 1)
		   return 1;
	}
	return 0;
}

void movePlayer(int direction) {
	vertex newLocation = player.pos;

	if(direction == UP) {
		newLocation.y--;
	}
	if(direction == DOWN) {
		newLocation.y++;
	}
	if(direction == LEFT) {
		newLocation.x--;
	}
	if(direction == RIGHT) {
		newLocation.x++;
	}

	if(!VertexWallsCollide(newLocation))
	   player.pos = newLocation;
}

void boardSet(vertex pos, char value, char color) {
	board[2*(pos.y*bX+pos.x)] = value;
	board[2*(pos.y*bX+pos.x)+1] = color;
}

void setupGame() {
	board = (char *)malloc(2*resolution*sizeof(char));
	clearBoard();

	fallInterval = 0.25f;
	playerFallInterval = 0.05f;

	player.icon = 0x02;
	player.color = LIGHTBLUE;
	player.pos = vertexWith(halfX, halfY-5);

	wallCount = 0;
	walls[0] = wallWith(vertexWith(13, halfY+5), 7);
	wallCount++;
}

void clearBoard() {
	int d;
	for(d=0;d<2*resolution;d+=2) {
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

void interrupt get_scan() {
	asm {
		cli
		in al, 060h
		mov gb_scan, al
		in al, 061h
		mov bl, al
		or al, 080h
		out 061h, al
		mov al, bl
		out 061h, al
		mov al, 020h
		out 020h, al
		sti
	}

	switch(gb_scan) {
		case ESC_make:
			gb_status[gb_scan] = 1;
			break;
		case LEFT_make:
			gb_status[LEFT_make] = 1;
			break;
		case LEFT_break:
			gb_status[LEFT_make] = 0;
			break;
		case RIGHT_make:
			gb_status[RIGHT_make] = 1;
			break;
		case RIGHT_break:
			gb_status[RIGHT_make] = 0;
			break;
		default:
			break;
	}

	*(gb_scan_q+gb_scan_tail) = gb_scan;
	++gb_scan_tail;
}

void init_keyboard() {
	BYTE far *bios_key_state;

	oldkb = getvect(9);

	bios_key_state = MK_FP(0x040, 0x017);
	*bios_key_state &= (~(32 | 64));

	oldkb();

	gb_scan_head = 0;
	gb_scan_tail = 0;
	gb_scan = 0;

	setvect(9, get_scan);
}

void deinit_keyboard() {
	setvect(9, oldkb);
}

void process_keyboard() {
	if(gb_status[ESC_make]) {
		processInput(ESC);
	}
	if(gb_status[LEFT_make]) {
		if(timer_tick-playerMoveTimer > (0.05/gameAccelerator)*timer_tickInterval) {
			processInput(LEFT);
			playerMoveTimer = timer_tick;
		}
	}
	if(gb_status[RIGHT_make]) {
		if(timer_tick-playerMoveTimer > (0.05/gameAccelerator)*timer_tickInterval) {
			processInput(RIGHT);
			playerMoveTimer = timer_tick;
		}
	}
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
