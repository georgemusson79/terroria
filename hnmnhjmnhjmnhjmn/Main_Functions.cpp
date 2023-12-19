#include "Main.h"
#include "Tiles.h"
#include "tileWalls.h"
#include "Tile.h"
#include "debug.h"
#include "Player.h"
#include "Colliders.h"
#include "Entities.h"
#include "Item_pickup.h"
#include "Cursor.h"
#include "Items.h"
#include <SDL.h>

Vector2 Main::convertCameraPosToWorldPos(iVector2 pos) {
    if (Main::camera != nullptr) {
        Vector2 topleft = Main::camera->cameraBounds.topLeft;
        double x = topleft.X + ((double)pos.X / Main::camera->zoom());
        double y = topleft.Y + ((double)pos.Y / Main::camera->zoom());
        return { x,y };
    }
}
iVector2 Main::convertWorldPosToCameraPos(Vector2 pos) {
	Vector2 topleft = Main::camera->cameraBounds.topLeft;
	int x = (pos.X - topleft.X) * Main::camera->zoom();
	int y = (pos.Y - topleft.Y) * Main::camera->zoom();
	return {x,y};
}

void Main::handleEvents(SDL_Event* e, void* ctx) {

    while (SDL_PollEvent(e)) {
        if (e->type == SDL_WINDOWEVENT) {
            if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                Main::WINDOW_HEIGHT = e->window.data2;
                Main::WINDOW_WIDTH = e->window.data1;

            }
        }
        if (e->type == SDL_QUIT) Debug::running = false;
        Main::handleKeyEvents(e);
        Main::handleMouseEvents(e);


    }
        




           
}

int Main::convertTileSizeToPixelSize(double value, Camera* camera) {
    return value * Main::camera->zoom();
}

double Main::convertPixelSizeToTileSize(int pixelValue,Camera* camera) {
    if (camera->zoom() == 0) return 0;
    return double(pixelValue) / camera->zoom();
}

void Main::updateTiles() {
    for (int x = 0; x < Main::WORLD_WIDTH; x++) {
        for (int y = 0; y < Main::WORLD_HEIGHT; y++) {
            Main::tiles[x][y]->update();
        }
    }
}

TextureInfo* Main::loadTextureWithInfo(std::string texture) {
    TextureInfo* info = new TextureInfo;
    info->texture = IMG_LoadTexture(Main::renderer, texture.c_str());
    if (info->texture!=NULL) SDL_QueryTexture(info->texture, NULL, NULL, &info->textureWidth, &info->textureHeight);
    return info;
}


SDL_Rect Main::getTextureFrame(int frameNumber, int totalFrames, TextureInfo* textureInfo,int startX, int startY) {
    return Main::getTextureFrame(frameNumber, totalFrames, textureInfo->texture, textureInfo->textureWidth, textureInfo->textureHeight);
}
SDL_Rect Main::getTextureFrame(int frameNumber, int totalFrames, SDL_Texture* texture, int textureWidth, int textureHeight, int startX, int startY) {
    int individualFrameWidth = textureWidth / totalFrames;
    int x = startX + (frameNumber * individualFrameWidth);
    return { x,startY,individualFrameWidth,textureHeight };
}

//handles key and mouse events not tied to player
void Main::handleMouseEvents(SDL_Event* e) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        auto pos = Cursor::cursorWorldPos;

        if (e->button.button == SDL_BUTTON_LEFT) {}
        if (e->button.button == SDL_BUTTON_RIGHT) {}
    }

}
   

void Main::handleKeyEvents(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        if (Main::player != nullptr) {

            if (e->key.keysym.sym == SDLK_b) {
                Item1* bob=new Item1;
                new ItemPickup(bob, Main::player->position);
            }

            //else if (e->key.keysym.sym == SDLK_q) Main::player->dropItem(Main::player->inventory[0][Main::player->selectedHBItem]);
            else if (e->key.keysym.sym == SDLK_F11) {
                if (Debug::fullscreen) SDL_SetWindowFullscreen(Main::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                else SDL_SetWindowFullscreen(Main::window,0);
                Debug::fullscreen = !Debug::fullscreen;
            }
            else if (e->key.keysym.sym == SDLK_LCTRL) {
                if (Main::isSmartCursorEnabled) {
                    Main::setCursorType(cursorType::MANUAL);
                    Main::isSmartCursorEnabled = false;
                }
                else {
                    Main::setCursorType(cursorType::SMART);
                    Main::isSmartCursorEnabled = true;
                }
            }

        }
    }
}

