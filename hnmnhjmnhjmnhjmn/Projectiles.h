//projectiles.h
#pragma once
#include "Entity.h"
class ItemSwing : public Entity {
public:
	Entity* owner = nullptr;
	float hitboxRotation = 0;
	Vector2 handOffset = { 0,0 };
	ItemSwing(Vector2 position,float width, float height, std::string texturePath,float hitboxRotation=0,Vector2 handOffset={0,0},Entity* owner=nullptr);
	virtual void handleCollisions();
	void update() override;
};