/*	DOSZSquare is a play off the equation used to generate
	the Mandelbrot fractal. f(Zn+1)=Zn^2+C
	This is a simple fractal generator, nothing too special	*/
// V1.0
// V1.1 - 	Fixed increase iteration button
//			Added ability to change power of
//				Mandelbrot set with buttons k, l

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <malloc.h>
#include <complex>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

using namespace std;

void initGame();

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

typedef struct rect2f {double minx, maxx, miny, maxy, width, height;}rect2f;
rect2f rect2fMake(double minx, double maxx, double miny, double maxy) {
	rect2f retVal;
	retVal.minx = minx;retVal.maxx = maxx;retVal.miny = miny;retVal.maxy = maxy;
	retVal.width = fabs(maxx-minx);retVal.height = fabs(maxy-miny);
	return retVal;
}

typedef struct complexEquationResult {
	bool escaped;
	int escapeIteration;
	float escapeDistance;
}complexEquationResult;

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

class Fractal {
public:
	Fractal() {
		bounds = rect2fMake(-2, 1, -1, 1);
		iterations = 64;
		escapeValue = 3.0f;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		buffer = new ScreenBuffer(width, height);
	};
	virtual void calculate(){};
	virtual void render() {
		for(int y=0;y<height;y++) {
			for(int x=0;x<width;x++) {
				int index = y*width+x;
				screen->setBufferxy(x, y, buffer->buffer[index].Char.AsciiChar, buffer->buffer[index].Attributes);
			}
		}
	};
	virtual void zoom(float amount) {
		rect2f newbounds = bounds;
		newbounds.minx += amount*newbounds.width;
		newbounds.maxx -= amount*newbounds.width;
		newbounds.miny += amount*newbounds.height;
		newbounds.maxy -= amount*newbounds.height;
		bounds = rect2fMake(newbounds.minx, newbounds.maxx, newbounds.miny, newbounds.maxy);
	};
	virtual void pan(vector2f amount) {
		float xvalue=0, yvalue=0;
		if(amount.x)
			xvalue = bounds.width/amount.x;
		if(amount.y)
			yvalue = bounds.height/amount.y;
		rect2f newbounds = bounds;
		newbounds.maxx += xvalue;
		newbounds.minx += xvalue;
		newbounds.maxy += yvalue;
		newbounds.miny += yvalue;
		bounds = rect2fMake(newbounds.minx, newbounds.maxx, newbounds.miny, newbounds.maxy);
	};
	virtual complexEquationResult iterateLocation(complex<long double>loc){};
	int resolution() {return width*height;};
	rect2f bounds;
	int iterations;
	float escapeValue;
	int width, height;
	ScreenBuffer *buffer;
};

class Mandelbrot : public Fractal {
public:
	Mandelbrot(){
		power = 2;
	};
	void calculate();
	complexEquationResult iterateLocation(complex<long double>loc);
	complex<long double>Z, C;
	int power;
};

void Mandelbrot::calculate() {
	float xDiv = bounds.width/width;
	float yDiv = bounds.height/height;
	for(int y=0;y<height;y++) {
		for(int x=0;x<width;x++) {
			complexEquationResult test;
			test = iterateLocation(complex<long double>(x*xDiv+bounds.minx, y*yDiv+bounds.miny));
			if(test.escaped) {
				buffer->setBufferxy(x, y, 'X', test.escapeIteration);
			}
			else {
				buffer->setBufferxy(x, y, 219, BLACK);
			}
		}
	}
}

complexEquationResult Mandelbrot::iterateLocation(complex<long double>loc) {
	complexEquationResult retVal;
	retVal.escaped = false;
	Z = complex<long double>(0, 0);
	C = loc;
	for(int d=0;d<iterations;d++) {
		double distance;
		Z = pow(Z, power) + C;
		distance = abs(Z);
		if(distance > escapeValue) {
			retVal.escaped = true;
			retVal.escapeDistance = distance;
			retVal.escapeIteration = d;
			return retVal;
		}
	}
	return retVal;
}

HANDLE hIn, hOut;

unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;

Mandelbrot *mandel;

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	mandel->calculate();

	while(GameActive) {
		processInput();
		render();
		Sleep(45);
	}

	return 0;
}

void initGame() {
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	mandel = new Mandelbrot();
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	screen->clearBuffer();
	mandel->render();
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
	if(keyPressed('W')) {
		mandel->pan(vector2fMake(0, -8.0f));
		mandel->calculate();
	}
	if(keyPressed('S')) {
		mandel->pan(vector2fMake(0, 8.0f));
		mandel->calculate();
	}
	if(keyPressed('A')) {
		mandel->pan(vector2fMake(-8.0f, 0));
		mandel->calculate();
	}
	if(keyPressed('D')) {
		mandel->pan(vector2fMake(8.0f, 0));
		mandel->calculate();
	}
	if(keyPressed('Q')) {
		mandel->zoom(-0.05f);
		mandel->calculate();
	}
	if(keyPressed('E')) {
		mandel->zoom(0.05f);
		mandel->calculate();
	}
	if(keyPressed('P')) {
		mandel->iterations++;
		mandel->calculate();
	}
	if(keyPressed('O')) {
		mandel->iterations--;
		mandel->calculate();
	}
	if(keyPressed('K')) {
		mandel->power--;
		mandel->calculate();
	}
	if(keyPressed('L')) {
		mandel->power++;
		mandel->calculate();
	}
}
