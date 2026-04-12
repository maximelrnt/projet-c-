#pragma once
#include "../core/Monster.h"
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

    // Constructeur pour le chargement brut
    BestiaryEntry(string name, string cat, int hp, int atk, int def, bool killed)
        : m_monsterName(name), m_category(cat), m_hp(hp), m_atk(atk), m_def(def), m_wasKilled(killed) {}

    void display() const;
    bool wasKilled() const { return m_wasKilled; }
};

class Bestiary {
private:
    vector<BestiaryEntry> m_entries;

public:
    void addEntry(const Monster& m, bool killed);
    void addRawEntry(string name, string cat, int hp, int atk, int def, bool killed);
    void display() const;
    void clear() { m_entries.clear(); }

    const vector<BestiaryEntry>& getEntries() const { return m_entries; }

    int totalKilled() const;
    int totalSpared() const;
};
