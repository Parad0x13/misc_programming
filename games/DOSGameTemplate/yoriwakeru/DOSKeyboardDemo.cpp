#include <stdio.h>
#include "DOSKeyboard.h"

DOSKeyboard* keyboard;
void processKeyPresses(unsigned int* keys);
void processKeyDown(unsigned int key);
void processKeyUp(unsigned int key);

bool gameActive;

int main() {
	keyboard = new DOSKeyboard(processKeyPresses);
	keyboard->bindKeyDown(processKeyDown);
	keyboard->bindKeyUp(processKeyUp);

	gameActive = true;
	while(gameActive) {
		keyboard->processInput();
	}

	return false;
}

void processKeyPresses(unsigned int* keys) {
	if(keys[VK_ESCAPE])gameActive = false;
}

void processKeyDown(unsigned int key) {
	printf("Pressed Key Down: %c\n", key);
}

void processKeyUp(unsigned int key) {
	printf("Pressed Key Up: %c\n", key);
}
