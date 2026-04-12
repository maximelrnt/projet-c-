#pragma once
#include <string>

using namespace std;

// Entity : classe abstraite qui represente n'importe quelle entite du jeu
// c'est la classe mere de Player et Monster
class Entity {
protected:
    string m_name;
    int m_hp;
    int m_hpMax;
    int m_atk;
    int m_def;

public:
    // constructuer avec les stats de base
    Entity(string name, int hp, int atk, int def)
        : m_name(name), m_hp(hp), m_hpMax(hp), m_atk(atk), m_def(def) {}

    virtual ~Entity() {}

    // getters basiques
    string getName() const { return m_name; }
    int getHp() const { return m_hp; }
    int getHpMax() const { return m_hpMax; }
    int getAtk() const { return m_atk; }
    int getDef() const { return m_def; }

    // setter pour les hp, on borne entre 0 et max
    void setHp(int hp) {
        m_hp = hp;
        if (m_hp > m_hpMax) m_hp = m_hpMax;
        if (m_hp < 0) m_hp = 0;
    }

    // verifie si l'entite est encore en vie
    bool isAlive() const { return m_hp > 0; }

    // applique des degats et retounre le montant
    int takeDamage(int dmg) {
        m_hp -= dmg;
        if (m_hp < 0) m_hp = 0;
        return dmg;
    }

    // methodes virtuelles pure = chaque sous classe doit les implementer
    virtual void display() const = 0;
    virtual string getType() const = 0;
};
