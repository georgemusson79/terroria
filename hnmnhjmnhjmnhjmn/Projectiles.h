//projectiles.h
#pragma once
#include "Entity.h"
class ItemSwing : public Entity {
public:
	bool canFlipWhenUsed = false; //player can turn around when using the item
	float defaultRotation = 0; //rotation when held in arm
	Entity* owner = nullptr;
	int useAnimation;
	float hitboxRotation = 0;
	Vector2 handOffset = { 0,0 };
	ItemSwing(Vector2 position,float width, float height, std::string texturePath,float hitboxRotation=0,Vector2 handOffset={0,0},Entity* owner=nullptr, int useAnimation=0, float defaultRotation=0);
	virtual void handleCollisions();
	virtual Vector2 moveEntity(Vector2 velocity) override { return { 0,0 }; };
	void update() override;
};

class Arrow : public Entity {
public:
	bool piercing = false;
	Entity* owner = nullptr;
	Arrow(Vector2 position,int damage,Vector2 initialVelocity,Entity* src=nullptr, double width=1,double height=2, std::string pathToTexture = "assets\\projectiles\\arrow.png", bool hostile = false, bool friendly = false);
	virtual void update() override;
	virtual bool onHitNPC(Entity* NPC, Entity* src = nullptr) override;
	virtual void onTileCollision(std::vector<Tile*>& tiles) override;
	virtual bool onHitPlayer(Player* player, Entity* src = nullptr) override;
};

class Bullet : public Arrow {
public:
	int maxDistance = 50;
	Bullet(Vector2 position, int damage, Vector2 initialVelocity, Entity* src = nullptr, bool hostile = false, bool friendly = false);
	void update() override;
	void onTileCollision(std::vector<Tile*>& tiles) override;
};