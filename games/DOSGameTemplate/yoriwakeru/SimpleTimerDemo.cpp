#include "SimpleTimer.h"
#include <iostream>
#include <conio.h>

using namespace std;

void ping() {
	static unsigned int number = 0;
	cout << "Ping! " << number++ << endl;
}

int main() {
	SimpleTimer* timer = new SimpleTimer(ping, 1.0f / 15.0f, true);
	registerTimer(timer, true);

	while(!kbhit())
		manageTimers();

	return false;
}
