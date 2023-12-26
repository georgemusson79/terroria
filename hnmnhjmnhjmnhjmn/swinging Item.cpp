//projectiles.cpp
#include "Projectiles.h"
#include "Colliders.h"
#include "Player.h"
#include <iostream>
#include "Main.h"
ItemSwing::ItemSwing(Vector2 position, float width, float height, std::string texturePath,float hitboxRotation,Vector2 handOffset,Entity* owner) : Entity(position,width,height,-1,texturePath,false,false,false) {
	this->deleteHitboxes();
	this->handOffset = handOffset;
	RotatableHitbox* hb = new RotatableHitbox(this->center, this->width, this->height);
	this->hitboxRotation = hitboxRotation;
	this->owner = owner;
	hb->setRotation(hitboxRotation);
	this->hitboxes.push_back(hb);
	this->invulnerable = true;
	this->displayName = "weapon";

}
void ItemSwing::update() {;

	if (this->owner == nullptr) this->despawn();
	this->invulnerable = true;
	Entity::update();
	if (this->owner!=nullptr) this->velocity = this->owner->velocity;
	for (Entity* e : Main::entities) {
		if (this != e and this->collidesWith(e)) {
			this->onHitNPC(e);
		}
	}
}



