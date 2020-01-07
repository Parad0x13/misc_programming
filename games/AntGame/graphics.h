#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <windows.h>
#include "main.h"
#include "actor.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

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

COORD COORDWith(int x, int y);

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
};

extern ScreenBuffer *screen;

void gotoxy(int x, int y);
void render();
void renderActors();
void swapBuffers();
bool COORDOnScreen(COORD loc);

#endif
