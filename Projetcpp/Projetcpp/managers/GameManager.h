// GameManager.h — Chef d'orchestre du jeu
//   AGREGATION  : GameManager o-- Player* (pointeur, pas de creation directe)
//   COMPOSITION : GameManager *-- Bestiary (attribut direct)
//   DEPENDANCE  : GameManager ..> FileLoader, SaveManager, Combat, ActCatalogue
#pragma once
#include "../core/Player.h"
#include "../core/Monster.h"
#include "Bestiary.h"
#include "../items/Item.h"
#include <vector>
#include <string>

using namespace std;

class GameManager {
private:
    Player*         m_player;       // AGREGATION : pointeur, cree/detruit par GameManager
    Bestiary        m_bestiary;     // COMPOSITION : attribut direct
    vector<Monster> m_monsterPool;
    bool            jeuEnCours;

public:
    GameManager();
    ~GameManager();

    void demarrer();
    void showMainMenu();
    void lancerCombat();
    void showEnding();
};
