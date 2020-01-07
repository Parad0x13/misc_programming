#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<time.h>

#define EOL 0x0A

#define worldName "Adventure Land"

#define active 1
#define inactive 0

#define random -1
#define true 1
#define false 0

#define enemyTurn 0
#define playerTurn 1

// Item Types
#define Money 0
#define Potion 1
#define Weapon 2
#define Shield 3

// Area Types
#define Plains 0
#define Meadow 1
#define Swamp 2
#define Volcano 3
#define Cave 4
#define Tundra 5
#define Dessert 6

typedef char *string;

int areaTypeCount = 7;

int rnd(int range) {
	return rand()%range;
}

void seed() {
	srand((unsigned)time(NULL));
}

int randomAreaType() {
	return rnd(areaTypeCount);
}

string items[5] = {
	"coin",
	"potion",
	"woodenSword",
	"elixer",
	"poison"
};
int itemType[5] = {
	Money,
	Potion,
	Weapon,
	Potion,
	Potion
};
// itemStrength used mostly for weapons
int itemStrength[5] = {
	0,		// coin
	0,		// potion
	5,		// woodenSword
	0,		// elixer
	0,		// poison
};
float itemRarity[5] = {
	0.0f,
	5.0f,
	10.0f,
	15.0f,
	20.0f
};
int itemCount = 5;

typedef struct {
	string name;
	int count;
	float rarity;
}Item;

Item *newItem(string name, int count) {
	Item *retVal = malloc(sizeof(Item));
	string correctName = name;
	if(!itemExists(name)) {
		printf("Item %s does not exist in game, reverting to potion instead\n", name);
		correctName = "potion";
	}
	retVal->name = correctName;
	retVal->count = count;
	retVal->rarity = itemRarity[indexForItem(name)];
	return retVal;
}

typedef struct {
	string weapon;
	string shield;
}Equipment;

typedef struct {
	string name;
	int health;
	int mana;

	Item *Inventory[20];
	int inventoryCount;
	Equipment equipment;

	//int unassignedPerks;
	int level;
	int exp;
	int strength;//, strengthPerks;
	int intelligence;//, intelligencePerks;
	int agility;//, agilityPerks;
	int luck;//, luckPerks;
}Actor;

Actor *defaultActor() {
	Actor *retVal = malloc(sizeof(Actor));
	retVal->inventoryCount = 0;
	retVal->equipment.weapon = "none";
	retVal->equipment.shield = "none";
	retVal->level = 0;
	retVal->exp = 0;
	retVal->strength = 0;
	retVal->intelligence = 0;
	retVal->agility = 0;
	retVal->luck = 0;
	return retVal;
}

string monsters[2] = {
	"Gnome",
	"Rabbit",
};
// health, mana, strength, intelligence, agility
int monsterBaseStats[2][5] = {
	20, 0, 5, 2, 7,		// Gnome
	10, 0, 2, 2, 20		// Rabbit
};
int monsterCount = 2;

int monsterExist(string name) {
	int d;
	for(d=0;d<monsterCount;d++) {
		if(!strcmp(monsters[d], name))
			return true;
	}
	return false;
}

int indexForMonster(string name) {
	int d, retVal;
	for(d=0;d<monsterCount;d++) {
		if(!strcmp(monsters[d], name))
			retVal = d;
	}
	return retVal;
}

string randomMonsterName() {
	return monsters[rnd(monsterCount)];
}

Actor *newMonster(int level, int levelRange);

typedef struct {
	int x, y;
}vertex;

vertex vertexWith(int x, int y) {
	vertex retVal;
	retVal.x = x;
	retVal.y = y;
	return retVal;
}

typedef struct {
	int opened;
	Item *Loot[10];
	int lootCount;
}Chest;

Chest *newChest(int level) {
	Chest *retVal = malloc(sizeof(Chest));
	retVal->opened = false;
	retVal->lootCount = 0;

	retVal->lootCount = 1;
	retVal->Loot[0] = newItem("potion", 1);

	return retVal;
}

void addItemToChest(Chest *chest, Item *item);

// Can't progress until you cleared the area
typedef struct {
	vertex location;
	int cleared;
	int type;
	int level, levelRange;
	int monsterCount;
	Actor *Monsters[10];
	int chestCount;
	Chest *Chests[10];	// Cant be searched until all Monsters are gone
	int movesSinceLastVisit;
	// NPCs
}Area;

