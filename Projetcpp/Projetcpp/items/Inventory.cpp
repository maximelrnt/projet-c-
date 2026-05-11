// Inventory.cpp — COMPOSITION : possede et libere les Item*
#include "Inventory.h"
#include "../core/Player.h"
#include <iostream>

using namespace std;

Inventory::Inventory() {
    // m_items est un vector vide par defaut, rien a initialiser
}

// Destructeur : libere chaque Item* (COMPOSITION)
Inventory::~Inventory() {
    for (int i = 0; i < (int)m_items.size(); i++) {
        delete m_items[i];
    }
    m_items.clear();
}

void Inventory::addItem(Item* item) {
    m_items.push_back(item);
}

// POLYMORPHISME : applyEffect() appelle la bonne sous-classe via le pointeur Item*
bool Inventory::useItem(int index, Player& player) {
    if (index < 0 || index >= (int)m_items.size()) {
        return false;
    }

    Item* item = m_items[index];
    if (item->use()) {
        item->applyEffect(player);
        return true;
    }

    return false;
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

    for (int i = 0; i < (int)m_items.size(); i++) {
        if (m_items[i]->getQuantity() > 0) return true;
    }
    return false;
}

int Inventory::size() const {
    return (int)m_items.size();
}
