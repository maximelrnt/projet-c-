#pragma once
#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "Item.h"
#include <vector>
#include <string>

using namespace std;

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
    void chargerItems(string chemin);
    void chargerMonstres(string chemin);

    void showMainMenu();
    void lancerCombat();
};
