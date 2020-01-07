#include <iostream>
#include <conio.h>

using namespace std;

template<class T>
pair<T, T> iterateSubtraction(pair<T, T> input) {
	pair<T, T> retVal = input;
	if(retVal.first >= retVal.second) {
		retVal.first -= retVal.second;
		retVal.second += 10;
	}
	else {
		retVal.first *= 100;
		retVal.second -= 5;
		retVal.second *= 10;
		retVal.second += 5;
	}
	return retVal;
}

int main() {
	pair<uint64_t, uint64_t> input;
	int n = 16;
	input.first = 5*n;
	input.second = 5;
	cout << input.first << ":" << input.second << endl;
	for(int d=0;d<50;d++) {
		input = iterateSubtraction(input);
		cout << input.first << ":" << input.second << endl;
		getch();
	}
	return true;
}