Area *newArea(vertex location, int type, int level, int levelRange, int monsters, int chests) {
	Area *retVal = malloc(sizeof(Area));

	retVal->location.x = location.x;
	retVal->location.y = location.y;

	retVal->cleared = false;

	if(type == random)
		retVal->type = randomAreaType();
	else
		retVal->type = type;

	retVal->level = level;
	retVal->levelRange = levelRange;
	if(monsters) {
		int d;
		retVal->monsterCount = rnd(3);
		for(d=0;d<retVal->monsterCount;d++)
			retVal->Monsters[d] = newMonster(retVal->level, retVal->levelRange);
	}
	else
		retVal->monsterCount = 0;

	if(chests) {
		int d;
		retVal->chestCount = rnd(3);
		for(d=0;d<retVal->chestCount;d++)
			retVal->Chests[d] = newChest(retVal->level);
	}
	else
		retVal->chestCount = 0;

	retVal->movesSinceLastVisit = 0;

	return retVal;
}

void gameSetup();
void gameLoop();
void battleLoop();
void switchTurns();
void endTheGame();
void endTheBattle();
void enterBattle(int monsterNumber);
void monsterAttack();
void playerAttack();
void awaitInput();
void parseInput();
void processInput();
int processGameCommands(string input);
int processAreaCommands(string input);
int processItemCommands(string input);
int processBattleCommands(string input);
void listCommands();
void listBattleCommands();
void listItems();
void inventoryCommand();
void listInventory(Actor *actor);
void listEquipment(Actor *actor);
void addHealth(Actor *actor, int amount);
void removeHealth(Actor *actor, int amount);
void addMana(Actor *actor, int amount);
void removeMana(Actor *actor, int amount);
void walkCommand();
void giveCommand();
void dropCommand();
void useCommand();
void openCommand();
void equipCommand();
void removeCommand();
void statsCommand();
void searchCommand();
void removeEquipment(Actor *actor, string name);
void battleCommand();
int itemExists(string theItem);
int indexForItem(string theItem);
void useItem(string theItem);
void addToInventory(Actor *actor, Item *item, int verbose);
void removeFromInventory(Actor *actor, Item *item, int verbose);
void removeIndexFromInventoryArray(Actor *actor, int theIndex);
void removeIndexFromChest(Chest *chest, int theIndex);
void displayActorStats(Actor *actor);
int inventoryContainsItem(Actor *actor, string theItem);
int inventoryIndexForItem(Actor *actor, string theItem);
int equipmentContainsItem(Actor *actor, string theItem);
void searchArea(vertex location);
int isAreaClear(Area *area);
int aliveMonsterCount(Area *area);
void clearArea(Area *area);
void tryToClearArea(Area *area);
int monsterExistsInArea(int monsterNumber, Area *area);
int inBattle();
int isAlive(Actor *actor);
void battleActionHappened();
int battleActionHasHappened();
void lootEnemy();
Item *itemWithRarity(lootRarity);
int areaExistsAtLocation(vertex location);
Area *areaAtLocation(vertex location);
int areaIndexAtLocation(vertex location);
void createAreaAtLocation(vertex location);
int indexForItemInChest(Chest *chest, string item);

void addItemToChest(Chest *chest, Item *item) {
	int nextIndex = chest->lootCount;
	chest->Loot[nextIndex] = item;
}

void removeItemFromChest(Chest *chest, Item *item) {
	int index = indexForItemInChest(chest, item->name);
	if(index == -1)
			printf("Item does not exist in chest\n");
	else {
		removeIndexFromChest(chest, index);
	}
}

int indexForItemInChest(Chest *chest, string item) {
	int d;
	for(d=0;d<chest->lootCount;d++) {
		if(!strcmp(chest->Loot[d]->name, item))
			return d;
	}
	return -1;	// Does not exist
}

Actor *newMonster(int level, int levelRange) {
	Actor *retVal = defaultActor();
	string monsterName = randomMonsterName();
	int monsterIndex = indexForMonster(monsterName);

	retVal->name = monsterName;

	if(levelRange > 0)
		retVal->level = level + (rnd(levelRange*2)-levelRange);
	else
		retVal->level = level;
	retVal->health = monsterBaseStats[monsterIndex][0];
	retVal->mana = monsterBaseStats[monsterIndex][1];
	retVal->strength = monsterBaseStats[monsterIndex][2];
	retVal->intelligence = monsterBaseStats[monsterIndex][3];
	retVal->agility = monsterBaseStats[monsterIndex][4];

	if(!strcmp(monsterName, "Gnome")) {
		addToInventory(retVal, newItem("elixer", 1), false);
		addToInventory(retVal, newItem("coin", rnd(10*retVal->level)), false);
	}
	else if(!strcmp(monsterName, "Gnome")) {
		addToInventory(retVal, newItem("coin", rnd(5*retVal->level)), false);
	}

	return retVal;
}

