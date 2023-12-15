#include "Entities.h"
#include "Colliders.h"
#include "Player.h"
#include "Main.h"

Entity1::Entity1(Vector2 position) : Entity(position, 10, 10,100, "assets\\Items\\sword.png") {
	this->dmgImmuneMaxTime = 100;
	this->deleteHitboxes();
	this->hitboxes.push_back(new RotatableHitbox(this->center, this->width, this->height));
	this->gravity = false;
	dynamic_cast<RotatableHitbox*>(this->hitboxes[0])->setRotation(45);
}


void Entity1::update() {
	Entity::update();
	this->rotation += 1;
	dynamic_cast<RotatableHitbox*>(this->hitboxes[0])->setRotation(rotation);
}