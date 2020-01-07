/*  -- Bryan Tremblay, Keikaku Studios --
	Template for Windows DOS games
	Has multi-key support, render cycle,
	a random number generator, double buffering,
	and game actor support with fill rendering
	using Axis Aligned Bounding Boxes
	Everything to start a powerful DOS game
			-- Change Log --
1.1 -	Added some code to the render function
1.2 -	Added COORDwith() function
			Improved setBufferxy
			Added rnd() and seed() and added it to main()
			Added COORDOnScreen()
1.3 - 	Collapsed some code to reduce code length
			Added code for default Actor objects
1.4 -	Added a ScreenBuffer class and implemented it
1.5 -	Greatly improved Vectors
			Added AABBs for Actors
1.6 -	Added more Vector operand overloads
1.7 -	Improved AABB2f struct, uses %g instead of %f for printf now
1.8 -	Attempted to get Actors to render their full AABBs
			Added GameLogic(), and Actor logic functions
			Changed some accessor methods for AABB2f's and added size()
1.9 -	Added physics code, physics still need much work
			Added minClamp/maxClamp/absMinClamp functions to vect2f
			Improved MajorAxis function for vect2f
2.0 -	Added limits.h for MAX_INT
			Improved collision detection, there is still much to be done
			Set up default physics scene
			Improved rendering for AABBs, method is not yet perfect though
2.1 -	Worked on AABB rendering some more, minor tweaks
2.2 -	Added more collision code
		Fixed vect2f abs() code
2.3 -	Working on revamping collision code
2.4 -	Working on collision code
2.5 -	Added some more collision code
2.6 -	Cleaned code up some
*/

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

#define GRAVITY -9.81

using namespace std;

void initGame();
void GameLogic();
void resolveCollisions();

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

struct vect2f {
	vect2f(){}
	vect2f(float _x, float _y) {x = _x;y = _y;}
	double DotProduct(vect2f v) {return x*v.x + y*v.y;}
	vect2f Perpendicular() {return vect2f(-y, x);}
	void print() {printf("(%g, %g)", x, y);}
	void minClamp(vect2f v) {x = (x<v.x)?x:v.x;y = (y<v.y)?y:v.y;}
	void maxClamp(vect2f v) {x = !(x<v.x)?x:v.x;y = !(y<v.y)?y:v.y;}
	// absMinClamp returns a minimized clamp to zero in either direction.
	// E.g. (-10, 11) of (2, 2) will return (-2, 2)
	void absMinClamp(vect2f v) {vect2f _v = v.abs();minClamp(_v);maxClamp(-_v);}
	//void absMaxClamp(vect2f v){};	// Don't know mathematically how to do this
	vect2f MajorAxis() {return (fabs(x)>fabs(y))?vect2f(x/fabs(x), 0):vect2f(0, y/fabs(y));}
	vect2f abs() {return vect2f(fabs(x), fabs(y));}
	vect2f roundUpDown() {
		float xCheck = floor(x)+0.5f, yCheck = floor(y)+0.5f;
		float newX, newY;
		if(x<xCheck) newX = xCheck-0.5f;
		else newX = xCheck+0.5f;
		if(y<yCheck) newY = yCheck-0.5f;
		else newY = yCheck+0.5f;
		return vect2f(newX, newY);
	}
	vect2f operator - () {return vect2f(-x, -y);}
	vect2f operator + (vect2f v) {return vect2f(x+v.x, y+v.y);}
	vect2f operator + (double f) {return vect2f(x+f, y+f);}
	vect2f operator += (vect2f v) {x += v.x;y += v.y;}
	vect2f operator += (double f) {x += f;y += f;}
	vect2f operator ++ () {++x;++y;}		// Prefix
	vect2f operator ++ (int) {x++;y++;}		// Postfix
	vect2f operator - (vect2f v) {return vect2f(x-v.x, y-v.y);}
	vect2f operator - (double f) {return vect2f(x-f, y-f);}
	vect2f operator -= (vect2f v) {x -= v.x;y -= v.y;}
	vect2f operator -= (double f) {x -= f;y -= f;}
	vect2f operator -- () {--x;--y;}		// Prefix
	vect2f operator -- (int) {x--;y--;}		// Postfix
	vect2f operator * (vect2f v) {return vect2f(x*v.x, y*v.y);}
	vect2f operator * (double f) {return vect2f(x*f, y*f);}
	vect2f operator / (vect2f v) {return vect2f(x/v.x, y/v.y);}
	vect2f operator / (double f) {return vect2f(x/f, y/f);}
	float x, y;
};

