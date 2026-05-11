// Player.cpp — Implementation de Player (herite de Entity)
#include "Player.h"
#include <iostream>

using namespace std;

// Constructeur : appelle Entity puis init les compteurs a 0
// m_inventory est construit automatiquement (COMPOSITION)
Player::Player(const string& name, int hp, int atk, int def)
    : Entity(name, hp, atk, def)
{
    m_kills     = 0;
    m_spared    = 0;
    m_victories = 0;
    // m_inventory est construit automatiquement par son constructeur par defaut (COMPOSITION)
}

// Le destructeur d'Inventory libere les Item* (COMPOSITION)
Player::~Player() = default;

void Player::display() const {
    cout << m_name << " (PLAYER) - HP : " << m_hp << "/" << m_hpMax << endl;
}

void Player::displayStats() const {
    cout << "--- STATISTIQUES DE " << m_name << " ---" << endl;
    cout << "  HP : " << m_hp << " / " << m_hpMax << endl;
    cout << "  ATK : " << m_atk << " | DEF : " << m_def << endl;
    cout << "  Victoires totales : " << m_victories << endl;
    cout << "  Monstres Tues : " << m_kills << " | Epargnes : " << m_spared << endl;
}
