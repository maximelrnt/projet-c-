#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

GameManager::GameManager() : m_player(nullptr), jeuEnCours(true) {}

GameManager::~GameManager() {
    delete m_player;
}

void GameManager::demarrer() {
    string nomJoueur;
    cout << "=== BIENVENU DANS ALTERDUNE ===" << endl;
    cout << "Entrez le nom de votre personnage : ";
    cin >> nomJoueur;

    m_player = new Player(nomJoueur, 50, 10, 5); // 50 HP, 10 ATK, 5 DEF

    chargerItems("csv/items.csv");
    chargerMonstres("csv/monsters.csv");

    cout << "\nChargement termine. Bonne chance, " << nomJoueur << " !" << endl;
    showMainMenu();
}

void GameManager::chargerItems(string chemin) {
    ifstream fichier(chemin);
    if (!fichier.is_open()) {
        fichier.open("items.csv");
        if(!fichier.is_open()) return;
    }

    string ligne, entete;
    getline(fichier, entete); 

    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string nom, type, valS, qteS;
        
        getline(ss, nom, ';');
        getline(ss, type, ';');
        getline(ss, valS, ';');
        getline(ss, qteS, ';');

        if (!nom.empty()) {
            Item* it = new Item(nom, type, stoi(valS), stoi(qteS));
            m_player->getInventory().push_back(it);
        }
    }
    fichier.close();
}

void GameManager::chargerMonstres(string chemin) {
    ifstream fichier(chemin);
    if (!fichier.is_open()) {
        fichier.open("monsters.csv");
        if(!fichier.is_open()) return;
    }

    string ligne, entete;
    getline(fichier, entete);

    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string cat, nom, hp, atk, def, mGoal, a1, a2, a3, a4;

        getline(ss, cat, ';');
        getline(ss, nom, ';');
        getline(ss, hp, ';');
        getline(ss, atk, ';');
        getline(ss, def, ';');
        getline(ss, mGoal, ';');
        getline(ss, a1, ';');
        getline(ss, a2, ';');
        getline(ss, a3, ';');
        getline(ss, a4, ';');

        if (nom.empty()) continue;

        MonsterCategory mc = MonsterCategory::NORMAL;
        if (cat == "MINIBOSS") mc = MonsterCategory::MINIBOSS;
        if (cat == "BOSS") mc = MonsterCategory::BOSS;

        vector<string> acts;
        if (!a1.empty() && a1 != " ") acts.push_back(a1);
        if (!a2.empty() && a2 != " ") acts.push_back(a2);
        if (!a3.empty() && a3 != " ") acts.push_back(a3);
        if (!a4.empty() && a4 != " ") acts.push_back(a4);

        Monster m(nom, mc, stoi(hp), stoi(atk), stoi(def), stoi(mGoal), acts);
        m_monsterPool.push_back(m);
    }
    fichier.close();
}

void GameManager::showMainMenu() {
    while (jeuEnCours) {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "1. Demarrer un combat" << endl;
        cout << "2. Voir l'inventaire" << endl;
        cout << "3. Voir le bestiaire" << endl;
        cout << "4. Voir mes statistiques" << endl;
        cout << "5. Quitter" << endl;
        cout << "Choix : ";

        int choix;
        cin >> choix;

        switch (choix) {
            case 1: cout << "[Simulation] Lancement combat..." << endl; break;
            case 2: 
                // Simulation affichage en l'absence de vraie classe Inventory
                if(m_player->getInventory().empty()) cout << "  Inventaire vide.\n";
                else {
                    for(auto* it : m_player->getInventory()) it->display();
                }
                break;
            case 3: m_bestiary.display(); break;
            case 4: m_player->displayStats(); break;
            case 5: jeuEnCours = false; break;
            default: cout << "Choix invalide." << endl;
        }
        
        if (choix == 1) break;
    }
}
