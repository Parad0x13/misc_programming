#include <vector>
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "GameManager.h"
#include "ChessPosition.h"

using namespace std;

char ChessPiece::icon() {
	if(type == P && color == WHITE)return 'P';
	if(type == P && color == BLACK)return 'p';
	if(type == N && color == WHITE)return 'N';
	if(type == N && color == BLACK)return 'n';
	if(type == B && color == WHITE)return 'B';
	if(type == B && color == BLACK)return 'b';
	if(type == R && color == WHITE)return 'R';
	if(type == R && color == BLACK)return 'r';
	if(type == Q && color == WHITE)return 'Q';
	if(type == Q && color == BLACK)return 'q';
	if(type == K && color == WHITE)return 'K';
	if(type == K && color == BLACK)return 'k';
	return '?';
}

void ChessPiece::render() {
	cout << icon();
}

void ChessPiece::log() {
	if(color == BLACK)cout << "Black ";
	else cout << "White ";
	if(type == P)cout << "Pawn";
	else if(type == N)cout << "Knight ";
	else if(type == B)cout << "Bishop ";
	else if(type == R)cout << "Rook ";
	else if(type == Q)cout << "Queen ";
	else if(type == K)cout << "King ";
	char letter = fileNames[pos->file];
	cout << letter << (pos->rank+1) << endl;
}

