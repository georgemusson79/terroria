//Player.cpp
#include "Main.h"
#include "debug.h"
#include "Player.h"
#include "Colliders.h"
#include "Vector2.h"
#include "Item.h"
#include "Cursor.h"
#include "Tiles.h"
#include "Item_pickup.h"
#include "Entities.h"
Player::Player(Vector2 pos) {
	this->displayName = "player";
	this->frameCount = 3;
	this->renderToScreen = true;
	this->health = 400;
	this->maxHealth = 400;
	this->setPos(pos.X,pos.Y);
	this->width =2;
	this->maxXVelocity = 0.4;
	this->height = 3.5;
	this->setTexture("assets\\player\\plr.png");
	Vector2 armDims= { this->width * 0.2,this->height * 0.4};
//	this->armPos = { center.X - (armDims.X / 2),position.Y + (this->height * 0.4) };
	this->arm = new Arm({ 0,-0.4 }, { 0, armDims.Y }, armDims.X, armDims.Y, "assets\\player\\arm.png", false, this,this->position);
	//this->armTexture = IMG_LoadTexture(Main::renderer,"assets\\player\\arm.png");
	this->hitboxes.push_back(new SquareHitbox({ this->position.X,this->position.Y }, this->width, this->height));
	this->inventory= std::vector<std::vector<std::shared_ptr<Item>>>(inventoryRows, std::vector<std::shared_ptr<Item>>(inventoryColumns));
}

Player::Player() {
	
}

Player::~Player() {
	this->arm->despawn();
	//if (this->swingItem!=nullptr) this->swingItem->despawn();
}



void Player::update() {
	this->handleKeyInput();
	this->handleMouseInput();
	this->velocity = this->moveEntity(this->velocity);
	if (this->openedChest != nullptr && this->position.distance({ this->openedChest->X,this->openedChest->Y }) > 9) this->openedChest->close();
	this->animationTimePassed++;;
	Main::camera->setPos(this->center.X, this->center.Y);
	this->handleWalking();
	checkDmgImmune();
	Main::updateLightMap(this->center, 500);
	Main::player->isCollidingWithGUI = false;
}


void Player::clearInventory() {
	for (int row = 0; row < this->inventoryRows; row++) {
		for (int col = 0; col < this->inventoryColumns; col++) {
			this->inventory[row][col].reset();
		}
	}
}



bool Player::hurt(int dmg, float kb,Entity* src) {
	if (Entity::hurt(dmg,kb,src)) {
		this->invulnerable = true;
		this->dmgImmuneTime = SDL_GetTicks();
		return true;
	}
	return false;

}

void Player::kill() {
	if (this == Main::player) {
		Main::player = nullptr;
		delete this;
	}
}


bool Player::pickup(std::shared_ptr<Item> item) {
	for (int x = 0; x < inventoryRows; x++) {
		for (int y = 0; y < inventoryColumns; y++) {
			if (inventory[x][y] == nullptr) {
				inventory[x][y] = item;
				return true;
			}
			else if (inventory[x][y]->id == item->id && (inventory[x][y]->count + item->count) <= item->maxStack) {
				inventory[x][y]->setCount(inventory[x][y]->count + item->count);
				return true;
			}
		}
	}
	return false;
}


