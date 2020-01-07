#ifndef __INPUTHANDLER__
#define __INPUTHANDLER__

#include "main.h"

class InputHandler {
public:
	InputHandler();
	~InputHandler();

	string inputQueue[10];
	string currentInput;
	int inputQueueCount;

	void awaitInput();
	void parseInput();
	void processInput();
};

#endif
