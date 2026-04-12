#include "Combat.h"
#include "../items/HealItem.h"
#include "ActCatalogue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

using namespace std;

// ================================================================
//  codes couleur ANSI pour la console
// ================================================================
static const char* R   = "\033[0m";         // reset
static const char* BOLD= "\033[1m";
static const char* RED = "\033[1;91m";
static const char* YEL = "\033[1;93m";
static const char* CYA = "\033[1;96m";
static const char* WHI = "\033[1;97m";
static const char* GRN = "\033[1;92m";
static const char* MAG = "\033[1;95m";
static const char* BLU = "\033[1;94m";
static const char* DIM = "\033[2m";
static const char* GRY = "\033[90m";

// caracteres de bordure pour les boites (UTF-8)
static const string TL = "\u250C";
static const string TR = "\u2510";
static const string BL = "\u2514";
static const string BR = "\u2518";
static const string HZ = "\u2500";
static const string VT = "\u2502";
static const string ML = "\u251C";
static const string MR = "\u2524";
static const string TM = "\u252C";
static const string BM = "\u2534";
static const string CR = "\u253C";
// bordure double pour le cadre principal
static const string DTL= "\u2554";
static const string DTR= "\u2557";
static const string DBL= "\u255A";
static const string DBR= "\u255D";
static const string DHZ= "\u2550";
static const string DVT= "\u2551";
static const string DML= "\u2560";
static const string DMR= "\u2563";

static const int BOX_WIDTH = 56;  // largeur de la boite

// ================================================================
//  fonctions utilitaires pour l'affichage
// ================================================================

// repete une string n fois
static string repeat(const string& s, int n) {
    string r;
    for (int i = 0; i < n; i++) r += s;
    return r;
}

// calcule la longueur visible d'un texte (sans les codes ANSI)
static int visibleLen(const string& s) {
    int len = 0;
    bool inEsc = false;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '\033') { inEsc = true; continue; }
        if (inEsc) { if (s[i] == 'm') inEsc = false; continue; }
        len++;
    }
    return len;
}

// padde une string a droite pour atteindre une largeur fixe
static string padRight(const string& s, int width) {
    int vl = visibleLen(s);
    if (vl >= width) return s;
    return s + string(width - vl, ' ');
}

// cree une ligne dans la boite avec le contenu centre
static string boxLine(const string& content, const string& color = "") {
    int vl = visibleLen(content);
    int padding = max(0, BOX_WIDTH - vl);
    int lpad = padding / 2;
    int rpad = padding - lpad;
    return DVT + " " + color + string(lpad, ' ') + content + string(rpad, ' ') + R + " " + DVT;
}

// ligne de boite aligne a gauche
static string boxLineL(const string& content, const string& colorCode = "") {
    int vl = visibleLen(content);
    int rpad = max(0, BOX_WIDTH - vl);
    return DVT + " " + colorCode + content + string(rpad, ' ') + R + " " + DVT;
}

// haut de la boite
static string boxTop() {
    return DTL + repeat(DHZ, BOX_WIDTH + 2) + DTR;
}
// bas de la boite
static string boxBot() {
    return DBL + repeat(DHZ, BOX_WIDTH + 2) + DBR;
}
// milieu de la boite (separateur)
static string boxMid() {
    return DML + repeat(DHZ, BOX_WIDTH + 2) + DMR;
}
// ligne vide dans la boite
static string boxEmpty() {
    return DVT + string(BOX_WIDTH + 2, ' ') + DVT;
}

// ================================================================
//  barre de HP (verte, jaune ou rouge selon le pourcentage)
// ================================================================
string Combat::makeHpBar(int hp, int hpMax, int barLen) const {
    if (hpMax <= 0) hpMax = 1;
    int filled = max(0, min(barLen, hp * barLen / hpMax));
    int empty  = barLen - filled;

    // couleur selon le pourcentage de vie
    string barColor;
    double pct = (double)hp / hpMax;
    if (pct > 0.5)      barColor = GRN;
    else if (pct > 0.25) barColor = YEL;
    else                 barColor = RED;

    string bar = barColor + "[";
    for (int i = 0; i < filled; i++) bar += "\u2588"; // bloc plein
    bar += DIM;
    for (int i = 0; i < empty;  i++) bar += "\u2591"; // bloc vide
    bar += R + barColor + "]" + R;
    return bar;
}

