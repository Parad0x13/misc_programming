#include <iostream>
#include "vect.h"
#include "DOSScreen.h"
#include "DOSKeyboard.h"
#include "SimpleTimer.h"

#define FPS 30

using namespace std;

typedef enum {
	STILL,
	LEFT,
	RIGHT,
	UP,
	DOWN,
}DIRECTIONS;

DOSScreen *screen;
DOSKeyboard *keyboard;
bool gameActive, gameQuit = false;
int globalBackgroundColor = BLACK;

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

class Wall {
public:
	Wall(vect2f loc, int color) : loc(loc), color(color) {}
	void render() {screen->set(loc.x, loc.y, (char)219, color);}

	vect2f loc;
	int color;
};

vector<Wall *> walls;

class Actor {
public:
	Actor(char icon='?', int color=LIGHTMAGENTA, vect2f loc=vect2f(0, 0)) : icon(icon), color(color), loc(loc) {
		direction = STILL;
		lastMoveTick = clock();
		speed.y = 0.03f;
		speed.x = speed.y * 1.5f;
		alive = true;
	}
	void render() {screen->set(loc.x, loc.y, icon, color);}
	void tick() {
		bool shouldMove = false;
		clock_t now = clock();
		double delta = (now-lastMoveTick) / (float)CLOCKS_PER_SEC;
		if((direction == UP || direction == DOWN) && delta >= speed.x)shouldMove = true;
		if((direction == LEFT || direction == RIGHT) && delta >= speed.y)shouldMove = true;

		if(shouldMove) {
			walls.push_back(new Wall(loc, color));
			if(direction == UP)loc.y++;
			if(direction == DOWN)loc.y--;
			if(direction == LEFT)loc.x--;
			if(direction == RIGHT)loc.x++;
			lastMoveTick = now;
		}

		for(int d=0;d<walls.size();d++) {
			if(loc == walls[d]->loc)alive = false;
			if(loc.x < 0 || loc.x > screen->width)alive = false;
			if(loc.y < 0 || loc.y > screen->height)alive = false;
		}
	}

	bool alive;
	char icon;
	int color, direction;
	clock_t lastMoveTick;
	vect2f loc, speed;
};
vector<Actor *>actors;
Actor *player1, *player2;

void processKeyPresses(unsigned int *keys) {
	if(keys[VK_ESCAPE])gameQuit = true;
}

void processKeyDown(unsigned int key, bool repeat) {
	if(key == 'W')player1->direction = UP;
	if(key == 'S')player1->direction = DOWN;
	if(key == 'A')player1->direction = LEFT;
	if(key == 'D')player1->direction = RIGHT;

	if(key == VK_UP)player2->direction = UP;
	if(key == VK_DOWN)player2->direction = DOWN;
	if(key == VK_LEFT)player2->direction = LEFT;
	if(key == VK_RIGHT)player2->direction = RIGHT;
}

void processKeyUp(unsigned int key) {
	//
}

void render() {
	screen->clear();

	for(int d=0;d<walls.size();d++)walls[d]->render();
	for(int d=0;d<actors.size();d++)actors[d]->render();

	screen->swapBuffers();
}

void gameLogic() {
	player1->tick();
	player2->tick();

	if(!player1->alive || !player2->alive)Sleep(1000);
	if(!player1->alive)gameActive = false;
	if(!player2->alive)gameActive = false;
}

int main() {
	seed();
	screen = new DOSScreen(DOS_SCREEN_WIDTH, DOS_SCREEN_HEIGHT);
	screen->backgroundColor = globalBackgroundColor;
	screen->yAxisAlignment = AXIS_ALIGNMENT_NORMAL;
	keyboard = new DOSKeyboard(processKeyPresses);
	keyboard->bindKeyDown(processKeyDown);
	keyboard->bindKeyUp(processKeyUp);
	SimpleTimer *renderTimer = new SimpleTimer(render, 1.0f/(float)FPS, true);
	registerTimer(renderTimer, true, true);

	player1 = new Actor(0x01, LIGHTBLUE, vect2f(0, 0));
	actors.push_back(player1);

	player2 = new Actor(0x01, LIGHTRED, vect2f(0, 0));
	actors.push_back(player2);

	for(;;) {
		walls.clear();
		player1->loc = vect2f(0, screen->height/2);
		player2->loc = vect2f(screen->width-1, screen->height/2);
		player1->direction = RIGHT;
		player2->direction = LEFT;
		player1->alive = true;
		player2->alive = true;
		gameActive = true;
		while(gameActive && !gameQuit) {
			keyboard->processInput();
			manageTimers();
			gameLogic();
		}
		if(gameQuit)break;
	}
}
