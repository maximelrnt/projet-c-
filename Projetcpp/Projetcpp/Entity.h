/*
 * Entity.h
 *
 * Classe abstraite de base pour tous les personnages du jeu (Joueur et Monstres).
 * Elle définit les attributs essentiels comme le nom et les points de vie (HP).
 * Elle impose une interface commune avec les fonctions virtuelles pures
 * 'attaquer' et 'afficherInfos', permettant le polymorphisme en combat.
 */
#pragma once
#include <string>
using namespace std;

class Entity {
protected:
    string nom;
    int hpMax;
    int hpActuels;

public:
    Entity(string nom, int hpMax) : nom(nom), hpMax(hpMax), hpActuels(hpMax) {}
    virtual ~Entity() {}

    string getNom() const { return nom; }
    int getHpMax() const { return hpMax; }
    int getHpActuels() const { return hpActuels; }

    void soigner(int montant) {
        hpActuels += montant;
        if (hpActuels > hpMax) hpActuels = hpMax;
    }

    void recevoirDegats(int degats) {
        hpActuels -= degats;
        if (hpActuels < 0) hpActuels = 0;
    }

    bool estVivant() const { return hpActuels > 0; }

    virtual void attaquer(Entity& cible) = 0;
    virtual void afficherInfos() const = 0;
};
