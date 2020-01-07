#include <time.h>
#include <math.h>
#include <string.h>
#include "object.h"
#include "graphics.h"

#include <conio.h>

using namespace std;

Object::Object() {
	name = "default_object";
	loc = vertexfMake(0, 0);
	icon = 'X';
	affectedByGravity = false;
	vel = ZeroVector();
	accel = ZeroVector();
	stationary = false;
}

void Object::RememberState() {
	prevLoc = loc;
	prevVel = vel;
	prevAccel = accel;
}

void Object::Move(double delta) {
	if(!stationary) {
		loc.x = loc.x + (vel.x*delta) + (0.5f*accel.x*(delta*delta));
		loc.y = loc.y + (vel.y*delta) + (0.5f*accel.y*(delta*delta));
	}
}

void Object::ApplyPhysics(double delta) {
	ApplyGravity(delta);
	ApplyMotion(delta);
}

void Object::ApplyGravity(double delta) {
	if(affectedByGravity) {
		//accel.y += GRAVITY*delta;
		accel.y = GRAVITY;
	}
}

void Object::ApplyMotion(double delta) {
	if(!stationary) {
		vel.x = vel.x + (accel.x*delta);
		vel.y = vel.y - (accel.y*delta);	// minus for the y normalization

		if(fabs(vel.y) > TERMINAL_VELOCITY) {
			if(vel.y < 0)
				vel.y = -TERMINAL_VELOCITY;
			else
				vel.y = TERMINAL_VELOCITY;
		}
		if(fabs(vel.x) > TERMINAL_VELOCITY) {
			if(vel.x < 0)
				vel.x = -TERMINAL_VELOCITY;
			else
				vel.x = TERMINAL_VELOCITY;
		}
	}
}

bool Object::CollidesWith(Object *object) {
	if(prevLoc.x-loc.x > 1.0f) {	// Transition skipped a space
		// Handle this situation! Also for y axis too
		// To preserve collision fidelity
	}

	if(cmpvertexf(loc, object->loc))
		return true;
	return false;
}

// This is fubar right now... Need to learn to do it the right way
void Object::CollidedWith(Object *object) {
	// This is a horrible way of doing it...
	int obfuscatedDim = NO_DIM;

//	if(fabs(prevVel.y) > fabs(prevVel.x))
//		obfuscatedDim = Y_DIM;
//	else if(fabs(prevVel.y) < fabs(prevVel.y))
//		obfuscatedDim = X_DIM;
//	else
//		obfuscatedDim = NO_DIM;

	color = YELLOW;
	if((int)object->prevLoc.x == (int)prevLoc.x && (int)object->prevLoc.y > (int)prevLoc.y) {
		obfuscatedDim = Y_DIM;
		color = RED;
	}
//	obfuscatedDim = Y_DIM;

	float meX, meY, youX, youY;
	meX = floor(loc.x);
	meY = floor(loc.y);
	youX = floor(object->loc.x);
	youY = floor(object->loc.y);
//	printf("meX: %f\tmeY: %f\t%s\nyouX: %f\tyouY: %f\t%s\n", meX, meY, name, youX, youY, object->name);
//	printf("meX: %f\tmeY: %f\nyouX: %f\tyouY: %f\n", loc.x, loc.y, object->loc.x, object->loc.y);
	//getch();

	if(strcmp(object->name, "player")) {
		if(obfuscatedDim == Y_DIM) {
			vel.y = 0.0f;
			loc.y = prevLoc.y;
//			if(prevLoc.y < loc.y)
//				loc.y = object->loc.y-1.0f;
//			else
//				loc.y = object->loc.y+1.0f;
		}
		else if(obfuscatedDim == X_DIM) {
			vel.x = 0.0f;
			loc.x = prevLoc.x;
//			if(prevLoc.x < loc.x)
//				loc.x = object->loc.x-1.0f;
//			else
//				loc.x = object->loc.x+1.0f;
		}
		else if(obfuscatedDim == NO_DIM) {	// Velocity pulled in both directions equally
//			vel = ZeroVector();
//			loc = prevLoc;
		}
	}
	else if(strcmp(object->name, "stone")) {
		//
	}
}

Object *Player_Object() {
	Object *retVal = new Object();
	retVal->name = "player";
	retVal->icon = 0x01;
	retVal->color = LIGHTGREEN;
	retVal->affectedByGravity = true;
	return retVal;
}

Object *Player_ObjectAt(vertexf vertex) {
	Object *retVal = Player_Object();
	retVal->loc = vertex;
	return retVal;
}

Object *Stone_Object() {
	Object *retVal = new Object();
	retVal->name = "stone";
	retVal->icon = 'S';
	retVal->color = WHITE;
	return retVal;
}

Object *Stone_ObjectAt(vertexf vertex) {
	Object *retVal = Stone_Object();
	retVal->loc = vertex;
	return retVal;
}

Area::Area() {
	//
}

bool Area::ContainsObject(Object *object) {
	for(vector<Object*>::size_type d=0;d<objects.size();d++) {
		if(objects[d] == object)
			return true;
	}
	return false;
}

bool Area::AddObject(Object *object) {
	if(!ContainsObject(object)) {
		objects.push_back(object);
		return true;
	}
	return false;
}

bool Area::RemoveObject(Object *object) {
	if(ContainsObject(object)) {
		int index = indexForObject(object);
		if(index != -1) {
			vector<Object*>::iterator i = objects.begin();
			i += index;
			objects.erase(i);
			return true;
		}
		return false;	// Index doesn't exist for some reason...
	}
	return false;
}

int Area::indexForObject(Object *object) {
	for(vector<Object*>::size_type d=0;d<objects.size();d++) {
		if(objects[d] == object)
			return d;
	}
	return -1;
}
