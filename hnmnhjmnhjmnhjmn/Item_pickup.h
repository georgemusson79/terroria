#pragma once
#include "Entity.h"
class Item;
class ItemPickup : public Entity {
private:
	int starttime;
public:
	Item* item;
	int aliveTime = 0;
	int despawnTime = 300000;
	ItemPickup(Item* item,Vector2 position);
	void update() override;
	~ItemPickup();
	
};