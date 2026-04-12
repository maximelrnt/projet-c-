#pragma once
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// ActionAct : represente une action pacifiste (ACT) dans le jeu
// chaque action a un ID, un texte affiche et un impact sur la mercy
class ActAction {
private:
    string m_id;            // identifiant unique (ex: "FLATTER")
    string m_displayText;   // texte affiche au joueur
    int m_mercyImpact;      // combien ca change la mercy (+, -, ou 0)

public:
    // constructuer
    ActAction(string id, string text, int mercyImpact)
        : m_id(id), m_displayText(text), m_mercyImpact(mercyImpact) {}

    // getters
    string getId() const { return m_id; }
    string getDisplayText() const { return m_displayText; }

    // calcul de l'impact sur la mercy avec un peut de hasard
    int getMercyImpact() const {
        if (m_mercyImpact > 0) {
            // variance aleatoire entre -10 et +20
            int variance = (rand() % 31) - 10;
            int resultat = m_mercyImpact + variance;
            if (resultat < 5) resultat = 5;  // minimum 5
            return resultat;
        }
        else if (m_mercyImpact < 0) {
            // malus suplementaire aleatoire
            int variance = (rand() % 21);
            return m_mercyImpact - variance;
        }
        return 0;  // impact neutre
    }

    void display() const;
};
