#include "tileWall.h"
#include "Main.h"
#include "Tiles.h"
#include <cassert>
#include <cmath>

SDL_Texture *TileWall::getTexture() {
  return Main::tileWallTexture[this->getID()];
}

bool TileWall::draw(SDL_Renderer *renderer, Camera &camera) {
  if (Main::world->tileAt(this->X, this->Y) == nullptr ||
      Main::world->tileAt(this->X, this->Y)->translucent) {
    CornersRect bounds = camera.cameraBounds;
    Vector2 topLeft = bounds.topLeft;
    int zoom = camera.zoom();
    double newX = (this->X - topLeft.X) * zoom;
    double newY = (this->Y - topLeft.Y) * zoom;
    newX = std::round(newX);
    newY = std::round(newY);
    SDL_Rect dstrect = {newX, newY, camera.zoom(), camera.zoom()};
    if ((dstrect.x < Main::WINDOW_WIDTH && dstrect.x + dstrect.w > 0) &&
        (dstrect.y < Main::WINDOW_HEIGHT && dstrect.y + dstrect.h > 0)) {
      double brightness = ((double)Main::lightMap[this->X][this->Y] / 15) * 255;
      if (brightness > 255)
        brightness = 255;
      SDL_SetTextureColorMod(this->getTexture(), brightness, brightness,
                             brightness);
      SDL_RenderCopy(renderer, this->getTexture(), NULL, &dstrect);
      return true;
    }
  }
  return false;
}

TileWall::TileWall(uint16_t tileID, uint16_t X, uint16_t Y)
    : Tile(tileID, X, Y, 10) {}

void TileWall::create() {
  bool isInRange = Main::world->isInWorld(this->X, this->Y);
  if (!isInRange)
    std::cout << "\n"
              << this->X << " " << this->Y << " " << Main::world->WORLD_WIDTH
              << " " << Main::world->WORLD_HEIGHT << "\n";
  assert(isInRange);
  Main::world->setTileWallAt(this->X, this->Y, nullptr);
  Main::world->setTileWallAt(this->X, this->Y, new TileWall(*this));
}

void TileWall::destroy(bool dropItem) {
  Main::world->setTileWallAt(this->X, this->Y, nullptr);
  if (dropItem)
    this->dropItem();
  delete this;
}
