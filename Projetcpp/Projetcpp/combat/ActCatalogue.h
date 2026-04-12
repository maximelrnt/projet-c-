#pragma once
#include "ActionAct.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

// ActCatalogue : contient toutes les actions ACT du jeu
// c'est une classe statique, on l'instancie pas
class ActCatalogue {
private:
    static map<string, ActAction> s_actions;  // dictionnaire des actions

public:
    // initialise toutes les actions
    static void init();

    // recupere une action par son ID
    static const ActAction* getAction(const string& id);

    // affiche les actions disponibles pour un monstre
    static void displayAvailable(const vector<string>& ids, vector<ActAction>& outAvailable);
};
