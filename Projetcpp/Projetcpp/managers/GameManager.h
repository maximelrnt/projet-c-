#pragma once
#include "../core/Player.h"
#include "../core/Monster.h"
#include "Bestiary.h"
#include "../items/Item.h"
#include <vector>
#include <string>

using namespace std;

// GameManager : c'est le chef d'orchestre du jeu
// il gere le menu principal, les combats, la sauvegard, etc
class GameManager {
private:
    Player* m_player;              // pointeur vers le joueur
    Bestiary m_bestiary;           // le bestiaire
    vector<Monster> m_monsterPool; // la liste des monstres
    bool jeuEnCours;               // flag pour savoir si le jeu tourne

public:
    GameManager();
    ~GameManager();

    // lance le jeu
    void demarrer();

    // boucle du menu principal
    void showMainMenu();

    // lance un combat aleatoire
    void lancerCombat();

    // affiche la fin du jeu
    void showEnding();
};
