#pragma once
//items.h
#include "Item.h"

class Player;

class Item1 : public Item {
public:
	Item1();
	bool use(Player* player) override;
};

class WoodItem : public Item {
public:
	WoodItem();
	bool use(Player* player) override;

};