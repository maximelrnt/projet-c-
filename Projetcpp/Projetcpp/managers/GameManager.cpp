#include "GameManager.h"
#include "../items/HealItem.h"
#include "../combat/Combat.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

#include "../combat/ActCatalogue.h"
#include "FileLoader.h"
#include "SaveManager.h"

using namespace std;

// codes couleur ANSI pour les menus
static const char* GOLD  = "\033[1;33m";
static const char* CYAN  = "\033[1;36m";
static const char* WHITE = "\033[1;97m";
static const char* GRN   = "\033[1;92m";
static const char* RED   = "\033[1;91m";
static const char* YEL   = "\033[1;93m";
static const char* MAG   = "\033[1;95m";
static const char* BLU   = "\033[1;94m";
static const char* DIM   = "\033[2m";
static const char* BOLD  = "\033[1m";
static const char* RESET = "\033[0m";

// constructuer et destructeur
GameManager::GameManager() : m_player(nullptr), jeuEnCours(true) {}

GameManager::~GameManager() {
    delete m_player;  // on libere le joueur
}

// affiche le titre du jeu en ASCII art avec des couleurs
static void afficherTitre() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout << "\n";
    cout << GOLD << "  #========================================================#" << RESET << "\n";
    cout << GOLD << "  #                                                        #" << RESET << "\n";
    cout << GOLD << "  #" << WHITE << "      _    _   _____ _____ ____  ____  _   _ _   _ ___  " << GOLD << "#" << RESET << "\n";
    cout << GOLD << "  #" << WHITE << "     / \\  | | |_   _| ____| __ \\|  _ \\| | | | \\ | | __| " << GOLD << "#" << RESET << "\n";
    cout << GOLD << "  #" << WHITE << "    / _ \\ | |   | | |  _| |  / /| | | | | | |  \\| |  _| " << GOLD << "#" << RESET << "\n";
    cout << GOLD << "  #" << WHITE << "   / ___ \\| |___| | | |___|  \\ \\| |_| | |_| | |\\  | |__ " << GOLD << "#" << RESET << "\n";
    cout << GOLD << "  #" << WHITE << "  /_/   \\_\\_____|_| |_____|_|\\_\\|____/ \\___/|_| \\_|____| " << GOLD << "#" << RESET << "\n";
    cout << GOLD << "  #                                                        #" << RESET << "\n";
    cout << GOLD << "  #          " << CYAN << "~~  D U N E    E D I T I O N  ~~" << GOLD << "            #" << RESET << "\n";
    cout << GOLD << "  #                                                        #" << RESET << "\n";
    cout << GOLD << "  #" << DIM << "        Un RPG console inspire d'Undertale" << GOLD << "             #" << RESET << "\n";
    cout << GOLD << "  #" << DIM << "          ESILV A3  --  Projet C++ POO" << GOLD << "                #" << RESET << "\n";
    cout << GOLD << "  #                                                        #" << RESET << "\n";
    cout << GOLD << "  #========================================================#" << RESET << "\n";
    cout << "\n";
}

