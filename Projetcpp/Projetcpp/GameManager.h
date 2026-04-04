/*
 * GameManager.h
 *
 * Classe "cerveau" du projet qui coordonne le démarrage, le chargement
 * des fichiers (CSV) et l'affichage du menu principal. Elle assure la
 * transition entre les phases d'exploration (Bestiaire, Inventaire) et
 * les phases de combat. C'est ici que l'on vérifie la fin de la partie.
 */
#pragma once
#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "Item.h"
#include <vector>
#include <string>

using namespace std;

class GameManager {
private:
    Player* joueur;
    vector<Monster*> monstresDisponibles; // Catalogue des monstres du CSV
    Bestiary bestiaire;
    bool jeuEnCours;

public:
    GameManager();
    ~GameManager();

    // Initialisation
    void demarrer();
    void chargerItems(string chemin);
    void chargerMonstres(string chemin);

    // Boucle de jeu
    void menuPrincipal();
    void lancerCombat();
    void afficherStatistiques();
    void verifierFinPartie();
};
