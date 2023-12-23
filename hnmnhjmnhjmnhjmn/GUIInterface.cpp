#include "gui.h"
#include "Main.h"
#include "Player.h"
#include "Item.h"
#include "Colliders.h"
#include "debug.h"
#include "Cursor.h"
#include "Tiles.h"
#include "Item_specialisedTypes.h"


GUIinterface::InventoryTile::InventoryTile(InventoryFrames type, SDL_Point onScreenPos, int size, std::shared_ptr<Item>& item,SpecialisedType itemType,int place,bool isHotbar) : type(type),onScreenPos(onScreenPos),size(size), itemType(itemType),item(item),isHotbar(isHotbar),place(place) {
	//why are you gae
	hitbox = new SquareHitbox(Main::convertCameraPosToWorldPos({ onScreenPos.x,onScreenPos.y }),Main::convertPixelSizeToTileSize(size), Main::convertPixelSizeToTileSize(size));
	this->texture = GUIinterface::inventoryTileTexture;
	this->outputRect = { onScreenPos.x,onScreenPos.y,size,size };
	this->update();
}

void GUIinterface::InventoryTile::renderTile() {
	this->inputRect = Main::getTextureFrame((int)type, 4, this->texture.get());
	SDL_RenderCopy(Main::renderer, texture->texture, &inputRect, &outputRect);
	if (this->item != nullptr) {
		SDL_Rect itemRect;
		float margin = float(size) / 8;
		itemRect.x = outputRect.x + margin; itemRect.y = outputRect.y += margin;
		itemRect.w = size -(2*margin); itemRect.h = size-(2*margin);
		this->item->renderTexture(&itemRect);
		if (this->item->count>1) {
			//puts number displaying item count in bottom right corner of tile
			int digits = std::to_string(this->item->count).size();
			itemRect.y = onScreenPos.y + (size * 0.6);
			itemRect.h = outputRect.h / 2;
			itemRect.w = outputRect.h * digits / 4;
			itemRect.x = onScreenPos.x + (outputRect.w * 0.9) - itemRect.w;
			SDL_RenderCopy(Main::renderer, this->item->itemTextCount.get(), NULL, &itemRect);
		}
	}
	if (Debug::renderHitboxes) this->hitbox->renderHitbox();
}


void GUIinterface::InventoryTile::renderTile(SDL_Color color_mod) {
	SDL_SetTextureColorMod(this->texture->texture, color_mod.r, color_mod.g, color_mod.b);
	InventoryTile::renderTile();
	SDL_SetTextureColorMod(this->texture->texture, 255, 255, 255);
}




GUIinterface::InventoryTile::~InventoryTile() {
	if (hitbox!=nullptr) delete hitbox;
	this->texture = nullptr;
}



void GUIinterface::renderPlayerHealh(Player* player) {
	int health = player->health;
	int size = Main::WINDOW_WIDTH * 0.02;
	int heartCount = player->maxHealth / 20;
	SDL_Rect startPos = { Main::WINDOW_WIDTH * 0.7,Main::WINDOW_HEIGHT * 0.03,size,size };
	SDL_Rect pos = startPos;
	int gap = size + (size / 5);
	for (int i = 0; i < heartCount; i++) {
		if (health <= 0) break;
		GUIPlayerHeart heart(health);
		health -= 20;
		heart.render(pos);
		if ((i+1)%10==0) {
			pos.y += gap;
			pos.x = startPos.x;
		}
		else pos.x += gap;
	}
	int w = size*4;
	
	int x = (heartCount > 10) ? (startPos.x + gap * 5) - (w / 2) : (startPos.x + gap * heartCount/2) - (w / 2);
	std::string str = "Life:" + std::to_string(player->health) + "/" + std::to_string(player->maxHealth);
	std::shared_ptr<SDL_Texture> text = GUIinterface::getTextPointer(str);
	pos = { x,0,w,startPos.y };
	SDL_RenderCopy(Main::renderer, text.get(), NULL, &pos);
}

void GUIinterface::renderPlayerInventory(Player* player) {
	if (player == nullptr) return;
	int place = 0;
	int scale = Main::WINDOW_WIDTH / 35;
	int gap = scale / 8;
	SDL_Point startPos = { Main::WINDOW_HEIGHT / 50, Main::WINDOW_HEIGHT / 30 };
	SDL_Point pos = startPos;
	SDL_Point endPos = { startPos.x + (player->inventoryColumns * (scale + gap)),startPos.y };
	SDL_Rect inputRect;
	SDL_Rect outputRect;
	for (int x = 0; x < player->inventoryRows; x++) {
		for (int y = 0; y < player->inventoryColumns; y++) {
			GUIinterface::InventoryTile* tile = nullptr;
			if (x==0 && y == player->selectedHBItem) {
				tile = new GUIinterface::InventoryTile(InventoryFrames::PLAYER_SELECTED_HOTBAR, pos, scale, Main::player->inventory[x][y],SpecialisedType::GENERIC,place);
				place++;
				if (tile->item != nullptr) {
					std::shared_ptr<SDL_Texture> text = GUIinterface::getTextPointer(tile->item->name);
					int textLetterCount = tile->item->name.size();
					SDL_Rect nameRect;
					nameRect.w = textLetterCount * Main::WINDOW_WIDTH / 120;
					nameRect.y = 0;
					nameRect.x = (endPos.x / 2) - (nameRect.w / 2);
					nameRect.h = endPos.y;
					SDL_RenderCopy(Main::renderer, text.get(), NULL, &nameRect);
				}
			}
			else tile = new GUIinterface::InventoryTile(InventoryFrames::PLAYER_INVENTORY, pos, scale, Main::player->inventory[x][y]);
			pos.x += scale + gap;
			tile->renderTile();
			delete tile;
		}
		pos.y += scale + gap;
		pos.x = startPos.x;
	}
	if (player->openedChest != nullptr) player->openedChest->renderInventory(pos);



}

