#include "Entity.h"
#include "Tiles.h"
#include "Main.h"
#include "debug.h"
#include "Vector2.h"
#include "Colliders.h"
#include <fstream>
#include "Player.h"
#include <filesystem>
#include <cmath>
#include <iostream>
void Entity::update() {

	this->velocity = moveEntity(this->velocity);
	this->timeBetweenFrames++;
	if (this->damage > 0) {
		std::vector<Entity*> entities = this->getEntityCollisions();
		for (Entity* e : entities) {
				this->onHitNPC(e, this);
			
		}
		if (Main::player!=nullptr && this->collidesWith(Main::player)) this->onHitPlayer(Main::player, this);
	}
}


std::vector<Entity*> Entity::getEntityCollisions() {
	//can be optimised
	std::vector<Entity*> tgts = {};
	for (Entity* e : Main::entities) if (this->collidesWith(e) && this!=e) tgts.push_back(e);
	return tgts;
}

bool Entity::checkIfOnGround(std::vector<Tile*>& tiles) {
	if (this->vDirection == 1) {
		for (auto& tile : tiles) {
			if (tile->Y >= this->position.Y) {
				return true;
			}
		}
	}
	return false;
}


void Entity::onTileCollision(std::vector<Tile*>& tiles) {

}

bool Entity::onHitNPC(Entity* NPC,Entity* src) {
	if ((this->friendly && NPC->hostile) || (this->hostile && NPC->friendly)) {
		return NPC->hurt(this->damage, this->kbDealt, src);
	}
	return false;
}

bool Entity::onHitPlayer(Player* player,Entity* src) {
	if (this->hostile) return player->hurt(this->damage, this->kbDealt, src);
	return false;
}

bool Entity::setX(double X) {
	if (X + this->width <= Main::WORLD_WIDTH+1 && X>0) {
		this->oldPos.X = this->position.X;
		this->oldPos.Y = this->position.Y;
		this->position.X = X;
		this->center.X =  X + (this->width / 2);
		this->updateHitboxes();
		return true;
	}
	return false;
}

bool Entity::setY(double Y) {
	if (Y + this->height <= Main::WORLD_HEIGHT+1 && Y>0) {
		this->oldPos.Y = this->position.Y;
		this->oldPos.X = this->position.X;
		this->position.Y = Y;
		this->center.Y = Y + (this->height / 2);

		this->updateHitboxes();
		return true;
	}
	return false;
}

bool Entity::setPos(double X, double Y) {
	if (X+this->width <= Main::WORLD_WIDTH+1 && X>0 && Y+this->height <= Main::WORLD_HEIGHT+1 && Y>0) {
		this->oldPos.X = this->position.X;
		this->oldPos.Y = this->position.Y;
		this->position.Y = Y;
		this->position.X = X;
		this->center = { X + (this->width / 2),Y + (this->height / 2) };
		this->updateHitboxes();
		return true;
	}
	return false;
}

bool Entity::setCenter(double X, double Y) {
	Vector2 pos = { X - (this->width / 2),Y - (this->height / 2) };
	return this->setPos(pos.X, pos.Y);
}

bool Entity::renderEntity() {
	if (!this->active) return false;
	if (!this->renderToScreen) return false;
	iVector2 renderPos = Main::convertWorldPosToCameraPos(this->position);
	Vector2 dims = { Main::convertTileSizeToPixelSize(this->width*this->scale), Main::convertTileSizeToPixelSize(this->height*this->scale) };
	SDL_Rect r = { renderPos.X,renderPos.Y,dims.X,dims.Y };
	SDL_Rect partToRender = Main::getTextureFrame(this->animationFrame, this->frameCount, this->texture, this->textureWidth, this->textureHeight, 0, 0);
	SDL_RenderCopyEx(Main::renderer, this->texture, &partToRender, &r,this->rotation,NULL,this->getSpriteDirection());
	if (Debug::renderHitboxes) this->renderHitboxes();
	return true;
}


bool Entity::toBeDeleted() {
	return this->markForDeletion;
}

float Entity::getDistanceTravelled() {
	return std::sqrt(std::pow(distanceTravelled.X, 2) + std::pow(distanceTravelled.Y, 2));
}

