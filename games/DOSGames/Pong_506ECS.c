// Pong 13.0 506ECS version
// Written to entertain myself and coworkers while deployed to Kirkuk Iraq
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#include<math.h>
#include<alloc.h>

#define timer_tickInterval 60
#define RenderInterval 0.0333f

#define bX 80
#define bY 25
#define resolution (bX*bY)
#define halfX (bX/2)
#define halfY (bY/2)

#define ESC 0x1B
#define UP 0x48
#define DOWN 0x50

#define LEFT 666	// temp
#define RIGHT 667	// temp

#define PlayerUp 'w'
#define PlayerDown 's'
#define PlayerSpecial1 'd'
#define PlayerSpecial2 'a'
#define EnemyUp UP
#define EnemyDown DOWN
#define EnemySpecial1 LEFT
#define EnemySpecial2 RIGHT

#define mUp 1
#define mDown 2

#define bUp 1	// Never used
#define bDown 2	// Never used
#define bLeft 3
#define bRight 4
#define bUpLeft 5
#define bUpRight 6
#define bDownLeft 7
#define bDownRight 8

#define movingUpwards 1
#define movingCenter 2
#define movingDownwards 3

#define NONE 0
#define PLAYER 1
#define ENEMY 2

#define BYTE unsigned char
#define NUM_SCAN_QUE 256

#define ESC_make 1
#define UP_make 72
#define UP_break 200
#define DOWN_make 80
#define DOWN_break 208
#define LEFT_make 75
#define LEFT_break 203
#define RIGHT_make 77
#define RIGHT_break 205
#define W_make 0x11
#define W_break 0x91
#define S_make 0x1F
#define S_break 0x9F
#define D_make 0x20
#define D_break 0xA0
#define A_make 0x1E
#define A_break 0x9E

#define defaultStarDensity 50
#define defaultPaddleSize 2
#define maxPower 5

#define HeatColorCount 7

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

int HeatColors[HeatColorCount] = {
	LIGHTGRAY, DARKGRAY, LIGHTBLUE, BLUE, YELLOW, LIGHTRED, RED,
};

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
	int *HeatValues;
	int size;
	int direction;
	vertex pos;
	int PowerUp1Active, PowerUp2Active;
	clock_t heatTimer, PowerUp1Timer, PowerUp2Timer;
}paddle;

paddle paddleWith(int x, int y, int size) {
	paddle retVal;
	int d, panels = 2*size+1;

	int *HeatValues = (int *)malloc(panels*sizeof(int));

	for(d=0;d<panels;d++)
		HeatValues[d] = 0;

	retVal.pos = vertexWith(x, y);
	retVal.size = size;
	retVal.direction = movingCenter;
	retVal.HeatValues = HeatValues;

	retVal.PowerUp1Active = 0;
	retVal.PowerUp2Active = 0;

	return retVal;
}

paddle changePaddleSize(paddle thePaddle, int newSize) {
	paddle retVal = paddleWith(thePaddle.pos.x, thePaddle.pos.y, newSize);
	int d;

	retVal.direction = thePaddle.direction;
	retVal.PowerUp1Active = thePaddle.PowerUp1Active;
	retVal.PowerUp2Active = thePaddle.PowerUp2Active;
	for(d=0;d<thePaddle.size;d++)
		retVal.HeatValues[d] = thePaddle.HeatValues[d];

	return retVal;
}

typedef struct {
	char icon;
	vertex pos;
	int tailSize;
	vertex *tail;
	int angle;
	float speed;
	int hits;
	clock_t moveTimer;
}ball;

ball ballWith(int x, int y, int tailSize, float speed) {
	ball retVal;
	int d;

	retVal.icon = 0x01;
	retVal.hits = 0;
	retVal.pos = vertexWith(x, y);
	retVal.tailSize = tailSize;
	retVal.tail = (vertex *)malloc(tailSize*sizeof(vertex));
	retVal.angle = bLeft;
	retVal.speed = speed;
	for(d=0;d<tailSize;d++)
		retVal.tail[d] = vertexWith(x, y);

	return retVal;
}

typedef struct {
	int *stars;
	float speed;
	int density;
	clock_t moveTimer;
}stars;

stars starsWith(float speed, int density) {
	stars retVal;
	int d;

	retVal.speed = speed;
	retVal.density = density;
	retVal.stars = (int *)malloc(resolution*sizeof(int));

	for(d=0;d<resolution;d++) {
		int value = rnd(density);
		if(value == 1)
			retVal.stars[d] = 1;
	}

	return retVal;
}

