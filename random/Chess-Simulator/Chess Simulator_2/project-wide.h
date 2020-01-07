#ifndef __PROJECTWIDE
#define __PROJECTWIDE

#define SHOULD_LOG false
#define LOG if(SHOULD_LOG){cout << this << " BEGIN " << __FUNCTION__ << endl;}

#include <iostream>
#include <string>

const uint64_t DarkSquares = 0xAA55AA55AA55AA55;

/*
	Data is mapped little endian so that the last bit in unsigned64_t is A1
	Birds eye view from white's perspective is:
	56 57 58 59 60 61 62 63
	48 49 50 51 52 53 54 55
	40 41 42 43 44 45 46 47
	32 33 34 35 36 37 38 39
	24 25 26 27 28 29 30 31
	16 17 18 19 20 21 22 23
	08 09 10 11 12 13 14 15
	00 01 02 03 04 05 06 07
*/
enum enumSquares {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,
	offBoard
};

// [TODO]: Refactor this to use a struct or something
// Right now it's split to decrease potential calculations
int squareXIndex(int square);
int squareYIndex(int square);
int translateSquare(int startingSquare, int xDelta, int yDelta);

const std::string squareNames[64] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

enum enumColors {
	BLACK, WHITE
};

enum enumPieces {
	Empty, P, R, N, B, Q, K, p, r, n, b, q, k, ENPASSANT
};

bool isEnemy(int a, int b);
bool isCorrectColor(int color, int piece);
bool isWhite(int piece);
bool isBlack(int piece);

const std::string pieceIcons = ".PRNBQKprnbqkE";

#endif
