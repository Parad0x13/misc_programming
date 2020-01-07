#include "gameLogic.h"
#include "object.h"
#include "collisionDetection.h"

#include <conio.h>

clock_t lastPhysics = 0;

void GameLogic() {
	clock_t now = clock();
	double delta = (now - lastPhysics) / 1000.0f;

//	getch();
//	delta = 0.1f;

	int size = world->objects.size();
	for(int d=0;d<size;d++)
		world->objects[d]->RememberState();
	for(int d=0;d<size;d++)
		world->objects[d]->ApplyPhysics(delta);
	for(int d=0;d<size;d++)
		world->objects[d]->Move(delta);
	collisionDetection();

	lastPhysics = now;
}
