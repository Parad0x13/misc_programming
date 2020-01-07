/*	Explore RPG made to emulate Minecraft for DOS
	Created by Bryan Tremblay V1.1	*/

#include <stdio.h>
#include <alloc.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define BOOL int
#define RenderInterval (1.0f/20.0f)

#define Console_X 80
#define Console_Y 25
#define Console_Dimension (Console_X * Console_Y)

#define ESC		0x1B
#define UP		0x48
#define DOWN	0x50
#define LEFT	0x4B
#define RIGHT	0x4D

#define Air_Icon			'A'
#define WorldEdge_Icon		177
#define Player_Icon			0x02

/*	--== Game Level Managment ==--
	The game areas will be broken into partitions:
	World:	Area that encompasses all data about the environment in its entirety. There can be more than one world
	Area:	Partition of a world that is centered around the characters location. Area updates itself dynamically as required
	The screen will be populated with objects that appear inside the Area
*/

/*	--== Structure Definitions ==--	*/
typedef struct {
	int x, y;
}Point;

typedef struct {
	int height, width;
}Size;

/*	--== Structure Definitions ==--	*/

Point PointWith(int theX, int theY) {
	Point retVal;
	retVal.x = theX;
	retVal.y = theY;
	return retVal;
}

Point PointWithPoint(Point thePoint) {
	Point retVal;
	retVal.x = thePoint.x;
	retVal.y = thePoint.y;
	return retVal;
}

Size SizeWith(int theWidth, int theHeight) {
	Size retVal;

	retVal.height = theHeight;
	retVal.width = theWidth;

	return retVal;
}

/*	--== Class Declarations ==--	*/

class Object {
public:
	char *name;
	char icon;
	int color;

	Object() {
		name = "Default Object";
		icon = 'D';
		color = WHITE;
	}
	Object(char *theName, char theIcon, int theColor);
};

class Air : public Object {
public:
	Air() {
		name = "Air";
		icon = Air_Icon;
		color = WHITE;
	}
};

class WorldEdge : public Object {
public:
	WorldEdge() {
		name = "WorldEdge";
		icon = WorldEdge_Icon;
		color = WHITE;
	}
};

class OutsideSpace : public Object {
public:
	OutsideSpace() {
		name = "Outside Space";
		icon = 'O';
		color = WHITE;
	}
};

class LifeForm {
public:
	char *name;
	int health;
	Point positionInWorld;

	LifeForm() {
		name = "Default LifeForm";
		health = 100;
		positionInWorld = PointWith(0, 0);
	}
};

class Space {
public:
	Size size;
	Object *space;

	Space() {
		size = SizeWith(0, 0);
		space = (Object *)malloc(size.height*size.width*sizeof(Object));
	}
	Space(Size theSize);
	Space(Space parentSpace, Point topLeft, Size theSize);
	
	Object GetObjectAtLocation(Point theLocation);
	void PlaceObjectAtLocation(Object theObject, Point theLocation);
};

class World {
public:
	char *name;
	Space space;

	World() {
		name = "Default World";
		space = Space(SizeWith(0, 0));
	}
	World(Size theSize);
	void FillWithAir();
};

class Area {
public:
	char *name;
	Space space;

	Area();
	Area(Space theSpace, Point topLeft, Size size);
};

/*	--== Class Definitions ==--	*/

Object::Object(char *theName, char theIcon, int theColor) {
    name = theName;
    icon = theIcon;
    color = theColor;
}

Space::Space(Size theSize) {
	int d;
	size = theSize;
	space = (Object *)malloc(size.height*size.width*sizeof(Object));

	for(d=0;d<size.width*size.height;d++)
		space[d] = Object();
}

Space::Space(Space parentSpace, Point topLeft, Size theSize) {
	int x, y;

	size = theSize;
	space = (Object *)malloc(size.height*size.width*sizeof(Object));

	for(y=0;y<theSize.height;y++) {
		for(x=0;x<theSize.width;x++) {
			Point theLocation = PointWith(topLeft.x+x, topLeft.y+y);
			Object theObject;
			theObject = parentSpace.GetObjectAtLocation(theLocation);
			PlaceObjectAtLocation(theObject, PointWith(x, y));
		}
	}
}

Object Space::GetObjectAtLocation(Point theLocation) {
	if(theLocation.x < size.width && theLocation.y < size.height && theLocation.x >= 0 && theLocation.y >= 0) {	// Inside the space
		return space[theLocation.y*size.width+theLocation.x];
	}
	return OutsideSpace();
}

void Space::PlaceObjectAtLocation(Object theObject, Point theLocation) {
	space[theLocation.y*size.width+theLocation.x] = theObject;
}

World::World(Size theSize) {
	name = "Default Name";
	space = Space(theSize);
}

