#include "GameState.h"
#include <assert.h>
#include <sstream>
#include <bitset>
#include <cmath>

#include "Bitboard.h"
#include "PieceArray.h"
#include "libs.h"

#include "FEN.h"

void GameState::init() {
	enPassantLocation = offBoard;
	wCastleK = wCastleQ = bCastleK = bCastleQ = true;
	is_check = is_checkmate = is_draw = false;
	halfMoves = 0;
	fullMoves = 1;
	activeColor = WHITE;
	clearMoves();
}

GameState::GameState() {
	init();
}

GameState::GameState(string FEN) {
	init();
	Bitboard bitboard;
	vector<string> sections = split(FEN, ' ');
	assert(sections.size() == 6);
	vector<string> ranks = split(sections[0], '/');
	assert(ranks.size() == 8);
	for(int d=0;d<8;d++) {
		int index = d*8;
		for(int dd=ranks[d].length()-1;dd>=0;dd--) {
			char piece = ranks[d][dd];
			uint64_t *applicableMask = NULL;
			if(piece == '1')index += 1;
			else if(piece == '2')index += 2;
			else if(piece == '3')index += 3;
			else if(piece == '4')index += 4;
			else if(piece == '5')index += 5;
			else if(piece == '6')index += 6;
			else if(piece == '7')index += 7;
			else if(piece == '8')index += 8;
			else if(piece == 'P')applicableMask = &bitboard.data[wPawns];
			else if(piece == 'R')applicableMask = &bitboard.data[wRooks];
			else if(piece == 'N')applicableMask = &bitboard.data[wKnights];
			else if(piece == 'B')applicableMask = &bitboard.data[wBishops];
			else if(piece == 'Q')applicableMask = &bitboard.data[wQueens];
			else if(piece == 'K')applicableMask = &bitboard.data[wKing];
			else if(piece == 'p')applicableMask = &bitboard.data[bPawns];
			else if(piece == 'r')applicableMask = &bitboard.data[bRooks];
			else if(piece == 'n')applicableMask = &bitboard.data[bKnights];
			else if(piece == 'b')applicableMask = &bitboard.data[bBishops];
			else if(piece == 'q')applicableMask = &bitboard.data[bQueens];
			else if(piece == 'k')applicableMask = &bitboard.data[bKing];

			if(applicableMask != NULL) {
				// Need to convert FEN's big endianness to little endianness
				*applicableMask |= uint64_t(1)<<(63-index);
				index++;
			}
		}
	}
	pieceArray = bitboard.toPieceArray();

	if(sections[1] == "w")activeColor = WHITE;
	else activeColor = BLACK;

	if(sections[2].find('K') != std::string::npos)wCastleK = true;
	else wCastleK = false;
	if(sections[2].find('Q') != std::string::npos)wCastleQ = true;
	else wCastleQ = false;
	if(sections[2].find('k') != std::string::npos)bCastleK = true;
	else bCastleK = false;
	if(sections[2].find('q') != std::string::npos)bCastleQ = true;
	else bCastleQ = false;

	if(sections[3] != "-") {
		for(int index=0;index<64;index++) {
			if(squareNames[index] == sections[3]) {
				enPassantLocation = index;
				break;
			}
		}
	}
	else
		enPassantLocation = offBoard;
}

string GameState::FEN() {
	Bitboard bitboard = pieceArray.toBitboard();
	string retVal = "";
	for(int d=7;d>=0;d--) {		// Switching FEN's big endian to little endian here
		int emptyCounter = 0;
		for(int dd=0;dd<8;dd++) {
			int index = d*8 + dd;
			int icon = Empty;

			/// if you want to use a Bitboard instead of PieceArray use:
			if(isBitSet(bitboard.data[wPawns], index))icon = P;
			else if(isBitSet(bitboard.data[wRooks], index))icon = R;
			else if(isBitSet(bitboard.data[wKnights], index))icon = N;
			else if(isBitSet(bitboard.data[wBishops], index))icon = B;
			else if(isBitSet(bitboard.data[wQueens], index))icon = Q;
			else if(isBitSet(bitboard.data[wKing], index))icon = K;
			else if(isBitSet(bitboard.data[bPawns], index))icon = p;
			else if(isBitSet(bitboard.data[bRooks], index))icon = r;
			else if(isBitSet(bitboard.data[bKnights], index))icon = n;
			else if(isBitSet(bitboard.data[bBishops], index))icon = b;
			else if(isBitSet(bitboard.data[bQueens], index))icon = q;
			else if(isBitSet(bitboard.data[bKing], index))icon = k;

			/// if you want to use a PieceArray instead of Bitboard use:
			///icon = pieceArray.data[index];

			if(icon == Empty)emptyCounter++;

			if(icon != Empty) {
				if(emptyCounter > 0) {
					stringstream counterStream;
					counterStream << emptyCounter;
					retVal += counterStream.str();
				}
				emptyCounter = 0;
				retVal += pieceIcons[icon];
			}
		}
		if(emptyCounter > 0) {
			stringstream counterStream;
			counterStream << emptyCounter;
			retVal += counterStream.str();
		}
		if((7-d) < 7)retVal += "/";
	}

	if(activeColor == WHITE)retVal += " w";
	else retVal += " b";

	string castleData = "";
	if(wCastleK)castleData += "K";
	if(wCastleQ)castleData += "Q";
	if(bCastleK)castleData += "k";
	if(bCastleQ)castleData += "q";
	if(castleData == "")castleData = "-";
	retVal += " " + castleData;

	if(enPassantLocation != offBoard)retVal += " " + squareNames[enPassantLocation];
	else retVal += " -";

	stringstream hm, fm;
	hm << halfMoves;
	fm << fullMoves;
	retVal += " " + hm.str();
	retVal += " " + fm.str();

	return retVal;
}

