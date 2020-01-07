/* 1010! Solver V1.0 by Bryan Tremblay 05Jul2015@1430MT
	So this solver was written to solve a game called 1010! for iPhone. The game is simple but I couldn't figure out
	how to make it more deterministic like you could with other games like 2048 or sudoku etc... I decided to try and
	make a brute solver and see how far it got. As of the time of this writing I've only ever followed it through one
	time and got a score barely under Maureen's highest score. It took me the better part of an entire day to achieve this.	*/
// Make sure to compile with the flag -std=c++11
// Memory Leaks out the Wazzooo!!!

/*	Mechanics of 1.0:
	Places pieces A B C in every order in every possible position.
	Score is calculated considering:
		- How many blocks are filled in after move
		- Likelyhood that a 3x3 block will be available next move
		- Likelyhood that a 5x1 block will be available next move
		- Likelyhood that a 1x5 block will be available next move
	Score starts high and is reduced according to the above searches. The lowest score is chosen as the next iteration	*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <climits>
#include <memory>

// These cutoffs determine how in depth the algorithm searches for available contiguous placement spots
// The larger the cutoffs the better the search, but the longer it takes
#define CUTOFF_3x3 3000
#define CUTOFF_L5H 3000
#define CUTOFF_L5V 3000

// Shape_Size_Direction
#define dot new Block("1")
#define square2 new Block("11.11")
#define square3 new Block("111.111.111")
#define line2h new Block("11")
#define line2v new Block("1.1")
#define line3h new Block("111")
#define line3v new Block("1.1.1")
#define line4h new Block("1111")
#define line4v new Block("1.1.1.1")
#define line5h new Block("11111")
#define line5v new Block("1.1.1.1.1")
#define l21 new Block("11.10")
#define l22 new Block("11.01")
#define l23 new Block("10.11")
#define l24 new Block("01.11")
#define l31 new Block("111.100.100")
#define l32 new Block("111.001.001")
#define l33 new Block("100.100.111")
#define l34 new Block("001.001.111")

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))elems.push_back(item);
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

template<class T>
void log(std::vector<T> &data) {
	for(std::string string : data)
		std::cout << string << std::endl;
}

int lengthOfString(std::string str) {
	return strlen(str.c_str());
}

using namespace std;

class Block {
public:
	string raw;
	vector<string>data;

	Block(string raw_input) {
		raw = raw_input;
		data = split(raw, '.');

		width = 0;
		for(string str : data)
			width = max(width, lengthOfString(str));
		height = data.size();
	}

	void render() {
		log(data);
	}

	int width, height;
};

class Board {
public:
	bool gameLost = false;
	vector<string>data;
	Block *lastPlacedBlock;
	int lastPlacedBlockX, lastPlacedBlockY;

	Board() {
		for(int d=0;d<10;d++)
			data.push_back("0000000000");
	}

	Board *copy() {
		Board *retVal = new Board();
		retVal->gameLost = gameLost;
		retVal->data = data;
		retVal->lastPlacedBlock = lastPlacedBlock;
		retVal->lastPlacedBlockX = lastPlacedBlockX;
		retVal->lastPlacedBlockY = lastPlacedBlockY;
		return retVal;
	}

	bool place(Block *block, int x_delta, int y_delta) {
		bool canPlace = true;
		vector<string>newLines;

		for(int y=0;y<block->height;y++) {
			// Check if goes beyond board limits
			if(y + y_delta >= 10) {
				canPlace = false;
				break;
			}
			string blockLine = block->data[y];
			string boardLine = data[y + y_delta];
			for(int x=0;x<lengthOfString(blockLine);x++) {
				// Check if goes beyond board limits
				if(x + x_delta >= 10) {
					canPlace = false;
					break;
				}

				if(boardLine[x + x_delta] == '1' && blockLine[x] == '1')  {
					canPlace = false;
					break;
				}

				if(blockLine[x] == '1')
					boardLine[x + x_delta] = blockLine[x];
			}

			if(canPlace)
				newLines.push_back(boardLine);
			else break;
		}

		if(canPlace) {
			for(int y=0;y<newLines.size();y++)
				data[y + y_delta] = newLines[y];
		}
		else {
			gameLost = true;
			return false;
		}

		// Need to clear full row/colums
		vector<int>rowsToClear;
		vector<int>columnsToClear;
		// Check Rows
		for(int y=0;y<10;y++) {
			string line = data[y];
			bool allFull = true;
			for(int x=0;x<lengthOfString(line);x++) {
				if(line[x] != '1') {
					allFull = false;
					break;
				}
			}
			if(allFull)
				rowsToClear.push_back(y);
		}
		// Check Columns
		for(int x=0;x<10;x++) {
			bool allFull = true;
			for(int y=0;y<10;y++) {
				string line = data[y];
				if(line[x] != '1') {
					allFull = false;
					break;
				}
			}
			if(allFull)
				columnsToClear.push_back(x);
		}

		for(int row : rowsToClear)clearRow(row);
		for(int column : columnsToClear)clearColumn(column);

		lastPlacedBlock = block;
		lastPlacedBlockX = x_delta;
		lastPlacedBlockY = y_delta;

		return true;
	}

	bool optimizePlace(Block *block) {
		vector<int>bestLoc = this->bestLocationFor(block);
		bool canPlace = this->place(block, bestLoc[0], bestLoc[1]);
		return canPlace;
	}

	Board *optimizePlace(vector<Block *> blocks) {
		Block *a = blocks[0];
		Block *b = blocks[1];
		Block *c = blocks[2];

		Board *retVal, *board;
		int bestScore = INT_MAX, testScore;

		string order;
		vector<Block *> blocksInOrder;
		blocksInOrder.push_back(a);
		blocksInOrder.push_back(b);
		blocksInOrder.push_back(c);

		// a b c
		board = this->copy();
		board->optimizePlace(a);board->optimizePlace(b);board->optimizePlace(c);
		testScore = board->score();
		if(testScore < bestScore && !board->gameLost){
			retVal = board;
			bestScore = testScore;
			order = "abc";
			blocksInOrder[0] = a;
			blocksInOrder[1] = b;
			blocksInOrder[2] = c;
		}

		// a c b
		board = this->copy();
		board->optimizePlace(a);board->optimizePlace(c);board->optimizePlace(b);
		testScore = board->score();
		if(testScore < bestScore && !board->gameLost){
			retVal = board;
			bestScore = testScore;
			order = "acb";
			blocksInOrder[0] = a;
			blocksInOrder[1] = c;
			blocksInOrder[2] = b;
		}

		// b a c
		board = this->copy();
		board->optimizePlace(b);board->optimizePlace(a);board->optimizePlace(c);
		testScore = board->score();
		if(testScore < bestScore && !board->gameLost){
			retVal = board;
			bestScore = testScore;
			order = "bac";
			blocksInOrder[0] = b;
			blocksInOrder[1] = a;
			blocksInOrder[2] = c;
		}

		// b c a
		board = this->copy();
		board->optimizePlace(b);board->optimizePlace(c);board->optimizePlace(a);
		testScore = board->score();
		if(testScore < bestScore && !board->gameLost){
			retVal = board;
			bestScore = testScore;
			order = "bca";
			blocksInOrder[0] = b;
			blocksInOrder[1] = c;
			blocksInOrder[2] = a;
		}

		// c a b
		board = this->copy();
		board->optimizePlace(c);board->optimizePlace(a);board->optimizePlace(b);
		testScore = board->score();
		if(testScore < bestScore && !board->gameLost){
			retVal = board;
			bestScore = testScore;
			order = "cab";
			blocksInOrder[0] = c;
			blocksInOrder[1] = a;
			blocksInOrder[2] = b;
		}

		// c b a
		board = this->copy();
		board->optimizePlace(c);board->optimizePlace(b);board->optimizePlace(a);
		testScore = board->score();
		if(testScore < bestScore && !board->gameLost){
			retVal = board;
			bestScore = testScore;
			order = "cba";
			blocksInOrder[0] = c;
			blocksInOrder[1] = b;
			blocksInOrder[2] = a;
		}

		if(lengthOfString(order) > 0) {
			cout << "Found best order to be " << order << endl;
			board = this->copy();

			vector<int>bestLoc;

			cout << "Place " << order[0] << endl;
			bestLoc = board->bestLocationFor(blocksInOrder[0]);
			cout << "Location of " << order[0] << " is " << bestLoc[0] << ", " << bestLoc[1] << endl;
			board->optimizePlace(blocksInOrder[0]);
			board->render();

			cout << "Place " << order[1] << endl;
			bestLoc = board->bestLocationFor(blocksInOrder[1]);
			cout << "Location of " << order[1] << " is " << bestLoc[0] << ", " << bestLoc[1] << endl;
			board->optimizePlace(blocksInOrder[1]);
			board->render();

			cout << "Place " << order[2] << endl;
			bestLoc = board->bestLocationFor(blocksInOrder[2]);
			cout << "Location of " << order[2] << " is " << bestLoc[0] << ", " << bestLoc[1] << endl;
			board->optimizePlace(blocksInOrder[2]);
			board->render();
		}
		else
			retVal = NULL;

		// In the event that no placement combination works at all we should lose the game
		// This should be caught however, for example the initial board
		//		should never run because it failed in the first place
		//	However it's better just to check just in case
		if(retVal == NULL) {
			retVal = this->copy();
			retVal->gameLost = true;
		}

		return retVal;
	}

	void clearRow(int row) {
		data[row] = "0000000000";
	}

	void clearColumn(int column) {
		for(int y=0;y<10;y++) {
			string line = data[y];
			line[column] = '0';
			data[y] = line;
		}
	}

	vector<int>bestLocationFor(Block *block) {
		vector<int>retVal;
		retVal.push_back(0);
		retVal.push_back(0);

		int bestScore = INT_MAX;
		for(int y=0;y<10;y++) {
			for(int x=0;x<10;x++) {
				unique_ptr<Board> boardCopy(this->copy());
				if(boardCopy->place(block, x,  y)) {
					int newScore = boardCopy->score();
					if(newScore < bestScore) {
						bestScore = newScore;
						retVal[0] = x;
						retVal[1] = y;
					}
				}
				else {
					//
				}
			}
		}

		return retVal;
	}

	// The lower the score the better
	// Score start high and certain situations reduce the score
	int score() {
		int retVal = 1000000;
		for(string line : data) {
			for(int x=0;x<lengthOfString(line);x++) {
				if(line[x] == '1')
					retVal++;
			}
		}

		int square3PlacementAvailability = 0;
		for(int y=0;y<10;y++) {
			for(int x=0;x<10;x++) {
				unique_ptr<Board> test(this->copy());
				if(test->place(square3, x, y))
					square3PlacementAvailability++;
				if(square3PlacementAvailability >= CUTOFF_3x3)break;
			}
			if(square3PlacementAvailability >= CUTOFF_3x3)break;
		}
		retVal -= square3PlacementAvailability*100;

		int line5hPlacementAvailability = 0;
		for(int y=0;y<10;y++) {
			for(int x=0;x<10;x++) {
				unique_ptr<Board> test(this->copy());
				if(test->place(line5h, x, y))
					line5hPlacementAvailability++;
				if(line5hPlacementAvailability >= CUTOFF_L5H)break;
			}
			if(line5hPlacementAvailability >= CUTOFF_L5H)break;
		}
		retVal -= line5hPlacementAvailability*100;

		int line5vPlacementAvailability = 0;
		for(int y=0;y<10;y++) {
			for(int x=0;x<10;x++) {
				unique_ptr<Board> test(this->copy());
				if(test->place(line5v, x, y))
					line5vPlacementAvailability++;
				if(line5vPlacementAvailability >= CUTOFF_L5V)break;
			}
			if(line5vPlacementAvailability >= CUTOFF_L5V)break;
		}
		retVal -= line5vPlacementAvailability*100;

		return retVal;
	}

	void render() {
		Board *lastPlacedBlock_BoardRender;
		if(lastPlacedBlock != NULL) {
			lastPlacedBlock_BoardRender = new Board();
			lastPlacedBlock_BoardRender->place(lastPlacedBlock, lastPlacedBlockX, lastPlacedBlockY);
		}

		for(int y=0;y<10;y++) {
			string line = data[y];
			for(int x=0;x<10;x++) {
				char value = line[x];

				if(lastPlacedBlock_BoardRender != NULL) {
					string otherLine = lastPlacedBlock_BoardRender->data[y];
					char otherValue = otherLine[x];

					if(value == '1' && otherValue == '1')
						value = (char)219;
				}

				if(value == '1')
					value = (char)178;
				else if(value == '0')
					value = '.';

				cout << value;
			}
			cout << endl;
		}
	}
};

vector<Block *>blocksFrom3Inputs(string input) {
	vector<Block *>retVal;

	vector<string> inputs = split(input, ' ');

	for(string str : inputs) {
		if(str.compare("dot") == 0)retVal.push_back(dot);
		if(str.compare("square2") == 0)retVal.push_back(square2);
		if(str.compare("square3") == 0)retVal.push_back(square3);
		if(str.compare("line2h") == 0)retVal.push_back(line2h);
		if(str.compare("line2v") == 0)retVal.push_back(line2v);
		if(str.compare("line3h") == 0)retVal.push_back(line3h);
		if(str.compare("line3v") == 0)retVal.push_back(line3v);
		if(str.compare("line4h") == 0)retVal.push_back(line4h);
		if(str.compare("line4v") == 0)retVal.push_back(line4v);
		if(str.compare("line5h") == 0)retVal.push_back(line5h);
		if(str.compare("line5v") == 0)retVal.push_back(line5v);
		if(str.compare("l21") == 0)retVal.push_back(l21);
		if(str.compare("l22") == 0)retVal.push_back(l22);
		if(str.compare("l23") == 0)retVal.push_back(l23);
		if(str.compare("l24") == 0)retVal.push_back(l24);
		if(str.compare("l31") == 0)retVal.push_back(l31);
		if(str.compare("l32") == 0)retVal.push_back(l32);
		if(str.compare("l33") == 0)retVal.push_back(l33);
		if(str.compare("l34") == 0)retVal.push_back(l34);
	}

	return retVal;
}

vector<Block *>get3Input() {
	string input;
	cout << "Enter 3 Inputs" << endl;
	getline(cin, input);

	ofstream logfile;
	logfile.open("logfile.txt", ios_base::app);
	logfile << input << endl;
	logfile.close();

	return blocksFrom3Inputs(input);
}

int main() {
	Board *board = new Board();

	vector<vector<Block *>>blockVectorsToRun;

	if(ifstream("logfile.txt")) {
		cout << "Already Exists" << endl;
		ifstream t("logfile.txt");
		stringstream buffer;
		buffer << t.rdbuf();
		string str = buffer.str();

		vector<string>lines = split(str, '\n');

		if(lines[0].compare("Board") == 0) {
			lines.erase(lines.begin());
			string rawBoard = lines[0];
			lines.erase(lines.begin());
			board->data = split(rawBoard, '.');
		}

		for(string line : lines) {
			vector<Block *>blocks = blocksFrom3Inputs(line);

			blockVectorsToRun.push_back(blocks);
		}
	}

	for(;;) {
		vector<Block *>toRun;

		if(blockVectorsToRun.size() > 0) {
			toRun = blockVectorsToRun[0];
			blockVectorsToRun.erase(blockVectorsToRun.begin());
		}
		else
			toRun = get3Input();

		cout << "Trying to add these blocks:" << endl;
		for(Block *block : toRun) {
			block->render();
			cout << endl;
		}

		board = board->optimizePlace(toRun);
		if(!board->gameLost)
			cout << endl;
		else {
			cout << "Game Lost" << endl;
			break;
		}
	}

	return false;
}
