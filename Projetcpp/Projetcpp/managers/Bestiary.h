#pragma once
#include "../core/Monster.h"
#include <string>
#include <vector>

using namespace std;

// BestiaryEntry : une entree dans le bestiaire, garde les infos d'un monstre vaincu
struct BestiaryEntry {
    string m_monsterName;
    string m_category;
    int m_hp;
    int m_atk;
    int m_def;
    bool m_wasKilled;  // true = tue, false = epargne

    // constructuer depuis un monstre
    BestiaryEntry(const Monster& m, bool killed)
        : m_monsterName(m.getName()), m_category(m.getCategoryStr()),
          m_hp(m.getHpMax()), m_atk(m.getAtk()), m_def(m.getDef()), m_wasKilled(killed) {}

    // constructuer pour le chargement de sauvegarde
    BestiaryEntry(string name, string cat, int hp, int atk, int def, bool killed)
        : m_monsterName(name), m_category(cat), m_hp(hp), m_atk(atk), m_def(def), m_wasKilled(killed) {}

    void display() const;
    bool wasKilled() const { return m_wasKilled; }
};

// Bestiary : la liste de tous les monstres vaincus
class Bestiary {
private:
    vector<BestiaryEntry> m_entries;

public:
    // ajouter une entree depuis un monstre
    void addEntry(const Monster& m, bool killed);

    // ajouter une entree brute (pour la sauvegarde)
    void addRawEntry(string name, string cat, int hp, int atk, int def, bool killed);

    // afficher le bestiaire
    void display() const;

    // vider le bestiaire (pour le chargement)
    void clear() { m_entries.clear(); }

    // acces aux entrees
    const vector<BestiaryEntry>& getEntries() const { return m_entries; }

    // compter les tues et les epargnes
    int totalKilled() const;
    int totalSpared() const;
};
