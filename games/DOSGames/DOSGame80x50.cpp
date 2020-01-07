//	This barebones code allows the developer to easily create
//	80x50 resolution games in DOS prompt. ASCII characters are not allowed
//  Developed by Bryan A. Tremblay on 16May13

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define BOARD_WIDTH SCREEN_WIDTH
#define BOARD_HEIGHT (SCREEN_HEIGHT*2)

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

using namespace std;

void initGame();
void GameLogic();
void initWinHandles();
void gotoxy(int x, int y);
void render();
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

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

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

class ScreenBuffer {
public:
	ScreenBuffer();
	~ScreenBuffer() {free(buffer);free(values);};
	SMALL_RECT region() {SMALL_RECT retVal = {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1};return retVal;};
	void clearValues();
	void setValuexy(int x, int y, int color);
	void prepareScreenBuffer();
	int *values;
	CHAR_INFO *buffer;
}*screen;

ScreenBuffer::ScreenBuffer() {
	values = (int *)malloc(BOARD_WIDTH*BOARD_HEIGHT*sizeof(int));
	buffer = (CHAR_INFO *)malloc(SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(CHAR_INFO));
	clearValues();
}

void ScreenBuffer::clearValues() {
	for(int d=0;d<BOARD_WIDTH*BOARD_HEIGHT;d++)
		values[d] = BLACK;
}

void ScreenBuffer::setValuexy(int x, int y, int color) {
    int index = y*BOARD_WIDTH+x;
	values[index] = color;
}

void ScreenBuffer::prepareScreenBuffer() {
	for(int y=0;y<SCREEN_HEIGHT;y++) {
		for(int x=0;x<SCREEN_WIDTH;x++) {
			int yIndex = y*2;
			int topColor = values[yIndex*SCREEN_WIDTH+x];
            int bottomColor = values[(yIndex+1)*SCREEN_WIDTH+x];
			if(COORDOnScreen(COORDWith(x, y)) && COORDOnScreen(COORDWith(x, y+1))) {
				int index = y*SCREEN_WIDTH+x;
				buffer[index].Char.AsciiChar = 223;
                buffer[index].Attributes = topColor | bottomColor<<4;
			}
		}
	}
}

HANDLE hIn, hOut;

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
		GameLogic();
		Sleep(45);
	}
	return 0;
}

void initGame() {
	screen = new ScreenBuffer();
}

void GameLogic() {
	//
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	screen->clearValues();

	// Place rendering calls here

	screen->prepareScreenBuffer();
	swapBuffers();
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
}
