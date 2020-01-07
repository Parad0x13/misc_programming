#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	InitEngine();
}

void PhysicsEngine::InitEngine()
{
	ObjectCount = 0;

	Stars[0].x = 0.2f;
	Stars[0].y = 0.0f;
	Stars[0].mass = 10.0f;
	Stars[0].xVel = 0.02f;
	Stars[0].yVel = 0.0f;
	ObjectCount++;
};

void PhysicsEngine::RunEngine()
{
	for(int d=0;d<ObjectCount;d++)
	{
		Stars[d].x += Stars[0].xVel;
		Stars[d].y += Stars[0].yVel;
	}
};