typedef struct {
	int status;
	int turn;
	int turnOwner;
	int actionHappened;
	Actor *enemy;
}Battle;

Battle *newBattle() {
	Battle *retVal = malloc(sizeof(Battle));
	retVal->status = inactive;
	retVal->actionHappened = false;
	return retVal;
}

Battle *currentBattle;

string areaType[7] = {
	"Plains",
	"Meadow",
	"Swamp",
	"Volcano",
	"Cave",
	"Tundra",
	"Dessert"
};

Area *world[100];	// Update to be dynamic later
int worldSize = 100;
int exploredAreas;
int currentArea;

Actor *player;
char currentInput[20];
string inputQueue[5];
int inputQueueCount;

int gameMode;

string gameCommands[9] = {
	"exit",
	"help/list",
	"inventory",
	"equipment",
	"give item [#]",
	"drop item [#]",
	"items",
	"stats/info [player/enemy]",
	"walk/move [(n)orth/(s)outh/(e)ast/(w)est (u)p/(d)down/(l)eft/(r)ight]"
};
int gameCommandCount = 9;

string areaCommands[3] = {
	"search",
	"battle monster #",
	"open chest #"
};
int areaCommandCount = 3;

string itemCommands[3] = {
	"equip item",
	"remove item",
	"use item"
};
int itemCommandCount = 3;

string battleCommands[1] = {
	"attack"
};
int battleCommandCount = 1;

void main() {
	seed();

	gameSetup();
	printf("\n");
	printf("Welcome to %s!\n", worldName);

	gameLoop();
}

void gameSetup() {
	player = defaultActor();
	player->name = "player";
	player->health = 100;
	player->mana = 100;
	player->level = 1;
	player->exp = 0;
	player->strength = 15;
	player->intelligence = 15;
	player->agility = 15;
	player->luck = 0;

	gameMode = active;

	Item *woodenSword = newItem("woodenSword", 1);
	addToInventory(player, woodenSword, false);

	player->equipment.weapon = "none";
	player->equipment.shield = "none";

	exploredAreas = 1;
	world[0] = newArea(vertexWith(0, 0), random, 1, 0, true, true);
	currentArea = 0;
	tryToClearArea(world[currentArea]);

	currentBattle = newBattle();
}

void gameLoop() {
	while(gameMode == active) {
		awaitInput();
		parseInput();
		processInput();
	}
}

void battleLoop() {
	while(currentBattle->status == active) {
		if(currentBattle->turnOwner == playerTurn) {
			printf("[Your Turn]\n");
			awaitInput();
			parseInput();
			processInput();
		}
		else {
			printf("[Opponents Turn]\n");
			monsterAttack();
		}
		currentBattle->turn++;

		if(currentBattle->actionHappened) {
			switchTurns();
			currentBattle->actionHappened = false;
		}

		if(player->health <= 0) {
			printf("Player has died\n");
			endTheGame();
		}
		else if(currentBattle->enemy->health <= 0) {
			printf("Enemy has died\n");
			lootEnemy();
			currentBattle->status = inactive;
		}
	}
	tryToClearArea(world[currentArea]);
}

void switchTurns() {
	if(currentBattle->turnOwner == playerTurn)
		currentBattle->turnOwner = enemyTurn;
	else
		currentBattle->turnOwner = playerTurn;
}

void endTheGame() {
	if(inBattle())
		endTheBattle();
	printf("See you later!\n");
	gameMode = inactive;
}

void endTheBattle() {
	printf("Battle ending\n");
	currentBattle->status = inactive;
}

void enterBattle(int monsterNumber) {
	Battle *battle = newBattle();
	battle->turn = 0;
	battle->status = active;
	battle->enemy = world[currentArea]->Monsters[monsterNumber];

	printf("\n");
	printf("You are entering combat with %s Lvl %i\n", battle->enemy->name, battle->enemy->level);
	printf("\n");

	if(player->agility > world[currentArea]->Monsters[monsterNumber]->agility)
		battle->turnOwner = playerTurn;
	else if(player->agility < world[currentArea]->Monsters[monsterNumber]->agility)
		battle->turnOwner = enemyTurn;
	else {
		int d = rnd(2);
		if(d == 0)
			battle->turnOwner = playerTurn;
		else
			battle->turnOwner = enemyTurn;
	}

	currentBattle = battle;
	battleLoop();
}

