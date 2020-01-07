/*	Known Bugs:
	If a player grabs enough items that don't already exist in the world the excess items will be destroyed
	Text rendering is all effed up*/
// Written within 2 weeks around 18Sep2013

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>

#define SpriteWidth			8
#define SpriteHeight		8
#define SpriteResolution	(SpriteWidth*SpriteHeight)

#define UP		771
#define DOWN	772
#define LEFT	773
#define RIGHT	774
#define STILL	775

#define Sprite_QuestionMark			"0000000000111000010001000000010000011000000000000001000000000000"
#define Sprite_PlayerFaceUp			"0011110001000010101001011000000110000001100000010100001000111100"
#define Sprite_PlayerFaceDown		"0011110001000010100000011000000110100101100000010100001000111100"
#define Sprite_PlayerFaceLeft		"0011110001000110100000011100100110000001100000010100001000111100"
#define Sprite_PlayerFaceRight		"0011110001100010100000011001001110000001100000010100001000111100"
#define Sprite_PlayerFaceStill		"0011110001000010100000011010010110000001100000010100001000111100"
#define Sprite_Wall					"1111111111111111112222111122221111222211112222111111111111111111"
#define Sprite_DroppedItem			"0000100010000010000001000010000100000000100010100010000000000100"
#define Sprite_Cactus				"0000000000010000001110101011101011111110011111000011100000111000"
#define Sprite_Water				"1110000000001111111100000000001100111100110000000000111101111000"
#define Sprite_Lilypad				"0000000000111100001111101111111011111110111111100111110000000000"
#define Sprite_Cup					"0000000000000000000000001100001111000011110000110110011000111100"
#define Sprite_Dirt					"2111121112211112111111121222221111121111221112221222111111211121"
#define Sprite_Fire					"0000001001010000000001001001001000101100011111000111101011011111"
#define Sprite_Steam				"0000100001010001010001010101010001010110010100101101010110010001"
#define Sprite_Sand					"1212221222222121212212222222222112212212221222222222212121221222"
#define Sprite_Clay					"1111111112111121111121111111111112111121111211111111112111111111"
#define Sprite_Bedrock				"1121112222221111122222221112211222222111111122111222222222222111"

#define QUICKSELECT_COUNT		3

using namespace std;

// Set values are used so that data tags will never coincide with item tags
enum {
	TAG_NULL =				1000,
	TAG_STARTOFSAVEFILE =	1001,
	TAG_STARTOFITEMS =		1002,
	TAG_NEWITEM =			1003,
	TAG_ENDOFITEMS =		1004,
	TAG_STARTOFINVENTORY =	1005,
	TAG_ENDOFINVENTORY =	1006,
	TAG_ENDOFSAVEFILE =		1007,
	TAG_STARTOFITEMDATA =	1008,
	TAG_ENDOFITEMDATA =		1009,
};

// Don't need to be part of data storage enums, values shouldn't reach that high
enum {
	TAG_ITEMDATA_DROPPEDSTATUS_DROPPED,
	TAG_ITEMDATA_DROPPEDSTATUS_NOTDROPPED,
	TAG_ITEMDATA_HEALTH,
};

enum {
	TAG_DEFAULT,
	TAG_PLAYER,
	TAG_DIRT,
	TAG_WALL,
	TAG_WATER,
	TAG_FIRE,
	TAG_LILYPAD,
	TAG_CACTUS,
	TAG_CUP,
	TAG_STEAM,
	TAG_SAND,
	TAG_CLAY,
	TAG_BEDROCK,
};

enum {
	GAMESTATUS_NORMAL,
	GAMESTATUS_INVENTORY,
	GAMESTATUS_MENU,
};

enum {
	USE,
	PLACE,
};

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

void glInit();
void OnIdle();
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void display(void);

int masterTag = 100;
int screenWidth, screenHeight;
char keyboard[256] = {0};
float zoomLevel = 5.0f;
float glWidth, glHeight;
int currentQuickSelect = 0;
int currentInventoryX = 0, currentInventoryY = 0;
int gameStatus = GAMESTATUS_NORMAL;
float HUDWidth, HUDHeight;

vector<string>menuOptions;
int currentMenuSelection = 0, menuSelectionCount = 4;

typedef struct {
	float x, y;
}Point;
Point cameraCenter;

Point PointWith(float _x, float _y) {
		Point retVal;
		retVal.x = _x;
		retVal.y = _y;
		return retVal;
}

class Color {
public:
	Color() {r = 1.0f;g = 1.0f;b = 1.0f;}
	Color(float _r, float _g, float _b) {r=_r;g=_g;b=_b;}
	Color(int _r, int _g, int _b) {r = _r/255.0f;g = _g/255.0f;b = _b/255.0f;}
	Color(unsigned int hex) {r=(hex>>16&0xFF)/255.0f;g=(hex>>8&0xFF)/255.0f;b=(hex>>0&0xFF)/255.0f;}
	float r, g, b;
};

class Sprite {
public:
	Sprite() {
		setInitialColor(new Color(0xFF00FF));
		for(int d=0;d<SpriteResolution;d++)bitmap[d] = 0;
		setBitmap(Sprite_QuestionMark);
	}
	Sprite(Color *_color, string data) {
		setInitialColor(_color);
		standardSetup(data);
	}
	Sprite(vector<Color *>_palette, string data) {
		setInitialPalette(_palette);
		standardSetup(data);
	}
	void standardSetup(string data) {
		shouldInterpolate = false;
		interpolationScalar = 1.0f;
		interpolationColor = new Color(1.0f, 0.0f, 0.0f);
		for(int d=0;d<SpriteResolution;d++)bitmap[d] = 0;
		setBitmap(data);
	}
	void setColor(Color *_color) {
		vector<Color *>_palette;
		_palette.push_back(_color);
		setPalette(_palette);
	}
	void setInitialColor(Color *_color) {
		setColor(_color);
		initialPalette = palette;
	}
	void setPalette(vector<Color *>_palette) {
		palette = (_palette);
	}
	void setInitialPalette(vector<Color *>_palette) {
		setPalette(_palette);
		initialPalette = palette;
	}
	void resetPalette() {
		palette = initialPalette;
	}
	void setBitmap(string data) {
		for(int d=0;d<SpriteResolution;d++) {
			// This means only 9 values can be used in the palette as 0 is reserved for transparent
			bitmap[d] = data[d] - '0';
		}
	}
	void render(Point loc, Point size) {
		Point dotSize = PointWith(size.x/SpriteWidth, size.y/SpriteHeight);

		for(int y=0;y<SpriteHeight;y++) {
			for(int x=0;x<SpriteWidth;x++) {
				int colorValue = bitmap[y*SpriteWidth+x];
				bool shouldDraw = true;
				if(colorValue == 0)shouldDraw = false;
				else {
					Color *color = palette[colorValue-1];
					assert(colorValue <= palette.size());	// Tried to use a color that doesn't exist
					glColor3f(color->r, color->g, color->b);
					if(shouldInterpolate) {
						float rPrime, gPrime, bPrime;
						rPrime = color->r + interpolationScalar * (interpolationColor->r - color->r);
						gPrime = color->g + interpolationScalar * (interpolationColor->g - color->g);
						bPrime = color->b + interpolationScalar * (interpolationColor->b - color->b);
						glColor3f(rPrime, gPrime, bPrime);
					}
				}
				if(shouldDraw) {
					int invertedY = SpriteHeight-y-1;
					float theX = loc.x+(dotSize.x*x), theY = loc.y+(dotSize.y*invertedY);
					glBegin(GL_QUADS);
						glVertex2f(theX, theY);						// Lower Left
						glVertex2f(theX+dotSize.x, theY);			// Lower Right
						glVertex2f(theX+dotSize.x, theY+dotSize.y);	// Upper Right
						glVertex2f(theX, theY+dotSize.y);			// Upper Left
					glEnd();
				}
			}
		}
	}
	bool shouldInterpolate;
	Color *interpolationColor;
	float interpolationScalar;
	vector<Color *>palette, initialPalette;
	int bitmap[SpriteResolution];
};

