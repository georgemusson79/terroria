#include "Projectiles.h"
#include "Item_pickup.h"
#include "Player.h"
#include "Colliders.h"
#include "Items.h"
Arrow::Arrow(Vector2 position, int damage, Vector2 initialvelocity, Entity* src, double width, double height,std::string pathToTexture, bool hostile, bool friendly) : Entity(position, width, height, -1, pathToTexture, true, false, true) {
	this->hostile = hostile;
	this->friendly = friendly;
	if (src != nullptr) {
		this->hostile = src->hostile;
		this->friendly = src->friendly;
	}
	this->displayName = "arrow";
	this->velocity = initialvelocity;
	this->kbDealt = 0.4;
	this->damage = damage;
	this->hitboxes.push_back(new RotatableHitbox(this->center, this->width, this->height));
	this->tileCollision = true;
}

void Arrow::update() {
	Entity::update();
	this->setRotation(Main::toDegrees(std::atan2(this->velocity.Y, this->velocity.X))+90);
}


bool Arrow::onHitNPC(Entity* NPC, Entity* src) {
	bool res = Entity::onHitNPC(NPC, src);
	if (res && !this->piercing) this->kill();
	return res;
	
}
bool Arrow::onHitPlayer(Player* player, Entity* src) {
	bool res = Entity::onHitPlayer(player, src);
	if (res && !this->piercing) this->kill();
	return res;
}

void Arrow::onTileCollision(std::vector<Tile*>& tiles) {
	if (Main::randomInt(0, 2) == 1) new ItemPickup(std::shared_ptr<Item>(new ArrowItem()), this->center);
	this->kill();
}

