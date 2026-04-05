#include "SaveManager.h"
#include "HealItem.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string SaveManager::trim(const string& s) {
    int start = (int)s.find_first_not_of(" \t\r\n");
    if (start == (int)string::npos) return "";
    int end = (int)s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool SaveManager::saveGame(const string& filename, const Player& player, const Bestiary& bestiary) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "# ALTERDUNE SAVE FILE v1\n";
    file << "PLAYER_NAME=" << player.getName() << "\n";
    file << "PLAYER_HP=" << player.getHp() << "\n";
    file << "PLAYER_HP_MAX=" << player.getHpMax() << "\n";
    file << "PLAYER_ATK=" << player.getAtk() << "\n";
    file << "PLAYER_DEF=" << player.getDef() << "\n";
    file << "VICTORIES=" << player.getVictories() << "\n";
    file << "KILLS=" << player.getKills() << "\n";
    file << "SPARED=" << player.getSpared() << "\n";

    // Inventaire
    file << "INVENTORY=";
    const auto& items = player.getInventory().getItems();
    for (size_t i = 0; i < items.size(); ++i) {
        file << items[i]->getName() << ";" << items[i]->getType() << ";" 
             << items[i]->getValue() << ";" << items[i]->getQuantity();
        if (i < items.size() - 1) file << "|";
    }
    file << "\n";

    // Bestiaire
    file << "BESTIARY=";
    const auto& entries = bestiary.getEntries();
    for (size_t i = 0; i < entries.size(); ++i) {
        file << entries[i].m_monsterName << ";" << entries[i].m_category << ";"
             << entries[i].m_hp << ";" << entries[i].m_atk << ";" 
             << entries[i].m_def << ";" << (entries[i].m_wasKilled ? "KILLED" : "SPARED");
        if (i < entries.size() - 1) file << "|";
    }
    file << "\n";

    file.close();
    return true;
}

bool SaveManager::loadGame(const string& filename, Player& player, Bestiary& bestiary) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); // HEADER

    if (line != "# ALTERDUNE SAVE FILE v1") {
        cout << "[SaveManager] Format de sauvegarde invalide." << endl;
        return false;
    }

    int hp = 0, hpMax = 0, atk = 0, def = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t pos = line.find('=');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos);
        string val = line.substr(pos + 1);
        val = trim(val);

        if (key == "PLAYER_NAME") player.setName(val);
        else if (key == "PLAYER_HP") hp = stoi(val);
        else if (key == "PLAYER_HP_MAX") hpMax = stoi(val);
        else if (key == "PLAYER_ATK") atk = stoi(val);
        else if (key == "PLAYER_DEF") def = stoi(val);
        else if (key == "VICTORIES") player.setVictories(stoi(val));
        else if (key == "KILLS") player.setKills(stoi(val));
        else if (key == "SPARED") player.setSpared(stoi(val));
        else if (key == "INVENTORY") {
            // Nettoyage de l'inventaire actuel
            Inventory& inv = player.getInventory();
            for (auto item : inv.getItems()) delete item;
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

    // Appliquer stats globales
    player.loadStateHp(hp, hpMax);
    player.setStats(atk, def);

    file.close();
    return true;
}