class Entity;
vector<Entity *>entitiesAtLocation(int x, int y);
class Entity {
public:
	Entity() {
		name = "Entity";
		isAlive = true;
		isTrespassable = false;
		isPlaceable = true;
		isDestructable = true;
		isDroppable = true;
		isDropped = false;
		isConsumable = false;
		reviveSpeed = 0.01f;
		isHurtByWater = false;
		shouldBeHurtByWater = false;
		waterDamage = 0.02f;
		isHurtByFire = true;
		shouldBeHurtByFire = false;
		fireDamage = 0.06f;
		isAffectedByGravity = false;
		gravityAffectRate = 0.1f;
		moveDuration = 0.15f;
		isJumping = false;
		jumpDuration = 0.5f;

		collisionDamage = 0;
		constantDamage = 0;
		maxStackable = INT_MAX;
		loc.x = 0;
		loc.y = 0;
		vel.x = 0;
		vel.y = 0;
		setStartIcon(new Sprite());
		lastTick = lastHealthTick = lastGravityTick = lastJumpTick = lastMoveTick = clock();
		maxHealth = 1.0f;
		health = maxHealth;
		healthAffectRate = 0.1f;
		tag = masterTag++;
		typeTag = TAG_DEFAULT;
		maxStackable = 64;
	}
	virtual vector<int>getItemData() {
		vector<int>retVal;

		if(isDropped)retVal.push_back(TAG_ITEMDATA_DROPPEDSTATUS_DROPPED);
		if(health != maxHealth) {
			retVal.push_back(TAG_ITEMDATA_HEALTH);
			retVal.push_back(health*100.0f);
		}

		return retVal;
	}
	virtual void initWithItemData(vector<int>itemData) {
		for(int d=0;d<itemData.size();d++) {
			if(itemData[d] == TAG_ITEMDATA_DROPPEDSTATUS_DROPPED)setDropped();
			if(itemData[d] == TAG_ITEMDATA_HEALTH) {
				health = itemData[d+1]/100.0f;
				d++;
			}
		}
	}
	Point gridLoc() {
		return PointWith(loc.x*SpriteWidth, loc.y*SpriteHeight);
	}
	void setStartIcon(Sprite *_icon) {
		startIcon = _icon;
		icon = _icon;
	}
	void resetIcon() {
		icon = startIcon;
		icon->shouldInterpolate = false;
	}
	virtual void render() {
		render(gridLoc(), PointWith(SpriteWidth, SpriteHeight));
	}
	void render(Point _loc, Point _size) {
		if(icon->shouldInterpolate)icon->interpolationScalar = maxHealth-health;
		icon->render(_loc, _size);
	}
	virtual void tick() {
		now = clock();
		delta = (double)(now-lastTick)/CLOCKS_PER_SEC;
		lastTick = now;
		if(isAffectedByGravity)tickGravity();
		manageHealth();
		manageMovement();
	}
	bool canMove(Point newLoc) {
		vector<Entity *>hasCollidedWith = entitiesAtLocation(newLoc.x, newLoc.y);
		bool canMove = true;
		for(int d=0;d<hasCollidedWith.size();d++) {
			collidedWith(hasCollidedWith[d]);
			if(!hasCollidedWith[d]->isTrespassable)
				canMove = false;
		}
		return canMove;
	}
	void tickGravity() {
		gravityDelta = (double)(now-lastGravityTick)/CLOCKS_PER_SEC;
		if(!isJumping) {
			if(gravityDelta >= gravityAffectRate) {
				vel.y--;
				move();
				lastGravityTick = now;
			}
		}
		else {
			jumpDelta = (double)(now-lastJumpTick)/CLOCKS_PER_SEC;
			if(jumpDelta >= jumpDuration) {
				isJumping = false;
				lastJumpTick = now;
			}
		}
	}
	virtual void manageMovement() {
		moveDelta = (double)(now-lastMoveTick)/CLOCKS_PER_SEC;
		if(moveDelta >= moveDuration) {
			if(name == "Player")
				cout << "Firing" << endl;
			lastMoveTick = now;
			move();
		}
	}
	void jump() {
		bool sturdyFoothold = false;
		vector<Entity *>entitiesBelow = entitiesAtLocation(loc.x, loc.y-1);
		for(int d=0;d<entitiesBelow.size();d++) {
			if(!entitiesBelow[d]->isTrespassable)
				sturdyFoothold = true;
		}
		bool clearOverhead = true;
		vector<Entity *>entitiesAbove = entitiesAtLocation(loc.x, loc.y+1);
		for(int d=0;d<entitiesAbove.size();d++) {
			if(!entitiesAbove[d]->isTrespassable)
				clearOverhead = false;
		}

		if(sturdyFoothold && clearOverhead) {
			isJumping = true;
			lastJumpTick = clock();
			vel.y++;
		}
	}
	virtual void collidedWith(Entity *entity) {
		if(entity->typeTag == TAG_PLAYER) {
			if(isDropped)entity->collidedWith(this);
		}

		if(entity->isDropped) {}
		else
			health -= entity->collisionDamage;
	}
	void move() {
		vel.x = min(vel.x, 1.0f);
		vel.x = max(vel.x, -1.0f);
		vel.y = min(vel.y, 1.0f);
		vel.y = max(vel.y, -1.0f);
		if(vel.x != 0 || vel.y != 0) {
			Point newLoc = loc;
			newLoc.x += vel.x;
			newLoc.y += vel.y;
			vel.x = 0;
			vel.y = 0;

			if(canMove(newLoc))loc = newLoc;
		}
	}
	virtual bool itemCanBePlacedOnTopOfThis(int tag) {
		return false;
	}
	void manageHealth() {
		healthDelta = (double)(now-lastHealthTick)/CLOCKS_PER_SEC;

		if(healthDelta >= healthAffectRate) {
			float totalPain = 0;

			if(isHurtByWater && shouldBeHurtByWater) {
				totalPain += waterDamage;
				shouldBeHurtByWater = false;
			}
			if(isHurtByFire && shouldBeHurtByFire) {
				totalPain += fireDamage;
				shouldBeHurtByFire = false;
			}

			health -= totalPain;

			health += reviveSpeed;

			lastHealthTick = now;
		}
		if(health < maxHealth) {
			if(!icon->shouldInterpolate) {
				icon->interpolationColor = new Color(1.0f, 0.0f, 0.0f);
				icon->shouldInterpolate = true;
			}
		}
		else icon->shouldInterpolate = false;

		if(health <= 0)die();
		health = min(health, maxHealth);
	}
	virtual void die() {
		isAlive = false;
		if(isDroppable && !isDropped)setDropped();
	}
	void setDropped() {
		isAlive = true;
		isDropped = true;
		isTrespassable = true;
		isAffectedByGravity = true;
		health = maxHealth;
		icon = new Sprite(new Color(0x00FFFF), Sprite_DroppedItem);
	}

