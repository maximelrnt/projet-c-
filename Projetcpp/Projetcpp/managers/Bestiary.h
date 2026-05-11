// Bestiary.h — COMPOSITION : Bestiary *-- BestiaryEntry
#pragma once
#include "../core/Monster.h"
#include <string>
#include <vector>

using namespace std;

// Struct simple pour stocker les infos d'un monstre vaincu
struct BestiaryEntry {
    string m_monsterName;
    string m_category;
    int    m_hp;
    int    m_atk;
    int    m_def;
    bool   m_wasKilled;

    // Depuis un Monster
    BestiaryEntry(const Monster& m, bool killed) {
        m_monsterName = m.getName();
        m_category    = m.getCategoryStr();
        m_hp          = m.getHpMax();
        m_atk         = m.getAtk();
        m_def         = m.getDef();
        m_wasKilled   = killed;
    }

    // Depuis des donnees brutes (chargement sauvegarde)
    BestiaryEntry(const string& name, const string& cat, int hp, int atk, int def, bool killed) {
        m_monsterName = name;
        m_category    = cat;
        m_hp          = hp;
        m_atk         = atk;
        m_def         = def;
        m_wasKilled   = killed;
    }

    void display() const;
    bool wasKilled() const { return m_wasKilled; }
};

// Collection de BestiaryEntry (COMPOSITION)
class Bestiary {
private:
    vector<BestiaryEntry> m_entries;  // COMPOSITION : possede les entrees

public:
    Bestiary();
    ~Bestiary();

    void addEntry(const Monster& m, bool killed);
    void addRawEntry(const string& name, const string& cat, int hp, int atk, int def, bool killed);
    void display() const;
    void clear() { m_entries.clear(); }

    const vector<BestiaryEntry>& getEntries() const { return m_entries; }

    int totalKilled() const;
    int totalSpared() const;
};
