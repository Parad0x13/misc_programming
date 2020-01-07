// Trying to run cellular automata to mimic hydrodynamics
// http://www.buickj.myweb.port.ac.uk/Publications/PDF/Thesis/node30.html

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

//#define BoardWidth 14
//#define BoardHeight 10
#define BoardWidth 80
#define BoardHeight 25

using namespace std;

void initGame();
void GameLogic();
void initWinHandles();
void gotoxy(int x, int y);
void render();
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

COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}

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

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
}DIRECTIONS;

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
	buffer = (CHAR_INFO *)malloc(_width*_height*sizeof(CHAR_INFO));
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

HANDLE hIn, hOut;

unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;

class Particle {
public:
	Particle(){};
	Particle(int _x, int _y, int _direction);
	~Particle(){};
	void determineIcon() {
		if(direction == LEFT)icon = 27;
		else if(direction == RIGHT)icon = 26;
		else if(direction == UP)icon = 24;
		else if(direction == DOWN)icon = 25;
	};

	int x, y, direction, newDirection;
	bool collisionResolved;
	char icon;
};

Particle::Particle(int _x, int _y, int _direction) {
	collisionResolved = false;
	x = _x;
	y = _y;
	direction = _direction;
	newDirection = _direction;
	icon = 219;
}

class ParticleBuffer {
public:
	ParticleBuffer(){};
	~ParticleBuffer(){};
	void addParticle(Particle *particle);

	void iterate() {
		iterateMove();
		resolveCollisions();
	}
	void iterateMove();
	void resolveCollisions();

	vector<Particle *>particles;
}*particleBuffer;

void ParticleBuffer::addParticle(Particle *particle) {
	particles.push_back(particle);
}

void ParticleBuffer::iterateMove() {
	for(int d=0;d<particles.size();d++) {
		Particle *particle = particles[d];
		if(particle->direction == UP)
			particle->y--;
		else if(particle->direction == DOWN)
			particle->y++;
		else if(particle->direction == LEFT)
		    particle->x--;
		else if(particle->direction == RIGHT)
		    particle->x++;

		if(particle->y <= 0)particle->newDirection = DOWN;
		if(particle->y >= BoardHeight-1)particle->newDirection = UP;
        if(particle->x <= 0)particle->newDirection = RIGHT;
		if(particle->x >= BoardWidth-1)particle->newDirection = LEFT;
	}
}

void ParticleBuffer::resolveCollisions() {
    for(int d=0;d<particles.size();d++) {
		Particle *thisParticle = particles[d];
		for(int dd=0;dd<particles.size();dd++) {
			Particle *thatParticle = particles[dd];
			if(thisParticle != thatParticle) {
				if(thisParticle->x == thatParticle->x && thisParticle->y == thatParticle->y) {
					if(!thisParticle->collisionResolved && !thatParticle->collisionResolved) {
						if(thisParticle->direction == LEFT && thatParticle->direction == RIGHT) {
							thisParticle->newDirection = UP;
							thatParticle->newDirection = DOWN;
							thisParticle->collisionResolved = true;
                	        thatParticle->collisionResolved = true;
						}
						if(thisParticle->direction == UP && thatParticle->direction == DOWN) {
							thisParticle->newDirection = LEFT;
							thatParticle->newDirection = RIGHT;
							thisParticle->collisionResolved = true;
							thatParticle->collisionResolved = true;
						}
					}
				}
			}
		}
	}
	for(int d=0;d<particles.size();d++) {
		particles[d]->direction = particles[d]->newDirection;
		particles[d]->collisionResolved = false;
		particles[d]->determineIcon();
	}
}

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	while(GameActive) {
		processInput();
		render();
		GameLogic();
//		getchar();
		Sleep(45);
//		Sleep(500);
	}

	return 0;
}

void initGame() {
	screen = new ScreenBuffer(BoardWidth, BoardHeight);
	particleBuffer = new ParticleBuffer();

	for(int y=0;y<BoardHeight;y++) {
		for(int x=0;x<BoardWidth;x++) {
			int chance = rnd(3);
			if(chance == 1) {
				int direction = rnd(4);
				particleBuffer->addParticle(new Particle(x, y, direction));
			}
		}
	}
	// This is to test off the .gif from http://en.wikipedia.org/wiki/File:HPP_small.gif (Needs a 14x10 grid)
//	particleBuffer->addParticle(new Particle(4, 1, UP));
//	particleBuffer->addParticle(new Particle(1, 6, LEFT));
//	particleBuffer->addParticle(new Particle(3, 6, RIGHT));
//	particleBuffer->addParticle(new Particle(8, 3, RIGHT));
}

void GameLogic() {
	particleBuffer->iterate();
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	screen->clearBuffer();

    for(int d=0;d<BoardWidth;d++) {
		screen->setBufferxy(d, 0, 219, LIGHTBLUE);
		screen->setBufferxy(d, BoardHeight-1, 219, LIGHTBLUE);
	}
    for(int d=0;d<BoardHeight;d++) {
		screen->setBufferxy(0, d, 219, LIGHTBLUE);
		screen->setBufferxy(BoardWidth-1, d, 219, LIGHTBLUE);
	}

    for(int d=0;d<particleBuffer->particles.size();d++) {
		Particle *particle = particleBuffer->particles[d];
		screen->setBufferxy(particle->x, particle->y, particle->icon, YELLOW);
	}

//	screen->setBufferxy(5, 5, 220, RED);
//	screen->setBufferxy(6, 5, 223, RED);

	swapBuffers();
}

void swapBuffers() {
	COORD dwBufferSize = {BoardWidth, BoardHeight};
	COORD dwBufferCoord = {0, 0};
    SMALL_RECT rcRegion = {0, 0, BoardWidth-1, BoardHeight-1};
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
