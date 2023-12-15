//tile.cpp
#include "Main.h"
Tile::Tile(int tileID, uint16_t X, uint16_t Y) {
    this->tileID = tileID;
    this->X = X;
    this->Y = Y;
}
Tile::Tile() {
    this->X = 0;
    this->Y = 0;
    this->tileID = AIR;
}
void Tile::draw(SDL_Renderer* renderer) {
    if (tileID != AIR) {
        SDL_Rect dstrect = { X,Y,10,10 };
        SDL_RenderCopy(renderer, Main::tileTexture[tileID], NULL, &dstrect);
    }
}