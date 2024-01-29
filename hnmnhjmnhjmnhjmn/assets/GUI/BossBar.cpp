#include "BossBar.h"

BossBar::BossBar(int health, std::string text, SDL_Rect renderDims) {
	this->maxHealth = health;
	this->displayText = text;
}