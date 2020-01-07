#include "Area.h"

string areaTypes[7] = {
	"Plains",
	"Meadow",
	"Swamp",
	"Volcano",
	"Cave",
	"Tundra",
	"Dessert"
};
int areaTypeCount = 7;

Area::Area() {
	printf("Warning, default Area() has been called\n");
	type = 0;
	location = vertexWith(0, 0);
}

Area::Area(int _type, vertex *_location) {
	if(_type == -1)
		type = rnd(areaTypeCount);
	else
		type = _type;

	location = _location;
}

World::World() {
	int d;
	areaCount = 0;
}

void World::addArea(Area *area) {
	if(!areaExistsAtLocation(area->location)) {
		Area **newAreas, **oldAreas;
		int newAreaCount = areaCount+1;
		int d;

		oldAreas = areas;
		newAreas = new Area *[newAreaCount];
		newAreas[newAreaCount-1] = area;

		areaCount = newAreaCount;
		areas = newAreas;
		delete oldAreas;
	}
	else
		printf("Area already exists at that location\n");
}

void World::removeAreaAtLocation(vertex *location) {
	printf("Removing area from world at location (%i, %i)\n", location->x, location->y);

	if(areaExistsAtLocation(location)) {
		removeAreaAtIndex(indexForAreaAtLocation(location));
	}
	else
		printf("Area at location (%i, %i) does not exist and can't be removed\n", location->x, location->y);
}

void World::removeAreaAtIndex(int index) {
	Area **newAreas, **oldAreas;
	int newAreaCount = areaCount-1;
	int i, d;

	oldAreas = areas;
	newAreas = new Area *[newAreaCount];

	i = index;
	while(++i < areaCount)
		oldAreas[i-1] = oldAreas[i];
	for(d=0;d<newAreaCount;d++)
		newAreas[d] = oldAreas[d];

	areaCount = newAreaCount;
	areas = newAreas;
	delete oldAreas;
}

int World::areaExistsAtLocation(vertex *location) {
	int d;
	for(d=0;d<areaCount;d++) {
		if(areas[d]->location->x == location->x && areas[d]->location->y == location->y) {
			return true;
		}
	}
	return false;
}

int World::indexForAreaAtLocation(vertex *location) {
	int d;
	for(d=0;d<areaCount;d++) {
		if(areas[d]->location->x == location->x && areas[d]->location->y == location->y)
			return d;
	}
	printf("Area at location (%i, %i) does not exist\n", location->x, location->y);
}
