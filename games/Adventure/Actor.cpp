#include "Actor.h"
#include "Item.h"

Actor::Actor() {
	name = "Default_Actor";
	health = 100;
	mana = 100;
	inventory = new Inventory();
	equipment = new Equipment();
	level = 0;
	exp = 0;
	strength = 0;
	intelligence = 0;
	agility = 0;
	luck = 0;
}
