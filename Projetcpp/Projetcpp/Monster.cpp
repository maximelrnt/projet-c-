#include "Monster.h"
#include <iostream>

using namespace std;


Monster::Monster(string name, MonsterCategory cat, int hp, int atk, int def, int mercyGoal, vector<string> actIds)
    : Entity(name, hp, atk, def)
{
    this->m_category = cat;
    this->m_mercy = 0;
    this->m_mercyGoal = mercyGoal;
    this->m_actIds = actIds;
}

void Monster::display() const {
    
    cout << this->m_name << " (" << this->getCategoryStr() << ") - HP : " << this->m_hp << "/" << this->m_hpMax
        << " | Mercy : " << this->m_mercy << "/" << this->m_mercyGoal << endl;
}

string Monster::getCategoryStr() const {
    if (this->m_category == MonsterCategory::NORMAL) {
        return "NORMAL";
    }
    if (this->m_category == MonsterCategory::MINIBOSS) {
        return "MINIBOSS";
    }
    return "BOSS";
}

int Monster::getActCount() const {
    if (this->m_category == MonsterCategory::NORMAL) {
        return 2;
    }
    if (this->m_category == MonsterCategory::MINIBOSS) {
        return 3;
    }
    
    return 4;
}

Monster Monster::clone() const {
    
    return Monster(
        this->m_name,
        this->m_category,
        this->m_hpMax,
        this->m_atk,
        this->m_def,
        this->m_mercyGoal,
        this->m_actIds
    );
}