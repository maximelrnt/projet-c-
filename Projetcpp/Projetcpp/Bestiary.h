// =============================================================================
// Bestiary.h
//
// Le Bestiaire est le journal des monstres rencontrés pendant la partie.
// À chaque fin de combat, une entrée est ajoutée : elle contient le nom du monstre,
// sa catégorie, ses statistiques, et le résultat (tué ou épargné).
//
// C'est aussi le Bestiaire qui permet au GameManager de calculer, en fin de partie,
// si le joueur a un profil Génocidaire, Pacifiste ou Neutre.
// =============================================================================
#pragma once
#include <string>
#include <vector>

using namespace std;

// Structure représentant une entrée dans le bestiaire (un monstre vaincu)
struct BestiaryEntry {
    string nom;
    string categorie;
    int hp;
    int atk;
    int def;
    bool eteEpargne;    // true = épargné via Mercy, false = tué au combat
};

class Bestiary {
private:
    vector<BestiaryEntry> entrees;  // Liste de tous les monstres vaincus

public:
    // Ajoute un monstre vaincu au bestiaire
    void ajouterEntree(const string& nom, const string& categorie,
                       int hp, int atk, int def, bool eteEpargne);

    // Affiche tous les monstres vaincus avec leurs infos
    void afficher() const;

    // Retourne le nombre de monstres tués (utile pour la fin de partie)
    int getNbTues() const;

    // Retourne le nombre de monstres épargnés (utile pour la fin de partie)
    int getNbEpargnes() const;
};
