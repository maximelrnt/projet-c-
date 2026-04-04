// =============================================================================
// Item.h
//
// Représente un objet consommable dans l'inventaire du joueur.
// Pour l'instant, seul le type HEAL est géré : quand le joueur utilise un Item,
// il récupère un certain nombre de HP. Chaque Item a un nom, un type (string),
// une valeur (quantité de HP soignés) et une quantité disponible.
// Les objets sont chargés depuis le fichier items.csv au démarrage du jeu.
// =============================================================================
#pragma once
#include <string>

using namespace std;

class Item {
private:
    string nom;
    string type;   // Toujours "HEAL" dans ce projet
    int valeur;         // Nombre de HP rendus à l'utilisation
    int quantite;       // Nombre d'utilisations restantes

public:
    // Constructeur
    Item(const string& nom, const string& type, int valeur, int quantite);

    // Getters
    string getNom() const;
    string getType() const;
    int getValeur() const;
    int getQuantite() const;

    // Retourne true si l'item est disponible (quantite > 0) et décrémente la quantite
    bool utiliser();

    // Affiche les informations de l'item dans la console
    void afficher() const;
};
