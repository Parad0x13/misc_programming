#include "FEN.h"
#include "libs.h"
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

void renderFEN(string FEN) {
	vector<string> sections = split(FEN, ' ');
	assert(sections.size() == 6);
	vector<string> ranks = split(sections[0], '/');
	assert(ranks.size() == 8);

	for(int d=0;d<8;d++) {
		for(int dd=0;dd<ranks[d].length();dd++) {
			char piece = ranks[d][dd];
			int emptySpaces = 0;
			// [TODO]: refactor this here, ugly
			if(piece == '1')emptySpaces = 1;
			else if(piece == '2')emptySpaces = 2;
			else if(piece == '3')emptySpaces = 3;
			else if(piece == '4')emptySpaces = 4;
			else if(piece == '5')emptySpaces = 5;
			else if(piece == '6')emptySpaces = 6;
			else if(piece == '7')emptySpaces = 7;
			else if(piece == '8')emptySpaces = 8;
			for(int ddd=0;ddd<emptySpaces;ddd++)cout << '.';
			if(emptySpaces == 0)
				cout << piece;
		}
		cout << endl;
	}
	cout << FEN << endl;
	cout << endl;
}
