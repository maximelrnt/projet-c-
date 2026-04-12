#pragma once
#include "Entity.h"
#include "../items/Inventory.h"

using namespace std;

// Player : le joueur du jeu, herite de Entity
// il a un inventaire et des compteurs de kills/spared/victoires
class Player : public Entity {
private:
    Inventory m_inventory;  // composition : le joueur possede un inventaire
    int m_kills;
    int m_spared;
    int m_victories;

public:
    Player(string name, int hp, int atk, int def);
    ~Player();

    // override des methodes virtuelles de Entity
    void display() const override;
    string getType() const override { return "PLAYER"; }

    // acces a l'inventaire
    Inventory& getInventory() { return m_inventory; }
    const Inventory& getInventory() const { return m_inventory; }

    // ajouter un kill ou un spare (ca ajoute aussi une victoire)
    void addKill() { m_kills++; m_victories++; }
    void addSpared() { m_spared++; m_victories++; }

    // getters
    int getKills() const { return m_kills; }
    int getSpared() const { return m_spared; }
    int getVictories() const { return m_victories; }

    // setters pour le chargement de sauvegarde
    void setKills(int k) { m_kills = k; }
    void setSpared(int s) { m_spared = s; }
    void setVictories(int v) { m_victories = v; }
    void setName(const string& n) { m_name = n; }
    void loadStateHp(int hp, int maxHp) { m_hp = hp; m_hpMax = maxHp; }
    void setStats(int atk, int def) { m_atk = atk; m_def = def; }

    // affiche les stats completes du joueur
    void displayStats() const;

    // verifi si le joueur a gagne (10 victores)
    bool hasWon() const { return m_victories >= 10; }
};