//bool Player::renderEntity() {
//	if (!this->renderToScreen) return false;
//	int alpha = 255;
//	if (this->invulnerable) alpha = 128;
//	SDL_SetTextureAlphaMod(this->texture, alpha);
//	SDL_SetTextureAlphaMod(this->armTexture, alpha);
//	iVector2 renderPos = Main::convertWorldPosToCameraPos(this->position);
//	Vector2 dims = { Main::convertTileSizeToPixelSize(this->width * this->scale), Main::convertTileSizeToPixelSize(this->height * this->scale) };
//	SDL_Rect r = { renderPos.X,renderPos.Y,dims.X,dims.Y };
//	SDL_Rect partToRender = Main::getTextureFrame(this->animationFrame, this->frameCount, this->texture, this->textureWidth, this->textureHeight, 0, 0);
//	SDL_RenderCopyEx(Main::renderer, this->texture, &partToRender, &r, this->rotation, &this->rotationPoint, this->getSpriteDirection());
//	if (Debug::renderHitboxes) this->renderHitboxes();
//
//	this->armDims= { this->width * 0.2,this->height * 0.4};
//	this->armPos = { center.X - (armDims.X / 2),position.Y + (this->height * 0.4) };
//	SDL_Point armdims = { Main::convertTileSizeToPixelSize(armDims.X), Main::convertTileSizeToPixelSize(armDims.Y) };
//	iVector2 armpos = Main::convertWorldPosToCameraPos({ center.X-(armDims.X/2),position.Y+(this->height*0.4)});
//	SDL_Rect armRect = { armpos.X,armpos.Y,armdims.x,armdims.y };
//	SDL_Point rotationPt = { armRect.w/2, 0};
//	double rotation = armRotation * this->hDirection;
//	SDL_RenderCopyEx(Main::renderer, this->armTexture, NULL, &armRect,rotation,&rotationPt,this->getSpriteDirection());
//	Vector2 handPos = this->getHandPos();
//	Main::drawSquare(handPos, { 255,0,0 }, armRect.w);
//
//
//	return true;
//}




Vector2 Player::moveEntity(Vector2 velocity) {
	Vector2 vel=Entity::moveEntity(velocity);
	if (!this->isWalking && this->onGround)  {
		vel.X = 0;
	}
	return vel;
}

void Player::jump() {
	if (this->onGround) this->velocity.Y =-abs(jumpVelocity) - vAcceleration;
}


void Player::rightClick() {
	Vector2 pos = Cursor::cursorWorldPos;
	if (Main::tiles[pos.X][pos.Y] != nullptr) {
		Main::tiles[pos.X][pos.Y]->onRightClick(this);
		//Main::tiles[pos.X][pos.Y]->destroy();
	}
}

bool Player::dropItem(std::shared_ptr<Item>& item) {
	//there is probably something wrong here
	//TODO: track item and see where it disappears
	if (item != nullptr) {
		std::shared_ptr<Item> item2;
		item2.swap(item);
		ItemPickup* pickup = new ItemPickup(item2, this->position);
		pickup->velocity = Vector2(0.3 * this->hDirection, -0.3);
	}
	return false;
}


void Player::leftClick() {
	Vector2 pos = Cursor::cursorWorldPos;
	if (Main::tiles[pos.X][pos.Y] != nullptr) Main::tiles[pos.X][pos.Y]->onLeftClick(this);
	if (this->arm->timeToNextUse > 0) return;
//use item
	std::shared_ptr<Item> &selectedItem = this->inventory[0][selectedHBItem];
	if (selectedItem.get() != nullptr) {
		this->arm->useItemCancel();
		this->arm->setHeldItem(selectedItem);
		float r = -Main::getAngle(this->center, pos);
		if (this->arm->useHeldItem(r) && selectedItem->consumable) {
			selectedItem->setCount(selectedItem->getCount() - 1);
			if (selectedItem->getCount() == 0)  selectedItem = nullptr;
		}
	}
}

void Player::handleMouseInput() {
	Uint32 mousePressed = SDL_GetMouseState(nullptr, nullptr);
	if (mousePressed & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		this->leftClick();
		//auto pos = Cursor::cursorWorldPos;
		//if (mousePressed!=Main::heldMouseKeys && !Main::player->isCollidingWithGUI) Chest* h = new Chest(pos.X, pos.Y, true); 
	}

	else if (mousePressed & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		this->rightClick();
	}
	Main::heldMouseKeys = mousePressed;
}

