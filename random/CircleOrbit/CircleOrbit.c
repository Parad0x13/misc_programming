#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define no 0
#define yes 1
#define NO no
#define YES yes
#define bool int

#define rad2deg(radians) ((radians) * (180.0 / M_PI))
#define deg2rad(angle) ((angle) / 180.0 * M_PI)

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)

void init();
void iterate();
void clearScreen();
void setXY(int x, int y, char value, bool isCentered);
void render();

char *screen;
float outerSpeed = 10.0f;

typedef struct star {
	char icon;
	float deg, distance;
	int direction;
}star;

int starCount = 5;
star stars[5];

star starWith(char _icon, float _deg, float _distance, int _direction) {
	star retVal;
	retVal.icon = _icon;
	retVal.deg = _deg;
	retVal.distance = _distance;
	retVal.direction = _direction;
	return retVal;
}

void renderStar(star _star) {
	setXY((12.0f/8.0f)*(_star.distance*cos(deg2rad(_star.deg))), _star.distance*sin(deg2rad(_star.deg)), _star.icon, yes);
}

int main() {
	init();
	while(!kbhit()) {
		render();
		iterate();
		usleep(100000);
	}
}

void init() {
	screen = calloc(SCREEN_RESOLUTION+1, sizeof(char));
	stars[0] = starWith('.', 90, SCREEN_HEIGHT*0.5f, 1);
	stars[1] = starWith('?', 270, SCREEN_HEIGHT*0.4f, -1);
	stars[2] = starWith('+', 180, SCREEN_HEIGHT*0.3f, 1);
	stars[3] = starWith('<', 0, SCREEN_HEIGHT*0.2f, 1);
	stars[4] = starWith('@', 45, SCREEN_HEIGHT*0.1f, -1);
}

void iterate() {
	int d;
	for(d=0;d<starCount;d++)
		stars[d].deg += outerSpeed/stars[d].distance * stars[d].direction;
}

void clearScreen() {
	memset(screen, ' ', SCREEN_RESOLUTION);
}

void setXY(int x, int y, char value, bool isCentered) {
	int xOffset = 0, yOffset = 0;
	if(isCentered) {
		xOffset = SCREEN_WIDTH/2;
		yOffset = SCREEN_HEIGHT/2;
	}
	int index = (y+yOffset)*SCREEN_WIDTH+(x+xOffset);
	screen[index] = value;
}

void render() {
	clearScreen();

	setXY(0, 0, 219, yes);
	int d;
	for(d=0;d<starCount;d++)renderStar(stars[d]);

	fputs(screen, stdout);
}
