#include <cstdint>
#include "game_datatypes.h"
#include <vector>
#pragma once
struct SDL_Texture;
struct SDL_Renderer;
class Tile;
class Player;
class SquareHitbox;
class Camera;
namespace Main {
    extern std::vector<std::vector<Tile*>> tiles;
}


class Tile {
public:
    int health = 100;
    bool translucent = false;
    uint16_t X;
    uint16_t Y;
    uint16_t lightStrength = 0;
    bool isLight = false;
    bool isMineable = true;
    bool isMultiTile = false;
    bool isSolid = true;
    bool dynamicTile = false;
    bool isLiquid = false;
    uint16_t pickaxePower = 0;
    bool getIsMineable();
    bool getIsSolid();
    bool getIsLiquid();
    int getPickaxePower();
    template <typename tileInstance>
    static inline void create(tileInstance tile) {
        static_assert(std::is_base_of_v<Tile, tileInstance>, "tileInstance not of type tile");
        if (Main::tiles[tile.X][tile.Y] != nullptr) Main::tiles[tile.X][tile.Y]->destroy();
        Main::tiles[tile.X][tile.Y] = new tileInstance(tile);
    }
    inline virtual void create();
    inline virtual void destroy();
    virtual void updateLightMap();
    virtual void onRightClick(Player* player);
    virtual void onLeftClick(Player* player);
    virtual void update();
    virtual SDL_Texture* getTexture();
    virtual bool draw(SDL_Renderer* renderer, Camera& camera);
    inline virtual SquareHitbox getHitbox();
    bool collides(Vector2 pos);
    Tile(uint16_t tileID, uint16_t X, uint16_t Y);
    Tile();
    virtual ~Tile();
    int getID();
protected:
    uint16_t tileID = 0;
};

class MultiTileObject : public Tile {
    //must be created with new unlike normal tiles
protected:
    bool placeIntoWorld;

public:
    std::vector<Tile*> tileList = {};
    bool isParent = false;
    Tile* parent = nullptr;
    uint16_t width;
    uint16_t height;
    MultiTileObject(uint16_t id, uint16_t X, uint16_t Y, uint16_t W, uint16_t H,bool isParent, MultiTileObject* parent=nullptr);
    virtual void destroy() override;
    bool draw(SDL_Renderer* renderer, Camera& camera) override;
    virtual void create() override;
    void generate();
    virtual void update() override;
    virtual void onLeftClick(Player* player) override;
    virtual void onRightClick(Player* player) override;
};