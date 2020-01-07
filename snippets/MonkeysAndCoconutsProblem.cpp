// Bryan Tremblay 03May2015@1130L Colorado Springs.
// I wrote this to solve a question posed by a video by Numberphile:
//		https://www.youtube.com/watch?v=U9qU20VmvaU
// I got the answer right the first time but wow... what an explination he gave
// I understood like... none... absolutely none of how he found the answer using mathematics

/*
	The question goes like this:
	5 Sailors are stranded on an island with a monkey
	For food the sailors collect coconuts through the day and then go to bed
	Each sailor secretly wakes up in the middle of the night and splits the pile of coconuts
		equally between hiself and his fellow sailors. After doing so there is one coconut left
		which he then give to the monkey. Afterwards he hides his pile and merges the 4 remaining piles
	After all 5 sailors have done this they all wake up and split the remaining pile
		of coconuts evenly between each other. This time there are no coconuts left to give the monkey
	The question then becomes how many coconuts did the sailors collect the day before?

	There are infinite solutions but the lowest value is 3121
*/

#include <iostream>

#define BAD_COCONUT_COUNT -1

using namespace std;

int coconutsAfterOneHiding(int sailors, int coconuts) {
	int sailorsCut = coconuts / sailors;
	int monkeysCut = coconuts % sailors;

	// The monkey must have one coconut after a sailor hides his stash
	if(monkeysCut != 1)return BAD_COCONUT_COUNT;

	int finalCoconuts = coconuts - sailorsCut - monkeysCut;

	return finalCoconuts;
}

void runSituation(int sailors, int coconuts) {
	bool parametersSatisfied = false;

	int currentCoconuts = coconuts;
	for(int d=0;d<sailors;d++) {
		currentCoconuts = coconutsAfterOneHiding(sailors, currentCoconuts);
		if(currentCoconuts == BAD_COCONUT_COUNT)break;
		if(d+1 == sailors && (currentCoconuts % sailors) == 0)parametersSatisfied = true;
	}

	if(parametersSatisfied)
		cout << "Parameters satisfied starting with " << coconuts << " coconuts with " << currentCoconuts << " coconuts left over" << endl;
}

int main() {
	for(int d=0;d<100000;d++)
		runSituation(5, d);
	return false;
}
