#include "world.h"
#include "Main.h"
#include "Tiles.h"
#include <cmath>

World::World() {
  int width = WORLD_WIDTH + 10;
  int height = WORLD_HEIGHT + 10;
  tiles = std::vector<std::vector<Tile *>>(width, std::vector<Tile *>(height));
  tileWalls = std::vector<std::vector<TileWall *>>(
      width, std::vector<TileWall *>(height));
}
World::~World() {}

void World::sinTiles() {
  int prevY = NULL;
  int y = 0;
  for (int x = 0; x < WORLD_WIDTH; x++) {
    double radX = x * (M_PI / 180);
    y = abs(sin(radX) * (WORLD_HEIGHT / 4)) + 1000;
    if (y > 0 && y < Main::WINDOW_HEIGHT) {
      if (prevY != NULL) {
        int mdy = abs(prevY - y);
        for (int dy = prevY; dy < mdy + prevY; dy++) {
          if (dy > 0 && dy < WORLD_HEIGHT)
            Dirt(x, dy);
          /*      for (dy + 4; y < Main::world->WORLD_HEIGHT; y++) {
                    if (y < prevY + 20) Dirt(x, y);
                    else Stone(x, y);
                }*/
        }
      }
      prevY = y;
    }
  }
}