#include "Entity.h"
#include "Tiles.h"
#include "Main.h"
#include "debug.h"
#include "Vector2.h"
#include "Colliders.h"
void Entity::update() {
	this->velocity = moveEntity(this->velocity);
}



void::Entity::getEntityCollisions() {

}

void Entity::onTileCollision() {

}

void Entity::onHitNPC() {

}

void Entity::onHitPlayer() {
	
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
	if (!this->renderToScreen) return false;
	iVector2 renderPos = Main::convertWorldPosToCameraPos(this->position);
	Vector2 dims = { Main::convertTileSizeToPixelSize(this->width*this->scale), Main::convertTileSizeToPixelSize(this->height*this->scale) };
	SDL_Rect r = { renderPos.X,renderPos.Y,dims.X,dims.Y };
	SDL_Rect partToRender = Main::getTextureFrame(this->animationFrame, this->frameCount, this->texture, this->textureWidth, this->textureHeight, 0, 0);
	SDL_RenderCopyEx(Main::renderer, this->texture, &partToRender, &r,this->rotation,NULL,this->getSpriteDirection());
	std::cout << this->rotation << "\n";
	if (Debug::renderHitboxes) this->renderHitboxes();
	return true;
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

Entity::Entity(Vector2 position, float width, float height,int health,std::string pathToTexture, bool gravity,bool defaultCollider, bool tileCollision) : position(position),width(width),height(height),gravity(gravity),tileCollision(tileCollision){
	this->setTexture(pathToTexture);
	setPos(position.X,position.Y);
	if (defaultCollider) hitboxes.push_back(new SquareHitbox(this->position, this->width, this->height));
	this->rotationPoint = { Main::convertTileSizeToPixelSize(width)/2,Main::convertTileSizeToPixelSize(height) / 2 };
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

void Entity::setRotation(double rotation) {
	this->rotation = rotation;
	for (Hitbox* hitbox : this->hitboxes) {
		if (hitbox->type == hitboxType::ROTATABLE) {
			dynamic_cast<RotatableHitbox*>(hitbox)->setRotation(rotation);
		}
	}
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
		if (velocity.X < 0) this->hDirection = -1;
		else if (velocity.X > 0) this->hDirection = 1;
		if (!Debug::noclip) tiles = this->collidesWithTiles();
		if (tiles.size() > 0) {
			this->onTileCollision();
			velocity.X = 0;
			this->setX(oldPos.X);
		}
	}
	else velocity.X = 0;
	if (this->setY(this->position.Y + velocity.Y)) {
		onGround = false;
		if (velocity.Y < 0) this->vDirection = -1;
		else if (velocity.Y > 0) this->vDirection = 1;
		if (!Debug::noclip) tiles = this->collidesWithTiles();
		if (tiles.size() > 0) {
			this->onTileCollision();
			if (this->vDirection == 1) onGround = true;
			velocity.Y = 0;
			this->setY(oldPos.Y);
		}
		else if (this->gravity == true && velocity.Y != this->maxYVelocity) velocity.Y += vAcceleration;
	}
	else velocity.Y = 0;
	return velocity;
}

void Entity::setTexture(std::string path) {
	this->deleteTexture();
	this->texture = IMG_LoadTexture(Main::renderer, path.c_str());
	assert(SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureHeight) == 0);
}


void Entity::deleteTexture() {
	if (this->texture != nullptr) SDL_DestroyTexture(this->texture);
	this->texture = nullptr;
}

void Entity::kill() {
	this->markForDeletion = true;
}

bool Entity::hurt(int dmg,float kb,Entity* src) {
	this->checkDmgImmune();
	if (this->invulnerable) return false;
	this->health -= dmg;
	float kbres = (kbResist == 0) ? 1 : kbResist;
	kb = kb / kbres;

	this->velocity += {kb, -abs(kb)*((float)rand() / RAND_MAX)};
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