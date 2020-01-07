// You can only move Up, Center, and Down but you must dodge oncoming enemies
// Score increases over time but also by button mashing to go up and down. It's harder than it sounds!
#include <iostream>
#include "vect.h"
#include "DOSScreen.h"
#include "DOSKeyboard.h"
#include "SimpleTimer.h"

#define FPS 60
#define VERSION "1.0"

using namespace std;

DOSScreen *screen;
DOSKeyboard *keyboard;
bool gameActive;
unsigned long long score = 0;
unsigned int moveScore = 0;

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

typedef enum {
	LEFT,
	RIGHT,
}DIRECTIONS;

typedef enum {
	DOWN =		0,
	CENTER =	1,
	UP =		2,
}POSITIONS;

typedef enum {
	DEFAULT,
	PLAYER,
	ENEMY,
}TAGS;

typedef enum {
	VERYEASY,
	EASY,
	MEDIUM,
	TRYING,
	DIFFICULT,
	CHALLENGE,
	CRAZY,
	DIFFICULTY_COUNT,
}DIFFICULTIES;
int gameDifficulty = VERYEASY;

int globalEnemyColor, globalBackgroundColor;
double globalEnemySpeed = 0.025f;

class Actor {
public:
	Actor(char icon='?', int frontColor=LIGHTMAGENTA, vect2f loc=vect2f(0, 0)) : icon(icon), frontColor(frontColor), loc(loc) {
		size = vect2f(1, 1);
		type = DEFAULT;
		shouldDie = false;
	}
	void render() {
		for(int y=0;y<size.y;y++) {
			for(int x=0;x<size.x;x++)
				screen->set(loc.x + x, loc.y + y, icon, frontColor, globalBackgroundColor);
		}
	}
	virtual void tick(double delta) {
		//
	}
	bool collidesWith(Actor *actor) {
		float ax1 = loc.x, ax2 = loc.x + size.x;
		float ay1 = loc.y, ay2 = loc.y + size.y;
		float bx1 = actor->loc.x, bx2 = actor->loc.x + actor->size.x;
		float by1 = actor->loc.y, by2 = actor->loc.y + actor->size.y;
		if(ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)return true;
		return false;
	}

	bool shouldDie;
	char icon;
	int type;
	int frontColor, backColor;
	vect2f loc, size;
};
vector<Actor *>actors;

class Player : public Actor {
public:
	Player() {
		type = PLAYER;
		icon = 0x01;
		frontColor = LIGHTGREEN;
		loc = vect2f(screen->width/2, CENTER);
	}
	virtual void tick(double delta) {
		for(int d=0;d<actors.size();d++) {
			Actor *actor = actors[d];
			if(actor->type == ENEMY) {
				if(this->collidesWith(actor))
					gameActive = false;
			}
		}
	}
}*player;

class Enemy : public Actor {
public:
	Enemy() {
		type = ENEMY;
		frontColor = LIGHTRED;
		direction = rnd(2);
		if(direction == LEFT){loc = vect2f(screen->width-1, rnd(3));icon = '<';}
		if(direction == RIGHT){loc = vect2f(0, rnd(3));icon = '>';}
		lastMoveDelta = 0;
		speed = globalEnemySpeed;
	}
	virtual void tick(double delta) {
		Actor::tick(delta);
		lastMoveDelta += delta;
		if(lastMoveDelta >= speed) {
			move();
			lastMoveDelta = 0;
		}
		frontColor = globalEnemyColor;
	}
	void move() {
		if(direction == LEFT) {
			loc.x--;
			if(loc.x < 0)shouldDie = true;
		}
		if(direction == RIGHT) {
			loc.x++;
			if(loc.x > screen->width)shouldDie = true;
		}
	}
	int direction;
	double lastMoveDelta, speed;
};

void generateEnemy() {
	actors.push_back(new Enemy());
}

void processKeyPresses(unsigned int *keys) {
	if(keys[VK_ESCAPE])gameActive = false;

	bool u = false, d = false;
	u = keys['W']?true:u;
	d = keys['S']?true:d;
	u = keys[VK_UP]?true:u;
	d = keys[VK_DOWN]?true:d;
	if(u && d)player->loc.y = CENTER;
	else if(u && !d)player->loc.y = UP;
	else if(!u && d)player->loc.y = DOWN;
	else if(!u && !d)player->loc.y = CENTER;
}

