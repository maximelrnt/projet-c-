#pragma once
#include <string>

using namespace std;

class Entity {
protected:
    string m_name;
    int m_hp;
    int m_hpMax;
    int m_atk;
    int m_def;

public:
    Entity(string name, int hp, int atk, int def) 
        : m_name(name), m_hp(hp), m_hpMax(hp), m_atk(atk), m_def(def) {}
    virtual ~Entity() {}

    // setter et getter
    string getName() const { return m_name; }
    int getHp() const { return m_hp; }
    int getHpMax() const { return m_hpMax; }
    int getAtk() const { return m_atk; }
    int getDef() const { return m_def; }

    void setHp(int hp) {
        m_hp = hp;
        if (m_hp > m_hpMax) m_hp = m_hpMax;
        if (m_hp < 0) m_hp = 0;
    }

    bool isAlive() const { return m_hp > 0; }

    int takeDamage(int dmg) {
        m_hp -= dmg;
        if (m_hp < 0) m_hp = 0;
        return dmg;
    }

    // Virtuelles
    virtual void display() const = 0;
    virtual string getType() const = 0;     
};
