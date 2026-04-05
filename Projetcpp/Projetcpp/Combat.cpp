#include "Combat.h"
#include "HealItem.h"
#include "ActCatalogue.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <windows.h>   // Sleep()

using namespace std;

// Codes couleur ANSI
static const char* COL_RESET   = "\033[0m";
static const char* COL_BOLD    = "\033[1m";
static const char* COL_RED     = "\033[1;91m";
static const char* COL_YELLOW  = "\033[1;93m";
static const char* COL_CYAN    = "\033[1;96m";
static const char* COL_WHITE   = "\033[1;97m";
static const char* COL_GREEN   = "\033[1;92m";
static const char* COL_MAGENTA = "\033[1;95m";

// ---------------------------------------------------------------
Combat::Combat(Player& player, Monster monster)
    : m_player(player), m_monster(monster), m_result(CombatResult::PLAYER_DEAD) {}

// ---------------------------------------------------------------
//  Retourne la couleur ANSI selon la categorie du monstre
// ---------------------------------------------------------------
string Combat::getCouleurMonstre() const {
    if (m_monster.getCategory() == MonsterCategory::BOSS)     return COL_RED;
    if (m_monster.getCategory() == MonsterCategory::MINIBOSS) return COL_YELLOW;
    return COL_CYAN;
}

// ---------------------------------------------------------------
//  Mappe le nom du monstre vers le chemin de son art ASCII
// ---------------------------------------------------------------
string Combat::getAsciiFilePath() const {
    string nom = m_monster.getName();
    if (nom == "Slime Bleu")           return "Data/slime-bleu.txt";
    if (nom == "Rat Geant")            return "Data/rat-geant.txt";
    if (nom == "Chevalier Rouille")    return "Data/chevalier-rouille.txt";
    if (nom == "Sorciere des Marais")  return "Data/sorciere-des-marais.txt";
    if (nom == "Dragon Ancien")        return "Data/dragon-ancien.txt";
    if (nom == "Lich Corrompue")       return "Data/lich-corrompue.txt";
    return "";
}

// ---------------------------------------------------------------
//  Affiche l'art ASCII du monstre avec son nom, puis attend 2s
// ---------------------------------------------------------------
void Combat::afficherMonstreApparition() const {
    string couleur = getCouleurMonstre();
    string chemin  = getAsciiFilePath();

    cout << "\n";

    // Titre : nom + categorie
    cout << couleur;
    cout << "  +------------------------------------------+" << endl;
    cout << "  |  !! " << m_monster.getName()
         << "  [" << m_monster.getCategoryStr() << "]  apparait !!" << endl;
    cout << "  +------------------------------------------+" << endl;
    cout << COL_RESET << "\n";

    // Art ASCII depuis le fichier
    ifstream f(chemin);
    if (f.is_open()) {
        string ligne;
        while (getline(f, ligne)) {
            cout << couleur << "     " << ligne << COL_RESET << "\n";
        }
        f.close();
    }

    cout << "\n";

    // 2 secondes de pause pour lire l'art
    Sleep(2000);
}

// ---------------------------------------------------------------
//  Boucle principale du combat
// ---------------------------------------------------------------
void Combat::run() {
    afficherMonstreApparition();

    while (m_player.isAlive() && m_monster.isAlive()) {
        system("cls"); // On nettoie l'ecran pour le nouveau tour
        afficherStatut();

        bool continuer = tourJoueur();
        if (!continuer) break;

        if (!m_monster.isAlive()) {
            m_result = CombatResult::MONSTER_KILLED;
            Sleep(1500); // Laisser lire le message de mort
            break;
        }

        bool joueurVivant = tourMonstre();
        Sleep(1500); // Laisser lire l'attaque du monstre avant de refresh

        if (!joueurVivant) {
            m_result = CombatResult::PLAYER_DEAD;
            break;
        }
    }

    // Securite : verifier l'etat final
    if (m_result != CombatResult::MONSTER_SPARED) {
        if (!m_player.isAlive())    m_result = CombatResult::PLAYER_DEAD;
        else if (!m_monster.isAlive()) m_result = CombatResult::MONSTER_KILLED;
    }

    if (m_result == CombatResult::MONSTER_KILLED) {
        afficherVictoire();
    }
}

