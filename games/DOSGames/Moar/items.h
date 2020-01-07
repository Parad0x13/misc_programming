#ifndef __ITEMS__
#define __ITEMS__

typedef char *string;

class Item {
public:
	Item();
	bool alive;
	string name;
	char icon;
};

Item::Item() {
	alive = true;
	name = "default_item";
	icon = 'D';
}

Item *Stone_Item() {
	Item *retVal = new Item;
	retVal->name = "stone";
	retVal->icon = 'S';
	return retVal;
}

#endif