void restartGame();
ball resetBall(ball theBall);
void PlayGame();
void Goodbye();
ball BallLogic(ball theBall);
ball BallMove(ball theBall);
int BallTestScore(ball theBall);
void Animations();
void PaddleHeatProcess(paddle thePaddle);

void processInput(int value);
void processPlayerInput(int value);
void processEnemyInput(int value);

void render(void);
void renderPaddle(paddle thePaddle);
void renderBall(ball theBall);
void renderPowers();
void renderStars();

void moveStars();

int ColorForHeatValue(int HeatValue);
paddle HeatPaddleFromCollition(paddle thePaddle, vertex CollidingObject, int strength);

void incrementPower(int whichPower);
void decrementPower(int whichPower);
void specialPower(int whichPlayer, int whichPower);

void setupBoard();
void clearBoard();
void printBoard();

void setupPieces();

void boardSet(int x, int y, char value, char color);

void interrupt get_scan();
void init_keyboard();
void deinit_keyboard();
void process_keyboard();

static void interrupt(far *oldkb)();

BYTE gb_scan;
BYTE gb_scan_q[NUM_SCAN_QUE];
BYTE gb_scan_head;
BYTE gb_scan_tail;
BYTE gb_status[256];

char far *Screen_base = (char far*)0xB8000000;

volatile unsigned long timer_tick, slow_timer_tick;
static void interrupt(far *old_timer)();

static void interrupt new_timer();
void init_timer();
void deinit_timer();

vertex moveVertex(vertex location, int direction);

clock_t playerMoveTimer, enemyMoveTimer;

int vertexToWallCollide(vertex location);
int vertexToPaddleCollide(vertex location, paddle thePaddle);
int vertexToScoreCollide(vertex location);

char *board;
ball Ball, Ball2;
paddle PlayerPaddle, EnemyPaddle;
stars Stars;
float gameAccelerator;
clock_t renderTimer, accelerationTimer;

unsigned int PlayerScore, EnemyScore, gameInPlay;
unsigned int PlayerPower, EnemyPower;

void main() {
	seed();

	init_keyboard();
	init_timer();

	PlayerScore = 0;
	EnemyScore = 0;
	PlayerPower = 0;
	EnemyPower = 0;

	setupBoard();
	setupPieces();

	restartGame();

	Goodbye();
	deinit_keyboard();
	deinit_timer();
}

void restartGame() {
	gameAccelerator = 1.0f;

	gameInPlay = 1;

	PlayGame();
}

ball resetBall(ball theBall) {
	gameAccelerator = 1.0f;

	theBall.hits = 0;
	theBall.pos.x = halfX;
	theBall.pos.y = halfY;
	return theBall;
}

void PlayGame() {
	accelerationTimer = timer_tick;
	PlayerPaddle.direction = movingCenter;
	EnemyPaddle.direction = movingCenter;

	while(gameInPlay) {
		if(timer_tick-renderTimer > RenderInterval*timer_tickInterval) {
			render();
			renderTimer = timer_tick;
		}
		if(timer_tick-accelerationTimer > 5.0f*timer_tickInterval) {
			gameAccelerator += 0.1f;
			accelerationTimer = timer_tick;
		}
		Ball = BallLogic(Ball);
		Ball2 = BallLogic(Ball2);
		Animations();
		process_keyboard();
	}
}

void Goodbye() {
	clrscr();
	gotoxy(29, 8);
	printf("|                     ");
	gotoxy(29, 9);
	printf("|                    |");
	gotoxy(29, 10);
	printf("|          O         |");
	gotoxy(29, 11);
	printf("             \\_      |");

	gotoxy(25, 15);
	printf("Goodbye! Thank you for Playing!");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
}

ball BallLogic(ball theBall) {
	if(timer_tick-theBall.moveTimer > (theBall.speed/gameAccelerator)*timer_tickInterval) {
		theBall = BallMove(theBall);
		theBall.moveTimer = timer_tick;
	}
	if(BallTestScore(theBall) == PLAYER) {
		EnemyScore++;
		incrementPower(ENEMY);
		decrementPower(PLAYER);
		theBall = resetBall(theBall);
	}
	if(BallTestScore(theBall) == ENEMY) {
		PlayerScore++;
		incrementPower(PLAYER);
		decrementPower(ENEMY);
		theBall = resetBall(theBall);
	}
	return theBall;
}

