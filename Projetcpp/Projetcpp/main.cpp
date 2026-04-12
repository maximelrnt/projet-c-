// main.cpp - point d'entree du jeu ALTERDUNE
// on active les couleurs ANSI dans la console Windows
// puis on lance le GameManager qui gere tout

#include "managers/GameManager.h"
#include <ctime>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    // active les codes couleur ANSI dans la console Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    // initialise le generateur aleatoire
    srand(static_cast<unsigned int>(time(NULL)));

    // on cree le jeu et on le lance
    GameManager game;
    game.demarrer();

    return 0;
}