struct AABB2f {
	AABB2f(){}
	AABB2f(vect2f _center, vect2f _size) {m_center = _center;m_radius = _size/2;}
	bool Intersects(AABB2f v) {
		if(fabs(center().x-v.center().x) > (radius().x + v.radius().x)) return false;
		if(fabs(center().y-v.center().y) > (radius().y + v.radius().y)) return false;
		return true;
	}
	void Translate(vect2f translation) {setCenter(center()+translation);}
	AABB2f withTranslation(vect2f translation) {return AABB2f(center()+translation, radius());}
	void print() {printf("Center:(%g, %g)Size:(%g x %g)", center().x, center().y, size().x, size().y);}
	AABB2f operator + (AABB2f a) {return AABB2f(center()+a.center(), size()+a.size());}

	vect2f center() {return m_center;}
	void setCenter(vect2f center) {m_center = center;}
	vect2f radius() {return m_radius;}
	void setRadius(vect2f radius) {m_radius = radius;}
	vect2f size() {return vect2f(radius().x*2, radius().y*2);}
	void setSize(vect2f size) {setRadius(vect2f(size.x/2, size.y/2));}

private:
	vect2f m_center;vect2f m_radius;
};

struct Physics {
	Physics() {
		tickTime = clock();prevTickTime = tickTime;
		affectedByGravity = false;collidable = false;
		vel = vect2f(0, 0);
		maxVel = vect2f(INT_MAX, INT_MAX);
	}
	void tick() {
		prevTickTime = tickTime;tickTime = clock();
		timeDelta = (tickTime-prevTickTime)/1000.0f;
		if(affectedByGravity) {
			vel += vect2f(0, -GRAVITY*timeDelta);
		}
		vel.minClamp(maxVel);
	}
	void stopMovement() {vel = vect2f(0, 0);}
	vect2f translationDelta() {return vel*timeDelta;}
	clock_t tickTime, prevTickTime;
	bool affectedByGravity, collidable;
	float timeDelta;
	vect2f vel, maxVel;
};

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

class ScreenBuffer {
public:
	ScreenBuffer(){}
	ScreenBuffer(int _width, int _height) {
		width = _width;height = _height;
		buffer = (CHAR_INFO *)malloc(resolution()*sizeof(CHAR_INFO));
		clearBuffer();
	}
	~ScreenBuffer() {free(buffer);}
	int resolution(){return width*height;}
	SMALL_RECT region() {SMALL_RECT retVal = {0, 0, width-1, height-1};return retVal;}
	void setBufferxy(int x, int y, char character, int color) {
		if(COORDOnScreen(COORDWith(x, y))) {
			int index = y*width+x;
			setBufferIndex(index, character, color);
		}
	}
	void setBufferIndex(int index, char character, int color) {
		if(index < resolution()) {
			buffer[index].Char.AsciiChar = character;
			buffer[index].Attributes = color;
		}
	}
	void floodBuffer(char character, int color) {for(int d=0;d<resolution();d++)setBufferIndex(d, character, color);}
	void clearBuffer() {floodBuffer(' ', WHITE);}
	int height, width;
	CHAR_INFO *buffer;
}*screen;

