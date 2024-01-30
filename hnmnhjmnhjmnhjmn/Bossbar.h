#include <iostream>
#include <SDL.h>
class TextureInfo;
class Entity;
class BossBar {
protected:
	std::string displayText;
	std::shared_ptr<SDL_Texture> displayTextTexture;
	std::shared_ptr<TextureInfo> healthTexture;
	std::shared_ptr<SDL_Texture> bossBarFrame;
	std::shared_ptr<SDL_Texture> bossHeadTexture;
public:
	int maxHealth;
	Entity* src;
	int health;
	std::string getText();
	void setBossHeadTexture(std::string path);
	void setBossBarFrameTexture(std::string path);
	void setText(std::string text, SDL_Color color={255,255,255});
	BossBar(int health, std::string text, std::string pathToBossHead, Entity* src = nullptr, std::string pathToBossHealthBar = "assets\\GUI\\bossBar_health.png", std::string pathToBossBarFrame = "assets\\GUI\\bossBar.png", SDL_Color textColor = { 255,255,255 });
	virtual void render();
	void setHealthTexture(std::string path);
};