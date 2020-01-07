/*	LeftRight is a simple reaction game
	Press Left||Right depending on which side is white	*/
// V1.0 - There is a problem with input being rendered too quickly
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <malloc.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define LEFT	1010
#define RIGHT	1011

using namespace std;

void initGame();
void chooseDirection();

void initWinHandles();
void gotoxy(int x, int y);
void render();
void renderLeft();
void renderRight();
void renderActors();
void swapBuffers();
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

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

class ScreenBuffer {
public:
	ScreenBuffer(){};
	ScreenBuffer(int _width, int _height);
	~ScreenBuffer() {free(buffer);};
	int resolution(){return width*height;};
	SMALL_RECT region() {SMALL_RECT retVal = {0, 0, width-1, height-1};return retVal;};
	void setBufferxy(int x, int y, char character, int color);
	void setBufferIndex(int index, char character, int color);
	void floodBuffer(char character, int color);
	void clearBuffer() {floodBuffer(' ', WHITE);};
	int height, width;
	CHAR_INFO *buffer;
}*screen;

ScreenBuffer::ScreenBuffer(int _width, int _height) {
	width = _width;height = _height;
	buffer = (CHAR_INFO *)malloc(resolution()*sizeof(CHAR_INFO));
	clearBuffer();
}

void ScreenBuffer::setBufferxy(int x, int y, char character, int color) {
	if(COORDOnScreen(COORDWith(x, y))) {
		int index = y*width+x;
		setBufferIndex(index, character, color);
	}
}

void ScreenBuffer::setBufferIndex(int index, char character, int color) {
	if(index < resolution()) {
		buffer[index].Char.AsciiChar = character;
		buffer[index].Attributes = color;
	}
}

void ScreenBuffer::floodBuffer(char character, int color) {
	for(int d=0;d<resolution();d++)
		setBufferIndex(d, character, color);
}

class Actor {
public:
	Actor();
	virtual void render();
	vector2f loc;
	char icon;
	int color;
};

Actor::Actor() {
	loc = vector2fMake(3, 3);
	icon = '?';
	color = LIGHTMAGENTA;
}

void Actor::render() {
	screen->setBufferxy((int)loc.x, (int)loc.y, icon, color);
}

vector<Actor *>actors;

HANDLE hIn, hOut;

unsigned int Keys[MAX_VK_SIZE];

bool GameActive, gameLost;
int gameDirection, score;

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	while(GameActive) {
		if(!gameLost) {
			processInput();
			render();
			Sleep(45);
		}
		else {
			screen->clearBuffer();
			swapBuffers();
			gotoxy(0, 0);
			printf("You lost with a score of: %i\n", score);
			GameActive = false;
		}
	}

	return 0;
}

void initGame() {
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	chooseDirection();
	GameActive = true;
	gameLost = false;
	score = 0;
}

void chooseDirection() {
	if(rnd(2) == 0)
		gameDirection = LEFT;
	else
		gameDirection = RIGHT;
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	screen->clearBuffer();
	if(gameDirection == LEFT)
		renderLeft();
	else
		renderRight();
	swapBuffers();
}

void renderLeft() {
	actors.clear();
	for(int x=0;x<SCREEN_WIDTH/2;x++) {
		for(int y=0;y<SCREEN_HEIGHT;y++) {
			screen->setBufferxy(x, y, 219, WHITE);
		}
	}
}

void renderRight() {
	actors.clear();
	for(int x=0;x<SCREEN_WIDTH/2;x++) {
		for(int y=0;y<SCREEN_HEIGHT;y++) {
			screen->setBufferxy(x+SCREEN_WIDTH/2+1, y, 219, WHITE);
		}
	}
}

void renderActors() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->render();
	}
}

void swapBuffers() {
	COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
	COORD dwBufferCoord = {0, 0};
	SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1};
	WriteConsoleOutput(hOut, (CHAR_INFO *)screen->buffer, dwBufferSize, dwBufferCoord, &rcRegion);
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
	if(keyPressed(VK_LEFT)) {
		if(gameDirection == LEFT) {
			chooseDirection();
			score++;
		}
		else {
			gameLost = true;
		}
	}
	if(keyPressed(VK_RIGHT)) {
		if(gameDirection == RIGHT) {
			chooseDirection();
			score++;
		}
		else {
			gameLost = true;
		}
	}
}
