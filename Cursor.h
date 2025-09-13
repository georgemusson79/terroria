#pragma once
#include "Vector2.h"
#include "gui.h"

class Item;


namespace Cursor {
    extern Vector2 cursorWorldPos;
    extern GUIinterface::InventoryTile* selectedInvTile;
    Vector2 WorldPos();
    bool swapItem(GUIinterface::InventoryTile* invTile);
    iVector2 ScreenPos();
    int selectedTile();
    void update();
    bool dropItem(Player* player);
    extern bool isHoldingItem;
    bool isLeftPressed();
    bool isRightPressed();
    extern int swapItemCooldown;
    extern std::shared_ptr<Item> item;

};