// HealItem.cpp — Implementation de HealItem (herite de Item)
#include "HealItem.h"
#include "../core/Player.h"
#include <iostream>

using namespace std;

// Appel au constructeur de Item avec le type "HEAL" fixe
HealItem::HealItem(const string& name, int value, int quantity)
    : Item(name, "HEAL", value, quantity)
{
    // Le type "HEAL" est passe en dur a la classe mere Item
}

void HealItem::applyEffect(Player& player) {
    int ancienHp = player.getHp();
    int nouveauHp = ancienHp + m_value;

    if (nouveauHp > player.getHpMax()) {
        nouveauHp = player.getHpMax();
    }

    player.setHp(nouveauHp);

    int soinsReels = nouveauHp - ancienHp;
    cout << "  Vous utilisez " << m_name << " et recuperez " << soinsReels << " HP !" << endl;
    cout << "  HP : " << player.getHp() << "/" << player.getHpMax() << endl;
}

void HealItem::display() const {
    cout << "  - " << m_name << " [HEAL] +" << m_value << " HP | Quantite : " << m_quantity << endl;
}
