#include "Main.h"
#include "Tile.h"
#include "Cursor.h"
#include "gui.h"
#include "Colliders.h"
#include "Item.h"
#include "Player.h"
#include "Vector2.h"
#include "Item_pickup.h"

namespace Cursor {
	Vector2 cursorWorldPos;
	std::shared_ptr<Item> item = nullptr;
	GUIinterface::InventoryTile* selectedInvTile = nullptr;
	bool isHoldingItem = false;
	int swapItemCooldown = 0;
}

iVector2 Cursor::ScreenPos() {
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	return { x,y };
}

Vector2 Cursor::WorldPos() {
	return Main::convertCameraPosToWorldPos(Cursor::ScreenPos());
}

int Cursor::selectedTile() {
	Vector2 pos = Cursor::WorldPos();
	return Main::tiles[pos.X][pos.Y]->getID();
}

bool Cursor::swapItem(GUIinterface::InventoryTile* invTile) {
	if (invTile == nullptr && Cursor::item == nullptr) return false;
	else if (invTile->itemType != SpecialisedType::GENERIC && Cursor::isHoldingItem && Cursor::item->itemType != invTile->itemType) return false;



	if (Cursor::isHoldingItem && invTile->item!=nullptr && Cursor::item->id == invTile->item->id && invTile->item->count + Cursor::item->count <= invTile->item->maxStack) {
		invTile->item->setCount(invTile->item->count + Cursor::item->count);
		Cursor::item.reset();
	}
	else {
		std::shared_ptr<Item> temp = Cursor::item;
		Cursor::item = invTile->item;
		invTile->item = temp;
	}
	Cursor::isHoldingItem = (Cursor::item != nullptr) ? true : false;
	Cursor::swapItemCooldown = SDL_GetTicks();
	return true;
}


void Cursor::update() {
	Cursor::cursorWorldPos = Cursor::WorldPos();
}

bool Cursor::isLeftPressed() {
	int x;
	if (SDL_GetMouseState(&x, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) return true;
	return false;
}
bool Cursor::isRightPressed() {
	int x;
	if (SDL_GetMouseState(&x, NULL) && SDL_BUTTON(SDL_BUTTON_RIGHT)) return true;
	return false;
}

bool Cursor::dropItem(Player* player) {
	if (Cursor::isHoldingItem) {
		double length = Cursor::WorldPos().distance(player->position);
		Vector2 normalised = (Cursor::WorldPos() - player->position) / Vector2(length, length);
		Vector2 velocity = normalised * Vector2(0.3, -0.3);
		ItemPickup* pickup=new ItemPickup(Cursor::item,player->position);
		pickup->velocity = velocity;
		Cursor::item.reset();
		Cursor::isHoldingItem = false;
		return true;
	}
	return false;
}