class Actor {
public:
	Actor() {
		AABB = AABB2f(vect2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), vect2f(3, 3));
		icon = '?';
		color = LIGHTMAGENTA;
	}
	virtual void render() {
		vect2f newCent = AABB.center().roundUpDown();
		vect2f newRad = AABB.radius().roundUpDown();
		vect2f newSize = AABB.size().roundUpDown();
		vect2f loc = newCent-newRad;
		for(int y=0;y<newSize.y;y++) {
			for(int x=0;x<newSize.x;x++)screen->setBufferxy(loc.x+x, loc.y+y, icon, color);
		}
	}
	virtual void logic(){};
	virtual void applyPhysics() {
		physics.tick();
		move();
	}
	virtual void move() {AABB.Translate(physics.translationDelta());}
	virtual void revertMove() {AABB.Translate(-physics.translationDelta());}
	AABB2f AABB;
	AABB2f AABBTime() {
		AABB2f retVal = AABB;
		vect2f delta = physics.translationDelta();
		retVal.setCenter(retVal.center() + delta/2.0f);
		retVal.setRadius(retVal.radius() + delta.abs()/2.0f);
		return retVal;
	}
	Physics physics;
	char icon;
	int color;
};

class Collision {
public:
	Collision() {};
	Collision(Actor *one, Actor *two) {
		actorA = one;actorB = two;
		sameActor = false;intersect = false;willIntersect = false;collisionCanOccur = true;
		if(actorA == actorB) {sameActor = true;collisionCanOccur = false;};
		if(!actorA->physics.collidable || !actorB->physics.collidable) collisionCanOccur = false;
		if(collisionCanOccur) {
			if(actorA->AABB.Intersects(actorB->AABB))
				intersect = true;
			if(one->AABBTime().Intersects(two->AABBTime()))
				willIntersect = true;
			relVel = actorB->physics.vel - actorA->physics.vel;
		}
	};
	~Collision() {};

	bool sameActor, intersect, willIntersect, collisionCanOccur;
	vect2f relVel;
private:
	Actor *actorA, *actorB;
};

vector<Actor *>actors;

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
		GameLogic();
		Sleep(45);
	}

	return false;
}

void initGame() {
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);

	// START Default Actor Setup
	Actor *one = new Actor(), *two = new Actor(), *three = new Actor, *four = new Actor();
	one->icon = 'X';one->AABB.setSize(vect2f(SCREEN_WIDTH-2, 1));
	one->AABB.setCenter(vect2f(SCREEN_WIDTH/2, 20));
	one->physics.collidable = true;
	two->icon = 'X';two->AABB.setSize(vect2f(3, 3));
	two->AABB.setCenter(vect2f(SCREEN_WIDTH/2, 10));
	two->physics.vel = vect2f(0, -10);two->color = LIGHTGREEN;
	two->physics.affectedByGravity = true;two->physics.collidable = true;
	three->icon = 'X';three->AABB.setSize(vect2f(5, 3));
	three->AABB.setCenter(vect2f(5, 15));
	three->physics.vel = vect2f(18, -18);
	three->color = LIGHTBLUE;three->physics.affectedByGravity = true;
	three->physics.collidable = true;
	four->icon = 'X';four->AABB.setSize(vect2f(7, 3));
	four->AABB.setCenter(vect2f(60, 10));
	four->physics.vel = vect2f(-12, -4);four->color = YELLOW;
	four->physics.affectedByGravity = true;four->physics.collidable = true;
	actors.push_back(one);actors.push_back(two);actors.push_back(three);actors.push_back(four);
	// END Default Actor Setup
}

void GameLogic() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->logic();
	}
	for(int d=0;d<actors.size();d++) {
		actors[d]->applyPhysics();
	}
	resolveCollisions();
}

void resolveCollisions() {
	for(int a=0;a<actors.size();a++) {
		for(int b=0;b<actors.size();b++) {
			Actor *me = actors[a];Actor *you = actors[b];
			Collision *collision = new Collision(me, you);
			if(collision->collisionCanOccur) {
				if(collision->intersect) {
					vect2f delta = me->physics.translationDelta();
					me->physics.vel -= delta;
					me->AABB.Translate(-delta);
				}
				if(collision->willIntersect) {
					me->physics.vel = vect2f(0, 0);
				}
			}
		}
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
}
