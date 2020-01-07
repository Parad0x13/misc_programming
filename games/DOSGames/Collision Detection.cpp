/* Collision Detection Program... */
/* Uses tutorials from:
	http://www.codeproject.com/Articles/15573/2D-Polygon-Collision-Detection
	http://content.gpwiki.org/index.php/Polygon_Collision
	http://devmaster.net/forums/topic/12561-separating-axis-theorem/
*/
// V1.1 - Added initGame and some more things to collide with.
		// Realized that if you collide with objects from the bottom
		// collision will not work. Will have to work on AABBTime()
// V1.2 - Changed collisionDetection to collisionResolver
		// Changed AABB from rectangle to circular AABBs
		// Added createWall() funtion
		// Vastly improved the collision functions
// V1.3 - Minor changes
// V1.4 - Transitioned to DOSGame Template 1.4
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <malloc.h>
#include <math.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define GRAVITY -9.81f

#define HORIZONTAL_WALL	1010
#define VERTICAL_WALL	1011

using namespace std;

struct vect2f {
	vect2f(){};
	vect2f(float _x, float _y) {x = _x;y = _y;};
	double DotProduct(vect2f v) {return x*v.x + y*v.y;};
	vect2f Perpendicular() {return vect2f(-y, x);};
	void print() {printf("(%f, %f)", x, y);};
	//vect2f MajorAxis() {if(fabs(x) > fabs(y)) return vect2f(x, 0);else return vect2f(0, y);};
	vect2f MajorAxis() {
		int xSign = 1, ySign = 1;
		if(x < 0)
			xSign = -1;
		if(y < 0)
			ySign = -1;
		if(fabs(x) > fabs(y))
			return vect2f(1*xSign, 0);
		else return
			vect2f(0, 1*ySign);
	};
	vect2f operator - () {return vect2f(-x, -y);};
	vect2f operator + (vect2f v) {return vect2f(x+v.x, y+v.y);};
	vect2f operator + (double f) {return vect2f(x+f, y+f);};
	vect2f operator - (vect2f v) {return vect2f(x-v.x, y-v.y);};
	vect2f operator - (double f) {return vect2f(x-f, y-f);};
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
	vect2f distanceTo(AABB2f AABB) {
		AABB2f a = AABB2f(center, radius), b = AABB;
		// Shrink b to a point, increase A by B's radius
		a.radius = a.radius + b.radius;
		b.radius = vect2f(0, 0);
		vect2f distance = b.center-a.center;
		//vect2f normal = distance.MajorAxis();
		// Calculate the position of the normal by multipliying
		// by half of A's size and adding onto A by the major axis
		//normal = normal*(a.radius/2) + ((a.radius*normal)/2);
		return distance;
	};
	vect2f normalTo(AABB2f AABB) {
		vect2f distance = distanceTo(AABB);
		vect2f normal = distance.MajorAxis();
		return normal;
	};

	vect2f center;vect2f radius;
};

void initGame();
void gameLogic();

void createWall(vect2f start, int size, int type);

void initWinHandles();
void gotoxy(int x, int y);
void render();
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

typedef struct collisionReport {
	bool intersect;
	bool willIntersect;
}collisionReport;

class Actor {
public:
	Actor();
	virtual void render();
	virtual void applyPhysics();
	virtual void collisionResolver();
	virtual void move();
	clock_t tick, prevTick;
	float timeInterval;
	char icon;
	int color;
	bool affectedByGravity;
	AABB2f AABB, prevAABB;
	vect2f vel, accel;
	vect2f prevVel;
};

collisionReport testCollision(Actor *one, Actor *two) {
	collisionReport retVal;
	retVal.intersect = false;
	retVal.willIntersect = false;

	if(one->AABB.Intersects(two->AABB))
		retVal.intersect = true;
//	if(AABB2fIntersect(one->AABBTime(), two->AABB))
//		retVal.willIntersect = true;
	return retVal;
}

