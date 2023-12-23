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