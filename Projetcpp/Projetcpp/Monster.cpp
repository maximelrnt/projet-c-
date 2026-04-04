/*
 * Monster.cpp
 * Implementation de la logique des monstres. Gere l'attaque aleatoire
 * et l'evolution de la jauge Mercy.
 */
#include "Monster.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Monster::Monster(string nom, int hpMax, int atk, int def, int objMercy, string categorie)
    : Entity(nom, hpMax), atk(atk), def(def), jaugeMercy(0), objMercy(objMercy), categorie(categorie) {}

void Monster::modifierMercy(int montant) {
    jaugeMercy += montant;
    if (jaugeMercy < 0) jaugeMercy = 0;
    if (jaugeMercy > objMercy) jaugeMercy = objMercy;
}

void Monster::attaquer(Entity& cible) {
    // Formule imposée par l'énoncé : degats = rand(0, HP_max_cible)
    int degats = rand() % (cible.getHpMax() + 1);
    if (degats == 0)
        cout << "  " << nom << " a rate son attaque !" << endl;
    else {
        cout << "  " << nom << " vous inflige " << degats << " degats." << endl;
        cible.recevoirDegats(degats);
    }
}

void Monster::afficherInfos() const {
    cout << "--- MONSTRE : " << nom << " (" << categorie << ") ---" << endl;
    cout << "  HP : " << hpActuels << " / " << hpMax << endl;
    cout << "  ATK : " << atk << " | DEF : " << def << endl;
    cout << "  MERCY : " << jaugeMercy << " / " << objMercy << endl;
}