int GameState::evaluate() {
	LOG
	int color = BLACK;
	if(activeColor == BLACK)
		color = WHITE;
	int P_score = 0, P_score_prime = 0;
	int R_score = 0, R_score_prime = 0;
	int N_score = 0, N_score_prime = 0;
	int B_score = 0, B_score_prime = 0;
	int Q_score = 0, Q_score_prime = 0;

	for(int d=0;d<64;d++) {
		int piece = pieceArray.pieceAt(d);
		if(piece == P || piece == p) {
			if(isCorrectColor(color, piece))P_score++;
			else P_score_prime++;
		}
		if(piece == R || piece == r) {
			if(isCorrectColor(color, piece))R_score++;
			else R_score_prime++;
		}
		if(piece == N || piece == n) {
			if(isCorrectColor(color, piece))N_score++;
			else N_score_prime++;
		}
		if(piece == B || piece == b) {
			if(isCorrectColor(color, piece))B_score++;
			else B_score_prime++;
		}
		if(piece == Q || piece == q) {
			if(isCorrectColor(color, piece))Q_score++;
			else Q_score_prime++;
		}
	}

	int materialScore =	9*(Q_score - Q_score_prime) +
						5*(R_score - R_score_prime) +
						3*(B_score - B_score_prime + N_score - N_score_prime) +
						1*(P_score - P_score_prime);

	vector<vector<BasicMove> > moves = legal_moves(color);
	int mobility = moves[0].size() + moves[1].size();
	if(color ==  BLACK) {
		materialScore = -materialScore;
		mobility = -mobility;
	}
	if(SHOULD_LOG)cout << "materialScore became: " << materialScore << " mobility became: " << mobility << endl;

	int finalScore = materialScore + mobility;

	return finalScore;
}

BasicMove GameState::best_move() {
	LOG
	BasicMoveWithScore moveWithScore = GameState::minimax(*this, 2);
	GameState *address = this;
	if(SHOULD_LOG)cout << "best move for " << address << " happens to be: " << moveWithScore.move.log() << " which scores: " << moveWithScore.score << endl;
	return moveWithScore.move;
}

BasicMoveWithScore GameState::minimax(GameState gameState, int depth) {
	if(SHOULD_LOG)cout << "minimax at depth of " << depth << endl;
	if(depth == 0 || gameState.is_checkmate || gameState.is_draw) {
		if(SHOULD_LOG)cout << "depth has stopped with activeColor: " << gameState.activeColor << endl;
		return BasicMoveWithScore(BasicMove(), gameState.evaluate());
	}
	else {
		if(gameState.activeColor == WHITE) {
			if(SHOULD_LOG)cout << "maxi" << endl;
			int bestScore = -999999;
			BasicMove bestMove;
			vector<vector<BasicMove> > moves = gameState.legal_moves(gameState.activeColor);
			vector<BasicMove> allMoves;
			allMoves.insert(allMoves.end(), moves[0].begin(), moves[0].end());
			allMoves.insert(allMoves.end(), moves[1].begin(), moves[1].end());
			for(int d=0;d<allMoves.size();d++) {
				GameState newState = gameState;
				newState.make_move(allMoves[d]);
				BasicMoveWithScore newMoveWithScore = GameState::minimax(newState, depth - 1);
				if(SHOULD_LOG) {
					cout << "maxi has found move: " << allMoves[d].log() << endl;
					cout << "maxi gave it a score of: " << newMoveWithScore.score << endl;
				}
				int score = newMoveWithScore.score;

				if(score > bestScore) {
					bestScore = score;
					bestMove = allMoves[d];
				}
			}
			return BasicMoveWithScore(bestMove, bestScore);
		}
		else {
			if(SHOULD_LOG)cout << "mini" << endl;
			int bestScore = 999999;
			BasicMove bestMove;
			vector<vector<BasicMove> > moves = gameState.legal_moves(gameState.activeColor);
			vector<BasicMove> allMoves;
			allMoves.insert(allMoves.end(), moves[0].begin(), moves[0].end());
			allMoves.insert(allMoves.end(), moves[1].begin(), moves[1].end());
			for(int d=0;d<allMoves.size();d++) {
				GameState newState = gameState;
				newState.make_move(allMoves[d]);
				BasicMoveWithScore newMoveWithScore = GameState::minimax(newState, depth - 1);
				if(SHOULD_LOG) {
					cout << "mini has found move: " << allMoves[d].log() << endl;
					cout << "mini gave it a score of: " << newMoveWithScore.score << endl;
				}
				int score = newMoveWithScore.score;

				if(score < bestScore) {
					bestScore = score;
					bestMove = allMoves[d];
				}
			}
			return BasicMoveWithScore(bestMove, bestScore);
		}
	}
}

