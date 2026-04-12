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
//  CODES ANSI
// ================================================================
static const char* R   = "\033[0m";         // Reset
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

// Caracteres boite (UTF-8 box-drawing)
static const string TL = "\u250C"; // ┌
static const string TR = "\u2510"; // ┐
static const string BL = "\u2514"; // └
static const string BR = "\u2518"; // ┘
static const string HZ = "\u2500"; // ─
static const string VT = "\u2502"; // │
static const string ML = "\u251C"; // ├
static const string MR = "\u2524"; // ┤
static const string TM = "\u252C"; // ┬
static const string BM = "\u2534"; // ┴
static const string CR = "\u253C"; // ┼
// Double bordure pour le cadre externe
static const string DTL= "\u2554"; // ╔
static const string DTR= "\u2557"; // ╗
static const string DBL= "\u255A"; // ╚
static const string DBR= "\u255D"; // ╝
static const string DHZ= "\u2550"; // ═
static const string DVT= "\u2551"; // ║
static const string DML= "\u2560"; // ╠
static const string DMR= "\u2563"; // ╣

static const int BOX_WIDTH = 56; // largeur interieure de la boite

// ================================================================
//  Helpers utilitaires
// ================================================================
static string repeat(const string& s, int n) {
    string r; for (int i = 0; i < n; i++) r += s; return r;
}

// Pad une string jusqu'a une largeur (en comptant uniquement les char visibles)
// Attention: les codes ANSI ne comptent pas comme caracteres visibles
static int visibleLen(const string& s) {
    int len = 0; bool inEsc = false;
    for (char c : s) {
        if (c == '\033') { inEsc = true; continue; }
        if (inEsc) { if (c == 'm') inEsc = false; continue; }
        len++;
    }
    return len;
}

static string padRight(const string& s, int width) {
    int vl = visibleLen(s);
    if (vl >= width) return s;
    return s + string(width - vl, ' ');
}

// Ligne de boite double avec contenu centre
static string boxLine(const string& content, const string& color = "") {
    string vis = content;
    int vl = visibleLen(content);
    int padding = max(0, BOX_WIDTH - vl);
    int lpad = padding / 2;
    int rpad = padding - lpad;
    return DVT + " " + color + string(lpad, ' ') + content + string(rpad, ' ') + R + " " + DVT;
}

// Ligne de boite double avec contenu aligne a gauche
static string boxLineL(const string& content, const string& colorCode = "") {
    int vl = visibleLen(content);
    int rpad = max(0, BOX_WIDTH - vl);
    return DVT + " " + colorCode + content + string(rpad, ' ') + R + " " + DVT;
}

static string boxTop() {
    return DTL + repeat(DHZ, BOX_WIDTH + 2) + DTR;
}
static string boxBot() {
    return DBL + repeat(DHZ, BOX_WIDTH + 2) + DBR;
}
static string boxMid() {
    return DML + repeat(DHZ, BOX_WIDTH + 2) + DMR;
}
static string boxEmpty() {
    return DVT + string(BOX_WIDTH + 2, ' ') + DVT;
}

// ================================================================
//  Construction de la barre HP
// ================================================================
string Combat::makeHpBar(int hp, int hpMax, int barLen) const {
    if (hpMax <= 0) hpMax = 1;
    int filled = max(0, min(barLen, hp * barLen / hpMax));
    int empty  = barLen - filled;

    // Couleur selon pourcentage
    string barColor;
    double pct = (double)hp / hpMax;
    if (pct > 0.5)      barColor = GRN;
    else if (pct > 0.25) barColor = YEL;
    else                 barColor = RED;

    string bar = barColor + "[";
    for (int i = 0; i < filled; i++) bar += "\u2588"; // █
    bar += DIM;
    for (int i = 0; i < empty;  i++) bar += "\u2591"; // ░
    bar += R + barColor + "]" + R;
    return bar;
}

// ================================================================
//  Lecture des lignes ASCII du monstre
// ================================================================
vector<string> Combat::lireAsciiLines() const {
    vector<string> lines;
    string chemin = getAsciiFilePath();
    if (chemin.empty()) return lines;
    ifstream f(chemin);
    if (!f.is_open()) return lines;
    string ligne;
    while (getline(f, ligne)) {
        // Enlever \r si present
        if (!ligne.empty() && ligne.back() == '\r') ligne.pop_back();
        lines.push_back(ligne);
    }
    return lines;
}

