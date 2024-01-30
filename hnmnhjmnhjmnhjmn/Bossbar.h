#include <iostream>
#include <SDL.h>
class BossBar {
protected:
	std::string displayText;
	std::shared_ptr<SDL_Texture> displayTextTexture;
	std::shared_ptr<SDL_Texture> healthTexture;
	std::shared_ptr<SDL_Texture> bossBarFrame;
	SDL_Rect renderDims;
	std::shared_ptr<SDL_Texture> bossHeadTexture;
public:
	int maxHealth;
	int health;
	std::string getText();
	void setBossHeadTexture(std::string path);
	virtual void setText(std::string text, SDL_Color color={255,255,255});
	BossBar(int health, std::string text,SDL_Rect renderDims, std::string pathToBossHead, SDL_Color textColor = { 255,255,255 });
	virtual void render();
	~BossBar();
};