void GameState::clearMoves() {
	LOG
	all_pseudolegal_moves.clear();
	all_legal_moves.clear();
}

// [TODO]: Move move checking to bit twiddling for speedups, ... maybe someday...
vector<vector<BasicMove> > GameState::pseudolegal_moves(int boardPosition) {
	LOG
	vector<vector<BasicMove> > retVal;
	vector<BasicMove> pushes;
	vector<BasicMove> attacks;

	if(all_pseudolegal_moves.find(boardPosition) != all_pseudolegal_moves.end()) {
		if(SHOULD_LOG)cout << "recalculation of pseudolegal move has been averted" << endl;
		return all_pseudolegal_moves[boardPosition];
	}

	if(SHOULD_LOG) {
		GameState *address = this;
		cout << "calculating for: " << squareNames[boardPosition] << " in: " << address << endl;
	}

	int piece = pieceArray.pieceAt(boardPosition);
	if(piece == P || piece == p) {
		// Move forward one normally
		int forwardSquare = offBoard;
		if(piece == P)forwardSquare = translateSquare(boardPosition, 0, 1);
		else if(piece == p)forwardSquare = translateSquare(boardPosition, 0, -1);
		if(pieceArray.pieceAt(forwardSquare) == Empty)
			pushes.push_back(BasicMove(boardPosition, forwardSquare));
		// Move forward two
		int forward2Squares = offBoard;
		if(piece == P)forward2Squares = translateSquare(boardPosition, 0, 2);
		else if(piece == p)forward2Squares = translateSquare(boardPosition, 0, -2);
		int yIndex = squareYIndex(boardPosition);
		if(	((yIndex == 1 && piece == P) || (yIndex == 6 && piece == p)) &&
			pieceArray.pieceAt(forwardSquare) == Empty &&
			pieceArray.pieceAt(forward2Squares) == Empty)
			pushes.push_back(BasicMove(boardPosition, forward2Squares));
		// Attack diagonally normally
		/// Left
		int forwardLSquare = offBoard;
		if(piece == P)forwardLSquare = translateSquare(boardPosition, -1, 1);
		else if(piece == p)forwardLSquare = translateSquare(boardPosition, -1, -1);
		int forwardLPiece = pieceArray.pieceAt(forwardLSquare);
		if(isEnemy(piece, forwardLPiece))
			attacks.push_back(BasicMove(boardPosition, forwardLSquare));
		/// Right
		int forwardRSquare = offBoard;
		if(piece == P)forwardRSquare = translateSquare(boardPosition, 1, 1);
		else if(piece == p)forwardRSquare = translateSquare(boardPosition, 1, -1);
		int forwardRPiece = pieceArray.pieceAt(forwardRSquare);
		if(isEnemy(piece, forwardRPiece))
			attacks.push_back(BasicMove(boardPosition, forwardRSquare));
		// Attack diagonally en passant
		if(enPassantLocation != offBoard) {
			if(forwardLSquare == enPassantLocation)
				attacks.push_back(BasicMove(boardPosition, forwardLSquare));
			else if(forwardRSquare == enPassantLocation)
				attacks.push_back(BasicMove(boardPosition, forwardRSquare));
		}
	}
	else if(piece == N || piece == n) {
		// Movement/attack are the same
		int spots[8] = {Empty};
		spots[0] = translateSquare(boardPosition, -2, +1);
		spots[1] = translateSquare(boardPosition, -1, +2);
		spots[2] = translateSquare(boardPosition, +1, +2);
		spots[3] = translateSquare(boardPosition, +2, +1);
		spots[4] = translateSquare(boardPosition, -2, -1);
		spots[5] = translateSquare(boardPosition, -1, -2);
		spots[6] = translateSquare(boardPosition, +1, -2);
		spots[7] = translateSquare(boardPosition, +2, -1);
		for(int d=0;d<8;d++) {
			int thePiece = pieceArray.pieceAt(spots[d]);
			if(thePiece == Empty)
				pushes.push_back(BasicMove(boardPosition, spots[d]));
			else if(isEnemy(piece, thePiece))
				attacks.push_back(BasicMove(boardPosition, spots[d]));
		}
	}
	else if(piece == R || piece == r) {
		// Ray attack/move
		/// Left
		vector<vector<BasicMove> > leftRay = ray_move_attack(boardPosition, -1, 0);
		pushes.insert(pushes.end(), leftRay[0].begin(), leftRay[0].end());
		attacks.insert(attacks.end(), leftRay[1].begin(), leftRay[1].end());
		/// Right
		vector<vector<BasicMove> > rightRay = ray_move_attack(boardPosition, 1, 0);
		pushes.insert(pushes.end(), rightRay[0].begin(), rightRay[0].end());
		attacks.insert(attacks.end(), rightRay[1].begin(), rightRay[1].end());
		/// Up
		vector<vector<BasicMove> > upRay = ray_move_attack(boardPosition, 0, 1);
		pushes.insert(pushes.end(), upRay[0].begin(), upRay[0].end());
		attacks.insert(attacks.end(), upRay[1].begin(), upRay[1].end());
		/// Down
		vector<vector<BasicMove> > downRay = ray_move_attack(boardPosition, 0, -1);
		pushes.insert(pushes.end(), downRay[0].begin(), downRay[0].end());
		attacks.insert(attacks.end(), downRay[1].begin(), downRay[1].end());
	}
	else if(piece == B || piece == b) {
		// Ray attack/move
		/// Up Left
		vector<vector<BasicMove> > upLeftRay = ray_move_attack(boardPosition, -1, 1);
		pushes.insert(pushes.end(), upLeftRay[0].begin(), upLeftRay[0].end());
		attacks.insert(attacks.end(), upLeftRay[1].begin(), upLeftRay[1].end());
		/// Up Right
		vector<vector<BasicMove> > upRightRay = ray_move_attack(boardPosition, 1, 1);
		pushes.insert(pushes.end(), upRightRay[0].begin(), upRightRay[0].end());
		attacks.insert(attacks.end(), upRightRay[1].begin(), upRightRay[1].end());
		/// Down Left
		vector<vector<BasicMove> > downLeftRay = ray_move_attack(boardPosition, -1, -1);
		pushes.insert(pushes.end(), downLeftRay[0].begin(), downLeftRay[0].end());
		attacks.insert(attacks.end(), downLeftRay[1].begin(), downLeftRay[1].end());
		/// Down Right
		vector<vector<BasicMove> > downRightRay = ray_move_attack(boardPosition, 1, -1);
		pushes.insert(pushes.end(), downRightRay[0].begin(), downRightRay[0].end());
		attacks.insert(attacks.end(), downRightRay[1].begin(), downRightRay[1].end());
	}
	else if(piece == Q || piece == q) {
		// Ray attack/move
		/// Left
		vector<vector<BasicMove> > leftRay = ray_move_attack(boardPosition, -1, 0);
		pushes.insert(pushes.end(), leftRay[0].begin(), leftRay[0].end());
		attacks.insert(attacks.end(), leftRay[1].begin(), leftRay[1].end());
		/// Right
		vector<vector<BasicMove> > rightRay = ray_move_attack(boardPosition, 1, 0);
		pushes.insert(pushes.end(), rightRay[0].begin(), rightRay[0].end());
		attacks.insert(attacks.end(), rightRay[1].begin(), rightRay[1].end());
		/// Up
		vector<vector<BasicMove> > upRay = ray_move_attack(boardPosition, 0, 1);
		pushes.insert(pushes.end(), upRay[0].begin(), upRay[0].end());
		attacks.insert(attacks.end(), upRay[1].begin(), upRay[1].end());
		/// Down
		vector<vector<BasicMove> > downRay = ray_move_attack(boardPosition, 0, -1);
		pushes.insert(pushes.end(), downRay[0].begin(), downRay[0].end());
		attacks.insert(attacks.end(), downRay[1].begin(), downRay[1].end());
		/// Up Left
		vector<vector<BasicMove> > upLeftRay = ray_move_attack(boardPosition, -1, 1);
		pushes.insert(pushes.end(), upLeftRay[0].begin(), upLeftRay[0].end());
		attacks.insert(attacks.end(), upLeftRay[1].begin(), upLeftRay[1].end());
		/// Up Right
		vector<vector<BasicMove> > upRightRay = ray_move_attack(boardPosition, 1, 1);
		pushes.insert(pushes.end(), upRightRay[0].begin(), upRightRay[0].end());
		attacks.insert(attacks.end(), upRightRay[1].begin(), upRightRay[1].end());
		/// Down Left
		vector<vector<BasicMove> > downLeftRay = ray_move_attack(boardPosition, -1, -1);
		pushes.insert(pushes.end(), downLeftRay[0].begin(), downLeftRay[0].end());
		attacks.insert(attacks.end(), downLeftRay[1].begin(), downLeftRay[1].end());
		/// Down Right
		vector<vector<BasicMove> > downRightRay = ray_move_attack(boardPosition, 1, -1);
		pushes.insert(pushes.end(), downRightRay[0].begin(), downRightRay[0].end());
		attacks.insert(attacks.end(), downRightRay[1].begin(), downRightRay[1].end());
	}
	else if(piece == K || piece == k) {
		// Movement/attack are the same
		int spots[8] = {Empty};
		spots[0] = translateSquare(boardPosition, -1, +1);
		spots[1] = translateSquare(boardPosition, +0, +1);
		spots[2] = translateSquare(boardPosition, +1, +1);
		spots[3] = translateSquare(boardPosition, -1, +0);
		spots[4] = translateSquare(boardPosition, +1, +0);
		spots[5] = translateSquare(boardPosition, -1, -1);
		spots[6] = translateSquare(boardPosition, +0, -1);
		spots[7] = translateSquare(boardPosition, +1, -1);
		for(int d=0;d<8;d++) {
			int thePiece = pieceArray.pieceAt(spots[d]);
			if(thePiece == Empty)
				pushes.push_back(BasicMove(boardPosition, spots[d]));
			else if(isEnemy(piece, thePiece))
				attacks.push_back(BasicMove(boardPosition, spots[d]));
		}
		// Castle queenside/kingside
		// Cannot castle if privileges have been lost
			// Privileges will be lost during movement of piece, or capturing of rooks
		// Cannot castle if space between rook and king are obstructed
		// Cannot castle if in check (legal_move check)
		// Cannot castle if any space traversed is in check (legal_move check)
		// I could nest this once more to negate an extra check, but I don't want to
		if(piece == K && wCastleK && pieceArray.pieceAt(e1) == K) {
			if(pieceArray.pieceAt(f1) == Empty && pieceArray.pieceAt(g1) == Empty)
				pushes.push_back(BasicMove(boardPosition, g1));
		}
		if(piece == K && wCastleQ && pieceArray.pieceAt(e1) == K) {
			if(	pieceArray.pieceAt(d1) == Empty && pieceArray.pieceAt(c1) == Empty &&
				pieceArray.pieceAt(b1) == Empty)
				pushes.push_back(BasicMove(boardPosition, c1));
		}
		if(piece == k && bCastleK && pieceArray.pieceAt(e8) == k) {
			if(pieceArray.pieceAt(f8) == Empty && pieceArray.pieceAt(g8) == Empty)
				pushes.push_back(BasicMove(boardPosition, g8));
		}
		if(piece == k && bCastleQ && pieceArray.pieceAt(e8) == k) {
			if(	pieceArray.pieceAt(d8) == Empty && pieceArray.pieceAt(c8) == Empty &&
				pieceArray.pieceAt(b8) == Empty)
				pushes.push_back(BasicMove(boardPosition, c8));
		}
	}

	retVal.push_back(pushes);
	retVal.push_back(attacks);
	if(SHOULD_LOG) {
		cout << "decided on:" << endl;
		BasicMove::render(retVal);
	}
	all_pseudolegal_moves[boardPosition] = retVal;
	return retVal;
}

