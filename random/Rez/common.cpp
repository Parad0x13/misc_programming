#include "common.h"
#include <ctime>
#include <cstdlib>

using namespace std;

getRand(int range) {
	if(!randSeeded) {
		srand(time(0));
		randSeeded = true;
	}

	return rand()%range;
}
