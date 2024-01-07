#include "Items.h"

CopperPickaxe::CopperPickaxe() {
	this->canFlipWhenUsed = true;
	this->maxStack = 9;
	this->width = 2;
	this->height = 3;
	this->handOffset = {0,this->height/2};
	this->useTime = 40;
	this->pickaxePower = 5;
	this->animationTime = 40;
	this->useAnimation = 0;
	this->name = "Copper Pickaxe";
	this->setTexture("assets\\Items\\CopperPickaxe.png");
	this->id = 4;
	this->damage = 3;
	this->kb = 0.7;
	this->melee = true;
	this->itemType = SpecialisedType::GENERIC;
	this->rotationInInventory = 45;
}