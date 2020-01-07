#ifndef __CHESSMOVE__
#define __CHESSMOVE__

#include <iostream>

class ChessPosition;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void stripPrefixDots(std::string &input);

class ChessMove {
public:
	ChessMove() {}
	ChessMove(std::string toParse);
	ChessMove(int color, int type, ChessPosition *start, ChessPosition *end) : color(color), type(type), start(start), end(end) {
		//
	}
	std::string toParsableString();
	void log() {std::cout << toParsableString() << std::endl;}

	int color;
	int type;
	ChessPosition *start, *end;
};

#endif
