/*	Decided to create white and black move/attack bitmasks
	separately instead of dynamically for speedups	*/
/*	Decided against using this for now since I don't know how to
	handle ray-tracing pieces like pawn-push+2 or rook/bishop/queen	*/
#ifndef __BITBOARD
#define __BITBOARD

#include <iostream>

class PieceArray;

enum enumBitBoard {
	wPawns, wRooks, wKnights, wBishops, wQueens, wKing,
	bPawns, bRooks, bKnights, bBishops, bQueens, bKing,
};

// Bitboard indexes:
// wPawns, wRooks, wKnights, wBishops, wQueens, wKing
// bPawns, bRooks, bKnights, bBishops, bQueens, bKing
class Bitboard {
friend class PieceArray;
public:
	Bitboard();
	PieceArray toPieceArray();

	uint64_t data[12];
};

#endif
