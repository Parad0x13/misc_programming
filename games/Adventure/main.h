#ifndef __MAIN__
#define __MAIN__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <malloc.h>

#define EOL 0x0A

class Game;
class InputHandler;

int rnd(int range);

typedef char *string;

extern Game *game;
extern InputHandler *inputHandler;

typedef struct vertex {
	int x, y;
}vertex;
vertex *vertexWith(int _x, int _y);

#endif
