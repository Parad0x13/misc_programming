#include "graphics.h"

ScreenBuffer *screen;

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

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

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	screen->clearBuffer();
	renderActors();
	swapBuffers();
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
