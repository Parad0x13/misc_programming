#include "project-wide.h"

using namespace std;

int squareXIndex(int square) {
	return square - squareYIndex(square)*8;
}

int squareYIndex(int square) {
	return square/8;
}

int translateSquare(int startingSquare, int xDelta, int yDelta) {
	int startYIndex = squareYIndex(startingSquare);
	int startXIndex = squareXIndex(startingSquare);	// [TODO]: Fix redundant maths

	int newXIndex = startXIndex + xDelta;
	int newYIndex = startYIndex + yDelta;
	if(newXIndex < 0 || newXIndex > 7 || newYIndex < 0 || newYIndex > 7)
		return offBoard;

	return newYIndex*8 + newXIndex;
}

// Relies on enumPieces
bool isEnemy(int a, int b) {
	if(a > 0 && a < 7 && b > 6 && b < 13)return true;
	else if(b > 0 && b < 7 && a > 6 && a < 13)return true;
	return false;
}

// Relies on enumPieces
bool isCorrectColor(int color, int piece) {
	if(color == WHITE && (piece > 0 && piece < 7))return true;
	else if(color == BLACK && (piece > 6 && piece < 13))return true;
	return false;
}

bool isWhite(int piece) {
	if(isCorrectColor(WHITE, piece))return true;
	return false;
}

bool isBlack(int piece) {
	if(isCorrectColor(BLACK, piece))return true;
	return false;
}