void Entity::despawn() {
	this->deleteHitboxes();
	this->markForDeletion = true;
}
void Entity::updateHitboxes() {
	Vector2 offset = this->position - this->oldPos;
	for (int i = 0; i < this->hitboxes.size(); i++) {
		this->hitboxes[i]->setPosition(this->hitboxes[i]->position + offset);
	}
}

SDL_RendererFlip Entity::getSpriteDirection() {
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	if (this->hDirection != 1 && this->canFlipH) flipType = SDL_FLIP_HORIZONTAL;
	if (this->vDirection != 1 && this->canFlipV) flipType = SDL_FLIP_VERTICAL;
	if (this->hDirection != 1 && this->canFlipH && this->canFlipV && this->vDirection != 1) flipType = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	return flipType;
}

Entity::Entity(Vector2 position, float width, float height,int health,std::string pathToTexture, bool gravity,bool defaultCollider, bool tileCollision) : position(position),width(width),height(height),gravity(gravity),tileCollision(tileCollision),health(health),maxHealth(health){
	this->setTexture(pathToTexture);
	setPos(position.X,position.Y);
	if (defaultCollider) hitboxes.push_back(new SquareHitbox(this->position, this->width, this->height));
	this->rotationPoint = { Main::convertTileSizeToPixelSize(width)/2,Main::convertTileSizeToPixelSize(height) / 2 };
	Main::entitiesToSpawn.push_back(this);
}

void Entity::addToEntitiesList() {
	Main::entities.push_back(this);
}

Entity::Entity(Vector2 position, float width, float height,int health, SDL_Texture* t, bool gravity, bool defaultCollider, bool tileCollision) : position(position), width(width), height(height), gravity(gravity), tileCollision(tileCollision),health(health),maxHealth(health) {
	this->texture = t;
	setPos(position.X, position.Y);
	if (defaultCollider) hitboxes.push_back(new SquareHitbox(this->position, this->width, this->height));
	this->rotationPoint = { Main::convertTileSizeToPixelSize(width) / 2,Main::convertTileSizeToPixelSize(height) / 2 };
	Main::entities.push_back(this);
}

Entity::Entity() : renderToScreen(false) {
}

bool Entity::switchFrames(int frame) {
	if (frame<this->frameCount && this->timeBetweenFrames>this->minTimeBetweenFrames) {
		this->animationFrame = frame;
		this->timeBetweenFrames = 0;
		return true;
	}
	return false;
}

void Entity::setRotation(double rotation) {
	if (rotation == this->rotation) return;
	this->rotation = rotation;
	for (Hitbox* hitbox : this->hitboxes) {
		if (hitbox->type == hitboxType::ROTATABLE) {
			dynamic_cast<RotatableHitbox*>(hitbox)->setRotation(rotation);
		}
	}
}

void Entity::setRotationAround(double rotation, Vector2 point,RotationType rotationType) {
	this->rotation = rotation;
	for (Hitbox* hitbox : this->hitboxes) {
		//hitbox->setCenter(Main::rotatePt(hitbox->getCenter(), point, rotation));
		if (hitbox->type == hitboxType::ROTATABLE) {
			RotatableHitbox* hb = dynamic_cast<RotatableHitbox*>(hitbox);
			hb->setRotation(0);
			hb->setRotation(rotation);
		}
	}
	Vector2 pos;
	if (rotationType == RotationType::ABSOLUTE) {
		double angle=Main::getAngle(point, this->center);
		pos = Main::rotatePt(this->center, point, angle);
		this->setCenter(pos.X, pos.Y);
	}
	pos= Main::rotatePt(this->center, point, rotation);
	this->setCenter(pos.X, pos.Y);
}

Entity::~Entity() {

	SDL_DestroyTexture(this->texture);
	this->deleteHitboxes();
}
void Entity::renderHitboxes() {
	for (Hitbox* hitbox:this->hitboxes) hitbox->renderHitbox();
}

void Entity::deleteHitboxes() {
	for (Hitbox* hitbox : this->hitboxes) {
		delete hitbox;
		this->hitboxes = {};
	}
}

bool Entity::collidesWith(Vector2 position) {
	for (Hitbox* hitbox : this->hitboxes) if (hitbox->collidesWith(position)) return true;
	return false;
}

