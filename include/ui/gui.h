#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include "game_datatypes.h"
#include "Item_specialisedTypes.h"
class Player;
class SquareHitbox;
class Item;


enum class InventoryFrames {
	CHEST_INTERFACE, PLAYER_INVENTORY, PLAYER_SELECTED_INVENTORY, PLAYER_SELECTED_HOTBAR
};
namespace GUIinterface {
	extern std::shared_ptr<TextureInfo> inventoryTileTexture;
	extern SDL_Texture* heartTexture;
	extern SDL_Texture* manaTexture;
	extern std::shared_ptr<SDL_Texture> manaText;

	class GUIPlayerHeart {
	public:
		SDL_Texture* texture = nullptr;
		int maxHealth = 20;
		int health = 10;
		GUIPlayerHeart(int health);
		GUIPlayerHeart() {}
		void render(SDL_Rect r);

	};

	class GUIPlayerMana : public GUIPlayerHeart {
	public:
		GUIPlayerMana(int health);
	};


	class InventoryTile {
	public:
		int place = 0;
		int size;
		bool isHotbar = false;
		SpecialisedType itemType = SpecialisedType::GENERIC;
		std::shared_ptr<TextureInfo> texture = nullptr;
		SDL_Texture* itemCountTexture = nullptr;
		SDL_Point onScreenPos;
		InventoryFrames type;
		SquareHitbox* hitbox = nullptr;
		std::shared_ptr<Item>& item;
		InventoryTile(InventoryFrames type,SDL_Point onScreenPos,int size, std::shared_ptr<Item>& item,SpecialisedType itemType=SpecialisedType::GENERIC, int place=0, bool isHotbar=false);
		void renderTile();
		void renderTile(SDL_Color color_mod);
		void update();
		~InventoryTile();
	private:
		SDL_Rect inputRect;
		SDL_Rect outputRect;
	};

	std::shared_ptr<SDL_Texture> getTextPointer(std::string text, int size = 60, SDL_Color color = { 255,255,255,255 }, std::string fontPath = "assets\\fonts\\ANDYB.TTF",int style=TTF_STYLE_NORMAL);
	void renderPlayerInventory(Player* player);
	void renderPlayerInvArmor(Player* player);
	void renderPlayerHotbar(Player* player);
	void renderPlayerHealh(Player* player);
	void renderPlayerMana(Player* player);
}
