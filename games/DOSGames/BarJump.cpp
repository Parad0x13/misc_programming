/* Bryan Tremblay, Keikaku Studios */
// BarJump, to compete with Lee's bar jumping game
//		I cheated SO BAD in developing this code
//		This is certainly NOT production code
// V1.0
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <malloc.h>
#include <math.h>
#include <conio.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define HORIZONTAL	101
#define VERTICAL	102

#define gameMode 1
#define crazyMode 1

using namespace std;

void initGame();

void initWinHandles();
void gotoxy(int x, int y);
void render();
void gameLogic();
void renderActors();
void swapBuffers();
bool COORDOnScreen(COORD loc);

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

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

struct vect2f {
	vect2f(){};
	vect2f(float _x, float _y) {x = _x;y = _y;};
	double DotProduct(vect2f v) {return x*v.x + y*v.y;};
	vect2f Perpendicular() {return vect2f(-y, x);};
	void print() {printf("(%g, %g)", x, y);};
	vect2f MajorAxis() {if(fabs(x) > fabs(y)) return vect2f(x/fabs(x), 0);else return vect2f(0, y/fabs(y));};
	vect2f operator - () {return vect2f(-x, -y);};
	vect2f operator + (vect2f v) {return vect2f(x+v.x, y+v.y);};
	vect2f operator + (double f) {return vect2f(x+f, y+f);};
	vect2f operator += (vect2f v) {x += v.x;y += v.y;};
	vect2f operator += (double f) {x += f;y += f;};
	vect2f operator ++ () {++x;++y;};		// Prefix
	vect2f operator ++ (int) {x++;y++;};	// Postfix
	vect2f operator - (vect2f v) {return vect2f(x-v.x, y-v.y);};
	vect2f operator - (double f) {return vect2f(x-f, y-f);};
	vect2f operator -= (vect2f v) {x -= v.x;y -= v.y;};
	vect2f operator -= (double f) {x -= f;y -= f;};
	vect2f operator -- () {--x;--y;};		// Prefix
	vect2f operator -- (int) {x--;y--;};	// Postfix
	vect2f operator * (vect2f v) {return vect2f(x*v.x, y*v.y);};
	vect2f operator * (double f) {return vect2f(x*f, y*f);};
	vect2f operator / (vect2f v) {return vect2f(x/v.x, y/v.y);};
	vect2f operator / (double f) {return vect2f(x/f, y/f);};
	float x, y;
};

struct AABB2f {
	AABB2f(){};
	AABB2f(vect2f _center, vect2f _size) {center = _center;radius = _size/2;};
	bool Intersects(AABB2f v) {
		if(fabs(center.x-v.center.x) > (radius.x + v.radius.x)) return false;
		if(fabs(center.y-v.center.y) > (radius.y + v.radius.y)) return false;
		return true;
	};
	void Translate(vect2f translation) {center = center + translation;};
	AABB2f withTranslation(vect2f translation) {return AABB2f(center+translation, radius);};
	void print() {printf("Center:(%g, %g)Size:(%g x %g)", center.x, center.y, radius.x*2, radius.y*2);};
	AABB2f operator + (AABB2f a) {return AABB2f(center+a.center, (radius*2)+(a.radius*2));};
	vect2f center;vect2f radius;
};

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

class ScreenBuffer {
public:
	ScreenBuffer(){};
	ScreenBuffer(int _width, int _height);
	~ScreenBuffer() {free(buffer);};
	int resolution(){return width*height;};
	SMALL_RECT region() {SMALL_RECT retVal = {0, 0, width-1, height-1};return retVal;};
	void setBufferxy(int x, int y, char character, int color);
	void setBufferIndex(int index, char character, int color);
	void floodBuffer(char character, int color);
	void clearBuffer() {floodBuffer(' ', WHITE);};
	int height, width;
	CHAR_INFO *buffer;
}*screen;

ScreenBuffer::ScreenBuffer(int _width, int _height) {
	width = _width;height = _height;
	buffer = (CHAR_INFO *)malloc(resolution()*sizeof(CHAR_INFO));
	clearBuffer();
}

void ScreenBuffer::setBufferxy(int x, int y, char character, int color) {
	if(COORDOnScreen(COORDWith(x, y))) {
		int index = y*width+x;
		setBufferIndex(index, character, color);
	}
}

void ScreenBuffer::setBufferIndex(int index, char character, int color) {
	if(index < resolution()) {
		buffer[index].Char.AsciiChar = character;
		buffer[index].Attributes = color;
	}
}

void ScreenBuffer::floodBuffer(char character, int color) {
	for(int d=0;d<resolution();d++)
		setBufferIndex(d, character, color);
}

class Actor {
public:
	Actor();
	virtual void render();
	virtual void logic(){};
	AABB2f AABB;
	char icon;
	int color;
};

Actor::Actor() {
	AABB = AABB2f(vect2f(3, 2), vect2f(1, 1));
	icon = '?';
	color = LIGHTMAGENTA;
}

void Actor::render() {
	if(crazyMode)
		color = clock()+rnd(100);
	screen->setBufferxy((int)AABB.center.x, (int)AABB.center.y, icon, color);
}

vector<Actor *>actors;

class Wall : public Actor {
public:
	Wall(){};
	Wall(vect2f loc, int _size, int _type, float _speed, int _color) {
		color = _color;
		icon = 'X';
		size = _size;
		type = _type;
		origin = loc;
		canMove = true;
		speed = _speed;
		if(type == HORIZONTAL)		// Origin is far left
			AABB = AABB2f(vect2f(origin.x+size/2, origin.y), vect2f(size*2, 0.75));
		else if(type == VERTICAL)	// Origin is bottom
			AABB = AABB2f(vect2f(origin.x, origin.y-size/2), vect2f(0.75, size-.5));
	};
	void logic();
	void render();
	int size, type;
	vect2f origin;
	bool canMove;
	float speed;
};

