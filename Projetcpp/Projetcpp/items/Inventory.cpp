#include "Inventory.h"
#include "../core/Player.h"
#include <iostream>

using namespace std;

Inventory::~Inventory() {
    for (auto item : m_items) {
        delete item;
    }
    m_items.clear();
}

void Inventory::addItem(Item* item) {
    m_items.push_back(item);
}

bool Inventory::useItem(int index, Player& player) {
    if (index < 0 || index >= (int)m_items.size()) {
        return false;
    }
    
    Item* item = m_items[index];
    if (item->use()) {
        item->applyEffect(player);
        return true;
    }
    
    return false; // L'objet n'a plus de quantité restante
}

void Inventory::display() const {
    if (isEmpty()) {
        cout << "  Inventaire vide." << endl;
        return;
    }
    
    for (int i = 0; i < (int)m_items.size(); i++) {
        cout << "  [" << (i + 1) << "] ";
        m_items[i]->display();
    }
}

bool Inventory::isEmpty() const {
    return m_items.empty();
}

bool Inventory::hasUsableItems() const {
    if (isEmpty()) return false;
    
    for (auto item : m_items) {
        if (item->getQuantity() > 0) return true;
    }
    return false;
}

int Inventory::size() const {
    return (int)m_items.size();
}
