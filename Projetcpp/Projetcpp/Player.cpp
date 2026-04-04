/*
 * Player.cpp
 *
 * Implémentation des mécanismes du joueur : gestion de l'inventaire, 
 * calcul des dégâts d'attaque (aléatoires comme imposé par l'énoncé),
 * et suivi des statistiques globales de la partie.
 */
#include "Player.h"
#include <iostream>
#include <cstdlib> // Pour rand()

using namespace std;

Player::Player(string nom, int hpMax) 
    : Entity(nom, hpMax), victoires(0), monstresTues(0), monstresEpargnes(0) {}

void Player::afficherInventaire() const {
    if (inventaire.empty()) {
        cout << "  Votre inventaire est vide." << endl;
        return;
    }
    cout << "=== VOTRE INVENTAIRE ===" << endl;
    for (size_t i = 0; i < inventaire.size(); ++i) {
        cout << "  [" << i << "] ";
        inventaire[i].afficher();
    }
}

void Player::utiliserItem(int index) {
    if (index < 0 || (size_t)index >= inventaire.size()) {
        cout << "  Index d'item invalide." << endl;
        return;
    }

    Item& item = inventaire[index];
    if (item.utiliser()) {
        cout << "  Vous utilisez " << item.getNom() << " et recuperez " << item.getValeur() << " HP." << endl;
        soigner(item.getValeur());
    } else {
        cout << "  Plus de " << item.getNom() << " disponible." << endl;
    }
}

void Player::attaquer(Entity& cible) {
    // degats = rand(0, HP_max_cible) comme défini dans l'énoncé
    int degats = rand() % (cible.getHpMax() + 1);

    if (degats == 0) {
        cout << "  L'attaque a echouee !" << endl;
    } else {
        cout << "  Vous infligez " << degats << " degats a " << cible.getNom() << "." << endl;
        cible.recevoirDegats(degats);
    }
}

void Player::afficherInfos() const {
    cout << "--- STATISTIQUES DE " << nom << " ---" << endl;
    cout << "  HP : " << hpActuels << " / " << hpMax << endl;
    cout << "  Victoires : " << victoires << endl;
    cout << "  Tue : " << monstresTues << " | Epargne : " << monstresEpargnes << endl;
}

void Player::ajouterVictoire(bool tue) {
    victoires++;
    if (tue) monstresTues++;
    else monstresEpargnes++;
}
