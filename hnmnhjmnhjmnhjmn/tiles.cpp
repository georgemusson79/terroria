//tile.cpp
#include "Tiles.h"
Wood::Wood(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(WOOD, X, Y) {
	this->pickaxePower = 10;
	if (placeIntoWorld) Tile::create(*this);
}


Stone::Stone(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(STONE, X, Y) {
	this->pickaxePower = 20;
	if (placeIntoWorld) Tile::create(*this);
}

Air::Air(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(AIR, X, Y) {
	this->isSolid = false;
	this->isMineable = false;
	if (placeIntoWorld) Tile::create(*this);
}


Dirt::Dirt(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(DIRT, X, Y) {
	this->pickaxePower = 10;
	if (placeIntoWorld) this->create();
}

void Dirt::update() {
}

Obamium::Obamium(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(DIRT, X, Y) {
	if (placeIntoWorld) Tile::create(*this);
}






