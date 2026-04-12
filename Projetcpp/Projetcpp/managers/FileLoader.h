#pragma once
#include "../items/Item.h"
#include "../core/Monster.h"
#include "../core/Player.h"
#include <vector>
#include <string>

using namespace std;

// FileLoader : classe utilitaire statique pour lire les fichiers CSV
// elle charge les items et les monstres depuis les fichiers de donnes
class FileLoader {
public:
    // charge les items depuis le csv et les ajoute a l'inventaire du joueur
    static void loadItems(const string& filepath, Player& player);

    // charge les monstres depuis le csv et retourne un vector
    static vector<Monster> loadMonsters(const string& filepath);

private:
    // enleve les espaces autour d'un string
    static string trim(const string& s);
};
