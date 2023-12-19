//main.cpp
#define SDL_MAIN_HANDLED
#include "Main.h"
#include "Player.h"
#include "Entities.h"
#include "Projectiles.h"
#include "Tile.h"
#include "Tiles.h"
#include "gui.h"
#include "debug.h"
#include "tileWall.h"
#include "tileWalls.h"
#include "Colliders.h"
#include "Items.h"
#include "Item.h"
#include "Cursor.h"
CircleHitbox* Main::testcbox = new CircleHitbox;
int Main::frames = 0;
int Main::WINDOW_WIDTH = 1920/2;
double Main::gravity = 9;
int Main::WINDOW_HEIGHT = 1080/2;
bool Main::scaleToWindowSize = false;
uint16_t Main::WORLD_HEIGHT = 1500;
uint16_t Main::WORLD_WIDTH = 7000;
std::vector<Entity*> Main::entities = {};
std::map<int,SDL_Texture*> Main::tileTexture;
std::map<int, SDL_Texture*> Main::tileWallTexture;
std::map<int, SDL_Texture*> Main::backgrounds;
std::vector<std::vector<Tile*>> Main::tiles = std::vector<std::vector<Tile*>>(Main::WORLD_WIDTH+1, std::vector<Tile*>(Main::WORLD_HEIGHT+1));
std::vector<std::vector<TileWall*>> Main::tileWalls = std::vector<std::vector<TileWall*>>(Main::WORLD_WIDTH + 1, std::vector<TileWall*>(Main::WORLD_HEIGHT + 1));


uint16_t Main::defaultLightValue = 0;
std::vector<std::vector<uint16_t>> Main::lightMap(Main::WORLD_WIDTH + 1, std::vector<uint16_t>(Main::WORLD_HEIGHT+1,Main::defaultLightValue));
std::vector<iVector2> Main::changedLights;



SDL_Window* Main::window;
SDL_Renderer* Main::renderer;
SDL_Renderer* r2;
SDL_Cursor* Main::cursor;
bool Main::isSmartCursorEnabled = false;
Camera* Main::camera = nullptr;
Player* Main::player = nullptr;
Player* Main::p2 = nullptr;
void gotofirstdirt() {
    for (int x = 0; x < Main::WORLD_WIDTH; x++) {
        for (int y = 0; y < Main::WORLD_HEIGHT; y++) if (Main::tiles[x][y]->getID() == DIRT) {
            Main::player = new Player({ (double)x+100, (double)y });
            return;
        }
    }
}

void loadTileTexturesToMap() {

}
void sinTiles() {
    int prevY = NULL;
    int y = 0;
    for (int x = 0; x < Main::WORLD_WIDTH; x++) {
        double radX = x * (M_PI / 180);
        y = abs(sin(radX) * (Main::WORLD_HEIGHT / 4)) + 1000;
        if (y > 0 && y < Main::WINDOW_HEIGHT) {
            if (prevY != NULL) {
                int mdy = abs(prevY - y);
                for (int dy = prevY; dy < mdy + prevY; dy++) {
                    if (dy > 0 && dy < Main::WORLD_HEIGHT)
                        Dirt(x, dy);
                  /*      for (dy + 4; y < Main::WORLD_HEIGHT; y++) {
                            if (y < prevY + 20) Dirt(x, y);
                            else Stone(x, y);
                        }*/
                }
            }
            prevY = y;
        }
 
    }
}
template <typename TileInstance, typename TileWallInstance>
void temp_populateHalfTiles(TileInstance tile, TileWallInstance tilewall) {
    static_assert(std::is_base_of_v<Tile, TileInstance>, "input is not of type tile");
    static_assert(std::is_base_of_v<TileWall, TileWallInstance>, "input is not of type tilewall");
    for (int y = Main::WORLD_HEIGHT/2; y <= Main::WORLD_HEIGHT; y++) {
        for (int x = 0; x <= Main::WORLD_WIDTH; x++) {
            TileWallInstance(x, y);
            TileInstance(x, y);
        }
    }
}

template <typename TileInstance,typename TileWallInstance>
void temp_populateAllTiles(TileInstance tile,TileWallInstance tilewall) {
    static_assert(std::is_base_of_v<Tile, TileInstance>, "input is not of type tile");
    static_assert(std::is_base_of_v<TileWall, TileWallInstance>, "input is not of type tilewall");
    for (int y = 0; y < Main::WORLD_HEIGHT; y++) {
        for (int x = 0; x < Main::WORLD_WIDTH; x++) {
            TileWallInstance(x, y);
            TileInstance(x, y);
        }
    }
}
int main() {
    //this must happen first
    Main::init();

    temp_populateHalfTiles(Dirt(2,2,false),WoodWall(2,2,false));
    Main::camera=new Camera(1000,1000,Main::renderer);
    Main::player = new Player({ 500,Main::WORLD_HEIGHT / 2 });
    Main::player->setY(Main::player->position.Y - 10);
    Main::p2 = new Player({ 500,Main::WORLD_HEIGHT/2 });
    SDL_Event e;
   
    Main::testcbox->position = Main::player->position;
    Main::testcbox->r = 10;
    Main::testcbox->active = false;
    int starttime = SDL_GetTicks();
    int frmstart = SDL_GetTicks();
    Wood(Main::player->position.X, Main::player->position.Y -100);
    bool increasing = true;
    //Entity1* en=new Entity1({ Main::player->position.X + 10, Main::player->position.Y });
    //Entity1* en2=new Entity1(en->position);
    //ItemSwing* obj = new ItemSwing(Main::player->position, 10, 10, "assets\\Items\\sword.png");
    while (Debug::running) {
        Main::resetLightMap();

        
        Cursor::update();
        Main::handleEvents(&e, nullptr);
        if (Main::player != nullptr) {
      
            Main::player->update();
            //Main::player->selectedHBItem++;
            //if (Main::player->selectedHBItem == 10) Main::player->selectedHBItem = 0;
        }
        Main::testcbox->r+=0.01;
        for (int e = 0; e < Main::entities.size(); e++) {
            if (Main::entities[e] != nullptr && Main::entities[e]->markForDeletion == false) Main::entities[e]->update();
        }
        for (Tile* t : Main::testcbox->collidesWithTiles()) t->destroy();
        Main::removeDeletedEntities();
        Main::camera->renderScreen();
        //handle fps: should be put in function and needs work

        if (SDL_GetTicks() - frmstart < 1000 / 60) {
            
            //while (SDL_GetTicks() - frmstart < 1000 / 60 && Main::vsync) {}
            frmstart = SDL_GetTicks();
            
        }
        else {
            frmstart = SDL_GetTicks();
        }
        Main::frames++;
        if (SDL_GetTicks() - starttime > 1000) {
            std::cout << Main::frames << "\n";
            Main::frames = 0;
            starttime = SDL_GetTicks();
        }
    }
}
