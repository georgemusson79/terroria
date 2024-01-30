#include "Bossbar.h"
#include "gui.h"
#include "Main.h"
void BossBar::setText(std::string text,SDL_Color color) {
	this->displayText = text;
	this->displayTextTexture=GUIinterface::getTextPointer(text, 60, color);
}

BossBar::BossBar(int health, std::string text, SDL_Rect renderDims,std::string pathToBossHead,SDL_Color textColor={255,255,255}) {
	this->maxHealth = health;
	this->health = health;
	this->setText(text, textColor);
	this->setBossHeadTexture(pathToBossHead);
}

std::string BossBar::getText() {
	return this->displayText;
}

void BossBar::setBossHeadTexture(std::string path) {
	if (path == "") this->bossHeadTexture.reset();
	else this->bossHeadTexture = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(Main::renderer, path.c_str()));
}