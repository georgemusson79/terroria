#include "game_datatypes.h"
#include <SDL.h>
TextureInfo::~TextureInfo() {
	if (this->texture!=nullptr) SDL_DestroyTexture(texture);
}