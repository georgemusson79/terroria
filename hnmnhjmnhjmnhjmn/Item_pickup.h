#pragma once
#include "Entity.h"
class Item;
class ItemPickup : public Entity {
private:
	bool pickedUp = false;
	int starttime;
	std::shared_ptr<Item> item_sharedPtr = nullptr;
public:
	Item* item;
	int aliveTime = 0;
	int despawnTime = 300000;
	ItemPickup(Item* item,Vector2 position);

	ItemPickup(std::shared_ptr<Item> ptr, Vector2 position);
	void update() override;
	//~ItemPickup();
	
};