//items.cpp
#include "Items.h"
#include "Main.h"
#include "Cursor.h"
#include "Player.h"
#include "Tile.h"
#include "Tiles.h"

Item1::Item1() {
	this->maxStack = 9;
	this->useTime = 50;
	this->width = 1;
	this->height = 5;
	this->name = "cool sword bro";
	this->setTexture("assets\\Items\\sword.png");
	this->setHotbarTexture("assets\\Items\\swordh.png");
	this->id = 0;
	this->melee = true;
	this->itemType = SpecialisedType::GENERIC;
	this->handOffset = { 0.4,0.4 };
	this->rotationInInventory = 45;
}

bool Item1::use(Player* player) {
	//use(player);
	Vector2 pos = Cursor::WorldPos();
	if (pos.distance(player->position) < player->placeRng and Main::inWorldBounds(pos) and (Main::tiles[pos.X][pos.Y] == nullptr or Main::tiles[pos.X][pos.Y]->getID() == AIR)) {
		player->swingAnim(this);
		new Wood(pos.X, pos.Y);
		return true;
	}
}

WoodItem::WoodItem() {
	this->setTexture("assets\\tiles\\wood.png");
	this->width = 1;
	this->height = 1;
	this->itemType = SpecialisedType::GENERIC;
	this->id = 1;
	this->name = "Wood tile";
	this->maxStack = 999;
	this->useTime = 5;
	this->consumable = true;
}

bool WoodItem::use(Player* player) {
	//use(player);
	Vector2 pos = Cursor::WorldPos();
	if (pos.distance(player->position) < player->placeRng and Main::inWorldBounds(pos) and (Main::tiles[pos.X][pos.Y] == nullptr or Main::tiles[pos.X][pos.Y]->getID() == AIR)) {
		player->swingAnim(this);
		new Wood(pos.X, pos.Y);
		return true;
	}
	else return false;
}
