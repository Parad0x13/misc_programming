#ifndef __MAIN__
#define __MAIN__

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))
#define DEGREES_TO_RADIANS(degrees) ((degrees) * (M_PI / 180.0))

#define MAX_ENEMIES 100
#define FRIENDLY 0
#define ENEMY 1

using namespace std;

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

COORD pos(int x, int y);

void initWinHandles();
void gotoxy(int x, int y);

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

HANDLE hIn;
HANDLE hOut;

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

typedef struct _vertex {
	float x, y;
}vertex;

vertex vertexWith(float x, float y) {
	vertex retVal;
	retVal.x = x;
	retVal.y = y;
	return retVal;
}

typedef struct _direction {
	float x, y;
}direction;

direction directionWith(float x, float y) {
	direction retVal;
	retVal.x = x;
	retVal.y = y;
	return retVal;
}

typedef struct _velocity {
	float x, y;
}velocity;

velocity velocityWith(float x, float y) {
	velocity retVal;
	retVal.x = x;
	retVal.y = y;
	return retVal;
}

velocity zeroVelocity() {
	return velocityWith(0, 0);
}

class Actor {
public:
	vertex loc;
	velocity vel;
	void collisionDetection();
};

bool cmpVertex(vertex first, vertex second) {
	if((int)first.x == (int)second.x && (int)first.y == (int)second.y)
		return true;
	return false;
}

bool GameActive = true;
int GameScore = 0;

#endif