ball BallMove(ball theBall) {
	int d;
	vertex newLocation = theBall.pos;

	if(theBall.angle == bLeft)
		newLocation.x--;
	if(theBall.angle == bRight)
		newLocation.x++;
	if(theBall.angle == bUpRight) {
		newLocation.x++;
		newLocation.y++;
	}
	if(theBall.angle == bDownRight) {
		newLocation.x++;
		newLocation.y--;
	}
	if(theBall.angle == bUpLeft) {
		newLocation.x--;
		newLocation.y++;
	}
	if(theBall.angle == bDownLeft) {
		newLocation.x--;
		newLocation.y--;
	}

	if(vertexToPaddleCollide(newLocation, PlayerPaddle)) {
		theBall.hits++;
		incrementPower(PLAYER);
		PlayerPaddle = HeatPaddleFromCollition(PlayerPaddle, newLocation, 500);
		if(PlayerPaddle.direction == movingUpwards)
			theBall.angle = bUpRight;
		else if(PlayerPaddle.direction == movingCenter)
			theBall.angle = bRight;
		else if(PlayerPaddle.direction == movingDownwards)
			theBall.angle = bDownRight;
	}
	else if(vertexToPaddleCollide(newLocation, EnemyPaddle)) {
		theBall.hits++;
		incrementPower(ENEMY);
		EnemyPaddle = HeatPaddleFromCollition(EnemyPaddle, newLocation, 500);
		if(EnemyPaddle.direction == movingUpwards)
			theBall.angle = bUpLeft;
		else if(EnemyPaddle.direction == movingCenter)
			theBall.angle = bLeft;
		else if(EnemyPaddle.direction == movingDownwards)
			theBall.angle = bDownLeft;
	}
	else if(vertexToWallCollide(newLocation)) {
		if(theBall.angle == bDownRight)
			theBall.angle = bUpRight;
		else if(theBall.angle == bUpRight)
			theBall.angle = bDownRight;
		else if(theBall.angle == bDownLeft)
			theBall.angle = bUpLeft;
		else if(theBall.angle == bUpLeft)
			theBall.angle = bDownLeft;
	}
	else {
		for(d=theBall.tailSize;d>0;d--)
			theBall.tail[d] = theBall.tail[d-1];
		theBall.tail[0] = theBall.pos;
		theBall.pos = newLocation;
	}
	return theBall;
}

int BallTestScore(ball theBall) {
	if(vertexToScoreCollide(theBall.pos)) {
		if(theBall.pos.x > EnemyPaddle.pos.x)
			return ENEMY;
		else if(theBall.pos.x < PlayerPaddle.pos.x)
			return PLAYER;
	}

	return NONE;
}

void Animations() {
	if(timer_tick-PlayerPaddle.heatTimer > (0.1f/gameAccelerator)*timer_tickInterval) {
		PaddleHeatProcess(PlayerPaddle);
		PlayerPaddle.heatTimer = timer_tick;
	}
	if(timer_tick-EnemyPaddle.heatTimer > (0.1f/gameAccelerator)*timer_tickInterval) {
		PaddleHeatProcess(EnemyPaddle);
		EnemyPaddle.heatTimer = timer_tick;
	}
	if(PlayerPaddle.PowerUp2Active == 1 && timer_tick-PlayerPaddle.PowerUp2Timer > 3.0f*timer_tickInterval) {
		PlayerPaddle.PowerUp2Active = 0;
		if(PlayerPaddle.size > defaultPaddleSize) {
			PlayerPaddle = changePaddleSize(PlayerPaddle, PlayerPaddle.size-1);
			PlayerPaddle.PowerUp2Active = 1;
			PlayerPaddle.PowerUp2Timer = timer_tick;
		}
	}
	if(EnemyPaddle.PowerUp2Active == 1 && timer_tick-EnemyPaddle.PowerUp2Timer > 3.0f*timer_tickInterval) {
		EnemyPaddle.PowerUp2Active = 0;
		if(EnemyPaddle.size > defaultPaddleSize) {
			EnemyPaddle = changePaddleSize(EnemyPaddle, EnemyPaddle.size-1);
			EnemyPaddle.PowerUp2Active = 1;
			EnemyPaddle.PowerUp2Timer = timer_tick;
		}
	}
	if(timer_tick-Stars.moveTimer > Stars.speed*timer_tickInterval) {
		moveStars();
		Stars.moveTimer = timer_tick;
	}
}

