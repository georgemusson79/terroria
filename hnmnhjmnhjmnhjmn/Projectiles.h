#pragma once
#include "Entity.h"
class ItemSwing : public Entity {
public:
	float hitboxRotation = 0;
	ItemSwing(Vector2 position,float width, float height, std::string texturePath,float hitboxRotation=0);
	void update() override;
	
};