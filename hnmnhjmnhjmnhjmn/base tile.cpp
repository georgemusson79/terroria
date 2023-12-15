//tile.cpp
#include "game_datatypes.h"
#include "Tiles.h"
#include "Tile.h"
#include "Main.h"
#include "Colliders.h"
#include "Player.h"
#include <cassert>
Tile::Tile(uint16_t tileID, uint16_t X, uint16_t Y) : tileID(tileID), X(X), Y(Y) {
}
Tile::Tile() : tileID(AIR), X(0),Y(0),isSolid(false),isMineable(false) {
}

int Tile::getID() {
    return this->tileID;
}
bool Tile::draw(SDL_Renderer* renderer, Camera &camera) {
    if (tileID != AIR) {
        /*double newX = (this->X - pos.X);
        double newY = (this->Y - pos.Y);
        SDL_Rect dstrect = { newX*camera.zoom(),newY*camera.zoom(),camera.zoom(),camera.zoom() };*/
        CornersRect bounds = camera.cameraBounds;
        Vector2 topLeft = bounds.topLeft;
        int zoom = camera.zoom();
        double newX = (this->X - topLeft.X) * zoom;
        double newY = (this->Y - topLeft.Y) * zoom;
        newX = std::round(newX);
        newY = std::round(newY);
        SDL_Rect dstrect = { newX,newY,camera.zoom(),camera.zoom() };
        if ((dstrect.x < Main::WINDOW_WIDTH && dstrect.x+dstrect.w>0) && (dstrect.y < Main::WINDOW_HEIGHT && dstrect.y+dstrect.h>0)) {
            double brightness = ((double)Main::lightMap[this->X][this->Y] / 15)*255;
            if (brightness > 255) brightness = 255;
            SDL_SetTextureColorMod(this->getTexture(), brightness, brightness, brightness);
            SDL_RenderCopy(renderer, this->getTexture(), NULL, &dstrect);
            return true;
        }
    }
    return false;
}

SDL_Texture* Tile::getTexture() {
    return Main::tileTexture[tileID];
}

//Tile& Tile::operator=(Tile& tile) {
//    this->hitbox = tile.hitbox;
//    this->isLiquid = tile.isLiquid;
//    this->X = tile.X;
//    this
//}

void Tile::create() {
    bool isInRange = (this->X >= 0 && this->Y >= 0 && this->Y < Main::tiles[0].size() && this->X < Main::tiles.size());
    if (!isInRange) std::cout << "\n" << this->X << " " << this->Y << " " << Main::tiles[0].size() << " " << Main::tiles.size() << "\n";
    assert(isInRange);
    if (Main::tiles[X][Y] != nullptr) Main::tiles[X][Y]->destroy();
    Main::tiles[X][Y] = new Tile(*this);
    if (this->lightStrength > 0) this->updateLightMap();

}

void Tile::destroy() {
    if (this->isMultiTile) {
        Tile* tile = Main::tiles[X][Y];
        MultiTileObject* obj = dynamic_cast<MultiTileObject*>(tile);
        obj->destroy();
    }

    else if (this->X <= Main::WORLD_WIDTH && this->Y <= Main::WORLD_HEIGHT && this->X >= 0 && this->Y >= 0) {
        Main::tiles[X][Y] = nullptr;
        delete this;
    }
}


bool Tile::getIsSolid() {
    return this->isSolid;
}

bool Tile::getIsLiquid() {
    return this->isLiquid;
}

bool Tile::getIsMineable() {
    return this->isMineable;
}

int Tile::getPickaxePower() {
    return this->pickaxePower;
}

bool Tile::collides(Vector2 pos) {
    if (pos.X >= this->X && pos.X < this->X + 1 && pos.X >= this->Y && pos.Y < this->Y + 1) return true;
    return false;
}

void Tile::update(){
}

Tile::~Tile() {
}
SquareHitbox Tile::getHitbox() {
    //only works correctly if hitbox width/height is less than or equal to 1
    //hitbox generates from top left to bottom right
    return SquareHitbox({ (double)X,(double)Y }, 1,1);
}

void Tile::onRightClick(Player* player) {
}
void Tile::onLeftClick(Player* player) {
}

void Tile::updateLightMap() {
    Main::updateLightMap({ this->X,this->Y }, this->lightStrength);
}