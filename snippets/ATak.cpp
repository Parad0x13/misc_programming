#include <iostream>
#include <vector>

using namespace std;

enum States {
	NORMAL_WHITE,
	NORMAL_BLACK,
};

class Board {
public:
	Board(int size) {
		for(int d = 0;d < size;d++) {
			state.push_back(vector<int>);
		}
	}

	void render() {
		cout << "I am a board" << endl;
	}

	vector<vector<int>> state;
};

int main() {
	Board* board = new Board(3);
	board->render();

	system("pause");

	return 0;
}
