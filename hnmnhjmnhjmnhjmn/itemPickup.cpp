#include "Item_pickup.h"
#include "Item.h"
#include "Main.h"
#include "Player.h"

ItemPickup::ItemPickup(std::shared_ptr<Item> ptr, Vector2 position) : Entity(position, ptr->width, ptr->height, -1, ptr->texturePath) {
	this->item_sharedPtr = ptr;
	this->item = this->item_sharedPtr.get();
	this->maxYVelocity = 0.09;
	this->vAcceleration = 0.01;
	this->starttime = SDL_GetTicks();
	this->displayName = "itempickup";
	this->setRotation(this->item_sharedPtr->rotationInInventory);
}

void ItemPickup::update() {
	if (this->toBeDeleted()) return;
	Entity::update();
	this->aliveTime = SDL_GetTicks() - this->starttime;
	if (this->collidesWith(Main::player)) {
		if (this->aliveTime > 400 && !this->toBeDeleted()) {
			this->pickedUp = (this->item_sharedPtr==nullptr) ? Main::player->pickup(item) : Main::player->pickup(this->item_sharedPtr);
			if (this->pickedUp) this->despawn();
		}
	}
	if (this->onGround) this->velocity.X = 0;
	if (this->aliveTime > this->despawnTime) this->despawn();
}

ItemPickup::~ItemPickup() {
}
