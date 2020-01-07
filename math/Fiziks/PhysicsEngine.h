#ifndef _PHYSICSENGINE_
#define _PHYSICSENGINE_

struct Object
{
	float x, y, mass, xVel, yVel;
};

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine(){};

	void InitEngine();
	void RunEngine();

	int ObjectCount;
	Object Stars[10];
};

#endif