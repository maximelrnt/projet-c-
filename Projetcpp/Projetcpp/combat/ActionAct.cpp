// ActionAct.cpp
#include "ActionAct.h"
#include <iostream>

using namespace std;

ActAction::ActAction(const string& id, const string& text, int mercyImpact) {
    m_id          = id;
    m_displayText = text;
    m_mercyImpact = mercyImpact;
}

void ActAction::display() const {
    cout << "  [" << m_id << "] " << m_displayText;
    if (m_mercyImpact > 0)
        cout << "  (+Mercy)";
    else if (m_mercyImpact < 0)
        cout << "  (-Mercy)";
    cout << endl;
}
