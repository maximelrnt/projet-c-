// Bestiary.cpp
#include "Bestiary.h"
#include <iostream>

using namespace std;

Bestiary::Bestiary() {
    // m_entries est un vector vide par defaut
}

Bestiary::~Bestiary() {
    m_entries.clear();
}

void BestiaryEntry::display() const {
    cout << "  [" << m_category << "] " << m_monsterName
         << " | HP:" << m_hp << " ATK:" << m_atk << " DEF:" << m_def
         << " | " << (m_wasKilled ? "TUE" : "EPARGNE") << endl;
}

void Bestiary::addEntry(const Monster& m, bool killed) {
    m_entries.push_back(BestiaryEntry(m, killed));
}

void Bestiary::addRawEntry(const string& name, const string& cat, int hp, int atk, int def, bool killed) {
    m_entries.push_back(BestiaryEntry(name, cat, hp, atk, def, killed));
}

void Bestiary::display() const {
    if (m_entries.empty()) {
        cout << "  Aucun monstre rencontre pour l'instant." << endl;
        return;
    }
    cout << "=== BESTIAIRE ===" << endl;
    for (int i = 0; i < (int)m_entries.size(); i++) {
        m_entries[i].display();
    }
}

int Bestiary::totalKilled() const {
    int count = 0;
    for (int i = 0; i < (int)m_entries.size(); i++) {
        if (m_entries[i].wasKilled()) count++;
    }
    return count;
}

int Bestiary::totalSpared() const {
    int count = 0;
    for (int i = 0; i < (int)m_entries.size(); i++) {
        if (!m_entries[i].wasKilled()) count++;
    }
    return count;
}
