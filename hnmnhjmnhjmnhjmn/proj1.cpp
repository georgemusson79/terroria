#include "Projectiles.h"
#include "Colliders.h"
ItemSwing::ItemSwing(Vector2 position, float width, float height, std::string texturePath,float hitboxRotation) : Entity(position,width,height,-1,texturePath,false,false,false) {
	this->deleteHitboxes();
	RotatableHitbox* hb = new RotatableHitbox(this->center, this->width, this->height);
	this->hitboxRotation = hitboxRotation;
	hb->setRotation(hitboxRotation);
	this->hitboxes.push_back(hb);
	this->invulnerable = true;

}

void ItemSwing::update() {
	this->rotation += 1;
	Entity::update();
	
	dynamic_cast<RotatableHitbox*>(this->hitboxes[0])->setRotation(rotation+hitboxRotation);
}