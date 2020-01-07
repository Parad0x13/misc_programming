/*	Exploring Game made with DOSGame Template 2.9
1.1 -	Changed initial walls a bit
1.2 -	Fixed a movement/collision bug with the DownRight direction
		Added house creation method and doors
		Added breaking block code
		Added tooltip and inventory
1.3 -	Added 24bit bitmap code and map loading ability
1.4 -	Added Build/Normal/Break mode
		Added health to Items
		Removed CreateHouse
		Added PowerPlate
		Added PowerCable, still needs work
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

#define STILL		1000
#define UP			1001
#define DOWN		1002
#define LEFT		1003
#define RIGHT		1004
#define UPLEFT		1005
#define UPRIGHT		1006
#define DOWNLEFT	1007
#define DOWNRIGHT	1008
#define CARDINAL	1009
#define ORTHOGONAL	1010

#define OPEN		101010
#define CLOSED		101011
#define HORIZONTAL	2020
#define VERTICAL	2022

#define BREAK_MODE	555
#define BUILD_MODE	556
#define NORMAL_MODE	557

#define NO_EMPTY_SLOTS	999

using namespace std;

void initGame();
void setupWorld();
void GameLoop();
void GameLogic();

void ControlCamera();

void initWinHandles();
void gotoxy(int x, int y);
void render();
void renderStatus();
void renderActors();
void swapBuffers();
bool COORDOnScreen(COORD loc);

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
int keyInitialPressed(unsigned int key);
void clearInitialKeyPresses();
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

struct RGB {void log(){printf("[%i %i %i]", r, g, b);};unsigned char r, g, b;};

class Bitmap {
public:
	Bitmap(){};
	Bitmap(char *fileName);
	RGB colorAtXY(int x, int y);
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER bitmapFileHeader;
	unsigned char *bitmapData;
	int width, height;
};

Bitmap::Bitmap(char *filename) {
	FILE *filePtr; //our file pointer
	int imageIdx=0; //image index counter
	unsigned char tempRGB; //our swap variable
	unsigned int fileSize;

	filePtr = fopen(filename,"rb");
	if(filePtr == NULL) {
		printf("Error opening file, does it exist?\n");
		bitmapData = NULL;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(bitmapFileHeader.bfType != 0x4D42) {
		printf("File does not seem to be of Bitmap type\n");
		fclose(filePtr);
		bitmapData = NULL;
	}

	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	if(bitmapInfoHeader.biBitCount != 24) {
		printf("Must have a 24bit bitmap!\n");
		fclose(filePtr);
		bitmapData = NULL;
	}
	else
		fileSize = bitmapInfoHeader.biWidth*bitmapInfoHeader.biHeight*3;

	bitmapData = (unsigned char*)malloc(fileSize);
	if(!bitmapData) {
		printf("Error allocating enough space to store Bitmap\n");
		free(bitmapData);
		fclose(filePtr);
		bitmapData = NULL;
	}

	fread(bitmapData, fileSize, 1, filePtr);

	if(bitmapData == NULL) {
		printf("Could not import Bitmap data!\n");
		fclose(filePtr);
		bitmapData = NULL;
	}

	for(imageIdx = 0;imageIdx < fileSize;imageIdx+=3) {
		tempRGB = bitmapData[imageIdx];
		bitmapData[imageIdx] = bitmapData[imageIdx + 2];
		bitmapData[imageIdx + 2] = tempRGB;
	}

	fclose(filePtr);
	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;
}

RGB Bitmap::colorAtXY(int x, int y) {
	unsigned int index = (y*width+x)*3;
	RGB retVal;
	retVal.r = bitmapData[index];
	retVal.g = bitmapData[index+1];
	retVal.b = bitmapData[index+2];
	return retVal;
}

COORD CameraLocation;
COORD COORDWith(int x, int y) {COORD retVal;retVal.X = x;retVal.Y = y;return retVal;}
bool COORDcmp(COORD one, COORD two) {if(one.X == two.X && one.Y == two.Y)return true;return false;}

class ScreenBuffer {
public:
	ScreenBuffer(){};
	ScreenBuffer(int _width, int _height);
	~ScreenBuffer() {free(buffer);};
	int resolution(){return width*height;};
	SMALL_RECT region() {SMALL_RECT retVal = {0, 0, width-1, height-1};return retVal;};
	void setScreenxy(int x, int y, char character, int color);
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

void ScreenBuffer::setScreenxy(int x, int y, char character, int color) {
	if(COORDOnScreen(COORDWith(x, y))) {
		int index = y*width+x;
		setBufferIndex(index, character, color);
	}
}

void ScreenBuffer::setBufferxy(int x, int y, char character, int color) {
	int CameraX = x+SCREEN_WIDTH/2-CameraLocation.X;
	int CameraY = y+SCREEN_HEIGHT/2-CameraLocation.Y;
	if(COORDOnScreen(COORDWith(CameraX, CameraY))) {
		int index = CameraY*width+CameraX;
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

class ToolTip {
public:
	ToolTip() {
		resetToolTip();
		tickTime = clock();
		tickTimeout = 1.0f;
	};
	void tick() {
		clock_t now = clock();
		if((now-tickTime) > tickTimeout*1000) {
			tickTime = now;
			resetToolTip();
		}
	};
	void resetToolTip() {
		message = "";
	};
	void setToolTip(char *_message) {
		message = _message;
		tickTime = clock();
	};
	char *message;

	clock_t tickTime;
	float tickTimeout;
};

ToolTip *tooltip;

class Actor {
public:
	Actor();
	virtual void render() {screen->setBufferxy(loc.X, loc.Y, icon, determineColor());};
	virtual void logic();
	virtual void move(int direction);
	virtual void interactWith(Actor *actor){};
	virtual void affectHealth(int value) {health += value;if(health < 0)health = 0;else if(health > 100)health = 100;};
	virtual void manageHealth();
	virtual void use(){};
	virtual int determineColor() {
		int newColor = color;
		if(health <= 50)newColor = YELLOW;
		if(health <= 25)newColor = LIGHTRED;
		if(broken)newColor = color;
		return newColor;
	};
	char *name;
	char icon, invIcon;
	int color;
	COORD loc;
	int health, healthRegenRate;
	int movement;
	bool collidable, canRegainHealth;
	clock_t lastRegainHealth;
	float regainHealthTimeout;
	bool breakable, broken;
	bool providingPower, providingSourcePower;
	bool hasPower, hasSourcePower, hadPower, hadSourcePower;
	bool affectedByPower;
};

vector<Actor *>actors;

bool locationIsClearForActor(COORD loc, Actor *actor);

vector<Actor *>actorsAtLocation(COORD loc) {
	vector<Actor *>retVal;
	for(int d=0;d<actors.size();d++) {
		if(COORDcmp(actors[d]->loc, loc))
			retVal.push_back(actors[d]);
	}
	return retVal;
}

Actor::Actor() {
	name = "Actor";
	icon = '?';
	invIcon = icon;
	color = LIGHTMAGENTA;
	loc = COORDWith(0, 0);
	breakable = false;
	broken = false;
	health = 100;
	healthRegenRate = 1;
	lastRegainHealth = clock();
	regainHealthTimeout = 0.25f;
	movement = STILL;
	collidable = true;
	canRegainHealth = true;
	providingPower = false; providingSourcePower = false;
	hasPower = false;hadPower = false;
	hasSourcePower = false;hadSourcePower = false;
	affectedByPower = false;
}

void Actor::logic() {
	move(movement);
	manageHealth();
	if(affectedByPower) {
		if(hasPower)hadPower = true;
		else hadPower = false;
		if(hasSourcePower)hadSourcePower = true;
		else hadSourcePower = false;
		hasPower = false;
		hasSourcePower = false;
		vector<Actor *>toCheckPower;
		vector<Actor *>up = actorsAtLocation(COORDWith(loc.X, loc.Y-1));
		vector<Actor *>down = actorsAtLocation(COORDWith(loc.X, loc.Y+1));
		vector<Actor *>left = actorsAtLocation(COORDWith(loc.X-1, loc.Y));
		vector<Actor *>right = actorsAtLocation(COORDWith(loc.X+1, loc.Y));
		toCheckPower.insert(toCheckPower.end(), up.begin(), up.end());
		toCheckPower.insert(toCheckPower.end(), down.begin(), down.end());
		toCheckPower.insert(toCheckPower.end(), left.begin(), left.end());
		toCheckPower.insert(toCheckPower.end(), right.begin(), right.end());

		for(int d=0;d<toCheckPower.size();d++) {
			if(toCheckPower[d]->providingPower)
				hasPower = true;
			if(toCheckPower[d]->providingSourcePower)
				hasSourcePower = true;
		}
	}
}

void removeActor(Actor *actor) {
	for(int d=0;d<actors.size();d++) {
		if(actors[d] == actor) {
			actors.erase(actors.begin()+d);
			break;
		}
	}
}

void Actor::move(int direction) {
	COORD newLoc = loc;
	switch(direction) {
		case UP:newLoc.Y--;break;
		case DOWN:newLoc.Y++;break;
		case LEFT:newLoc.X--;break;
		case RIGHT:newLoc.X++;break;
		case UPLEFT:newLoc.X--;newLoc.Y--;break;
		case UPRIGHT:newLoc.X++;newLoc.Y--;break;
		case DOWNLEFT:newLoc.X--;newLoc.Y++;break;
		case DOWNRIGHT:newLoc.X++;newLoc.Y++;break;
		default:break;
	}
	movement = STILL;
	switch(direction) {
		case UP:
			if(locationIsClearForActor(newLoc, this))
				loc = newLoc;
			else {	// Collision Detected
				vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X, loc.Y-1));
				for(int d=0;d<toInteractWith.size();d++)
					interactWith(toInteractWith[d]);
			}
			break;
		case DOWN:
			if(locationIsClearForActor(newLoc, this))
				loc = newLoc;
			else {	// Collision Detected
				vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X, loc.Y+1));
				for(int d=0;d<toInteractWith.size();d++)
					interactWith(toInteractWith[d]);
			}
			break;
		case LEFT:
			if(locationIsClearForActor(newLoc, this))
				loc = newLoc;
			else {	// Collision Detected
				vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X-1, loc.Y));
				for(int d=0;d<toInteractWith.size();d++)
					interactWith(toInteractWith[d]);
			}
			break;
		case RIGHT:
			if(locationIsClearForActor(newLoc, this))
				loc = newLoc;
			else {	// Collision Detected
				vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X+1, loc.Y));
				for(int d=0;d<toInteractWith.size();d++)
					interactWith(toInteractWith[d]);
			}
			break;
		case UPLEFT:
			if(locationIsClearForActor(newLoc, this)) {
				if(locationIsClearForActor(COORDWith(loc.X-1, loc.Y), this) ||
				locationIsClearForActor(COORDWith(loc.X, loc.Y-1), this))
					loc = newLoc;
			}
			else {
				if(locationIsClearForActor(COORDWith(loc.X-1, loc.Y), this))
					loc = COORDWith(loc.X-1, loc.Y);
				else if(locationIsClearForActor(COORDWith(loc.X, loc.Y-1), this))
					loc = COORDWith(loc.X, loc.Y-1);
				else {	// Collision Detected
					vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X-1, loc.Y));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
					toInteractWith = actorsAtLocation(COORDWith(loc.X, loc.Y-1));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
				}
			}
			break;
		case UPRIGHT:
			if(locationIsClearForActor(newLoc, this)) {
				if(locationIsClearForActor(COORDWith(loc.X+1, loc.Y), this) ||
				locationIsClearForActor(COORDWith(loc.X, loc.Y-1), this))
					loc = newLoc;
			}
			else {
				if(locationIsClearForActor(COORDWith(loc.X+1, loc.Y), this))
					loc = COORDWith(loc.X+1, loc.Y);
				else if(locationIsClearForActor(COORDWith(loc.X, loc.Y-1), this))
					loc = COORDWith(loc.X, loc.Y-1);
				else {	// Collision Detected
					vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X+1, loc.Y));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
					toInteractWith = actorsAtLocation(COORDWith(loc.X, loc.Y-1));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
				}
			}
			break;
		case DOWNLEFT:
			if(locationIsClearForActor(newLoc, this)) {
				if(locationIsClearForActor(COORDWith(loc.X-1, loc.Y), this) ||
				locationIsClearForActor(COORDWith(loc.X, loc.Y+1), this))
					loc = newLoc;
			}
			else {
				if(locationIsClearForActor(COORDWith(loc.X-1, loc.Y), this))
					loc = COORDWith(loc.X-1, loc.Y);
				else if(locationIsClearForActor(COORDWith(loc.X, loc.Y+1), this))
					loc = COORDWith(loc.X, loc.Y+1);
				else {	// Collision Detected
					vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X-1, loc.Y));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
					toInteractWith = actorsAtLocation(COORDWith(loc.X, loc.Y+1));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
				}
			}
			break;
		case DOWNRIGHT:
			if(locationIsClearForActor(newLoc, this)) {
				if(locationIsClearForActor(COORDWith(loc.X+1, loc.Y), this) ||
				locationIsClearForActor(COORDWith(loc.X, loc.Y+1), this))
					loc = newLoc;
			}
			else {
				if(locationIsClearForActor(COORDWith(loc.X+1, loc.Y), this))
					loc = COORDWith(loc.X+1, loc.Y);
				else if(locationIsClearForActor(COORDWith(loc.X, loc.Y+1), this))
					loc = COORDWith(loc.X, loc.Y+1);
				else {	// Collision Detected
					vector<Actor *>toInteractWith = actorsAtLocation(COORDWith(loc.X+1, loc.Y));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
					toInteractWith = actorsAtLocation(COORDWith(loc.X, loc.Y+1));
					for(int d=0;d<toInteractWith.size();d++)
						interactWith(toInteractWith[d]);
				}
			}
			break;
		default:break;
	}
}

void Actor::manageHealth() {
	if(canRegainHealth) {
		clock_t now = clock();
		if((now-lastRegainHealth) > regainHealthTimeout*1000) {
			affectHealth(healthRegenRate);
			lastRegainHealth = now;
		}
	}
}

class Exp : public Actor {
public:
	Exp(COORD _loc, int _amount) {
		name = "Exp";
		icon = 0x04;
		loc = _loc;
		amount = _amount;
	};
	void logic() {
		color = YELLOW;
		if(amount>10)
			color = LIGHTGREEN;
	};
	int amount;
};

class Item : public Actor {
public:
	Item() {healthRegenRate = 5;breakable = true;};
	void logic() {
		Actor::logic();
		if(broken)
			icon = '.';
	};
	void hit(int damage) {
		affectHealth(-damage);
		if(health <= 0)
			broken = true;
	};
	void render() {
		int newColor = LIGHTGREEN;
		if(health <= 50)newColor = YELLOW;
		if(health <= 25)newColor = LIGHTRED;

		Actor::render();
	}
};

class Inventory {
public:
	Inventory() {
		selected = 1;
		loc = COORDWith(1, 24);
		for(int d=0;d<10;d++) {
			names[d] = "";
			count[d] = 0;
		}
	};
	void render() {
		for(int d=0;d<10;d++) {
			char icon = '?';
			int color = LIGHTMAGENTA;
			if(!strcmp(names[d], "")) {icon = ' ';color = WHITE;}
			else {
				icon = icons[d];
				color = colors[d];
			}
			screen->setScreenxy(loc.X+d, loc.Y, icon, color);
		}
		screen->setScreenxy(loc.X-1, loc.Y, 219, LIGHTGRAY);
		for(int d=0;d<12;d++)screen->setScreenxy(loc.X+d-1, loc.Y-1, 219, LIGHTGRAY);
		screen->setScreenxy(loc.X+10, loc.Y, 219, LIGHTGRAY);
		screen->setScreenxy(loc.X+selected-1, loc.Y-1, 'v', YELLOW);
	};
	int nextEmptySlot() {
		for(int d=0;d<10;d++) {
			if(!strcmp(names[d], ""))
				return d;
		}
		return NO_EMPTY_SLOTS;
	};
	void addItem(Item *item, int _count) {
		int nextSlot = nextEmptySlot();
		if(nextSlot != NO_EMPTY_SLOTS) {
			names[nextSlot] = item->name;
			icons[nextSlot] = item->invIcon;
			colors[nextSlot] = item->color;
			count[nextSlot] = _count;
		}
	};
	char *names[10];
	char icons[10];
	int colors[10];
	int count[10];
	int selected;
	COORD loc;
};

class Wall : public Item {
public:
	Wall() {
		name = "Wall";
		icon = 219;
		invIcon = 219;
		color = WHITE;
	};
};

void createWall(COORD startLoc, int Direction, int size) {
	int xinc = 0, yinc = 0;
	if(Direction == HORIZONTAL)xinc = 1;
	else if(Direction == VERTICAL)yinc = 1;
	for(int d=0;d<size;d++) {
		COORD newLoc = COORDWith(startLoc.X+d*xinc, startLoc.Y+d*yinc);
		Wall *wall = new Wall();
		wall->loc = newLoc;
		actors.push_back(wall);
	}
}

class Door : public Item {
public:
	Door(COORD _loc) {
		icon = 'D';
		invIcon = '|';
		name = "Door";
		color = LIGHTGREEN;
		loc = _loc;
		status = CLOSED;
		affectedByPower = true;
	};
	void logic() {
		Item::logic();
		if(!broken) {
			if(hasPower)
				status = OPEN;
			else {
				if(hadPower)
					status = CLOSED;
			}
			if(status == CLOSED) {
				icon = '_';
				collidable = true;
			}
			else if(status == OPEN) {
				icon = '|';
				collidable = false;
			}
		}
	};
	void use() {
		if(status == CLOSED)
			status = OPEN;
		else if(status == OPEN)
			status = CLOSED;
	};
	int status;
};

class Cactus : public Item {
public:
	Cactus(COORD _loc) {
		name = "Cactus";
		icon = 176;
		invIcon = 176;
		color = LIGHTGREEN;
		loc = _loc;
	};
};

class PowerPlate : public Item {
public:
	PowerPlate(COORD _loc) {
		icon = 178;
		invIcon = 178;
		name = "Door";
		color = RED;
		loc = _loc;
		collidable = false;
		tickTimeout = 1.0f;
		active = false;
	};
	void tick() {
		if(!active) {
			clock_t now = clock();
			if((now-tickTime) > tickTimeout*1000) {
				tickTime = now;
				active = false;
				providingPower = false;
				providingSourcePower = false;
			}
		}
	};
	int determineColor() {
		if(providingPower)
			return LIGHTRED;
		else if(!providingPower)
			return RED;
	};
	void logic() {
		Item::logic();
		if(active){providingPower = true;providingSourcePower = true;}

		if(actorsAtLocation(loc).size() > 1) {
			tickTime = clock();
			active = true;
		}
		else
			active = false;
		tick();
	};
	float tickTimeout;
	clock_t tickTime;
	bool active;
};

class PowerCable : public Item {
public:
	PowerCable(COORD _loc) {
		icon = 'c';
		invIcon = 'c';
		name = "PowerCable";
		loc = _loc;
		color = RED;
		collidable = false;
		tickTimeout = 0.1f;
		active = false;
		affectedByPower = true;
	};
	void tick() {
		if(!active) {
			clock_t now = clock();
			if((now-tickTime) > tickTimeout*1000) {
				tickTime = now;
				active = false;
				providingPower = false;
			}
		}
	};
	int determineColor() {
		if(providingPower)
			return LIGHTRED;
		else if(!providingPower)
			return RED;
	};
	void logic() {
		Item::logic();
		if(active)providingPower = true;

		if(hasSourcePower) {
			tickTime = clock();
			active = true;
		}
		else {
			active = false;
		}
		tick();
	};
	float tickTimeout;
	clock_t tickTime;
	bool active;
};

class Player : public Actor {
public:
	Player() {
		icon = 0x01;
		color = LIGHTGREEN;
		exp = 0;
		inventory = new Inventory();
		mode = NORMAL_MODE;
	};
	void panMode(int direction) {
		// BreakMode NormalMode BuildMode
		if(direction == LEFT && mode == BREAK_MODE)mode = BUILD_MODE;
		else if(direction == LEFT && mode == NORMAL_MODE)mode = BREAK_MODE;
		else if(direction == LEFT && mode == BUILD_MODE)mode = NORMAL_MODE;
		else if(direction == RIGHT && mode == BREAK_MODE)mode = NORMAL_MODE;
		else if(direction == RIGHT && mode == NORMAL_MODE)mode = BUILD_MODE;
		else if(direction == RIGHT && mode == BUILD_MODE)mode = BREAK_MODE;
	};
	void touch(int direction);
	void logic();
	void render();
	void interactWith(Actor *actor);
	void breakBlock();
	void placeBlock();
	Inventory *inventory;
	int exp;
	int touchDirection;
	int mode;
};

void Player::touch(int direction) {
	Actor *object;
	touchDirection = direction;
	vector<Actor *>touching;
	if(touchDirection == UP)
		touching = actorsAtLocation(COORDWith(loc.X, loc.Y-1));
	if(touchDirection == DOWN)
		touching = actorsAtLocation(COORDWith(loc.X, loc.Y+1));
	if(touchDirection == LEFT)
		touching = actorsAtLocation(COORDWith(loc.X-1, loc.Y));
	if(touchDirection == RIGHT)
		touching = actorsAtLocation(COORDWith(loc.X+1, loc.Y));

	for(int d=0;d<touching.size();d++) {
		Actor *object = touching[d];
		if(!strcmp(object->name, "Wall")) {
			if(mode == BREAK_MODE)((Item *)object)->hit(25);
		}
		if(!strcmp(object->name, "Door")) {
			if(mode == BREAK_MODE)((Item *)object)->hit(25);
			else if(mode == NORMAL_MODE)((Item *)object)->use();
		}
		if(!strcmp(object->name, "Cactus")) {
			if(mode == BREAK_MODE)((Item *)object)->hit(25);
		}
		tooltip->setToolTip(object->name);
	}
}

void Player::logic() {
	Actor::logic();

	touchDirection = STILL;
}

void Player::breakBlock() {
	vector<Actor *>blocks;
	COORD blockLoc = loc;
	if(touchDirection == UP)blockLoc = COORDWith(loc.X, loc.Y-1);
	if(touchDirection == DOWN)blockLoc = COORDWith(loc.X, loc.Y+1);
	if(touchDirection == LEFT)blockLoc = COORDWith(loc.X-1, loc.Y);
	if(touchDirection == RIGHT)blockLoc = COORDWith(loc.X+1, loc.Y);

	blocks = actorsAtLocation(blockLoc);
	for(int d=0;d<blocks.size();d++) {
		Actor *block = blocks[d];
		if(block && block!=this) {
			if(((Item *)block)->breakable)
				((Item *)block)->broken = true;
		}
	}
}

void Player::placeBlock() {
	//
}

void Player::render() {
	Actor::render();

	int touchColor = MAGENTA;
	if(mode == BREAK_MODE)touchColor = LIGHTRED;
	else if(mode == NORMAL_MODE)touchColor = YELLOW;
	else if(mode == BUILD_MODE)touchColor = LIGHTGREEN;
	if(touchDirection == UP)screen->setBufferxy(loc.X, loc.Y-1, '^', touchColor);
	else if(touchDirection == DOWN)screen->setBufferxy(loc.X, loc.Y+1, 'v', touchColor);
	else if(touchDirection == LEFT)screen->setBufferxy(loc.X-1, loc.Y, '<', touchColor);
	else if(touchDirection == RIGHT)screen->setBufferxy(loc.X+1, loc.Y, '>', touchColor);
	inventory->render();
	screen->setScreenxy(inventory->loc.X+12, inventory->loc.Y, 'B', LIGHTRED);
	screen->setScreenxy(inventory->loc.X+13, inventory->loc.Y, 'N', YELLOW);
	screen->setScreenxy(inventory->loc.X+14, inventory->loc.Y, 'B', LIGHTGREEN);
	int modeOffset;
	if(mode == BREAK_MODE)modeOffset = 0;
	if(mode == NORMAL_MODE)modeOffset = 1;
	if(mode == BUILD_MODE)modeOffset = 2;
	screen->setScreenxy(inventory->loc.X+12+modeOffset, inventory->loc.Y-1, 'v', YELLOW);
}

void Player::interactWith(Actor *actor) {
	if(!strcmp(actor->name, "Wall")) {
		if(((Item *)actor)->broken) {
			inventory->addItem((Item *)actor, 1);
			removeActor(actor);
		}
	}
	if(!strcmp(actor->name, "Door")) {
		if(((Item *)actor)->broken) {
			inventory->addItem((Item *)actor, 1);
			removeActor(actor);
		}
	}
	if(!strcmp(actor->name, "Cactus")) {
		if(((Item *)actor)->broken) {
			inventory->addItem((Item *)actor, 1);
			removeActor(actor);
		}
		else
			affectHealth(-2);
	}
	if(!strcmp(actor->name, "Exp")) {
		exp+=((Exp *)actor)->amount;
		removeActor(actor);
	}
	tooltip->setToolTip(actor->name);
}

Player *player;

HANDLE hIn, hOut;

unsigned int Keys[MAX_VK_SIZE];
unsigned int KeyInitialPress[MAX_VK_SIZE];

bool GameActive = true;

int main(int argc, char *argv[]) {
	seed();
	initWinHandles();
	initKB();
	initGame();

	GameLoop();

	return 0;
}

void initGame() {
	tooltip = new ToolTip();
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	CameraLocation = COORDWith(0, 0);
	setupWorld();
}

void setupWorld() {
	player = new Player();
	actors.push_back(player);

	Bitmap *world = new Bitmap("world.bmp");
	for(int y=0;y<world->height;y++) {
		for(int x=0;x<world->width;x++) {
			COORD worldCOORD = COORDWith(x-world->width/2, -(y-world->height/2));
			RGB color = world->colorAtXY(x, y);
			if(color.r==0&&color.g==255&&color.b==0) {
				player->loc = worldCOORD;
			}
			if(color.r==255&&color.g==255&&color.b==255) {
				Wall *wall = new Wall();
				wall->loc = worldCOORD;
				actors.push_back(wall);
			}
			if(color.r==30&&color.g==170&&color.b==70) {
				Door *door = new Door(worldCOORD);
				actors.push_back(door);
			}
			if(color.r==180&&color.g==230&&color.b==30) {
				Cactus *cactus = new Cactus(worldCOORD);
				actors.push_back(cactus);
			}
			if(color.r==200&&color.g==150&&color.b==0) {
				PowerPlate *powerplate = new PowerPlate(worldCOORD);
				actors.push_back(powerplate);
			}
			if(color.r==200&&color.g==100&&color.b==0) {
				PowerCable *powercable = new PowerCable(worldCOORD);
				actors.push_back(powercable);
			}
		}
	}
}

void GameLoop() {
	while(GameActive) {
		processInput();
		render();
		GameLogic();
		tooltip->tick();
		Sleep(45);
	}
}

void GameLogic() {
	for(int d=0;d<actors.size();d++) {
		actors[d]->logic();
	}
}

void ControlCamera() {
	int CameraMaxDistanceX = 15;
	int CameraMaxDistanceY = 8;
	if(fabs(CameraLocation.X-player->loc.X) > CameraMaxDistanceX) {
		if(player->loc.X < CameraLocation.X) CameraLocation.X--;
		else CameraLocation.X++;
	}
	if(fabs(CameraLocation.Y-player->loc.Y) > CameraMaxDistanceY) {
		if(player->loc.Y < CameraLocation.Y) CameraLocation.Y--;
		else CameraLocation.Y++;
	}
}

bool locationIsClearForActor(COORD loc, Actor *actor) {
	for(int d=0;d<actors.size();d++) {
		if(actors[d] != actor && COORDcmp(actors[d]->loc, loc) && actors[d]->collidable)
			return false;
	}
	return true;
}

Actor *actorAtLocation(COORD loc) {
	for(int d=0;d<actors.size();d++) {
		if(COORDcmp(actors[d]->loc, loc))
			return actors[d];
	}
	return NULL;
}

bool actorAtLocationOtherThan(Actor *actor) {
	for(int d=0;d<actors.size();d++) {
		if(COORDcmp(actors[d]->loc, actor->loc) && actors[d] != actor)
			return true;
	}
	return false;
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {COORD pos = {x, y};SetConsoleCursorPosition(hOut, pos);}

void render() {
	ControlCamera();
	screen->clearBuffer();
	renderActors();
	player->render();	// Ensure player is always on top
	swapBuffers();
	renderStatus();
}

void renderStatus() {
	gotoxy(0, 0);
	printf("Health: %i\n", player->health);
	printf("Exp: %i\n", player->exp);
	printf("Loc: (%i, %i)\n", player->loc.X, player->loc.Y);
	printf("%s", tooltip->message);
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
	clearInitialKeyPresses();
}

void clearInitialKeyPresses() {
	for(int d=0;d<MAX_VK_SIZE;d++)
		KeyInitialPress[d] = false;
}

void addKBHold(unsigned int key) {
	if(!keyPressed(key)) {
		Keys[key] = true;
		KeyInitialPress[key] = true;
	}
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

int keyInitialPressed(unsigned int key) {
	int index = (int)key;
	if(KeyInitialPress[index])
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
		if(InRec.Event.KeyEvent.bKeyDown) {
			addKBHold(key);
		}
		else {
			removeKBHold(key);
		}
	}
}

// Virtual Keys for letters are the same as their ASCII counterparts
// Therefore they are not defined in windows.h
// Must use their Capped version too, lower 'a' won't work etc...
void processKeyPresses() {
	if(keyPressed(VK_ESCAPE))
		GameActive = false;

	if(keyPressed('W'))
		player->movement = UP;
	if(keyPressed('S'))
		player->movement = DOWN;
	if(keyPressed('A'))
		player->movement = LEFT;
	if(keyPressed('D'))
		player->movement = RIGHT;
	if(keyPressed('W') && keyPressed('A'))
		player->movement = UPLEFT;
	if(keyPressed('W') && keyPressed('D'))
		player->movement = UPRIGHT;
	if(keyPressed('S') && keyPressed('A'))
		player->movement = DOWNLEFT;
	if(keyPressed('S') && keyPressed('D'))
		player->movement = DOWNRIGHT;

	if(keyInitialPressed(VK_UP))
		player->touch(UP);
	if(keyInitialPressed(VK_DOWN))
		player->touch(DOWN);
	if(keyInitialPressed(VK_LEFT))
		player->touch(LEFT);
	if(keyInitialPressed(VK_RIGHT))
		player->touch(RIGHT);

	if(keyInitialPressed('Q'))player->panMode(LEFT);
	if(keyInitialPressed('E'))player->panMode(RIGHT);

	if(keyPressed('1'))player->inventory->selected = 1;
	if(keyPressed('2'))player->inventory->selected = 2;
	if(keyPressed('3'))player->inventory->selected = 3;
	if(keyPressed('4'))player->inventory->selected = 4;
	if(keyPressed('5'))player->inventory->selected = 5;
	if(keyPressed('6'))player->inventory->selected = 6;
	if(keyPressed('7'))player->inventory->selected = 7;
	if(keyPressed('8'))player->inventory->selected = 8;
	if(keyPressed('9'))player->inventory->selected = 9;
	if(keyPressed('0'))player->inventory->selected = 10;

	clearInitialKeyPresses();
}
