#include "Entities.h"
#include "Vector2.h"
#include "Main.h"
#include "Player.h"
#include "Colliders.h"
#include "Item_pickup.h"
Zombie::Zombie(Vector2 position) : Entity(position, 2, 3, 200, "assets\\enemy\\zombie1.png") {
	this->dmgImmuneMaxTime = 20;
	this->displayName = "zombie";
	this->maxXVelocity = 0.2;
	this->defense = 3;
	this->damage = 60;
	this->kbResist = 3;
	this->kbDealt = 0.7;
	this->frameCount = 1;
	this->friendly = false;
	this->hostile = true;
	dynamic_cast<SquareHitbox*>(this->hitboxes[0])->h -= 0.1;
	this->frameCount = 3;
	//this->arm = new Arm({0,0}, {0, 1.9}, 0.5, 2, "assets\\player\\arm2.png", true, this);
	this->setTexture("assets\\enemy\\zombie1.png");
	//this->arm->setHeldItem(std::shared_ptr<Item>(new TestSword()));
}

void Zombie::kill() {
	Entity::kill();
	//this->arm->despawn();
	new ItemPickup(std::shared_ptr<Item>(new WoodItem), this->position);
}

void Zombie::walk(Vector2 pos) {

	if (this->position.distance(pos) > 1) {
		if (this->onGround) {
			this->walking = true;
			this->hAcceleration = (this->position.X < pos.X) ? 0.1 : -0.1;
			this->velocity.X = this->hAcceleration;
			this->switchFrames((this->animationFrame == 0) ? 1 : 0);
		}

	}
	else {
		this->walking = false;
		this->switchFrames(1);
	}
}

void Zombie::update() {
	if (Main::player != nullptr) Entity::walk(this, Main::player->position, 0.1, 0, 1, 0);
	Entity::update();
	//if (Main::player != nullptr) this->walk(Main::player->position);
	
	if (this->onGround && !this->walking) this->velocity.X = 0;
	if (this->collidesWith(Main::player)) Main::player->hurt(this->damage, this->kbDealt, this);
}

