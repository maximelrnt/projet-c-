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

vector<ActAction> ActAction::getCatalogue() {
    return {
        ActAction("FLATTER", "Tu flattes le monstre avec eloquence.", +20),
        ActAction("CHANTER", "Tu chantes une melodie apaisante.", +25),
        ActAction("DANSER", "Tu executes une danse bizarre.", +15),
        ActAction("ENCOURAGER", "Tu encourages le monstre.", +10),
        ActAction("INTERROGER", "Tu poses des questions sur sa vie.", +5),
        ActAction("REGARDER", "Tu observes le monstre en silence.", 0),
        ActAction("INSULTER", "Tu insultes sauvagement le monstre !", -20),
        ActAction("PROVOQUER", "Tu provoques le monstre avec arrogance.", -30),
        ActAction("MIMER", "Tu mimes ses attaques de facon comique.", +10),
        ActAction("PLEURER", "Tu fonds en larmes devant le monstre.", +5)
    };
}
