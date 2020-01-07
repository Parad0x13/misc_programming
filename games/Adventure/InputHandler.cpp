#include "InputHandler.h"

InputHandler::InputHandler() {
	//
}

void InputHandler::awaitInput() {
	char stuff[50];

	printf("Input Command: \n");

	fgets(stuff, 20, stdin);
	printf("is: %s\n", stuff);

	// Cant use scanf because of the space formatting
//	fgets(currentInput, 20, stdin);

	// Remove trailing newline, if there
//	if(currentInput[strlen(currentInput) - 1] == EOL)
//		currentInput[strlen(currentInput) - 1] = 0x00;
}

void InputHandler::parseInput() {
	char *p;

	inputQueueCount = 0;
	p = strtok(currentInput, " ");

	while(p != NULL) {
		inputQueue[inputQueueCount] = p;
		inputQueueCount++;
		p = strtok(NULL, " ");
	}
}

void InputHandler::processInput() {
	printf("kejrkerjekrjek");
}
