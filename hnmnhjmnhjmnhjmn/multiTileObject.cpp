//multtileobject.cpp
#include "Tiles.h"
#include "Main.h"

MultiTileObject::MultiTileObject(uint16_t id,uint16_t X, uint16_t Y, uint16_t W, uint16_t H,int health,bool isParent, MultiTileObject* parent) : Tile(id,X,Y,health) {
    this->width = W;
    this->translucent = true;
    this->placeIntoWorld = placeIntoWorld;
    this->height = H;
    this->isParent = isParent;
    if (!this->isParent) this->parent = parent;
    this->isMultiTile = true;
}

bool MultiTileObject::draw(SDL_Renderer* renderer, Camera& camera) {
    if (this->isParent) {
        if (tileID != AIR) {
            CornersRect bounds = camera.cameraBounds;
            Vector2 topLeft = bounds.topLeft;
            int zoom = camera.zoom();
            double newX = (this->X - topLeft.X) * zoom;
            double newY = (this->Y - topLeft.Y) * zoom;
            newX = std::round(newX);
            newY = std::round(newY);
            SDL_Rect dstrect = { newX,newY,camera.zoom() * this->width,camera.zoom() * this->height };
            if ((dstrect.x < Main::WINDOW_WIDTH && dstrect.x + dstrect.w>0) && (dstrect.y < Main::WINDOW_HEIGHT && dstrect.y + dstrect.h>0)) {
                double brightness = ((double)Main::lightMap[this->X][this->Y] / 15) * 255;
                if (brightness > 255) brightness = 255;
                SDL_SetTextureColorMod(this->getTexture(), brightness, brightness, brightness);
                SDL_RenderCopy(renderer, this->getTexture(), NULL, &dstrect);
                return true;
            }
        }
    }

    else return this->parent->draw(renderer,camera);
    return false;
}

void MultiTileObject::destroy(bool dropItem) {
    if (this->isParent) {
        for (Tile* tile : this->tileList) {
            if (Main::inWorldBounds({ tile->X,tile->Y })) {
                Vector2 pos = { tile->X,tile->Y };
                Main::tiles[pos.X][pos.Y] = nullptr;
                tile->markedForDeletion = true;
                Main::tilesToDelete.insert(tile);
            }
            
        }
        Main::tiles[this->X][this->Y] = nullptr;
        if (dropItem) this->dropItem();
        Main::tilesToDelete.insert(this);
        return;
        

        
    }
    else this->parent->destroy();
}
void MultiTileObject::generate() {
    if (placeIntoWorld) {
        bool inRange = (this->X >= 0 && this->Y >= 0 && this->Y + this->height < Main::tiles[0].size() && this->X + this->width < Main::tiles.size());
        assert(inRange);
        if (Main::tiles[X][Y] != nullptr) {
            Main::tiles[X][Y]->destroy();
        }
        Main::tiles[X][Y] = this;

        if (this->lightStrength > 0) this->updateLightMap();
    }
}


void MultiTileObject::update() {
    if (parent != nullptr) parent->update();
}

void MultiTileObject::onRightClick(Player* player) {
    if (parent != nullptr) parent->onRightClick(player);
}

void MultiTileObject::onLeftClick(Player* player) {
    if (parent != nullptr) parent->onLeftClick(player);
}


void MultiTileObject::create() {
    this->placeIntoWorld = true;
    if (isParent) {
        this->generate();
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (x != 0 || y != 0) {
                    MultiTileObject* cpy = new MultiTileObject(*this);
                    cpy->isParent = false;
                    cpy->parent = this;
                    cpy->X += x;
                    cpy->Y += y;
                    cpy->generate();
                    this->tileList.push_back(Main::tiles[X + x][Y + y]);
                }
            }
        }
    }
}

bool MultiTileObject::mine(int pickaxePower, int axePower, int hammerPower,bool dropItem) {
    if (this->parent!=nullptr) {
   
        return this->parent->mine(pickaxePower, axePower, hammerPower,dropItem);
    }
    else return Tile::mine(pickaxePower, axePower, hammerPower,dropItem);
}













