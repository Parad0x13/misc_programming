#ifndef __GAME__
#define __GAME__

#include "kb.h"
#include "graphics.h"

#define AREA_SIZE SCREEN_WIDTH
#define MAX_GAME_SIZE (AREA_SIZE*2)
#define MAX_PAN (MAX_GAME_SIZE-AREA_SIZE)

void initGame();
void runGame();
void GameLogic();

void panLeft();
void panRight();

extern bool GameActive;
extern int panLocation;

#endif