void PaddleHeatProcess(paddle thePaddle) {
	int d;
	int panels = (2*thePaddle.size)+1;
	int *tempHeatValues;
	tempHeatValues = (int *)malloc(panels*sizeof(int));

	tempHeatValues[0] = (thePaddle.HeatValues[0]+thePaddle.HeatValues[1])/2;
	tempHeatValues[0] -= 10;
	if(tempHeatValues[0] < 0)
		tempHeatValues[0] = 0;

	for(d=1;d<panels-1;d++) {
		tempHeatValues[d] = (thePaddle.HeatValues[d]+thePaddle.HeatValues[d-1]+thePaddle.HeatValues[d+1])/3;
		tempHeatValues[d] -= 10;
		if(tempHeatValues[d] < 0)
			tempHeatValues[d] = 0;
	}

	tempHeatValues[panels-1] = (thePaddle.HeatValues[panels-1]+thePaddle.HeatValues[panels-2])/2;
	tempHeatValues[panels-1] -= 10;
	if(tempHeatValues[panels-1] < 0)
		tempHeatValues[panels-1] = 0;

	for(d=0;d<panels;d++)
		thePaddle.HeatValues[d] = tempHeatValues[d];
}

paddle HeatPaddleFromCollition(paddle thePaddle, vertex CollidingObject, int strength) {
	paddle retVal = thePaddle;

	int location = retVal.pos.y - CollidingObject.y;

	retVal.HeatValues[retVal.size-location] += strength;

	return retVal;
}

void processInput(int value) {
	if(value == ESC)
		gameInPlay = 0;

	processPlayerInput(value);
	processEnemyInput(value);
}

void processPlayerInput(int value) {
	vertex newLocation = PlayerPaddle.pos;

	if(value == PlayerUp)
		newLocation = moveVertex(newLocation, mUp);
	if(value == PlayerDown)
		newLocation = moveVertex(newLocation, mDown);

	if(value == PlayerSpecial1)
		specialPower(PLAYER, PlayerSpecial1);
	if(value == PlayerSpecial2)
		specialPower(PLAYER, PlayerSpecial2);

	if(!vertexToWallCollide(newLocation)) {
		if(newLocation.y > PlayerPaddle.pos.y)
			PlayerPaddle.direction = movingUpwards;
		else if(newLocation.y < PlayerPaddle.pos.y)
			PlayerPaddle.direction = movingDownwards;

		PlayerPaddle.pos = newLocation;
	}
}

void processEnemyInput(int value) {
	vertex newLocation = EnemyPaddle.pos;

	if(value == EnemyUp)
		newLocation = moveVertex(newLocation, mUp);
	if(value == EnemyDown)
		newLocation = moveVertex(newLocation, mDown);

	if(value == EnemySpecial1)
		specialPower(ENEMY, EnemySpecial1);
	if(value == EnemySpecial2)
		specialPower(ENEMY, EnemySpecial2);

	if(!vertexToWallCollide(newLocation)) {
		if(newLocation.y > EnemyPaddle.pos.y)
			EnemyPaddle.direction = movingUpwards;
		else if(newLocation.y < EnemyPaddle.pos.y)
			EnemyPaddle.direction = movingDownwards;

		EnemyPaddle.pos = newLocation;
	}
}

void render() {
	clearBoard();

	renderStars();
	renderBall(Ball);
	renderBall(Ball2);
	renderPaddle(PlayerPaddle);
	renderPaddle(EnemyPaddle);
	renderPowers();

	printBoard();

	textcolor(LIGHTGRAY);
	gotoxy(23, bY);
	cprintf("506 ECS's Score: %i, Kirkuk's Score: %i", PlayerScore, EnemyScore);
	gotoxy(1, 1);
}

