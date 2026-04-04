/*
 * main.cpp
 * Point d'entree du programme ALTERDUNE.
 * Active les couleurs ANSI dans la console Windows,
 * puis lance le GameManager.
 */
#include "GameManager.h"
#include <ctime>
#include <cstdlib>
#include <windows.h>

using namespace std;

int main() {
    // Active les codes couleur ANSI dans la console Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    srand(static_cast<unsigned int>(time(NULL)));

    GameManager game;
    game.demarrer();

    return 0;
}