// ================================================================
//  lit les lignes ASCII du fichier du monstre
// ================================================================
vector<string> Combat::lireAsciiLines() const {
    vector<string> lines;
    string chemin = getAsciiFilePath();
    if (chemin.empty()) return lines;

    ifstream f(chemin);
    if (!f.is_open()) return lines;

    string ligne;
    while (getline(f, ligne)) {
        // enleve le \r si il y a (windows)
        if (!ligne.empty() && ligne.back() == '\r') ligne.pop_back();
        lines.push_back(ligne);
    }
    return lines;
}

// ================================================================
//  ecran principal de combat (style pokemon avec les barres HP)
// ================================================================
void Combat::afficherEcranCombat(const string& message) const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    string couleur = getCouleurMonstre();
    string catStr  = m_monster.getCategoryStr();

    // on lit l'art ASCII du monstre
    vector<string> ascii = lireAsciiLines();
    // on padde a 8 lignes minimum
    while ((int)ascii.size() < 8) ascii.push_back("");

    // haut de la boite
    cout << couleur << boxTop() << R << "\n";

    // nom + categorie du monstre
    string header = string(BOLD) + m_monster.getName() + R
                  + couleur + "  [" + catStr + "]";
    cout << couleur << boxLineL(header, couleur) << R << "\n";
    cout << couleur << boxMid() << R << "\n";

    // art ASCII a gauche + stats monstre a droite
    int hpMaxM = m_monster.getHpMax();
    int hpM    = m_monster.getHp();
    int mercyM = m_monster.getMercy();
    int mercyG = m_monster.getMercyGoal();
    string hpBarM    = makeHpBar(hpM,    hpMaxM, 16);
    string mercyBar  = makeHpBar(mercyM, mercyG,  16);

    for (int i = 0; i < 8; i++) {
        string art = (i < (int)ascii.size()) ? ascii[i] : "";
        int artVlen = visibleLen(art);
        string artPad = couleur + art + R + string(max(0, 24 - artVlen), ' ');

        string side = "";
        if (i == 1) side = string(WHI) + "HP  " + hpBarM  + " " + to_string(hpM)  + "/" + to_string(hpMaxM);
        if (i == 2) side = string(MAG) + "MERCY" + string(MAG) + " " + mercyBar + " " + to_string(mercyM) + "/" + to_string(mercyG);

        string row = artPad + "  " + side;
        cout << DVT << " " << couleur << padRight(row, BOX_WIDTH) << R << " " << DVT << "\n";
    }

    cout << couleur << boxMid() << R << "\n";

    // infos du joueur
    int hpJ    = m_player.getHp();
    int hpMaxJ = m_player.getHpMax();
    string hpBarJ = makeHpBar(hpJ, hpMaxJ, 16);
    string joueurLine = string(GRN) + m_player.getName() + R
                      + "  HP " + hpBarJ
                      + " " + GRN + to_string(hpJ) + "/" + to_string(hpMaxJ) + R;
    cout << boxLineL(joueurLine) << "\n";
    cout << couleur << boxMid() << R << "\n";

    // message central si il y en a un
    if (!message.empty()) {
        stringstream mss(message);
        string mline;
        while (getline(mss, mline)) {
            cout << boxLineL(mline) << "\n";
        }
        cout << couleur << boxMid() << R << "\n";
    }

    // menu FIGHT / ACT / ITEM / MERCY
    string menu =
        string(WHI) + "> " + RED  + "FIGHT" + R + "  " +
        CYA + "ACT" + R + "  " +
        YEL + "ITEM" + R + "  " +
        MAG + "MERCY" + R;
    cout << boxLineL(menu) << "\n";
    cout << couleur << boxBot() << R << "\n";
}

// ================================================================
//  apparition du monstre (style pokemon)
// ================================================================
void Combat::afficherApparitionPokemon() const {
    string couleur = getCouleurMonstre();
    vector<string> ascii = lireAsciiLines();

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout << "\n";
    cout << couleur << boxTop() << R << "\n";
    string msg = "  Un " + string(BOLD) + m_monster.getName() + R + couleur
               + " [" + m_monster.getCategoryStr() + "] sauvage apparait !";
    cout << couleur << boxLineL(msg, couleur) << R << "\n";
    cout << couleur << boxMid() << R << "\n";
    cout << boxEmpty() << "\n";

    for (int i = 0; i < (int)ascii.size(); i++) {
        cout << couleur << boxLineL("  " + ascii[i], couleur) << R << "\n";
    }

    cout << boxEmpty() << "\n";
    cout << couleur << boxBot() << R << "\n\n";

    Sleep(2500);
}

// ================================================================
//  constructuer
// ================================================================
Combat::Combat(Player& player, Monster monster)
    : m_player(player), m_monster(monster), m_result(CombatResult::PLAYER_DEAD) {}

