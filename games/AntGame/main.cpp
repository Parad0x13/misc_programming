#include "main.h"

HANDLE hIn, hOut;

int main() {
	initProgram();
	runGame();
	return false;
}

void initProgram() {
	initWinHandles();
	initKB();
	screen = new ScreenBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void initWinHandles() {
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}
