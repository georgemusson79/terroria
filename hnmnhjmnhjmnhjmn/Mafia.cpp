#include "Entities.h"
#include "Main.h"
#include "Player.h"
#include "Items.h"

MafiaMan::MafiaMan(Vector2 position) : ArmedEntity(position, 2, 3.5, 100, "assets\\enemy\\mafia.png",{},0) {
	this->friendly = false;
	this->hostile = true;
	Vector2 armDims = { this->width / 4,this->height /3  };
	Arm* a = new Arm({ -armDims.X/2,-armDims.Y/5 }, { 0,armDims.Y }, armDims.X, armDims.Y, "assets\\enemy\\mafia_arm.png", false, this,this->center);
	Arm* b = new Arm({ armDims.X / 2,-armDims.Y / 5 }, { 0,armDims.Y }, armDims.X, armDims.Y, "assets\\enemy\\mafia_arm.png", false, this, this->center);
	std::shared_ptr<Item> heldItem(new Pistol);
	this->ammo = std::shared_ptr<Item>(new MusketBallItem);
	
	a->setAlwaysDisplayHeldItem(true);
	b->setAlwaysDisplayHeldItem(true);
	this->setArms(0, { a,b });
	a->setHeldItem(heldItem);
	b->setHeldItem(heldItem);
	this->dmgImmuneMaxTime = 20;
}

void MafiaMan::update() {
	this->hDirection = Main::setSign(Main::player->center.X- this->center.X, this->hDirection);
	ArmedEntity::update();
	if (this->onGround) this->velocity.X = 0;
	//arms[0]->pointAt(Main::player->center);
	//std::cout << this->hDirection << "\n";
	double r = -Main::getAngle(arms[guninuse]->getShoulderPos(), Main::player->center);
	//int notinuse = (guninuse == 0) ? 1 : 0;
	//if (arms[notinuse]->canUseHeldItem()) {
	for (auto arm : arms) {
		arm->restingRotation = -Main::getAngle(arms[guninuse]->getShoulderPos(), Main::player->center);
		arm->useHeldItem(r, ammo->getRangedProjectile(this->center, this, { 0,0 }));
	}

		//guninuse = (guninuse == 0) ? 1 : 0;
	//}

	//if (this->arms[0]->canUseHeldItem()) {
	//	
	//	
	//if (this->onGround) this->velocity.X = 0;
}