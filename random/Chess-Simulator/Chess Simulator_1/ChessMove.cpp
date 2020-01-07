#include <sstream>
#include <vector>
#include "ChessMove.h"
#include "ChessPiece.h"
#include "GameManager.h"
#include "ChessPosition.h"

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))elems.push_back(item);
	return elems;
}

vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

void stripPrefixDots(string &input) {while(input[0] == '.')input.erase(input.begin());}

ChessMove::ChessMove(string toParse) {
	// Moves shall be in the format ...Qb3/b2
	string value = toParse;
	string first3 = value.substr(0, 3);
	if(first3 == "...")color = WHITE;
	else color = BLACK;
	stripPrefixDots(value);
	char typeChar = value[0];
	if(typeChar == 'P')type = P;
	else if(typeChar == 'N')type = N;
	else if(typeChar == 'B')type = B;
	else if(typeChar == 'R')type = R;
	else if(typeChar == 'Q')type = Q;
	else if(typeChar == 'K')type = K;
	value.erase(value.begin());
	vector<string> locations = split(value, '/');
	start = new ChessPosition(locations[0]);
	end = new ChessPosition(locations[1]);
}

string ChessMove::toParsableString() {
	string final;
	if(color == WHITE)final += "...";
	if(type == P)final += "P";
	else if(type == N)final += "N";
	else if(type == B)final += "B";
	else if(type == R)final += "R";
	else if(type == Q)final += "Q";
	else if(type == K)final += "K";
	final += start->toParsableString();
	final += "/";
	final += end->toParsableString();
	return final;
}
