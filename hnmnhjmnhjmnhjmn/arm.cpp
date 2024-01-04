#include "Entities.h"
#include "Colliders.h"
#include "Projectiles.h"
#include "Player.h"
#include "Item.h"

Arm::Arm(Vector2 shoulderPos, Vector2 handPos, float width, float height, std::string pathToTexture, bool useDefaultHitbox, Entity* owner, Vector2 startPos) : Entity(shoulderPos, width, height, -1, pathToTexture, false, false, false) {
	this->renderPriority = 0;
	this->defaultShoulderPos = shoulderPos;
	this->position = startPos;
	this->defaultHandPos = handPos;
	this->owner = owner;
	if (this->owner != nullptr) {
		this->friendly = owner->friendly;
		this->hostile = owner->hostile;
	}
	this->displayName = owner->displayName+"'s arm";
	this->updatePos();
	if (useDefaultHitbox) {
		this->hitboxes.push_back(new RotatableHitbox(this->center, width, height));
	}
}

void Arm::positionItem() {
	if (this->swingItem == nullptr) return;
	double itemRotation = (this->rotation + 180);
	//this->swingItem->setRotation(0);
	Vector2 handPos = this->getHandPos(swingItem->handOffset);
	//Vector2 handPos = this->getHandPos({ this->swingItem->handOffset.X,Main::setSign(hDirection,this->swingItem->handOffset.Y) });
	Vector2 itemCenter = { handPos.X ,handPos.Y + (swingItem->height / 2) };
	Vector2 newPos = Main::rotatePt(itemCenter, handPos, this->rotation+Main::setSign(this->hDirection,swingItem->defaultRotation));
	this->swingItem->setCenter(newPos.X, newPos.Y);
	this->swingItem->setRotation(itemRotation + Main::setSign(this->hDirection,swingItem->defaultRotation));
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
	if (this->usingItem) this->useItemAnimation();
	if (this->timeToNextUse != 0) this->timeToNextUse--;
	if (this->usingItem || this->swingItemActiveOverride) this->positionItem();
	this->hDirection = this->owner->hDirection;
	//this->rotation = Main::setSign(this->hDirection,this->rotation);
	this->setRotationAround(this->rotation, this->getShoulderPos(), RotationType::ABSOLUTE);

}


void Arm::setOwner(Entity* owner) {
	this->owner = owner;
}
Entity* Arm::getOwner() {
	return this->owner;
}


void Arm::pokeAnim(float angle) {
	this->rotation = angle;
	animationFramesPassed++;
}

void Arm::swingAnim() {
	 if (abs(this->rotation) < 300 && this->heldItem->animationTime > 0) {
		float inc = ((double(270 - 90) / this->heldItem->animationTime) * this->hDirection);
		this->rotation = Main::setSign(this->hDirection,this->rotation) + inc;
		animationFramesPassed++;
	 }

}


void Arm::useItemCancel() {
	this->usingItem = false;
	if (!this->swingItemActiveOverride && this->swingItem != nullptr) {
		this->swingItem->active = false;
		this->rotation = 0;
	}
}

void Arm::useItemAnimation(float angle) {
 	if (!this->markForDeletion && !this->usingItem && heldItem != nullptr) {
		this->rotation = Main::setSign(this->hDirection,90);
		this->pokeAnimationRotation = angle;
		this->usingItem = true;
		this->swingItem->active = true;
		this->animationFramesPassed = 0;
	}

	if (heldItem == nullptr || this->animationFramesPassed >= heldItem->animationTime) {
		this->usingItem = false;
		if (!this->swingItemActiveOverride && this->swingItem != nullptr) {
			this->swingItem->active = false;
			this->rotation = 0;
		}
		return;
	}


	switch (swingItem->useAnimation) {
	case (0):
		this->swingAnim();
		break;
	case (1):
		this->pokeAnim(this->pokeAnimationRotation);
	}

}

bool Arm::useHeldItem(float angle) {
	if (this->heldItem != nullptr && this->timeToNextUse==0 && this->heldItem->isUseable) {
		this->rotation = angle;
		if (this->heldItem->use((Player*)this->owner)) {
			this->useItemAnimation(angle);
			this->timeToNextUse = this->heldItem->useTime;
			return true;
		}
		else this->rotation = 0;
	}
	return false;
}


Vector2 Arm::getHandPos(Vector2 itemOffset) { //the pivot point for items that are held
	Vector2 base = this->getShoulderPos();
	Vector2 end = base + Vector2(this->defaultHandPos.X, this->defaultHandPos.Y);
	end = Main::rotatePt(end - (Vector2(this->hDirection,this->hDirection)*itemOffset), base, this->rotation);
	return end;
}
Vector2 Arm::getShoulderPos() {
	Vector2 pos = this->owner->center + this->defaultShoulderPos;
	pos.X += (this->width / 2);
	return pos;
}

void Arm::setHeldItem(std::shared_ptr<Item> item) {
	if (this->heldItem!=nullptr && item!=nullptr) if (item->name == this->heldItem->name) return;
	this->deleteHeldItem();
	this->heldItem = item;
	this->swingItem = item->getItemProjectile(this->getHandPos(item->handOffset), this->owner);
	this->swingItem->active = false;
}


ItemSwing* Arm::getSwingItem() {
	return this->swingItem;
}
std::shared_ptr<Item> Arm::getItem() {
	return this->heldItem;
}

Arm::~Arm() {
	this->deleteHeldItem();
}

void Arm::deleteHeldItem() {
	if (this->swingItem != nullptr) this->swingItem->despawn();
	this->swingItem = nullptr;
}