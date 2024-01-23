#include "Projectiles.h"
#include "Item_pickup.h"
#include "Player.h"
#include "Colliders.h"
#include "Items.h"
Arrow::Arrow(Vector2 position, int damage, Entity* src, bool hostile , bool friendly) : Entity(position, 1, 2, -1, "assets\\projectiles\\arrow.png", true, false, true) {
	this->hostile = hostile;
	this->friendly = friendly;
	if (src != nullptr) {
		this->hostile = src->hostile;
		this->friendly = src->friendly;
	}
	this->displayName = "arrow";
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

void Arrow::onTileCollision() {
	if (Main::randomInt(0, 2) == 1) new ItemPickup(std::shared_ptr<Item>(new ArrowItem()), this->center);
	this->kill();
}

