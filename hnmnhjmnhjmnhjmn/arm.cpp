#include "Entities.h"
#include "Colliders.h"
#include "Projectiles.h"
#include "Player.h"
#include "Item.h"

Arm::Arm(Vector2 shoulderPos, Vector2 handPos, float width, float height, std::string pathToTexture, bool useDefaultHitbox, Entity* owner) : Entity(shoulderPos, width, height, -1, pathToTexture, false, false, false) {
	this->defaultShoulderPos = shoulderPos;
	this->defaultHandPos = handPos;
	this->owner = owner;
	this->updatePos();
	if (useDefaultHitbox) {
		this->hitboxes.push_back(new RotatableHitbox(this->center, width, height));
	}
}

void Arm::useItem() {
	if (this->swingItem == nullptr) return;
	double itemRotation = (this->rotation + 180);
	this->swingItem->setRotation(0);
	Vector2 handPos = this->getHandPos(this->swingItem->handOffset);
	Vector2 itemCenter = { handPos.X + (this->swingItem->width / 2),handPos.Y + (swingItem->height / 2) };
	Vector2 newPos = Main::rotatePt(itemCenter, handPos, this->rotation);
	this->swingItem->setCenter(newPos.X, newPos.Y);
	this->swingItem->active = true;
	this->swingItem->setRotation(itemRotation);
}


bool Arm::renderEntity() {
	Entity::renderEntity();
	Main::drawSquare(this->getHandPos({ 0,0 }), { 255,0,0 }, 3);
	return true;
}

void Arm::updatePos() {
	this->setPos(this->getShoulderPos().X - (this->width / 2), this->getShoulderPos().Y);
}


void Arm::update() {
	this->updatePos();
	if (this->usingItem) this->useItem();
	this->hDirection = this->owner->hDirection;
	this->rotation = Main::setSign(this->hDirection, this->rotation);
	this->setRotationAround(Main::setSign(this->hDirection,this->rotation), this->getShoulderPos(), RotationType::ABSOLUTE);

}

void Arm::swingAnim() {
	if (!this->usingItem) {
		this->rotation = 0;
		this->usingItem = true;
	}

	if (this->heldItem!=nullptr && this->usingItem && abs(this->rotation) < 300 && this->heldItem->useTime != 0) {
		//bruh this code sucks
		this->rotation += (double(270 - 90) / this->heldItem->useTime) * this->hDirection;
	}


	else {
		this->usingItem = false;
		this->swingItem->active = false;
		this->rotation = 0;
	}

}

Vector2 Arm::getHandPos(Vector2 itemOffset) { //the pivot point for items that are held
	Vector2 base = this->getShoulderPos();
	Vector2 end = base + Vector2(this->hDirection * this->defaultHandPos.X, this->defaultHandPos.Y);
	end = Main::rotatePt(end - itemOffset, base, this->rotation);
	return end;
}
Vector2 Arm::getShoulderPos() {
	Vector2 pos = this->owner->center + this->defaultShoulderPos;
	pos.X + (this->width / 2);
	return pos;
}

void Arm::setHeldItem(Item* item) {
	delete this->heldItem;
	if (this->swingItem!=nullptr) this->swingItem->despawn();
	this->heldItem = item;
	this->swingItem = item->getItemProjectile(this->getHandPos(item->handOffset), this->owner);
}

ItemSwing* Arm::getSwingItem() {
	return this->swingItem;
}
Item* Arm::getItem() {
	return this->heldItem;
}

Arm::~Arm() {
	delete this->heldItem;
	this->swingItem->despawn();
}