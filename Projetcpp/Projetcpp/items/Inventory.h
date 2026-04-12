#pragma once
#include "Item.h"
#include <vector>

using namespace std;

class Player;  // forward declaration

// Inventory : gere la liste des objets du joueur
// c'est une composition dans Player (le joueur "possede" un inventaire)
class Inventory {
private:
    vector<Item*> m_items;  // tableau de pointeurs vers les items

public:
    Inventory() {}
    ~Inventory();  // le destructeur libere la memoire des items

    // acces aux items (pour la sauvegarde et l'affichage)
    const vector<Item*>& getItems() const { return m_items; }
    vector<Item*>& getItemsRaw() { return m_items; }

    // ajouter un item
    void addItem(Item* item);

    // utiliser un item a un index donne
    bool useItem(int index, Player& player);

    // afficher tout l'inventaire
    void display() const;

    // verifications
    bool isEmpty() const;
    bool hasUsableItems() const;  // verifie si il reste des objets utilisables
    int size() const;
};
