/*
 * Player.h
 *
 * Classe représentant le personnage principal du jeu. Coordonne son inventaire,
 * ses victoires, ainsi que ses interactions (attaquer, soigner).
 * Elle hérite d'Entity pour les fonctionnalités de points de vie et de nom.
 */
#pragma once
#include "Entity.h"
#include "Item.h"
#include <vector>

using namespace std;

class Player : public Entity {
private:
    vector<Item> inventaire;
    int victoires;
    int monstresTues;
    int monstresEpargnes;

public:
    Player(string nom, int hpMax);

    // Getters de statistiques pour les fins multiples
    int getVictoires() const { return victoires; }
    int getMonstresTues() const { return monstresTues; }
    int getMonstresEpargnes() const { return monstresEpargnes; }

    // Gestion de l'inventaire
    void ajouterItem(const Item& item) { inventaire.push_back(item); }
    void afficherInventaire() const;
    void utiliserItem(int index);

    // Enregistrement des résultats de combat
    void ajouterVictoire(bool tue);

    // Implémentation des méthodes virtuelles de Entity
    virtual void attaquer(Entity& cible) override;
    virtual void afficherInfos() const override;
};
