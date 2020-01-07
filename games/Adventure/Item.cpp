#include "Item.h"

Item::Item() {
	name = "Default_Item";
	count = 0;
}

Item::Item(string _name, int _count) {
	name = _name;
	count = _count;
}

Inventory::Inventory() {
	int d;
    itemCount = 0;
	items = new Item *[itemCount];

	for(d=0;d<itemCount;d++)
		items[d] = new Item;
}

void Inventory::display() {
	int d;
	printf("Inventory has %i items in it\n", itemCount);
	for(d=0;d<itemCount;d++)
		printf("\t%s x%i\n", items[d]->name, items[d]->count);
}

void Inventory::addItem(Item *item, int verbose) {
	if(verbose)
		printf("Adding %s x%i to inventory\n", item->name, item->count);
	if(containsItemNamed(item->name)) {
		int index = indexForItemNamed(item->name);
		items[index]->count += item->count;
	}
	else
		addItemToIndex(item);
}

void Inventory::addItemToIndex(Item *item) {
	Item **newItems, **oldItems;
	int newCount = itemCount+1;
	int d;

	oldItems = items;
	newItems = new Item *[newCount];
	for(d=0;d<itemCount;d++)
		newItems[d] = oldItems[d];
	newItems[newCount-1] = item;

	itemCount = newCount;
	items = newItems;
	delete oldItems;
}

void Inventory::removeItem(Item *item, int verbose) {
	int ableToRemove = false;

	if(containsItemNamed(item->name)) {
		int index = indexForItemNamed(item->name);
		if(item->count > items[index]->count)
			item->count = items[index]->count;
		items[index]->count -= item->count;
		ableToRemove = true;

		if(items[index]->count == 0)
			removeItemFromIndex(index);
	}

	if(verbose) {
		if(ableToRemove)
			printf("Removed %s x%i from inventory\n", item->name, item->count);
		else
			printf("Can't remove that from inventory\n");
	}
}

void Inventory::removeItemFromIndex(int index) {
	printf("Removing from index: %i\n", index);
	Item **newItems, **oldItems;
	int newCount = itemCount-1;
	int i, d;

	oldItems = items;
	newItems = new Item *[newCount];

	i = index;
	while(++i < itemCount)
		oldItems[i-1] = oldItems[i];
	for(d=0;d<newCount;d++)
		newItems[d] = oldItems[d];

	itemCount = newCount;
	items = newItems;
	delete oldItems;
}

int Inventory::containsItemNamed(string itemName) {
	int d;
	for(d=0;d<itemCount;d++) {
		if(!strcmp(items[d]->name, itemName))
			return true;
	}
	return false;
}

int Inventory::indexForItemNamed(string itemName) {
	int d;
	for(d=0;d<itemCount;d++) {
		if(!strcmp(items[d]->name, itemName))
			return d;
	}
	printf("Inventory does not contain that item\n");
	return -1;
}

Equipment::Equipment() {
	Weapon = "none";
	Shield = "none";
}