// demarage du jeu : titre, saisie du nom, chargement csv
void GameManager::demarrer() {
#ifdef _WIN32
    PlaySoundA("Data/musiques/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    system("killall -q aplay");
    system("aplay -q Data/musiques/menu.wav &");
#endif

    afficherTitre();

    cout << GOLD << "  >> " << WHITE << "Entrez le nom de votre personnage : " << RESET;
    string nomJoueur;
    cin >> nomJoueur;

    // on cree le joueur avec des stats de base
    m_player = new Player(nomJoueur, 100, 20, 10);

    // on initialise le catalogue d'actions
    ActCatalogue::init();

    // on propose de charger une sauvegard
    cout << "\n" << GOLD << "  >> " << WHITE << "Charger la sauvegarde existante ? " << DIM << "(o/n) : " << RESET;
    char choixLoad;
    cin >> choixLoad;

    if (choixLoad == 'o' || choixLoad == 'O') {
        if (SaveManager::loadGame("Data/Save/save1.sav", *m_player, m_bestiary)) {
            cout << GRN << "  Sauvegarde chargee avec succes !" << RESET << "\n";
            // on charge quand meme les monstres pour les combats
            m_monsterPool = FileLoader::loadMonsters("csv/monsters.csv");
            cout << "\n" << CYAN << "  Heureux de vous revoir, " << WHITE << m_player->getName() << CYAN << " !" << RESET << endl;
        }
        else {
            cout << YEL << "  Echec du chargement. Lancement d'une nouvelle partie..." << RESET << "\n";
            FileLoader::loadItems("csv/items.csv", *m_player);
            m_monsterPool = FileLoader::loadMonsters("csv/monsters.csv");
            cout << "\n" << CYAN << "  Bienvenue, " << WHITE << nomJoueur << CYAN << " !" << RESET << endl;
        }
    }
    else {
        FileLoader::loadItems("csv/items.csv", *m_player);
        m_monsterPool = FileLoader::loadMonsters("csv/monsters.csv");
        cout << "\n" << CYAN << "  Bienvenue, " << WHITE << nomJoueur << CYAN << " !" << RESET << endl;
    }

    // on affiche le resumme de depart
    m_player->display();
    cout << "  Inventaire de depart :" << endl;
    m_player->getInventory().display();
    cout << "\n" << YEL << "  Objectif : vaincre 10 ennemis. Bonne chance !" << RESET << endl;

    cout << "\n" << DIM << "  (Appuyez sur Enter pour continuer)" << RESET;
    cin.ignore(); cin.get();

    showMainMenu();
}

// boucle du menu principal
void GameManager::showMainMenu() {
    while (jeuEnCours && m_player->isAlive() && !m_player->hasWon()) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        // barre de progression des victoires
        int victoires = m_player->getVictories();
        int total = 10;
        string progBar = "[";
        for (int i = 0; i < total; i++) {
            if (i < victoires) progBar += GRN + string("#") + RESET;
            else               progBar += DIM + string(".") + RESET;
        }
        progBar += "]";

        cout << "\n";
        cout << GOLD << "  #========================================#" << RESET << "\n";
        cout << GOLD << "  #                                        #" << RESET << "\n";
        cout << GOLD << "  #" << WHITE << "           MENU  PRINCIPAL" << GOLD << "              #" << RESET << "\n";
        cout << GOLD << "  #                                        #" << RESET << "\n";
        cout << GOLD << "  #========================================#" << RESET << "\n";
        cout << "\n";
        cout << "     " << WHITE << "Victoires : " << GRN << victoires << WHITE << " / " << total << "   " << progBar << RESET << "\n";
        cout << "\n";
        cout << "     " << RED  << "[1]" << WHITE << " Demarrer un combat" << RESET << "\n";
        cout << "     " << YEL  << "[2]" << WHITE << " Voir l'inventaire" << RESET << "\n";
        cout << "     " << CYAN << "[3]" << WHITE << " Voir le bestiaire" << RESET << "\n";
        cout << "     " << BLU  << "[4]" << WHITE << " Voir mes statistiques" << RESET << "\n";
        cout << "     " << GRN  << "[5]" << WHITE << " Sauvegarder la partie" << RESET << "\n";
        cout << "     " << MAG  << "[6]" << WHITE << " Quitter" << RESET << "\n";
        cout << "\n";
        cout << "  " << GOLD << ">> " << WHITE << "Choix : " << RESET;

        int choix;
        cin >> choix;

        switch (choix) {
        case 1:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            lancerCombat();
            break;

        case 2:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            if (m_player->getInventory().isEmpty()) {
                cout << "\n" << YEL << "  Inventaire vide." << RESET << endl;
            }
            else {
                cout << "\n" << GOLD << "  #===================#" << RESET << "\n";
                cout << GOLD << "  #" << WHITE << "    INVENTAIRE" << GOLD << "      #" << RESET << "\n";
                cout << GOLD << "  #===================#" << RESET << "\n\n";
                m_player->getInventory().display();
            }
            cout << "\n" << DIM << "  (Appuyez sur Enter pour continuer)" << RESET;
            cin.ignore(); cin.get();
            break;

        case 3:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            cout << "\n" << GOLD << "  #===================#" << RESET << "\n";
            cout << GOLD << "  #" << WHITE << "    BESTIAIRE" << GOLD << "       #" << RESET << "\n";
            cout << GOLD << "  #===================#" << RESET << "\n\n";
            m_bestiary.display();
            cout << "\n" << DIM << "  (Appuyez sur Enter pour continuer)" << RESET;
            cin.ignore(); cin.get();
            break;

        case 4:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            cout << "\n" << GOLD << "  #===================#" << RESET << "\n";
            cout << GOLD << "  #" << WHITE << "   STATISTIQUES" << GOLD << "    #" << RESET << "\n";
            cout << GOLD << "  #===================#" << RESET << "\n\n";
            m_player->displayStats();
            cout << "\n" << DIM << "  (Appuyez sur Enter pour continuer)" << RESET;
            cin.ignore(); cin.get();
            break;

        case 5:
            // sauvegarde
            if (SaveManager::saveGame("Data/Save/save1.sav", *m_player, m_bestiary)) {
                cout << "\n" << GRN << "  Partie sauvegardee dans Data/Save/save1.sav !" << RESET << endl;
            }
            else {
                cout << "\n" << RED << "  Erreur lors de la sauvegarde." << RESET << endl;
            }
            cout << "\n" << DIM << "  (Appuyez sur Enter pour continuer)" << RESET;
            cin.ignore(); cin.get();
            break;

        case 6:
            jeuEnCours = false;
            break;

        default:
            cout << RED << "  Choix invalide." << RESET << endl;
            Sleep(1000);
        }
    }

    // fin du jeu
    if (m_player->hasWon()) {
        showEnding();
    }
    else if (!m_player->isAlive()) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << "\n";
        cout << RED << "  #========================================#" << RESET << "\n";
        cout << RED << "  #                                        #" << RESET << "\n";
        cout << RED << "  #" << WHITE << BOLD << "            G A M E   O V E R" << RED << "           #" << RESET << "\n";
        cout << RED << "  #                                        #" << RESET << "\n";
        cout << RED << "  #========================================#" << RESET << "\n";
        cout << "\n";
        cout << DIM << "  Vous avez ete vaincu..." << RESET << "\n";
        cout << "  " << RED << "Kills : " << WHITE << m_player->getKills() << RESET;
        cout << "  " << MAG << "Epargnes : " << WHITE << m_player->getSpared() << RESET << "\n";
    }
}

