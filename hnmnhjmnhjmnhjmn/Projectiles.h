//projectiles.h
#pragma once
#include "Entity.h"
class ItemSwing : public Entity {
public:
	float defaultRotation = 0; //rotation when held in arm
	Entity* owner = nullptr;
	int useAnimation;
	float hitboxRotation = 0;
	Vector2 handOffset = { 0,0 };
	ItemSwing(Vector2 position,float width, float height, std::string texturePath,float hitboxRotation=0,Vector2 handOffset={0,0},Entity* owner=nullptr, int useAnimation=0, float defaultRotation=0);
	virtual void handleCollisions();
	void update() override;
};