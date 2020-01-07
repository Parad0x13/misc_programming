#ifndef __BULLET__
#define __BULLET__

#define MAX_BULLETS 100

class Bullet : public Actor {
public:
	Bullet(vertex _loc, velocity _vel, int _type);
	void runLogic();
	void collisionDetection();

	bool alive;
	int type;	// E.g. friendly, or enemy bullet
	unsigned char icon;
	unsigned int color;
};

Bullet *bullets[MAX_BULLETS];
int bulletCount = 0;

int nextAvailableBulletIndex() {
	int retVal = -1;

	for(int d=0;d<MAX_BULLETS;d++) {
		if(bullets[d] == NULL)
			return d;
	}

	return retVal;
}

void newBullet(vertex loc, velocity vel, int type) {
	int index = nextAvailableBulletIndex();
	if(index != -1) {
		bullets[index] = new Bullet(loc, vel, type);
	}
}

void removeBulletAtIndex(int index) {
	delete bullets[index];
	bullets[index] = NULL;
}

Bullet::Bullet(vertex _loc, velocity _vel, int _type) {
	alive = true;
	icon = '.';
	type = _type;
	if(type == FRIENDLY)
		color = LIGHTBLUE;
	else if(type == ENEMY)
		color = LIGHTRED;
	else
		color = WHITE;
	loc = _loc;
	vel = _vel;
}

void Bullet::runLogic() {
	float friction = 0.0f;
	loc.x += vel.x;
	loc.y += vel.y;
	vel.x = vel.x - vel.x*friction;
	vel.y = vel.y - vel.y*friction;

	collisionDetection();
}

void Bullet::collisionDetection() {
	if(loc.x < 0) {
		alive = false;
	}
	if(loc.x > SCREEN_WIDTH-1) {
		alive = false;
	}
	if(loc.y < 0) {
		alive = false;
	}
	if(loc.y > SCREEN_HEIGHT-1) {
		alive = false;
	}
}

#endif
