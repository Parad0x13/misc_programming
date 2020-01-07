#ifndef __ACTOR__
#define __ACTOR__

typedef struct _velocity {
	float x, y;
}velocity;

velocity velocityWith(float x, float y) {
	velocity retVal;
	retVal.x = x;
	retVal.y = y;
	return retVal;
}

velocity stillVelocity() {
	return velocityWith(0, 0);
};

bool cmpVelocities(velocity vel1, velocity vel2) {
	if(vel1.x == vel2.x && vel1.y == vel2.y)
		return true;
	return false;
}

typedef struct _vertexi {
	int x, y;
}vertexi;

vertexi vertexiMake(int _x, int _y) {
	vertexi retVal;
	retVal.x = _x;
	retVal.y = _y;
	return retVal;
}

bool cmpVertexi(vertexi vert1, vertexi vert2) {
	if(vert1.x == vert2.x && vert1.y == vert2.y)
		return true;
	return false;
}

typedef struct _vertexf {
	float x, y;
}vertexf;

vertexf vertexfMake(float _x, float _y) {
	vertexf retVal;
	retVal.x = _x;
	retVal.y = _y;
	return retVal;
}

vertexi vertexftoi(vertexf loc) {
	vertexi retVal;
	retVal.x = (int)loc.x;
	retVal.y = (int)loc.y;
	return retVal;
}

class Actor {
public:
	Actor();

	bool alive;
	string name;
	char icon;
	velocity vel, testVel;
	vertexf loc, testLoc;
};

Actor::Actor() {
	alive = true;
	name = "default_actor";
	icon = 'a';
	vel = stillVelocity();
	loc = vertexfMake(0, 0);
}

Actor *Player_Actor() {
	Actor *retVal = new Actor;
	retVal->name = "player";
	retVal->icon = 0x01;
	return retVal;
}

#endif
