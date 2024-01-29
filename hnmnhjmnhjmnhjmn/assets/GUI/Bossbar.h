#include <iostream>
#include <SDL.h>
class BossBar {
protected:
	std::string displayText;
	SDL_Texture* dispalyTextTexture;
	SDL_Texture* healthTexture;
	SDL_Texture* bossBarFrame;
	SDL_Rect renderDims;
public:
	int maxHealth;
	int health;
	std::string getText();
	void setText(std::string text);
	BossBar(int health, std::string text,SDL_Rect renderDims);
	void render();
	~BossBar();
};