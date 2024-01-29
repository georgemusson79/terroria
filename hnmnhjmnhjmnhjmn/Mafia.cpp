#include "Entities.h"
#include "Main.h"
#include "Player.h"
#include "Items.h"

MafiaMan::MafiaMan(Vector2 position) : ArmedEntity(position, 2, 3.5, 100, "assets\\enemy\\mafia.png",{},0) {
	this->friendly = false;
	this->hostile = true;
	Vector2 armDims = { this->width / 4,this->height /3  };
	Arm* a = new Arm({ 0,-armDims.Y/5 }, { 0,armDims.Y }, armDims.X, armDims.Y, "assets\\enemy\\mafia_arm.png", false, this,this->center);
	std::shared_ptr<Item> heldItem(new Pistol);
	this->ammo = std::shared_ptr<Item>(new MusketBallItem);
	a->setHeldItem(heldItem);
	this->setArms(1, { a });
}

void MafiaMan::update() {
	ArmedEntity::update();
	std::cout << this->onGround << " " << this->tileCollision << "\n";
	if (this->arms[0]->canUseHeldItem()) {
		double r = -Main::getAngle(this->arms[0]->getShoulderPos(), Main::player->center);
		
	}
	if (this->onGround) this->velocity.X = 0;
}