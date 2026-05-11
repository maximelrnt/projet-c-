// Combat.h — ASSOCIATION : Combat --> Player& (reference, pas de possession)
//            COMPOSITION : Combat *-- Monster (copie locale)
#pragma once
#include "../core/Player.h"
#include "../core/Monster.h"
#include "ActionAct.h"
#include <string>
#include <vector>

enum class CombatResult { PLAYER_DEAD, MONSTER_KILLED, MONSTER_SPARED };

class Combat {
private:
    Player&      m_player;   // ASSOCIATION : reference vers le joueur
    Monster      m_monster;  // COMPOSITION : copie locale du monstre
    CombatResult m_result;

public:
    Combat(Player& player, Monster monster);

    void run();
    CombatResult getResult() const { return m_result; }

private:
    bool tourJoueur();
    bool tourMonstre();
    int  calculerDegats(int atk);

    void afficherStatut() const;
    void afficherSeparateur() const;
    void afficherMonstreApparition() const;
    void afficherVictoire() const;

    std::string getAsciiFilePath() const;
    std::string getCouleurMonstre() const;
    std::string makeHpBar(int hp, int hpMax, int barLen = 20) const;
    void afficherEcranCombat(const std::string& message = "") const;
    void afficherApparitionPokemon() const;
    std::vector<std::string> lireAsciiLines() const;
};
