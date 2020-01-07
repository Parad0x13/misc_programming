#include <iostream>
#include "vect.h"
#include "DOSScreen.h"
#include "DOSKeyboard.h"
#include "SimpleTimer.h"

#define FPS 30

using namespace std;

DOSScreen *screen;
DOSKeyboard *keyboard;
bool gameActive;
int globalBackgroundColor = BLACK;

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

class Actor {
public:
	Actor(char icon='?', int color=LIGHTMAGENTA, vect2f loc=vect2f(0, 0)) : icon(icon), color(color), loc(loc) {
		size = vect2f(1, 1);
	}
	void render() {
		for(int y = 0;y < size.y;y++) {
			for(int x = 0;x < size.x;x++)
				screen->set(loc.x + x, loc.y + y, icon, color);
		}
	}
	bool collidesWith(Actor *actor) {
		float ax1 = loc.x, ax2 = loc.x + size.x;
		float ay1 = loc.y, ay2 = loc.y + size.y;
		float bx1 = actor->loc.x, bx2 = actor->loc.x + actor->size.x;
		float by1 = actor->loc.y, by2 = actor->loc.y + actor->size.y;
		if(ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)return true;
		return false;
	}

	char icon;
	int color;
	vect2f loc, size;
};
vector<Actor *>actors;
Actor* player;

void processKeyPresses(unsigned int* keys) {
	if(keys[VK_ESCAPE])gameActive = false;
}

void processKeyDown(unsigned int key, bool repeat) {
	if(key == 'W')player->loc.y++;
	if(key == 'S')player->loc.y--;
	if(key == 'A')player->loc.x--;
	if(key == 'D')player->loc.x++;
}

void processKeyUp(unsigned int key) {
	//
}

void render() {
	screen->clear();

	for(int d = 0;d < actors.size();d++)
		actors[d]->render();

	screen->swapBuffers();
}

void gameLogic() {
	//
}

int main() {
	seed();
	screen = new DOSScreen(DOS_SCREEN_WIDTH, DOS_SCREEN_HEIGHT);
	screen->backgroundColor = globalBackgroundColor;
	screen->yAxisAlignment = AXIS_ALIGNMENT_NORMAL;
	keyboard = new DOSKeyboard(processKeyPresses);
	keyboard->bindKeyDown(processKeyDown);
	keyboard->bindKeyUp(processKeyUp);
	SimpleTimer* renderTimer = new SimpleTimer(render, 1.0f / (float)FPS, true);
	registerTimer(renderTimer, true, true);

	player = new Actor(0x01, LIGHTGREEN, vect2f(screen->width / 2, screen->height / 2));
	actors.push_back(player);

	gameActive = true;
	while(gameActive) {
		keyboard->processInput();
		manageTimers();
		gameLogic();
	}
}
