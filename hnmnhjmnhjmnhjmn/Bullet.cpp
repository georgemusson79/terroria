#include "Projectiles.h"
#include "Main.h"
#include "Entity.h"
#include "Colliders.h"
Bullet::Bullet(Vector2 position, int damage, Vector2 initalVelocity,Entity* src, bool hostile, bool friendly) : Entity(position,0.1, 1, -1, "assets\\projectiles\\bullet.png", false, false, true) {
	this->hostile = hostile;
	this->friendly = friendly;
	if (src != nullptr) {
		this->hostile = src->hostile;
		this->friendly = src->friendly;
	}
	this->kbDealt = 0.4;
	this->damage = damage;
	this->hitboxes.push_back(new RotatableHitbox(this->center, this->width, this->height));
	this->tileCollision = true;
	this->displayName = "Musket Ball";
	this->gravity = false;
	this->velocity = initalVelocity;
	this->setRotation(Main::toDegrees(std::atan2(this->velocity.Y, this->velocity.X)) + 90);

}

void Bullet::update() {
	Entity::update();
	Main::updateLightMap(this->center, 4);
	if (this->velocity!=this->oldVelocity) this->setRotation(Main::toDegrees(std::atan2(this->velocity.Y, this->velocity.X)) + 90);
	if (this->getDistanceTravelled() > this->maxDistance) {
		this->kill();
	}
}

void Bullet::onTileCollision() {
	this->kill();
}