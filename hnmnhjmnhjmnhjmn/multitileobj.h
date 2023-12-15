#pragma once
#include "Tiles.h"
class MultiTileObject : public Tile {
protected:
    bool placeIntoWorld;

public:
    std::vector<Tile*> tileList = {};
    bool isParent = false;
    Tile* parent = nullptr;
    uint16_t width;
    uint16_t height;
    MultiTileObject(uint16_t id, uint16_t X, uint16_t Y, uint16_t W, uint16_t H, bool isParent, MultiTileObject* parent = nullptr);
    virtual void destroy() override;
    bool draw(SDL_Renderer* renderer, Camera& camera) override;
    template <typename T>
    void create() {
        this->placeIntoWorld = true;
        if (isParent) {
            this->generate();
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    if (x != 0 || y != 0) {
                        //MultiTileObject* cpy = new MultiTileObject(*this);
                        T* cpy = new T(*this);
                        cpy->isParent = false;
                        cpy->parent = this;
                        cpy->X += x;
                        cpy->Y += y;
                        cpy->generate();
                        this->tileList.push_back(Main::tiles[X + x][Y + y]);
                    }
                }
            }
        }
    }
    virtual void generate();
    virtual void update() override;

};