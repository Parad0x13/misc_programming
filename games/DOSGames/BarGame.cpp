/*			BarGame 1.0
	Simple game of avoiding the bars	*/

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define EnemyCount 12
#define UP 1
#define DOWN 2

using namespace std;

void initGame();

void initWinHandles();
void gotoxy(int x, int y);
void render();
void setBufferxy(int x, int y, char character, int color);
void swapBuffers();
void floodBuffer(char character, int color);
void clearBuffer();

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

bool insideScreen(COORD loc);

void gameLogic();

typedef enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
}COLORS;

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

COORD COORDwith(int x, int y) {
	COORD retVal;
	retVal.X = x;
	retVal.Y = y;
	return retVal;
}

class Actor {
public:
	Actor();

	void render();
	void move();

	COORD loc;
	char icon;
	int color;
};

Actor::Actor() {
	loc = COORDwith(5, 5);
	icon = '?';
	color = YELLOW;
}

void Actor::render() {
	setBufferxy(loc.X, loc.Y, icon, color);
}

class Player : public Actor {
public:
	Player();
};

Player::Player() {
	icon = 0x02;
	color = LIGHTGREEN;
}

class Enemy : public Actor {
public:
	Enemy();

	void render();
	void move();
	bool shouldMove();

	int size;
	clock_t tick;
	float moveSpeed;
	int direction;
};

Enemy::Enemy() {
	icon = 232;
	color = LIGHTRED;

	static int offset = 10;
	loc = COORDwith(loc.X+offset, loc.Y);
	loc.Y = rnd(25);
	offset += 5;
	size = 5;

	direction = rnd(2);
	if(direction == 0)
		direction = UP;
	else if(direction == 1)
		direction = DOWN;

	moveSpeed = 0.05f;
	int maxRandom = 10;
	float randomValue = rnd(maxRandom);
	randomValue -= maxRandom/2;
	randomValue /= 100;
	moveSpeed += randomValue;
	tick = clock();
}

void Enemy::render() {
	setBufferxy(loc.X, loc.Y, icon, color);

	for(int d=0;d<size;d++)
		setBufferxy(loc.X, loc.Y-d, icon, color);
	for(int d=0;d<size;d++)
		setBufferxy(loc.X, loc.Y+d, icon, color);
}

void Enemy::move() {
	if(shouldMove()) {
		if(direction == UP)
			loc.Y--;
		else if(direction == DOWN)
			loc.Y++;

		if(loc.Y+size >= 25)
			direction = UP;
		if(loc.Y-size <= 0)
			direction = DOWN;
	}
}

bool Enemy::shouldMove() {
	bool retVal;
	clock_t start = clock();
	float delta = start-tick;
	if(delta >= 1000*moveSpeed) {
		tick = start;
		return true;
	}
	return false;
}

HANDLE hIn;
HANDLE hOut;

// Aparantly you can have multiple screen buffers. Learn to do this later
CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];  // In [y][x] format
unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;

Player *player;
Enemy *enemies[EnemyCount];

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	while(GameActive) {
		processInput();
		render();
		gameLogic();
		Sleep(45);
	}

	return EXIT_SUCCESS;
}

void initGame() {
	player = new Player();
	for(int d=0;d<EnemyCount;d++) {
		enemies[d] = new Enemy();
	}
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(hOut, pos);
}

void render() {
	clearBuffer();

	player->render();
	for(int d=0;d<EnemyCount;d++)
		enemies[d]->render();

	swapBuffers();
}

void setBufferxy(int x, int y, char character, int color) {
	if(insideScreen(COORDwith(x, y))) {
		buffer[y][x].Char.AsciiChar = character;
		buffer[y][x].Attributes = color;
	}
}

void swapBuffers() {
	COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
	COORD dwBufferCoord = {0, 0};
	SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1};
	WriteConsoleOutput(hOut, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void floodBuffer(char character, int color) {
	for(int y=0;y<SCREEN_HEIGHT;y++) {
		for(int x=0;x<SCREEN_WIDTH;x++)
			setBufferxy(x, y, character, color);
	}
}

void clearBuffer() {
	CHAR_INFO blank = {' ', WHITE};
	for(int y=0;y<SCREEN_HEIGHT;y++) {
		for(int x=0;x<SCREEN_WIDTH;x++)
			buffer[y][x] = blank;
	}
}

void initKB() {
	for(int d=0;d<MAX_VK_SIZE;d++)
		Keys[d] = 0x0000;
}

void addKBHold(unsigned int key) {
	if(!keyPressed(key))
		Keys[key] = true;
}

void removeKBHold(unsigned int key) {
	if(keyPressed(key))
		Keys[key] = false;
}

int keyPressed(unsigned int key) {
	int index = (int)key;
	if(Keys[index])
		return true;
	return false;
}

void processInput() {
	INPUT_RECORD InRec;
	DWORD EventCount, NumRead;

	GetNumberOfConsoleInputEvents(hIn, &EventCount);
	while(EventCount > 0) {
		ReadConsoleInput(hIn, &InRec, 1, &NumRead);
		processInput_Record(InRec);
		GetNumberOfConsoleInputEvents(hIn, &EventCount);
	}
	processKeyPresses();
}

void processInput_Record(INPUT_RECORD InRec) {
	if(InRec.EventType == KEY_EVENT) {
		unsigned int key = InRec.Event.KeyEvent.wVirtualKeyCode;
		if(InRec.Event.KeyEvent.bKeyDown)
			addKBHold(key);
		else
			removeKBHold(key);
	}
}

// Virtual Keys for letters are the same as their ASCII counterparts
// Therefore they are not defined in windows.h
// Must use their Capped version too, lower 'a' won't work etc...
void processKeyPresses() {
	if(keyPressed(VK_ESCAPE))
		GameActive = false;

	/*if(keyPressed('W'))
		player->loc.Y--;
	if(keyPressed('S'))
		player->loc.Y++;
	if(keyPressed('A'))
		player->loc.X--;
	if(keyPressed('D'))
		player->loc.X++;*/
}

bool insideScreen(COORD loc) {
	if(loc.X <= 80 && loc.X >= 0 && loc.Y <= 25 && loc.Y >= 0)
		return true;
	return false;
}

void gameLogic() {
	for(int d=0;d<EnemyCount;d++) {
		enemies[d]->move();
	}
}