vector<vector<BasicMove> > GameState::ray_move_attack(int boardPosition, int xDirection, int yDirection) {
	vector<vector<BasicMove> > retVal;
	vector<BasicMove> pushes;
	vector<BasicMove> attacks;

	int piece = pieceArray.pieceAt(boardPosition);
	for(int d=0;d<8;d++) {
		int square = translateSquare(boardPosition, (d+1)*xDirection, (d+1)*yDirection);
		int thePiece = pieceArray.pieceAt(square);
		if(thePiece == Empty)
			pushes.push_back(BasicMove(boardPosition, square));
		else if(isEnemy(piece, thePiece)) {
			attacks.push_back(BasicMove(boardPosition, square));
			break;
		}
		else break;
	}

	retVal.push_back(pushes);
	retVal.push_back(attacks);
	return retVal;
}

vector<vector<BasicMove> > GameState::legal_moves(int color) {
	LOG
	vector<vector<BasicMove> > retVal;
	vector<BasicMove> pushes;
	vector<BasicMove> attacks;

	for(int d=0;d<64;d++) {
		if(isCorrectColor(color, pieceArray.pieceAt(d))) {
			vector<vector<BasicMove> > pieceMoves = legal_moves_at(d);
			pushes.insert(pushes.end(), pieceMoves[0].begin(), pieceMoves[0].end());
			attacks.insert(attacks.end(), pieceMoves[1].begin(), pieceMoves[1].end());
		}
	}

	retVal.push_back(pushes);
	retVal.push_back(attacks);
	return retVal;
}

