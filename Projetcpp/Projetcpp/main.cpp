/*
 * main.cpp
 * Point d'entree du programme ALTERDUNE.
 * Active les couleurs ANSI dans la console Windows,
 * puis lance le GameManager.
 */
#include "managers/GameManager.h"
#include <ctime>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    // Active les codes couleur ANSI dans la console Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    srand(static_cast<unsigned int>(time(NULL)));

    GameManager game;
    game.demarrer();

    return 0;
}