/*  -- Bryan Tremblay, Keikaku Studios --
	Basic template for Windows DOS games
	includes double buffering for graphics
	and multi-key support for KB
				V1.1					*/
/*	Change Log	*/
// 1.1 - Added some code to the render function
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <cstdlib>	// for rand()
#include <vector>

#include <conio.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define Actor_Default	100
#define Actor_Player	101
#define Actor_Enemy		102
#define Actor_BarEnemy	103
#define Actor_HBarEnemy	104
#define Actor_VBarEnemy	105

#define Icon_Player		0x01
#define Icon_Enemy		0x04
#define Icon_BarEnemy	219

#define UP		0
#define DOWN	1
#define LEFT	2
#define RIGHT	3

using namespace std;

void initLevelOne();
void deleteAllActors();

void initWinHandles();
void gotoxy(int x, int y);
void render();
void setBufferxy(int x, int y, char character, int color);
void swapBuffers();
void floodBuffer(char character, int color);
void clearBuffer();

void gameLogic();
void collisionDetection();

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

COORD COORDWith(int x, int y) {
	COORD retVal;
	retVal.X = x;
	retVal.Y = y;
	return retVal;
}

typedef enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
}COLORS;

class Actor {
public:
	Actor();
	virtual void setLoc(COORD newLoc);
	virtual void render();
	virtual void move(int direction);
	virtual void logic();
	virtual void tick();
	virtual bool collidesWith(Actor *actor);
	virtual void collidedWith(Actor *actor);

	int type;
	bool alive;
	clock_t time;
	char icon;
	int color;
	COORD loc, prevLoc;
};

Actor::Actor() {
	type = Actor_Default;
	alive = true;
	setLoc(COORDWith(0, 0));
	prevLoc = loc;
	icon = 'I';
	color = LIGHTMAGENTA;
	time = clock();
}

void Actor::setLoc(COORD newLoc) {
	loc = newLoc;
}

void Actor::render() {
	setBufferxy(loc.X, loc.Y, icon, color);
}

void Actor::move(int direction) {
	prevLoc = loc;
	if(direction == UP)
		loc.Y--;
	else if(direction == DOWN)
		loc.Y++;
	else if(direction == LEFT)
		loc.X--;
	else if(direction == RIGHT)
		loc.X++;
}

void Actor::logic() {
	//
}

void Actor::tick() {
	time = clock();
}

bool Actor::collidesWith(Actor *actor) {
	if(loc.X == actor->loc.X && loc.Y == actor->loc.Y)
		return true;
	return false;
}

void Actor::collidedWith(Actor *actor) {
	//
}

class Player : public Actor {
public:
	Player();
	void render();
	void move(int direction);
	void collidedWith(Actor *actor);
};

