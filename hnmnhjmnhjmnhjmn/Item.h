//item.h
#include "headers.h"
#include "Item_specialisedTypes.h"
#include "Vector2.h"
#pragma once
class Texture;
class Entity;
class ItemSwing;
class Player;


class Item {

public:
	bool null = false;
	float defaultHeldRotation = 0;
	SpecialisedType itemType = SpecialisedType::GENERIC;
	std::shared_ptr<SDL_Texture> texture = nullptr;
	std::string texturePath = "";
	std::string name = "";
	std::shared_ptr<SDL_Texture> itemTextCount = nullptr;
	int count = 1;
	int damage = 0;
	float kb = 0;
	int id = -1;
	int width = 0;
	int height = 0;
	int useTime =0;
	int useAnimation = -1;
	int useSound = -1;
	int maxStack = 999;
	int animationTime = -1;
	int ammoID = 0;
	bool isUseable = true;
	bool ranged = false;
	bool isAmmo = false;
	bool consumable = false;
	bool isStackable = true;
	bool melee = false;
	std::string hotbarTexturePath = "";
	Vector2 handOffset = { 0,0 }; //where the item should be held in relation to the player hand
	double rotationInInventory = 0;
	std::shared_ptr<SDL_Texture> hotbarTexture = nullptr;

	ItemSwing* getItemProjectile(Vector2 position,Entity* owner);
	virtual bool use(Player* player);
	virtual bool shoot();
	virtual void renderTexture(SDL_Rect* pos);
	virtual void setTexture(std::string path);
	virtual void setHotbarTexture(std::string path);
	void setCount(int count);
	int getCount();
	virtual ~Item();
	Item();

	
};