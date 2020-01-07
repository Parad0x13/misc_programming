#include "BasicMove.h"
#include <iostream>

using namespace std;

string BasicMove::log() {
	if(isNullMove())return "nullMove";
	return squareNames[from] + '-' + squareNames[to];
}

void BasicMove::render(vector<vector<BasicMove> > moves) {
	vector<BasicMove> pushes = moves[0];
	vector<BasicMove> attacks = moves[1];
	for(int d=7;d>=0;d--) {
		for(int dd=0;dd<8;dd++) {
			int index = d*8 + dd;
			char icon = '.';
			for(int ddd=0;ddd<pushes.size();ddd++) {
				if(pushes[ddd].to == index)icon = 'X';
				else if(pushes[ddd].from == index)icon = 'O';
			}
			for(int ddd=0;ddd<attacks.size();ddd++) {
				if(attacks[ddd].to == index)icon = 'A';
				else if(attacks[ddd].from == index)icon = 'O';
			}
			cout << icon;
		}
		cout << endl;
	}
}

bool BasicMove::isNullMove() {
	if(to == a1 && from == a1)return true;
	return false;
}
