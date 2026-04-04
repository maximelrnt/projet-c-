// =============================================================================
// ActionAct.h
//
// Représente une action non-violente que le joueur peut choisir pendant un combat,
// à la place d'attaquer. Chaque action a un identifiant unique (id), un texte
// affiché au joueur (description) et un impact sur la jauge de Mercy du monstre
// (impactMercy : positif pour amadouer, négatif pour provoquer, 0 pour neutre).
//
// Le catalogue complet des actions disponibles est défini directement dans
// ActionAct.cpp via la fonction statique getCatalogue(). Il contient au minimum
// 8 actions différentes, dont au moins 2 avec un impact négatif.
// =============================================================================
#pragma once
#include <string>
#include <vector>

using namespace std;

class ActionAct {
private:
    int id;
    string description;
    int impactMercy;  // Valeur ajoutée à la jauge Mercy (peut être négative)

public:
    // Constructeur
    ActionAct(int id, const string& description, int impactMercy);

    // Getters
    int getId() const;
    string getDescription() const;
    int getImpactMercy() const;

    // Affiche l'action dans la console
    void afficher() const;

    // Retourne le catalogue complet des 8+ actions disponibles dans le jeu
    static vector<ActionAct> getCatalogue();
};
