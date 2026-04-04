#pragma once
#include "Player.h"
#include "Monster.h"
#include "ActionAct.h"
#include <string>

/*
 * CombatResult : les 3 issues possibles d'un combat.
 */
enum class CombatResult { PLAYER_DEAD, MONSTER_KILLED, MONSTER_SPARED };

/*
 * Combat : gere un combat complet entre le joueur et un monstre.
 * Orchestre les tours joueur/monstre, les menus FIGHT/ACT/ITEM/MERCY,
 * affiche l'art ASCII du monstre au debut, et retourne un CombatResult.
 */
class Combat {
private:
    Player& m_player;
    Monster m_monster;
    CombatResult m_result;

public:
    Combat(Player& player, Monster monster);

    void run();
    CombatResult getResult() const { return m_result; }

private:
    bool tourJoueur();
    bool tourMonstre();
    int calculerDegats(int atk);
    void afficherStatut() const;
    void afficherSeparateur() const;

    // Affichage ASCII du monstre au debut du combat
    void afficherMonstreApparition() const;
    std::string getAsciiFilePath() const;
    std::string getCouleurMonstre() const;

    // Affichage de la victoire
    void afficherVictoire() const;
};
