#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "object.h"
#include "constants.h"
#include "kb.h"
#include "graphics.h"
#include "render.h"
#include "gameLogic.h"

void initWinHandles();

HANDLE hIn;
HANDLE hOut;
bool GameActive;

Area *world;
Object *player;
vertexi cameraFocus;

int main() {
	initWinHandles();
	initKB();

	Object *test;

	world = new Area();
	player = Player_ObjectAt(vertexfMake(0, -5));
	world->AddObject(player);
	world->AddObject(Stone_ObjectAt(vertexfMake(-7, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(-6, 4)));
	world->AddObject(Stone_ObjectAt(vertexfMake(-5, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(-4, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(-3, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(-2, 3)));
	test = Stone_ObjectAt(vertexfMake(-1, 3));
	test->extra = "left";
	world->AddObject(test);
//	world->AddObject(Stone_ObjectAt(vertexfMake(-1, 3)));
	test = Stone_ObjectAt(vertexfMake(0, 3));
	test->extra = "center";
	world->AddObject(test);
//	world->AddObject(Stone_ObjectAt(vertexfMake(0, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(1, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(2, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(3, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(4, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(5, 3)));
	world->AddObject(Stone_ObjectAt(vertexfMake(5, 2)));
	cameraFocus = vertexiMake(0, 0);

	GameActive = true;

	while(GameActive) {
		processInput();
		GameLogic();
		render();
		Sleep(45);
	}

	return false;
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}