bool Entity::collidesWith(Hitbox* inputHitbox) {
	if (this->entityCollision) for (Hitbox* entityHitbox : this->hitboxes) if (entityHitbox->collidesWith(inputHitbox)) return true;
	return false;
}

bool Entity::collidesWith(Tile* tile) {
	if (tile->isSolid) {
		for (Hitbox* entityHitbox : this->hitboxes) if (entityHitbox->collidesWith(tile)) return true;
	}
	return false;
}



bool Entity::collidesWith(Entity* entity) {
	if (entity==nullptr || entity->getKilled() || this->getKilled() || this->toBeDeleted() || entity->toBeDeleted() || !entity->entityCollision || this->entityCollision == false || !this->active || !entity->active) return false;
	for (Hitbox* hitbox : entity->hitboxes) if (this->collidesWith(hitbox)) return true;
	return false;
}

std::vector<Tile*> Entity::collidesWithTiles() {
	std::vector<Tile*> tiles = {};
	for (Hitbox* hitbox : hitboxes) {
		std::vector<Tile*> tileList = hitbox->collidesWithTiles();
		tiles.insert(tiles.end(), tileList.begin(), tileList.end());
	}
	return tiles;
}


Vector2 Entity::moveEntity(Vector2 velocity) {
	Vector2 oldPos = this->position;
	this->oldVelocity = this->velocity;
	std::vector < Tile*> tiles;
	if (this->setX(this->position.X + velocity.X)) {
		this->distanceTravelled.X += abs(velocity.X);
		if (velocity.X < 0) {
			oldHDirection = this->hDirection;
			this->hDirection = -1;
		}
		else if (velocity.X > 0) {
			oldHDirection = this->hDirection;
			this->hDirection = 1;
		}
		if (!Debug::noclip) tiles = this->collidesWithTiles();
		if (tiles.size() > 0) {
			this->onTileCollision(tiles);
			velocity.X = 0;
			this->setX(oldPos.X);
		}
	}
	else velocity.X = 0;
	if (this->gravity == true && velocity.Y != this->maxYVelocity) velocity.Y += vAcceleration;
	if (this->setY(this->position.Y + velocity.Y)) {
		this->distanceTravelled.Y += abs(velocity.Y);
		onGround = false;
		if (velocity.Y < 0) this->vDirection = -1;
		else if (velocity.Y > 0) this->vDirection = 1;
		if (!Debug::noclip) tiles = this->collidesWithTiles();
		if (tiles.size() > 0) {
			this->onTileCollision(tiles);
			if (this->vDirection == 1) onGround = true;
			velocity.Y = 0;
			this->setY(oldPos.Y);
		}
	}
	else {
		//this->setPos(this->oldPos.Y);
		this->velocity.Y==0;
	}
	
	return velocity;
}

void Entity::setTexture(std::string path) {
	this->deleteTexture();
	this->texture = IMG_LoadTexture(Main::renderer, path.c_str());
	SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureHeight);
}


void Entity::deleteTexture() {
	SDL_DestroyTexture(this->texture);
	this->texture = nullptr;
}

void Entity::kill() {
	this->killed = true;
	this->despawn();
}

bool Entity::getKilled() {
	return this->killed;
}

bool Entity::hurt(int dmg,float kb,Entity* src) {
	if (this->maxHealth == -1 or this->dmgImmuneMaxTime==-1) return false;
	this->checkDmgImmune();
	if (this->invulnerable) return false;
	this->health -= dmg;
	float kbres = (kbResist == 0) ? 1 : kbResist;
	kb = kb / kbres;

	float kbDirection = (src==nullptr) ? 0 : (this->center.X - src->center.X > 0) ? 1 : -1;
	
	this->velocity += {kb*kbDirection, -abs(kb)*((float)rand() / RAND_MAX)};
	this->dmgImmuneTime = SDL_GetTicks();
	this->invulnerable = true;
	if (this->health <= 0) this->kill();
	return true;
}

void Entity::checkDmgImmune() {
	if (this->invulnerable) {
		if (SDL_GetTicks() - dmgImmuneTime > dmgImmuneMaxTime && dmgImmuneMaxTime!=-1) this->invulnerable = false;
	}
}

bool Entity::checkOnSameTeam(Entity* e) {
	if (this->hostile && e->hostile) return true;
	if (e->friendly && this->friendly) return false;
	return false;
}