// Should only return moves that can be made by the active color
vector<vector<BasicMove> > GameState::legal_moves_at(int boardPosition) {
	//LOG
	vector<vector<BasicMove> > retVal;
	vector<BasicMove> pushes;
	vector<BasicMove> attacks;

	if(all_legal_moves.find(boardPosition) != all_legal_moves.end()) {
		if(SHOULD_LOG)cout << "recalculation of legal move has been averted" << endl;
		return all_legal_moves[boardPosition];
	}

	int piece = pieceArray.pieceAt(boardPosition);
	if(piece == Empty) {
		retVal.push_back(pushes);
		retVal.push_back(attacks);
		return retVal;
	}

	vector<vector<BasicMove> > pseudolegals = pseudolegal_moves(boardPosition);

	int color = WHITE;
	if(isBlack(piece))color = BLACK;

	for(int d=0;d<pseudolegals[0].size();d++) {
		if(isCorrectColor(color, piece) && is_move_valid(pseudolegals[0][d], color))
			pushes.push_back(pseudolegals[0][d]);
	}
	for(int d=0;d<pseudolegals[1].size();d++) {
		if(isCorrectColor(color, piece) && is_move_valid(pseudolegals[1][d], color))
			attacks.push_back(pseudolegals[1][d]);
	}

	retVal.push_back(pushes);
	retVal.push_back(attacks);
	all_legal_moves[boardPosition] = retVal;
	return retVal;
}

