#include "HealItem.h"
#include "Player.h"
#include <iostream>

using namespace std;

HealItem::HealItem(string name, int value, int quantity)
    : Item(name, "HEAL", value, quantity) {}

void HealItem::applyEffect(Player& player) {
    int ancienHp = player.getHp();
    int nouveauHp = ancienHp + m_value;

    // On ne peut pas depasser le HP max
    if (nouveauHp > player.getHpMax())
        nouveauHp = player.getHpMax();

    player.setHp(nouveauHp);

    int soinsReels = nouveauHp - ancienHp;
    cout << "  Vous utilisez " << m_name << " et recuperez " << soinsReels << " HP !" << endl;
    cout << "  HP : " << player.getHp() << "/" << player.getHpMax() << endl;
}

void HealItem::display() const {
    cout << "  - " << m_name << " [HEAL] +" << m_value << " HP | Quantite : " << m_quantity << endl;
}
