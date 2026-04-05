#pragma once
#include "Player.h"
#include "Bestiary.h"
#include "Inventory.h"
#include <string>

using namespace std;

/*
 * SaveManager : Gère l'enregistrement et le chargement d'une partie.
 * Ecrit les stats du joueur, du bestiaire et de l'inventaire dans un simple fichier .sav
 */
class SaveManager {
public:
    static bool saveGame(const string& filename, const Player& player, const Bestiary& bestiary);
    static bool loadGame(const string& filename, Player& player, Bestiary& bestiary);

private:
    static string trim(const string& s);
};
