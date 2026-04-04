/*
 * GameManager.cpp
 *
 * Implémentation de la lecture des fichiers CSV et de la boucle de jeu.
 * Le parseur utilise un système simple basé sur ';' comme séparateur.
 */
#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

GameManager::GameManager() : joueur(nullptr), jeuEnCours(true) {}

GameManager::~GameManager() {
    delete joueur;
    for (auto m : monstresDisponibles) delete m;
}

void GameManager::demarrer() {
    string nomJoueur;
    cout << "=== BIENVENU DANS ALTERDUNE ===" << endl;
    cout << "Entrez le nom de votre personnage : ";
    cin >> nomJoueur;

    joueur = new Player(nomJoueur, 50); // 50 HP par défaut

    // Chargement des données CSV depuis le dossier courant ou csv/
    chargerItems("csv/items.csv");
    chargerMonstres("csv/monsters.csv");

    cout << "\nChargement termine. Bonne chance, " << nomJoueur << " !" << endl;
    menuPrincipal();
}

void GameManager::chargerItems(string chemin) {
    ifstream fichier(chemin);
    if (!fichier.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir " << chemin << endl;
        // On essaie dans le repertoire courant
        fichier.open("items.csv");
        if(!fichier.is_open()) return;
    }

    string ligne, entete;
    getline(fichier, entete); // Sauter la ligne de titre

    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string nom, type, valS, qteS;
        
        getline(ss, nom, ';');
        getline(ss, type, ';');
        getline(ss, valS, ';');
        getline(ss, qteS, ';');

        if (!nom.empty()) {
            Item it(nom, type, stoi(valS), stoi(qteS));
            joueur->ajouterItem(it);
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

        Monster* m = nullptr;
        if (cat == "NORMAL")   m = new NormalMonster(nom, stoi(hp), stoi(atk), stoi(def), stoi(mGoal));
        else if (cat == "MINIBOSS") m = new MiniBossMonster(nom, stoi(hp), stoi(atk), stoi(def), stoi(mGoal));
        else if (cat == "BOSS")     m = new BossMonster(nom, stoi(hp), stoi(atk), stoi(def), stoi(mGoal));

        if (m) {
            if (!a1.empty()) m->ajouterIdAction(stoi(a1));
            if (!a2.empty()) m->ajouterIdAction(stoi(a2));
            if (!a3.empty()) m->ajouterIdAction(stoi(a3));
            if (!a4.empty()) m->ajouterIdAction(stoi(a4));
            monstresDisponibles.push_back(m);
        }
    }
    fichier.close();
}

void GameManager::menuPrincipal() {
    while (jeuEnCours) {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "1. Demarrer un combat" << endl;
        cout << "2. Voir l'inventaire et les objets" << endl;
        cout << "3. Voir le bestiaire" << endl;
        cout << "4. Voir mes statistiques" << endl;
        cout << "5. Quitter" << endl;
        cout << "Choix : ";

        int choix;
        cin >> choix;

        switch (choix) {
            case 1: cout << "[Simulation] Lancement combat..." << endl; break;
            case 2: joueur->afficherInventaire(); break;
            case 3: bestiaire.afficher(); break;
            case 4: joueur->afficherInfos(); break;
            case 5: jeuEnCours = false; break;
            default: cout << "Choix invalide." << endl;
        }
        
        if (choix == 1) break; // Sort pour lancer un eventuel combat (A coder)
    }
}