void renderPaddle(paddle thePaddle) {
	int d;
	if(thePaddle.pos.x < bX && thePaddle.pos.y < bY+1 && thePaddle.pos.x >= 0 && thePaddle.pos.y >= 0)
		boardSet(thePaddle.pos.x, thePaddle.pos.y, 219, ColorForHeatValue(thePaddle.HeatValues[thePaddle.size]));

	for(d=1;d<thePaddle.size+1;d++) {
		if(thePaddle.pos.x < bX && thePaddle.pos.y-d < bY+1 && thePaddle.pos.x >= 0 && thePaddle.pos.y-d >= 0)
			boardSet(thePaddle.pos.x, thePaddle.pos.y-d, 219, ColorForHeatValue(thePaddle.HeatValues[thePaddle.size-d]));
		if(thePaddle.pos.x < bX && thePaddle.pos.y+d < bY+1 && thePaddle.pos.x >= 0 && thePaddle.pos.y+d >= 0)
			boardSet(thePaddle.pos.x, thePaddle.pos.y+d, 219, ColorForHeatValue(thePaddle.HeatValues[thePaddle.size+d]));
	}
}

void renderBall(ball theBall) {
	int d, color;
	color = BLUE;
	if(theBall.hits >= 5)
		color = LIGHTRED;
	for(d=0;d<Ball.tailSize;d++) {
		if(d == Ball.tailSize-1 && theBall.hits >= 5)
			color = RED;
		boardSet(theBall.tail[d].x, theBall.tail[d].y, theBall.icon, color);
	}
	color = LIGHTBLUE;
	boardSet(theBall.pos.x, theBall.pos.y, theBall.icon, color);
}

void renderPowers() {
	int d, color;

	for(d=0;d<PlayerPower;d++) {
		if(PlayerPower < maxPower)
			color = RED;
		else if(PlayerPower == maxPower)
			color = GREEN;
		boardSet(2, bY-d, 219, color);
		boardSet(3, bY-d, 219, color);
	}

	for(d=0;d<EnemyPower;d++) {
		if(EnemyPower < maxPower)
			color = RED;
		else if(EnemyPower == maxPower)
			color = GREEN;
		boardSet(bX-1, bY-d, 219, color);
		boardSet(bX-2, bY-d, 219, color);
	}
}

void renderStars() {
	int x, y;
	for(y=0;y<bY;y++) {
		for(x=0;x<bX;x++) {
			if(Stars.stars[y*bX+x] == 1)
				boardSet(x+1, y+1, 'X', DARKGRAY);
		}
	}
}

void moveStars() {
	int x, y;
	int value, d;
	for(y=0;y<bY;y++) {
		for(x=bX-1;x>0;x--) {
			Stars.stars[y*bX+x] = Stars.stars[y*bX+(x-1)];
		}
		Stars.stars[y*bX] = 0;
		value = rnd(Stars.density);
		if(value == 1)
			Stars.stars[y*bX] = 1;
	}

	for(y=0;y<bY;y++) {
		for(x=0;x<bX;x++) {
			if(Stars.stars[y*bX+x] == 1) {
				vertex point = vertexWith(x+1, y+1);
				if(vertexToPaddleCollide(point, PlayerPaddle)) {
					PlayerPaddle = HeatPaddleFromCollition(PlayerPaddle, point, 100);
				}
				if(vertexToPaddleCollide(point, EnemyPaddle)) {
					EnemyPaddle = HeatPaddleFromCollition(EnemyPaddle, point, 100);
				}
			}
		}
	}
}

int ColorForHeatValue(int HeatValue) {
	float increments = 100.0f/HeatColorCount;
	float value = ceil((float)HeatValue/increments);
	if(value < 1)
		value = 1;
	if(value > HeatColorCount)
		value = HeatColorCount;

	return HeatColors[(int)value-1];
}

void incrementPower(int whichPower) {
	if(whichPower == PLAYER && PlayerPower < maxPower)
		PlayerPower++;
	if(whichPower == ENEMY && EnemyPower < maxPower)
		EnemyPower++;
}

void decrementPower(int whichPower) {
	if(whichPower == PLAYER && PlayerPower > 0)
		PlayerPower--;
	if(whichPower == ENEMY && EnemyPower > 0)
		EnemyPower--;
}

