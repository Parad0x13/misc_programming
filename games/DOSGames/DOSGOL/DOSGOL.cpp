// Not a very good Game Of Life simulator...
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DOSX 80
#define DOSY 24

#define DEAD 0
#define ALIVE 1

void initGrid();
void drawGrid();
void play();

static int grid[DOSX*DOSY], grid_temp[DOSX*DOSY];

int rnd(int range) {return(rand()%range);}
void seed() {srand((unsigned)time(NULL));}

int main() {
	seed();
	initGrid();

	int run = 0;
	for(;;) {
		for(int d=0;d<1;d++) {		// Has to be an odd number, even will miss the two-period oscillators
			play();
			run++;
		}
		drawGrid();
		Sleep(10);
		if(run>=250) {
			initGrid();
			run = 0;
		}
	}
	return 0;
}

void initGrid() {
	for(int d=0;d<DOSX*DOSY;d++)grid[d] = rnd(2)?DEAD:ALIVE;
}

void play() {
	int friends;
	for(int y=0;y<DOSY;y++) {
		for(int x=0;x<DOSX;x++) {
			friends = 0;
			int pos = y*DOSX+x;
			friends = grid[pos-DOSX-1]+grid[pos-DOSX]+grid[pos-DOSX+1]+grid[pos-1]+grid[pos+1]+grid[pos+DOSX-1]+grid[pos+DOSX]+grid[pos+DOSX+1];
			// Birth
			if(grid[pos] == DEAD && friends == 3)
				grid_temp[pos] = ALIVE;
			// Death
			if(grid[pos] == ALIVE && friends<2)
				grid_temp[pos] = DEAD;
			if(grid[pos] == ALIVE && friends>3)
				grid_temp[pos] = DEAD;
			// Survival
			if(grid[pos] == ALIVE && friends == 2)
				grid_temp[pos] = ALIVE;
			if(grid[pos] == ALIVE && friends == 3)
				grid_temp[pos] = ALIVE;
		}
	}
	memcpy(grid, grid_temp, sizeof(grid));
}

void drawGrid() {
	COORD coord = {0, 0};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	for(int y=0;y<DOSY;y++) {
		for(int x=0;x<DOSX;x++) {
			if(grid[y*DOSX+x] == DEAD)
				printf(" ");
			if(grid[y*DOSX+x] == ALIVE)
				printf("X");
		}
	}
}
