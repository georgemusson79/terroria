#include "Entities.h"
#include "Colliders.h"
#include "Projectiles.h"
#include "Player.h"

Arm::Arm(Vector2 shoulderPos, Vector2 handPos, float width, float height, std::string pathToTexture, bool useDefaultHitbox, Entity* owner) : Entity(shoulderPos, width, height, -1, pathToTexture, false, false, false) {
	this->defaultShoulderPos = shoulderPos;
	this->defaultHandPos = handPos;
	this->owner = owner;
	if (useDefaultHitbox) {
		this->hitboxes.push_back(new RotatableHitbox(this->center, width, height));
		this->hitboxes[0]->setCenter(this->center);

	}
	Vector2 pos = this->owner->position + this->defaultShoulderPos;
	this->setPos(pos.X, pos.Y);
}

void Arm::useItem() {
	if (this->heldItem == nullptr) return;
	double itemRotation = (this->rotation + 180);
	this->heldItem->setRotation(0);
	Vector2 handPos = this->getHandPos(this->heldItem->handOffset);
	Vector2 itemCenter = { handPos.X + (heldItem->width / 2),handPos.Y + (heldItem->height / 2) };
	Vector2 newPos = Main::rotatePt(itemCenter, handPos, this->rotation);
	this->heldItem->setCenter(newPos.X, newPos.Y);

	this->heldItem->setRotation(itemRotation);
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
	this->useItem();
	this->hDirection = this->owner->hDirection;
	this->rotation = Main::setSign(this->hDirection, this->rotation);
	this->setRotationAround(this->rotation, this->getShoulderPos(), RotationType::ABSOLUTE);

}

Vector2 Arm::getHandPos(Vector2 itemOffset) { //the pivot point for items that are held
	Vector2 base = this->getShoulderPos();
	Vector2 end = base + Vector2(this->hDirection * this->defaultHandPos.X, this->defaultHandPos.Y);
	end = Main::rotatePt(end - itemOffset, base, this->rotation);
	return end;
}
Vector2 Arm::getShoulderPos() {
	Vector2 pos = this->owner->position + this->defaultShoulderPos;
	pos.X + (this->width / 2);
	return pos;
}