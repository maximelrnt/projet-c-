#include "Monster.h"
#include <iostream>

using namespace std;

Monster::Monster(string name, MonsterCategory cat, int hp, int atk, int def, int mercyGoal, vector<string> actIds)
    : Entity(name, hp, atk, def), m_category(cat), m_mercy(0), m_mercyGoal(mercyGoal), m_actIds(actIds) {}

void Monster::display() const {
    cout << m_name << " (" << getCategoryStr() << ") - HP : " << m_hp << "/" << m_hpMax 
         << " | Mercy : " << m_mercy << "/" << m_mercyGoal << endl;
}

string Monster::getCategoryStr() const {
    if (m_category == MonsterCategory::NORMAL) return "NORMAL";
    if (m_category == MonsterCategory::MINIBOSS) return "MINIBOSS";
    return "BOSS";
}

int Monster::getActCount() const {
    if (m_category == MonsterCategory::NORMAL) return 2;
    if (m_category == MonsterCategory::MINIBOSS) return 3;
    return 4;
}

Monster Monster::clone() const {
    // Retourne une nouvelle instance avec la jauge de Mercy et les HP ré-initialisés
    return Monster(m_name, m_category, m_hpMax, m_atk, m_def, m_mercyGoal, m_actIds);
}