// ================================================================
//  Ecran principal de combat style Pokemon
// ================================================================
void Combat::afficherEcranCombat(const string& message) const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    string couleur = getCouleurMonstre();
    string catStr  = m_monster.getCategoryStr();

    // --- Lignes ASCII du monstre (max 8 lignes) ---
    vector<string> ascii = lireAsciiLines();
    // Padding a 8 lignes pour une hauteur fixe
    while ((int)ascii.size() < 8) ascii.push_back("");

    // ╔══ Header ══╗
    cout << couleur << boxTop() << R << "\n";

    // Nom + categorie du monstre
    string header = string(BOLD) + m_monster.getName() + R
                  + couleur + "  [" + catStr + "]";
    cout << couleur << boxLineL(header, couleur) << R << "\n";
    cout << couleur << boxMid() << R << "\n";

    // Art ASCII du monstre (cadre gauche) + Stats monstre a droite
    // On affiche max 8 lignes
    int hpMaxM = m_monster.getHpMax();
    int hpM    = m_monster.getHp();
    int mercyM = m_monster.getMercy();
    int mercyG = m_monster.getMercyGoal();
    string hpBarM    = makeHpBar(hpM,    hpMaxM, 16);
    string mercyBar  = makeHpBar(mercyM, mercyG,  16);

    for (int i = 0; i < 8; i++) {
        string art = (i < (int)ascii.size()) ? ascii[i] : "";
        // Largeur art = 24 chars, padded
        int artVlen = visibleLen(art);
        string artPad = couleur + art + R + string(max(0, 24 - artVlen), ' ');

        string side = "";
        if (i == 1) side = string(WHI) + "HP  " + hpBarM  + " " + to_string(hpM)  + "/" + to_string(hpMaxM);
        if (i == 2) side = string(MAG) + "MERCY" + string(MAG) + " " + mercyBar + " " + to_string(mercyM) + "/" + to_string(mercyG);

        string row = artPad + "  " + side;
        cout << DVT << " " << couleur << padRight(row, BOX_WIDTH) << R << " " << DVT << "\n";
    }

    cout << couleur << boxMid() << R << "\n";

    // --- Separateur : infos joueur ---
    int hpJ    = m_player.getHp();
    int hpMaxJ = m_player.getHpMax();
    string hpBarJ = makeHpBar(hpJ, hpMaxJ, 16);
    string joueurLine = string(GRN) + m_player.getName() + R
                      + "  HP " + hpBarJ
                      + " " + GRN + to_string(hpJ) + "/" + to_string(hpMaxJ) + R;
    cout << boxLineL(joueurLine) << "\n";
    cout << couleur << boxMid() << R << "\n";

    // --- Message central ---
    if (!message.empty()) {
        // Afficher le message ligne par ligne (peut contenir \n)
        stringstream mss(message);
        string mline;
        while (getline(mss, mline)) {
            cout << boxLineL(mline) << "\n";
        }
        cout << couleur << boxMid() << R << "\n";
    }

    // --- Menu FIGHT / ACT / ITEM / MERCY ---
    string menu =
        string(WHI) + "> " + RED  + "FIGHT" + R + "  " +
        CYA + "ACT" + R + "  " +
        YEL + "ITEM" + R + "  " +
        MAG + "MERCY" + R;
    cout << boxLineL(menu) << "\n";
    cout << couleur << boxBot() << R << "\n";
}

// ================================================================
//  Apparition du monstre (style Pokemon : intro cinematique)
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

    for (const string& line : ascii) {
        cout << couleur << boxLineL("  " + line, couleur) << R << "\n";
    }

    cout << boxEmpty() << "\n";
    cout << couleur << boxBot() << R << "\n\n";

    Sleep(2500);
}

// ================================================================
//  Constructeur
// ================================================================
Combat::Combat(Player& player, Monster monster)
    : m_player(player), m_monster(monster), m_result(CombatResult::PLAYER_DEAD) {}