void monsterAttack() {
	if(inBattle()) {
		string weapon = player->equipment.weapon;
		int weaponIndex = indexForItem(weapon);
		int weaponStrength;
		int damage;

		if(!strcmp(currentBattle->enemy->equipment.weapon, "none"))
			weaponStrength = 0;
		else
			weaponStrength = itemStrength[weaponIndex];

		damage = currentBattle->enemy->strength;
		printf("%s is attacking for %i damage\n", currentBattle->enemy->name, damage);
		removeHealth(player, damage);

		battleActionHappened();
	}
	else
		printf("Not in battle!\n");
	printf("\n");
}

void playerAttack() {
	if(inBattle()) {
		string weapon = player->equipment.weapon;
		int weaponIndex = indexForItem(weapon);
		int weaponStrength;
		int damage;

		if(!strcmp(player->equipment.weapon, "none"))
			weaponStrength = 0;
		else
			weaponStrength = itemStrength[weaponIndex];

		damage = player->strength +	weaponStrength;
		printf("Player is attacking for %i damage\n", damage);
		removeHealth(currentBattle->enemy, damage);

		battleActionHappened();
	}
	else
		printf("Not in Battle!\n");
	printf("\n");
}

void awaitInput() {
	if(inBattle())
		printf("Enter a battle command: ");
	else
		printf("Enter a command: ");

	// Cant use scanf because of the space formatting
	fgets(currentInput, 20, stdin);

	// Remove trailing newline, if there
	if(currentInput[strlen(currentInput) - 1] == EOL)
		currentInput[strlen(currentInput) - 1] = 0x00;
}

void parseInput() {
	char *p;

	inputQueueCount = 0;
	p = strtok(currentInput, " ");

	while(p != NULL) {
		inputQueue[inputQueueCount] = p;
		inputQueueCount++;
		p = strtok(NULL, " ");
	}
}

void processInput() {
	string input = inputQueue[0];

	if(processGameCommands(input)){}
	else if(processAreaCommands(input)){}
	else if(processItemCommands(input)){}
	else if(processBattleCommands(input)){}
	else {
		if(inBattle())
			printf("Not a valid battle command, try: help/list\n");
		else
			printf("Not a valid command, try: help/list\n");
	}
}

int processGameCommands(string input) {
	if(!strcmp(input, "exit"))
		endTheGame();
	else if(!strcmp(input, "help") || !strcmp(input, "list"))
		listCommands();
	else if(!strcmp(input, "inventory"))
		inventoryCommand();
	else if(!strcmp(input, "equipment"))
		listEquipment(player);
	else if(!strcmp(input, "give"))
		giveCommand();
	else if(!strcmp(input, "drop"))
		dropCommand();
	else if(!strcmp(input, "items"))
		listItems();
	else if(!strcmp(input, "stats") || !strcmp(input, "info"))
		statsCommand();
	else if(!strcmp(input, "walk") || !strcmp(input, "move"))
		walkCommand();
	else
		return false;
	return true;
}

int processAreaCommands(string input) {
	if(!strcmp(input, "search"))
		searchCommand();
	else if(!strcmp(input, "battle"))
		battleCommand();
	else if(!strcmp(input, "open"))
		openCommand();
	else
		return false;
	return true;
}

int processItemCommands(string input) {
	if(!strcmp(input, "equip"))
		equipCommand();
	else if(!strcmp(input, "remove"))
		removeCommand();
	else if(!strcmp(input, "use"))
		useCommand();
	else
		return false;
	return true;
}

int processBattleCommands(string input) {
	if(!strcmp(input, "attack"))
		playerAttack();
	else
		return false;
	return true;
}

void listCommands() {
	int d;
	printf("\n-- List of Commands --\n");

	printf("Game Commands:\n");
	for(d=0;d<gameCommandCount;d++)
		printf("\t%s\n", gameCommands[d]);

	printf("Area Commands:\n");
	for(d=0;d<areaCommandCount;d++)
		printf("\t%s\n", areaCommands[d]);

	printf("Item Commands:\n");
	for(d=0;d<itemCommandCount;d++)
		printf("\t%s\n", itemCommands[d]);

	printf("Battle Commands:\n");
	for(d=0;d<battleCommandCount;d++)
		printf("\t%s\n", battleCommands[d]);

	printf("\n");
}

void listItems() {
	int d;
	printf("\n-- List of Items --\n");
	for(d=0;d<itemCount;d++)
		printf("%s\n", items[d]);
	printf("\n");
}

