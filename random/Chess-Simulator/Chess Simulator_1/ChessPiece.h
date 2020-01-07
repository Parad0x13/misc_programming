#ifndef __CHESSPIECE__
#define __CHESSPIECE__

class ChessPosition;
class ChessBoard;

typedef enum {
	P, N, B, R, Q, K,
}PIECE_TYPE;

class ChessPiece {
public:
	ChessPiece(ChessPosition *pos, int type, int color) : pos(pos), type(type), color(color) {
		hasMoved = false;
		pawnJustMovedForwardTwice = false;
	}
	char icon();
	void render();
	void log();
	std::vector<ChessPosition *> possibleMovementPositionsOnBoard(ChessBoard *board);
	bool canMoveToOnBoard(ChessPosition *_pos, ChessBoard *board);

	ChessPosition *pos;
	int type;
	int color;
	bool hasMoved;
	bool pawnJustMovedForwardTwice;
};

#endif
