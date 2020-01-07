#ifndef __Player__
#define __Player__

void collisionDetection();

class Player : public Actor {
public:
	Player(int _type);
	bool canShoot();
	void shoot(float direction);	// In radians
	void runLogic();
	void collisionDetection();
	float fireRate();
	void takeDamage(int damage);

	bool alive;
	int type;	// E.g. friendly, or enemy player
	unsigned char icon;
	unsigned int color;

	int health;
	float speed, maxSpeed;
	int ShotsPerSec;
	float fireSpeed;
	clock_t lastFire;
};

Player *mainPlayer;
Player *enemies[MAX_ENEMIES];
int enemyCount;

void Player::takeDamage(int damage) {
	health -= damage;
	if(health < 0)
		health = 0;
	if(health == 0) {
		alive = false;
		if(type == ENEMY)
			GameScore++;
	}
}

int nextAvailableEnemyIndex() {
	int retVal = -1;

	for(int d=0;d<MAX_ENEMIES;d++) {
		if(enemies[d] == NULL)
			return d;
	}

	return retVal;
}

void newEnemy(vertex loc, velocity vel, int health) {
	int index = nextAvailableEnemyIndex();
	if(index != -1) {
		enemies[index] = new Player(ENEMY);
		enemies[index]->loc = loc;
		enemies[index]->vel = vel;
		enemies[index]->health = health;
	}
}

Player::Player(int _type) {
	type = _type;
	alive = true;
	if(type == FRIENDLY) {
		icon = 0x01;
		color = LIGHTGREEN;
	}
	else if(type == ENEMY) {
		icon = 0x04;
		color = YELLOW;
	}
	else {
		icon = '?';
		color = WHITE;
	}
	health = 1;
	loc = vertexWith(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	vel = velocityWith(1, 0);
	speed = 0.2f;
	maxSpeed = 1.0f;
	ShotsPerSec = 10;
	fireSpeed = 2.0f;
}

float Player::fireRate() {
	return 1.0f/(float)ShotsPerSec;
}

bool Player::canShoot() {
	unsigned int delta = clock() - lastFire;
	if(delta >= fireRate()*1000.0f)
		return true;
	return false;
}

void removeEnemyAtIndex(int index) {
	delete enemies[index];
	enemies[index] = NULL;
}

void Player::shoot(float direction) {
	newBullet(loc, velocityWith(cos(direction), -sin(direction)), FRIENDLY);	// Negative because of screen coord system
	lastFire = clock();
}

void Player::runLogic() {
	float friction = 0.05f;
	loc.x += vel.x;
	loc.y += vel.y;
	vel.x = vel.x - vel.x*friction;
	vel.y = vel.y - vel.y*friction;

	if(type == FRIENDLY) {
		//
	}
	else if(type == ENEMY) {
		// Chase mainPlayer
		float xSize, ySize, total;
		xSize = loc.x - mainPlayer->loc.x;
		ySize = loc.y - mainPlayer->loc.y;
		total = fabs(xSize)+fabs(ySize);

		velocity newVel;
		newVel.x = vel.x - speed*(xSize/total);
		newVel.y = vel.y - speed*(ySize/total);

		if(fabs(newVel.x) < maxSpeed)
			vel.x = newVel.x;
		if(fabs(newVel.y) < maxSpeed)
			vel.y = newVel.y;
	}
	else {}
}

void Player::collisionDetection() {
	if(loc.x < 0) {
		loc.x = 0;
		vel.x = 0;
	}
	if(loc.x > SCREEN_WIDTH-1) {
		loc.x = SCREEN_WIDTH-1;
		vel.x = 0;
	}
	if(loc.y < 0) {
		loc.y = 0;
		vel.y = 0;
	}
	if(loc.y > SCREEN_HEIGHT-1) {
		loc.y = SCREEN_HEIGHT-1;
		vel.y = 0;
	}

	if(type == FRIENDLY) {
		//
	}
	else if(type == ENEMY) {
		// Test against bullets
		for(int d=0;d<MAX_BULLETS;d++) {
			if(bullets[d] && bullets[d]->alive) {
				if(cmpVertex(loc, bullets[d]->loc)) {
					takeDamage(1);
					//alive = false;
					bullets[d]->alive = false;
				}
			}
		}

		// Test against mainPlayer
		if(cmpVertex(loc, mainPlayer->loc)) {
			mainPlayer->takeDamage(1);
		}

		// Test against other enemies
		for(int d=0;d<MAX_ENEMIES;d++) {
			if(enemies[d]) {
				if(enemies[d]->alive) {
					if(enemies[d] != this) {
						if(cmpVertex(loc, enemies[d]->loc)) {
							vel.x *= -1;
							vel.y *= -1;
							enemies[d]->vel.x *= -1;
							enemies[d]->vel.y *= -1;
						}
					}
				}
			}
		}
	}
	else {}
}

#endif
