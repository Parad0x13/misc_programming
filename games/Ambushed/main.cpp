#define WIN32_LEAN_AND_MEAN

#include "main.h"
#include "Bullet.h"
#include "Player.h"
#include "Render.h"

unsigned int Keys[MAX_VK_SIZE];

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();

	nextAvailableBulletIndex();

	mainPlayer = new Player(FRIENDLY);

	int enemySpawnRate = 1000;
	clock_t lastSpawn = clock();
	clock_t start = clock();

	while(GameActive) {
		if(clock() - lastSpawn >= enemySpawnRate) {
			newEnemy(randomLocationOnEdge(), zeroVelocity(), 2);
			lastSpawn = clock();
		}

		processInput();
		render();

		// Handle Player
		if(mainPlayer->alive) {
			mainPlayer->runLogic();
			mainPlayer->collisionDetection();
		}
		if(!mainPlayer->alive)	// Required, in case died on collision detection
			GameActive = false;

		// Handle Bullets
		for(int d=0;d<MAX_BULLETS;d++) {
			if(bullets[d]) {
				if(bullets[d]->alive) {
					bullets[d]->runLogic();
					bullets[d]->collisionDetection();
				}
				if(!bullets[d]->alive) // Required, in case died on collision detection
					removeBulletAtIndex(d);
			}
		}

		// Handle Enemies
		for(int d=0;d<MAX_ENEMIES;d++) {
			if(enemies[d]) {
				if(enemies[d]->alive) {
					enemies[d]->runLogic();
					enemies[d]->collisionDetection();
				}
				if(!enemies[d]->alive)	// Required, in case died on collision detection
					removeEnemyAtIndex(d);
			}
		}

		// Add a leveling system here sometime
		if((start-clock())%100 == 0) {	// Player power up, game gets more difficult
			mainPlayer->ShotsPerSec++;
			if(enemySpawnRate > 300)
			enemySpawnRate -= 10;
		}

		Sleep(45);
	}

	gotoxy(0, 0);
	printf("You Died! With a score of: %i", GameScore);
	getch();

	return EXIT_SUCCESS;
}

COORD pos(int x, int y) {
	COORD retVal;
	retVal.X = x;
	retVal.Y = y;
	return retVal;
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(hOut, pos);
}

void initKB() {
	for(int d=0;d<MAX_VK_SIZE;d++)
		Keys[d] = 0x0000;
}

void addKBHold(unsigned int key) {
	if(!keyPressed(key))
		Keys[key] = true;
}

void removeKBHold(unsigned int key) {
	if(keyPressed(key))
		Keys[key] = false;
}

int keyPressed(unsigned int key) {
	int index = (int)key;
	if(Keys[index])
		return true;
	return false;
}

void processInput() {
	INPUT_RECORD InRec;
	DWORD EventCount, NumRead;

	GetNumberOfConsoleInputEvents(hIn, &EventCount);
	while(EventCount > 0) {
		ReadConsoleInput(hIn, &InRec, 1, &NumRead);
		processInput_Record(InRec);
		GetNumberOfConsoleInputEvents(hIn, &EventCount);
	}
	processKeyPresses();
}

void processInput_Record(INPUT_RECORD InRec) {
	if(InRec.EventType == KEY_EVENT) {
		unsigned int key = InRec.Event.KeyEvent.wVirtualKeyCode;
		if(InRec.Event.KeyEvent.bKeyDown)
			addKBHold(key);
		else
			removeKBHold(key);
	}
}

// Virtual Keys for letters are the same as their ASCII counterparts
// Therefore they are not defined in windows.h
// Must use their Capped version too, lower 'a' won't work etc...
void processKeyPresses() {
	if(keyPressed(VK_ESCAPE))
		GameActive = false;

	if(keyPressed('A'))
	    mainPlayer->vel.x -= 0.2f;
	if(keyPressed('D'))
	    mainPlayer->vel.x += 0.2f;
	if(keyPressed('W'))
	    mainPlayer->vel.y -= 0.2f;
	if(keyPressed('S'))
	    mainPlayer->vel.y += 0.2f;

	if(mainPlayer->canShoot()) {
		if(keyPressed(VK_UP) && !keyPressed(VK_DOWN) && !keyPressed(VK_LEFT) && !keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(90));
		else if(!keyPressed(VK_UP) && keyPressed(VK_DOWN) && !keyPressed(VK_LEFT) && !keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(270));
		else if(!keyPressed(VK_UP) && !keyPressed(VK_DOWN) && keyPressed(VK_LEFT) && !keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(180));
		else if(!keyPressed(VK_UP) && !keyPressed(VK_DOWN) && !keyPressed(VK_LEFT) && keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(0));
		else if(keyPressed(VK_UP) && !keyPressed(VK_DOWN) && keyPressed(VK_LEFT) && !keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(135));
		else if(keyPressed(VK_UP) && !keyPressed(VK_DOWN) && !keyPressed(VK_LEFT) && keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(45));
		else if(!keyPressed(VK_UP) && keyPressed(VK_DOWN) && keyPressed(VK_LEFT) && !keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(225));
		else if(!keyPressed(VK_UP) && keyPressed(VK_DOWN) && !keyPressed(VK_LEFT) && keyPressed(VK_RIGHT))
			mainPlayer->shoot(DEGREES_TO_RADIANS(315));
	}

	// Suicide Button
	if(keyPressed('K'))
		mainPlayer->alive = false;
}
