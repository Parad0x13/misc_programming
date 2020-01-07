#include <iostream>
#include <vector>
#include <assert.h>
#include <conio.h>

#include "GameState.h"
#include "FEN.h"

using namespace std;

int main() {
	GameState a = GameState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	renderFEN(a.FEN());

	while(getch() != 'q') {
		a.make_best_move();
		renderFEN(a.FEN());
	}
	//a.make_best_move();
	//renderFEN(a.FEN());
}

/* Test white promote
GameState a = GameState("8/2P5/8/8/8/8/2p5/8 w KQkq - 0 1");
renderFEN(a.FEN());
a.make_move(BasicMove(c7, c8));
a.make_move(BasicMove(c2, c1));
renderFEN(a.FEN());
*/

/* Test white En Passant
GameState a = GameState("8/8/8/1Pp5/8/8/8/8 w KQkq c6 0 1");
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(b5));
*/

/* Test black En Passant
GameState a = GameState("8/8/8/8/1Pp5/8/8/8 w KQkq b3 0 1");
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(c4));
*/

/* Test rooks
GameState a = GameState("8/6r1/8/8/1p1R2R1/8/8/8 w KQkq - 0 1");
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(d4));
BasicMove::render(a.pseudolegal_moves(g7));
*/

/* Test bishops
GameState a = GameState("8/1b4p1/8/8/8/2B5/6p1/8 w KQkq - 0 1");
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(c3));
BasicMove::render(a.pseudolegal_moves(b7));
*/

/* Test Queens
GameState a = GameState("8/8/8/8/3Q4/8/8/8 w KQkq - 0 1");
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(d4));
*/

/* Test Knights
GameState a = GameState("8/8/4p3/8/3N4/8/2P5/8 w KQkq - 0 1");
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(d4));
*/

/* Test castling privilege loss
GameState a = GameState("r3k2r/7P/8/8/8/8/8/R3K2R w KQkq - 0 1");
a.make_move(BasicMove(a1, a2));
a.make_move(BasicMove(a8, a3));
a.make_move(BasicMove(g7, h8));
renderFEN(a.FEN());
BasicMove::render(a.pseudolegal_moves(e8));
BasicMove::render(a.pseudolegal_moves(e1));
*/
