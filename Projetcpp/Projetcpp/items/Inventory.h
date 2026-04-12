#pragma once
#include "Item.h"
#include <vector>

using namespace std;

class Player; // Forward declaration pour la méthode useItem

/*
 * Inventory : Gère la liste des objets (Item*) possédés par le joueur.
 * Encapsule un vector<Item*> pour proposer une interface simple et claire
 * afin d'ajouter, afficher, ou utiliser des objets pendant le combat ou l'exploration.
 * Elle est responsable de libérer la mémoire allouée aux items lors de sa destruction.
 */
class Inventory {
private:
    vector<Item*> m_items;

public:
    Inventory() {}
    ~Inventory();

    // Accès aux objets (principalement pour les itérations d'interface ou la sauvegarde)
    const vector<Item*>& getItems() const { return m_items; }
    vector<Item*>& getItemsRaw() { return m_items; }

    void addItem(Item* item);
    bool useItem(int index, Player& player);
    void display() const;
    
    bool isEmpty() const;
    bool hasUsableItems() const; // Vérifie s'il reste des objets avec quantité > 0
    int size() const;
};