Player::Player() {
	type = Actor_Player;
	loc = COORDWith(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	icon = Icon_Player;
	color = LIGHTGREEN;
}

void Player::render() {
	Actor::render();
}

void Player::move(int direction) {
	Actor::move(direction);
}

void Player::collidedWith(Actor *actor) {
	if(actor->type == Actor_HBarEnemy)
		loc = prevLoc;
	if(actor->type == Actor_VBarEnemy)
		loc = prevLoc;
	if(actor->type == Actor_Enemy)
		alive = false;
}

class Enemy : public Actor {
public:
	Enemy();
};

Enemy::Enemy() {
	type = Actor_Enemy;
	icon = Icon_Enemy;
	setLoc(COORDWith(rnd(SCREEN_WIDTH), rnd(SCREEN_HEIGHT)));
	color = LIGHTRED;
}

class BarEnemy : public Enemy {
public:
	BarEnemy();
	void resetInitLoc();
	void logic();
	void changeDirection();
	void setInitValues(COORD _loc, int _size, int _range, float _speed, int _direction);

	COORD initLoc;
	int size, range;
	float speed;
	int direction;
};

BarEnemy::BarEnemy() {
	type = Actor_BarEnemy;
	resetInitLoc();
	size = 2;
	range = 3;
	speed = 0.05f;
	color = YELLOW;
	icon = Icon_BarEnemy;
}

void BarEnemy::resetInitLoc() {
	initLoc = loc;
}

void BarEnemy::logic() {
	Enemy::logic();
	if(clock()-time > speed*1000) {
		move(direction);
		int distance = sqrt((loc.X-initLoc.X)*(loc.X-initLoc.X)+(loc.Y-initLoc.Y)*(loc.Y-initLoc.Y));
		if(distance > range)
			changeDirection();
		tick();
	}
}

void BarEnemy::changeDirection() {
	if(direction == UP)
		direction = DOWN;
	else if(direction == DOWN)
		direction = UP;
	else if(direction == LEFT)
		direction = RIGHT;
	else if(direction == RIGHT)
		direction = LEFT;
}

void BarEnemy::setInitValues(COORD _loc, int _size, int _range, float _speed, int _direction) {
	loc = _loc;
	size = _size;
	range = _range;
	speed = _speed;
	direction = _direction;
	resetInitLoc();
}

class HBarEnemy : public BarEnemy {
public:
	HBarEnemy();
	void render();
	bool collidesWith(Actor *actor);
};

HBarEnemy::HBarEnemy() {
	type = Actor_HBarEnemy;
	direction = rnd(2);
	if(direction == 0)
		direction = LEFT;
	else if(direction == 1)
		direction = RIGHT;
}

void HBarEnemy::render() {
	setBufferxy(loc.X, loc.Y, icon, color);
	for(int d=0;d<size+1;d++) {
		setBufferxy(loc.X+d, loc.Y, icon, color);
		setBufferxy(loc.X-d, loc.Y, icon, color);
	}
}

bool HBarEnemy::collidesWith(Actor *actor) {
	if(loc.X == actor->loc.X && loc.Y == actor->loc.Y)
		return true;
	for(int d=0;d<size;d++) {
		if(loc.X == actor->loc.X+d && loc.Y == actor->loc.Y)
			return true;
		if(loc.X == actor->loc.X-d && loc.Y == actor->loc.Y)
			return true;
	}
	return false;
}

class VBarEnemy : public BarEnemy {
public:
	VBarEnemy();
	void render();
	bool collidesWith(Actor *actor);
};

VBarEnemy::VBarEnemy() {
	type = Actor_VBarEnemy;
	direction = rnd(2);
	if(direction == 0)
		direction = UP;
	else if(direction == 1)
		direction = DOWN;
}

void VBarEnemy::render() {
	setBufferxy(loc.X, loc.Y, icon, color);
	for(int d=0;d<size+1;d++) {
		setBufferxy(loc.X, loc.Y+d, icon, color);
		setBufferxy(loc.X, loc.Y-d, icon, color);
	}
}

bool VBarEnemy::collidesWith(Actor *actor) {
	if(loc.X == actor->loc.X && loc.Y == actor->loc.Y)
		return true;
	for(int d=0;d<size;d++) {
		if(loc.X == actor->loc.X && loc.Y+d == actor->loc.Y)
			return true;
		if(loc.X == actor->loc.X && loc.Y-d == actor->loc.Y)
			return true;
	}
	return false;
}

HANDLE hIn;
HANDLE hOut;

// Aparantly you can have multiple screen buffers. Learn to do this later
CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];  // In [y][x] format
unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;

Player *player;
vector<Actor *>actors;

int main(int argc, char *argv[]) {
//	Theres a major problem with collision detection.
//	if you run into a HBlockEnemy and press either
//	left or right you faze through it

	seed();
	initWinHandles();
	initKB();

	initLevelOne();

	while(GameActive) {
		processInput();
		gameLogic();
		collisionDetection();
		render();
		Sleep(45);
	}
	return 0;
}

