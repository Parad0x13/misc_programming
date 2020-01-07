#ifndef __GAMELOGIC__
#define __GAMELOGIC__

#define TERMINAL_VELOCITY	20.0f

clock_t physicsLast = 0;

void applyGravity(Actor *actor) {
	clock_t now = clock();
	float delta = now-physicsLast;
	delta /= 1000;

	actor->testVel = actor->vel;

	// V2 = V1 + A*DeltaT
	actor->testVel.x = actor->testVel.x;
	actor->testVel.y = actor->testVel.y + -9.81*delta;

	if(fabs(actor->testVel.y) > TERMINAL_VELOCITY) {
		if(actor->testVel.y < 0)
			actor->testVel.y = -TERMINAL_VELOCITY;
		else
			actor->testVel.y = -TERMINAL_VELOCITY;
	}
	if(fabs(actor->testVel.x) > TERMINAL_VELOCITY) {
		if(actor->testVel.x < 0)
			actor->testVel.x = -TERMINAL_VELOCITY;
		else
			actor->testVel.x = -TERMINAL_VELOCITY;
	}
}

void move(Actor *actor) {
	clock_t now = clock();
	float delta = now-physicsLast;
	delta /= 1000;

	actor->testLoc = actor->loc;

	actor->testLoc.x = actor->testLoc.x - actor->testVel.x*delta;
	actor->testLoc.y = actor->testLoc.y - actor->testVel.y*delta;
}

void chunkLogic() {
	for(int d=0;d<activeChunkCount;d++) {
		for(int dd=0;dd<MAX_ACTORS;dd++) {
			Actor *actor = activeChunks[d]->actors[dd];
			if(actor->alive) {
				applyGravity(actor);
				move(actor);
				Item *testCollision = actorCollidesWithItem(actor);
				if(testCollision == NULL) {
					actor->vel = actor->testVel;
					actor->loc = actor->testLoc;
				}
				else {
					actor->vel = stillVelocity();
				}
			}
		}
	}
	physicsLast = clock();
}

void gameLogic() {
	chunkLogic();
}

#endif
