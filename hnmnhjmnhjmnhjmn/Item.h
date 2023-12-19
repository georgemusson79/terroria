//item.h
#include "headers.h"
#include "Item_specialisedTypes.h"
#include "Vector2.h"
#pragma once
class Texture;
class Entity;
class ItemSwing;


class Item {

public:
	SpecialisedType itemType = SpecialisedType::GENERIC;
	std::shared_ptr<SDL_Texture> texture = nullptr;
	std::string texturePath = "";
	std::string name = "";
	std::shared_ptr<SDL_Texture> itemTextCount = nullptr;
	int count = 1;
	int id = -1;
	int width = 0;
	int height = 0;
	int useTime =15;
	int useAnimation = -1;
	int useSound = -1;
	int maxStack = 999;
	int animationTime = -1;
	int ammoID = 0;
	bool ranged = false;
	bool isAmmo = false;
	bool consumable = false;
	bool melee = false;
	Vector2 handOffset = { 0,0 }; //where the item should be held in relation to the player hand
	double rotationInInventory = 0;

	ItemSwing* getItemProjectile(Vector2 position);
	virtual bool use();
	virtual bool shoot();
	virtual void renderTexture(SDL_Rect* pos);
	virtual void setTexture(std::string path);
	void setCount(int count);
	inline int getCount();
	virtual ~Item();
	Item();

	
};