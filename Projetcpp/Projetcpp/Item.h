#pragma once
#include <string>

using namespace std;

class Item {
protected:
    string m_name;
    string m_type;
    int m_value;
    int m_quantity;

public:
    Item(string name, string type, int value, int quantity)
        : m_name(name), m_type(type), m_value(value), m_quantity(quantity) {}
    virtual ~Item() {}

    string getName() const { return m_name; }
    string getType() const { return m_type; }
    int getValue() const { return m_value; }
    int getQuantity() const { return m_quantity; }

    bool use() {
        if (m_quantity <= 0) return false;
        m_quantity--;
        return true;
    }

    virtual void display() const;
    
    // NB: L'énoncé veut que ceci soit abstrait, mais comme on ne crée
    // pas la classe HealItem, on laisse Item "normal" pour compiler.
};
