#ifndef __AREA__
#define __AREA__

#include "main.h"

extern string areaTypes[];
extern int areaTypeCount;

class Area {
public:
	Area();
	Area(int _type, vertex *_location);

	int type;
	vertex *location;
};

class World {
public:
	World();
	~World();

	Area **areas;
	int areaCount;

	void addArea(Area *area);
	void removeAreaAtLocation(vertex *location);
	void removeAreaAtIndex(int index);
	int areaExistsAtLocation(vertex *location);
	int indexForAreaAtLocation(vertex *location);
};

#endif
