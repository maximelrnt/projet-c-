// ActCatalogue.h — Classe statique : COMPOSITION ActCatalogue *-- ActAction
#pragma once
#include "ActionAct.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

// Catalogue global de toutes les actions ACT (10+ actions, map statique)
class ActCatalogue {
private:
    static map<string, ActAction> s_actions;  // COMPOSITION : possede les ActAction

public:
    static void init();
    static const ActAction* getAction(const string& id);
    static void displayAvailable(const vector<string>& ids, vector<ActAction>& outAvailable);
};