// ---------------------------------------------------------------
//  Tour du joueur : menu FIGHT / ACT / ITEM / MERCY
// ---------------------------------------------------------------
bool Combat::tourJoueur() {
    int choix = 0;

    cout << "\n" << COL_WHITE << "  Que faites-vous ?" << COL_RESET << endl;
    cout << "  [1] FIGHT  - Attaquer" << endl;
    cout << "  [2] ACT    - Agir" << endl;
    cout << "  [3] ITEM   - Utiliser un objet" << endl;
    cout << "  [4] MERCY  - Epargner" << endl;
    cout << "  Choix : ";
    cin >> choix;

    switch (choix) {

    case 1: {  // FIGHT
        int dmg = calculerDegats(m_player.getAtk());
        m_monster.takeDamage(dmg);
        cout << COL_GREEN << "  Vous attaquez " << m_monster.getName()
             << " : -" << dmg << " HP !" << COL_RESET << endl;
        if (!m_monster.isAlive()) {
            cout << COL_GREEN << "  " << m_monster.getName() << " s'effondre !" << COL_RESET << endl;
            m_result = CombatResult::MONSTER_KILLED;
            return false;
        }
        Sleep(1000); // Petite pause pour l'attaque
        break;
    }

    case 2: {  // ACT
        const vector<string>& actIds = m_monster.getActIds();
        
        vector<ActAction> dispo;
        ActCatalogue::displayAvailable(actIds, dispo);

        if (dispo.empty()) break;

        cout << "  [0] Annuler\n  Choix : ";
        int c; cin >> c;
        if (c == 0 || c < 1 || c > (int)dispo.size()) break;

        int impact = dispo[c - 1].getMercyImpact();
        m_monster.addMercy(impact);
        cout << COL_CYAN << "  " << dispo[c - 1].getDisplayText() << COL_RESET << endl;
        cout << "  Mercy : " << m_monster.getMercy() << "/" << m_monster.getMercyGoal();
        if (impact > 0) cout << COL_GREEN << "  (+" << impact << ")" << COL_RESET;
        else if (impact < 0) cout << COL_RED << "  (" << impact << ")" << COL_RESET;
        cout << endl;
        Sleep(1500); // Laisser lire l'action
        break;
    }

    case 3: {  // ITEM
        Inventory& inv = m_player.getInventory();
        if (inv.isEmpty()) { cout << "  Sac vide !" << endl; break; }

        cout << "\n  Inventaire :" << endl;
        if (!inv.hasUsableItems()) {
            cout << "  Tous les objets sont epuises !" << endl;
            break;
        }
        
        inv.display();

        cout << "  [0] Annuler\n  Choix : ";
        int c; cin >> c;
        if (c == 0 || c < 1 || c > inv.size()) break;

        if (inv.useItem(c - 1, m_player)) {
            cout << "  Effet applique !" << endl;
        } else {
            cout << "  Objet epuise !" << endl;
        }
        Sleep(1500);
        break;
    }

    case 4: {  // MERCY
        if (m_monster.isMercyFull()) {
            cout << COL_MAGENTA << "  Vous eprouvez de la pitie..." << endl;
            cout << "  " << m_monster.getName() << " s'en va. (Epargne)" << COL_RESET << endl;
            m_result = CombatResult::MONSTER_SPARED;
            return false;
        } else {
            cout << COL_YELLOW << "  " << m_monster.getName() << " n'est pas encore apaise !" << endl;
            cout << "  Mercy : " << m_monster.getMercy() << "/" << m_monster.getMercyGoal()
                 << "  -- Utilisez ACT." << COL_RESET << endl;
        }
        break;
    }

    default:
        cout << "  Choix invalide, tour perdu." << endl;
    }

    return true;
}

// ---------------------------------------------------------------
//  Tour du monstre : il attaque le joueur
// ---------------------------------------------------------------
bool Combat::tourMonstre() {
    int dmg = calculerDegats(m_monster.getAtk());
    m_player.takeDamage(dmg);
    cout << COL_RED << "\n  " << m_monster.getName() << " vous attaque : -" << dmg << " HP !"
         << COL_RESET << endl;
    cout << "  Vos HP : " << m_player.getHp() << "/" << m_player.getHpMax() << endl;

    if (!m_player.isAlive()) {
        cout << COL_RED << "  Vous etes a terre..." << COL_RESET << endl;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------
int Combat::calculerDegats(int atk) {
    if (atk <= 0) return 1;
    return rand() % atk + 1;
}

void Combat::afficherStatut() const {
    cout << "\n  " << COL_GREEN << m_player.getName() << COL_RESET
         << " : " << m_player.getHp() << "/" << m_player.getHpMax() << " HP" << endl;
    cout << "  " << getCouleurMonstre() << m_monster.getName() << COL_RESET
         << " : " << m_monster.getHp() << "/" << m_monster.getHpMax() << " HP"
         << "  |  Mercy : " << m_monster.getMercy() << "/" << m_monster.getMercyGoal() << endl;
}

void Combat::afficherSeparateur() const {
    cout << "\n" << COL_WHITE << "  -----------------------------" << COL_RESET << endl;
}

// ---------------------------------------------------------------
//  Affiche un cadre de victoire stylise
// ---------------------------------------------------------------
void Combat::afficherVictoire() const {
    system("cls"); // On vide l'ecran pour la victoire

    string couleur = getCouleurMonstre();

    cout << "\n\n";
    cout << COL_MAGENTA << "  ==============================================" << endl;
    cout << "  ||                                          ||" << endl;
    cout << "  ||               " << COL_YELLOW << "VICTOIRE !!!" << COL_MAGENTA << "               ||" << endl;
    cout << "  ||                                          ||" << endl;
    cout << "  ==============================================" << COL_RESET << "\n" << endl;

    cout << "  Vous avez battu " << couleur << m_monster.getName() << COL_RESET << " !" << endl;
    cout << "\n  Statistiques du monstre vaincu :" << endl;
    cout << "  - HP Max : " << m_monster.getHpMax() << endl;
    cout << "  - ATK    : " << m_monster.getAtk() << endl;
    cout << "  - DEF    : " << m_monster.getDef() << endl;
    cout << "\n  " << COL_CYAN << "Retour au voyage..." << COL_RESET << endl;

    // Pause de 2 secondes demandee par l'utilisateur
    Sleep(2000);
}
