#include "rand.h"

int rnd(int range)
{
	int r;

	r=rand()%range;
	return(r);
}

void seed()
{
	srand((unsigned)time(NULL));
}