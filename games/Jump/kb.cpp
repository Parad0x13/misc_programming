#include "kb.h"
#include "constants.h"
#include "object.h"

unsigned int Keys[MAX_VK_SIZE];
bool jumping;

void initKB() {
	for(int d=0;d<MAX_VK_SIZE;d++)
		Keys[d] = 0x0000;
}

void addKBHold(unsigned int key) {
	if(!keyPressed(key))
		Keys[key] = true;
}

void removeKBHold(unsigned int key) {
	if(keyPressed(key))
		Keys[key] = false;
}

int keyPressed(unsigned int key) {
	int index = (int)key;
	if(Keys[index])
		return true;
	return false;
}

void processInput() {
	INPUT_RECORD InRec;
	DWORD EventCount, NumRead;

	GetNumberOfConsoleInputEvents(hIn, &EventCount);
	while(EventCount > 0) {
		ReadConsoleInput(hIn, &InRec, 1, &NumRead);
		processInput_Record(InRec);
		GetNumberOfConsoleInputEvents(hIn, &EventCount);
	}
	processKeyPresses();
}

void processInput_Record(INPUT_RECORD InRec) {
	if(InRec.EventType == KEY_EVENT) {
		unsigned int key = InRec.Event.KeyEvent.wVirtualKeyCode;
		if(InRec.Event.KeyEvent.bKeyDown)
			addKBHold(key);
		else
			removeKBHold(key);
	}
}

// Virtual Keys for letters are the same as their ASCII counterparts
// Therefore they are not defined in windows.h
// Must use their Capped version too, lower 'a' won't work etc...
void processKeyPresses() {
	if(keyPressed(VK_ESCAPE))
		GameActive = false;

	if(keyPressed('W')) {
		if(!jumping) {
//			player->loc.y -= 1.0f;
//			player->vel.y -= GRAVITY/4;
			//player->vel.y -= GRAVITY;
			player->accel.y = -GRAVITY;
		}
		jumping = true;
	}
	else
		jumping = false;

	if(keyPressed('A')) {
		player->accel.x -= 2.0f;
//		player->vel.x -= 2.0f;
//		player->loc.x -= 1.0f;
	}

	if(keyPressed('D')) {
		player->accel.x += 2.0f;
//		player->vel.x += 2.0f;
//		player->loc.x += 1.0f;
	}
}
