#include "Entities.h"

Arm::Arm(Vector2 shoulderPos, Vector2 handPos, float width, float height, std::string pathToTexture, bool useDefaultHitbox, Entity* owner) : Entity(shoulderPos, width, height, -1, pathToTexture, false, useDefaultHitbox, false) {
	this->defaultShoulderPos = shoulderPos;
	this->defaultHandPos =handPos;
	this->owner = owner;

}

void Arm::update() {

}
