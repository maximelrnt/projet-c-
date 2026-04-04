// =============================================================================
// Item.cpp
//
// Implémentation des méthodes de la classe Item.
// Voir Item.h pour le rôle général de cette classe.
// =============================================================================
#include "Item.h"
#include <iostream>

using namespace std;

// Constructeur : initialise tous les attributs
Item::Item(const string& nom, const string& type, int valeur, int quantite)
    : nom(nom), type(type), valeur(valeur), quantite(quantite) {}

// --- Getters ---
string Item::getNom() const { return nom; }
string Item::getType() const { return type; }
int Item::getValeur() const { return valeur; }
int Item::getQuantite() const { return quantite; }

// Tente d'utiliser l'item. Retourne true si succès, false si stock vide.
bool Item::utiliser() {
    if (quantite <= 0) return false;
    quantite--;
    return true;
}

// Affiche les infos de l'item en console
void Item::afficher() const {
    cout << "  - " << nom << " [" << type << "] +" << valeur << " HP | Quantite : " << quantite << endl;
}
