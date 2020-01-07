#include "PieceArray.h"
#include "Bitboard.h"
#include "project-wide.h"

#include <iostream>
#include <bitset>
using namespace std;

PieceArray::PieceArray() {
	//memset(data, 0, sizeof(data));
	memset(data, 0, sizeof(int) * 64);
}

Bitboard PieceArray::toBitboard() {
	Bitboard retVal;
	for(int d=0;d<64;d++) {
		if(data[d] == P)retVal.data[wPawns] |= uint64_t(1)<<d;
		else if(data[d] == R)retVal.data[wRooks] |= uint64_t(1)<<d;
		else if(data[d] == N)retVal.data[wKnights] |= uint64_t(1)<<d;
		else if(data[d] == B)retVal.data[wBishops] |= uint64_t(1)<<d;
		else if(data[d] == Q)retVal.data[wQueens] |= uint64_t(1)<<d;
		else if(data[d] == K)retVal.data[wKing] |= uint64_t(1)<<d;
		else if(data[d] == p)retVal.data[bPawns] |= uint64_t(1)<<d;
		else if(data[d] == r)retVal.data[bRooks] |= uint64_t(1)<<d;
		else if(data[d] == n)retVal.data[bKnights] |= uint64_t(1)<<d;
		else if(data[d] == b)retVal.data[bBishops] |= uint64_t(1)<<d;
		else if(data[d] == q)retVal.data[bQueens] |= uint64_t(1)<<d;
		else if(data[d] == k)retVal.data[bKing] |= uint64_t(1)<<d;
	}
	return retVal;
}

int PieceArray::pieceAt(int square) {
	if(square == offBoard)
		return offBoard;
	return data[square];
}

void PieceArray::setPiece(int square, int value) {
	if(square != offBoard)
		data[square] = value;
}
