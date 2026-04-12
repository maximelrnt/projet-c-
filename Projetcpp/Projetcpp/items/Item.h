#pragma once
#include <string>

using namespace std;

class Player;  // forward declaration pour eviter les inclusions circulaires

// Item : classe abstraite pour les objets du jeu
// chaque type d'objet (soin, etc) doit implementer applyEffect()
class Item {
protected:
    string m_name;
    string m_type;     // le type d'objet genre "HEAL"
    int m_value;       // la valeur de l'effet (hp soignes par ex)
    int m_quantity;    // combien il en reste

public:
    // constructuer
    Item(string name, string type, int value, int quantity)
        : m_name(name), m_type(type), m_value(value), m_quantity(quantity) {}

    virtual ~Item() {}

    // getters
    string getName() const { return m_name; }
    string getType() const { return m_type; }
    int getValue() const { return m_value; }
    int getQuantity() const { return m_quantity; }

    // utilise un objet (diminue la quantite de 1)
    bool use() {
        if (m_quantity <= 0) return false;
        m_quantity--;
        return true;
    }

    // methode virtuelle pure : chaque sous-classe definit son effet
    virtual void applyEffect(Player& player) = 0;

    // affichage de base
    virtual void display() const;
};
