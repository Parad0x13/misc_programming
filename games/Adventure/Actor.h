#ifndef __ACTOR__
#define __ACTOR__

#include "main.h"

class Inventory;
class Equipment;

class Actor {
public:
	Actor();
	~Actor();

	string name;
	int health, mana;

	Inventory *inventory;
	Equipment *equipment;

	int level, exp;
	int strength, intelligence, agility, luck;
};

#endif
