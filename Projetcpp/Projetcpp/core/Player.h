#pragma once
#include "Entity.h"
#include "../items/Inventory.h"
#include <vector>

using namespace std;

class Player : public Entity {
private:
    Inventory m_inventory;
    int m_kills;
    int m_spared;
    int m_victories;

public:
    Player(string name, int hp, int atk, int def);
    ~Player();

    virtual void display() const override;
    virtual string getType() const override { return "PLAYER"; }

    // Renvoie une référence vers l'objet Inventory encapsulé
    Inventory& getInventory() { return m_inventory; }
    const Inventory& getInventory() const { return m_inventory; }

    void addKill() { m_kills++; m_victories++; }
    void addSpared() { m_spared++; m_victories++; }
    void addVictory() { m_victories++; }

    int getKills() const { return m_kills; }
    int getSpared() const { return m_spared; }
    int getVictories() const { return m_victories; }

    void setKills(int k) { m_kills = k; }
    void setSpared(int s) { m_spared = s; }
    void setVictories(int v) { m_victories = v; }
    void setName(const string& n) { m_name = n; }
    void loadStateHp(int hp, int maxHp) { m_hp = hp; m_hpMax = maxHp; }
    void setStats(int atk, int def) { m_atk = atk; m_def = def; }

    void displayStats() const;
    bool hasWon() const { return m_victories >= 10; }
};
