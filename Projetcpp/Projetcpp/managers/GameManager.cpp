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
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

#include "../combat/ActCatalogue.h"
#include "FileLoader.h"
#include "SaveManager.h"

using namespace std;

// ---------------------------------------------------------------
//  Constructeur / Destructeur
// ---------------------------------------------------------------
GameManager::GameManager() : m_player(nullptr), jeuEnCours(true) {}

GameManager::~GameManager() {
    delete m_player;
}

// ---------------------------------------------------------------
//  Ecran titre avec couleurs ANSI
// ---------------------------------------------------------------
static void afficherTitre() {
    const char* GOLD  = "\033[1;33m";
    const char* CYAN  = "\033[1;36m";
    const char* WHITE = "\033[1;97m";
    const char* RESET = "\033[0m";

    cout << GOLD;
    cout << "\n  +==============================================+\n";
    cout << "  ||                                            ||\n";
    cout << "  ||  " << WHITE << "  ___  _    _____  ___  ____  _   _ _  _ ___  " << GOLD << "||\n";
    cout << "  ||  " << WHITE << " / _ \\| |    |_ _|| __||  _ \\| | | | \\| | __| " << GOLD << "||\n";
    cout << "  ||  " << WHITE << "| |_| | |__   | |  | |_ | |_) | |_| | .` | _|  " << GOLD << "||\n";
    cout << "  ||  " << WHITE << " \\___/|____|  |_|  |___||____/ \\___/|_|\\_|___| " << GOLD << "||\n";
    cout << "  ||                                            ||\n";
    cout << "  ||        " << CYAN << "~~ D U N E   E D I T I O N ~~" << GOLD << "        ||\n";
    cout << "  ||                                            ||\n";
    cout << "  ||     " << WHITE << "Un RPG console inspire d'Undertale     " << GOLD << " ||\n";
    cout << "  ||     " << WHITE << "  ESILV A3  --  Projet C++ POO  " << GOLD << "       ||\n";
    cout << "  ||                                            ||\n";
    cout << "  +==============================================+\n";
    cout << RESET << "\n";
}

// ---------------------------------------------------------------
//  Demarrage du jeu : saisie du nom, chargement CSV, menu
// ---------------------------------------------------------------
void GameManager::demarrer() {
    afficherTitre();

    string nomJoueur;
    cout << "  Entrez le nom de votre personnage : ";
    cin >> nomJoueur;

    m_player = new Player(nomJoueur, 100, 20, 10);

    ActCatalogue::init();

    cout << "\n  Voulez-vous charger la sauvegarde existante (Data/Save/save1.sav) ? (o/n) : ";
    char choixLoad;
    cin >> choixLoad;

    if (choixLoad == 'o' || choixLoad == 'O') {
        if (SaveManager::loadGame("Data/Save/save1.sav", *m_player, m_bestiary)) {
            cout << "  Sauvegarde chargee avec succes !\n";
            // On charge quand meme les monstres pour la pool de combat
            m_monsterPool = FileLoader::loadMonsters("csv/monsters.csv");
            cout << "\n--- Heureux de vous revoir, " << m_player->getName() << " ! ---" << endl;
        } else {
            cout << "  Echec du chargement. Lancement d'une nouvelle partie...\n";
            FileLoader::loadItems("csv/items.csv", *m_player);
            m_monsterPool = FileLoader::loadMonsters("csv/monsters.csv");
            cout << "\n--- Bienvenue, " << nomJoueur << " ! ---" << endl;
        }
    } else {
        FileLoader::loadItems("csv/items.csv", *m_player);
        m_monsterPool = FileLoader::loadMonsters("csv/monsters.csv");
        cout << "\n--- Bienvenue, " << nomJoueur << " ! ---" << endl;
    }
    m_player->display();
    cout << "  Inventaire de depart :" << endl;
    m_player->getInventory().display();
    cout << "\nObjectif : vaincre 10 ennemis. Bonne chance !" << endl;

    showMainMenu();
}

