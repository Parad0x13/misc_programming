#include "render.h"
#include "graphics.h"
#include "object.h"

void render() {
	clearBuffer();

	renderWorld();

	swapBuffers();

	renderPlayerVelocity();
}

void renderWorld() {
	int itemCount = world->objects.size();
	for(int d=0;d<itemCount;d++) {
		Object *object = world->objects[d];
		int screenX, screenY;
		screenX = cameraFocus.x + (int)object->loc.x + SCREEN_WIDTH/2;
		screenY = cameraFocus.y + (int)object->loc.y + SCREEN_HEIGHT/2;
		if(!vertexiOutsideScreen(vertexiMake(screenX, screenY))) {
			setBufferxy(screenX, screenY, object->icon, object->color);
		}
	}
	// Re render player
	Object *object = player;
	int screenX, screenY;
	screenX = cameraFocus.x + (int)object->loc.x + SCREEN_WIDTH/2;
	screenY = cameraFocus.y + (int)object->loc.y + SCREEN_HEIGHT/2;
	if(!vertexiOutsideScreen(vertexiMake(screenX, screenY))) {
		setBufferxy(screenX, screenY, object->icon, object->color);
	}
}

void renderPlayerVelocity() {
	gotoxy(0, 0);
	printf("loc:\t\t(%f, %f)", player->loc.x, player->loc.y);
	printf("\n");
	printf("prevLoc:\t(%f, %f)", player->prevLoc.x, player->prevLoc.y);
	printf("\n");
	printf("accel:\t\t(%f, %f)", player->accel.x, player->accel.y);
	printf("\n");
	printf("prevAccel:\t(%f, %f)", player->prevAccel.x, player->prevAccel.y);
	printf("\n");
	printf("vel:\t\t(%f, %f)", player->vel.x, player->vel.y);
	printf("\n");
	printf("prevVel:\t(%f, %f)", player->prevVel.x, player->prevVel.y);
	printf("\n");
}
