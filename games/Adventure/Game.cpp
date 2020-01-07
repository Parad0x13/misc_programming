#include "Game.h"
#include "Actor.h"
#include "Area.h"

Game::Game() {
	DeactivateGame();
	setupNewGame();
}

void Game::ActivateGame() {
	state = Active_GameState;
}

void Game::DeactivateGame() {
	state = Inactive_GameState;
}

void Game::setupNewGame() {
	setupPlayer();
	setupWorld();
	playerLocation = vertexWith(0, 0);
	ActivateGame();
}

void Game::setupPlayer() {
	player = new Actor();
}

void Game::setupWorld() {
	world = new World();
	world->addArea(new Area(-1, playerLocation));
}
