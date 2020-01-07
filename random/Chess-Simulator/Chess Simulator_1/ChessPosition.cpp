#include "ChessPosition.h"

using namespace std;

string fileNames = "abcdefgh";

string ChessPosition::toParsableString() {
	string final;
	final += fileNames[file];
	final += rank + '0' + 1;
	return final;
}
