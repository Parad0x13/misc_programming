#ifndef __OBJECT__
#define __OBJECT__

#include <vector>
#include "constants.h"

#define AREA_WIDTH	20
#define AREA_HEIGHT	20

class Object {
public:
	Object();
	void RememberState();
	void Move(double delta);
	void ApplyPhysics(double delta);
	void ApplyGravity(double delta);
	void ApplyMotion(double delta);
	bool CollidesWith(Object *object);
	void CollidedWith(Object *object);

	string name;

	string extra;	// Testing purposes

	char icon;
	int color;
	vertexf loc, prevLoc;	// World coordinates
	m_vector vel, prevVel;
	m_vector accel, prevAccel;
	bool affectedByGravity;
	bool stationary;
};

Object *Player_Object();
Object *Player_ObjectAt(vertexf vertex);
Object *Stone_ObjectAt(vertexf vertex);

class Area {
public:
	Area();
	bool ContainsObject(Object *object);
	bool AddObject(Object *object);
	bool RemoveObject(Object *object);
	int indexForObject(Object *object);

	std::vector<Object*>objects;
};

extern Area *world;
extern Object *player;

#endif
