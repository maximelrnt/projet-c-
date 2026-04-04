#pragma once
#include <string>
#include <vector>

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
    int getMercyImpact() const { return m_mercyImpact; }

    void display() const;

    // Cette fonction simule l'ActCatalogue en attendant sa vraie création
    static vector<ActAction> getCatalogue();
};
