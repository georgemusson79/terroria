#pragma once
#include "Entity.h"
class Entity1 : public Entity {
public:
	int x = 55;
	Entity1(Vector2 position);
	//void updateHitboxes() override;
	void update() override;
};

class Zombie : public Entity {
public:

	bool walking = false;
	void walk(Vector2 pos);
	virtual void kill() override;
	Zombie(Vector2 position);
	void update() override;
};