void Main::setCursorType(cursorType type) {
    SDL_FreeCursor(Main::cursor);
    SDL_Surface* s;
    s = (type == cursorType::SMART) ? IMG_Load("assets\\cursor\\smart_cursor.png") : IMG_Load("assets\\cursor\\manual_cursor.png");
    Main::cursor=SDL_CreateColorCursor(s, 0, 0);
    SDL_SetCursor(Main::cursor);
    SDL_FreeSurface(s);
}

void Main::removeDeletedEntities() {
    std::vector<Entity*> deleteEntities = {};
    for (auto entity : Main::entities) {
        if (entity->markForDeletion) deleteEntities.push_back(entity);
    }
    for (auto entity : deleteEntities) {
        auto it=std::find(Main::entities.begin(), Main::entities.end(), entity);
        if (it != Main::entities.end()) Main::entities.erase(it);
    }
}


double Main::toDegrees(double val) {
    return val * (180 / M_PI);
}
double Main::toRadians(double val) {
    return val * (M_PI / 180);
}

double Main::setSign(double src, double tgt) {
    return (src < 0) ? -abs(tgt) : abs(tgt);
}


void Main::updateLightMap(Vector2 position, int lightStrength) {
    //TODO: make light render only on screen
    double maxDistance = sqrt(lightStrength);
    Vector2 topLeft = { position.X - maxDistance,position.Y - maxDistance };
    for (int x = topLeft.X; x < position.X + maxDistance; x++) {
        for (int y = topLeft.Y; y < position.Y + maxDistance; y++) {
            double distance = position.distance({ x,y });
            if (x > 0 && y > 0 && x <= Main::WORLD_WIDTH && y <= Main::WORLD_HEIGHT && distance<maxDistance) {
                distance = (distance == 0) ? 1 : distance;
                double strength = lightStrength / std::pow(distance, 2);
                if (strength > Main::lightMap[x][y]) {
                    Main::lightMap[x][y] = strength;
                    Main::changedLights.push_back({x,y});
                }
            }
        }
    }
}

void Main::resetLightMap() {
    for (auto point : Main::changedLights) Main::lightMap[point.X][point.Y] = Main::defaultLightValue;
    Main::changedLights = {};
}

bool Main::inWorldBounds(Vector2 pos) {
    if (pos >= Vector2(0, 0) && pos.X <= Main::WORLD_WIDTH && pos.Y <= Main::WORLD_HEIGHT) return true;
    return false;
}


void Main::renderPos(Vector2 pos) {
    SDL_SetRenderDrawColor(Main::renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(Main::renderer, pos.X - 10, pos.Y - 10, pos.X + 10, pos.Y + 10);
    SDL_SetRenderDrawColor(Main::renderer, 0, 0, 0, 0);
}


void Main::drawSquare(Vector2 center, SDL_Color color, int width) {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
    SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(Main::renderer, color.r, color.g, color.b, 255);
    center = Main::convertWorldPosToCameraPos(center).convertToVector2();
    SDL_Rect rect = { center.X - (width / 2),center.Y - (width / 2),width,width };
    SDL_RenderFillRect(Main::renderer, &rect);
    SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
}

Vector2 Main::getIntersectPt(Line l1, Line l2) {
    double x1 = l1.start.X;
    double y1 = l1.start.Y;
    double x2 = l1.end.X;
    double y2 = l1.end.Y;

    double x3 = l2.start.X;
    double y3 = l2.start.Y;
    double x4 = l2.end.X;
    double y4 = l2.end.Y;

    double denomiator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (denomiator == 0) {
        return { std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    }

    double intersectX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denomiator;
    double intersectY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denomiator;

    return { intersectX, intersectY };
}

Vector2 Main::rotatePt(Vector2 in, Vector2 around, double rotation) {
    Vector2 org = in - around;
    double rad = Main::toRadians(rotation);
    double matrix[2][2];
    matrix[0][0] = cos(rad);
    matrix[0][1] = -sin(rad);
    matrix[1][0] = sin(rad);
    matrix[1][1] = cos(rad);
    Vector2 res = { matrix[0][0] * org.X + matrix[0][1] * org.Y,matrix[1][0] * org.X + matrix[1][1] * org.Y };
    res += around;
    return res;
}