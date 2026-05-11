// Inventory.h — COMPOSITION dans Player : Player *-- Inventory
//               COMPOSITION de Item*  : Inventory *-- Item
#pragma once
#include "Item.h"
#include <vector>

using namespace std;

class Player;

class Inventory {
private:
    vector<Item*> m_items;  // COMPOSITION : possede les items (delete dans ~Inventory)

public:
    Inventory();
    ~Inventory();  // libere les Item*

    const vector<Item*>& getItems()    const { return m_items; }
    vector<Item*>&       getItemsRaw()       { return m_items; }

    void addItem(Item* item);
    bool useItem(int index, Player& player);
    void display() const;

    bool isEmpty()        const;
    bool hasUsableItems() const;
    int  size()           const;
};