void World::FillWithAir() {
	int x, y;
	for(y=0;y<space.size.height;y++) {
		for(x=0;x<space.size.width;x++) {
			space.PlaceObjectAtLocation(Air(), PointWith(x, y));
		}
	}
}

Area::Area(Space theSpace, Point topLeft, Size theSize) {
	name = "Default Area";
	space = Space(theSpace, topLeft, theSize);
}

/*	--== Function Declarations ==--	*/

void PrepareGame();
void GameLoop();
void render();
void renderPlayer();
void renderSpace(Space theSpace, Point topLeft);
void clearBoard();
void boardToScreenBuffer();
void printScreen();
void ProcessInput(int theInput);
void processPlayerInput(int theInput);

/*	--== Global Variables ==--	*/

World GlobalWorld;
BOOL GameRunning;
clock_t renderTimer;
int pauseLength = 10;
LifeForm Player;

unsigned char far *Screen_base = (unsigned char far*)0xB8000000;
unsigned char far *ScreenBuffer;

Object *board;

/*	--== Function Definitions ==--	*/

void main() {
	PrepareGame();

	clrscr();
	GameLoop();
	clrscr();
}

void PrepareGame() {
	ScreenBuffer = (unsigned char far*)malloc(2*Console_Dimension*sizeof(char));
	board = (Object *)malloc(Console_Dimension*sizeof(Object));

	GlobalWorld = World(SizeWith(30, 10));
	GlobalWorld.FillWithAir();
	GlobalWorld.space.PlaceObjectAtLocation(Object(), PointWith(6, 2));

	Player.name = "Player";
	Player.positionInWorld = PointWith(5, 2);
}

void GameLoop() {
	if(!renderTimer)
		renderTimer = clock();
	GameRunning = 1;

	while(GameRunning) {
		if(kbhit())
			ProcessInput(getch());

		render();
		//delay(pauseLength);
	}
}

void render() {
	int x, y;

	clearBoard();

	renderSpace(GlobalWorld.space, PointWith(0, 0));
	renderPlayer();

	boardToScreenBuffer();

	printScreen();
}

void renderPlayer() {
	board[Player.positionInWorld.y*Console_X+Player.positionInWorld.x] = Object("Player", Player_Icon, WHITE);
}

void renderSpace(Space theSpace, Point topLeft) {
	int x, y;
	int XOffset, YOffset;

//	XOffset = Console_X/2;
//	YOffset = Console_Y/2;

	if(topLeft.x<0 || topLeft.y<0 || topLeft.x>theSpace.size.width || topLeft.y>theSpace.size.height) {
		printf("Invalid parameters passed to renderSpace, program may not continue as intended");
		getch();
	}

	for(y=0;y<Console_Y;y++) {
		for(x=0;x<Console_X;x++) {
			int theX, theY;
			theX = x+topLeft.x;
			theY = y+topLeft.y;
			Object theObject = theSpace.GetObjectAtLocation(PointWith(theX, theY));
			if(!strcmp(theObject.name, "Outside Space"))	// Lies outside the space
				board[y*Console_X+x] = WorldEdge();
			else	// Lies inside the space
				board[y*Console_X+x] = theObject;
		}
	}
}

void clearBoard() {
	int d;
	for(d=0;d<Console_Dimension;d++)
		board[d] = WorldEdge();
}

void boardToScreenBuffer() {
	int d;

	unsigned char far *target = ScreenBuffer;
	for(d=0;d<Console_Dimension;d++) {
		*target = board[d].icon;
		*(target+1) = board[d].color;
		++target;
		++target;
	}
}

void printScreen() {
	int d;
	unsigned char far *target = Screen_base;
	for(d=0;d<Console_Dimension*2;d+=2) {
		*target = ScreenBuffer[d];
		*(target+1) = ScreenBuffer[d+1];
		++target;
		++target;
	}
}

void ProcessInput(int theInput) {
	if(theInput == ESC)
		GameRunning = 0;
	if(theInput == 'd' || theInput == 'a' || theInput == 'w' || theInput == 's')
		processPlayerInput(theInput);
}

void processPlayerInput(int theInput) {
	Point newLocation;
	switch(theInput) {
		case 'w':
			newLocation = PointWith(Player.positionInWorld.x, Player.positionInWorld.y-1);
			break;
		case 's':
			newLocation = PointWith(Player.positionInWorld.x, Player.positionInWorld.y+1);
			break;
		case 'a':
			newLocation = PointWith(Player.positionInWorld.x-1, Player.positionInWorld.y);
			break;
		case 'd':
			newLocation = PointWith(Player.positionInWorld.x+1, Player.positionInWorld.y);
			break;
		default:
			break;
	}
	Object willCollideWith = GlobalWorld.space.GetObjectAtLocation(newLocation);
	if(!strcmp(willCollideWith.name, "Air"))
		Player.positionInWorld = newLocation;
}
