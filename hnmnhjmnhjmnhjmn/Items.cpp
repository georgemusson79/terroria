//items.cpp
#include "Items.h"

Item1::Item1() {
	this->maxStack = 9;
	this->useTime = 50;
	this->width = 5;
	this->height = 5;
	this->name = "cool sword bro";
	this->setTexture("assets\\Items\\sword.png");
	this->id = 0;
	this->melee = true;
	this->itemType = SpecialisedType::GENERIC;
}