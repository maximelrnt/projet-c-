#pragma once
#include "ActionAct.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

/*
 * ActCatalogue : Classe utilitaire statique qui centralise toutes les
 * actions pacifistes (ACT) du jeu.
 * Gère une map pour rechercher n'importe quelle action par son identifiant unique.
 */
class ActCatalogue {
private:
    static map<string, ActAction> s_actions;

public:
    static void init();
    static const ActAction* getAction(const string& id);
    static void displayAvailable(const vector<string>& ids, vector<ActAction>& outAvailable);
};
