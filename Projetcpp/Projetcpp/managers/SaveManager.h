// SaveManager.h — Classe utilitaire statique (DEPENDANCE depuis GameManager)
#pragma once
#include "../core/Player.h"
#include "Bestiary.h"
#include "../items/Inventory.h"
#include <string>

using namespace std;

class SaveManager {
public:
    static bool saveGame(const string& filename, const Player& player, const Bestiary& bestiary);
    static bool loadGame(const string& filename, Player& player, Bestiary& bestiary);

private:
    static string trim(const string& s);
};
