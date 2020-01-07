#ifndef __PIECEARRAY
#define __PIECEARRAY

#include <string.h>

class Bitboard;

class PieceArray {
friend class Bitboard;
public:
	PieceArray();
	Bitboard toBitboard();
	int pieceAt(int square);
	void setPiece(int square, int value);

private:
	int data[64];
};

#endif