	bool isAlive, isTrespassable, isPlaceable, isDestructable, isDroppable, isDropped, isConsumable;
	bool isHurtByWater, isHurtByFire;
	bool isAffectedByGravity, isJumping;
	bool shouldBeHurtByWater, shouldBeHurtByFire;
	float reviveSpeed, waterDamage, fireDamage;
	float gravityAffectRate, jumpDuration, moveDuration;
	Sprite *icon, *startIcon;
	Point loc;
	Point vel;
	clock_t now, lastTick, lastHealthTick, lastGravityTick, lastJumpTick, lastMoveTick;
	float delta, healthDelta, gravityDelta, jumpDelta, moveDelta;
	int maxStackable;
	float health, maxHealth, collisionDamage, constantDamage;
	float healthAffectRate;
	int tag, typeTag;
	string name;
};
vector<Entity *>entities;

vector<Entity *>entitiesAtLocation(int x, int y) {
	vector<Entity *>retVal;
	for(int d=0;d<entities.size();d++) {
		if(entities[d]->loc.x == x && entities[d]->loc.y == y)
			retVal.push_back(entities[d]);
	}
	return retVal;
}

int indexForEntity(Entity *entity) {
	for(int d=0;d<entities.size();d++) {
		if(entities[d] == entity)
			return d;
	}
	assert(1);
}

class InventoryItem {
public:
	InventoryItem() {
		inventoryItemName = "InventoryItem";
		inventoryItemTag = TAG_DEFAULT;
		icon = new Sprite();
		count = 0;
		health = 100;
	}
	InventoryItem(int _inventoryItemTag, int _count, int _health, Sprite *_icon, string name) {
		inventoryItemName = name;
		icon = _icon;
		inventoryItemTag = _inventoryItemTag;
		count = _count;
		health = _health;
	}
	Sprite *icon;
	int inventoryItemTag, count, health;
	string inventoryItemName;
};

class Wall : public Entity {
public:
	Wall(Point _loc) {
		name = "Wall";
		loc = _loc;
		vector<Color *>palette;
		palette.push_back(new Color(0xFFFF00));
		palette.push_back(new Color(0xAAAA00));
		setStartIcon(new Sprite(palette, Sprite_Wall));
		typeTag = TAG_WALL;
	}
};

class Cactus : public Entity {
public:
	Cactus(Point _loc) {
		name = "Cactus";
		loc = _loc;
		setStartIcon(new Sprite(new Color(0x33FF33), Sprite_Cactus));
		typeTag = TAG_CACTUS;
		collisionDamage = 0.25f;
	}
};

class Sand : public Entity {
public:
	Sand(Point _loc) {
		name = "Sand";
		loc = _loc;
		vector<Color *>palette;
		palette.push_back(new Color(0xECD966));
		palette.push_back(new Color(0xE0CC50));
		setStartIcon(new Sprite(palette, Sprite_Sand));
		isTrespassable = true;
		typeTag = TAG_SAND;
	}
};

class Clay : public Entity {
public:
	Clay(Point _loc) {
		name = "Clay";
		loc = _loc;
		vector<Color *>palette;
		palette.push_back(new Color(0xC3C3C3));
		palette.push_back(new Color(0x7F7F7F));
		setStartIcon(new Sprite(palette, Sprite_Clay));
		isTrespassable = true;
		typeTag = TAG_CLAY;
	}
};

class Water : public Entity {
public:
	Water(Point _loc) {
		name = "Water";
		loc = _loc;
		setStartIcon(new Sprite(new Color(0x1111FF), Sprite_Water));
		typeTag = TAG_WATER;
		isTrespassable = true;
		isDestructable = false;
	}
	virtual bool itemCanBePlacedOnTopOfThis(int tag) {
		if(tag == TAG_LILYPAD)
			return true;
		return false;
	}
	virtual void tick() {
		Entity::tick();
		vector<Entity *>surrounding = entitiesAtLocation(loc.x, loc.y);
		for(int d=0;d<surrounding.size();d++) {
			surrounding[d]->shouldBeHurtByWater = true;
		}
	}
};

class Fire : public Entity {
public:
	Fire(Point _loc) {
		name = "Fire";
		loc = _loc;
		setStartIcon(new Sprite(new Color(0xEE4444), Sprite_Fire));
		typeTag = TAG_FIRE;
		isTrespassable = true;
		isDestructable = false;
		isDroppable = false;
		isHurtByFire = false;
	}
	virtual void tick() {
		Entity::tick();
		vector<Entity *>surrounding = entitiesAtLocation(loc.x, loc.y);
		for(int d=0;d<surrounding.size();d++) {
			surrounding[d]->shouldBeHurtByFire = true;
		}
	}
};

class Steam : public Entity {
public:
	Steam(Point _loc) {
		name = "Steam";
		loc = _loc;
		setStartIcon(new Sprite(new Color(0x666666), Sprite_Steam));
		typeTag = TAG_STEAM;
		isTrespassable = true;
	}
};

class Lilypad : public Entity {
public:
	Lilypad(Point _loc) {
		name = "Lilypad";
		loc = _loc;
		setStartIcon(new Sprite(new Color(0x55DD55), Sprite_Lilypad));
		typeTag = TAG_LILYPAD;
	}
};

class Cup : public Entity {
public:
	Cup(Point _loc) {
		name = "Cup";
		loc = _loc;
		setStartIcon(new Sprite(new Color(0xAABBCC), Sprite_Cup));
		typeTag = TAG_CUP;
	}
};

class Bedrock : public Entity {
public:
	Bedrock(Point _loc) {
		name = "Bedrock";
		loc = _loc;
		isDestructable = false;
		vector<Color *>palette;
		palette.push_back(new Color(0x444444));
		palette.push_back(new Color(0x333333));
		setStartIcon(new Sprite(palette, Sprite_Bedrock));
		typeTag = TAG_BEDROCK;
	}
};

