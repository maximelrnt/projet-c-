#pragma once
#include "Entity.h"
#include <vector>
#include <string>

using namespace std;

// les differents types de monstres
enum class MonsterCategory { NORMAL, MINIBOSS, BOSS };

// Monster : un monstre du jeu, herite de Entity
// chaque monstre a une categorie, une jauge mercy et des actions ACT
class Monster : public Entity {
private:
    MonsterCategory m_category;
    int m_mercy;       // jauge de mercy actuelle
    int m_mercyGoal;   // seuil pour pouvoir epargner
    vector<string> m_actIds;  // les IDs des actions (ex: "FLATTER")

public:
    Monster(string name, MonsterCategory cat, int hp, int atk, int def, int mercyGoal, vector<string> actIds);

    // methodes virtuelles overridees
    void display() const override;
    string getType() const override { return "MONSTER"; }

    // getters
    MonsterCategory getCategory() const { return m_category; }
    string getCategoryStr() const;
    int getMercy() const { return m_mercy; }
    int getMercyGoal() const { return m_mercyGoal; }

    // ajoute de la mercy, borne entre 0 et mercyGoal
    void addMercy(int amount) {
        m_mercy += amount;
        if (m_mercy < 0) m_mercy = 0;
        if (m_mercy > m_mercyGoal) m_mercy = m_mercyGoal;
    }

    // verifi si la mercy est pleine
    bool isMercyFull() const { return m_mercy >= m_mercyGoal; }

    // recuperer les actions disponibles
    const vector<string>& getActIds() const { return m_actIds; }
    int getActCount() const;

    // retounre une copie fraiche du monstre (hp remis a max)
    Monster clone() const;
};
