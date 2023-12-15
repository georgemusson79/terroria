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
	Main::entities.push_back(this);
	
};

void ItemPickup::update() {
	Entity::update();
	this->aliveTime = SDL_GetTicks() - this->starttime;
	if (this->collidesWith(Main::player)) {
		if (this->aliveTime > 400 && !this->markForDeletion && Main::player->pickup(item)) {
			this->markForDeletion = true;
		}
	}
	if (this->onGround) this->velocity.X = 0;
	if (this->aliveTime > this->despawnTime) this->markForDeletion = true;
}

ItemPickup::~ItemPickup() {
	delete item;
}
