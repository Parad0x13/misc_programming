#ifndef __BASICMOVE
#define __BASICMOVE

#include <vector>
#include <string>
#include "project-wide.h"

// This uses the enumSquare layout. Contains minimal data which can be abstracted later
class BasicMove {
public:
	BasicMove() {
		// aka null move
		from = a1;
		to = a1;
	}
	// [TODO]: refactor this, I can't remember the syntax for constructor():varA(varA), varB(varB) etc...
	BasicMove(int _from, int _to) {
		from = _from;
		to = _to;
	}

	std::string log();
	static void render(std::vector<std::vector<BasicMove> > moves);
	bool isNullMove();

	int from, to;
};

struct BasicMoveWithScore {
	BasicMove move;
	int score;

	// [TODO]: Refactor the heck out of this
	BasicMoveWithScore(BasicMove _move, int _score) {
		move = _move;
		score = _score;
	}
};

#endif
