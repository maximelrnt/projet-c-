#pragma once
#include "../items/Item.h"
#include "../core/Monster.h"
#include "../core/Player.h"
#include <vector>
#include <string>

using namespace std;

/*
 * FileLoader : Classe utilitaire statique dédiée à la lecture des fichiers CSV.
 * Elle extrait la logique de parsing hors du GameManager pour affiner
 * la séparation des responsabilités.
 */
class FileLoader {
public:
    static void loadItems(const string& filepath, Player& player);
    static vector<Monster> loadMonsters(const string& filepath);

private:
    static string trim(const string& s);
};
