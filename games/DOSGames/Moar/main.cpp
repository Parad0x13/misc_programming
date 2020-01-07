/* Moar, a game about collecting items, and leveling your smithing skill! */
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <math.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define SCREEN_RESOLUTION (SCREEN_WIDTH*SCREEN_HEIGHT)
#define MAX_VK_SIZE 256

HANDLE hIn;
HANDLE hOut;

#include "items.h"
#include "actor.h"
#include "chunk.h"
#include "render.h"
#include "collisionDetection.h"
#include "gameLogic.h"

void initWinHandles();

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

unsigned int Keys[MAX_VK_SIZE];

bool GameActive = true;
bool jumping = false;

using namespace std;

int main(int argc, char *argv[]) {
	initWinHandles();
	initKB();

	generateWorld();

	while(GameActive) {
		processInput();
		render();
		gameLogic();
		Sleep(45);
	}

	return EXIT_SUCCESS;
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

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
			if(player->vel.y == 0)
				player->vel.y += 5.0f;
			jumping = true;
		}
	}
	else
		jumping = false;

	if(keyPressed('A')) {
		player->vel.x += 1.0f;
	}
}
