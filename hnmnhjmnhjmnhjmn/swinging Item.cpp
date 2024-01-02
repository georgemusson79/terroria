//projectiles.cpp
#include "Projectiles.h"
#include "Colliders.h"
#include "Player.h"
#include <iostream>
#include "Main.h"
ItemSwing::ItemSwing(Vector2 position, float width, float height, std::string texturePath,float hitboxRotation,Vector2 handOffset,Entity* owner,int useAnimation,float defaultRotation) : Entity(position,width,height,-1,texturePath,false,false,false) {
	this->deleteHitboxes();
	this->handOffset = handOffset;
	this->defaultRotation = defaultRotation;
	RotatableHitbox* hb = new RotatableHitbox(this->center, this->width, this->height);
	this->hitboxRotation = hitboxRotation;
	this->owner = owner;
	hb->setRotation(hitboxRotation);
	this->hitboxes.push_back(hb);
	this->invulnerable = true;
	this->useAnimation= useAnimation;
	this->displayName = "weapon";
	if (this->owner != nullptr) {
		this->hostile = owner->hostile;
		this->friendly = owner->friendly;
	}
}

void ItemSwing::handleCollisions() {
	for (Entity* e : Main::entities) {
		if (this != e && this->owner != e && this->collidesWith(e)) {
			if (this->owner==nullptr || !this->owner->checkOnSameTeam(e)) this->onHitNPC(e,this->owner);
		}
	}
	if (this->owner != Main::player && Main::player!=nullptr) {
		if (this->collidesWith(Main::player)) {
			if (this->owner!=nullptr || !this->owner->checkOnSameTeam(Main::player)) this->onHitPlayer(Main::player, this->owner);
		}
	}
}

void ItemSwing::update() {
	this->hDirection = this->owner->hDirection;
	if (this->owner == nullptr) this->despawn();
	this->invulnerable = true;
	Entity::update();
	this->handleCollisions();
	if (this->owner!=nullptr) this->velocity = this->owner->velocity;

}



