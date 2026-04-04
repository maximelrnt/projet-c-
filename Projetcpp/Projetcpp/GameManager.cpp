#include "GameManager.h"
#include "HealItem.h"
#include "Combat.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

// ---------------------------------------------------------------
//  Fonction utilitaire : supprime les espaces en debut et fin de chaine
// ---------------------------------------------------------------
static string trim(const string& s) {
    int start = (int)s.find_first_not_of(" \t\r\n");
    if (start == (int)string::npos) return "";
    int end = (int)s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

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

    chargerItems("csv/items.csv");
    chargerMonstres("csv/monsters.csv");

    // Affichage initial demande par l'enonce
    cout << "\n--- Bienvenue, " << nomJoueur << " ! ---" << endl;
    m_player->display();
    cout << "  Inventaire de depart :" << endl;
    for (auto* item : m_player->getInventory()) {
        item->display();
    }
    cout << "\nObjectif : vaincre 10 ennemis. Bonne chance !" << endl;

    showMainMenu();
}

// ---------------------------------------------------------------
//  Chargement du CSV des items : nom; type; valeur; quantite
// ---------------------------------------------------------------
void GameManager::chargerItems(string chemin) {
    ifstream fichier(chemin);
    if (!fichier.is_open()) {
        // Fallback si le chemin relatif ne marche pas
        fichier.open("items.csv");
        if (!fichier.is_open()) {
            cout << "[Erreur] Impossible d'ouvrir items.csv" << endl;
            return;
        }
    }

    string ligne, entete;
    getline(fichier, entete);  // On saute la ligne d'en-tete

    while (getline(fichier, ligne)) {
        if (ligne.empty()) continue;

        stringstream ss(ligne);
        string nom, type, valS, qteS;

        getline(ss, nom,  ';');
        getline(ss, type, ';');
        getline(ss, valS, ';');
        getline(ss, qteS, ';');

        // On nettoie les espaces autour des valeurs
        nom  = trim(nom);
        type = trim(type);
        valS = trim(valS);
        qteS = trim(qteS);

        if (nom.empty() || valS.empty() || qteS.empty()) continue;

        if (type == "HEAL") {
            HealItem* it = new HealItem(nom, stoi(valS), stoi(qteS));
            m_player->getInventory().push_back(it);
        }
        // D'autres types pourront etre ajoutes ici plus tard
    }

    fichier.close();
}

// ---------------------------------------------------------------
//  Chargement du CSV des monstres
//  Format : categorie; nom; hp; atk; def; mercyGoal; act1; act2; [act3]; [act4]
// ---------------------------------------------------------------
void GameManager::chargerMonstres(string chemin) {
    ifstream fichier(chemin);
    if (!fichier.is_open()) {
        fichier.open("monsters.csv");
        if (!fichier.is_open()) {
            cout << "[Erreur] Impossible d'ouvrir monsters.csv" << endl;
            return;
        }
    }

    string ligne, entete;
    getline(fichier, entete);

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

        // Nettoyage des espaces
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

        MonsterCategory mc = MonsterCategory::NORMAL;
        if (cat == "MINIBOSS") mc = MonsterCategory::MINIBOSS;
        if (cat == "BOSS")     mc = MonsterCategory::BOSS;

        vector<string> acts;
        if (!a1.empty()) acts.push_back(a1);
        if (!a2.empty()) acts.push_back(a2);
        if (!a3.empty()) acts.push_back(a3);
        if (!a4.empty()) acts.push_back(a4);

        Monster m(nom, mc, stoi(hp), stoi(atk), stoi(def), stoi(mGoal), acts);
        m_monsterPool.push_back(m);
    }

    fichier.close();
}

// ---------------------------------------------------------------
//  Menu principal : tourne jusqu'a la fin du jeu
// ---------------------------------------------------------------
void GameManager::showMainMenu() {
    while (jeuEnCours && m_player->isAlive() && !m_player->hasWon()) {
        system("cls"); // On nettoie le menu pour que ce soit propre
        cout << "\n===============================" << endl;
        cout << "       MENU PRINCIPAL          " << endl;
        cout << "  Victoires : " << m_player->getVictories() << " / 10" << endl;
        cout << "===============================" << endl;
        cout << "  1. Demarrer un combat" << endl;
        cout << "  2. Voir l'inventaire" << endl;
        cout << "  3. Voir le bestiaire" << endl;
        cout << "  4. Voir mes statistiques" << endl;
        cout << "  5. Quitter" << endl;
        cout << "Choix : ";

        int choix;
        cin >> choix;

        switch (choix) {
        case 1:
            system("cls");
            lancerCombat();

            break;
        case 2:
            if (m_player->getInventory().empty()) {
                cout << "  Inventaire vide." << endl;
            } else {
                cout << "\n--- INVENTAIRE ---" << endl;
                for (auto* item : m_player->getInventory()) {
                    item->display();
                }
            }
            cout << "\n(Appuyez sur Enter pour continuer)";
            cin.ignore(); cin.get(); // Petite pause lecture
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
        return;
    }

    // On tire un monstre au hasard et on en fait une copie fraiche
    int index = rand() % (int)m_monsterPool.size();
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
