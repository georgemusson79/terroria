//items.cpp
#include "Items.h"
#include "Main.h"
#include "Cursor.h"
#include "Player.h"
#include "Tile.h"
#include "Entities.h"
#include "Tiles.h"

TestSword::TestSword() {
	this->canFlipWhenUsed = true;
	this->maxStack = 9;
	this->width = 1;
	this->height = 5;
	this->useTime =40;
	this->animationTime = 40;
	this->useAnimation = 0;
	this->name = "cool sword bro";
	this->setTexture("assets\\Items\\sword.png");
	this->setHotbarTexture("assets\\Items\\swordh.png");
	this->id = TEST_SWORD;
	this->damage = 20;
	this->kb = 0.7;
	this->melee = true;
	this->itemType = SpecialisedType::GENERIC;
	this->rotationInInventory = 45;
}

bool TestSword::use(Player* player) {
	return true;
}

WoodItem::WoodItem() {
	this->setTexture("assets\\tiles\\wood.png");
	this->width = 1;
	this->height = 1;
	this->itemType = SpecialisedType::GENERIC;
	this->id = WOOD_ITEM;
	this->name = "Wood tile";
	this->maxStack = 999;
	this->useTime = 10;
	this->useAnimation = 0;
	this->animationTime = 10;
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
				//new Chest(pos.X, pos.Y);
				Wood(pos.X, pos.Y);
				return true;
			}
		}
	}
	return false;
}

ArrowItem::ArrowItem() {
	this->damage = 10;
	this->ammoID = AmmoType::ARROW;
	this->id = ARROW_ITEM;
	this->isAmmo = true;
	this->name = "Wood Arrow";
	this->setTexture("assets\\projectiles\\arrow.png");
	this->height = 2;
	this->rotationInInventory = 180;
	this->width = 1;
	this->consumable = true;
}

Entity* ArrowItem::getRangedProjectile(Vector2 position, Entity* owner, Vector2 initialVelocity) {
	return new Arrow(position,10,initialVelocity, owner);
}

MusketBallItem::MusketBallItem() {
	this->damage = 7;
	this->ammoID = AmmoType::BULLET;
	this->id = MUSKET_BALL;
	this->isAmmo = true;
	this->name = "Musket Ball";
	this->setTexture("assets\\Items\\MusketBall.png");
	this->height = 1;
	this->width = 1;
	this->consumable = true;
}

Entity* MusketBallItem::getRangedProjectile(Vector2 position, Entity* owner,Vector2 initialvelocity) {
	return new Bullet(position, 7, initialvelocity, owner);
}
