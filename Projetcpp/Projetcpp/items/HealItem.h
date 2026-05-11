// HealItem.h — HERITAGE : HealItem <|-- Item
#pragma once
#include "Item.h"

class HealItem : public Item {
public:
    HealItem(const string& name, int value, int quantity);

    void applyEffect(Player& player) override;
    void display() const override;
};
