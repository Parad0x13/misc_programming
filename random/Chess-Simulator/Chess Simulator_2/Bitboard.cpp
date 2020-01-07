#include "Bitboard.h"
#include "PieceArray.h"
#include "project-wide.h"

Bitboard::Bitboard() {
	//memset(data, 0, sizeof(data));
	memset(data, 0, sizeof(uint64_t) * 12);
}

PieceArray Bitboard::toPieceArray() {
	PieceArray retVal;
	for(int d=0;d<64;d++) {
		if(uint64_t(1)&(data[wPawns]>>d))retVal.data[d] = P;
		else if(uint64_t(1)&(data[wRooks]>>d))retVal.data[d] = R;
		else if(uint64_t(1)&(data[wKnights]>>d))retVal.data[d] = N;
		else if(uint64_t(1)&(data[wBishops]>>d))retVal.data[d] = B;
		else if(uint64_t(1)&(data[wQueens]>>d))retVal.data[d] = Q;
		else if(uint64_t(1)&(data[wKing]>>d))retVal.data[d] = K;
		else if(uint64_t(1)&(data[bPawns]>>d))retVal.data[d] = p;
		else if(uint64_t(1)&(data[bRooks]>>d))retVal.data[d] = r;
		else if(uint64_t(1)&(data[bKnights]>>d))retVal.data[d] = n;
		else if(uint64_t(1)&(data[bBishops]>>d))retVal.data[d] = b;
		else if(uint64_t(1)&(data[bQueens]>>d))retVal.data[d] = q;
		else if(uint64_t(1)&(data[bKing]>>d))retVal.data[d] = k;
	}
	return retVal;
}
