#pragma once
#include "Monster.h"
#include <string>
#include <vector>

using namespace std;

// Représente une entrée dans le bestiaire.
struct BestiaryEntry {
    string m_monsterName;
    string m_category;
    int m_hp;
    int m_atk;
    int m_def;
    bool m_wasKilled;

    BestiaryEntry(const Monster& m, bool killed) 
        : m_monsterName(m.getName()), m_category(m.getCategoryStr()), 
          m_hp(m.getHpMax()), m_atk(m.getAtk()), m_def(m.getDef()), m_wasKilled(killed) {}

    void display() const;
    bool wasKilled() const { return m_wasKilled; }
};

class Bestiary {
private:
    vector<BestiaryEntry> m_entries;

public:
    void addEntry(const Monster& m, bool killed);
    void display() const;

    int totalKilled() const;
    int totalSpared() const;
};
