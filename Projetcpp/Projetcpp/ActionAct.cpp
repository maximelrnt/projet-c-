// =============================================================================
// ActionAct.cpp
//
// Implémentation de la classe ActionAct et définition du catalogue des actions.
// Voir ActionAct.h pour le rôle général de cette classe.
// =============================================================================
#include "ActionAct.h"
#include <iostream>

using namespace std;

// Constructeur : initialise l'id, la description et l'impact sur la jauge Mercy
ActionAct::ActionAct(int id, const string& description, int impactMercy)
    : id(id), description(description), impactMercy(impactMercy) {}

// --- Getters ---
int ActionAct::getId() const { return id; }
string ActionAct::getDescription() const { return description; }
int ActionAct::getImpactMercy() const { return impactMercy; }

// Affiche l'action avec son impact de manière lisible
void ActionAct::afficher() const {
    cout << "  [" << id << "] " << description;
    if (impactMercy > 0)
        cout << "  (+Mercy)";
    else if (impactMercy < 0)
        cout << "  (-Mercy)";
    cout << endl;
}

// Catalogue statique des 10 actions disponibles dans le jeu.
vector<ActionAct> ActionAct::getCatalogue() {
    return {
        ActionAct(1, "Parler calmement",         +20),
        ActionAct(2, "Complimenter",              +25),
        ActionAct(3, "Chanter une berceuse",      +30),
        ActionAct(4, "Faire un câlin",            +35),
        ActionAct(5, "Observer en silence",         0),
        ActionAct(6, "Imiter le monstre",           0),
        ActionAct(7, "Insulter le monstre",       -20),
        ActionAct(8, "Provoquer agressivement",   -30),
        ActionAct(9, "Proposer un accord",        +15),
        ActionAct(10, "Danser ridiculement",      +10),
    };
}
