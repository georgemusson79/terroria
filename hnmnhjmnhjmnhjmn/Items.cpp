//items.cpp
#include "Items.h"
#include "Main.h"
#include "Cursor.h"
#include "Player.h"
#include "Tile.h"
#include "Tiles.h"

TestSword::TestSword() {
	this->maxStack = 9;
	this->width = 1;
	this->height = 5;
	this->useTime = 30;
	this->name = "cool sword bro";
	this->setTexture("assets\\Items\\sword.png");
	this->setHotbarTexture("assets\\Items\\swordh.png");
	this->id = 0;
	this->damage = 20;
	this->kb = 0.7;
	this->melee = true;
	this->itemType = SpecialisedType::GENERIC;
	this->handOffset = { 0.4,0.4 };
	this->rotationInInventory = 45;
}

bool TestSword::use(Player* player) {
	player->swingAnim(this);
	return true;
}

WoodItem::WoodItem() {
	this->setTexture("assets\\tiles\\wood.png");
	this->width = 1;
	this->height = 1;
	this->itemType = SpecialisedType::GENERIC;
	this->id = 1;
	this->name = "Wood tile";
	this->maxStack = 999;
	this->useTime = 15;
	this->consumable = true;
}

bool WoodItem::use(Player* player) {
	//use(player);
	Vector2 pos = Cursor::WorldPos();
	if (pos.distance(player->position) < player->placeRng && !Main::checkForTile(pos)) {
		//check for adjacent tiles to place on
		Dirt d=Dirt(pos.X, pos.Y,false);
		if (Main::player->collidesWith(&d)) {
			return false;
		}
		for (Entity* e : Main::entities) {
			if (e->collidesWith(&d)) {
				return false;
			}
		}
		std::vector<Vector2> surroundingTiles = { {pos.X + 1,pos.Y},{pos.X,pos.Y + 1},{pos.X - 1,pos.Y},{pos.X,pos.Y - 1} };
		for (Vector2& p : surroundingTiles) {
			if (Main::checkForTile(p)) {
				player->swingAnim(this);
				//new Chest(pos.X, pos.Y);
				Wood(pos.X, pos.Y);
				return true;
			}
		}
	}
	return false;
}
