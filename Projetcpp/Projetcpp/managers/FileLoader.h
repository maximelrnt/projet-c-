// FileLoader.h — Classe utilitaire statique (DEPENDANCE depuis GameManager)
#pragma once
#include "../items/Item.h"
#include "../core/Monster.h"
#include "../core/Player.h"
#include <vector>
#include <string>

using namespace std;

class FileLoader {
public:
    static void loadItems(const string& filepath, Player& player);
    static vector<Monster> loadMonsters(const string& filepath);

private:
    static string trim(const string& s);
};
