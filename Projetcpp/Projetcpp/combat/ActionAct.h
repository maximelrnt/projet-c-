// ActionAct.h — COMPOSITION dans ActCatalogue : ActCatalogue *-- ActAction
#pragma once
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class ActAction {
private:
    string m_id;
    string m_displayText;
    int    m_mercyImpact;

public:
    ActAction(const string& id, const string& text, int mercyImpact);

    const string& getId()          const { return m_id;          }
    const string& getDisplayText() const { return m_displayText; }

    // Impact mercy avec variance aleatoire
    int getMercyImpact() const {
        if (m_mercyImpact > 0) {
            int variance = (rand() % 31) - 10;
            int resultat = m_mercyImpact + variance;
            if (resultat < 5) resultat = 5;
            return resultat;
        }
        else if (m_mercyImpact < 0) {
            int variance = (rand() % 21);
            return m_mercyImpact - variance;
        }
        return 0;
    }

    void display() const;
};
