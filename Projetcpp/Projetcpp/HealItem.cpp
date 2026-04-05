#include "HealItem.h"
#include "Player.h"
#include <iostream>

using namespace std;

HealItem::HealItem(string name, int value, int quantity)
    : Item(name, "HEAL", value, quantity)
{
    this->m_name = name;
    this->m_value = value;
    this->m_quantity = quantity;
}

void HealItem::applyEffect(Player& player) {
    int ancienHp = player.getHp();
    int nouveauHp = ancienHp + this->m_value;

    if (nouveauHp > player.getHpMax()) {
        nouveauHp = player.getHpMax();
    }

    player.setHp(nouveauHp);

    int soinsReels = nouveauHp - ancienHp;

    cout << "  Vous utilisez " << this->m_name << " et recuperez " << soinsReels << " HP !" << endl;
    cout << "  HP : " << player.getHp() << "/" << player.getHpMax() << endl;
}

void HealItem::display() const {
    cout << "  - " << this->m_name << " [HEAL] +" << this->m_value << " HP | Quantite : " << this->m_quantity << endl;
}