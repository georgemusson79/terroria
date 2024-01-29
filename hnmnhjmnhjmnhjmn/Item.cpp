//item.cpp
#include "Item.h"
#include "Projectiles.h"
#include "Main.h"
#include "Player.h"
#include "Tile.h"
#include <SDL_image.h>
#include "gui.h"
#include "Entities.h"
#include "Item_pickup.h"
#include "Cursor.h"

Item::Item() {
	this->null = true;
}

Entity* Item::getRangedProjectile(Vector2 position, Entity* owner,Vector2 initialVelocity) {
	return nullptr;
}

ItemSwing* Item::getItemProjectile(Vector2 position,Entity* owner=nullptr) {
	ItemSwing* obj = new ItemSwing(position, this->width, this->height, this->texturePath,0,handOffset,owner,this->useAnimation,this->defaultHeldRotation);
	obj->damage = this->damage;
	obj->kbDealt = this->kb;
	if (!this->melee) obj->deleteHitboxes();
	obj->canFlipWhenUsed = this->canFlipWhenUsed;
	return obj;
}

bool Item::shoot(Arm* src,double rotation, Entity* projectile){
	return true;
}

bool Item::use(Arm* arm) {
	return true;
}

bool Item::use(Arm* arm, Entity* proj) {
	return true;
}

bool Item::use(Player* player) {

	Vector2 cursorPos = Cursor::WorldPos();
	Tile* t = Main::getTileAt(cursorPos.X,cursorPos.Y);
	if (t != nullptr) {
		t->mine(this->pickaxePower, this->axePower, this->hammerPower, true);
	}
	return this->use(player->arm);
}

Item::~Item() {
	SDL_DestroyTexture(this->texture.get());
	SDL_DestroyTexture(this->hotbarTexture.get());
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