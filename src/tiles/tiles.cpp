// tile.cpp
#include "Tiles.h"
#include "Item_pickup.h"
#include "Items.h"
#include "Main.h"
Wood::Wood(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(WOOD, X, Y, 20) {
  this->minPickaxePower = 0;
  if (placeIntoWorld)
    Tile::create(*this);
}

void Wood::dropItem() {
  new ItemPickup(std::shared_ptr<Item>(new WoodItem), {this->X, this->Y});
}

Stone::Stone(uint16_t X, uint16_t Y, bool placeIntoWorld)
    : Tile(STONE, X, Y, 20) {
  this->minPickaxePower = 0;
  if (placeIntoWorld)
    Tile::create(*this);
}

Air::Air(uint16_t X, uint16_t Y, bool placeIntoWorld) : Tile(AIR, X, Y, -1) {
  this->isSolid = false;
  this->isMineable = false;
  if (placeIntoWorld)
    Tile::create(*this);
}

Dirt::Dirt(uint16_t X, uint16_t Y, bool placeIntoWorld, bool grassOnTop)
    : Tile(DIRT, X, Y, 15) {
  this->minPickaxePower = 0;
  this->grassOnTop = grassOnTop;
  if (placeIntoWorld)
    this->create();
}

void Dirt::randomTickUpdate() {
  if (!this->grassOnTop)
    return;
  Vector2 adjTileLeft = {this->X - 1, this->Y};
  Vector2 adjTileRight = {this->X + 1, this->Y};
  Vector2 adjTileTopLeft = {this->X - 1, this->Y - 1};
  Vector2 adjTileTopRight = {this->X + 1, this->Y - 1};
  std::vector<Vector2> adjTiles = {adjTileLeft, adjTileRight, adjTileTopLeft,
                                   adjTileTopRight};

  for (Vector2 tilePos : adjTiles) {
    Tile *tile = Main::world->tileAt(tilePos.X, tilePos.Y);
    Tile *tileAbove = Main::world->tileAt(tilePos.X, tilePos.Y - 1);
    if (tile == nullptr || tile->getID() != DIRT)
      continue;

    if (tileAbove != nullptr && tileAbove->getID() != AIR)
      continue;

    Dirt *dirt = dynamic_cast<Dirt *>(tile);

    if (!dirt->grassOnTop) {
      dirt->grassOnTop = true;
      return;
    }
  }
}

void Dirt::update() {
  if (this->grassOnTop) {
    Vector2 tileAbovePos = {this->X, this->Y - 1};
    Tile *tileAbove = Main::world->tileAt(tileAbovePos.X, tileAbovePos.Y);
    if (tileAbove != nullptr && tileAbove->getID() != AIR) {
      this->grassOnTop = false;
    }
  }
}

SDL_Texture *Dirt::getTexture() {
  if (this->grassOnTop)
    return Main::tileTexture[GRASS];
  return Main::tileTexture[tileID];
}

Obamium::Obamium(uint16_t X, uint16_t Y, bool placeIntoWorld)
    : Tile(DIRT, X, Y, 500) {
  if (placeIntoWorld)
    Tile::create(*this);
}
