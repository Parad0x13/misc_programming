#ifndef __RENDER__
#define __RENDER__

void render();
void renderPlayer(Player *player);
void renderEnemies();
void renderBullets();

void setBufferxy(int x, int y, char character, int color);
void swapBuffers();
void floodBuffer(char character, int color);
void clearBuffer();

CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];  // In [y][x] format

void render() {
	clearBuffer();
	renderBullets();
	renderPlayer(mainPlayer);
	renderEnemies();
	swapBuffers();

	gotoxy(0, 0);
	printf("Score: %i", GameScore);
}

void renderPlayer(Player *player) {
	int color;
	if(player->type == FRIENDLY) {
		color = player->color;
	}
	else if(player->type == ENEMY) {
		if(player->health == 1)
			color = YELLOW;
		else if(player->health == 2)
			color = LIGHTRED;
	}
	setBufferxy((int)player->loc.x, (int)player->loc.y, player->icon, color);
}

void renderEnemies() {
	for(int d=0;d<MAX_ENEMIES;d++) {
		if(enemies[d]) {
			if(enemies[d]->alive) {
				renderPlayer(enemies[d]);
			}
		}
	}
}

void renderBullets() {
	for(int d=0;d<MAX_BULLETS;d++) {
		if(bullets[d]) {
			if(bullets[d]->alive)
				setBufferxy((int)bullets[d]->loc.x, (int)bullets[d]->loc.y, bullets[d]->icon, bullets[d]->color);
		}
	}
}

void setBufferxy(int x, int y, char character, int color) {
	buffer[y][x].Char.AsciiChar = character;
	buffer[y][x].Attributes = color;
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

vertex randomLocationOnEdge() {
	vertex retVal;
	int x, y, edge;

	edge = rnd(4);
	if(edge == 0) {
		x = 0;
		y = rnd(SCREEN_HEIGHT-1);
	}
	else if(edge == 1) {
		x = rnd(SCREEN_WIDTH-1);
		y = 0;
	}
	else if(edge == 2) {
		x = SCREEN_WIDTH-1;
		y = rnd(SCREEN_HEIGHT-1);
	}
	else if(edge == 3) {
		x = rnd(SCREEN_WIDTH-1);
		y = SCREEN_HEIGHT-1;
	}
	retVal = vertexWith(x, y);

	return retVal;
}

#endif
