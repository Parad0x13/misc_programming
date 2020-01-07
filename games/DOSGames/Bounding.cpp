/* Bounding is a test at utilizing AABB's for collision detection */
// V1.0
#include <stdio.h>
#include <math.h>

#define timeSlice 0.25f

typedef struct vect2f {
	float x, y;
}vect2f;
vect2f vect2fMake(float x, float y) {vect2f retVal = {x, y};return retVal;}

typedef struct AABB2f {vect2f center;vect2f radius;}AABB;
AABB2f AABB2fMake(vect2f center, vect2f size) {vect2f radius = vect2fMake(size.x/2, size.y/2);AABB2f retVal = {center, radius};return retVal;};
bool AABB2fIntersect(AABB2f a, AABB2f b) {
	if(fabs(a.center.x-b.center.x) > (a.radius.x + b.radius.x)) return false;
	if(fabs(a.center.y-b.center.y) > (a.radius.y + b.radius.y)) return false;
	return true;
}

class Actor {
public:
	Actor(){
		loc = vect2fMake(0, 0);
		size = vect2fMake(1, 1);
		vel = vect2fMake(0, 0);
	};
	AABB2f AABB(){return AABB2fMake(loc, size);};
	bool collidesWith(Actor *other) {
		if(AABB2fIntersect(AABB(), other->AABB()))
			return true;
		return false;
	};
	void applyPhysics();
	vect2f loc, size, vel;
	vect2f prevLoc, prevSize, prevVel;
};

void Actor::applyPhysics() {
	prevLoc = loc;prevSize = size;prevVel = vel;

	vect2f delta;
	delta.x = vel.x * timeSlice;
	delta.y = vel.y * timeSlice;
	loc.x += delta.x;
	loc.y += delta.y;
	printf("Old loc:\tvect2f(%f, %f)\nHas become:\tvect2f(%f, %f)*\nDelta Vector:\tvect2f(%f, %f)*\n", prevLoc.x, prevLoc.y, loc.x, loc.y, delta.x, delta.y);
}

int main() {
	Actor *one = new Actor();
	one->loc = vect2fMake(0, 0);
	one->vel = vect2fMake(1, 0);
	Actor *two = new Actor();
	two->loc = vect2fMake(5, 0);

	for(;;) {
		one->applyPhysics();

		if(one->collidesWith(two))
			printf("Intersects\n");
		else
			printf("Does not intersect\n");
		getchar();
	}
	return false;
}
