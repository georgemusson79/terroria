#pragma once
#include "Entity.h"
class Arm;
class ItemSwing;
class Entity1 : public Entity {
public:
	int x = 55;
	Entity1(Vector2 position);
	//void updateHitboxes() override;
	void update() override;
};

class Zombie : public Entity {
public:
	Arm* arm = nullptr;
	bool walking = false;
	void walk(Vector2 pos);
	virtual void kill() override;
	Zombie(Vector2 position);
	void update() override;
};

class Arm : public Entity {
protected:

public:
	void updatePos();
	void useItem();
	float restingRotation = 0; //angle arm should be at when not doing anything
	Entity* owner;
	Vector2 defaultShoulderPos; //where to rotate around, relative to owner pos
	Vector2 defaultHandPos; //where to rotate held weapons around, relative to shoulder pos at 0 degrees
	float width; //width in tiles
	float height; //height in tiles
	ItemSwing* heldItem = nullptr;
	Arm(Vector2 shoulderPos, Vector2 handPos, float width, float height, std::string pathToTexture, bool hasHitbox, Entity* owner);
	Vector2 getHandPos(Vector2 itemOffset);
	Vector2 getShoulderPos();
	virtual void update() override;
	bool renderEntity() override;
};

