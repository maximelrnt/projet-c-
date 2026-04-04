/*
 * Monster.h
 *
 * Classe representant un ennemi du jeu. Chaque monstre possede une jauge Mercy
 * (pitie) et une liste d'actions ACT que le joueur peut lui appliquer.
 * Déclinée en 3 sous-classes (Normal, MiniBoss, Boss) pour le polymorphisme
 * imposé par l'énoncé : chacune limite le nombre d'actions ACT disponibles.
 */
#pragma once
#include "Entity.h"
#include <vector>
using namespace std;

class Monster : public Entity {
protected:
    int atk;
    int def;
    int jaugeMercy;
    int objMercy;
    string categorie;
    vector<int> idsActionsDisponibles;

public:
    Monster(string nom, int hpMax, int atk, int def, int objMercy, string categorie);

    int getAtk() const { return atk; }
    int getDef() const { return def; }
    int getJaugeMercy() const { return jaugeMercy; }
    int getObjMercy() const { return objMercy; }
    string getCategorie() const { return categorie; }
    const vector<int>& getIdsActions() const { return idsActionsDisponibles; }

    void modifierMercy(int montant);
    bool estEpargnable() const { return jaugeMercy >= objMercy; }
    void ajouterIdAction(int id) { idsActionsDisponibles.push_back(id); }

    virtual void attaquer(Entity& cible) override;
    virtual void afficherInfos() const override;
    virtual int getNbActionsMax() const = 0;  // Impose par les sous-classes
};

// 2 actions ACT disponibles
class NormalMonster : public Monster {
public:
    NormalMonster(string nom, int hpMax, int atk, int def, int objMercy)
        : Monster(nom, hpMax, atk, def, objMercy, "NORMAL") {}
    virtual int getNbActionsMax() const override { return 2; }
};

// 3 actions ACT disponibles
class MiniBossMonster : public Monster {
public:
    MiniBossMonster(string nom, int hpMax, int atk, int def, int objMercy)
        : Monster(nom, hpMax, atk, def, objMercy, "MINIBOSS") {}
    virtual int getNbActionsMax() const override { return 3; }
};

// 4 actions ACT disponibles
class BossMonster : public Monster {
public:
    BossMonster(string nom, int hpMax, int atk, int def, int objMercy)
        : Monster(nom, hpMax, atk, def, objMercy, "BOSS") {}
    virtual int getNbActionsMax() const override { return 4; }
};
