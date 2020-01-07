#ifndef __CHESSBOARD__
#define __CHESSBOARD__

#include <iostream>
#include <vector>

class ChessPosition;
class ChessPiece;
class ChessMove;

class ChessBoard {
public:
	ChessBoard();
	void reset();
	void render();
	ChessPiece *pieceAt(ChessPosition *pos);
	bool emptyDirectHorizontalTo(ChessPosition *a, ChessPosition *b);
	bool emptyDirectVerticalTo(ChessPosition *a, ChessPosition *b);
	bool emptyDirectDiagonalTo(ChessPosition *a, ChessPosition *b);
	ChessPiece *firstHorizontallyCollidedPieceWithinRange(ChessPosition *a, int range, int direction);
	ChessPiece *firstVerticallyCollidedPieceWithinRange(ChessPosition *a, int range, int direction);
	ChessPiece *firstDiagonallyCollidedPieceWithinRange(ChessPosition *a, int range, int xDirection, int yDirection);
	void setOverlayTo(ChessPiece *piece);
	void movePiece(ChessMove *move);
	std::string FENNotation();

	std::vector<ChessPiece *>pieces;

	bool overlayOn;
	ChessPiece *overlayPiece;
	int activeColor, halfMove, fullMove;
};

#endif
