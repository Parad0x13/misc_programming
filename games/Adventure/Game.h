#ifndef __GAME__
#define __GAME__

#include "main.h"

#define Inactive_GameState 0
#define Active_GameState 1

class Actor;
class Area;
class World;

class Game {
public:
	Game();
	~Game();

	int state;
	Actor *player;
	World *world;
	vertex *playerLocation;

	void ActivateGame();
	void DeactivateGame();
	void setupNewGame();
	void setupPlayer();
	void setupWorld();
};

#endif
