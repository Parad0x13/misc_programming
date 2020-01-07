#include "actor.h"

std::vector<Actor *>actors;

Actor::Actor() {
	icon = '?';
	color = LIGHTMAGENTA;
	loc = COORDWith(5, 6);
}

void Actor::logic() {
	//
}

void Actor::render() {
	screen->setBufferxy(loc.X-panLocation, loc.Y, icon, color);
}

Ant::Ant() {
	color = YELLOW;
	loc = COORDWith(5, 10);
	icon = '.';
	status = ANTSTATUS_Roaming;
}

void Ant::logic() {
	loc.X--;
}

AntPile::AntPile() {
	maxColonySize = 50;
	colonySize = 25;
	spawnRate = 0.5f;
	color = YELLOW;
	loc = COORDWith(15, SCREEN_HEIGHT/2);
}

void AntPile::render() {
	screen->setBufferxy(loc.X-3-panLocation, loc.Y, '_', color);
	screen->setBufferxy(loc.X-2-panLocation, loc.Y, '/', color);
	screen->setBufferxy(loc.X-1-panLocation, loc.Y-1, '|', color);
	screen->setBufferxy(loc.X+1-panLocation, loc.Y-1, '|', color);
	screen->setBufferxy(loc.X+2-panLocation, loc.Y, '\\', color);
	screen->setBufferxy(loc.X+3-panLocation, loc.Y, '_', color);
}
