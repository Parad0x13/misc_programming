#include <iostream>
#include <time.h>

using namespace std;

double timer_delta = 0;
clock_t clock_start;

void startClock() {
	clock_start = clock();
}

double stopClock() {
	clock_t clock_stop = clock();
	double delta = (clock_stop - clock_start) / (float)CLOCKS_PER_SEC;
	return delta;
}

int main() {
	startClock();
	for(int d=0;d<10000;d++){for(int dd=0;dd<10000;dd++)int number = d*d;}
	double delta = stopClock();

	cout << "Took " << delta << " seconds" << endl;
}
