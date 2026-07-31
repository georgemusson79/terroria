#include "world.h"
#include "Main.h"
#include "TileWalls.h"
#include "Tiles.h"
#include "engineDebug.h"
#include <cmath>
#include <random>

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

void World::updateWorld() {
  int tilesToUpdate = 500;
  for (int i = 0; i < tilesToUpdate; i++) {
    int x = Main::randomInt(0, WORLD_WIDTH);
    int y = Main::randomInt(0, WORLD_HEIGHT);
    if (Main::world->tileAt(x, y) != nullptr)
      Main::world->tileAt(x, y)->randomTickUpdate();
  }
}

void World::temp_populateByValueGen() {

  std::vector<int> tilePoints = this->valueNoiseGen(WORLD_WIDTH, 30, 20);

  for (int x = 0; x < tilePoints.size(); x++) {
    int y = WORLD_HEIGHT / 2;
    y -= tilePoints[x];
    for (int dy = y; dy <= WORLD_HEIGHT; dy++) {
      bool isGrass = (dy == y) ? true : false;
      new WoodWall(x, dy);
      new Dirt(x, dy, true, isGrass);
    }
  }
}

std::vector<int> World::valueNoiseGen(int distance, int frequency,
                                      int amplitude) {
  std::vector<int> yvals(distance);
  int numPoints = std::ceil((float)distance / (float)frequency) + 1;
  std::vector<float> values(numPoints);
  int sz = values.size();

  for (int i = 0; i < values.size(); i++) {
    int rnd = rand() % (amplitude + 1);
    if (rand() % 2 == 0)
      rnd *= -1;

    values[i] = rnd;
  }
  int step = 0;
  for (int pointPos = 0; pointPos < values.size() - 1 && step < yvals.size();
       pointPos++) {

    // smoothstep interpolation
    float A = values[pointPos];
    float B = values[pointPos + 1];
    for (float x = 0; x < frequency; x++) {
      float t = x / (float)frequency;
      t = smoothstep(t);
      float lerp = A + (t * (B - A));
      int intLerp = std::floor(lerp);
      yvals[step] = intLerp;
      step++;
      if (step >= yvals.size())
        break;
    }
  }

  return yvals;
}
