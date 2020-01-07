#include "game.h"

bool GameActive = true;
int panLocation = 0;

void runGame() {
	initGame();
	while(GameActive) {
		processInput();
		render();
		GameLogic();
		Sleep(45);
	}
}

void initGame() {
	actors.push_back(new Ant());
	actors.push_back(new AntPile());
}

void GameLogic() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->logic();
	}
}

void panLeft() {
	if(panLocation > 0)
		panLocation--;
}

void panRight() {
	if(panLocation < MAX_PAN)
		panLocation++;
}