// ================================================================
string Combat::getCouleurMonstre() const {
    if (m_monster.getCategory() == MonsterCategory::BOSS)     return RED;
    if (m_monster.getCategory() == MonsterCategory::MINIBOSS) return YEL;
    return CYA;
}

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

// Garde l'ancienne pour compatibilite (appelee dans run())
void Combat::afficherMonstreApparition() const {
    afficherApparitionPokemon();
}

// ================================================================
//  Boucle principale
// ================================================================
void Combat::run() {
    afficherMonstreApparition();

    while (m_player.isAlive() && m_monster.isAlive()) {
        afficherEcranCombat();

        bool continuer = tourJoueur();
        if (!continuer) break;

        if (!m_monster.isAlive()) {
            m_result = CombatResult::MONSTER_KILLED;
            Sleep(1500);
            break;
        }

        bool joueurVivant = tourMonstre();
        Sleep(1500);

        if (!joueurVivant) {
            m_result = CombatResult::PLAYER_DEAD;
            break;
        }
    }

    if (m_result != CombatResult::MONSTER_SPARED) {
        if (!m_player.isAlive())       m_result = CombatResult::PLAYER_DEAD;
        else if (!m_monster.isAlive()) m_result = CombatResult::MONSTER_KILLED;
    }

    if (m_result == CombatResult::MONSTER_KILLED) {
        afficherVictoire();
    }
}

// ================================================================
//  Tour du joueur
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

    case 1: { // FIGHT
        int dmg = calculerDegats(m_player.getAtk());
        m_monster.takeDamage(dmg);

        string msg = string(GRN) + "  Vous attaquez " + m_monster.getName()
                   + " : -" + to_string(dmg) + " HP !" + R;
        cout << msg << "\n";

        int oldMercy = m_monster.getMercy();
        if (oldMercy > 0) {
            int penalty = dmg + (oldMercy / 5);
            m_monster.addMercy(-penalty);
            int drop = oldMercy - m_monster.getMercy();
            if (drop > 0)
                cout << YEL << "  Le monstre perd confiance... Mercy -" << drop << R << "\n";
        }

        if (!m_monster.isAlive()) {
            cout << GRN << "  " << m_monster.getName() << " s'effondre !" << R << "\n";
            m_result = CombatResult::MONSTER_KILLED;
            return false;
        }
        Sleep(800);
        break;
    }

    case 2: { // ACT
        const vector<string>& actIds = m_monster.getActIds();
        vector<ActAction> dispo;
        ActCatalogue::displayAvailable(actIds, dispo);

        if (dispo.empty()) break;

        cout << "  " << GRY << "[0] Annuler" << R << "\n  Choix : ";
        int c; cin >> c;
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

    case 3: { // ITEM
        Inventory& inv = m_player.getInventory();
        if (inv.isEmpty()) { cout << "  Sac vide !\n"; break; }
        if (!inv.hasUsableItems()) { cout << "  Tous les objets sont epuises !\n"; break; }

        cout << "\n  Inventaire :\n";
        inv.display();
        cout << "  " << GRY << "[0] Annuler" << R << "\n  Choix : ";
        int c; cin >> c;
        if (c == 0 || c < 1 || c > inv.size()) break;

        if (inv.useItem(c - 1, m_player))
            cout << GRN << "  Effet applique !" << R << "\n";
        else
            cout << YEL << "  Objet epuise !" << R << "\n";
        Sleep(1500);
        break;
    }

    case 4: { // MERCY
        if (m_monster.isMercyFull()) {
            cout << MAG << "  Vous eprouvez de la pitie...\n";
            cout << "  " << m_monster.getName() << " s'en va. (Epargne)" << R << "\n";
            m_result = CombatResult::MONSTER_SPARED;
            return false;
        } else {
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
//  Tour du monstre
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
int Combat::calculerDegats(int atk) {
    if (atk <= 0) return 1;
    return rand() % atk + 1;
}

// ================================================================
//  Statut (ancienne methode conservee)
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
//  Ecran de victoire style Pokemon
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
    cout << MAG << boxLine(string(YEL) + string(BOLD) + "★  VICTOIRE !  ★" + R, "") << R << "\n";
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
