#pragma once
#include "../core/Player.h"
#include "../core/Monster.h"
#include "ActionAct.h"
#include <string>
#include <vector>

// les 3 resultats possibles d'un combat
enum class CombatResult { PLAYER_DEAD, MONSTER_KILLED, MONSTER_SPARED };

// Combat : gere un combat complet entre le joueur et un monstre
// fait les tours du joueur et du monstre, affiche les menus, etc
class Combat {
private:
    Player& m_player;      // reference vers le joueur
    Monster m_monster;      // copie du monstre (pour pas modifier l'original)
    CombatResult m_result;  // le resultat du combat

public:
    Combat(Player& player, Monster monster);

    // lance la boucle de combat
    void run();
    CombatResult getResult() const { return m_result; }

private:
    // tour du joueur (FIGHT/ACT/ITEM/MERCY)
    bool tourJoueur();

    // tour du monstre (il attaque)
    bool tourMonstre();

    // calcul aleatoire des degats
    int calculerDegats(int atk);

    // fonctions d'affichage
    void afficherStatut() const;
    void afficherSeparateur() const;

    // affichage ASCII du monstre au debut
    void afficherMonstreApparition() const;
    std::string getAsciiFilePath() const;
    std::string getCouleurMonstre() const;

    // ecran de victoire
    void afficherVictoire() const;

    // --- fonctions pour l'interface style pokemon (ANSI) ---
    std::string makeHpBar(int hp, int hpMax, int barLen = 20) const;
    void afficherEcranCombat(const std::string& message = "") const;
    void afficherApparitionPokemon() const;
    std::vector<std::string> lireAsciiLines() const;
};