void inventoryCommand() {
	string who;

	if(inputQueueCount > 1) {
		who = inputQueue[1];
	}
	else
		who = "player";

	if(!strcmp(who, "player"))
		listInventory(player);
	else if(!strcmp(who, "enemy")) {
		if(inBattle())
			listInventory(currentBattle->enemy);
		else
			printf("There is no enemy!\n");
	}
	else
		printf("Cannot list inventory on that!\n");
}

void listInventory(Actor *actor) {
	int d;
	printf("\n-- Inventory --\n");

	if(actor->inventoryCount == 0)
		printf("Inventory is empty\n");
	else {
		for(d=0;d<actor->inventoryCount;d++) {
			printf("%s(x%i)", actor->Inventory[d]->name, actor->Inventory[d]->count);
			if(equipmentContainsItem(actor, actor->Inventory[d]->name))
				printf("(E)\n");	// E for equipped!
			else
				printf("\n");
		}
	}

	printf("\n");
}

void listEquipment(Actor *actor) {
	printf("\n-- Character Equipment --\n");
	printf("Weapon:\n");
	if(!strcmp(actor->equipment.weapon, "none"))
		printf("\tNone(Fists)\n");
	else
		printf("\t%s\n", actor->equipment.weapon);

	printf("Shield:\n");
	if(!strcmp(actor->equipment.shield, "none"))
		printf("\tNone(Arms)\n");
	else
		printf("\t%s\n", actor->equipment.shield);
	printf("\n");
}

void addHealth(Actor *actor, int amount) {
	actor->health += amount;
	if(actor->health > 100)
		actor->health = 100;
}

void removeHealth(Actor *actor, int amount) {
	actor->health -= amount;
	if(actor->health < 0)
		actor->health = 0;
}

void addMana(Actor *actor, int amount) {
	actor->mana += amount;
	if(actor->mana > 100)
		actor->mana = 100;
}

void removeMana(Actor *actor, int amount) {
	actor->mana -= amount;
	if(actor->mana < 0)
		actor->mana = 0;
}

void walkCommand() {
	string direction = inputQueue[1];
	vertex newLocation;
	int ableToMove = false;
	int continueMoving = true;
	if(exploredAreas < worldSize) {
		if(!strcmp(direction, "n") || !strcmp(direction, "north") || !strcmp(direction, "u") || !strcmp(direction, "up")) {
			newLocation.x = world[currentArea]->location.x;
			newLocation.y = world[currentArea]->location.y-1;
		}
		else if(!strcmp(direction, "s") || !strcmp(direction, "south") || !strcmp(direction, "d") || !strcmp(direction, "down")) {
			newLocation.x = world[currentArea]->location.x;
			newLocation.y = world[currentArea]->location.y+1;
		}
		else if(!strcmp(direction, "e") || !strcmp(direction, "east") || !strcmp(direction, "l") || !strcmp(direction, "left")) {
			newLocation.x = world[currentArea]->location.x-1;
			newLocation.y = world[currentArea]->location.y;
		}
		else if(!strcmp(direction, "w") || !strcmp(direction, "west") || !strcmp(direction, "r") || !strcmp(direction, "right")) {
			newLocation.x = world[currentArea]->location.x+1;
			newLocation.y = world[currentArea]->location.y;
		}
		else {
			printf("Not a direction!\n");
			continueMoving = false;
		}

		if(continueMoving) {
			if(areaExistsAtLocation(newLocation)) {
				currentArea = areaIndexAtLocation(newLocation);
				ableToMove = true;
			}
			else {
				createAreaAtLocation(newLocation);
				currentArea = areaIndexAtLocation(newLocation);
				ableToMove = true;
			}
			if(ableToMove) {
				printf("\n");
				printf("You came across\n");
				searchArea(newLocation);
			}
			else
				printf("Unable to move to (%i, %i)\n", newLocation.x, newLocation.y);
		}
	}
	else
		printf("Explored Area Limit has been reached, sorry\n");
	printf("\n");
}

void giveCommand() {
	int d;
	int itemExists = 0;
	string itemName = inputQueue[1];
	for(d=0;d<itemCount;d++) {
		if(!strcmp(itemName, items[d])) {
			int itemIndex = indexForItem(itemName);
			Item *theItem = newItem(items[itemIndex], 1);
			itemExists = 1;

			if(inputQueueCount == 3)	// Can probably do a better job here
				theItem->count = atoi(inputQueue[2]);
			if(theItem->count > 0)
				addToInventory(player, theItem, 1);
			else
				printf("Unable to give item\n");
		}
	}
	if(!itemExists)
		printf("That item does not exist\n");
}