void GUIinterface::renderPlayerHotbar(Player* player) {
	if (player == nullptr) return;
	int scale = Main::WINDOW_WIDTH / 35;
	int gap = scale/8;
	SDL_Point startPos = { Main::WINDOW_HEIGHT / 50, Main::WINDOW_HEIGHT/30 };
	SDL_Point pos = startPos;
	SDL_Point endPos = { startPos.x + (player->inventoryColumns * (scale + gap)),startPos.y };
	SDL_Rect inputRect;
	SDL_Rect outputRect;
	for (int i = 0; i < player->inventoryColumns; i++) {
		GUIinterface::InventoryTile* tile = nullptr;
		if (i == player->selectedHBItem) {
			tile = new GUIinterface::InventoryTile(InventoryFrames::PLAYER_SELECTED_HOTBAR, pos, scale, Main::player->inventory[0][i],SpecialisedType::GENERIC,i,true);
			if (tile->item != nullptr) {
				std::shared_ptr<SDL_Texture> text = GUIinterface::getTextPointer(tile->item->name);
				int textLetterCount = tile->item->name.size();
				SDL_Rect nameRect;
				nameRect.w = textLetterCount * Main::WINDOW_WIDTH/120;
				nameRect.y = 0;
				nameRect.x = (endPos.x / 2) - (nameRect.w / 2);
				nameRect.h = endPos.y;
				SDL_RenderCopy(Main::renderer, text.get(), NULL, &nameRect);
			}

		}
		else tile=new GUIinterface::InventoryTile(InventoryFrames::PLAYER_INVENTORY, pos, scale, Main::player->inventory[0][i],SpecialisedType::GENERIC,i,true);
		pos.x += scale + gap;
		tile->renderTile();
		delete tile;
	}

}


void GUIinterface::renderPlayerInvArmor(Player* player) {
	if (player == nullptr) return;
	int scale = Main::WINDOW_WIDTH / 35;
	int gap = scale / 8;
	SDL_Rect startPos;
	startPos.x = Main::WINDOW_WIDTH * 0.95 - scale - (3*gap);
	int count = (scale + gap) * (3+player->accessoryCount);
	startPos.y = Main::WINDOW_HEIGHT-count;
	startPos.w = scale;
	startPos.h = scale;
	
	for (int i=0; i<3; i++) {
		SpecialisedType type = (SpecialisedType)(i + 1);
		GUIinterface::InventoryTile tile(InventoryFrames::PLAYER_INVENTORY, { startPos.x,startPos.y }, startPos.w, player->armor[i],type);
		tile.renderTile({ 0,255,0 });
		startPos.y += scale + gap;
	}

	for (int i = 0; i < player->accessoryCount; i++) {
		GUIinterface::InventoryTile tile(InventoryFrames::PLAYER_INVENTORY, { startPos.x,startPos.y }, startPos.w, player->accessories[i], SpecialisedType::ACCESSORY);
		tile.renderTile({ 0,255,128 });
		startPos.y += scale + gap;
	}
}


void GUIinterface::renderPlayerMana(Player* player) {
	int mana = player->mana;
	int stars = player->maxMana / 20;
	int size= Main::WINDOW_WIDTH * 0.02;
	SDL_Rect startPos = { Main::WINDOW_WIDTH*0.95,Main::WINDOW_HEIGHT * 0.03,size,size};
	SDL_Rect pos = startPos;
	int gap = size + (size / 5);
	for (int i = 0; i < stars; i++) {
		if (mana <= 0) break;
		GUIinterface::GUIPlayerMana star(mana);
		star.render(pos);
		pos.y += gap;
		mana -= 20;
	}

	int x = startPos.x - (size*0.25);
	int w = size/0.75;
	pos = { x,0,w,startPos.y };
	SDL_RenderCopy(Main::renderer, GUIinterface::manaText.get(), NULL, &pos);
}

std::shared_ptr<SDL_Texture> GUIinterface::getTextPointer(std::string text, int size, SDL_Color color, std::string fontPath , int style) {
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
	SDL_Surface* s = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), color, 0);
	SDL_Texture* t = SDL_CreateTextureFromSurface(Main::renderer, s);
	SDL_FreeSurface(s);
	TTF_CloseFont(font);
	return std::shared_ptr<SDL_Texture>(t, [](SDL_Texture* ptr) {
		SDL_DestroyTexture(ptr);
		});
}


void GUIinterface::InventoryTile::update() {
	if (this->hitbox->collidesWith(Cursor::cursorWorldPos)) {
		Main::player->isCollidingWithGUI = true;
		if (Main::player->inventoryOpen) this->type = InventoryFrames::PLAYER_SELECTED_INVENTORY;

		if (Cursor::isLeftPressed()) {
			if (SDL_GetTicks() - Cursor::swapItemCooldown > 500 && !this->isHotbar) Cursor::swapItem(this);
			else if (this->isHotbar) {
				Main::player->selectedHBItem = this->place;
			}
		}
	}
}
