#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

using namespace std;

void initGame();
void gameLogic();

void initWinHandles();
void gotoxy(int x, int y);
void render();
void renderActors();
void setBufferxy(int x, int y, char character, int color);
void swapBuffers();
void floodBuffer(char character, int color);
void clearBuffer();
bool COORDOnScreen(COORD loc);

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

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

typedef struct vector2f {float x, y;}vector2f;
vector2f vector2fMake(float x, float y) {vector2f retVal;retVal.x = x;retVal.y = y;return retVal;}
typedef struct rect2f {vector2f tl, tr, bl, br;}rect2f;
rect2f rect2fMake(vector2f tl, vector2f tr, vector2f bl, vector2f br) {rect2f retVal;retVal.tl = tl;retVal.tr = tr;retVal.bl = bl;retVal.br = br;return retVal;}

class Actor {
public:
	Actor();
	virtual void render();
	virtual void physics();
	rect2f AABB();
	vector2f prevLoc, loc;
	vector2f size;
	char icon;
	int color;
	vector2f vel;
	clock_t tick, prevTick;
	float deltaTime;
};

Actor::Actor() {
	loc = vector2fMake(3, 3);
	icon = '?';
	color = LIGHTMAGENTA;
	vel = vector2fMake(0, 0);
	size = vector2fMake(1, 1);
	tick = clock();
}

void Actor::render() {
	setBufferxy((int)loc.x, (int)loc.y, icon, color);
}

void Actor::physics() {
	prevLoc = loc;
	prevTick = tick;
	tick = clock();
	deltaTime = (tick-prevTick)/1000.0f;
	loc.x += vel.x*deltaTime;
}

rect2f Actor::AABB() {
	vector2f tl = vector2fMake(loc.x-size.x/2.0f, loc.y+size.y/2.0f);
	vector2f tr = vector2fMake(loc.x+size.x/2.0f, loc.y+size.y/2.0f);
	vector2f bl = vector2fMake(loc.x-size.x/2.0f, loc.y-size.y/2.0f);
	vector2f br = vector2fMake(loc.x+size.x/2.0f, loc.y-size.y/2.0f);
	return rect2fMake(tl, tr, bl, br);
}

class Player : public Actor {
public:
	Player();
	void render();
};

Player::Player() {
	loc = vector2fMake(5, 10);
	icon = 0x01;
	color = LIGHTGREEN;
	vel = vector2fMake(25, 0);
}

void Player::render() {
	setBufferxy((int)prevLoc.x, (int)prevLoc.y, icon, YELLOW);
	setBufferxy((int)loc.x, (int)loc.y, icon, color);
}

class Wall : public Actor {
public:
	Wall();
};

Wall::Wall() {
	loc = vector2fMake(50, 10);
	icon = 'W';
	color = LIGHTRED;
}

vector<Actor *>actors;

Player *player;

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

HANDLE hIn, hOut;

// Aparantly you can have multiple screen buffers. Learn to do this later
CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];  // In [y][x] format
unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;

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

	return 0;
}

void initGame() {
	player = new Player();
	actors.push_back(player);
	actors.push_back(new Wall());
}

void gameLogic() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->physics();
	}
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	clearBuffer();
	renderActors();
	swapBuffers();
}

void renderActors() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->render();
	}
}

void setBufferxy(int x, int y, char character, int color) {
	if(COORDOnScreen(COORDWith(x, y))) {
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
	for(int y=0;y<SCREEN_HEIGHT;y++) {
		for(int x=0;x<SCREEN_WIDTH;x++)
			setBufferxy(x, y, ' ', WHITE);
	}
}

bool COORDOnScreen(COORD loc) {
	if(loc.X >= 0 && loc.X <= SCREEN_WIDTH-1 && loc.Y >= 0 && loc.Y <= SCREEN_HEIGHT)
		return true;
	return false;
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
}