void dropCommand() {
	int d;
	int itemExists = 0;
	string itemName = inputQueue[1];
	printf("dropping %s\n", itemName);

	for(d=0;d<player->inventoryCount;d++) {
		if(!strcmp(itemName, player->Inventory[d]->name)) {
			int itemIndex = indexForItem(itemName);
			Item *theItem = newItem(items[itemIndex], 1);
			itemExists = 1;

			if(equipmentContainsItem(player, itemName))
				removeEquipment(player, itemName);

			if(inputQueueCount == 3)	// Can probably do a better job here
				theItem->count = atoi(inputQueue[2]);
			if(theItem->count > player->Inventory[d]->count)
				theItem->count = player->Inventory[d]->count;
			if(theItem->count > 0)
				removeFromInventory(player, theItem, 1);
			else
				printf("Unable to drop item\n");
		}
	}
	if(!itemExists)
		printf("You don't have any of that item\n");
}

void useCommand() {
	char *itemToUse = inputQueue[1];
	if(inventoryContainsItem(player, itemToUse)) {
		int index = inventoryIndexForItem(player, itemToUse);
		useItem(itemToUse);
	}
	else
		printf("Cannot use an item you don't have\n");
}

void openCommand() {
	char *itemToOpen = inputQueue[1];
	if(!strcmp(itemToOpen, "chest")) {
		if(isAreaClear(world[currentArea])) {
			int chestNumber = atoi(inputQueue[2]);
			if(chestNumber <= world[currentArea]->chestCount-1 && chestNumber >= 0) {		// Can't put -1
				if(!world[currentArea]->Chests[chestNumber]->opened) {
					if(world[currentArea]->Chests[chestNumber]->lootCount > 0) {
						int d;
						Item *theItem = newItem(world[currentArea]->Chests[chestNumber]->Loot[d]->name, world[currentArea]->Chests[chestNumber]->Loot[d]->count);
						for(d=0;d<world[currentArea]->Chests[chestNumber]->lootCount;d++) {
							addToInventory(player, theItem, true);
						}
					}
					else
						printf("That chest is empty!\n");
					world[currentArea]->Chests[chestNumber]->opened = true;
				}
				else
					printf("Chest is already opened!\n");
			}
			else
				printf("That chest does not exist!\n");
		}
		else
			printf("Cannot open chests until area is cleared!\n");
	}
	else
		printf("Cannot open\n");
}

void equipCommand() {
	string itemToEquip = inputQueue[1];
	if(itemExists(itemToEquip)) {
		int itemIndex = indexForItem(itemToEquip);

		if(inventoryContainsItem(player, itemToEquip)) {
			int type = itemType[itemIndex];
			if(type == Weapon) {
				player->equipment.weapon = items[itemIndex];	// Pointer problems...
				printf("Succesfully equipped %s to weapon slot!\n", itemToEquip);
			}
			else if(type == Shield) {
				player->equipment.shield = items[itemIndex];	// Pointer problems...
				printf("Succesfully equipped %s to shield slot!\n", itemToEquip);
			}
			else
				printf("Cannot equip that item\n");
		}
	}
	else
		printf("That item does not exist\n");
	printf("\n");
}

void removeCommand() {
	string itemToRemove = inputQueue[1];
	removeEquipment(player, itemToRemove);
}

void statsCommand() {
	string name = inputQueue[1];
	if(inputQueueCount > 1) {
		if(!strcmp(name, "enemy")) {
			if(inBattle())
				displayActorStats(currentBattle->enemy);
			else
				printf("There is no enemy!\n");
		}
		else if(!strcmp(name, "player"))
			displayActorStats(player);
		else
			printf("Cannot list stats for %s\n", name);
	}
	else if(inputQueueCount == 1)
		displayActorStats(player);
	else
		printf("Cannot list stats for %s\n", name);

	printf("\n");
}

void searchCommand() {
	// No need to scan for more input yet
	searchArea(world[currentArea]->location);
}

void removeEquipment(Actor *actor, string item) {
	if(itemExists(item)) {
		int itemIndex = indexForItem(item);

		if(equipmentContainsItem(actor, item)) {
			int type = itemType[itemIndex];
			if(type == Weapon) {
				actor->equipment.weapon = "none";
				printf("Succesfully removed %s from %s's weapon slot!\n", item, actor->name);
			}
			else if(type == Shield) {
				actor->equipment.shield = "none";
				printf("Succesfully removed %s from %s's shield slot!\n", item, actor->name);
			}
			else
				printf("Cannot remove that item\n");
		}
	}
	else
		printf("That item does not exist\n");
	printf("\n");
}