void Player::handleKeyInput() {
	const Uint8* keys=SDL_GetKeyboardState(nullptr);
	this->isWalking = false;
	if (keys[SDL_SCANCODE_SPACE]) {
		this->jump();
	}

	if (keys[SDL_SCANCODE_A]) {
		this->velocity.X -= (this->velocity.X > -this->maxXVelocity) ? this->hAcceleration : 0;
		this->isWalking = true;
	}


	if (keys[SDL_SCANCODE_D]) {
		this->velocity.X += (this->velocity.X < maxXVelocity) ? this->hAcceleration : 0;
		this->isWalking = true;
	}

	if (keys[SDL_SCANCODE_Q] && !Main::heldKeys[SDL_SCANCODE_Q]) this->dropItem(this->inventory[0][selectedHBItem]);

	if (keys[SDL_SCANCODE_ESCAPE] && !Main::heldKeys[SDL_SCANCODE_ESCAPE]) {
		Main::player->inventoryOpen = !Main::player->inventoryOpen;
		if (this->openedChest != nullptr) this->openedChest->close();
	}

	if (keys[SDL_SCANCODE_K] && !Main::heldKeys[SDL_SCANCODE_K]) Main::camera->setZoom(Main::camera->zoom() - 1);
	memcpy(Main::heldKeys, keys, SDL_NUM_SCANCODES);
}

bool Player::handleWalking() {
	if (abs(velocity.X) < 0.03) {
		animationFrame = (int)PlayerSprite::IDLE;
		if (!this->arm->usingItem) this->arm->rotation = 0;
		return false;
	}
 

	if (this->animationTimePassed < 15) return false;
	PlayerSprite walkAnims[] = {PlayerSprite::WALK1,PlayerSprite::IDLE,PlayerSprite::WALK2,PlayerSprite::IDLE};
	this->walkFrame++;
	if (this->walkFrame == std::size(walkAnims)) this->walkFrame = 0;
	this->animationFrame = (int)walkAnims[this->walkFrame];
	this->animationTimePassed = 0;
	if (!this->arm->usingItem) {
		switch (walkFrame) {
		case (0):
			this->arm->rotation = -45 * this->hDirection;
			break;
		case (2):
			this->arm->rotation = 45 * this->hDirection;;
			break;
		default:
			this->arm->rotation = 0;
		}

	}
	return true;
}

//void Player::swingAnim(Item* item) {
//	animationOverride = true;
//	if (!this->usingItem) {
//		this->heldItem = *item;
//		this->swingItem = item->getItemProjectile(this->armPos,this);
//		this->this->arm->rotation = 90;
//		this->usingItem = true;
//	}
//
//
//	if (this->usingItem and this->swingItem!=nullptr and this->this->arm->rotation<300 and this->heldItem.useTime!=0) {
//	//bruh this code sucks
//		this->this->arm->rotation += double(270 - 90) / this->heldItem.useTime;
//		double itemRotation = (this->arm->rotation + 180) * hDirection;
//		if (this->swingItem != nullptr) {
//			this->swingItem->setRotation(0);
//			Vector2 handPos = this->getHandPos(this->swingItem->handOffset);
//			Vector2 itemCenter = { handPos.X + (swingItem->width/2),handPos.Y + (swingItem->height/2) };
//			//move item center to account for rotation offset so that it remains in the players hand
//			Vector2 newPos=Main::rotatePt(itemCenter, handPos, this->arm->rotation*hDirection);
//			this->swingItem->setCenter(newPos.X, newPos.Y);
//			
//			this->swingItem->setRotation(itemRotation);
//			this->tempRotation += 1;
//			
//		}
//		
//	}
//
//
//	else {
//		this->animationOverride = false;
//		this->usingItem = false;
//		this->this->arm->rotation = 0;
//		this->swingItem->despawn();
//		this->swingItem = nullptr;
//	}
//
//}

//Vector2 Player::getHandPos(Vector2 offset) {
//	Vector2 base = { this->armPos.X + (this->armDims.X / 2),this->armPos.Y };
//	Vector2 end = { base.X,base.Y + this->armDims.Y };
//	end = Main::rotatePt(end-offset, base, this->arm->rotation*this->hDirection);
//	return end;
//}