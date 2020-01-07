#include "main.h"
#include "Game.h"
#include "Actor.h"
#include "Item.h"
#include "Area.h"
#include "InputHandler.h"

Game *game;
InputHandler *inputHandler;

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

vertex *vertexWith(int _x, int _y) {
	vertex *retVal = (vertex *)malloc(sizeof(vertex));
	retVal->x = _x;
	retVal->y = _y;
	return retVal;
}

int main() {
//	seed();

//	game = new Game;
	inputHandler = new InputHandler;

//	while(game->state == Active_GameState) {
//		inputHandler->awaitInput();
//		inputHandler->parseInput();
//		inputHandler->processInput();
//	}
	for(;;) {
		inputHandler->awaitInput();
	}

//	char stuff[50];
//	fgets(stuff, 20, stdin);
//	printf("is: %s\n", stuff);

	getch();

	return 0;
}
