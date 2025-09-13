//guiheart.cpp
#include "gui.h"
#include "Main.h"
#include "Player.h"
#include "Colliders.h"


GUIinterface::GUIPlayerHeart::GUIPlayerHeart(int health) : health(health), texture(GUIinterface::heartTexture) {
}


void GUIinterface::GUIPlayerHeart::render(SDL_Rect pos) {
	if (health < maxHealth) {
		int a = ((double)health / maxHealth) * 255;
		SDL_SetTextureAlphaMod(this->texture, a);
	}
	SDL_RenderCopy(Main::renderer, this->texture, NULL, &pos);
	SDL_SetTextureAlphaMod(this->texture, 255);
}

GUIinterface::GUIPlayerMana::GUIPlayerMana(int health) {
	this->health = health;
	this->texture = GUIinterface::manaTexture;
}