vector<Actor *>actors;
Actor *player;

Actor::Actor() {
	tick = clock();prevTick = tick;
	icon = '?';
	color = LIGHTMAGENTA;
	affectedByGravity = false;
	AABB = AABB2f(vect2f(3, 3), vect2f(1, 1));
	vel = vect2f(0, 0);
	accel = vect2f(0, 0);
}

void Actor::render() {
	screen->setBufferxy((int)AABB.center.x, (int)AABB.center.y, icon, color);
}

void Actor::applyPhysics() {
	prevVel = vel;prevTick = tick;
	tick = clock();
	timeInterval = (tick-prevTick)/1000.0f;
	// BEGIN CHEAT!!!
	timeInterval = 0.05f;
	// END CHEAT!!!
	vect2f newAccel = accel;
	if(affectedByGravity)
		newAccel = accel + vect2f(0, -GRAVITY);

	vel = vel + (newAccel*timeInterval);
	move();
}

void Actor::collisionResolver() {
	for(int d=0;d<actors.size();d++) {
		if(this != actors[d]) {
			collisionReport testCollide = testCollision(this, actors[d]);
			if(testCollide.willIntersect || testCollide.intersect) {
				vect2f distance = AABB.distanceTo(actors[d]->AABB);
				vect2f normal = AABB.normalTo(actors[d]->AABB);
				double relNv = (actors[d]->vel - vel).DotProduct(normal);
				double remove = relNv + distance.y/timeInterval;
//				vel = vect2f(0, 0);
				if(this == player) {
					gotoxy(20, 9);
					printf("normal:\t");
					normal.print();
					gotoxy(20, 10);
					printf("vel:\t");
					vel.print();
					gotoxy(20, 11);
					printf("loc:\t");
					AABB.center.print();
					gotoxy(20, 12);
					printf("their loc:\t");
					actors[d]->AABB.center.print();
					gotoxy(20, 13);
					printf("relNv:\t%f\n", relNv);
					gotoxy(20, 14);
					printf("remove:\t%f", remove);
					//if(fabs(relNv) > 0)
						getchar();
				}
				//normal.print();
				//getchar();
			}
		}
	}
}

void Actor::move() {
	vect2f displacement = ((vel+prevVel)/2.0f)*timeInterval;

	AABB.center = AABB.center+displacement;
}

HANDLE hIn, hOut;

unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	while(GameActive) {
		processInput();
		render();
		gameLogic();
		Sleep(45);
	}

	return 0;
}

void initGame() {
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);

	player = new Actor();
	player->icon = 0x01;player->color = YELLOW;player->AABB.center = vect2f(5, 3);player->affectedByGravity = true;
	actors.push_back(player);
	createWall(vect2f(5, 18), 1, HORIZONTAL_WALL);
}

void gameLogic() {
	for(int d=0;d<actors.size();d++)
		actors[d]->applyPhysics();
	for(int d=0;d<actors.size();d++)
		actors[d]->collisionResolver();
}

void createWall(vect2f start, int size, int type) {
	for(int d=0;d<size;d++) {
		Actor *wall = new Actor();
		wall->AABB.center = start;
		if(type == HORIZONTAL_WALL)
			wall->AABB.center.x += d;
		else if(type == VERTICAL_WALL)
			wall->AABB.center.y += d;
		wall->affectedByGravity = false;
		wall->icon = 219;
		wall->color = WHITE;
		actors.push_back(wall);
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
	player->render();	// So it always renders on top
	swapBuffers();
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

	if(keyPressed('W'))
		player->vel = player->vel + vect2f(0, -2);
	if(keyPressed('S'))
		player->vel = player->vel + vect2f(0, 2);
	if(keyPressed('A'))
		player->vel = player->vel + vect2f(-2, 0);
	if(keyPressed('D'))
		player->vel = player->vel + vect2f(2, 0);
	if(keyPressed('Q'))
		player->vel = vect2f(0, 0);
}
