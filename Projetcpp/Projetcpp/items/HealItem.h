#pragma once
#include "Item.h"

// HealItem : objet de soin, herite de Item
// quand on l'utilise ca soigne le joueur de m_value HP
class HealItem : public Item {
public:
    HealItem(string name, int value, int quantity);

    // reimplemente l'effet : soigne le joueur
    void applyEffect(Player& player) override;
    void display() const override;
};
