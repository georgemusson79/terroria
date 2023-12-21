#include "Item_pickup.h"
#include "Item.h"
#include "Main.h"
#include "Player.h"
ItemPickup::ItemPickup(Item* item, Vector2 position) : Entity(position, item->width, item->height,1, item->texturePath) {
	this->invulnerable = true;
	this->dmgImmuneMaxTime = -1;
	this->item = item;
	this->maxYVelocity = 0.09;
	this->vAcceleration = 0.01;
	this->starttime = SDL_GetTicks();
	this->addToEntitiesList();
};

ItemPickup::ItemPickup(std::shared_ptr<Item> ptr, Vector2 position) : Entity(position, ptr->width, ptr->height, 1, ptr->texturePath) {
	this->item_sharedPtr = ptr;
	this->item = this->item_sharedPtr.get();
	this->maxYVelocity = 0.09;
	this->vAcceleration = 0.01;
	this->starttime = SDL_GetTicks();
	this->addToEntitiesList();
}

void ItemPickup::update() {
	Entity::update();
	this->aliveTime = SDL_GetTicks() - this->starttime;
	if (this->collidesWith(Main::player)) {
		if (this->aliveTime > 400 && !this->toBeDeleted() && Main::player->pickup(item)) {
			this->despawn();
		}
	}
	if (this->onGround) this->velocity.X = 0;
	if (this->aliveTime > this->despawnTime) this->despawn();
}

ItemPickup::~ItemPickup() {
	delete item;
}
