#pragma once
#include <string>

using namespace std;

// Forward declaration pour eviter l'inclusion circulaire avec Player
class Player;

/*
 * Item : Classe abstraite de base pour tous les objets de l'inventaire.
 * Elle definit les proprietes communes (nom, type, valeur, quantite) et
 * oblige les sous-classes a implementer applyEffect() pour definir leur effet.
 */
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

    // Chaque sous-classe doit definir l'effet applique au joueur
    virtual void applyEffect(Player& player) = 0;
    virtual void display() const;
};
