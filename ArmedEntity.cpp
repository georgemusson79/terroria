#include "Entities.h"
#include "Main.h"

ArmedEntity::ArmedEntity(Vector2 position, float width, float height, int health, std::string pathToTexture, std::vector<Arm*> arms, int armRenderInFrontOfEntity, bool gravity, bool defaultCollider, bool tileCollision) : Entity(position, width, height, health, pathToTexture, gravity, defaultCollider, tileCollision) {
	this->armRenderInFrontOfEntity = armRenderInFrontOfEntity;
}


void ArmedEntity::setArms(int armRenderInFrontOfEntity,std::vector<Arm*> arms) {
	for (Arm* arm : this->arms) arm->despawn();
	this->arms = arms;
	this->armCount = arms.size();
	this->armRenderInFrontOfEntity = armRenderInFrontOfEntity;
	for (int i = 0; i < arms.size(); i++) {
		if (this->arms[i]->getOwner() == nullptr) this->arms[i]->setOwner(this);
		double priority = this->renderPriority - ((armRenderInFrontOfEntity - i) * 0.01);
		if (priority == this->renderPriority) priority -= 0.01;
		this->arms[i]->renderPriority = priority;
	}
}

int ArmedEntity::getArmCount() {
	return this->armCount;
}

ArmedEntity::~ArmedEntity() {
	for (auto arm : this->arms) arm->despawn();
}