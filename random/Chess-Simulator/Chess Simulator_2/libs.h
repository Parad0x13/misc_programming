#ifndef __LIBS
#define __LIBS

#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isBitSet(uint64_t x, int N);

vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);

#endif
