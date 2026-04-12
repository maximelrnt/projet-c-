#include "SaveManager.h"
#include "../items/HealItem.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// enleve les espaces au debut et a la fin
string SaveManager::trim(const string& s) {
    int start = (int)s.find_first_not_of(" \t\r\n");
    if (start == (int)string::npos) return "";
    int end = (int)s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// sauvegarde toute la partie dans un fichier
bool SaveManager::saveGame(const string& filename, const Player& player, const Bestiary& bestiary) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    // on ecrit l'entete
    file << "# ALTERDUNE SAVE FILE v1\n";
    file << "PLAYER_NAME=" << player.getName() << "\n";
    file << "PLAYER_HP=" << player.getHp() << "\n";
    file << "PLAYER_HP_MAX=" << player.getHpMax() << "\n";
    file << "PLAYER_ATK=" << player.getAtk() << "\n";
    file << "PLAYER_DEF=" << player.getDef() << "\n";
    file << "VICTORIES=" << player.getVictories() << "\n";
    file << "KILLS=" << player.getKills() << "\n";
    file << "SPARED=" << player.getSpared() << "\n";

    // on serialise l'inventaire (items separes par |, champs par ;)
    file << "INVENTORY=";
    const auto& items = player.getInventory().getItems();
    for (int i = 0; i < (int)items.size(); i++) {
        file << items[i]->getName() << ";" << items[i]->getType() << ";"
             << items[i]->getValue() << ";" << items[i]->getQuantity();
        if (i < (int)items.size() - 1) file << "|";
    }
    file << "\n";

    // on serialise le bestiaire
    file << "BESTIARY=";
    const auto& entries = bestiary.getEntries();
    for (int i = 0; i < (int)entries.size(); i++) {
        file << entries[i].m_monsterName << ";" << entries[i].m_category << ";"
             << entries[i].m_hp << ";" << entries[i].m_atk << ";"
             << entries[i].m_def << ";" << (entries[i].m_wasKilled ? "KILLED" : "SPARED");
        if (i < (int)entries.size() - 1) file << "|";
    }
    file << "\n";

    file.close();
    return true;
}

// charge une partie depuis un fichier de sauvegarde
bool SaveManager::loadGame(const string& filename, Player& player, Bestiary& bestiary) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line);  // on lit le header

    // on verifie que c'est bien un fichier de sauvegard alterdune
    if (line != "# ALTERDUNE SAVE FILE v1") {
        cout << "[SaveManager] Format de sauvegarde invalide." << endl;
        return false;
    }

    int hp = 0, hpMax = 0, atk = 0, def = 0;

    // on lit ligne par ligne
    while (getline(file, line)) {
        if (line.empty()) continue;

        // on separe la cle et la valeur sur '='
        size_t pos = line.find('=');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos);
        string val = line.substr(pos + 1);
        val = trim(val);

        // on rempli les donnees du joueur
        if (key == "PLAYER_NAME") player.setName(val);
        else if (key == "PLAYER_HP") hp = stoi(val);
        else if (key == "PLAYER_HP_MAX") hpMax = stoi(val);
        else if (key == "PLAYER_ATK") atk = stoi(val);
        else if (key == "PLAYER_DEF") def = stoi(val);
        else if (key == "VICTORIES") player.setVictories(stoi(val));
        else if (key == "KILLS") player.setKills(stoi(val));
        else if (key == "SPARED") player.setSpared(stoi(val));

        else if (key == "INVENTORY") {
            // on vide l'inventaire actuel et on le recharge
            Inventory& inv = player.getInventory();
            for (int i = 0; i < (int)inv.getItems().size(); i++) {
                delete inv.getItems()[i];
            }
            inv.getItemsRaw().clear();

            if (!val.empty()) {
                stringstream ss(val);
                string itemBloc;
                while (getline(ss, itemBloc, '|')) {
                    stringstream sBloc(itemBloc);
                    string iName, iType, iVal, iQte;
                    getline(sBloc, iName, ';');
                    getline(sBloc, iType, ';');
                    getline(sBloc, iVal, ';');
                    getline(sBloc, iQte, ';');

                    if (iType == "HEAL") {
                        inv.addItem(new HealItem(iName, stoi(iVal), stoi(iQte)));
                    }
                }
            }
        }

        else if (key == "BESTIARY") {
            // on vide le bestiaire et on le recharge
            bestiary.clear();
            if (!val.empty()) {
                stringstream ss(val);
                string bBloc;
                while (getline(ss, bBloc, '|')) {
                    stringstream sBloc(bBloc);
                    string bName, bCat, bHp, bAtk, bDef, bState;
                    getline(sBloc, bName, ';');
                    getline(sBloc, bCat, ';');
                    getline(sBloc, bHp, ';');
                    getline(sBloc, bAtk, ';');
                    getline(sBloc, bDef, ';');
                    getline(sBloc, bState, ';');

                    bool killed = (bState == "KILLED");
                    bestiary.addRawEntry(bName, bCat, stoi(bHp), stoi(bAtk), stoi(bDef), killed);
                }
            }
        }
    }

    // on applique les stats
    player.loadStateHp(hp, hpMax);
    player.setStats(atk, def);

    file.close();
    return true;
}