class Dirt : public Entity {
public:
	Dirt(Point _loc) {
		name = "Dirt";
		loc = _loc;
		vector<Color *>palette;
		palette.push_back(new Color(0x784800));
		palette.push_back(new Color(0x573400));
		setStartIcon(new Sprite(palette, Sprite_Dirt));
		typeTag = TAG_DIRT;
	}
};

Entity *spawnItem(int tag, Point loc, bool dropped);
Entity *createItem(int tag, Point loc, bool dropped);

class Player : public Entity {
public:
	Player(Point _loc) {
		name = "Player";
		loc = _loc;

		vector<Color *>palette;
		palette.push_back(new Color(0x00FF00));
		setStartIcon(new Sprite(palette, Sprite_PlayerFaceStill));

		typeTag = TAG_PLAYER;
		interactDirection = STILL;
		isDroppable = false;
		isHurtByWater = true;
		isAffectedByGravity = true;
		for(int d=0;d<16+QUICKSELECT_COUNT;d++)inventory[d] = NULL;
	}
	virtual void render() {
		Entity::render();
	}
	virtual void tick() {
		Entity::tick();
		if(interactDirection != STILL)interact();
	}
	virtual void manageMovement() {
		moveDelta = (double)(now-lastMoveTick)/CLOCKS_PER_SEC;
		if(moveDelta >= moveDuration) {
			if(keyboard['a'])vel.x--;
			if(keyboard['d'])vel.x++;
			if(keyboard[' '] || keyboard['w'])jump();
			lastMoveTick = now;
			move();
		}
	}
	virtual void collidedWith(Entity *entity) {
		Entity::collidedWith(entity);
		if(entity->isDropped)
			addToInventory(entity, 1);
	}
	void interact() {
		interactLoc = loc;
		if(interactDirection == UP)interactLoc.y++;
		if(interactDirection == DOWN)interactLoc.y--;
		if(interactDirection == LEFT)interactLoc.x--;
		if(interactDirection == RIGHT)interactLoc.x++;
		vector<Entity *>hasInteractedWith = entitiesAtLocation(interactLoc.x, interactLoc.y);
		// Trying to interact with something
		if(interactionType == USE) {
			if(hasInteractedWith.size() == 0) {
				if(interactDirection == UP)interactLoc.y++;
				if(interactDirection == DOWN)interactLoc.y--;
				if(interactDirection == LEFT)interactLoc.x--;
				if(interactDirection == RIGHT)interactLoc.x++;
				hasInteractedWith = entitiesAtLocation(interactLoc.x, interactLoc.y);
			}
			// We only care about the item on top
			if(hasInteractedWith.size() > 0) {
				Entity *entity = hasInteractedWith.back();
				interactWith(entity);
			}
		}
		// Trying to place something
		if(interactionType == PLACE) {
			if(inventory[currentQuickSelect] != NULL)
				if(tryToPlace(inventory[currentQuickSelect])) {
					if(inventory[currentQuickSelect]->count <= 0)
						inventory[currentQuickSelect] = NULL;
				}
		}
		interactDirection = STILL;
	}
	void interactWith(Entity *entity) {
		if(entity->isDropped) {
			addToInventory(entity, 1);
		}
		else {
			bool itemUsed = false;
			if(inventory[currentQuickSelect] != NULL) {
				int usingTag = inventory[currentQuickSelect]->inventoryItemTag;
				if(usingTag == TAG_CUP && entity->typeTag == TAG_WATER) {
					if(!inventoryFull()) {
						addToInventory(entity, 1);
						itemUsed = true;
						entity->die();
					}
				}
			}
			if(!itemUsed) {
				if(entity->isDestructable) {
					entity->health -= 0.5f;
				}
			}
		}
	}
	bool tryToPlace(InventoryItem *item) {
		Entity *entity = createItem(item->inventoryItemTag, interactLoc, false);

		if(entity != NULL) {
			if(entity->isPlaceable) {
				bool canPlace = true;
				vector<Entity *>atLocation = entitiesAtLocation(entity->loc.x, entity->loc.y);
				Entity *entityAtLocation = NULL;
				if(atLocation.size() > 0)entityAtLocation = atLocation.back();
				if(entityAtLocation != NULL) {
					// Need to reverse this as well so it works the other way around
					if(entityAtLocation->typeTag == TAG_FIRE && item->inventoryItemTag == TAG_WATER) {
						entityAtLocation->die();
						entity = createItem(TAG_STEAM, interactLoc, false);
						//spawnItem(TAG_STEAM, interactLoc, false);
					}
					else if(!entityAtLocation->itemCanBePlacedOnTopOfThis(entity->typeTag))
						canPlace = false;
				}

				if(canPlace) {
					if(entity != NULL)entities.push_back(entity);
					item->count--;
					return true;
				}
			}
		}
		return false;
	}
	int inventorySlotFor(Entity *entity) {
		for(int d=0;d<16+QUICKSELECT_COUNT;d++) {
			// Need to do work here with unstackable items
			if(inventory[d] != NULL) {
				if(inventory[d]->inventoryItemTag == entity->typeTag) {
					if(inventory[d]->health == entity->health) {
						if(inventory[d]->count < entity->maxStackable) {
							return d;
						}
					}
				}
			}
		}
		return -1;
	}
	int firstAvailableInventorySlot() {
		for(int d=0;d<16+QUICKSELECT_COUNT;d++) {
			if(inventory[d] == NULL)
				return d;
		}
		return -1;
	}
	int firstAvailableNonQuickslot() {
		for(int d=QUICKSELECT_COUNT;d<16+QUICKSELECT_COUNT;d++) {
			if(inventory[d] == NULL)
				return d;
		}
		return -1;
	}
	int firstAvailableQuickslot() {
		for(int d=0;d<QUICKSELECT_COUNT;d++) {
			if(inventory[d] == NULL)
				return d;
		}
		return -1;
	}
	bool inventoryFull() {
		int index = firstAvailableInventorySlot();
		if(index == -1)return true;
		return false;
	}
	void addToInventory(Entity *entity, int count) {
		int cantFit = 0;
		bool retVal = true;
		if(entity->isConsumable)entity->health;
		entity->health = entity->maxHealth;
		for(int d=0;d<count;d++) {
			int index = -1;
			if(index == -1)index = inventorySlotFor(entity);
			if(index == -1)index = firstAvailableInventorySlot();
			if(index == -1) {		// Could not find a slot
				cantFit=count-d;
				//for(int dd=0;dd<cantFit;dd++)spawnItem(entity->typeTag, interactLoc, true);
				cout << "Cannot fit " << cantFit << endl;
				break;
			}
			else {
				entity->resetIcon();
				InventoryItem *item = new InventoryItem(entity->typeTag, 1, entity->health, entity->icon, entity->name);
				if(inventory[index])inventory[index]->count++;
				else inventory[index] = item;
				entity->die();
			}
		}
	}
	void spliceIntoInventory(int tag, int count, int slot) {
		Entity *entity = createItem(tag, PointWith(0, 0), false);
		InventoryItem *item = new InventoryItem(entity->typeTag, count, entity->health, entity->icon, entity->name);
		inventory[slot] = item;
	}
	void swapInventorySpots(int index1, int index2) {
		InventoryItem *tmp = inventory[index1];
		inventory[index1] = inventory[index2];
		inventory[index2] = tmp;
	}
	void swapInventorySpots() {
		swapInventorySpots(currentQuickSelect, currentInventoryY*4+currentInventoryX+QUICKSELECT_COUNT);
	}
	void placeIntoStorageSpot() {
		int index = currentInventoryY*4+currentInventoryX + QUICKSELECT_COUNT;
		if(inventory[index] != NULL)index = firstAvailableNonQuickslot();
		if(index != -1)swapInventorySpots(currentQuickSelect, index);
	}
	void takeFromStorageSpot() {
		int index = currentQuickSelect;
		if(inventory[index] != NULL)index = firstAvailableQuickslot();
		if(index != -1)
			swapInventorySpots(currentInventoryY*4+currentInventoryX+QUICKSELECT_COUNT, index);
	}
	void sortStorage() {
		bool nullsInbetween = true;
		while(nullsInbetween) {
			int firstAvailableIndex = firstAvailableNonQuickslot();
			int atLeastOneOccupiedSlot = false;
			int furthestIndex = 0;
			for(int d=QUICKSELECT_COUNT-1;d<16+QUICKSELECT_COUNT;d++) {
				if(inventory[d] != NULL) {
					atLeastOneOccupiedSlot = true;
					if(d > furthestIndex)furthestIndex = d;
				}
			}
			if(furthestIndex > firstAvailableIndex)
				swapInventorySpots(firstAvailableIndex, furthestIndex);
			else
				nullsInbetween = false;
		}
		int workingIndex = QUICKSELECT_COUNT;
		bool sorted = false;
		while(!sorted) {
			bool swapHappened = false;
			if(inventory[workingIndex] == NULL || workingIndex == 16+QUICKSELECT_COUNT-1)
				sorted = true;
			for(int d=workingIndex;d<16+QUICKSELECT_COUNT;d++) {
				if(inventory[d] != NULL) {
					if(inventory[workingIndex]->inventoryItemName > inventory[d]->inventoryItemName) {
						swapInventorySpots(workingIndex, d);
						swapHappened = true;
					}
				}
			}
			if(!swapHappened)
				workingIndex++;
		}
	}
	void clearInventory() {for(int d=0;d<16+QUICKSELECT_COUNT;d++)inventory[d] = NULL;}