vector<int> GameState::attackedSquares(int friendlyColor) {
	LOG
	vector<int> retVal;

	for(int d=0;d<64;d++) {
		int piece = pieceArray.pieceAt(d);
		if(piece != Empty && !isCorrectColor(friendlyColor, piece)) {
			vector<vector<BasicMove> > pseudolegals = pseudolegal_moves(d);
			vector<BasicMove> pushes = pseudolegals[0];
			vector<BasicMove> attacks = pseudolegals[1];
			for(int dd=0;dd<pushes.size();dd++) {
				int piece = pieceArray.pieceAt(pushes[dd].from);
				// This is because the Pawn is the only piece that cannot attack where it moves
				if(piece != P && piece != p)
					retVal.push_back(pushes[dd].to);
			}
			for(int dd=0;dd<attacks.size();dd++)
				retVal.push_back(attacks[dd].to);
		}
	}

	return retVal;
}

/*			Move making logic:
	When make_move(move) is called the function assumes the move is valid.
	To ensure a valid move is played you must check it with is_move_valid(move)
	and then play it. This allows self generated moves to be played automatically
	without having to re-deduce the validity of the move and wasting cycles */
// [TODO]: Incorporate rule to signal when the board has returned to the same position 3 times
// Code is written for clarity, some things may not be optimal
void GameState::make_move(BasicMove move) {
	LOG
	if(move.isNullMove()) {
		cout << "cannot make " << move.log() << endl;
		return;
	}

	GameState *address = this;
	if(SHOULD_LOG)cout << "move made: " << move.log() << " in: " << address << endl;
	bool isPawnMove = false;		// For halfmove clock
	bool isCaptureMove = false;		// For halfmove clock
	bool isEnPassantPush = false;
	bool isEnPassantAttack = false;
	bool isPromoteMove = false;
	bool isKingMove = false;		// For castling privileges
	bool isCaptureRookMove = false;	// For castling privileges
	bool isMoveRookMove = false;	// For castling privileges
	bool isCastle = false;			// For castling privileges

	int fromPiece = pieceArray.pieceAt(move.from);
	int toPiece = pieceArray.pieceAt(move.to);

	if(fromPiece == P || fromPiece == p)
		isPawnMove = true;

	if(toPiece != Empty)
		isCaptureMove = true;

	// [TODO]: Refactor this horrible horrible horrible way of managing En Passant
	int from = move.from;
	int to = move.to;
	if(isPawnMove && activeColor == WHITE && (
	from == a2 || from == b2 || from == c2 || from == d2 ||
	from == e2 || from == f2 || from == g2 || from == h2) && (
	to == a4 || to == b4 || to == c4 || to == d4 ||
	to == e4 || to == f4 || to == g4 || to == h4))
		isEnPassantPush = true;
	if(isPawnMove && activeColor == BLACK && (
	from == a7 || from == b7 || from == c7 || from == d7 ||
	from == e7 || from == f7 || from == g7 || from == h7) && (
	to == a5 || to == b5 || to == c5 || to == d5 ||
	to == e5 || to == f5 || to == g5 || to == h5))
		isEnPassantPush = true;

	if(isPawnMove && move.to == enPassantLocation)isEnPassantAttack = true;

	if(isPawnMove && activeColor == WHITE && squareYIndex(move.to) == 7)
		isPromoteMove = true;
	else if(isPawnMove && activeColor == BLACK && squareYIndex(move.to) == 0)
		isPromoteMove = true;

	if(toPiece == R || toPiece == r)
		isCaptureRookMove = true;
	if(isCaptureRookMove) {
		if(move.to == a1)wCastleQ = false;
		else if(move.to == h1)wCastleK = false;
		else if(move.to == a8)bCastleQ = false;
		else if(move.to == h8)bCastleK = false;
	}

	if(fromPiece == R || fromPiece == r)
		isMoveRookMove = true;
	if(isMoveRookMove) {
		if(wCastleQ && move.from == a1)wCastleQ = false;
		else if(wCastleK && move.from == h1)wCastleK = false;
		else if(bCastleQ && move.from == a8)bCastleQ = false;
		else if(bCastleK && move.from == h8)bCastleK = false;
	}

	if(fromPiece == K || fromPiece == k) {
		isKingMove = true;
		if(	move.from == e1 && (move.to == c1 || move.to == g1))
			isCastle = true;
		else if(move.from == e8 && (move.to == c8 || move.to == g8))
			isCastle = true;
	}

	if(isKingMove) {
		if(fromPiece == K) {
			wCastleK = false;
			wCastleQ = false;
		}
		else if(fromPiece == k) {
			bCastleK = false;
			bCastleQ = false;
		}
	}

	if(isEnPassantAttack) {
		isCaptureMove = true;
		pieceArray.setPiece(move.from, Empty);
		pieceArray.setPiece(move.to, fromPiece);
		// Remove piece that initiated en passant
		// [TODO]: Fix how this is done, I don't like it
		if(enPassantLocation == a3)pieceArray.setPiece(a4, Empty);
		else if(enPassantLocation == b3)pieceArray.setPiece(b4, Empty);
		else if(enPassantLocation == c3)pieceArray.setPiece(c4, Empty);
		else if(enPassantLocation == d3)pieceArray.setPiece(d4, Empty);
		else if(enPassantLocation == e3)pieceArray.setPiece(e4, Empty);
		else if(enPassantLocation == f3)pieceArray.setPiece(f4, Empty);
		else if(enPassantLocation == g3)pieceArray.setPiece(g4, Empty);
		else if(enPassantLocation == h3)pieceArray.setPiece(h4, Empty);
		else if(enPassantLocation == a6)pieceArray.setPiece(a5, Empty);
		else if(enPassantLocation == b6)pieceArray.setPiece(b5, Empty);
		else if(enPassantLocation == c6)pieceArray.setPiece(c5, Empty);
		else if(enPassantLocation == d6)pieceArray.setPiece(d5, Empty);
		else if(enPassantLocation == e6)pieceArray.setPiece(e5, Empty);
		else if(enPassantLocation == f6)pieceArray.setPiece(f5, Empty);
		else if(enPassantLocation == g6)pieceArray.setPiece(g5, Empty);
		else if(enPassantLocation == h6)pieceArray.setPiece(h5, Empty);
	}
	else if(isCastle) {
		if(move.to == c1) {
			pieceArray.setPiece(e1, Empty);
			pieceArray.setPiece(a1, Empty);
			pieceArray.setPiece(c1, K);
			pieceArray.setPiece(d1, R);
		}
		else if(move.to == g1) {
			pieceArray.setPiece(e1, Empty);
			pieceArray.setPiece(h1, Empty);
			pieceArray.setPiece(g1, K);
			pieceArray.setPiece(f1, R);
		}
		else if(move.to == c8) {
			pieceArray.setPiece(e8, Empty);
			pieceArray.setPiece(a8, Empty);
			pieceArray.setPiece(c8, k);
			pieceArray.setPiece(d8, r);
		}
		else if(move.to == g8) {
			pieceArray.setPiece(e8, Empty);
			pieceArray.setPiece(h8, Empty);
			pieceArray.setPiece(g8, k);
			pieceArray.setPiece(f8, r);
		}
	}
	else {
		pieceArray.setPiece(move.from, Empty);
		pieceArray.setPiece(move.to, fromPiece);
	}

	// [TODO]: Allow promotion to more than just Queen, although never Pawn
	if(isPromoteMove && activeColor == WHITE)
		pieceArray.setPiece(move.to, Q);
	else if(isPromoteMove && activeColor == BLACK)
		pieceArray.setPiece(move.to, q);

	// This must occur after enPassantLocation has been fully utilized
	// [TODO]: Refactor this, I don't like how it is done
	if(isEnPassantPush) {
		if(from == a2)enPassantLocation = a3;
		else if(from == b2)enPassantLocation = b3;
		else if(from == c2)enPassantLocation = c3;
		else if(from == d2)enPassantLocation = d3;
		else if(from == e2)enPassantLocation = e3;
		else if(from == f2)enPassantLocation = f3;
		else if(from == g2)enPassantLocation = g3;
		else if(from == h2)enPassantLocation = h3;
		else if(from == a7)enPassantLocation = a6;
		else if(from == b7)enPassantLocation = b6;
		else if(from == c7)enPassantLocation = c6;
		else if(from == d7)enPassantLocation = d6;
		else if(from == e7)enPassantLocation = e6;
		else if(from == f7)enPassantLocation = f6;
		else if(from == g7)enPassantLocation = g6;
		else if(from == h7)enPassantLocation = h6;
	}
	else enPassantLocation = offBoard;	// Musnt' allow en passant to persist past 2 moves

	// Check if king is in check(mate), then set flags
	is_check = false;
	is_checkmate = false;
	is_draw = false;
	vector<int> attacked = attackedSquares(activeColor);

	// [TODO]: Keep track of king's location, so that we don't have to search it out every time
	int requestedKing = K;
	if(activeColor == BLACK)requestedKing = k;
	int kingLocation = offBoard;
	for(int d=0;d<64;d++) {
		if(pieceArray.pieceAt(d) == requestedKing) {
			kingLocation = d;
			break;
		}
	}
	for(int d=0;d<attacked.size();d++) {
		if(attacked[d] == kingLocation) {
			is_check = true;
			break;
		}
	}
	// Check checkmate and draw too
	if(isPawnMove || isCaptureMove)
		halfMoves = 0;
	else
		halfMoves++;
	if(activeColor == BLACK) {
		fullMoves++;
		activeColor = WHITE;
	}
	else
		activeColor = BLACK;

	clearMoves();
}

