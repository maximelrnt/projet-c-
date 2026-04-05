#include "Bestiary.h"
#include "Monster.h"
#include <iostream>

using namespace std;

void BestiaryEntry::display() const {
    cout << "  [" << m_category << "] " << m_monsterName
         << " | HP:" << m_hp << " ATK:" << m_atk << " DEF:" << m_def
         << " | " << (m_wasKilled ? "TUE" : "EPARGNE") << endl;
}

void Bestiary::addEntry(const Monster& m, bool killed) {
    m_entries.push_back(BestiaryEntry(m, killed));
}

void Bestiary::addRawEntry(string name, string cat, int hp, int atk, int def, bool killed) {
    m_entries.push_back(BestiaryEntry(name, cat, hp, atk, def, killed));
}

void Bestiary::display() const {
    if (m_entries.empty()) {
        cout << "  Aucun monstre rencontre pour l'instant." << endl;
        return;
    }
    cout << "=== BESTIAIRE ===" << endl;
    for (const auto& e : m_entries) {
        e.display();
    }
}

int Bestiary::totalKilled() const {
    int count = 0;
    for (const auto& e : m_entries) {
        if (e.wasKilled()) count++;
    }
    return count;
}

int Bestiary::totalSpared() const {
    int count = 0;
    for (const auto& e : m_entries) {
        if (!e.wasKilled()) count++;
    }
    return count;
}
