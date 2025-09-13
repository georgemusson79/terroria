//tile.cpp
#include "Tiles.h"
#include "Item_pickup.h"
#include "Items.h"
Wood::Wood(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(WOOD, X, Y,20) {
	this->minPickaxePower = 0;
	if (placeIntoWorld) Tile::create(*this);
}

void Wood::dropItem() {
	new ItemPickup(std::shared_ptr<Item>(new WoodItem), { this->X,this->Y });
}


Stone::Stone(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(STONE, X, Y,20) {
	this->minPickaxePower = 0;
	if (placeIntoWorld) Tile::create(*this);
}

Air::Air(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(AIR, X, Y,-1) {
	this->isSolid = false;
	this->isMineable = false;
	if (placeIntoWorld) Tile::create(*this);
}


Dirt::Dirt(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(DIRT, X, Y,15) {
	this->minPickaxePower = 0;
	if (placeIntoWorld) this->create();
}

void Dirt::update() {
}

Obamium::Obamium(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(DIRT, X, Y,500) {
	if (placeIntoWorld) Tile::create(*this);
}






