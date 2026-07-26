#ifndef AD23E3F6_AE31_4D9F_B95E_EB3BCD5AA512
#define AD23E3F6_AE31_4D9F_B95E_EB3BCD5AA512

#include <iostream>
#include <memory>
#include <vector>

class TileWall;
class Tile;

class World {
public:
  uint16_t WORLD_WIDTH = 7000;
  uint16_t WORLD_HEIGHT = 1500;
  World();
  ~World();
  std::vector<std::vector<Tile *>> tiles;
  std::vector<std::vector<TileWall *>> tileWalls;
  void sinTiles();
  template <typename TileInstance, typename TileWallInstance>
  void temp_populateHalfTiles(TileInstance tile, TileWallInstance tilewall) {
    static_assert(std::is_base_of_v<Tile, TileInstance>,
                  "input is not of type tile");
    static_assert(std::is_base_of_v<TileWall, TileWallInstance>,
                  "input is not of type tilewall");
    for (int y = WORLD_HEIGHT / 2; y <= WORLD_HEIGHT; y++) {
      for (int x = 0; x <= WORLD_WIDTH; x++) {
        new TileWallInstance(x, y);
        new TileInstance(x, y);
      }
    }
  }

  template <typename TileInstance, typename TileWallInstance>
  void temp_populateByValueGen(TileInstance tile, TileWallInstance tilewall) {
    static_assert(std::is_base_of_v<Tile, TileInstance>,
                  "input is not of type tile");
    static_assert(std::is_base_of_v<TileWall, TileWallInstance>,
                  "input is not of type tilewall");

    std::vector<int> tilePoints = this->valueNoiseGen(WORLD_WIDTH, 30, 20);

    for (int x = 0; x < tilePoints.size(); x++) {
      int y = WORLD_HEIGHT / 2;
      y -= tilePoints[x];
      for (int dy = y; dy <= WORLD_HEIGHT; dy++) {
        new TileWallInstance(x, dy);
        new TileInstance(x, dy);
      }
    }
  }

  std::vector<int> valueNoiseGen(int distance, int frequency, int amplitude);

  bool isInWorld(int x, int y) {
    return (x >= 0 && x <= WORLD_WIDTH && y >= 0 && y <= WORLD_HEIGHT);
  }

  Tile *tileAt(int x, int y) { return tiles[x][y]; }
  bool setTileAt(int x, int y, Tile *tile) {
    if (isInWorld(x, y)) {
      tiles[x][y] = tile;
      return true;
    }
    return false;
  }
  TileWall *tileWallAt(int x, int y) { return tileWalls[x][y]; }
  bool setTileWallAt(int x, int y, TileWall *tilewall) {
    if (isInWorld(x, y)) {
      tileWalls[x][y] = tilewall;
      return true;
    }
    return false;
  }

private:
  float smoothstep(float t) { return t * t * (3.0f - 2.0f * t); }
};

#endif /* AD23E3F6_AE31_4D9F_B95E_EB3BCD5AA512 */