void processKeyDown(unsigned int key, bool repeat) {
	moveScore++;
}

void processKeyUp(unsigned int key) {
	moveScore++;
}

void render() {
	screen->clear(219, globalBackgroundColor);

	for(int d=0;d<actors.size();d++)
		actors[d]->render();

	screen->swapBuffers();
}

void manageDifficulty() {
	static long long counter = 0;
	if(gameDifficulty < DIFFICULTY_COUNT) {
		if(counter == 500000) {
			gameDifficulty++;
			globalEnemySpeed *= 0.9f;
			counter = 0;
		}
		counter++;
	}
	if(gameDifficulty == VERYEASY){globalEnemyColor = YELLOW;globalBackgroundColor = BLACK;}
	else if(gameDifficulty == EASY){globalEnemyColor = BLACK;globalBackgroundColor = BROWN;}
	else if(gameDifficulty == MEDIUM){globalEnemyColor = LIGHTGRAY;globalBackgroundColor = DARKGRAY;}
	else if(gameDifficulty == TRYING){globalEnemyColor = BLUE;globalBackgroundColor = GREEN;}
	else if(gameDifficulty == DIFFICULT){globalEnemyColor = LIGHTMAGENTA;globalBackgroundColor = BLACK;}
	else if(gameDifficulty == CHALLENGE){globalEnemyColor = RED;globalBackgroundColor = CYAN;}
	else if(gameDifficulty == CRAZY){globalEnemyColor = LIGHTRED;globalBackgroundColor = WHITE;}
}

void manageActors() {
	bool shouldContinueErasing = true;
	while(shouldContinueErasing) {
		shouldContinueErasing = false;
		for(int d=0;d<actors.size();d++) {
			if(actors[d]->shouldDie) {
				shouldContinueErasing = true;
				actors.erase(actors.begin() + d);
				break;
			}
		}
	}
}

clock_t lastTick = clock();
void gameLogic() {
	clock_t now = clock();
	double delta = (now-lastTick) / (float)CLOCKS_PER_SEC;
	for(int d=0;d<actors.size();d++)
		actors[d]->tick(delta);
	lastTick = now;

	manageDifficulty();
	manageActors();
}

bool shouldPlayGame;
void processInput(int argc, char **argv) {
	shouldPlayGame = true;
	if(argc > 1) {
		string command = argv[1];
		if(command == "-help") {
			cout << endl;
			cout << " *Bryan Tremblay 2013*" << endl;
			cout << "-- [3Dodge " << VERSION << " Help] --" << endl;
			cout << "Point of game:" << endl;
			cout << "\tAvoid the enemies coming at you from both sides" << endl;
			cout << endl;
			cout << "Controls:" << endl;
			cout << "\tMove Up:\tW or Up" << endl;
			cout << "\tMove Down:\tS or Down" << endl;
			cout << endl;
			cout << "Score is increased the longer you survive" << endl;
			cout << "Score also increases as you move up or down. It's harder than it seems!" << endl;
			shouldPlayGame = false;
		}
		else {
			cout << endl;
			cout << "Try using the command -help" << endl;
			shouldPlayGame = false;
		}
	}
}

int main(int argc, char **argv) {
	processInput(argc, argv);
	if(shouldPlayGame) {
		seed();
		screen = new DOSScreen(DOS_SCREEN_WIDTH, 3);
		screen->yAxisAlignment = AXIS_ALIGNMENT_NORMAL;
		keyboard = new DOSKeyboard(processKeyPresses);
		keyboard->bindKeyDown(processKeyDown);
		keyboard->bindKeyUp(processKeyUp);

		registerTimer(new SimpleTimer(render, 1.0f/(float)FPS, true), true, true);
		registerTimer(new SimpleTimer(generateEnemy, 0.4f, true), true, true);

		player = new Player();
		actors.push_back(player);

		gameActive = true;
		score = 0;
		while(gameActive) {
			keyboard->processInput();
			manageTimers();
			gameLogic();
			score++;
		}
		int finalScore = score/10000 + moveScore*5;
		cout << "Died with a score of: " << finalScore << endl;
	}
}