// ---------------------------------------------------------------
//  Menu principal : tourne jusqu'a la fin du jeu
// ---------------------------------------------------------------
void GameManager::showMainMenu() {
    while (jeuEnCours && m_player->isAlive() && !m_player->hasWon()) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << "\n===============================" << endl;
        cout << "       MENU PRINCIPAL          " << endl;
        cout << "  Victoires : " << m_player->getVictories() << " / 10" << endl;
        cout << "===============================" << endl;
        cout << "  1. Demarrer un combat" << endl;
        cout << "  2. Voir l'inventaire" << endl;
        cout << "  3. Voir le bestiaire" << endl;
        cout << "  4. Voir mes statistiques" << endl;
        cout << "  5. Sauvegarder la partie" << endl;
        cout << "  6. Quitter" << endl;
        cout << "Choix : ";

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
            if (m_player->getInventory().isEmpty()) {
                cout << "  Inventaire vide." << endl;
            } else {
                cout << "\n--- INVENTAIRE ---" << endl;
                m_player->getInventory().display();
            }
            cout << "\n(Appuyez sur Enter pour continuer)";
            cin.ignore(); cin.get(); 
            break;
        case 3:
            m_bestiary.display();
            cout << "\n(Appuyez sur Enter pour continuer)";
            cin.ignore(); cin.get();
            break;
        case 4:
            m_player->displayStats();
            cout << "\n(Appuyez sur Enter pour continuer)";
            cin.ignore(); cin.get();
            break;
        case 5:
            if (SaveManager::saveGame("Data/Save/save1.sav", *m_player, m_bestiary)) {
                cout << "  Partie sauvegardee dans Data/Save/save1.sav !" << endl;
            } else {
                cout << "  Erreur lors de la sauvegarde." << endl;
            }
            cout << "\n(Appuyez sur Enter pour continuer)";
            cin.ignore(); cin.get();
            break;
        case 6:
            jeuEnCours = false;
            break;
        default:
            cout << "  Choix invalide." << endl;
        }
    }

    // Verification de l'issue apres la boucle
    if (m_player->hasWon()) {
        showEnding();
    } else if (!m_player->isAlive()) {
        cout << "\n===============================" << endl;
        cout << "         GAME OVER             " << endl;
        cout << "===============================" << endl;
        cout << "Vous avez ete vaincu." << endl;
        cout << "Kills : " << m_player->getKills()
             << " | Epargnes : " << m_player->getSpared() << endl;
    }
}

// ---------------------------------------------------------------
//  Lancer un combat aleatoire contre un monstre du pool
// ---------------------------------------------------------------
void GameManager::lancerCombat() {
    if (m_monsterPool.empty()) {
        cout << "  Aucun monstre disponible !" << endl;
        Sleep(2000);
        return;
    }

    // On tire un monstre au hasard et on en fait une copie fraiche
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> uni(0, m_monsterPool.size() - 1);
    int index = uni(rng);
    
    Monster monstre = m_monsterPool[index].clone();

    // Lancement du combat
    Combat combat(*m_player, monstre);
    combat.run();

    // On traite le resultat
    CombatResult resultat = combat.getResult();

    cout << "\n-----------------------------" << endl;
    if (resultat == CombatResult::PLAYER_DEAD) {
        // La boucle du menu s'arretera toute seule (isAlive() == false)

    } else if (resultat == CombatResult::MONSTER_KILLED) {
        cout << "Victoire ! " << monstre.getName() << " a ete elimine." << endl;
        m_player->addKill();
        m_bestiary.addEntry(monstre, true);
        cout << "Victoires : " << m_player->getVictories() << "/10" << endl;

    } else {  // MONSTER_SPARED
        cout << monstre.getName() << " a ete epargne. +1 victoire." << endl;
        m_player->addSpared();
        m_bestiary.addEntry(monstre, false);
        cout << "Victoires : " << m_player->getVictories() << "/10" << endl;
    }

    if (resultat != CombatResult::PLAYER_DEAD) {
        cout << "\n(Appuyez sur Enter pour continuer)";
        cin.ignore(10000, '\n');
        cin.get();
    }
}

// ---------------------------------------------------------------
//  Fin du jeu : affiche le type de fin selon kills / spared
// ---------------------------------------------------------------
void GameManager::showEnding() {
    cout << "\n===============================" << endl;
    cout << "         FIN DU JEU            " << endl;
    cout << "===============================" << endl;
    cout << "Vous avez vaincu 10 ennemis !" << endl;
    cout << "Tues : " << m_player->getKills()
         << " | Epargnes : " << m_player->getSpared() << endl;
    cout << endl;

    int kills  = m_player->getKills();
    int spared = m_player->getSpared();

    if (kills == 0) {
        cout << "--- FIN PACIFISTE ---" << endl;
        cout << "Vous avez epargne chacun de vos adversaires." << endl;
        cout << "La paix regne sur Alterdune." << endl;

    } else if (spared == 0) {
        cout << "--- FIN GENOCIDAIRE ---" << endl;
        cout << "Vous avez elimine tous vos adversaires sans pitie." << endl;
        cout << "Alterdune tremble a l'evocation de votre nom." << endl;

    } else {
        cout << "--- FIN NEUTRE ---" << endl;
        cout << "Vous avez tue et epargne..." << endl;
        cout << "Alterdune se souvient de vous avec ambiguite." << endl;
    }
}
