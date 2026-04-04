/*
 * main.cpp
 *
 * Point d'entrée du programme ALTERDUNE.
 * Il se contente d'instancier le GameManager et de lancer
 * la fonction de démarrage qui gère toute la logique du projet.
 */
#include "GameManager.h"
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    // Initialisation du générateur de nombres aléatoires pour les combats
    srand(static_cast<unsigned int>(time(NULL)));

    // Création du moteur de jeu et lancement de la partie
    GameManager game;
    game.demarrer();

    return 0;
}