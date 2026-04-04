#pragma once
#include "Entity.h"
#include "Item.h"
#include <vector>

using namespace std;

class Player : public Entity {
private:
    vector<Item*> m_inventory; // En respectant "sans créer de nouvelle classe", on utilise un vector plutôt que la classe Inventory pour le moment
    int m_kills;
    int m_spared;
    int m_victories;

public:
    Player(string name, int hp, int atk, int def);
    ~Player();

    virtual void display() const override;
    virtual string getType() const override { return "PLAYER"; }

    // On retourne le vector pour simuler l'inventaire
    vector<Item*>& getInventory() { return m_inventory; }

    void addKill() { m_kills++; m_victories++; }
    void addSpared() { m_spared++; m_victories++; }
    void addVictory() { m_victories++; }

    int getKills() const { return m_kills; }
    int getSpared() const { return m_spared; }
    int getVictories() const { return m_victories; }

    void displayStats() const;
    bool hasWon() const { return m_victories >= 10; }
};
