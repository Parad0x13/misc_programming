#include "collisionDetection.h"
#include "object.h"

void collisionDetection() {
	int size = world->objects.size();
	for(int d=0;d<size;d++) {
		for(int dd=d+1;dd<size;dd++) {
			Object *me = world->objects[d];
			Object *him = world->objects[dd];
			if(me->CollidesWith(him)) {
				me->CollidedWith(him);
				him->CollidedWith(me);
			}
		}
	}
}
