#ifndef __ACTOR__
#define __ACTOR__

#include <vector>
#include "graphics.h"

#define ANTSTATUS_Roaming	100
#define ANTSTATUS_Attacking	101

class Actor {
public:
	Actor();
	virtual void render();
	virtual void logic();
	char icon;
	int color;
	COORD loc;
};

extern std::vector<Actor *>actors;

class Ant : public Actor {
public:
	Ant();
	void logic();
	int status;
};

class AntPile : public Actor {
public:
	AntPile();
	void render();
	int colonySize, maxColonySize;
	float spawnRate;
};

#endif
