#include "Bossbar.h"
#include "gui.h"
#include "Main.h"
#include "Entity.h"
void BossBar::setText(std::string text,SDL_Color color) {
	this->displayText = text;
	this->displayTextTexture=GUIinterface::getTextPointer(text, 60, color);
}

BossBar::BossBar(int health, std::string text, std::string pathToBossHead, Entity* src, std::string pathToBossHealthBar, std::string pathToBossBarFrame, SDL_Color textColor) {
	this->maxHealth = health;
	this->health = health;
	this->setText(text, textColor);
	this->setBossHeadTexture(pathToBossHead);
	this->setHealthTexture(pathToBossHealthBar);
	this->setBossBarFrameTexture(pathToBossBarFrame);
	this->src = src;
}

std::string BossBar::getText() {
	return this->displayText;
}

void BossBar::setBossHeadTexture(std::string path) {
	if (path == "") this->bossHeadTexture.reset();
	else this->bossHeadTexture = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(Main::renderer, path.c_str()),SDL_DestroyTexture);
}

void BossBar::setBossBarFrameTexture(std::string path) {
	if (path == "") this->bossBarFrame.reset();
	else this->bossBarFrame = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(Main::renderer, path.c_str()), SDL_DestroyTexture);
}

void BossBar::setHealthTexture(std::string path) {
	this->healthTexture = std::shared_ptr<TextureInfo>(Main::loadTextureWithInfo(path));
}

void BossBar::render() {
	if (Main::doesBossBarExist) return;
	Main::doesBossBarExist = true;
	if (this->src != nullptr) {
		this->health = src->health;
		this->maxHealth = src->maxHealth;
	}

	Vector2 screenCenter = { Main::WINDOW_WIDTH / 2,Main::WINDOW_HEIGHT / 2 };
	int bossBarWidth = Main::WINDOW_WIDTH/1.6;
	SDL_Rect renderBarDims = {screenCenter.X - bossBarWidth / 2,Main::WINDOW_HEIGHT * 0.85,bossBarWidth,Main::WINDOW_HEIGHT / 13 };
	SDL_RenderCopy(Main::renderer, this->bossBarFrame.get(), NULL, &renderBarDims);

	SDL_Rect healthRectSrc = { 0,0,this->healthTexture->textureWidth * ((double)this->health / this->maxHealth),this->healthTexture->textureHeight };
	SDL_Rect healthRectDst = { Main::WINDOW_WIDTH * 0.227083,Main::WINDOW_HEIGHT * 0.875926,Main::WINDOW_WIDTH * 0.553125,Main::WINDOW_HEIGHT * 0.029630 };
	healthRectDst.w *= (double)this->health / this->maxHealth;
	SDL_RenderCopy(Main::renderer, this->healthTexture->texture, &healthRectSrc, &healthRectDst);

	SDL_Rect headPos = { Main::WINDOW_WIDTH * 0.190542, Main::WINDOW_HEIGHT * 0.870370, Main::WINDOW_WIDTH * 0.036292,Main::WINDOW_HEIGHT * 0.04333 };
	SDL_RenderCopy(Main::renderer, this->bossHeadTexture.get(), NULL, &headPos);
}
