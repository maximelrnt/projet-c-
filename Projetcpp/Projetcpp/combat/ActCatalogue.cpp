#include "ActCatalogue.h"
#include <iostream>

using namespace std;

// definition de la variable statique
map<string, ActAction> ActCatalogue::s_actions;

// on remplit le catalogue avec toutes les actions du jeu
void ActCatalogue::init() {
    s_actions.clear();

    // --- actions generiques ---
    s_actions.insert({"FLATTER",    ActAction("FLATTER",    "Tu flattes le monstre avec eloquence.",       +20)});
    s_actions.insert({"CHANTER",    ActAction("CHANTER",    "Tu chantes une melodie apaisante.",           +25)});
    s_actions.insert({"DANSER",     ActAction("DANSER",     "Tu executes une danse bizarre.",               +15)});
    s_actions.insert({"ENCOURAGER", ActAction("ENCOURAGER", "Tu encourages le monstre.",                    +10)});
    s_actions.insert({"INTERROGER", ActAction("INTERROGER", "Tu poses des questions sur sa vie.",            +5)});
    s_actions.insert({"REGARDER",   ActAction("REGARDER",   "Tu observes le monstre en silence.",             0)});
    s_actions.insert({"INSULTER",   ActAction("INSULTER",   "Tu insultes sauvagement le monstre !",         -20)});
    s_actions.insert({"PROVOQUER",  ActAction("PROVOQUER",  "Tu provoques le monstre avec arrogance.",      -30)});
    s_actions.insert({"MIMER",      ActAction("MIMER",      "Tu mimes ses attaques de facon comique.",      +10)});
    s_actions.insert({"PLEURER",    ActAction("PLEURER",    "Tu fonds en larmes devant le monstre.",         +5)});

    // --- Garfield (NORMAL) ---
    s_actions.insert({"DORMIR",         ActAction("DORMIR",         "Tu t'endors a cote de Garfield... il apprecie.",     +30)});
    s_actions.insert({"MANGER_LASAGNE", ActAction("MANGER_LASAGNE", "Tu lui offres des lasagnes. Son regard s'illumine !",+40)});

    // --- Bob l'Eponge (NORMAL) ---
    s_actions.insert({"ABSORBER", ActAction("ABSORBER", "Tu absorbes la situation comme Bob absorbe tout !",  +20)});
    s_actions.insert({"RIRE",     ActAction("RIRE",     "Tu ris avec lui. Son rire contagieux detend l'air.", +25)});

    // --- Homer Simpson (MINIBOSS) ---
    s_actions.insert({"ETRANGLER",    ActAction("ETRANGLER",    "Tu fais semblant de l'etrangler... il rigole ?",   -10)});
    s_actions.insert({"BEURP",        ActAction("BEURP",        "Tu laches un rot tonitruant. Homer est impressionne.", +15)});
    s_actions.insert({"D_OH",         ActAction("D_OH",         "Tu cries D'OH ! Homer se sent compris.",            +20)});
    s_actions.insert({"MANGER_DONUT", ActAction("MANGER_DONUT", "Tu manges un donut sous ses yeux. Il est jaloux.",   -5)});

    // --- Bugs Bunny (MINIBOSS) ---
    s_actions.insert({"BLAGUER",   ActAction("BLAGUER",   "Tu lui racontes une blague. Bugs rit de bon coeur.",   +25)});
    s_actions.insert({"DEGUISER",  ActAction("DEGUISER",  "Tu te deguises en canard. Bugs applaudit le show.",    +30)});
    s_actions.insert({"GRIGNOTER", ActAction("GRIGNOTER", "Tu grignottes une carotte lentement. Il est perplexe.", +10)});

    // --- Shrek (BOSS) ---
    s_actions.insert({"HURLER_MARAIS",  ActAction("HURLER_MARAIS",  "Tu hurles 'C'EST MON MARAI !' Shrek est touche.",     +20)});
    s_actions.insert({"FAIRE_PROUT",    ActAction("FAIRE_PROUT",    "Un prout magistral. Shrek acquiesce avec respect.",   +35)});
    s_actions.insert({"CROQUER_OIGNON", ActAction("CROQUER_OIGNON", "Tu croquen un oignon cru. Shrek est emu.",            +40)});
    s_actions.insert({"INTIMIDER",      ActAction("INTIMIDER",      "Tu essaies d'intimider Shrek. Il ricane.",             -15)});

    // --- Eric Cartman (BOSS) ---
    s_actions.insert({"RAGER",       ActAction("RAGER",       "Tu piques une rage. Cartman se reconnait en toi.",   +15)});
    s_actions.insert({"PLEURNICHER", ActAction("PLEURNICHER", "Tu pleurnisches. Cartman est machiavelement attendri.", +20)});
    s_actions.insert({"MANIPULER",   ActAction("MANIPULER",   "Tu tentes de le manipuler. Il voit clair dans ton jeu.", -10)});
}

// cherche une action par son ID dans la map
const ActAction* ActCatalogue::getAction(const string& id) {
    auto it = s_actions.find(id);
    if (it != s_actions.end()) {
        return &(it->second);
    }
    return nullptr;  // pas trouve
}

// affiche les actions dispo pour un monstre et remplit le vector outAvailable
void ActCatalogue::displayAvailable(const vector<string>& ids, vector<ActAction>& outAvailable) {
    outAvailable.clear();

    // on parcourt les ids et on recupere les actions correspondantes
    for (int i = 0; i < (int)ids.size(); i++) {
        const ActAction* act = getAction(ids[i]);
        if (act != nullptr) {
            outAvailable.push_back(*act);
        }
    }

    if (outAvailable.empty()) {
        cout << "  Aucune action disponible." << endl;
        return;
    }

    // on affiche la liste numerotee
    for (int i = 0; i < (int)outAvailable.size(); i++) {
        cout << "  [" << (i + 1) << "] " << outAvailable[i].getId() << "\n";
    }
}
