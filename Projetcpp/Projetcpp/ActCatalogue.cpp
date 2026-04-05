#include "ActCatalogue.h"
#include <iostream>

using namespace std;

map<string, ActAction> ActCatalogue::s_actions;

void ActCatalogue::init() {
    s_actions.clear();
    s_actions.insert({"FLATTER", ActAction("FLATTER", "Tu flattes le monstre avec eloquence.", +20)});
    s_actions.insert({"CHANTER", ActAction("CHANTER", "Tu chantes une melodie apaisante.", +25)});
    s_actions.insert({"DANSER", ActAction("DANSER", "Tu executes une danse bizarre.", +15)});
    s_actions.insert({"ENCOURAGER", ActAction("ENCOURAGER", "Tu encourages le monstre.", +10)});
    s_actions.insert({"INTERROGER", ActAction("INTERROGER", "Tu poses des questions sur sa vie.", +5)});
    s_actions.insert({"REGARDER", ActAction("REGARDER", "Tu observes le monstre en silence.", 0)});
    s_actions.insert({"INSULTER", ActAction("INSULTER", "Tu insultes sauvagement le monstre !", -20)});
    s_actions.insert({"PROVOQUER", ActAction("PROVOQUER", "Tu provoques le monstre avec arrogance.", -30)});
    s_actions.insert({"MIMER", ActAction("MIMER", "Tu mimes ses attaques de facon comique.", +10)});
    s_actions.insert({"PLEURER", ActAction("PLEURER", "Tu fonds en larmes devant le monstre.", +5)});
}

const ActAction* ActCatalogue::getAction(const string& id) {
    auto it = s_actions.find(id);
    if (it != s_actions.end()) {
        return &(it->second);
    }
    return nullptr;
}

void ActCatalogue::displayAvailable(const vector<string>& ids, vector<ActAction>& outAvailable) {
    outAvailable.clear();
    for (const string& id : ids) {
        const ActAction* act = getAction(id);
        if (act != nullptr) {
            outAvailable.push_back(*act);
        }
    }
    
    if (outAvailable.empty()) {
        cout << "  Aucune action disponible." << endl;
        return;
    }

    for (int i = 0; i < (int)outAvailable.size(); i++) {
        cout << "  [" << (i + 1) << "] ";
        outAvailable[i].display();
    }
}
