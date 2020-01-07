#include <iostream>
#include <string>

using namespace std;

string masks[] = {"11000", "00011", "00101", "00110", "01001", "01010", "01100", "10001", "10010", "10100"};

class POSTNET {
public:
	POSTNET(string _preprocessed) : preprocessed(_preprocessed) {};
	~POSTNET() {};

	string preprocessed;

	string processed() {
		string retVal = "";

		// Add Frame Bar
		retVal += "1";

		// Process preprocessed
		for(string::iterator it = preprocessed.begin();it != preprocessed.end();it++) {
			char value_char = *it;
			int value = atoi(&value_char);
			retVal += masks[value];
		}

		// Add Frame Bar
		retVal += "1";

		return retVal;
	}

	void render() {
		string tmp = processed();

		// Print top part
		for(int d = 0;d < 2;d++) {
			for(string::iterator it = tmp.begin();it != tmp.end();it++) {
				char value_char = *it;
				int value = atoi(&value_char);
				if(value == 0) cout << " ";
				else cout << "X";
				cout << " ";
			}
			cout << endl;
		}
		// Print bottom part
		for(int d = 0;d < 2;d++) {
			for(string::iterator it = tmp.begin();it != tmp.end();it++) {
				cout << "X";
				cout << " ";
			}
			cout << endl;
		}
	}
};

int main(int argc, char** argv) {
	cout << "This will not work correctly" << endl;
	string postcode = "80915";
	if(argc == 2) postcode = string(argv[1]);
	cout << postcode << endl;

	POSTNET* postnet = new POSTNET(postcode);
	postnet->render();

	return true;
}
