#pragma once
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class ActAction {
private:
    string m_id;
    string m_displayText;
    int m_mercyImpact;

public:
    ActAction(string id, string text, int mercyImpact) 
        : m_id(id), m_displayText(text), m_mercyImpact(mercyImpact) {}

    string getId() const { return m_id; }
    string getDisplayText() const { return m_displayText; }
    
    int getMercyImpact() const { 
        if (m_mercyImpact > 0) {
            int variance = (rand() % 31) - 10; // de -10 a +20 variance
            int resultat = m_mercyImpact + variance;
            if (resultat < 5) resultat = 5; 
            return resultat;
        } else if (m_mercyImpact < 0) {
            int variance = (rand() % 21); // 0 a 20 de malus supplementaire
            return m_mercyImpact - variance;
        }
        return 0;
    }

    void display() const;

};
