#pragma once
//main.h
#include "headers.h"
#include "game_datatypes.h"
class Hitbox;
class Player;
class CircleHitbox;
class Entity;
class Tile;
class Camera;
class Player;
class TileWall;
class Item;
class ItemPickup;
struct Line;

enum skyBackgrounds {
    DEFAULT
};

enum class cursorType {
    MANUAL,SMART
};




namespace Main {
    extern int frames;
    extern SDL_Cursor* cursor;
    extern bool isSmartCursorEnabled;
    extern std::map<int, SDL_Texture*> tileTexture;
    extern std::map<int,SDL_Texture*> tileWallTexture;
    extern std::vector<std::vector<Tile*>> tiles;
    extern std::vector<std::vector<uint16_t>> lightMap;
    extern std::vector<iVector2> changedLights;
    extern std::vector<std::vector<TileWall*>> tileWalls;
    extern std::map<int, SDL_Texture*> backgrounds;
    extern uint16_t defaultLightValue;
    extern std::vector<Entity*> entities;
    extern std::vector<Entity*> entitiesToSpawn;
    extern SDL_Renderer* renderer;
    extern SDL_Window* window;
    extern bool scaleToWindowSize;
    extern uint16_t WORLD_WIDTH;
    extern uint16_t WORLD_HEIGHT;
    extern int WINDOW_WIDTH;
    extern int WINDOW_HEIGHT;
    extern double gravity;
    extern Camera* camera;
    extern Player* player;
    extern CircleHitbox* testcbox;
    extern Player* p2; //test player
    extern Uint8 heldKeys[SDL_NUM_SCANCODES];
    extern Uint32 heldMouseKeys;
    void handleKeyEvents(SDL_Event* e);
    void handleMouseEvents(SDL_Event* e);
    Vector2 convertCameraPosToWorldPos(iVector2 pos);
    iVector2 convertWorldPosToCameraPos(Vector2 pos);
    int convertTileSizeToPixelSize(double value, Camera* camera=Main::camera);
    double convertPixelSizeToTileSize(int pixelSize, Camera* camera = Main::camera);
    void handleEvents(SDL_Event* e, void* ctx = nullptr);
    void updateTiles();
    void init();
    void initAllTextures();
    TextureInfo* loadTextureWithInfo(std::string texture);
    SDL_Rect getTextureFrame(int frameNumber, int totalFrames, TextureInfo* textureInfo, int startX = 0, int startY = 0);
    SDL_Rect getTextureFrame(int frameNumber,int totalFrames,SDL_Texture* texture,int textureWidth,int textureHeight, int startX = 0, int startY = 0);
    void setCursorType(cursorType type);
    void removeDeletedEntities();
    void spawnEntities();
    double toDegrees(double val);
    double toRadians(double val);
    double setSign(double src, double tgt);
    void updateLightMap(Vector2 position, int lightStrength);
    void resetLightMap();
    Vector2 rotatePt(Vector2 in, Vector2 around, double rotation);
    double getAngle(Vector2 pointA, Vector2 pointB);
    bool inWorldBounds(Vector2 pos);
    void renderPos(Vector2 pos);
    void drawSquare(Vector2 center, SDL_Color color, int width);
    ItemPickup* convertItemToItemPickup(Item* item, Vector2 position);
    Vector2 getIntersectPt(Line l1, Line l2);
    bool checkForTile(Vector2 pos);
    template <typename T>
    T* clone(T& obj) {
        return new T(obj);
    }
}


class Camera {
private:
    CornersRect checkCameraBoundsAt(Vector2 pos);
    double X = 0;
    SDL_Renderer* renderer;
    double Y = 0;
    double zoomScale = 16;
public:
    CornersRect cameraBounds = { {0,0},{0,0},{0,0},{0,0} };
    void blockPlacementPositionDraw(Vector2 pos);
    bool setX(double X, bool renderScrOnMove=false);
    void getPos(int* x, int* y);
    bool setY(double Y, bool renderScrOnMove=false);
    bool renderScreen(bool renderHitboxes = false);
    bool renderTiles();
    bool renderTileWalls();
    bool renderEntities();
    bool setPos(double X, double Y, bool renderScrOnMove=false);
    Camera(double X, double Y,SDL_Renderer* renderer ,bool renderScr=false);
    Camera(SDL_Renderer* renderer, bool renderScr=false);
    Camera();
    Vector2 getPos();
    bool setZoom(int scale, bool renderScrOnZoom=false);
    int zoom();
    void updateCameraBounds();
    void renderPlayer(Player* player);
    void renderPlayerGUI(Player* player);
};



