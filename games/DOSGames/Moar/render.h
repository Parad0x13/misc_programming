#ifndef __RENDER__
#define __RENDER__

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

// Aparantly you can have multiple screen buffers. Learn to do this later
CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];  // In [y][x] format

void gotoxy(int x, int y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(hOut, pos);
}

void setBufferxy(int x, int y, char character, int color) {
	buffer[y][x].Char.AsciiChar = character;
	buffer[y][x].Attributes = color;
}

void renderChunk(Chunk *chunk) {
	// Items
	for(int x=0;x<CHUNK_WIDTH;x++) {
		for(int y=0;y<CHUNK_HEIGHT;y++) {
			Item *item = chunk->items[y][x];
			if(item->alive) {
				setBufferxy(x, y, item->icon, WHITE);
			}
		}
	}
	// Actors
	for(int d=0;d<MAX_ACTORS;d++) {
		Actor *actor = chunk->actors[d];
		if(actor->alive) {
			setBufferxy((int)actor->loc.x, (int)actor->loc.y, actor->icon, WHITE);
		}
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

void render() {
	clearBuffer();

	for(int d=0;d<activeChunkCount;d++)
		renderChunk(activeChunks[d]);

	swapBuffers();
}

#endif
