// Keyboard manager for DOS by Bryan Tremblay of Keikaku Studios
/*	Pass a key processing function to this manager and enjoy!
	Currently it cannot handle shift, alt, control, function keys	*/
/*	processFunction information:
		Virtual Keys for letters are the same as their ASCII counterparts
		Therefore they are not defined in windows.h
		Must use their Capped version too, lower 'a' won't work etc...	*/
// V1.1
#ifndef __DOSKEYBOARD__
#define __DOSKEYBOARD__

#include <windows.h>

#define MAX_VK_SIZE 256
class DOSKeyboard {
public:
	DOSKeyboard(void (*processFunction)(unsigned int *)) : processFunction(processFunction) {
		hIn = GetStdHandle(STD_INPUT_HANDLE);
		memset(keys, 0x00, MAX_VK_SIZE * sizeof(unsigned int));
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
		processFunction(keys);
	}
	void keyDown(unsigned int key) {
		if(keyDownFunction != NULL)keyDownFunction(key, keys[key]?true:false);
		keys[key] = true;
	}
	void keyUp(unsigned int key) {
		if(keyUpFunction != NULL)keyUpFunction(key);
		keys[key] = false;
	}
	void processInput_Record(INPUT_RECORD InRec) {
		if(InRec.EventType == KEY_EVENT) {
			unsigned int key = InRec.Event.KeyEvent.wVirtualKeyCode;
			if(InRec.Event.KeyEvent.bKeyDown)keyDown(key);
			else keyUp(key);
		}
	}
	void bindKeyDown(void (*keyDownFunction)(unsigned int, bool)) {
		this->keyDownFunction = keyDownFunction;
	}
	void bindKeyUp(void (*keyUpFunction)(unsigned int)) {
		this->keyUpFunction = keyUpFunction;
	}
	unsigned int keys[MAX_VK_SIZE];
	HANDLE hIn;
	void (*processFunction)(unsigned int *);
	void (*keyDownFunction)(unsigned int, bool);
	void (*keyUpFunction)(unsigned int);
};

#endif
