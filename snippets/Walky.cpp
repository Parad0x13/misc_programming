#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

using namespace std;

#define GAME_WIDTH	80
#define GAME_HEIGHT	24

int playerX = 5, playerY = 5;

string buffer;

void clearScreen() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

render() {
	buffer = "";
	clearScreen();
	for(int vy=0;vy<GAME_HEIGHT;vy++) {
		for(int vx=0;vx<GAME_WIDTH;vx++) {
			if(vx == playerX && vy == playerY)
				buffer += "$";
			else
				buffer += ".";
		}
		buffer += "\n";
	}
	cout << buffer;
}

int main() {
	for(;;) {
		render();
		char input = getch();
		if(input == 'w')playerY--;
		if(input == 's')playerY++;
		if(input == 'a')playerX--;
		if(input == 'd')playerX++;
		if(input == 0x1B)return 0;
	}
}
