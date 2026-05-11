// Player.h — HERITAGE : Player <|-- Entity
//            COMPOSITION : Player *-- Inventory
#pragma once
#include "Entity.h"
#include "../items/Inventory.h"

using namespace std;

class Player : public Entity {
private:
    Inventory m_inventory;   // COMPOSITION : le joueur possede un inventaire
    int m_kills;
    int m_spared;
    int m_victories;

public:
    Player(const string& name, int hp, int atk, int def);
    ~Player() override;

    // Override Entity
    void   display() const override;
    string getType() const override { return "PLAYER"; }

    // Acces inventaire (composition)
    Inventory&       getInventory()       { return m_inventory; }
    const Inventory& getInventory() const { return m_inventory; }

    void addKill()   { m_kills++;  m_victories++; }
    void addSpared() { m_spared++; m_victories++; }

    int getKills()     const { return m_kills;     }
    int getSpared()    const { return m_spared;    }
    int getVictories() const { return m_victories; }

    // Setters sauvegarde
    void setKills(int k)                { m_kills = k;                }
    void setSpared(int s)               { m_spared = s;               }
    void setVictories(int v)            { m_victories = v;            }
    void setName(const string& n)       { m_name = n;                 }
    void loadStateHp(int hp, int maxHp) { m_hp = hp; m_hpMax = maxHp; }
    void setStats(int atk, int def)     { m_atk = atk; m_def = def;   }

    void displayStats() const;
    bool hasWon() const { return m_victories >= 10; }
};
