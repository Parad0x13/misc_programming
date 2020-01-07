// Remake of old RubikASwap
#include <conio.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>

#define ESC			27
#define xAxis		100
#define yAxis		101
#define zAxis		102
#define xPrimeAxis	103
#define yPrimeAxis	104
#define zPrimeAxis	105

using namespace std;

string allowedSymbols = "UDLRFB2'";

int syllabusSize = 18;
string unrotatedSyllabus[] =	{"U", "D", "L", "R", "F", "B", "U'", "D'", "L'", "R'", "F'", "B'", "U2", "D2", "L2", "R2", "F2", "B2"};

string xAxisSyllabus[] =		{"B", "F", "L", "R", "U", "D", "B'", "F'", "L'", "R'", "U'", "D'", "B2", "F2", "L2", "R2", "U2", "D2"};
string xPrimeAxisSyllabus[] =	{"F", "B", "L", "R", "D", "U", "F'", "B'", "L'", "R'", "D'", "U'", "F2", "B2", "L2", "R2", "D2", "U2"};

string yAxisSyllabus[] =		{"U", "D", "B", "F", "L", "R", "U'", "D'", "B'", "F'", "L'", "R'", "U2", "D2", "B2", "F2", "L2", "R2"};
string yPrimeAxisSyllabus[] =	{"U", "D", "F", "B", "R", "L", "U'", "D'", "F'", "B'", "R'", "L'", "U2", "D2", "F2", "B2", "R2", "L2"};

string zAxisSyllabus[] =		{"R", "L", "U", "D", "F", "B", "R'", "L'", "U'", "D'", "F'", "B'", "R2", "L2", "U2", "D2", "F2", "B2"};
string zPrimeAxisSyllabus[] =	{"L", "R", "D", "U", "F", "B", "L'", "R'", "D'", "U'", "F'", "B'", "L2", "R2", "D2", "U2", "F2", "B2"};

string stringToUpper(string input) {
	transform(input.begin(), input.end(), input.begin(), ::toupper);
	return input;
}

string getAlgorithm() {
	string algorithm;
	cout << endl;
	cout << "Enter Algorithm (e.g. UR2FBRB2RU2LB2RU'D'R2FR'LB2U2F2): ";
	cin >> algorithm;
	algorithm = stringToUpper(algorithm);
	cout << "Algorithm: " << algorithm << endl;
	return algorithm;
}

bool syntaxCheck(string input) {
	for(int d=0;d<allowedSymbols.size();d++)
		input.erase(remove(input.begin(), input.end(), allowedSymbols[d]), input.end());
	return input.size()==0?true:false;
	// TODO: Check if two primes in a row, or two numbers in a row
	// TODO: Check if algorithm has at least one symbol
}

int symbolCountInString(string input) {
	// I don't really like how this is done, what if there are symbols with 3 characters?
	input.erase(remove(input.begin(), input.end(), '\''), input.end());
	input.erase(remove(input.begin(), input.end(), '2'), input.end());
	return input.size();
}

string symbolAtIndex(string input, int index) {
	bool atIndex = false;
	int offset = 0, readingIndex = 0;
	int lastSymbolSize;
	string symbol;
	assert(input.size() > index);
	while(!atIndex) {
		lastSymbolSize = 1;
		if(input.size() >= offset+1) {
			if(input[offset+1] == '\'' || input[offset+1] == '2')lastSymbolSize++;
		}

		if(readingIndex == index) {
			symbol = input.substr(offset, lastSymbolSize);
			atIndex = true;
		}
		else {
			readingIndex++;
			offset += lastSymbolSize;
		}
	}
	return symbol;
}

string rotateAlgorithm(string input, int axis) {
	string retVal;
	string *syllabus;
	if(axis == xAxis)syllabus = xAxisSyllabus;
	else if(axis == xPrimeAxis)syllabus = xPrimeAxisSyllabus;
	else if(axis == yAxis)syllabus = yAxisSyllabus;
	else if(axis == yPrimeAxis)syllabus = yPrimeAxisSyllabus;
	else if(axis == zAxis)syllabus = zAxisSyllabus;
	else if(axis == zPrimeAxis)syllabus = zPrimeAxisSyllabus;
	assert(axis);
	for(int d=0;d<symbolCountInString(input);d++) {
		string symbol = symbolAtIndex(input, d);
		int index;
		for(int dd=0;dd<syllabusSize;dd++) {
			if(unrotatedSyllabus[dd] == symbol)index = dd;
		}
		retVal += syllabus[index];
	}
	return retVal;
}

int main() {
	string algorithm;
	bool havePassingAlgorithm = false;
	while(!havePassingAlgorithm) {
		algorithm = getAlgorithm();
		bool doesSyntaxPass = syntaxCheck(algorithm);
		if(doesSyntaxPass)havePassingAlgorithm = true;
		else {
			cout << "Algorithm does not pass syntax check, try using only these symbols:" << endl;
			cout << allowedSymbols << endl;
		}
	}
	cout << "Algorithm passes syntax check" << endl;
	cout << "Rotate the algorithm with x, y, z or Prime with X, Y, Z. Press ESC to exit" << endl;

	bool shouldContinueRotating = true;
	while(shouldContinueRotating) {
		cout << "Input: ";
		char input = getch();
		string command;
		bool shouldShowAlgorithm = true;
		if(input == 'x')command = "X";
		else if(input == 'X')command = "X'";
		else if(input == 'y')command = "Y";
		else if(input == 'Y')command = "Y'";
		else if(input == 'z')command = "Z";
		else if(input == 'Z')command = "Z'";
		cout << command << endl;
		if(input == ESC){shouldContinueRotating = false;shouldShowAlgorithm = false;}
		else if(command == "X")algorithm = rotateAlgorithm(algorithm, xAxis);
		else if(command == "X'")algorithm = rotateAlgorithm(algorithm, xPrimeAxis);
		else if(command == "Y")algorithm = rotateAlgorithm(algorithm, yAxis);
		else if(command == "Y'")algorithm = rotateAlgorithm(algorithm, yPrimeAxis);
		else if(command == "Z")algorithm = rotateAlgorithm(algorithm, zAxis);
		else if(command == "Z'")algorithm = rotateAlgorithm(algorithm, zPrimeAxis);
		else {cout << "Not a valid option" << endl;shouldShowAlgorithm = false;}
		if(shouldShowAlgorithm)cout << "Algorithm: " << algorithm << endl;
	}

	return false;
}
