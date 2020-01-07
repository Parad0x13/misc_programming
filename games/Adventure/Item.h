#ifndef __ITEM__
#define __ITEM__

#include "main.h"

class Item {
public:
	Item();
	~Item();
	Item(string _name, int _count);

	string name;
	int count;
};

class Inventory {
public:
	Inventory();
	~Inventory();

	Item **items;
	int itemCount;

	void display();
	void addItem(Item *item, int verbose);
	void addItemToIndex(Item *item);
	void removeItem(Item *item, int verbose);
	void removeItemFromIndex(int index);
	int containsItemNamed(string itemName);
	int indexForItemNamed(string itemName);
};

class Equipment {
public:
	Equipment();
	~Equipment();

	string Weapon;
	string Shield;
};

#endif
