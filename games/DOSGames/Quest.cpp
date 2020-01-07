// Quest 9.0
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>

#define RenderInterval (1.0f/20.0f)

#define bX 80
#define bY 24
#define boardResolution (bX*bY)
#define halfX (bX/2)
#define halfY (bY/2)

#define ESC 0x1B
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4B
#define RIGHT 0x4D

#define Item_Player		0x02
#define Item_Air		' '
#define Item_VertWall		'|'
#define Item_HorizWall		'-'
#define Item_EdgeOfWorld	177

void initHandlers();
void GameLoop();

void clearScreen();
void printScreen();

void render();
void renderPlayer();
void renderWorld();
void renderHUD();

void clearBoard();
void printWorld();
void printHUD();
void processInput(int input);
void playerInput(int input);

void setupBoard();
void setupWorld();

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

typedef struct {
	int x, y;
}vertex;

vertex vertexWith(int theX, int theY) {
	vertex retVal;
	retVal.x=theX;
	retVal.y=theY;
	return retVal;
}

vertex vertexWithVertex(vertex theVertex) {
	vertex retVal;
	retVal.x=theVertex.x;
	retVal.y=theVertex.y;
	return retVal;
}

typedef struct {
	char *space;
	int x, y;
	int dimention;
}area;

area areaWith(int x, int y) {
	int d;
	area retVal;
	retVal.x = x;
	retVal.y = y;
	retVal.space = (char *)malloc(retVal.x*retVal.y*sizeof(char));
	retVal.dimention = retVal.x*retVal.y;

	for(d=0;d<retVal.dimention;d++)
		retVal.space[d] = Item_Air;

	for(d=0;d<retVal.x;d++) {
		retVal.space[d] = Item_EdgeOfWorld;
		retVal.space[(retVal.y-1)*retVal.x+d] = Item_EdgeOfWorld;
	}
	for(d=0;d<retVal.y;d++) {
		retVal.space[d*retVal.x] = Item_EdgeOfWorld;
		retVal.space[d*retVal.x+(retVal.x-1)] = Item_EdgeOfWorld;
	}

	return retVal;
}

CHAR_INFO ScreenBuffer[bX][bY];

int ScreenBufferSize = boardResolution*sizeof(char)*2;
int HUDWidth = bX, HUDHeight = 10;
int GameRunning;

char *board, *HUD;
area *currentWorld;
area outside;

vertex PlayerPos;
clock_t renderTimer;
int pauseLength = 10;

HANDLE hIn;
HANDLE hOut;

void sleep(unsigned int mseconds) {
	clock_t goal = mseconds + clock();
	while (goal > clock());
}

int main() {
	initHandlers();
	setupWorld();
	setupBoard();

	clearScreen();
	GameLoop();
	return 0;
}

void initHandlers() {
    hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void GameLoop() {
	if(!renderTimer)
		renderTimer = clock();
	GameRunning = 1;

	while(GameRunning) {
		if(kbhit())
			processInput(getch());

		render();
		sleep(pauseLength);
	}

	clearScreen();
}

void clearScreen() {
	char buffer[boardResolution];
	for(int i=0;i<boardResolution;i++)
	    buffer[i] = ' ';

	DWORD nWritten;
	WriteConsole(hOut, buffer, boardResolution, &nWritten, NULL);
}

void printScreen() {
    DWORD nWritten;
	WriteConsole(hOut, ScreenBuffer, boardResolution, &nWritten, NULL);
}

void render() {
	clearBoard();

//	renderWorld();
	renderPlayer();
//	renderHUD();

	printScreen();
}

void renderPlayer() {
	int XOffset, YOffset;
	XOffset = 0;
	YOffset = 0;

	ScreenBuffer[PlayerPos.x][PlayerPos.y].Char.AsciiChar = 'T';
	ScreenBuffer[PlayerPos.x][PlayerPos.y].Attributes = BLUE;
}

void renderWorld() {
	int x, y;
	int XOffset, YOffset;

	XOffset = PlayerPos.x-halfX;
	YOffset = PlayerPos.y-halfY;

	for(y=0;y<bY;y++) {
		for(x=0;x<bX;x++) {
			int worldY = y+YOffset, worldX = x+XOffset;
			if(worldY >= 0 && worldY < currentWorld[0].y && worldX >= 0 && worldX < currentWorld[0].x) {
				board[y*bX+x] = currentWorld[0].space[(y+YOffset)*currentWorld[0].x+(x+XOffset)];
			}
		}
	}
	printWorld();
}

void renderHUD() {
	int d;
	int health = 100;
	char *target = HUD;

	for(d=0;d<HUDWidth*HUDHeight*2;d+=2) {
		*target = 0x03;
		*(target+1) = GREEN;
		++target;
		++target;
	}
	printHUD();
}

void clearBoard() {
	int d;
	for(d=0;d<boardResolution;d++)
		board[d] = ' ';
}

void printWorld() {
	for(int y=0;y<bY;y++) {
		for(int x=0;x<bX;x++) {
			ScreenBuffer[x][y].Char.AsciiChar = board[y*bX+x];
			ScreenBuffer[x][y].Attributes = LIGHTGRAY;
		}
	}
}

void printHUD() {
	for(int y=0;y<bY;y++) {
		for(int x=0;x<bX;x++) {
			ScreenBuffer[x][y].Char.AsciiChar = HUD[y*bX+x];
			ScreenBuffer[x][y].Attributes = LIGHTGRAY;
		}
	}
}

void processInput(int value) {
	if(value == ESC)
		GameRunning = 0;

	playerInput(value);
}

void playerInput(int value) {
	char objectInteract;
	vertex newPosition = vertexWithVertex(PlayerPos);

	if(value == 'w') {
		newPosition.y--;
		objectInteract = currentWorld[0].space[newPosition.y*currentWorld[0].x+newPosition.x];
		if(objectInteract == Item_Air) {
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Air;
			PlayerPos = vertexWithVertex(newPosition);
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Player;
		}
	}
	if(value == 's') {
		newPosition.y++;
		objectInteract = currentWorld[0].space[(PlayerPos.y+1)*currentWorld[0].x+PlayerPos.x];
		if(objectInteract == Item_Air) {
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Air;
			PlayerPos = vertexWithVertex(newPosition);
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Player;
		}
	}
	if(value == 'a') {
		newPosition.x--;
		objectInteract = currentWorld[0].space[PlayerPos.y*currentWorld[0].x+(PlayerPos.x-1)];
		if(objectInteract == Item_Air) {
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Air;
			PlayerPos = vertexWithVertex(newPosition);
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Player;
		}
	}
	if(value == 'd') {
		newPosition.x++;
		objectInteract = currentWorld[0].space[PlayerPos.y*currentWorld[0].x+(PlayerPos.x+1)];
		if(objectInteract == Item_Air) {
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Air;
			PlayerPos = vertexWithVertex(newPosition);
//			currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Player;
		}
	}
}

void setupBoard() {
	board = (char *)malloc(boardResolution*sizeof(char));
	HUD = (char *)malloc(HUDWidth*sizeof(char)*2);
}

void setupWorld() {
	int d;
	outside = areaWith(30, 15);

	currentWorld = &outside;

	PlayerPos = vertexWith(5, 5);
	//currentWorld[0].space[PlayerPos.y*currentWorld[0].x+PlayerPos.x] = Item_Player;
}
