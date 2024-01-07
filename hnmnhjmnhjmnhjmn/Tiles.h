//tiles.h
#include "Tile.h"
#include <SDL.h>

#pragma once
class Item;
enum tileIDEnum {
	DIRT, STONE, WOOD,HCRYSTAL,CHEST, AIR
};

class Dirt :public Tile {
public:
	Dirt(uint16_t X, uint16_t Y, bool placeIntoWorld=true);
	void update() override;
};

class Stone:public Tile {
public:
	Stone(uint16_t X, uint16_t Y, bool placeIntoWorld = true);
};

class Wood :public Tile {
public:
	Wood(uint16_t X, uint16_t Y, bool placeIntoWorld = true);
	void dropItem() override;
};

class Air :public Tile {
public:
	Air(uint16_t X, uint16_t Y, bool placeIntoWorld = true);
};

class Obamium : public Tile {
public:
	int someNum=55;
	Obamium(uint16_t X, uint16_t Y, bool placeIntoWorld = true);
};

class HeartCrystal : public MultiTileObject {
public:
	HeartCrystal(uint16_t X, uint16_t Y, bool placeIntoWorld = true,bool isParent=true, MultiTileObject* parent=nullptr);
	int x = 4;
	void update() override {
		std::cout << x;
	}
};

class Chest : public MultiTileObject {
private:
	SDL_Texture* texture;
	Player* player = nullptr;
public:
	SDL_Texture* getTexture() override;
	int rows = 6;
	int columns = 9;
	bool isOpen = false;
	Chest(uint16_t X, uint16_t Y, bool placeIntoWorld = true, bool isParent = true, MultiTileObject* parent = nullptr);
	std::vector<std::vector<std::shared_ptr<Item>>> inventory = {};
	void onRightClick(Player* player) override;
	~Chest();
	void renderInventory(SDL_Point startPos);
	void open(Player* player);
	void close();
};