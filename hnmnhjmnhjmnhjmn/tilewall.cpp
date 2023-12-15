#include "TileWall.h"
#include "Main.h"
#include "Tiles.h"
#include <cassert>



SDL_Texture* TileWall::getTexture() {
    return Main::tileWallTexture[this->getID()];
}

bool TileWall::draw(SDL_Renderer* renderer, Camera& camera) {
    if (Main::tiles[this->X] [this->Y] == nullptr || Main::tiles[this->X][this->Y]->translucent) {
        CornersRect bounds = camera.cameraBounds;
        Vector2 topLeft = bounds.topLeft;
        int zoom = camera.zoom();
        double newX = (this->X - topLeft.X) * zoom;
        double newY = (this->Y - topLeft.Y) * zoom;
        newX = std::round(newX);
        newY = std::round(newY);
        SDL_Rect dstrect = { newX,newY,camera.zoom(),camera.zoom() };
        if ((dstrect.x < Main::WINDOW_WIDTH && dstrect.x + dstrect.w>0) && (dstrect.y < Main::WINDOW_HEIGHT && dstrect.y + dstrect.h>0)) {
            double brightness = ((double)Main::lightMap[this->X][this->Y] / 15) * 255;
            if (brightness > 255) brightness = 255;
            SDL_SetTextureColorMod(this->getTexture(), brightness, brightness, brightness);
            SDL_RenderCopy(renderer, this->getTexture(), NULL, &dstrect);
            return true;
        }
    }
    return false;
}

TileWall::TileWall(uint16_t tileID, uint16_t X, uint16_t Y) : Tile(tileID, X, Y) {

}

void TileWall::create() {
    bool isInRange = (this->X >= 0 && this->Y >= 0 && this->Y < Main::tileWalls[0].size() && this->X < Main::tileWalls.size());
    assert(isInRange);
    delete Main::tileWalls[this->X][this->Y];
    Main::tileWalls[this->X][this->Y] = new TileWall(*this);
}

void TileWall::destroy() {
    Main::tileWalls[this->X][this->Y] = nullptr;
    delete this;
}
