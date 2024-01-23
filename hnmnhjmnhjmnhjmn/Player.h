#pragma once
#include <array>
//player.h
#include "Projectiles.h"
#include "Main.h"
#include "Entity.h"
#include "Items.h"
#include <functional>
#include <memory>

enum class PlayerSprite {
     WALK1, IDLE, WALK2
};
class Chest;
class Arm;
class Player : public Entity {
private:
    //Item heldItem;
    //ItemSwing* swingItem = nullptr;
    int tempRotation = 0;
    bool useItemAnim = false;
    bool animationOverride = false;
    int walkFrame = 0;
    bool handleWalking();
    SDL_Texture* armTexture = nullptr;
    double armRotation = 0;
    Vector2 armPos = { 0,0 };
    Vector2 armRotationPt = { 0,0 };
    Vector2 armDims = { 0,0 };

public:
    Arm* arm = nullptr;
    //void swingAnim(Item* item);
    bool usingItem = false;
    bool isWalking = false;
    bool isCollidingWithGUI = false;
    Chest* openedChest = nullptr;
    double deceleration = 0.03;
    bool renderInventory = false;
    bool inventoryOpen = false;
    double jumpVelocity = 0.5;
    int inventoryColumns = 10;
    int placeRng = 6;
    int inventoryRows = 5;
    int selectedHBItem = 0;
    int accessoryCount = 6;
    int selectedInventoryItem = -1;
    int temp_cooldown = 0;
    int mana =100;
    int maxMana = 100;
    //static void blockSelectDraw(SDL_Renderer* renderer, SDL_Rect r);
    std::array<std::shared_ptr<Item>, 3> armor = {nullptr,nullptr,nullptr};
    std::vector<std::shared_ptr<Item>> accessories = std::vector<std::shared_ptr<Item>>(accessoryCount, nullptr);
    std::vector<std::vector<std::shared_ptr<Item>>> inventory;
    int animationTimePassed = 0;
    bool getCursorDistance();
    Player(Vector2 pos);
    Player();
    bool has(int itemID,Vector2* itemPos);
    ~Player();
    void update() override;
    void placeBlock(Tile* t);
    //bool renderEntity() override;
    void clearInventory();
    template <typename ItemType>
    bool pickup(ItemType* item, int row, int column, bool replace = false) {
        static_assert(std::is_base_of_v<Item, ItemType>, "item is not of type Item");
        ItemType cpy = *item;
        if (row < this->inventoryRows && column < this->inventoryColumns) {
            if (replace || inventory[row][column]==nullptr) {
                inventory[row][column] = std::shared_ptr<Item>(new ItemType(cpy));
                return true;
            }
            else {
                
                if (item->id== inventory[row][column]->id && item->isStackable && (inventory[row][column]->count+item->count)<=item->maxStack) {
                    inventory[row][column]->setCount(inventory[row][column]->count + item->count);
                    return true;
                }
            }
        }
        return false;
    }
    bool has(AmmoType ammoID, Vector2* pos);
    bool pickup(std::shared_ptr<Item>);
    bool pickup(Item* item) {
        for (int x = 0; x < inventoryRows; x++) {
            for (int y = 0; y < inventoryColumns; y++) {
                if (inventory[x][y] == nullptr) {
                    inventory[x][y] = std::shared_ptr<Item>(item);
                    return true;
                }
                else if (inventory[x][y]->id == item->id && (inventory[x][y]->count + item->count) <= item->maxStack) {
                    inventory[x][y]->setCount(inventory[x][y]->count + item->count);
                    return true;
                }
            }
        }
        return false;
    }

    template <typename ItemType>
    bool pickup(ItemType* item) {
        static_assert(std::is_base_of_v<Item, ItemType>, "item is not of type Item");
        ItemType cpy = *item;

        for (int x = 0; x < inventoryRows; x++) {
            for (int y = 0; y < inventoryColumns; y++) {
                if (inventory[x][y] == nullptr) {
                    inventory[x][y] = std::shared_ptr<Item>(new ItemType(cpy));
                    return true;
                }
                else if (inventory[x][y]->id == item->id && (inventory[x][y]->count + item->count) <= item->maxStack) {
                    inventory[x][y]->setCount(inventory[x][y]->count + item->count);
                    return true;
                }
            }
        }
        return false;
    }



    
    bool hurt(int dmg, float kb=0,Entity* src=nullptr) override;
    void kill() override;
    Vector2 moveEntity(Vector2 velocity) override;
    void rightClick();
    void leftClick();
    void handleMouseInput();
    void handleKeyInput();
    void jump();
    bool dropItem(std::shared_ptr<Item>& item);
    bool removeFromInventory(int row, int col, int amount);
    

};