// retounre la couleur ANSI selon la categorie du monstre
string Combat::getCouleurMonstre() const {
    if (m_monster.getCategory() == MonsterCategory::BOSS)     return RED;
    if (m_monster.getCategory() == MonsterCategory::MINIBOSS) return YEL;
    return CYA;  // normal
}

// retounre le chemin vers le fichier ASCII du monstre
string Combat::getAsciiFilePath() const {
    string nom = m_monster.getName();
    if (nom == "Garfield")       return "Data/garfield.txt";
    if (nom == "Bob l Eponge")   return "Data/bob-leponge.txt";
    if (nom == "Homer Simpson")  return "Data/homer-simpson.txt";
    if (nom == "Bugs Bunny")     return "Data/bugs-bunny.txt";
    if (nom == "Shrek")          return "Data/shrek.txt";
    if (nom == "Eric Cartman")   return "Data/eric-cartman.txt";
    return "";
}

// appelle l'afichage pokemon (garde pour compatibilite)
void Combat::afficherMonstreApparition() const {
    afficherApparitionPokemon();
}

// ================================================================
//  boucle principale du combat
// ================================================================
void Combat::run() {
    afficherMonstreApparition();

    // tant que les deux sont en vie, on continue
    while (m_player.isAlive() && m_monster.isAlive()) {
        afficherEcranCombat();

        bool continuer = tourJoueur();
        if (!continuer) break;

        // si le monstre est mort apres le tour du joueur
        if (!m_monster.isAlive()) {
            m_result = CombatResult::MONSTER_KILLED;
            Sleep(1500);
            break;
        }

        // tour du monstre
        bool joueurVivant = tourMonstre();
        Sleep(1500);

        if (!joueurVivant) {
            m_result = CombatResult::PLAYER_DEAD;
            break;
        }
    }

    // on determine le resultat final
    if (m_result != CombatResult::MONSTER_SPARED) {
        if (!m_player.isAlive())       m_result = CombatResult::PLAYER_DEAD;
        else if (!m_monster.isAlive()) m_result = CombatResult::MONSTER_KILLED;
    }

    // si le monstre est tue, on affiche l'ecran de victoire
    if (m_result == CombatResult::MONSTER_KILLED) {
        afficherVictoire();
    }
}

// ================================================================
//  tour du joueur : il choisit FIGHT, ACT, ITEM ou MERCY
// ================================================================
bool Combat::tourJoueur() {
    string couleur = getCouleurMonstre();
    int choix = 0;

    cout << "\n" << WHI << "  Que faites-vous ? " << R;
    cout << RED << "[1]FIGHT" << R << "  "
         << CYA << "[2]ACT"   << R << "  "
         << YEL << "[3]ITEM"  << R << "  "
         << MAG << "[4]MERCY" << R << "\n";
    cout << "  Choix : ";
    cin >> choix;

    switch (choix) {

    case 1: {
        // FIGHT : on attaque le monstre
        int dmg = calculerDegats(m_player.getAtk());
        m_monster.takeDamage(dmg);

        cout << GRN << "  Vous attaquez " << m_monster.getName()
             << " : -" << dmg << " HP !" << R << "\n";

        // si on attaque, la mercy diminue aussi
        int oldMercy = m_monster.getMercy();
        if (oldMercy > 0) {
            int penalty = dmg + (oldMercy / 5);
            m_monster.addMercy(-penalty);
            int drop = oldMercy - m_monster.getMercy();
            if (drop > 0)
                cout << YEL << "  Le monstre perd confiance... Mercy -" << drop << R << "\n";
        }

        // verifier si le monstre est mort
        if (!m_monster.isAlive()) {
            cout << GRN << "  " << m_monster.getName() << " s'effondre !" << R << "\n";
            m_result = CombatResult::MONSTER_KILLED;
            return false;
        }
        Sleep(800);
        break;
    }

    case 2: {
        // ACT : choisir une action pacifiste
        const vector<string>& actIds = m_monster.getActIds();
        vector<ActAction> dispo;
        ActCatalogue::displayAvailable(actIds, dispo);

        if (dispo.empty()) break;

        cout << "  " << GRY << "[0] Annuler" << R << "\n  Choix : ";
        int c;
        cin >> c;
        if (c == 0 || c < 1 || c > (int)dispo.size()) break;

        int impact = dispo[c - 1].getMercyImpact();
        m_monster.addMercy(impact);
        cout << CYA << "  " << dispo[c - 1].getDisplayText() << R << "\n";
        cout << "  Mercy : " << m_monster.getMercy() << "/" << m_monster.getMercyGoal();
        if (impact > 0)      cout << GRN << "  (+" << impact << ")" << R;
        else if (impact < 0) cout << RED << "  (" << impact << ")" << R;
        cout << "\n";
        Sleep(1500);
        break;
    }

    case 3: {
        // ITEM : utiliser un objet de l'inventaire
        Inventory& inv = m_player.getInventory();
        if (inv.isEmpty()) { cout << "  Sac vide !\n"; break; }
        if (!inv.hasUsableItems()) { cout << "  Tous les objets sont epuises !\n"; break; }

        cout << "\n  Inventaire :\n";
        inv.display();
        cout << "  " << GRY << "[0] Annuler" << R << "\n  Choix : ";
        int c;
        cin >> c;
        if (c == 0 || c < 1 || c > inv.size()) break;

        if (inv.useItem(c - 1, m_player))
            cout << GRN << "  Effet applique !" << R << "\n";
        else
            cout << YEL << "  Objet epuise !" << R << "\n";
        Sleep(1500);
        break;
    }

    case 4: {
        // MERCY : epargner le monstre si la jauge est pleine
        if (m_monster.isMercyFull()) {
            cout << MAG << "  Vous eprouvez de la pitie...\n";
            cout << "  " << m_monster.getName() << " s'en va. (Epargne)" << R << "\n";
            m_result = CombatResult::MONSTER_SPARED;
            return false;
        }
        else {
            cout << YEL << "  " << m_monster.getName() << " n'est pas encore apaise !\n";
            cout << "  Mercy : " << m_monster.getMercy() << "/" << m_monster.getMercyGoal()
                 << "  -- Utilisez ACT." << R << "\n";
        }
        break;
    }

    default:
        cout << RED << "  Choix invalide, tour perdu." << R << "\n";
    }

    return true;
}

