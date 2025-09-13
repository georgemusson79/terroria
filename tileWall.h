#pragma once
#include "Tile.h"
class TileWall;
namespace Main {
    extern std::vector<std::vector<TileWall*>> tileWalls;
}
class TileWall : public Tile {
public:
    TileWall(uint16_t tileID, uint16_t X, uint16_t Y);
    virtual void create() override;
    virtual void destroy(bool dropItem=false) override;
    bool draw(SDL_Renderer* renderer, Camera& camera) override;
    SDL_Texture* getTexture() override;
    template <typename tileWallInstance>
    static inline void create(tileWallInstance tileWall) {
        static_assert(std::is_base_of_v<TileWall, tileWallInstance>, "tileInstance not of type tileWall");
        delete Main::tileWalls[tileWall.X][tileWall.Y];
        Main::tileWalls[tileWall.X][tileWall.Y] = new tileWallInstance(tileWall);
    }
};