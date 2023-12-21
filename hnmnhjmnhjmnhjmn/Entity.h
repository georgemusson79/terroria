#pragma once
#include "headers.h"
#include "game_datatypes.h"

struct SDL_Point;
struct SDL_texture;

class Hitbox;
class Tile;
class Entity {
protected:
    int frameCount = 1;
    int animationFrame = 0;
    int textureWidth = 0;
    int textureHeight = 0;
    bool markForDeletion = false;
    void addToEntitiesList();
public:
    bool null = false;
    bool renderToScreen = true;
    std::vector<Hitbox*> hitboxes = {};
    Vector2 velocity = { 0,0 };
    Vector2 oldVelocity = { 0,0 };
    float width = 0;
    float height = 0;
    Vector2 oldPos = { 0,0 };
    Vector2 position = { 0,0 };
    int dmgImmuneMaxTime = 1000;
    int dmgImmuneTime = std::numeric_limits<int>::max();
    int vDirection = 1;
    float scale = 1;
    int hDirection = 1;
    double rotation = 0;
    SDL_Point rotationPoint = { 999,999 };
    Vector2 center = { 0,0 };
    SDL_Texture* texture = nullptr;
    bool canFlipH = true;
    bool canFlipV = false;
    int health = 100;
    int maxHealth = 100;
    int damage = 0;
    float kbResist = 0;
    bool melee = false;
    bool invulnerable = false;
    bool onGround = true;
    int fallTime = 0;
    bool entityCollision = true;
    bool tileCollision = true;
    bool gravity = true;
    float vAcceleration = 0.02;
    float hAcceleration = 0.01;
    float maxXVelocity = 0.3;
    float maxYVelocity = 1;

    virtual void update();
    virtual void checkDmgImmune();
    virtual void onHitNPC();
    virtual void onHitPlayer();
    virtual void getEntityCollisions();
    virtual bool setX(double X);
    virtual bool setY(double Y);
    virtual bool setPos(double X, double Y);
    virtual bool setCenter(double X, double Y);
    virtual bool renderEntity();
    SDL_RendererFlip getSpriteDirection();
    virtual void updateHitboxes();
    virtual void renderHitboxes();
    virtual void deleteHitboxes();
    bool toBeDeleted();
    void despawn();
    inline virtual bool collidesWith(Vector2 position);
    inline virtual bool collidesWith(Hitbox* inputHitbox);
    inline virtual bool collidesWith(Tile* tile);
    inline virtual bool collidesWith(Entity* entity);
    virtual std::vector<Tile*> collidesWithTiles();
    virtual Vector2 moveEntity(Vector2 velocity);
    virtual void onTileCollision();
    void setTexture(std::string path);
    void deleteTexture();
    virtual bool hurt(int dmg, float kb = 0,Entity* src=nullptr);
    virtual void kill();
    Entity(Vector2 position, float width, float height, int health, std::string pathToTexture, bool gravity = true, bool defaultCollider = true, bool tileCollision = true);
    Entity(Vector2 position, float width, float height,int health, SDL_Texture* t, bool gravity = true, bool defaultCollider = true, bool tileCollision = true);
    Entity();
    virtual ~Entity();
    void setRotation(double rotation);
};