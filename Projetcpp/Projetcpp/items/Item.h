// Item.h — Classe abstraite, mere de HealItem (HERITAGE)
//          COMPOSITION : les Item* sont possedes par Inventory
#pragma once
#include <string>

using namespace std;

class Player;  // forward declaration

class Item {
protected:
    string m_name;
    string m_type;
    int    m_value;
    int    m_quantity;

public:
    Item(const string& name, const string& type, int value, int quantity) {
        m_name     = name;
        m_type     = type;
        m_value    = value;
        m_quantity = quantity;
    }

    virtual ~Item() = default;

    // Getters
    const string& getName() const { return m_name;     }
    const string& getType() const { return m_type;     }
    int getValue()          const { return m_value;    }
    int getQuantity()       const { return m_quantity; }

    bool use() {
        if (m_quantity <= 0) return false;
        m_quantity--;
        return true;
    }

    // Virtuelle pure → classe abstraite
    virtual void applyEffect(Player& player) = 0;

    virtual void display() const;
};
