#include "ActionAct.h"
#include <iostream>

using namespace std;

void ActAction::display() const {
    cout << "  [" << m_id << "] " << m_displayText;
    if (m_mercyImpact > 0)
        cout << "  (+Mercy)";
    else if (m_mercyImpact < 0)
        cout << "  (-Mercy)";
    cout << endl;
}


