/*
ofstream:	Stream class to write on files
ifstream:	Stream class to read from files
fstream:	Stream class to both read and write from/to files

Default mode flags:
ofstream:	ios::out
ifstream:	ios::in
fstream:	ios::in | ios::out

Flags:
ios::in		Open for input operations
ios::out	Open for output operations
ios::binary	Open in binary mode
ios::ate	Set the initial position at the end of the file.
			If this flag is not set to any value,
			the initial position is the beginning of the file
ios::app	All output operations are performed at the end of the file,
			appending the content to the current context of the file.
			This flag can only be used in streams open for output-only operations
ios::trunc	If the file opened for output operations already existed before,
			its previous content is deleted and replaced by the new one
*/
#include <iostream>
#include <fstream>

using namespace std;

void inputFile() {
	ifstream fileIn;
	fileIn.open("input.txt");

	if(fileIn.is_open()) {
		string line;
		while(getline(fileIn, line))cout << line << endl;
	}
	else cout << "Unable to open input file" << endl;

	fileIn.close();
}

void outputFile() {
	ofstream fileOut;
	fileOut.open("output.txt", ios::app);

	if(fileOut.is_open())fileOut << "New Info!!!" << endl;
	else cout << "Unable to create output file" << endl;

	fileOut.close();
}

int main(int argc, char **argv) {
	inputFile();
	outputFile();
}
