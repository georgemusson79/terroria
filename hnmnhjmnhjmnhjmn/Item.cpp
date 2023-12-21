//item.cpp
#include "Item.h"
#include "Projectiles.h"
#include "Main.h"
#include "Player.h"
#include <SDL_image.h>
#include "gui.h"
#include "Item_pickup.h"

Item::Item() {
	this->null = true;
}

ItemSwing* Item::getItemProjectile(Vector2 position) {
	ItemSwing* obj = new ItemSwing(position, this->width, this->height, this->texturePath,0,handOffset);
	if (!this->melee) obj->deleteHitboxes();
	return obj;
}

bool Item::shoot(){
	return true;
}



bool Item::use(Player* player) {
	player->swingAnim(this);
	return true;
}

Item::~Item() {
}



void Item::renderTexture(SDL_Rect* pos) {
	if (this->hotbarTexture == nullptr) SDL_RenderCopyEx(Main::renderer, this->texture.get(), NULL, pos, this->rotationInInventory, NULL, SDL_FLIP_NONE);
	else SDL_RenderCopy(Main::renderer, this->hotbarTexture.get(), NULL, pos);
}

void Item::setTexture(std::string path) {
	this->texturePath = path;
	if (this->texturePath != "") {
		SDL_Texture* t = IMG_LoadTexture(Main::renderer, this->texturePath.c_str());
		std::shared_ptr<SDL_Texture> s(t, [](SDL_Texture* ptr) {
			SDL_DestroyTexture(ptr);
			});
		this->texture = s;
	}
}

void Item::setHotbarTexture(std::string path) {
	this->hotbarTexturePath = path;
	if (this->hotbarTexturePath != "") {
		SDL_Texture* t = IMG_LoadTexture(Main::renderer, this->hotbarTexturePath.c_str());
		std::shared_ptr<SDL_Texture> s(t, [](SDL_Texture* ptr) {
			SDL_DestroyTexture(ptr);
			});
		this->hotbarTexture = s;
	}
}


void Item::setCount(int count) {
	this->count = count;
	this->itemTextCount = GUIinterface::getTextPointer(std::to_string(count));
}
int Item::getCount() {
	return this->count;
}