void initLevelOne() {
	deleteAllActors();

	player = new Player();
	player->setLoc(COORDWith(2, SCREEN_HEIGHT/2));
	actors.push_back(player);

	//COORD _loc, int _size, int _range, float _speed, int _direction

	VBarEnemy *a = new VBarEnemy();a->setInitValues(COORDWith(10, 0-5), 10, 2, 0.05f, DOWN);actors.push_back(a);
	VBarEnemy *b = new VBarEnemy();b->setInitValues(COORDWith(10, SCREEN_HEIGHT+5), 10, 2, 0.05f, UP);actors.push_back(b);
	VBarEnemy *c = new VBarEnemy();c->setInitValues(COORDWith(20, 0-4), 10, 2, 0.05f, UP);actors.push_back(c);
	VBarEnemy *d = new VBarEnemy();d->setInitValues(COORDWith(20, SCREEN_HEIGHT+4), 10, 2, 0.05f, DOWN);actors.push_back(d);
	VBarEnemy *e = new VBarEnemy();e->setInitValues(COORDWith(30, 0-3), 10, 2, 0.05f, DOWN);actors.push_back(e);
	VBarEnemy *f = new VBarEnemy();f->setInitValues(COORDWith(30, SCREEN_HEIGHT+3), 10, 2, 0.05f, UP);actors.push_back(f);
	VBarEnemy *g = new VBarEnemy();g->setInitValues(COORDWith(40, 0-2), 10, 2, 0.05f, UP);actors.push_back(g);
	VBarEnemy *h = new VBarEnemy();h->setInitValues(COORDWith(40, SCREEN_HEIGHT+2), 10, 2, 0.05f, DOWN);actors.push_back(h);
	VBarEnemy *i = new VBarEnemy();i->setInitValues(COORDWith(15, SCREEN_HEIGHT/2), 5, 10, 0.05f, DOWN);actors.push_back(i);
	VBarEnemy *j = new VBarEnemy();j->setInitValues(COORDWith(25, SCREEN_HEIGHT/2), 5, 10, 0.05f, UP);actors.push_back(j);
	VBarEnemy *k = new VBarEnemy();k->setInitValues(COORDWith(35, SCREEN_HEIGHT/2), 5, 10, 0.05f, DOWN);actors.push_back(k);
	HBarEnemy *l = new HBarEnemy();l->setInitValues(COORDWith(60, SCREEN_HEIGHT/4), 15, 3, 0.05f, LEFT);actors.push_back(l);
	HBarEnemy *m = new HBarEnemy();m->setInitValues(COORDWith(60, SCREEN_HEIGHT-SCREEN_HEIGHT/4-1), 15, 3, 0.05f, LEFT);actors.push_back(m);
	VBarEnemy *n = new VBarEnemy();n->setInitValues(COORDWith(70, SCREEN_HEIGHT/2), 5, 0, 0.05f, DOWN);actors.push_back(n);
}

void deleteAllActors() {
	for(int d=0;d<actors.size();d++)
		delete actors[d];
	actors.clear();
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(hOut, pos);
}

void render() {
	clearBuffer();
	for(int d=0;d<actors.size();d++) {
		if(actors[d]->alive)
			actors[d]->render();
	}
	swapBuffers();
}

void setBufferxy(int x, int y, char character, int color) {
	if(x >= 0 && x <= SCREEN_WIDTH-1 && y >= 0 && y <= SCREEN_HEIGHT) {
		buffer[y][x].Char.AsciiChar = character;
		buffer[y][x].Attributes = color;
	}
}

void swapBuffers() {
	COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
	COORD dwBufferCoord = {0, 0};
	SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1};
	WriteConsoleOutput(hOut, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void floodBuffer(char character, int color) {
	for(int y=0;y<SCREEN_HEIGHT;y++) {
		for(int x=0;x<SCREEN_WIDTH;x++)
			setBufferxy(x, y, character, color);
	}
}

void clearBuffer() {
	CHAR_INFO blank = {' ', WHITE};
	for(int y=0;y<SCREEN_HEIGHT;y++) {
		for(int x=0;x<SCREEN_WIDTH;x++)
			buffer[y][x] = blank;
	}
}

void gameLogic() {
	for(int d=0;d<actors.size();d++)
		actors[d]->logic();
}

void collisionDetection() {
	for(int d=0;d<actors.size();d++) {
		Actor *me = actors[d];
		for(int dd=0;dd<actors.size();dd++) {
			Actor *you = actors[dd];
			if(me != you) {
				if(me->collidesWith(you)) {
					me->collidedWith(you);
					you->collidedWith(me);
				}
			}
		}
	}
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

	if(keyPressed('W'))
		player->move(UP);
	if(keyPressed('S'))
		player->move(DOWN);
	if(keyPressed('A'))
		player->move(LEFT);
	if(keyPressed('D'))
		player->move(RIGHT);
}