void GameState::make_best_move() {
	LOG
	make_move(best_move());
}

// Don't NEED to use this, but for testing purposes it makes sure moves are not idiotic
// or if castling will move king past a checked position
bool GameState::is_move_valid(BasicMove move, int color) {
	LOG
	bool isCastle = false;	// Various checks need to be made if move is a castling one
	int fromPiece = pieceArray.pieceAt(move.from);
	int toPiece = pieceArray.pieceAt(move.to);

	// Not legal if there is no piece in the origin square
	if(fromPiece == Empty) {
		cout << "Error: Moved a piece that wasn't there" << endl;
		return false;
	}

	// Not legal if piece moves to occupied square who's color is the same
	if(toPiece != Empty && !isEnemy(fromPiece, toPiece)) {
		cout << "Error: Moved a piece onto a square with friendly piece" << endl;
		return false;
	}

	if(fromPiece == K || fromPiece == k) {
		if(	move.from == e1 && (move.to == c1 || move.to == g1))
			isCastle = true;
		else if(move.from == e8 && (move.to == c8 || move.to == g8))
			isCastle = true;
	}
	// Cannot castle through a spot that is in check
	if(isCastle) {
		vector<int> attacked = attackedSquares(color);
		vector<int> toKeepSafe;
		int subjectSquare = offBoard;
		if(move.to == c1) {
			toKeepSafe.push_back(c1);
			toKeepSafe.push_back(d1);
			toKeepSafe.push_back(e1);	// Ensures currently not in check
		}
		else if(move.to == g1) {
			toKeepSafe.push_back(g1);
			toKeepSafe.push_back(f1);
			toKeepSafe.push_back(e1);	// Ensures currently not in check
		}
		else if(move.to == c8) {
			toKeepSafe.push_back(c8);
			toKeepSafe.push_back(d8);
			toKeepSafe.push_back(e8);	// Ensures currently not in check
		}
		else if(move.to == g8) {
			toKeepSafe.push_back(g8);
			toKeepSafe.push_back(f8);
			toKeepSafe.push_back(e8);	// Ensures currently not in check
		}
		bool canCastle = true;
		for(int d=0;d<toKeepSafe.size();d++) {
			for(int dd=0;dd<attacked.size();dd++) {
				if(toKeepSafe[d] == attacked[dd]) {
					canCastle = false;
					subjectSquare = attacked[dd];
					break;
				}
			}
		}
		if(!canCastle) {
			cout << "Error: Tried to castle inappropriately. Path or King is in check on " << squareNames[subjectSquare] << endl;
			renderFEN(FEN());
			return false;
		}
	}

	return true;
}