	InventoryItem *inventory[16+QUICKSELECT_COUNT];	// First in array will be for Quick Select
	int interactDirection, interactionType;
	Point interactLoc;
};
Player *player;

Entity *createItem(int tag, Point loc, bool dropped) {
	Entity *retVal;

	if(tag == TAG_PLAYER)retVal = new Player(loc);
	else if(tag == TAG_WALL)retVal = new Wall(loc);
	else if(tag == TAG_CACTUS)retVal = new Cactus(loc);
	else if(tag == TAG_LILYPAD)retVal = new Lilypad(loc);
	else if(tag == TAG_WATER)retVal = new Water(loc);
	else if(tag == TAG_CUP)retVal = new Cup(loc);
	else if(tag == TAG_DIRT)retVal = new Dirt(loc);
	else if(tag == TAG_FIRE)retVal = new Fire(loc);
	else if(tag == TAG_STEAM)retVal = new Steam(loc);
	else if(tag == TAG_SAND)retVal = new Sand(loc);
	else if(tag == TAG_CLAY)retVal = new Clay(loc);
	else if(tag == TAG_BEDROCK)retVal = new Bedrock(loc);
	else retVal = NULL;

	if(retVal && dropped)
		retVal->setDropped();

	return retVal;
}

Entity *spawnItem(int tag, Point loc, bool dropped) {
	Entity *retVal = createItem(tag, loc, dropped);
	if(retVal != NULL) {
		entities.push_back(retVal);
	}
	return retVal;
}

void placePlayerOnTop() {
	if(player != NULL) {
		int index = -1;
		for(int d=0;d<entities.size();d++) {
			if(entities[d] == player) {
				index = d;
				break;
			}
		}
		if(index != -1) {
			entities.erase(entities.begin() + index);
			entities.push_back(player);
		}
	}
}

