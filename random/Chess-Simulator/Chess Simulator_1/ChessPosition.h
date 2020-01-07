#ifndef __CHESSPOSITION__
#define __CHESSPOSITION__

#include <iostream>

extern std::string fileNames;

typedef enum {
	MOVEMENT,
	ATTACK,
}POSITION_METADATA_TYPES;

class ChessPosition {
public:
	ChessPosition(int file, int rank) : file(file), rank(rank) {}
	ChessPosition(std::string input) {
		file = input[0] - 'a';
		rank = input[1] - '0' - 1;
	}
	bool isEqualTo(ChessPosition *b) {return ((file == b->file) && (rank == b->rank));}
	ChessPosition *copy() {
		ChessPosition *retVal = new ChessPosition(file, rank);
		return retVal;
	}
	std::string toParsableString();
	bool isOnBoard() {
		if(file >= 0 && file <= 7 && rank >= 0 && rank <= 7)return true;
		return false;
	}
	int file, rank;
	int metaData;
};

#endif
