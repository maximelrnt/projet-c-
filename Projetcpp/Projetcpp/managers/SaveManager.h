#pragma once
#include "../core/Player.h"
#include "Bestiary.h"
#include "../items/Inventory.h"
#include <string>

using namespace std;

// SaveManager : gere la sauvegarde et le chargement du jeu
// ecrit et lit les stats du joueur, l'inventaire et le bestiaire dans un fichier .sav
class SaveManager {
public:
    // sauvegarde la partie dans un fichier
    static bool saveGame(const string& filename, const Player& player, const Bestiary& bestiary);

    // charge une partie depuis un fichier
    static bool loadGame(const string& filename, Player& player, Bestiary& bestiary);

private:
    // enleve les espaces autour d'un string (utilitaire)
    static string trim(const string& s);
};
