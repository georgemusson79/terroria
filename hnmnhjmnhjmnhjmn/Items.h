#pragma once
//items.h
#include "Item.h"

class Player;

class TestSword : public Item {
public:
	TestSword();
	bool use(Player* player) override;
};

class WoodItem : public Item {
public:
	WoodItem();
	bool use(Player* player) override;

};


class ArrowItem : public Item {
public:
	ArrowItem();
};

class Bow : public Item {
public:
	float power;
	AmmoType useAmmoID = AmmoType::ARROW;
	Bow(std::string displayName,std::string pathToTexture, float width, float height, int damage, int critChance, float kb, float power);
	virtual bool shoot(Arm* src,Vector2 tgt) override;
	virtual bool use(Player* player) override;
};

class WoodBow : public Bow {
public:
	WoodBow();
};

class Gun : public Bow {
public:
	Gun(std::string displayName, std::string pathToTexture, float width, float height, int damage, int critChance, float kb, float power);
};

class Pistol : public Gun {
public:
	Pistol();
};


class CopperPickaxe : public Item {
public:
	CopperPickaxe();
};