// ================================================================
//  tour du monstre : il attaque le joueur
// ================================================================
bool Combat::tourMonstre() {
    int dmg = calculerDegats(m_monster.getAtk());
    m_player.takeDamage(dmg);
    cout << RED << "\n  " << m_monster.getName() << " vous attaque : -" << dmg << " HP !" << R << "\n";
    cout << "  Vos HP : " << m_player.getHp() << "/" << m_player.getHpMax() << "\n";

    if (!m_player.isAlive()) {
        cout << RED << "  Vous etes a terre..." << R << "\n";
        return false;
    }
    return true;
}

// ================================================================
//  calcul des degats : aleatoire entre 1 et atk
// ================================================================
int Combat::calculerDegats(int atk) {
    if (atk <= 0) return 1;
    return rand() % atk + 1;
}

// ================================================================
//  affichage du statut (anciene methode gardee au cas ou)
// ================================================================
void Combat::afficherStatut() const {
    string couleur = getCouleurMonstre();
    cout << "\n  " << GRN << m_player.getName() << R
         << " : " << m_player.getHp() << "/" << m_player.getHpMax() << " HP\n";
    cout << "  " << couleur << m_monster.getName() << R
         << " : " << m_monster.getHp() << "/" << m_monster.getHpMax() << " HP"
         << "  |  Mercy : " << m_monster.getMercy() << "/" << m_monster.getMercyGoal() << "\n";
}

void Combat::afficherSeparateur() const {
    cout << "\n" << WHI << "  -----------------------------" << R << "\n";
}

// ================================================================
//  ecran de victoire (quand le monstre est tue)
// ================================================================
void Combat::afficherVictoire() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    string couleur = getCouleurMonstre();

    cout << "\n";
    cout << MAG << boxTop() << R << "\n";
    cout << MAG << boxEmpty() << R << "\n";
    cout << MAG << boxLine(string(YEL) + string(BOLD) + "\u2605  VICTOIRE !  \u2605" + R, "") << R << "\n";
    cout << MAG << boxEmpty() << R << "\n";
    cout << MAG << boxMid()   << R << "\n";

    string vaincu = string(couleur) + m_monster.getName() + R;
    cout << boxLineL("  Vous avez vaincu " + vaincu + " !") << "\n";
    cout << MAG << boxMid() << R << "\n";
    cout << boxLineL(string(WHI) + "  Statistiques du vaincu :" + R) << "\n";
    cout << boxLineL("    HP Max  : " + to_string(m_monster.getHpMax())) << "\n";
    cout << boxLineL("    ATK    : " + to_string(m_monster.getAtk()))    << "\n";
    cout << boxLineL("    DEF    : " + to_string(m_monster.getDef()))    << "\n";
    cout << MAG << boxMid() << R << "\n";
    cout << boxLineL(string(CYA) + "  Retour au voyage..." + R) << "\n";
    cout << MAG << boxBot() << R << "\n\n";

    Sleep(2500);
}
