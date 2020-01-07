#ifndef __GAMESTATE
#define __GAMESTATE

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "project-wide.h"
#include "PieceArray.h"
#include "BasicMove.h"

class GameState {
public:
	GameState();
	GameState(std::string FEN);
	void init();
	std::string FEN();

	// This will change over time I'm sure
	int evaluate();	// Evaluate returns positive for white, negative for black

	BasicMove best_move();
	static BasicMoveWithScore minimax(GameState gameState, int depth);

	// Pseudolegal moves are all moves approved for pieces that follow movement rules
	std::map<int, std::vector<std::vector<BasicMove> > > all_pseudolegal_moves;
	std::map<int, std::vector<std::vector<BasicMove> > > all_legal_moves;
	//std::vector<std::vector<BasicMove> > all_pseudolegal_moves;
	void clearMoves();
	std::vector<std::vector<BasicMove> > pseudolegal_moves(int boardPosition);
	std::vector<std::vector<BasicMove> > ray_move_attack(int boardPosition, int xDirection, int yDirection);

	// Legal moves are all pseudolegal moves that do not put same color king in check
	std::vector<std::vector<BasicMove> > legal_moves(int color);
	std::vector<std::vector<BasicMove> > legal_moves_at(int boardPosition);

	std::vector<int> attackedSquares(int friendlyColor);

	void make_move(BasicMove move);
	void make_best_move();
	bool is_move_valid(BasicMove move, int color);

	// Would like to move away from PieceArrays to favor Bitboards
	// But right now I do not understand Bitboards well enough
	PieceArray pieceArray;
	int enPassantLocation;
	bool wCastleK, wCastleQ, bCastleK, bCastleQ;
	int halfMoves, fullMoves;
	bool activeColor;
	bool is_check, is_checkmate, is_draw;
};

#endif
