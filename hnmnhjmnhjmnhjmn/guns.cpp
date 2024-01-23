#include "Items.h"
Gun::Gun(std::string displayName, std::string pathToTexture, float width, float height, int damage, int critChance, float kb, float power) : Bow(displayName, pathToTexture, width, height, damage, critChance, kb, power) {
	this->useAmmoID = AmmoType::BULLET;
}

Pistol::Pistol() : Gun("Wooden Bow", "assets\\Items\\WoodBow.png", 2, 2, 10, 10, 1, 1) {
	this->id = PISTOL;
}