void battleCommand() {
	string itemToBattle = inputQueue[1];
	int indexToBattle = atoi(inputQueue[2]);
	if(!strcmp(itemToBattle, "monster")) {
		if(monsterExistsInArea(indexToBattle, world[currentArea])) {
			if(world[currentArea]->Monsters[indexToBattle]->health > 0)
				enterBattle(indexToBattle);
			else
				printf("That enemy is already dead!\n");
		}
		else
			printf("No Monster to battle!\n");
	}
	else
		printf("You can't battle that!\n");
	printf("\n");
}

int itemExists(string theItem) {
	int d, exist = false;
	for(d=0;d<itemCount;d++) {
		if(!strcmp(theItem, items[d]))
			exist = true;
	}
	return exist;
}

int indexForItem(string theItem) {
	int d, retVal;
	for(d=0;d<itemCount;d++) {
		if(!strcmp(theItem, items[d]))
			retVal = d;
	}
	return retVal;
}

void useItem(string theItem) {
	Item *item = newItem(theItem, 1);

	if(!strcmp(theItem, "potion")) {
		if(player->health < 100) {
			addHealth(player, 20);
			removeFromInventory(player, item, true);
			printf("Your health has increased by 20. Health: %i\n", player->health);
		}
		else
			printf("Your health is already full!\n");
	}
	else if(!strcmp(theItem, "elixer")) {
		if(player->mana < 100) {
			addMana(player, 20);
			removeFromInventory(player, item, true);
			printf("Your mana has increased by 20. Mana: %i\n", player->mana);
		}
		else
			printf("Your mana is already full!\n");
	}
	else if(!strcmp(theItem, "poison")) {
		removeHealth(player, 20);
		removeFromInventory(player, item, true);
		printf("Your health has decreased by 20. Health: %i\n", player->health);
	}
}

void addToInventory(Actor *actor, Item *item, int verbose) {
	int d, index;
	int exist = 0;

	if(inventoryContainsItem(actor, item->name)) {
		exist = 1;
		index = inventoryIndexForItem(actor, item->name);
	}
	if(!exist) {
		index = actor->inventoryCount;	// Already +1 to index, from inventoryCount
		actor->Inventory[index] = item;
		actor->inventoryCount++;
	}
	else
		actor->Inventory[index]->count += item->count;
	if(verbose)
		printf("%s(x%i) was successfully added to %s's inventory!\n", item->name, item->count, actor->name);
}

void removeFromInventory(Actor *actor, Item *item, int verbose) {
	int d, index;
	int exist = 0;

	if(inventoryContainsItem(actor, item->name)) {
		exist = 1;
		index = inventoryIndexForItem(actor, item->name);
	}

	if(!exist)
		printf("Cannot remove from %i's inventory\n", actor->name);
	else
		actor->Inventory[index]->count -= item->count;

	if(actor->Inventory[index]->count <= 0)
		removeIndexFromInventoryArray(actor, index);

	if(verbose)
		printf("%s(x%i) was successfully removed from %s's inventory!\n", item->name, item->count, actor->name);
}

void removeIndexFromInventoryArray(Actor *actor, int theIndex) {
	// Shift everything to the left from index
	int i = theIndex;

	while(++i < actor->inventoryCount)
		actor->Inventory[i-1] = actor->Inventory[i];
	actor->inventoryCount--;
}

void removeIndexFromChest(Chest *chest, int theIndex) {
	// Shift everything to the left from index
	int i = theIndex;

	while(++i < chest->lootCount)
		chest->Loot[i-1] = chest->Loot[i];
	chest->lootCount--;
}

void displayActorStats(Actor *actor) {
	printf("\n-- %s Stats --\n", actor->name);
	printf("Health: %i\n", actor->health);
	printf("Mana: %i\n", actor->mana);
	printf("Level: %i\n", actor->level);
	printf("Exp: %i\n", actor->exp);
	//printf("Unassigned Perks: %i\n", actor->unassignedPerks);
	printf("Strength: %i\n", actor->strength);
	printf("Intelligence: %i\n", actor->intelligence);
	printf("Agility: %i\n", actor->agility);
	printf("Luck: %i\n", actor->luck);
}

int inventoryContainsItem(Actor *actor, string theItem) {
	int retVal = 0;
	int d;
	for(d=0;d<actor->inventoryCount;d++) {
		if(!strcmp(actor->Inventory[d]->name, theItem))
			retVal = 1;
	}
	return retVal;
}

