// =============================================================================
// Bestiary.cpp
//
// Implémentation des méthodes de la classe Bestiary.
// Voir Bestiary.h pour le rôle général de cette classe.
// =============================================================================
#include "Bestiary.h"
#include <iostream>

using namespace std;

// Ajoute une nouvelle entrée au bestiaire après un combat terminé
void Bestiary::ajouterEntree(const string& nom, const string& categorie,
                              int hp, int atk, int def, bool eteEpargne) {
    BestiaryEntry entree = {nom, categorie, hp, atk, def, eteEpargne};
    entrees.push_back(entree);
}

// Affiche tous les monstres rencontrés pendant la partie
void Bestiary::afficher() const {
    if (entrees.empty()) {
        cout << "  Aucun monstre rencontre pour l'instant." << endl;
        return;
    }
    cout << "=== BESTIAIRE ===" << endl;
    for (const auto& e : entrees) {
        cout << "  [" << e.categorie << "] " << e.nom
                  << " | HP:" << e.hp << " ATK:" << e.atk << " DEF:" << e.def
                  << " | " << (e.eteEpargne ? "EPARGNE" : "TUE") << endl;
    }
}

// Compte les monstres tués
int Bestiary::getNbTues() const {
    int count = 0;
    for (const auto& e : entrees) {
        if (!e.eteEpargne) count++;
    }
    return count;
}

// Compte les monstres épargnés
int Bestiary::getNbEpargnes() const {
    int count = 0;
    for (const auto& e : entrees) {
        if (e.eteEpargne) count++;
    }
    return count;
}