void specialPower(int whichPlayer, int whichPower) {
	if(whichPlayer == PLAYER && whichPower == PlayerSpecial1 && PlayerPower == maxPower) {
		PlayerPower = 0;
		Ball.angle = bRight;
		Ball2.angle = bRight;
	}
	if(whichPlayer == ENEMY && whichPower == EnemySpecial1 && EnemyPower == maxPower) {
		EnemyPower = 0;
		Ball.angle = bLeft;
		Ball2.angle = bLeft;
	}
	if(whichPlayer == PLAYER && whichPower == PlayerSpecial2 && PlayerPower == maxPower) {
		PlayerPower = 0;
		PlayerPaddle.PowerUp2Active = 1;
		PlayerPaddle = changePaddleSize(PlayerPaddle, PlayerPaddle.size+3);
		PlayerPaddle.PowerUp2Timer = timer_tick;
	}
	if(whichPlayer == ENEMY && whichPower == EnemySpecial2 && EnemyPower == maxPower) {
		EnemyPower = 0;
		EnemyPaddle.PowerUp2Active = 1;
		EnemyPaddle = changePaddleSize(EnemyPaddle, EnemyPaddle.size+3);
		EnemyPaddle.PowerUp2Timer = timer_tick;
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

void setupPieces() {
	EnemyPaddle = paddleWith(halfX+35, halfY, defaultPaddleSize);
	PlayerPaddle = paddleWith(halfX-35, halfY, defaultPaddleSize);
	Ball = ballWith(halfX, halfY, 3, 0.04f);
	Ball2 = ballWith(halfX, halfY, 3, 0.04f);
	Ball2.angle = bRight;
	Ball2.icon = 0x02;
	Stars = starsWith(0.075f, defaultStarDensity);
}

void boardSet(int x, int y, char value, char color) {
	board[2*((y-1)*bX+(x-1))] = value;
	board[2*((y-1)*bX+(x-1))+1] = color;
}

vertex moveVertex(vertex location, int direction) {
	vertex retVal = location;

	if(direction == mUp)
		retVal.y--;
	if(direction == mDown)
		retVal.y++;

	return retVal;
}

int vertexToWallCollide(vertex location) {
	if(location.y <= 0)
		return 1;
	if(location.y > bY)
		return 1;

	return 0;
}

int vertexToPaddleCollide(vertex location, paddle thePaddle) {
	int d;
	if(location.x == thePaddle.pos.x && location.y == thePaddle.pos.y)
		return 1;

	for(d=1;d<thePaddle.size+1;d++) {
		if(location.x == thePaddle.pos.x && location.y == thePaddle.pos.y-d)
			return 1;
		if(location.x == thePaddle.pos.x && location.y == thePaddle.pos.y+d)
			return 1;
	}

	return 0;
}

int vertexToScoreCollide(vertex location) {
	if(location.x <= 0)
		return 1;
	if(location.x >= bX)
		return 1;

	return 0;
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
		case UP_make:
			gb_status[UP_make] = 1;
			break;
		case UP_break:
			gb_status[UP_make] = 0;
			break;
		case DOWN_make:
			gb_status[DOWN_make] = 1;
			break;
		case DOWN_break:
			gb_status[DOWN_make] = 0;
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
		case W_make:
			gb_status[W_make] = 1;
			break;
		case W_break:
			gb_status[W_make] = 0;
			break;
		case S_make:
			gb_status[S_make] = 1;
			break;
		case S_break:
			gb_status[S_make] = 0;
			break;
		case D_make:
			gb_status[D_make] = 1;
			break;
		case D_break:
			gb_status[D_make] = 0;
			break;
		case A_make:
			gb_status[A_make] = 1;
			break;
		case A_break:
			gb_status[A_make] = 0;
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
	if(gb_status[UP_make]) {
		if(timer_tick-playerMoveTimer > (0.05/gameAccelerator)*timer_tickInterval) {
			processInput(UP);
			playerMoveTimer = timer_tick;
		}
	}
	if(gb_status[DOWN_make]) {
		if(timer_tick-playerMoveTimer > (0.05/gameAccelerator)*timer_tickInterval) {
			processInput(DOWN);
			playerMoveTimer = timer_tick;
		}
	}
	if(gb_status[LEFT_make]) {
		processInput(LEFT);
	}
	if(gb_status[RIGHT_make]) {
		processInput(RIGHT);
	}
	if(gb_status[W_make]) {
		if(timer_tick-enemyMoveTimer > (0.05/gameAccelerator)*timer_tickInterval) {
			processInput('w');
			enemyMoveTimer = timer_tick;
		}
	}
	if(gb_status[S_make]) {
		if(timer_tick-enemyMoveTimer > (0.05/gameAccelerator)*timer_tickInterval) {
			processInput('s');
			enemyMoveTimer = timer_tick;
		}
	}
	if(gb_status[D_make]) {
		processInput('d');
	}
	if(gb_status[A_make]) {
		processInput('a');
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