void centerCameraOn(float x, float y) {
	cameraCenter.x = x;
	cameraCenter.y = y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glWidth = screenWidth/zoomLevel;
	glHeight = screenHeight/zoomLevel;
	float ratio = screenWidth * 1.0 / screenHeight;
	glOrtho(-glWidth/2.0f+cameraCenter.x, glWidth/2.0f+cameraCenter.x, -glHeight/2.0+cameraCenter.y, glHeight/2.0f+cameraCenter.y, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

class World {
public:
	World() {
		width = 100;
		height = 100;
		player = new Player(PointWith(0, 0));
		entities.push_back(player);
	}
	void generateWorld() {
		entities.clear();
		player = new Player(PointWith(5, 5));
		entities.push_back(player);
		for(int d=1;d<11;d++)entities.push_back(new Wall(PointWith(d, 1)));

		for(int d=0;d<width;d++)entities.push_back(new Bedrock(PointWith(d, 0)));
		for(int d=0;d<width;d++)entities.push_back(new Bedrock(PointWith(d, height-1)));
		for(int d=1;d<height-1;d++)entities.push_back(new Bedrock(PointWith(0, d)));
		for(int d=1;d<height-1;d++)entities.push_back(new Bedrock(PointWith(width-1, d)));
		placePlayerOnTop();
		centerCameraOn(player->gridLoc().x, player->gridLoc().y);
	}
	void save() {
		cout << "Saving World..." << endl;
		ofstream saveFile("world.sav", ios::trunc);
		if(saveFile.is_open()) {
			saveFile << TAG_STARTOFSAVEFILE << " ";
			bool itemsSaved = false;
			while(!itemsSaved) {
				saveFile << TAG_STARTOFITEMS << " ";
				for(int d=0;d<entities.size();d++) {
					saveFile << TAG_NEWITEM << " ";
					saveFile << entities[d]->typeTag << " ";
					saveFile << entities[d]->loc.x << " ";
					saveFile << entities[d]->loc.y << " ";
					vector<int>data = entities[d]->getItemData();
					if(data.size() > 0) {
						saveFile << TAG_STARTOFITEMDATA << " ";
						for(int dd=0;dd<data.size();dd++) {
							saveFile << data[dd] << " ";
						}
						saveFile << TAG_ENDOFITEMDATA << " ";
					}
				}
				saveFile << TAG_ENDOFITEMS << " ";
				itemsSaved = true;
			}
			bool inventorySaved = false;
			while(!inventorySaved) {
				saveFile << TAG_STARTOFINVENTORY << " ";
				for(int d=0;d<16+QUICKSELECT_COUNT;d++) {
					if(player->inventory[d] != NULL) {
						saveFile << player->inventory[d]->inventoryItemTag << " ";
						saveFile << player->inventory[d]->count << " ";
					}
					else {
						saveFile << TAG_NULL << " ";
						saveFile << 0 << " ";
					}
				}
				saveFile << TAG_ENDOFINVENTORY << " ";
				inventorySaved = true;
			}
			saveFile << TAG_ENDOFSAVEFILE;
		}
		saveFile.close();
		cout << "Succesfully Saved" << endl;
	}
	void load() {
		int value;
		cout << "Loading World..." << endl;
		entities.clear();
		ifstream loadFile("world.sav", ios::in|ios::binary);
		if(loadFile.is_open()) {
			loadFile >> value;
			if(value == TAG_STARTOFSAVEFILE) {
				bool itemsLoaded = false;
				Entity *latestEntity = NULL;
				while(!itemsLoaded) {
					loadFile >> value;
					if(value == TAG_STARTOFITEMS) {
						//
					}
					else if(value == TAG_NEWITEM) {
						int tag, x, y;
						loadFile >> tag;
						loadFile >> x;
						loadFile >> y;
						latestEntity = createItem(tag, PointWith(x, y), false);
						if(tag == TAG_PLAYER)player = (Player *)latestEntity;
						if(latestEntity != NULL)entities.push_back(latestEntity);
					}
					else if(value == TAG_ENDOFITEMS) {
						itemsLoaded = true;
					}
					else if(value == TAG_STARTOFITEMDATA) {
						bool itemDataLoaded = false;
						vector<int>data;
						while(!itemDataLoaded) {
							int tempValue;
							loadFile >> tempValue;
							if(tempValue != TAG_ENDOFITEMDATA) {
								data.push_back(tempValue);
							}
							else {
								latestEntity->initWithItemData(data);
								itemDataLoaded = true;
							}
						}
					}
					else {
						//
					}
				}
				bool inventoryLoaded = false;
				while(!inventoryLoaded) {
					int value;
					loadFile >> value;
					if(value == TAG_STARTOFINVENTORY) {
						for(int d=0;d<16+QUICKSELECT_COUNT;d++) {
							int tag, count;
							loadFile >> tag;
							loadFile >> count;
							if(tag != TAG_NULL)
								player->spliceIntoInventory(tag, count, d);
						}
					}
					inventoryLoaded = true;
				}
			}
		}
		else cout << "Unable to open file" << endl;

		loadFile.close();
		placePlayerOnTop();
		centerCameraOn(player->gridLoc().x, player->gridLoc().y);
		cout << "Succesfully Loaded" << endl;
	}
	int width, height;
}*world;

void setGameStatus(int mode) {
	if(mode == GAMESTATUS_NORMAL) {
		gameStatus = GAMESTATUS_NORMAL;
	}
	if(mode == GAMESTATUS_INVENTORY) {
		gameStatus = GAMESTATUS_INVENTORY;
	}
	if(mode == GAMESTATUS_MENU) {
		gameStatus = GAMESTATUS_MENU;
	}
}

int main(int argc, char **argv) {
	menuOptions.push_back("Return");
	menuOptions.push_back("Load");
	menuOptions.push_back("Save");
	menuOptions.push_back("Exit");

	seed();

	glutInit(&argc, argv);
	glutIdleFunc(&OnIdle);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Essplore!");
	glutKeyboardFunc(&keyboardDown);
	glutKeyboardUpFunc(&keyboardUp);
	glutSpecialFunc(&keyboardSpecial);
	glutDisplayFunc(&display);
	glInit();

	clock_t t1, t2;
	t1 = t2 = clock();
	while(t1 == t2)t2 = clock();
    // print resolution of clock()
	cout << (double)(t2 - t1) / CLOCKS_PER_SEC * 1000 << " ms.\n";

	world = new World();
	world->generateWorld();

	glutMainLoop();

	return EXIT_SUCCESS;
}

void manageCamera() {
	if(player->gridLoc().x+SpriteWidth >= glWidth/3+cameraCenter.x)centerCameraOn(cameraCenter.x+SpriteWidth, cameraCenter.y);
	else if(player->gridLoc().x <= -glWidth/3+cameraCenter.x)centerCameraOn(cameraCenter.x-SpriteWidth, cameraCenter.y);
	else if(player->gridLoc().y+SpriteHeight >= glHeight/3+cameraCenter.y)centerCameraOn(cameraCenter.x, cameraCenter.y+SpriteHeight);
	else if(player->gridLoc().y <= -glHeight/3+cameraCenter.y+HUDHeight)centerCameraOn(cameraCenter.x, cameraCenter.y-SpriteHeight);
}

void glInit() {
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
	centerCameraOn(0, 0);
}

#define CameraZoomInterval 0.005f
clock_t lastCameraZoomTick = clock();
void zoomCamera(float modifier) {
	clock_t now = clock();
	double delta = (double)(now-lastCameraZoomTick)/CLOCKS_PER_SEC;
	if(delta >= CameraZoomInterval) {
		zoomLevel *= modifier;
		centerCameraOn(cameraCenter.x, cameraCenter.y);
		lastCameraZoomTick = now;
	}
}

#define CameraPanInterval 0.005f
clock_t lastCameraPanTick = clock();
Point globalPan;
void panCamera(Point pan) {
	clock_t now = clock();
	double delta = (double)(now-lastCameraPanTick)/CLOCKS_PER_SEC;
	if(delta >= CameraPanInterval) {
		centerCameraOn(cameraCenter.x+pan.x, cameraCenter.y+pan.y);
		lastCameraPanTick = now;
	}
}

void purgeEntities() {
	vector<Entity *>toPurge;
	for(int d=0;d<entities.size();d++) {
		if(!entities[d]->isAlive)
			toPurge.push_back(entities[d]);
	}
	for(int d=0;d<toPurge.size();d++) {
		int index = indexForEntity(toPurge[d]);
		entities[index]->die();
		entities.erase(entities.begin() + index);
	}
}

void OnIdle() {
	for(int d=0;d<entities.size();d++)
		entities[d]->tick();
	if(keyboard['['])zoomCamera(0.99f);
	if(keyboard[']'])zoomCamera(1.01f);
	bool shouldPan = false;
	if(keyboard['o']){globalPan.y += 0.5f;shouldPan=true;}
	if(keyboard['l']){globalPan.y -= 0.5f;shouldPan=true;}
	if(keyboard['k']){globalPan.x -= 0.5f;shouldPan=true;}
	if(keyboard[';']){globalPan.x += 0.5f;shouldPan=true;}
	if(shouldPan) {
		panCamera(globalPan);
		globalPan = PointWith(0, 0);
	}
	manageCamera();
	purgeEntities();
}

void affectCurrentInventorySelection(int direction) {
	if(direction == UP)currentInventoryY--;
	else if(direction == DOWN)currentInventoryY++;
	else if(direction == RIGHT)currentInventoryX++;
	else if(direction == LEFT)currentInventoryX--;

	if(currentInventoryX == -1)currentInventoryX = 0;
	else if(currentInventoryX == 4)currentInventoryX = 3;
	else if(currentInventoryY == -1)currentInventoryY = 0;
	else if(currentInventoryY == 4)currentInventoryY = 3;
}

void affectQuickselect(int direction) {
	if(direction == RIGHT){currentQuickSelect--;if(currentQuickSelect==-1)currentQuickSelect=0;}
	if(direction == LEFT){currentQuickSelect++;if(currentQuickSelect==3)currentQuickSelect=2;}
}

void menuSelectionPressed() {
	if(menuOptions[currentMenuSelection] == "Return"){setGameStatus(GAMESTATUS_NORMAL);currentMenuSelection=0;}
	if(menuOptions[currentMenuSelection] == "Load"){world->load();setGameStatus(GAMESTATUS_NORMAL);currentMenuSelection=0;}
	if(menuOptions[currentMenuSelection] == "Save"){world->save();setGameStatus(GAMESTATUS_NORMAL);currentMenuSelection=0;}
	if(menuOptions[currentMenuSelection] == "Exit")exit(EXIT_SUCCESS);
}

void affectMenuSelection(int direction) {
	if(direction == UP)
		currentMenuSelection--;
	else if(direction == DOWN)
		currentMenuSelection++;

	if(currentMenuSelection < 0)
		currentMenuSelection = menuSelectionCount-1;
	if(currentMenuSelection > menuSelectionCount-1)
		currentMenuSelection = 0;
}

void keyboardDown(unsigned char key, int x, int y) {
	// 13 is Enter, 127 is Delete
	if(gameStatus == GAMESTATUS_NORMAL) {
		if(key == '\x1B')setGameStatus(GAMESTATUS_MENU);
		if(key == 'e')setGameStatus(GAMESTATUS_INVENTORY);
		if(key == 'a') {
			if(!keyboard['a']) {
				player->vel.x--;
				player->lastMoveTick = clock();
				player->move();
			}
		}
		if(key == 'd') {
			if(!keyboard['d']) {
				player->vel.x++;
				player->lastMoveTick = clock();
				player->move();
			}
		}
		if(key == ' ' || key == 'w') {
			if(!keyboard[' '] || !keyboard['w']) {
				player->jump();
				player->lastMoveTick = clock();
				player->move();
			}
		}
	}
	else if(gameStatus == GAMESTATUS_INVENTORY) {
		if(key == '\x1B')setGameStatus(GAMESTATUS_NORMAL);
		if(key == 'e')setGameStatus(GAMESTATUS_NORMAL);
		if(key == 'r')player->sortStorage();
		if(key == 'w')affectCurrentInventorySelection(UP);
		if(key == 's')affectCurrentInventorySelection(DOWN);
		if(key == 'a')affectCurrentInventorySelection(LEFT);
		if(key == 'd')affectCurrentInventorySelection(RIGHT);
	}
	else if(gameStatus == GAMESTATUS_MENU) {
		if(key == 'w')affectMenuSelection(UP);
		if(key == 's')affectMenuSelection(DOWN);
		if(key == '\x1B'){setGameStatus(GAMESTATUS_NORMAL);currentMenuSelection=0;}
		if(key == 13)menuSelectionPressed();
	}

	if(key == '1')currentQuickSelect = 0;
	if(key == '2')currentQuickSelect = 1;
	if(key == '3')currentQuickSelect = 2;
	if(key == 'z')affectQuickselect(RIGHT);
	if(key == 'x')affectQuickselect(LEFT);

	keyboard[key] = 1;
}

void keyboardUp(unsigned char key, int x, int y) {
	keyboard[key] = 0;
}

void keyboardSpecial(int key, int x, int y) {
	int mod = glutGetModifiers();

	if(gameStatus == GAMESTATUS_NORMAL) {
		player->interactionType = USE;
		if(mod == GLUT_ACTIVE_SHIFT)player->interactionType = PLACE;

		if(key == GLUT_KEY_UP)player->interactDirection = UP;
		if(key == GLUT_KEY_DOWN)player->interactDirection = DOWN;
		if(key == GLUT_KEY_LEFT)player->interactDirection = LEFT;
		if(key == GLUT_KEY_RIGHT)player->interactDirection = RIGHT;
	}
	else if(gameStatus == GAMESTATUS_INVENTORY) {
		if(key == GLUT_KEY_UP)player->placeIntoStorageSpot();
		if(key == GLUT_KEY_DOWN)player->takeFromStorageSpot();
		if(key == GLUT_KEY_LEFT)affectQuickselect(RIGHT);
		if(key == GLUT_KEY_RIGHT)affectQuickselect(LEFT);
	}
	else if(gameStatus == GAMESTATUS_MENU) {
		if(key == GLUT_KEY_UP)affectMenuSelection(UP);
		if(key == GLUT_KEY_DOWN)affectMenuSelection(DOWN);
	}
}

void renderHUD() {
	Point HUD_BL = PointWith(-glWidth/2+cameraCenter.x, -glHeight/2+cameraCenter.y);
	Point HUD_BR = PointWith(glWidth/2+cameraCenter.x, -glHeight/2+cameraCenter.y);
	Point HUD_TR = PointWith(glWidth/2+cameraCenter.x, -glHeight/2+glHeight/10+cameraCenter.y);
	Point HUD_TL = PointWith(-glWidth/2+cameraCenter.x, -glHeight/2+glHeight/10+cameraCenter.y);
	HUDWidth = HUD_BR.x-HUD_BL.x;
	HUDHeight = HUD_TR.y-HUD_BR.y;
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.1f, 0.1f);
		glVertex2f(HUD_BL.x, HUD_BL.y);
		glVertex2f(HUD_BR.x, HUD_BR.y);
		glVertex2f(HUD_TR.x, HUD_TR.y);
		glVertex2f(HUD_TL.x, HUD_TL.y);

		float corner = HUDHeight / 6.0f;
		float size = corner*12;
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(HUD_BL.x+corner, HUD_BL.y+corner);
		glVertex2f(HUD_BL.x+size, HUD_BL.y+corner);
		glVertex2f(HUD_BL.x+size, HUD_BL.y+corner*2);
		glVertex2f(HUD_BL.x+corner, HUD_BL.y+corner*2);

		float healthSize = (float)player->health/(float)player->maxHealth * size;
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(HUD_BL.x+corner, HUD_BL.y+corner);
		glVertex2f(HUD_BL.x+healthSize, HUD_BL.y+corner);
		glVertex2f(HUD_BL.x+healthSize, HUD_BL.y+corner*2);
		glVertex2f(HUD_BL.x+corner, HUD_BL.y+corner*2);
	glEnd();

	glLineWidth(2.0f);
	float squareSize = HUDHeight * 0.8f;
	float squareXOffset = HUDWidth * 0.5f;
	for(int d=0;d<3;d++) {
		Point BL = PointWith(HUD_BL.x+squareXOffset, HUD_BL.y+HUDHeight/2+squareSize/2);
		Point BR = PointWith(HUD_BL.x+squareXOffset+squareSize, HUD_BL.y+HUDHeight/2+squareSize/2);
		Point TR = PointWith(HUD_BL.x+squareXOffset+squareSize, HUD_BL.y+HUDHeight/2-squareSize/2);
		Point TL = PointWith(HUD_BL.x+squareXOffset, HUD_BL.y+HUDHeight/2-squareSize/2);
		if(player->inventory[d] != NULL) {
			float iconScalar = 0.75f;
			Point iconSize = PointWith(squareSize*iconScalar, squareSize*iconScalar);
			Point iconLoc = PointWith(TL.x+squareSize/2-iconSize.x/2, TL.y+squareSize/2-iconSize.y/2);
			player->inventory[d]->icon->render(iconLoc, iconSize);
		}
		glBegin(GL_LINE_STRIP);
			if(d == currentQuickSelect)glColor3f(0.5f, 0.5f, 0.5f);
			else glColor3f(0.2f, 0.2f, 0.2f);
			glVertex2f(BL.x, BL.y);
			glVertex2f(BR.x, BR.y);
			glVertex2f(TR.x, TR.y);
			glVertex2f(TL.x, TL.y);
			glVertex2f(BL.x, BL.y);
		glEnd();
		squareXOffset += squareSize * 1.5f;

		glLineWidth(1.0f);
		int count = 0;
		if(player->inventory[d] != 0)count = player->inventory[d]->count;
		stringstream out;
		out << count;
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(TR.x, TR.y);
		if(count > 0)glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)out.str().c_str());
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(HUD_BL.x+corner, HUD_BL.y+corner*2);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)"Health");
}

