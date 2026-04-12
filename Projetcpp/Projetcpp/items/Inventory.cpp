#include "Inventory.h"
#include "../core/Player.h"
#include <iostream>

using namespace std;

// destructeur : on libere la memoire de chaque item
Inventory::~Inventory() {
    for (int i = 0; i < (int)m_items.size(); i++) {
        delete m_items[i];
    }
    m_items.clear();
}

// ajoute un item au tableau
void Inventory::addItem(Item* item) {
    m_items.push_back(item);
}

// utilise l'item a l'index donne, applique l'efet sur le joueur
bool Inventory::useItem(int index, Player& player) {
    if (index < 0 || index >= (int)m_items.size()) {
        return false;
    }

    Item* item = m_items[index];
    if (item->use()) {
        item->applyEffect(player);
        return true;
    }

    return false;  // plus de quantite
}

// affiche la liste des items
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

// verifie si l'inventaire est vide
bool Inventory::isEmpty() const {
    return m_items.empty();
}

// verifie si il y a au moins un item avec quantite > 0
bool Inventory::hasUsableItems() const {
    if (isEmpty()) return false;

    for (int i = 0; i < (int)m_items.size(); i++) {
        if (m_items[i]->getQuantity() > 0) return true;
    }
    return false;
}

// retounre le nombre d'items
int Inventory::size() const {
    return (int)m_items.size();
}
