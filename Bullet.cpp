#include "Projectiles.h"
#include "Main.h"
#include "Entity.h"
#include "Colliders.h"
Bullet::Bullet(Vector2 position, int damage, Vector2 initalVelocity, Entity* src, bool hostile, bool friendly) : Arrow(position, damage, initalVelocity, src, 0.3, 1, "assets\\projectiles\\bullet.png") {
	this->displayName = "Musket Ball";
	this->gravity = false;
	this->setRotation(Main::toDegrees(std::atan2(this->velocity.Y, this->velocity.X)) + 90);

}

void Bullet::update() {
	if (this->velocity != this->oldVelocity) this->setRotation(Main::toDegrees(std::atan2(this->velocity.Y, this->velocity.X)) + 90);
	Entity::update();
	Main::updateLightMap(this->center, 2);
	if (this->getDistanceTravelled() > this->maxDistance) {
		this->kill();
	}
}

void Bullet::onTileCollision(std::vector<Tile*>& tiles) {
	this->kill();
}