void renderInventory() {
	float inventoryBlockSize = min(glWidth/8, glHeight/8);
	float totalSize = inventoryBlockSize*4;
	float iconScalar = 0.75f;
	Point TL = PointWith(cameraCenter.x-totalSize/2, cameraCenter.y+totalSize/2);
	for(int y=0;y<4;y++) {
		for(int x=0;x<4;x++) {
			Point loc = PointWith(TL.x+x*inventoryBlockSize, TL.y-y*inventoryBlockSize);
			if(x == currentInventoryX && y == currentInventoryY)glColor3f(0.6f, 0.6f, 0.6f);
			else glColor3f(0.2f, 0.2f, 0.2f);
			glBegin(GL_QUADS);
				glVertex2f(loc.x, loc.y);
				glVertex2f(loc.x+inventoryBlockSize, loc.y);
				glVertex2f(loc.x+inventoryBlockSize, loc.y+inventoryBlockSize);
				glVertex2f(loc.x, loc.y+inventoryBlockSize);
			glEnd();
			glColor3f(0.75f, 0.75f, 0.25f);
			glBegin(GL_LINE_STRIP);
				glVertex2f(TL.x+x*inventoryBlockSize, TL.y-y*inventoryBlockSize);
				glVertex2f(TL.x+x*inventoryBlockSize+inventoryBlockSize, TL.y-y*inventoryBlockSize);
				glVertex2f(TL.x+x*inventoryBlockSize+inventoryBlockSize, TL.y-y*inventoryBlockSize+inventoryBlockSize);
				glVertex2f(TL.x+x*inventoryBlockSize, TL.y-y*inventoryBlockSize+inventoryBlockSize);
				glVertex2f(TL.x+x*inventoryBlockSize, TL.y-y*inventoryBlockSize);
			glEnd();
			int inventoryIndex = y*4+x + QUICKSELECT_COUNT;
			if(player->inventory[inventoryIndex] != NULL) {
				Point iconSize = PointWith(inventoryBlockSize*iconScalar, inventoryBlockSize*iconScalar);
				Point iconLoc = PointWith(loc.x+inventoryBlockSize/2-iconSize.x/2, loc.y+inventoryBlockSize/2-iconSize.y/2);
				player->inventory[inventoryIndex]->icon->render(iconLoc, iconSize);
			}
			glLineWidth(1.0f);
			int count = 0;
			if(player->inventory[inventoryIndex] != 0)count = player->inventory[inventoryIndex]->count;
			stringstream out;
			out << count;
			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos2f(loc.x+0.1f, loc.y+0.1f);
			if(count > 0)glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)out.str().c_str());
		}
	}
}

