#include <iostream>
#include <cstdio>
#include <cassert>
#include <cmath>
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "ChessPosition.h"
#include "ChessMove.h"
#include "GameManager.h"

using namespace std;

#include <sstream>
template<class T>
string toString(T input) {
	std::stringstream ss;
	ss << input;
	return ss.str();
}

ChessBoard::ChessBoard() {
	this->reset();
	overlayOn = false;
	activeColor = WHITE;
	halfMove = 0;
	fullMove = 1;
}

void ChessBoard::reset() {
	pieces.clear();
	pieces.push_back(new ChessPiece(new ChessPosition("a1"), R, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("b1"), N, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("c1"), B, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("d1"), Q, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("e1"), K, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("f1"), B, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("g1"), N, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("h1"), R, WHITE));
	for(int file=0;file<8;file++)pieces.push_back(new ChessPiece(new ChessPosition(file, 1), P, WHITE));
	pieces.push_back(new ChessPiece(new ChessPosition("a8"), R, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("b8"), N, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("c8"), B, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("d8"), Q, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("e8"), K, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("f8"), B, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("g8"), N, BLACK));
	pieces.push_back(new ChessPiece(new ChessPosition("h8"), R, BLACK));
	for(int file=0;file<8;file++)pieces.push_back(new ChessPiece(new ChessPosition(file, 6), P, BLACK));
}

void ChessBoard::render() {
	vector<ChessPosition *>overlayPositions;
	if(overlayOn)
		overlayPositions = overlayPiece->possibleMovementPositionsOnBoard(this);

	for(int mark=0;mark<10;mark++)cout << '-';
	cout << endl;
	for(int rank=7;rank>=0;rank--) {
		cout << '|';
		for(int file=0;file<8;file++) {
			ChessPosition *pos = new ChessPosition(file, rank);
			ChessPiece *piece = pieceAt(pos);
			bool overlayAlreadyWritten = false;
			for(int d=0;d<overlayPositions.size();d++) {
				if(overlayPositions[d]->isEqualTo(pos)) {
					if(overlayPositions[d]->metaData == MOVEMENT)printf("X");
					else if(overlayPositions[d]->metaData == ATTACK)printf("A");
					else printf("?");
					overlayAlreadyWritten = true;
				}
			}
			if(!overlayAlreadyWritten) {
				if(piece != NULL)
					piece->render();
				else {
					if(rank%2 == 0 && file%2 != 0)printf("%c", ' ');
					else if(rank%2 != 0 && file%2 == 0)printf("%c", ' ');
					else printf("%c", 176);
				}
			}
		}
		cout << '|';
		cout << endl;
	}
	for(int mark=0;mark<10;mark++)cout << '-';
	cout << endl;
}

ChessPiece *ChessBoard::pieceAt(ChessPosition *pos) {
	for(int d=0;d<pieces.size();d++) {
		if(pieces[d]->pos->isEqualTo(pos))return pieces[d];
	}
	return NULL;
}

bool ChessBoard::emptyDirectHorizontalTo(ChessPosition *a, ChessPosition *b) {
	if(a->rank != b->rank)return false;									// Pieces are not on the same rank
	int direction = (a->file < b->file)?1:-1;
	int delta = abs(a->file - b->file)+1;
	for(int d=1;d<delta;d++) {
		ChessPiece *obsticle = pieceAt(new ChessPosition(a->file + d*direction, a->rank));
		if(obsticle != NULL)return false;								// A piece exists!
	}
	return true;
}

bool ChessBoard::emptyDirectVerticalTo(ChessPosition *a, ChessPosition *b) {
	if(a->file != b->file)return false;									// Pieces are not on the same file
	int direction = (a->rank < b->rank)?1:-1;
	int delta = abs(a->rank - b->rank)+1;
	for(int d=1;d<delta;d++) {
		ChessPiece *obsticle = pieceAt(new ChessPosition(a->file, a->rank + d*direction));
		if(obsticle != NULL)return false;								// A piece exists!
	}
	return true;
}

bool ChessBoard::emptyDirectDiagonalTo(ChessPosition *a, ChessPosition *b) {
	if(abs(a->file - b->file) != abs(a->rank - b->rank))return false;			// Pieces are not on the same diagonal
	int xDirection = (a->file < b->file)?1:-1;
	int yDirection = (a->rank < b->rank)?1:-1;
	int delta = abs(a->file - b->file)+1;		// xDelta and yDelta are equivalant
	for(int d=1;d<delta;d++) {
		ChessPiece *obsticle = pieceAt(new ChessPosition(a->file + d*xDirection, a->rank + d*yDirection));
		if(obsticle != NULL)return false;		// A piece exists!
	}
	return true;
}

ChessPiece *ChessBoard::firstHorizontallyCollidedPieceWithinRange(ChessPosition *a, int range, int direction) {
	for(int d=1;d<=range;d++) {
		ChessPosition *_pos = new ChessPosition(a->file + d*direction, a->rank);
		if(_pos->isOnBoard()) {
			ChessPiece *obsticle = pieceAt(_pos);
			if(obsticle != NULL)return obsticle;
		}
	}
	return NULL;
}

