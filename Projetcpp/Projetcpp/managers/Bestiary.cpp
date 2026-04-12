#include "Bestiary.h"
#include <iostream>

using namespace std;

// affiche une entree du bestiaire
void BestiaryEntry::display() const {
    cout << "  [" << m_category << "] " << m_monsterName
         << " | HP:" << m_hp << " ATK:" << m_atk << " DEF:" << m_def
         << " | " << (m_wasKilled ? "TUE" : "EPARGNE") << endl;
}

// ajoute un monstre au bestiaire
void Bestiary::addEntry(const Monster& m, bool killed) {
    m_entries.push_back(BestiaryEntry(m, killed));
}

// ajoute une entree brute (utilise pour le chargement de sauvegarde)
void Bestiary::addRawEntry(string name, string cat, int hp, int atk, int def, bool killed) {
    m_entries.push_back(BestiaryEntry(name, cat, hp, atk, def, killed));
}

// affiche tout le bestiaire
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

// compte le nombre de monstres tues
int Bestiary::totalKilled() const {
    int count = 0;
    for (int i = 0; i < (int)m_entries.size(); i++) {
        if (m_entries[i].wasKilled()) count++;
    }
    return count;
}

// compte le nombre de monstres epargnes
int Bestiary::totalSpared() const {
    int count = 0;
    for (int i = 0; i < (int)m_entries.size(); i++) {
        if (!m_entries[i].wasKilled()) count++;
    }
    return count;
}
