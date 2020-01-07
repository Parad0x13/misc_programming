#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define ENEMY_MAX_COUNT 20
#define ENEMY_CHANCE 5

#define STATUS_GAMEACTIVE 100
#define STATUS_JUSTLOST 101
#define gameSpeed 20000

void render();
void handleInput();
void clearScreen();
void tick();
void tickWallCreation();
void tickEnemies();
void moveWalls();
int newWallHeightFromPrevious(int previousHeight);
int hasCollided();
void setXY(int x, int y, char value);
int indexOfFirstAvailableEnemy();
int gameStatus;
long score = 0;

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

typedef struct {
	float x, y, alive;
	char icon;
}Entity;

typedef struct {
	int x, top, bottom;
}Wall;

void renderWall(Wall *_wall) {
	int d;
	char icon = '#';
	if(_wall->x == 79)icon = 'E';
	for(d=0;d<_wall->top;d++)setXY(_wall->x, d, icon);
	for(d=0;d<_wall->bottom;d++)setXY(_wall->x, SCREEN_HEIGHT-d-1, icon);
}

char *screen;
int size = (SCREEN_WIDTH*SCREEN_HEIGHT+1)*sizeof(char);
Entity player;
Entity enemies[ENEMY_MAX_COUNT];
Wall walls[80];
int d;
int currentTop = 3, currentBottom = 4;

int main() {
	seed();
	screen = malloc(size);

	player.alive = 1;
	player.x = 5;
	player.y = 12;
	player.icon = 0x01;

	for(d=0;d<SCREEN_WIDTH;d++) {
		walls[d].x = d;
		walls[d].top = currentTop;
		walls[d].bottom = currentBottom;
		tickWallCreation();
	}

	for(d=0;d<ENEMY_MAX_COUNT;d++) {
		enemies[d].alive = 0;
		enemies[d].icon = '<';
	}

	gameStatus = STATUS_GAMEACTIVE;
	while(gameStatus == STATUS_GAMEACTIVE) {
		render();
		tick();
		if(kbhit())handleInput();
		usleep(gameSpeed);
		score++;
	}
	printf("Died with a score of: %i", score);
}

void render() {
	clearScreen();

	if(player.alive)setXY(player.x, player.y, player.icon);
	for(d=0;d<80;d++)renderWall(&walls[d]);
	for(d=0;d<ENEMY_MAX_COUNT;d++) {
		if(enemies[d].alive)
			setXY(enemies[d].x, enemies[d].y, enemies[d].icon);
	}

	fputs(screen, stdout);
}

void handleInput() {
	char value = getch();
	if(value == 'w')player.y--;
	if(value == 's')player.y++;
}

int farthestIndex = SCREEN_WIDTH*SCREEN_HEIGHT-1;
void setXY(int x, int y, char value) {
	int index = y*SCREEN_WIDTH+x;
	if(index < farthestIndex)
		screen[index] = value;
}

int indexOfFirstAvailableEnemy() {
	int dd;
	for(dd=0;dd<ENEMY_MAX_COUNT;dd++) {
		if(!enemies[dd].alive)
			return dd;
	}
	return -1;
}

void clearScreen() {
	memset(screen, ' ', size);
	screen[size-1] = 0x00;
}

void tick() {
	if(hasCollided())
		gameStatus = STATUS_JUSTLOST;
	moveWalls();

	tickEnemies();
}

void tickWallCreation() {
	int variability = 0;
	int deviate = (rnd(10)==1)?1:0;
	if(deviate)variability = rnd(3)-1;
	currentTop += variability;

	variability = 0;
	deviate = (rnd(10)==1)?1:0;
	if(deviate)variability = rnd(3)-1;
	currentBottom += variability;

	if(currentTop+currentBottom >= SCREEN_HEIGHT-2) {
		currentTop--;
		currentBottom--;
	}
	if(currentTop<2)currentTop = 2;
	if(currentBottom<2)currentBottom = 2;
}

void tickEnemies() {
	if(rnd(ENEMY_CHANCE)==1) {
		int index = indexOfFirstAvailableEnemy();
		if(index != -1) {
			enemies[index].alive = 1;
			enemies[index].x = 79;
			enemies[index].y = rnd(24);
		}
	}
	for(d=0;d<ENEMY_MAX_COUNT;d++) {
		if(enemies[d].alive) {
			enemies[d].x--;
			if(enemies[d].x < 0)enemies[d].alive = 0;
		}
	}
}

void moveWalls() {
	for(d=0;d<80;d++) {
		walls[d].x--;
		if(walls[d].x == -1) {
			walls[d].x = 79;
			tickWallCreation();
			walls[d].top = currentTop;
			walls[d].bottom = currentBottom;
		}
	}
}

int hasCollided() {
	for(d=0;d<80;d++) {
		if(player.x == walls[d].x) {
			if(player.y < walls[d].top)return 1;
			if(player.y > SCREEN_HEIGHT-walls[d].bottom-1)return 1;
		}
	}
	for(d=0;d<ENEMY_MAX_COUNT;d++) {
		if(player.x == enemies[d].x && player.y == enemies[d].y)
			return 1;
	}
	return 0;
}
