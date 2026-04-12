#pragma once
#include "Item.h"

/*
 * HealItem : Objet de soin. Herite de Item.
 * Quand utilise, il restaure m_value points de vie au joueur,
 * sans depasser son HP maximum. C'est la seule sous-classe concrete
 * d'Item pour l'instant.
 */
class HealItem : public Item {
public:
    HealItem(string name, int value, int quantity);

    void applyEffect(Player& player) override;
    void display() const override;
};