void renderMenu() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

	float boxWidth = screenWidth * 0.75f;

	float maxHeight = boxWidth * 0.25f;
	float recommendedHeight = screenHeight/(float)menuSelectionCount * 0.75f;
	float boxHeight = min(maxHeight, recommendedHeight);

	float bufferHeight = boxHeight * 0.25f;
	float totalHeight = boxHeight * menuSelectionCount + bufferHeight * (menuSelectionCount-1);
	Point TL = PointWith(screenWidth/2-boxWidth/2, screenHeight/2+totalHeight/2);
	Point loc = TL;
	for(int d=0;d<menuSelectionCount;d++) {
		if(d == currentMenuSelection)glColor3f(0.6f, 0.6f, 0.6f);
		else glColor3f(0.2f, 0.2f, 0.2f);
		glBegin(GL_QUADS);
			glVertex2f(loc.x, loc.y);
			glVertex2f(loc.x, loc.y-boxHeight);
			glVertex2f(loc.x+boxWidth, loc.y-boxHeight);
			glVertex2f(loc.x+boxWidth, loc.y);
		glEnd();

		glDisable(GL_LINE_SMOOTH);
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
			glVertex2f(loc.x, loc.y);
			glVertex2f(loc.x, loc.y-boxHeight);
			glVertex2f(loc.x+boxWidth, loc.y-boxHeight);
			glVertex2f(loc.x+boxWidth, loc.y);
			glVertex2f(loc.x, loc.y);
		glEnd();
		glEnable(GL_LINE_SMOOTH);

		glColor3f(1.0f, 1.0f, 1.0f);
		int length = glutBitmapLength(GLUT_BITMAP_9_BY_15, (const unsigned char*)menuOptions[d].c_str());
		float height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
		glRasterPos2f(loc.x+boxWidth/2-length/2, loc.y-boxHeight/2-height/2);
		glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)menuOptions[d].c_str());

		loc.y -= bufferHeight + boxHeight;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-glWidth/2.0f+cameraCenter.x, glWidth/2.0f+cameraCenter.x, -glHeight/2.0+cameraCenter.y, glHeight/2.0f+cameraCenter.y, -1, 1);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if(gameStatus == GAMESTATUS_NORMAL) {
		for(int d=0;d<entities.size();d++)entities[d]->render();
		renderHUD();
	}
	if(gameStatus == GAMESTATUS_INVENTORY) {
		renderInventory();
		renderHUD();
	}
	if(gameStatus == GAMESTATUS_MENU) {
		renderMenu();
	}

	glFlush();
	glutPostRedisplay();
}