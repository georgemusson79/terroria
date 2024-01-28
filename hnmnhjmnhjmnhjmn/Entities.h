#pragma once
#include "Entity.h"
#include <vector>
class Arm;
class ItemSwing;
class Item;
class Entity1 : public Entity {
public:
	int x = 55;
	Entity1(Vector2 position);
	//void updateHitboxes() override;
	void update() override;
};

class Zombie : public Entity {
public:
	int weaponCooldown = 0;
	Arm* arm = nullptr;
	bool walking = false;
	void walk(Vector2 pos);
	virtual void kill() override;
	Zombie(Vector2 position);
	void update() override;
};

class Arm : public Entity {
protected:
	Entity* owner = nullptr;
	int animationFramesPassed = 0;
	std::shared_ptr<Item> heldItem = nullptr;
	ItemSwing* swingItem = nullptr;
public:
	float pokeAnimationRotation = 0;
	int timeToNextUse = 0;
	bool swingItemActiveOverride = false; //when true other code can decide when the swingitem is active or inactive
	bool usingItem = false;
	void setHeldItem(std::shared_ptr<Item> item);
	void deleteHeldItem();
	void useItemCancel();
	void setOwner(Entity* owner);
	ItemSwing* getSwingItem();
	std::shared_ptr<Item> getItem();
	bool useHeldItem(float angle=0);
	void swingAnim();
	void pokeAnim(float angle);
	void useItemAnimation(float startAngle = 0);
	void updatePos();
	void positionItem(); //move item so it is correctly placed in the hand
	float restingRotation = 0; //angle arm should be at when not doing anything
	Vector2 defaultShoulderPos; //where to rotate around, relative to owner center
	Vector2 defaultHandPos; //where to rotate held weapons around, relative to shoulder pos at 0 degrees
	float width; //width in tiles
	float height; //height in tiles
	Arm(Vector2 shoulderPos, Vector2 handPos, float width, float height, std::string pathToTexture, bool hasHitbox, Entity* owner,Vector2 startPos = { 500,500 });
	~Arm();
	Vector2 getHandPos(Vector2 itemOffset);
	Vector2 getShoulderPos();
	Entity* getOwner();
	virtual void update() override;
	bool renderEntity() override;
};

class ArmedEntity : public Entity {
protected:
	int armCount;
	int armRenderInFrontOfEntity = 0; //the in the arms array to render arms in front of the entity instead of behind
	std::vector<Arm*> arms = {};
public:
	void addArm(Arm* arm);
	ArmedEntity(Vector2 position, float width, float height, int health, std::string pathToTexture, int armCount,int armRenderInFrontOfEntity,bool gravity = true, bool defaultCollider = true, bool tileCollision = true);

};