ChessPiece *ChessBoard::firstVerticallyCollidedPieceWithinRange(ChessPosition *a, int range, int direction) {
	for(int d=1;d<=range;d++) {
		ChessPosition *_pos = new ChessPosition(a->file, a->rank + d*direction);
		if(_pos->isOnBoard()) {
			ChessPiece *obsticle = pieceAt(_pos);
			if(obsticle != NULL)return obsticle;
		}
	}
	return NULL;
}

ChessPiece *ChessBoard::firstDiagonallyCollidedPieceWithinRange(ChessPosition *a, int range, int xDirection, int yDirection) {
	for(int d=1;d<=range;d++) {
		ChessPosition *_pos = new ChessPosition(a->file + d*xDirection, a->rank + d*yDirection);
		if(_pos->isOnBoard()) {
			ChessPiece *obsticle = pieceAt(_pos);
			if(obsticle != NULL)return obsticle;
		}
	}
	return NULL;
}

void ChessBoard::setOverlayTo(ChessPiece *piece) {
	overlayOn = true;
	overlayPiece = piece;
}

void ChessBoard::movePiece(ChessMove *move) {
	// Ensure En Passant can't be counted for previous pawn movements
	for(int d=0;d<pieces.size();d++)pieces[d]->pawnJustMovedForwardTwice = false;
	halfMove++;

	ChessPiece *pointA = pieceAt(move->start);
	ChessPiece *pointB = pieceAt(move->end);
	if(pointA != NULL) {
		if(pointA->canMoveToOnBoard(move->end, this)) {
			// Determine En Passant
			if(pointA->type == P) {
				halfMove = 0;
				if(pointA->color == WHITE && (move->start->rank == 1 && move->end->rank == 3))
					pointA->pawnJustMovedForwardTwice = true;
				if(pointA->color == BLACK && (move->start->rank == 6 && move->end->rank == 4))
					pointA->pawnJustMovedForwardTwice = true;
			}
			// Determine if pawn promotion
			if(pointA->type == P) {
				if(pointA->color == WHITE && move->end->rank == 7)pointA->type = Q;
				if(pointA->color == BLACK && move->end->rank == 0)pointA->type = Q;
			}
			pointA->pos = move->end;
			pointA->hasMoved = true;
		}
		else cout << "Illegal move attempted: " << move->toParsableString() << endl;
	}
	// Cull taken piece from board
	if(pointB != NULL) {
		assert(pointB->color != pointA->color);
		int index = -1;
		for(int d=0;d<pieces.size();d++) {
			if(pieces[d] == pointB) {
				index = d;
				break;
			}
		}
		if(index != -1) {
			halfMove = 0;
			pieces.erase(pieces.begin()+index);
		}
	}

	if(activeColor == WHITE)activeColor = BLACK;
	else if(activeColor == BLACK) {
		activeColor = WHITE;
		fullMove++;
	}
}

string ChessBoard::FENNotation() {
	int emptyCount = 0;
	string retVal;
	for(int rank=7;rank>=0;rank--) {
		for(int file=0;file<8;file++) {
			ChessPiece *piece = pieceAt(new ChessPosition(file, rank));
			if(piece != NULL) {
				if(emptyCount > 0) {
					retVal += toString(emptyCount);
					emptyCount = 0;
				}
				retVal += piece->icon();
			}
			else {
				emptyCount++;
			}
		}
		if(emptyCount > 0)retVal += toString(emptyCount);
		emptyCount = 0;
		if(rank != 0)retVal += "/";
	}
	retVal += " ";
	retVal += (activeColor == WHITE)?"w":"b";
	retVal += " ";
	string castling;
	ChessPiece *whiteKing = pieceAt(new ChessPosition("e1"));
	ChessPiece *whiteQueenSideRook = pieceAt(new ChessPosition("a1"));
	ChessPiece *whiteKingSideRook = pieceAt(new ChessPosition("h1"));
	ChessPiece *blackKing = pieceAt(new ChessPosition("e8"));
	ChessPiece *blackQueenSideRook = pieceAt(new ChessPosition("a8"));
	ChessPiece *blackKingSideRook = pieceAt(new ChessPosition("h8"));
	if((whiteKing != NULL && whiteKingSideRook != NULL) && (!whiteKing->hasMoved && !whiteKingSideRook->hasMoved))castling += "K";
	if((whiteKing != NULL && whiteQueenSideRook != NULL) && (!whiteKing->hasMoved && !whiteQueenSideRook->hasMoved))castling += "Q";
	if((blackKing != NULL && blackKingSideRook != NULL) && (!blackKing->hasMoved && !blackKingSideRook->hasMoved))castling += "k";
	if((blackKing != NULL && blackQueenSideRook != NULL) && (!blackKing->hasMoved && !blackQueenSideRook->hasMoved))castling += "q";
	if(castling == "")castling += "-";
	retVal += castling;
	retVal += " ";
	// Oddly enough the En Passant point is the spot 'behind' the moved pawn
	string enPassant;
	for(int d=0;d<pieces.size();d++) {
		if(pieces[d]->pawnJustMovedForwardTwice) {
			ChessPosition *pos = pieces[d]->pos->copy();
			int direction = 1;
			if(pieces[d]->color == BLACK)direction = -1;
			pos->rank -= direction;
			enPassant = pos->toParsableString();
			break;
		}
	}
	if(enPassant == "")enPassant = "-";
	retVal += enPassant;
	retVal += " ";
	retVal += toString(halfMove);
	retVal += " ";
	retVal += toString(fullMove);

	return retVal;
}
