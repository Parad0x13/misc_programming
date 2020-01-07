#include <fstream>
#include <ctime>
#include "GameManager.h"
#include "ChessMove.h"

using namespace std;

void sleep(unsigned int mseconds) {
	clock_t goal = mseconds + clock();
	while(goal > clock());
}

void GameManager::readCommands() {
	ifstream fileIn;
	fileIn.open(fileName.c_str());
	commands.clear();
	if(fileIn.is_open()) {
		string line;
		while(getline(fileIn, line))commands.push_back(line);
	}
	else cout << "Unable to open input file" << endl;
	fileIn.close();
}

void GameManager::writeCommand(string command) {
	ofstream fileOut;
	fileOut.open(fileName.c_str(), ios::app);
	if(fileOut.is_open()) {
		fileOut << command << endl;
	}
	else cout << "Unable to create output file" << endl;
	fileOut.close();
}

void GameManager::annotateMove(ChessMove *move) {
	writeCommand(move->toParsableString());
}

void GameManager::tick() {
	readCommands();
	if(commands.size() > prevCommands.size()) {
		if(commands.size() - prevCommands.size() > 1)
			cout << "Note: More than one command was entered since last command processing, only processing last entered command" << endl;
		prevCommands = commands;
		processCommand(commands.back());
	}
}