// lance un combat contre un monstre aleatoire
void GameManager::lancerCombat() {
    if (m_monsterPool.empty()) {
        cout << YEL << "  Aucun monstre disponible !" << RESET << endl;
        Sleep(2000);
        return;
    }

    // on tire un monstre au hasard
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> uni(0, (int)m_monsterPool.size() - 1);
    int index = uni(rng);

    // on clone le monstre pour avoir une copie fraiche
    Monster monstre = m_monsterPool[index].clone();

#ifdef _WIN32
    PlaySoundA("Data/musiques/combat.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    system("killall -q aplay");
    system("aplay -q Data/musiques/combat.wav &");
#endif

    // on lance le combat
    Combat combat(*m_player, monstre);
    combat.run();

#ifdef _WIN32
    PlaySoundA("Data/musiques/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    system("killall -q aplay 2>/dev/null");
    system("aplay -q Data/musiques/menu.wav 2>/dev/null &");
#endif

    // on traite le resultat
    CombatResult resultat = combat.getResult();

    if (resultat == CombatResult::PLAYER_DEAD) {
        // la boucle s'arretera toute seule car isAlive() sera false
    }
    else if (resultat == CombatResult::MONSTER_KILLED) {
        cout << "\n" << GRN << "  Victoire ! " << WHITE << monstre.getName() << GRN << " a ete elimine." << RESET << endl;
        m_player->addKill();
        m_bestiary.addEntry(monstre, true);
        cout << "  " << YEL << "Victoires : " << WHITE << m_player->getVictories() << "/10" << RESET << endl;
    }
    else {
        // monstre epargne
        cout << "\n" << MAG << "  " << monstre.getName() << " a ete epargne. +1 victoire." << RESET << endl;
        m_player->addSpared();
        m_bestiary.addEntry(monstre, false);
        cout << "  " << YEL << "Victoires : " << WHITE << m_player->getVictories() << "/10" << RESET << endl;
    }

    if (resultat != CombatResult::PLAYER_DEAD) {
        cout << "\n" << DIM << "  (Appuyez sur Enter pour continuer)" << RESET;
        cin.ignore(10000, '\n');
        cin.get();
    }
}

// affiche la fin du jeu (3 fins possibles)
void GameManager::showEnding() {
#ifdef _WIN32
    PlaySoundA(NULL, NULL, 0); // arreter la musique
    system("cls");
#else
    system("killall -q aplay 2>/dev/null");
    system("clear");
#endif

    int kills  = m_player->getKills();
    int spared = m_player->getSpared();

    // couleur du cadre selon la fin
    const char* endColor = YEL;
    if (kills == 0)      endColor = CYAN;
    else if (spared == 0) endColor = RED;

    cout << "\n";
    cout << endColor << "  #========================================#" << RESET << "\n";
    cout << endColor << "  #                                        #" << RESET << "\n";
    cout << endColor << "  #" << WHITE << BOLD << "          F I N   D U   J E U" << endColor << "            #" << RESET << "\n";
    cout << endColor << "  #                                        #" << RESET << "\n";
    cout << endColor << "  #========================================#" << RESET << "\n";
    cout << "\n";

    cout << "  " << WHITE << "Vous avez vaincu 10 ennemis !" << RESET << "\n";
    cout << "  " << RED << "Tues : " << WHITE << kills << RESET;
    cout << "  " << MAG << "Epargnes : " << WHITE << spared << RESET << "\n\n";

    if (kills == 0) {
        // fin pacifiste
        cout << CYAN << "  ~~~ FIN PACIFISTE ~~~" << RESET << "\n\n";
        cout << "  " << WHITE << "Vous avez epargne chacun de vos adversaires." << RESET << "\n";
        cout << "  " << CYAN << "La paix regne sur Alterdune." << RESET << "\n";
    }
    else if (spared == 0) {
        // fin genocidaire
        cout << RED << "  ~~~ FIN GENOCIDAIRE ~~~" << RESET << "\n\n";
        cout << "  " << WHITE << "Vous avez elimine tous vos adversaires sans pitie." << RESET << "\n";
        cout << "  " << RED << "Alterdune tremble a l'evocation de votre nom." << RESET << "\n";
    }
    else {
        // fin neutre
        cout << YEL << "  ~~~ FIN NEUTRE ~~~" << RESET << "\n\n";
        cout << "  " << WHITE << "Vous avez tue et epargne..." << RESET << "\n";
        cout << "  " << YEL << "Alterdune se souvient de vous avec ambiguite." << RESET << "\n";
    }

    cout << "\n" << endColor << "  #========================================#" << RESET << "\n";
}
