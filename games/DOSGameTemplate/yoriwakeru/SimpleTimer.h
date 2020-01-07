// Timer And Manager by Bryan Tremblay of Keikaku Studios
/*	This is my first attempt at a timer manager. It runs in one thread
	so that's not great but it wasn't intended to be production quality.
	The real intent was to manage DOS game timers	*/
	// TODO - Pass data to selector
	// TODO - Figure out how to call this on member functions of classes
	//		Currently this only works on static functions as required by C++
	//		There seems to be a boost::bind function that can do what I need
// V1.0
#ifndef __SIMPLETIMER__
#define __SIMPLETIMER__

#include <time.h>
#include <vector>

static int globalTag = 100;

class SimpleTimer {
public:
	SimpleTimer(void (*selector)(), float period, bool repeats) : selector(selector), period(period), repeats(repeats) {
		isActive = false;
		isMarkedForDestruction = false;
		shouldFire = false;
		lastFireTick = clock();
		tag = globalTag++;
	}

	void start() {isActive = true;}
	void stop()  {isActive = false;}

	void tick() {
		if(isActive) {
			clock_t now = clock();
			double delta = (now-lastFireTick) / (float)CLOCKS_PER_SEC;
			if(delta >= period)shouldFire = true;
		}
	}

	void fire() {
		shouldFire = false;
		(*selector)();
		lastFireTick = clock();
		if(!repeats)isMarkedForDestruction = true;
	}

	clock_t lastFireTick;
	int tag;
	double period;
	bool isActive, isMarkedForDestruction, shouldFire, repeats;
	void (*selector)();
};
static std::vector<SimpleTimer *>timers;

void registerTimer(SimpleTimer *timer, bool startAutomatically, bool fireAutomatically) {
	if(startAutomatically)timer->start();
	if(fireAutomatically)timer->fire();
	timers.push_back(timer);
}

void manageTimers() {
	for(int d = 0;d < timers.size();d++)
		timers[d]->tick();
	for(int d = 0;d < timers.size();d++)
		if(timers[d]->shouldFire)timers[d]->fire();

	bool shouldContinueErasing = true;
	while(shouldContinueErasing) {
		shouldContinueErasing = false;
		for(int d = 0;d < timers.size();d++) {
			if(timers[d]->isMarkedForDestruction) {
				shouldContinueErasing = true;
				timers.erase(timers.begin() + d);
				break;
			}
		}
	}
}

#endif
