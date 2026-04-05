#pragma once
#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "Item.h"
#include <vector>
#include <string>

using namespace std;

/*
 * GameManager : Chef d'orchestre du jeu.
 * Il gere le demarrage (saisie du nom, chargement des CSV),
 * la boucle du menu principal, le declenchement des combats,
 * et les conditions de fin de partie (10 victoires, mort du joueur).
 */
class GameManager {
private:
    Player* m_player;
    Bestiary m_bestiary;
    vector<Monster> m_monsterPool;
    bool jeuEnCours;

public:
    GameManager();
    ~GameManager();

    void demarrer();
    void showMainMenu();
    void lancerCombat();
    void showEnding();
};
