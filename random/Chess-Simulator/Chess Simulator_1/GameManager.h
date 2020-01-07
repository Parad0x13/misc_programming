#ifndef __GAMEMANAGER__
#define __GAMEMANAGER__

#include <iostream>
#include <vector>

#define BLACK 0
#define WHITE 1

class ChessMove;

void sleep(unsigned int mseconds);

class GameManager {
public:
	GameManager() {
		fileName = "game.txt";
		simulatorName = "Simulator1";
	}

	void readCommands();
	void writeCommand(std::string command);
	void logCommands() {for(int d=0;d<commands.size();d++)std::cout << commands[d] << std::endl;}
	void annotateMove(ChessMove *move);
	void processCommand(std::string command) {processCommand_callback(commands.back());}
	void tick();

	int playerColor;
	void (*processCommand_callback)(std::string);
	std::string fileName, simulatorName;
	std::vector<std::string> prevCommands, commands;
};

#endif
