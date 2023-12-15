//initialiser function
#include "Main.h"
#include "Tiles.h"
#include "tileWalls.h"
#include "gui.h"
#include "debug.h"
namespace Main {
    Uint8 heldKeys[SDL_NUM_SCANCODES] = {};
    Uint32 heldMouseKeys = NULL;
}
namespace GUIinterface {
    std::shared_ptr<TextureInfo> inventoryTileTexture;
    SDL_Texture* heartTexture;
    SDL_Texture* manaTexture;
    std::shared_ptr<SDL_Texture> manaText;
}


void Main::init() {
    srand(time(0));
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    memcpy(heldKeys, SDL_GetKeyboardState(nullptr), SDL_NUM_SCANCODES);
    Main::setCursorType(cursorType::MANUAL);
    Main::window = SDL_CreateWindow("Terroria", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Main::WINDOW_WIDTH, Main::WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (Debug::vsync) Main::renderer=SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    else {
        std::cout << "vsync false\n";
        Main::renderer = SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED);
    }
    SDL_SetRenderDrawBlendMode(Main::renderer, SDL_BLENDMODE_BLEND);
    Main::initAllTextures();
}

void Main::initAllTextures() {
    //some textures are pre emptively initialised mostly for performance reasons
    Main::tileWallTexture.insert({ WOODWALL,IMG_LoadTexture(Main::renderer,"assets\\tiles\\wood.png") });
    Main::tileTexture.insert({ DIRT,IMG_LoadTexture(Main::renderer,"assets\\tiles\\dirt.png") });
    Main::tileTexture.insert({ STONE,IMG_LoadTexture(Main::renderer,"assets\\tiles\\stone.png") });
    Main::tileTexture.insert({ WOOD,IMG_LoadTexture(Main::renderer,"assets\\tiles\\wood.png") });
    Main::tileTexture.insert({ HCRYSTAL , IMG_LoadTexture(Main::renderer, "assets\\tiles\\heart.png") });
    Main::backgrounds.insert({ DEFAULT,IMG_LoadTexture(Main::renderer,"assets\\tiles\\sky.png") });
    GUIinterface::inventoryTileTexture = std::shared_ptr<TextureInfo>(Main::loadTextureWithInfo("assets\\GUI\\InventoryBackgrounds.png"));
    GUIinterface::heartTexture = IMG_LoadTexture(Main::renderer, "assets\\GUI\\heart.png");
    GUIinterface::manaTexture = IMG_LoadTexture(Main::renderer, "assets\\GUI\\mana.png");
    GUIinterface::manaText = GUIinterface::getTextPointer("Mana");
}