vector<ChessPosition *> ChessPiece::possibleMovementPositionsOnBoard(ChessBoard *board) {
	vector<ChessPosition *> retVal;
	if(type == P) {
		int direction;
		if(color == WHITE)direction = 1;
		if(color == BLACK)direction = -1;
		// Directly ahead
		bool canPlace = true;
		ChessPosition *_pos = pos->copy();
		_pos->rank += 1*direction;
		if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
		if(!_pos->isOnBoard())canPlace = false;
		_pos->metaData = MOVEMENT;
		if(canPlace)retVal.push_back(_pos);
		// Directly ahead twice on first move
		canPlace = true;
		_pos = pos->copy();
		_pos->rank += 2*direction;
		if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
		if(!_pos->isOnBoard())canPlace = false;
		if(color == WHITE && pos->rank != 1)canPlace = false;		// Cannot move forward twice if already moved
		if(color == BLACK && pos->rank != 6)canPlace = false;		// ^^
		_pos->metaData = MOVEMENT;
		if(canPlace)retVal.push_back(_pos);
		// Diagonally forward left if occupied by enemy piece
		ChessPiece *occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 1, -1, direction);
		if(occupant && occupant->color != color) {
			canPlace = true;
			_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Diagonally forward right if occupied by enemy piece
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 1, 1, direction);
		if(occupant && occupant->color != color) {
			canPlace = true;
			_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
	}
	else if(type == N) {
		// -2, 1
		ChessPosition *_pos = pos->copy();
		_pos->file += -2;
		_pos->rank += 1;
		ChessPiece *occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// -1, 2
		_pos = pos->copy();
		_pos->file += -1;
		_pos->rank += 2;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// 1, 2
		_pos = pos->copy();
		_pos->file += 1;
		_pos->rank += 2;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// 2, 1
		_pos = pos->copy();
		_pos->file += 2;
		_pos->rank += 1;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// 2, -1
		_pos = pos->copy();
		_pos->file += 2;
		_pos->rank += -1;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// 1, -2
		_pos = pos->copy();
		_pos->file += 1;
		_pos->rank += -2;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// -1, -2
		_pos = pos->copy();
		_pos->file += -1;
		_pos->rank += -2;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
		// -2, -1
		_pos = pos->copy();
		_pos->file += -2;
		_pos->rank += -1;
		occupant = board->pieceAt(_pos);
		if(_pos->isOnBoard()) {
			if(occupant != NULL)_pos->metaData = ATTACK;
			else _pos->metaData = MOVEMENT;
			bool shouldAdd = true;
			if(occupant != NULL && occupant->color == color)shouldAdd = false;
			if(shouldAdd)retVal.push_back(_pos);
		}
	}
	else if(type == B) {
		// Movement Diagonally Up Left
		int xDirection = -1, yDirection = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Up Right
		xDirection = 1, yDirection = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Down Left
		xDirection = -1, yDirection = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Down Right
		xDirection = 1, yDirection = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Attack Diagonally Up Left
		ChessPiece *occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, -1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Up Right
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, 1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Down Right
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, 1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Down Left
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, -1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
	}
	else if(type == R) {
		// Movement Horizontally Left
		int direction = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*direction;
			if(!board->emptyDirectHorizontalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Horizontally Right
		direction = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*direction;
			if(!board->emptyDirectHorizontalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Vertically Up
		direction = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->rank += d*direction;
			if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Vertically Down
		direction = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->rank += d*direction;
			if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Attack Left
		ChessPiece *occupant = board->firstHorizontallyCollidedPieceWithinRange(pos, 8, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Right
		occupant = board->firstHorizontallyCollidedPieceWithinRange(pos, 8, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Up
		occupant = board->firstVerticallyCollidedPieceWithinRange(pos, 8, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Down
		occupant = board->firstVerticallyCollidedPieceWithinRange(pos, 8, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
	}
	else if(type == Q) {
		// Movement Diagonally Up Left
		int xDirection = -1, yDirection = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Up Right
		xDirection = 1, yDirection = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Down Left
		xDirection = -1, yDirection = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Down Right
		xDirection = 1, yDirection = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Horizontally Left
		int direction = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*direction;
			if(!board->emptyDirectHorizontalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Horizontally Right
		direction = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*direction;
			if(!board->emptyDirectHorizontalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Vertically Up
		direction = 1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->rank += d*direction;
			if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Vertically Down
		direction = -1;
		for(int d=1;d<8;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->rank += d*direction;
			if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Attack Diagonally Up Left
		ChessPiece *occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, -1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Up Right
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, 1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Down Right
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, 1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Down Left
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 8, -1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Left
		occupant = board->firstHorizontallyCollidedPieceWithinRange(pos, 8, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Right
		occupant = board->firstHorizontallyCollidedPieceWithinRange(pos, 8, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Up
		occupant = board->firstVerticallyCollidedPieceWithinRange(pos, 8, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Down
		occupant = board->firstVerticallyCollidedPieceWithinRange(pos, 8, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
	}
	else if(type == K) {
		// Movement Diagonally Up Left
		int xDirection = -1, yDirection = 1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Up Right
		xDirection = 1, yDirection = 1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Down Left
		xDirection = -1, yDirection = -1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Diagonally Down Right
		xDirection = 1, yDirection = -1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*xDirection;
			_pos->rank += d*yDirection;
			if(!board->emptyDirectDiagonalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Horizontally Left
		int direction = -1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*direction;
			if(!board->emptyDirectHorizontalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Horizontally Right
		direction = 1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->file += d*direction;
			if(!board->emptyDirectHorizontalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Vertically Up
		direction = 1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->rank += d*direction;
			if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Movement Vertically Down
		direction = -1;
		for(int d=1;d<2;d++) {
			bool canPlace = true;
			ChessPosition *_pos = pos->copy();
			_pos->rank += d*direction;
			if(!board->emptyDirectVerticalTo(pos, _pos))canPlace = false;
			if(!_pos->isOnBoard())canPlace = false;
			_pos->metaData = MOVEMENT;
			if(canPlace)retVal.push_back(_pos);
		}
		// Attack Diagonally Up Left
		ChessPiece *occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 1, -1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Up Right
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 1, 1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Down Right
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 1, 1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Diagonally Down Left
		occupant = board->firstDiagonallyCollidedPieceWithinRange(pos, 1, -1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Left
		occupant = board->firstHorizontallyCollidedPieceWithinRange(pos, 1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Right
		occupant = board->firstHorizontallyCollidedPieceWithinRange(pos, 1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Up
		occupant = board->firstVerticallyCollidedPieceWithinRange(pos, 1, 1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
		// Attack Down
		occupant = board->firstVerticallyCollidedPieceWithinRange(pos, 1, -1);
		if(occupant != NULL && occupant->color != color) {
			ChessPosition *_pos = occupant->pos->copy();
			_pos->metaData = ATTACK;
			retVal.push_back(_pos);
		}
	}
	return retVal;
}

bool ChessPiece::canMoveToOnBoard(ChessPosition *_pos, ChessBoard *board) {
	vector<ChessPosition *> possiblePositions = possibleMovementPositionsOnBoard(board);
	for(int d=0;d<possiblePositions.size();d++) {
		if(possiblePositions[d]->isEqualTo(_pos))return true;
	}
	return false;
}
