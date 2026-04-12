#include "FileLoader.h"
#include "../items/HealItem.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// fonction pour enlever les espaces au debut et a la fin d'un string
string FileLoader::trim(const string& s) {
    int start = (int)s.find_first_not_of(" \t\r\n");
    if (start == (int)string::npos) return "";
    int end = (int)s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// charge les items depuis le fichier CSV et les ajoute au joueur
void FileLoader::loadItems(const string& filepath, Player& player) {
    ifstream fichier(filepath);
    if (!fichier.is_open()) {
        // si ca marche pas on essaye le chemin relatif
        fichier.open("csv/items.csv");
        if (!fichier.is_open()) {
            cout << "[Erreur] Impossible d'ouvrir " << filepath << endl;
            return;
        }
    }

    string ligne, entete;
    getline(fichier, entete);  // on saute la premiere ligne (les noms de colonnes)

    while (getline(fichier, ligne)) {
        if (ligne.empty()) continue;

        stringstream ss(ligne);
        string nom, type, valS, qteS;

        // on separe par les points-virgules
        getline(ss, nom,  ';');
        getline(ss, type, ';');
        getline(ss, valS, ';');
        getline(ss, qteS, ';');

        // on nettoie les espaces
        nom  = trim(nom);
        type = trim(type);
        valS = trim(valS);
        qteS = trim(qteS);

        if (nom.empty() || valS.empty() || qteS.empty()) continue;

        // on cree l'item selon son type
        if (type == "HEAL") {
            HealItem* it = new HealItem(nom, stoi(valS), stoi(qteS));
            player.getInventory().addItem(it);
        }
    }
    fichier.close();
}

// charge les monstres depuis le fichier CSV
vector<Monster> FileLoader::loadMonsters(const string& filepath) {
    vector<Monster> pool;

    ifstream fichier(filepath);
    if (!fichier.is_open()) {
        fichier.open("csv/monsters.csv");
        if (!fichier.is_open()) {
            cout << "[Erreur] Impossible d'ouvrir " << filepath << endl;
            return pool;
        }
    }

    string ligne, entete;
    getline(fichier, entete);  // on saute l'entete

    while (getline(fichier, ligne)) {
        if (ligne.empty()) continue;

        stringstream ss(ligne);
        string cat, nom, hp, atk, def, mGoal, a1, a2, a3, a4;

        getline(ss, cat,   ';');
        getline(ss, nom,   ';');
        getline(ss, hp,    ';');
        getline(ss, atk,   ';');
        getline(ss, def,   ';');
        getline(ss, mGoal, ';');
        getline(ss, a1,    ';');
        getline(ss, a2,    ';');
        getline(ss, a3,    ';');
        getline(ss, a4,    ';');

        // on nettoie tout
        cat   = trim(cat);
        nom   = trim(nom);
        hp    = trim(hp);
        atk   = trim(atk);
        def   = trim(def);
        mGoal = trim(mGoal);
        a1    = trim(a1);
        a2    = trim(a2);
        a3    = trim(a3);
        a4    = trim(a4);

        if (nom.empty() || hp.empty()) continue;

        // on determine la categorie
        MonsterCategory mc = MonsterCategory::NORMAL;
        if (cat == "MINIBOSS") mc = MonsterCategory::MINIBOSS;
        if (cat == "BOSS")     mc = MonsterCategory::BOSS;

        // on recupere les IDs d'actions
        vector<string> acts;
        if (!a1.empty()) acts.push_back(a1);
        if (!a2.empty()) acts.push_back(a2);
        if (!a3.empty()) acts.push_back(a3);
        if (!a4.empty()) acts.push_back(a4);

        Monster m(nom, mc, stoi(hp), stoi(atk), stoi(def), stoi(mGoal), acts);
        pool.push_back(m);
    }
    fichier.close();
    return pool;
}
