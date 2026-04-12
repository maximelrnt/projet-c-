#include "HealItem.h"
#include "../core/Player.h"
#include <iostream>

using namespace std;

// constructuer : on passe "HEAL" comme type a la classe mere
HealItem::HealItem(string name, int value, int quantity)
    : Item(name, "HEAL", value, quantity)
{
}

// applique l'effet de soin au joueur
void HealItem::applyEffect(Player& player) {
    int ancienHp = player.getHp();
    int nouveauHp = ancienHp + m_value;

    // on depasse pas le max
    if (nouveauHp > player.getHpMax()) {
        nouveauHp = player.getHpMax();
    }

    player.setHp(nouveauHp);

    int soinsReels = nouveauHp - ancienHp;
    cout << "  Vous utilisez " << m_name << " et recuperez " << soinsReels << " HP !" << endl;
    cout << "  HP : " << player.getHp() << "/" << player.getHpMax() << endl;
}

// affichage specifique pour un objet de soin
void HealItem::display() const {
    cout << "  - " << m_name << " [HEAL] +" << m_value << " HP | Quantite : " << m_quantity << endl;
}