int inventoryIndexForItem(Actor *actor, string theItem) {
	int retVal;
	int d;
	for(d=0;d<actor->inventoryCount;d++) {
		if(!strcmp(actor->Inventory[d]->name, theItem))
			retVal = d;
	}
	return retVal;
}

int equipmentContainsItem(Actor *actor, string theItem) {
	int retVal = false;
	if(!strcmp(actor->equipment.weapon, theItem))
		retVal = true;
	if(!strcmp(actor->equipment.shield, theItem))
		retVal = true;
	return retVal;
}

void searchArea(vertex location) {
	Area *area = areaAtLocation(location);

	printf("\nLocation: (%i, %i): ", area->location.x, area->location.y);
	if(!isAreaClear(area))
		printf("Not Cleared\n");
	else
		printf("Cleared!\n");

	printf("Area: %s\n", areaType[area->type]);

	printf("-- Monsters --\n");
	if(aliveMonsterCount(area) == 0)
		printf("\tNo Alive Monsters\n");
	else {
		int d;
		for(d=0;d<area->monsterCount;d++) {
			printf("\t%i) %s, Lvl %i\n", d, area->Monsters[d]->name, area->Monsters[d]->level);
		}
	}

	printf("-- Chests --\n");
	if(area->chestCount == 0)
		printf("\tNone\n");
	else {
		int d;
		for(d=0;d<area->chestCount;d++) {
			printf("\t%i) Chest %i is ", d, d);
			if(area->Chests[d]->opened == true)
				printf("opened\n");
			else
				printf("closed\n");
		}
	}
	printf("\n");
}

int isAreaClear(Area *area) {
	if(aliveMonsterCount(area) == 0)
		return true;
	return false;
}

int aliveMonsterCount(Area *area) {
	int d, retVal = 0;
	for(d=0;d<area->monsterCount;d++) {
		if(area->Monsters[d]->health > 0)
			retVal++;
	}
	return retVal;
}

void clearArea(Area *area) {
	area->cleared = true;
}

void tryToClearArea(Area *area) {
	if(isAreaClear(area))
		clearArea(area);
}

int monsterExistsInArea(int monsterNumber, Area *area) {
	if(monsterNumber <= area->monsterCount-1)
		return true;
	return false;
}

int inBattle() {
	if(currentBattle->status == active)
		return true;
	return false;
}

int isAlive(Actor *actor) {
	if(actor->health > 0)
		return true;
	return false;
}

void battleActionHappened() {
	currentBattle->actionHappened = true;
}

int battleActionHasHappened() {
	if(inBattle)
		return currentBattle->actionHappened;
}

void lootEnemy() {
	float lootRarity = 0;
	int exp, d;
	int lootCount = currentBattle->enemy->inventoryCount;

	string enemyName = currentBattle->enemy->name;

	// Use lootRarity sometime
	lootRarity += player->luck*10;
	lootRarity += currentBattle->enemy->level*10;
	lootRarity += currentBattle->enemy->strength*10;
	lootRarity += currentBattle->enemy->intelligence*5;
	lootRarity += currentBattle->enemy->agility*2;

	if(lootCount > 0) {
		// Eventually add functionality to remove item from monster after taking them
		for(d=0;d<lootCount;d++) {
			addToInventory(player, currentBattle->enemy->Inventory[d], true);
		}
	}
	else
		printf("Monster had no loot\n");

	exp = 5*currentBattle->enemy->level;
	printf("You gained %i EXP!\n", exp);
	player->exp += exp;
}

Item *itemWithRarity(lootRarity) {
	Item *retVal = newItem("blank", 0);
	return retVal;
}

int areaExistsAtLocation(vertex location) {
	int d;

	for(d=0;d<exploredAreas;d++) {
		if(world[d]->location.x == location.x && world[d]->location.y == location.y)
			return true;
	}
	return false;
}

Area *areaAtLocation(vertex location) {
	Area *retVal = NULL;
	int d;

	for(d=0;d<exploredAreas;d++) {
		if(world[d]->location.x == location.x && world[d]->location.y == location.y)
			return world[d];
	}
	return retVal;
}

int areaIndexAtLocation(vertex location) {
	int d;
	for(d=0;d<exploredAreas;d++) {
		if(world[d]->location.x == location.x && world[d]->location.y == location.y)
			return d;
	}
	printf("Couldn't find area, reverting to (0, 0)\n");
	return 0;	// Default index
}

void createAreaAtLocation(vertex location) {
	if(!areaExistsAtLocation(location)) {
		Area *area = newArea(location, random, 1, 0, true, true);
		world[exploredAreas] = area;
		exploredAreas++;
	}
	else
		printf("Area already exists at that location!\n");
}
