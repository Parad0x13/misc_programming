// Interesting game theory/statistics problem called Monty Hall: http://en.wikipedia.org/wiki/Monty_Hall_problem
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

// Good up to larger values, then mean gets skewed
int rand_lim(int limit) {
	int divisor = RAND_MAX/(limit+1), retVal;
	do {retVal = rand() / divisor;} while (retVal > limit);
	return retVal;
}

typedef enum {
	WIN,
	LOOSE,
}DoorTypes;

class Door {
public:
	Door(int type) : type(type) {}
	int type;
};

double playMontyHall(uint32_t playCount, bool shouldSwitch) {
	uint32_t winCount = 0;

	for(int d=0;d<playCount;d++) {
		vector<Door *> doors;
		doors.push_back(new Door(WIN));
		doors.push_back(new Door(LOOSE));
		doors.push_back(new Door(LOOSE));
		random_shuffle(doors.begin(), doors.end());
		Door *choice = doors[rand_lim(2)];
		Door *certainLooser = NULL;
		Door *finalChoice = NULL;
		while(certainLooser == NULL) {
			Door *attempt = doors[rand_lim(2)];
			if(attempt != choice && attempt->type == LOOSE)
				certainLooser = attempt;
		}
		if(shouldSwitch) {
			while(finalChoice == NULL) {
				Door *attempt = doors[rand_lim(2)];
				if(attempt != choice && attempt != certainLooser)
					finalChoice = attempt;
			}
		}
		else finalChoice = choice;
		if(finalChoice->type == WIN)winCount++;
	}
	return (winCount/(double)playCount)*100.0f;
}

int main() {
	srand(time(NULL));
	uint32_t playCount = 100000u;
	double withSwitch = playMontyHall(playCount, true);
	cout << "Won " << withSwitch << "% of the time when switching" << endl;
	double withoutSwitch = playMontyHall(playCount, false);
	cout << "Won " << withoutSwitch << "% of the time without switching" << endl;
}