Wall *floorWall;

void Wall::render() {
	if(crazyMode)
		color = clock()+rnd(100);
	if(type == HORIZONTAL) {
		for(int d=0;d<size;d++) {
			screen->setBufferxy((int)origin.x+d, (int)origin.y, icon, color);
		}
	}
	else if(type == VERTICAL) {
		for(int d=0;d<size;d++) {
			screen->setBufferxy((int)origin.x, (int)origin.y-d, icon, color);
		}
	}
}

void Wall::logic() {
	if(canMove) {
		origin -= vect2f(speed, 0);
		AABB.center.x = origin.x;
	}
	int destroySize;
	if(gameMode == 0)
		destroySize = -size;
	else if(gameMode == 1)
		destroySize = 0;
	if(origin.x < destroySize) {
		if(gameMode == 0)
			actors.push_back(new Wall(vect2f(SCREEN_WIDTH, 14-rnd(4)-2), 10+rnd(2), HORIZONTAL, rnd(400)/1000.0f+.6, rnd(6)+9));
		if(gameMode == 1)
			actors.push_back(new Wall(vect2f(SCREEN_WIDTH, 14-rnd(2)), 4+rnd(2), VERTICAL, rnd(400)/1000.0f+.6, rnd(6)+9));
		int index = 0;
		for(int d=0;d<actors.size();d++) {
			if(actors[d] == this) {
				index = d;
				break;
			}
		}
		actors.erase(actors.begin() + index);
	}
}

bool GameActive = true;
int score = 0;

class Player : public Actor {
public:
	Player(){
		icon = 0x01;
		color = LIGHTGREEN;
		canJump = true;
		stuck = true;
	};
	void logic() {
		if(!stuck) {
			AABB2f prevAABB = AABB;
			AABB.Translate(vect2f(0, 0.75));
			if(gameMode == 0) {
				for(int d=0;d<actors.size();d++) {
					if(actors[d] != this) {
						if(AABB.Intersects(actors[d]->AABB)) {
							if(actors[d] != floorWall) {
								AABB = prevAABB;
								canJump = true;
							}
							else {
								GameActive = false;
							}
						}
					}
				}
			}
			if(gameMode == 1) {
				for(int d=0;d<actors.size();d++) {
					if(actors[d] != this) {
						if(AABB.Intersects(actors[d]->AABB)) {
							if(actors[d] == floorWall) {
								AABB = prevAABB;
								canJump = true;
							}
							else {
								GameActive = false;
							}
						}
					}
				}
			}
		}
	};
	bool canJump, tryingToJump, stuck;
}*player;

HANDLE hIn, hOut;

unsigned int Keys[MAX_VK_SIZE];

clock_t startClock;

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	while(GameActive) {
		processInput();
		render();
		gameLogic();
		Sleep(20);
	}

	score = (clock()-startClock)/10;
	if(player->stuck)score = 0;
	gotoxy(10, 10);
	printf("You lost with a score of: %i\n", score);
	getchar();

	return 0;
}

void initGame() {
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);

	player = new Player();

	actors.push_back(player);
	floorWall = new Wall(vect2f(0, 15), SCREEN_WIDTH, HORIZONTAL, 0, LIGHTMAGENTA);
	floorWall->canMove = false;
	actors.push_back(floorWall);

	int amount;
	if(gameMode == 0)
		amount = 7;
	else if(gameMode == 1)
		amount = 5;
	for(int d=0;d<amount;d++) {
		if(gameMode == 0)
			actors.push_back(new Wall(vect2f(rnd(SCREEN_WIDTH), 14-rnd(4)-2), 8+rnd(2), HORIZONTAL, rnd(400)/1000.0f+.6, rnd(6)+9));
		if(gameMode == 1)
			actors.push_back(new Wall(vect2f(rnd(SCREEN_WIDTH), 14-rnd(4)), 4+rnd(2), VERTICAL, rnd(400)/1000.0f+.6, rnd(6)+9));
	}
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	screen->clearBuffer();
	renderActors();
	player->render();
	swapBuffers();
}

void gameLogic() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->logic();

		if(player->AABB.center.x == actors[d]->AABB.center.x && actors[d] != player && actors[d] != floorWall)
			score++;
	}
}

void renderActors() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->render();
	}
}

void swapBuffers() {
	COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
	COORD dwBufferCoord = {0, 0};
	SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1};
	WriteConsoleOutput(hOut, (CHAR_INFO *)screen->buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

bool COORDOnScreen(COORD loc) {
	if(loc.X >= 0 && loc.X <= SCREEN_WIDTH-1 && loc.Y >= 0 && loc.Y <= SCREEN_HEIGHT)
		return true;
	return false;
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
	if(keyPressed('W')) {
		if(!player->stuck) {
			if(player->canJump || player->tryingToJump) {
				player->tryingToJump = true;
				player->canJump = false;
				if(player->AABB.center.y > 3)
					player->AABB.Translate(vect2f(0, -2));
				else {
					player->canJump = false;
					player->tryingToJump = false;
				}
			}
		}
	}
	else {
		player->tryingToJump = false;
	}
	if(keyPressed('D')) {
		if(!player->stuck) {
			player->AABB.Translate(vect2f(0.75, 0));
		}
	}
	if(keyPressed('A')) {
		if(!player->stuck) {
			player->AABB.Translate(vect2f(-0.75, 0));
		}
	}
	if(keyPressed('S')) {
		startClock = clock();
		player->stuck = false;
	}
}
