//projectiles.cpp
#include "Projectiles.h"
#include "Colliders.h"
#include "Player.h"
#include "Main.h"
ItemSwing::ItemSwing(Vector2 position, float width, float height, std::string texturePath,float hitboxRotation,Vector2 handOffset) : Entity(position,width,height,-1,texturePath,false,false,false) {
	this->deleteHitboxes();
	this->handOffset = handOffset;
	RotatableHitbox* hb = new RotatableHitbox(this->center, this->width, this->height);
	this->hitboxRotation = hitboxRotation;
	hb->setRotation(hitboxRotation);
	this->hitboxes.push_back(hb);
	this->invulnerable = true;

}


