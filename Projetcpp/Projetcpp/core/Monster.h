#pragma once
#include "Entity.h"
#include <vector>
#include <string>

using namespace std;

enum class MonsterCategory { NORMAL, MINIBOSS, BOSS };

class Monster : public Entity {
private:
    MonsterCategory m_category;
    int m_mercy;
    int m_mercyGoal;
    vector<string> m_actIds; // IDs au format texte (ex "FLATTER")

public:
    Monster(string name, MonsterCategory cat, int hp, int atk, int def, int mercyGoal, vector<string> actIds);

    virtual void display() const override;
    virtual string getType() const override { return "MONSTER"; }

    MonsterCategory getCategory() const { return m_category; }
    string getCategoryStr() const;

    int getMercy() const { return m_mercy; }
    int getMercyGoal() const { return m_mercyGoal; }

    void addMercy(int amount) {
        m_mercy += amount;
        if (m_mercy < 0) m_mercy = 0;
        if (m_mercy > m_mercyGoal) m_mercy = m_mercyGoal;
    }

    bool isMercyFull() const { return m_mercy >= m_mercyGoal; }

    const vector<string>& getActIds() const { return m_actIds; }
    int getActCount() const;

    Monster clone() const;
};
