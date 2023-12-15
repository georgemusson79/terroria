#include "Tiles.h"
#include "Item.h"
#include "Player.h"
#include "Main.h"
#include "gui.h"
#include <SDL.h>


HeartCrystal::HeartCrystal(uint16_t X, uint16_t Y, bool placeIntoWorld, bool isParent, MultiTileObject* parent) : MultiTileObject(HCRYSTAL, X, Y, 4, 4, isParent, parent) {
	this->isSolid = false;
	this->placeIntoWorld = placeIntoWorld;
	if (this->placeIntoWorld) this->create();
}


Chest::Chest(uint16_t X, uint16_t Y, bool placeIntoWorld, bool isParent, MultiTileObject* parent) : MultiTileObject(CHEST, X, Y, 2, 2, isParent, parent) {
	this->isSolid = false;
	this->placeIntoWorld = placeIntoWorld;
	this->texture = IMG_LoadTexture(Main::renderer,"assets\\tiles\\chest.png");
	if (this->isParent) this->inventory = std::vector<std::vector<std::shared_ptr<Item>>>(columns, std::vector<std::shared_ptr<Item>>(rows));
	if (this->placeIntoWorld) this->create();

}

void Chest::onRightClick(Player* player) {
	if (this->isParent) {
		this->open(player);
	}
	else this->parent->onRightClick(player);
}

SDL_Texture* Chest::getTexture() {
	return this->texture;
}

Chest::~Chest() {
	SDL_DestroyTexture(this->texture);
	this->close();
}

void Chest::renderInventory(SDL_Point startPos) {
	int scale = Main::WINDOW_WIDTH / 35;
	int gap = scale / 8;
	SDL_Point pos = startPos;
	for (int y = 0; y < this->rows; y++) {
		for (int x = 0; x < this->columns; x++) {
			GUIinterface::InventoryTile tile(InventoryFrames::CHEST_INTERFACE, pos, scale, this->inventory[x][y]);
			tile.renderTile();
			pos.x += scale + gap;
		}
		pos.y += scale + gap;
		pos.x = startPos.x;
	}
}


void Chest::open(Player* player) {
	this->player = player;
	this->player->openedChest = this;
	this->isOpen = true;
	this->player->inventoryOpen = true;
	//std::cout << player->inventoryOpen;

}
void Chest::close() {
	if (this->player != nullptr) {
		this->player->openedChest = nullptr;
		this->player->inventoryOpen = false;
		this->player = nullptr;
		this->isOpen = false;
	}
}
