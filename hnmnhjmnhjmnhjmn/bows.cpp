#include "Items.h"
#include "Player.h"
#include "Entities.h"
#include "Cursor.h"

Bow::Bow(std::string displayName,std::string pathToTexture, float width, float height, int damage, int critChance, float kb, float power) {
	this->setTexture(pathToTexture);
	this->width = width;
	this->height = height;
	this->damage = damage;
	this->critChance = critChance;
	this->kb = kb;
	this->handOffset = handOffset;
	this->ranged = true;
	this->ammoID = AmmoType::NONE;
	this->useAnimation = 1;
	this->maxStack = 1;
	this->isStackable = false;
	this->isUseable = true;
	this->useTime = 30;
	this->name = displayName;
	this->animationTime = 40;
	this->defaultHeldRotation = 270;
	this->power = power;
	this->handOffset.X = float(this->height) / 2;
}

bool Bow::shoot(Arm* src,Vector2 tgt) {
	if (src->getOwner() != nullptr) {
		Vector2 handpos = src->getHandPos({ 0,0 });
		Arrow* a=new Arrow(handpos, this->damage,src);
		a->setCenter(handpos.X, handpos.Y);
		a->velocity = Main::normaliseTwoPoints(tgt, a->center) * Vector2(this->power, this->power);
		
	}
	return true;
}
bool Bow::use(Player* player) {
	Vector2 itemPos;

	if (player->has(this->useAmmoID,&itemPos)) {
		player->removeFromInventory(itemPos.X, itemPos.Y, 1);
		this->shoot(player->arm, Cursor::WorldPos());
		return true;
	}
	return false;
}

WoodBow::WoodBow() : Bow("Wooden Bow","assets\\Items\\WoodBow.png",2,2,10,10,1,1) {
	this->id = WOOD_BOW;
}