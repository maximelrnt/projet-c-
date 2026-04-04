# Plan d'implementation : ALTERDUNE (Projet C++)

> Derniere mise a jour : 04/04/2026
> Source de verite du binome : Louis Le Forestier & Maxime

---

## REGLES D'OR DU PROJET (Binome)

1. **Un paragraphe par classe** : Au tout debut de chaque fichier .h, un bloc commentaire C++ explique en quelques phrases le role global de la classe, ce qu'elle represente dans le jeu et comment elle interagit avec les autres. Pas de commentaires ligne par ligne.
2. **Ce fichier fait reference** : implementation_plan.md est la source de verite. On y revient a chaque fois qu'on commence une nouvelle classe ou phase.
3. **Comprehension mutuelle** : Les deux membres du binome doivent pouvoir relire n'importe quelle partie du code et comprendre a quoi elle sert.

---

## ETAT ACTUEL DU CODE (04/04/2026)

### Fichiers existants dans `Projetcpp/Projetcpp/`

| Fichier | Etat | Notes |
|---|---|---|
| `Entity.h` | ✅ Fait | Classe abstraite, `display()` et `getType()` virtuelles pures. |
| `Player.h / .cpp` | ✅ Fait | Herite de `Entity`. Inventaire = `vector<Item*>` |
| `Monster.h / .cpp` | ✅ Fait | Herite de `Entity`. `MonsterCategory` en enum class. |
| `Item.h / .cpp` | ✅ Fait | Classe **abstraite**. Methode `applyEffect(Player&)` virtuelle pure. |
| `HealItem.h / .cpp`| ✅ Fait | Sous-classe concrete de `Item`. Soigne le joueur. |
| `ActionAct.h / .cpp` | ✅ Fait | Classe `ActAction` presente. Catalogue statique integre. |
| `Bestiary.h / .cpp` | ✅ Fait | Journal de chasse fonctionnel. |
| `Combat.h / .cpp`   | ✅ Fait | Systeme de combat complet (FIGHT/ACT/ITEM/MERCY). |
| `GameManager.h / .cpp`| ✅ Fait | Gere le menu, chargement CSV, combat et fins multiples. |
| `main.cpp` | ✅ Fait | Point d'entree, active les couleurs ANSI. |
| `csv/items.csv` | ✅ Fait | 3 objets de soin. |
| `csv/monsters.csv` | ✅ Fait | IDs d'actions corriges (FLATTER, CHANTER...). |
| `Data/*.txt` | ✅ Fait | Art ASCII pour chaque monstre. |
| **Victoire UI** | ✅ Fait | Cadre "Victoire !!!" avec stats (Combat.cpp). |

### Ce qui est ABSENT et doit etre code

| Element | Priorite | Raison |
|---|---|---|
| `HealItem.h / .cpp` | 🔴 Haute | Necessaire pour respecter le polymorphisme (`Item` abstraite → `HealItem` concrete). Demande par l'enonce. |
| Classe `Inventory` | 🟠 Moyenne | Pour l'instant le `vector<Item*>` est dans `Player`. Creer `Inventory.h/.cpp` isolerait mieux la composition. |
| Classe `ActCatalogue` | 🟠 Moyenne | `ActAction::getCatalogue()` fonctionne, mais une vraie classe avec `std::map` et `getAction(id)` serait plus propre. |
| Classe `FileLoader` | 🟠 Moyenne | Le parsing CSV est dans `GameManager`. Le separer montrerait une meilleure separation des responsabilites. |
| **`Combat.h / .cpp`** | 🔴 CRITIQUE | Le systeme de combat est **absent**. C'est le coeur du gameplay. A coder en priorite. |
| Fins multiples (Ending) | 🟠 Moyenne | Logique pacifiste/génocidaire/neutre apres 10 victoires. |
| `SaveManager.h / .cpp` | 🔵 Bonus | Sauvegarde/chargement dans des fichiers `.sav`. |

---

## 1. Architecture des Classes

### Architecture cible (conforme a `Projet.md`)

```
Entity  (classe abstraite)
├── Player
└── Monster

Item (classe abstraite)
└── HealItem

Inventory  (composition dans Player)
ActAction + ActCatalogue
Bestiary (contains BestiaryEntry)
Combat
GameManager (= "Game" dans Projet.md)
FileLoader (utilitaire statique)
SaveManager (utilitaire statique, BONUS)
```

### Architecture actuelle (ce qui existe vraiment)

```
Entity  (classe abstraite)              ✅
├── Player (avec vector<Item*> inline)  ✅ (sans vrai Inventory)
└── Monster                             ✅

Item (NON abstraite pour l'instant)     ⚠️
ActAction (avec getCatalogue() static)  ⚠️ (sans vrai ActCatalogue)
Bestiary + BestiaryEntry (dans 1 .h)   ✅
GameManager                             ⚠️ (sans Combat)
main.cpp                                ✅
csv/items.csv + csv/monsters.csv        ⚠️ (IDs monstres a corriger)
```

---

## 2. Description des classes

### Entity (abstraite) — ✅ FAIT
Represente tout ce qui est vivant dans le jeu. Regroupe les attributs communs (nom, HP, ATK, DEF) et declare les methodes virtuelles pures `display()` et `getType()`. Grace a cette classe, Player et Monster sont manipulables via un meme pointeur de type `Entity*`.

### Player (herite de Entity) — ✅ FAIT
Represente le joueur. Possede un inventaire (vector<Item*>), des compteurs de kills/spared/victories. `hasWon()` retourne true a 10 victoires.

### Monster (herite de Entity) — ✅ FAIT
Represente un ennemi. Categorie en enum class (NORMAL/MINIBOSS/BOSS), jauge Mercy, liste d'IDs d'actions ACT. `clone()` retourne une copie fraiche avec HP max remis.

### Item (abstraite) — ⚠️ A CORRIGER
Classe de base des objets. Pour l'instant non abstraite. Il faut ajouter `virtual void applyEffect(Player& player) = 0` pour en faire la base de `HealItem`.

### HealItem (herite de Item) — ❌ A CREER
Soigne le joueur de `m_value` HP. Implemente `applyEffect()`. A creer en `HealItem.h` et `HealItem.cpp`.

### Inventory — ❌ A CREER (recommande)
Classe de composition dans Player. Encapsule le `vector<Item*>` et fournit `addItem()`, `useItem(index, player)`, `display()`, `isEmpty()`. Remplace le vecteur brut dans Player.

### ActAction — ⚠️ FAIT (partiellement)
Represente une action non-violente (id, texte, impact Mercy). Le catalogue des 10 actions est dans `getCatalogue()`.

### ActCatalogue — ❌ A CREER (recommande)
Classe utilitaire statique avec `std::map<string, ActAction>`. Methodes : `init()`, `getAction(id)`, `displayAvailable(ids)`. Remplace `getCatalogue()` dans ActAction.

### BestiaryEntry + Bestiary — ✅ FAIT
BestiaryEntry est un struct dans Bestiary.h. Fonctionnel. Si le temps le permet, separer BestiaryEntry dans son propre .h/.cpp.

### Combat — ❌ A CREER (CRITIQUE)
Gere un combat complet entre le joueur et un monstre. Boucle de tours Player/Monster. Menus FIGHT/ACT/ITEM/MERCY. Retourne un `CombatResult` (PLAYER_DEAD, MONSTER_KILLED, MONSTER_SPARED).

### GameManager — ⚠️ FAIT (partiellement)
Chef d'orchestre. Menu principal fonctionnel, chargement CSV integre. Manque : appel au systeme de Combat, conditions de fin (10 victoires + endings), liaison avec ACT (les IDs du CSV sont numeriques).

### FileLoader — ❌ A CREER (recommande)
Deplacer le code de chargement CSV hors de GameManager dans une classe statique distincte. Montre la separation des responsabilites.

### SaveManager — ❌ A CREER (BONUS)
Sauvegarde et chargement dans `Data/saves/slotX.sav`. Format cle=valeur.

---

## 3. Fichiers de Donnees

```
items.csv :    nom; type; valeur; quantite         ✅ OK
monsters.csv : categorie; nom; hp; atk; def; mercyGoal; act1; act2; [act3; act4]
               ⚠️ PROBLEME : les act1/act2... contiennent des numeros (1,2,3...)
                  au lieu des noms texte (FLATTER, CHANTER...).
                  A corriger pour etre coherent avec ActAction.
```

---

## 4. Feuille de Route (Phases)

### Phase 0 : Planification — ✅ DONE
- [x] Lire et comprendre l'enonce
- [x] Definir l'architecture des classes
- [x] Rediger Projet.md et implementation_plan.md

### Phase 1 : Structures de base et CSV — ✅ FAIT (partiel)
- [x] Creer `Item.h/.cpp` (non abstraite pour l'instant)
- [x] Creer `ActAction.h/.cpp` + catalogue des 10 actions
- [x] Creer `Bestiary.h/.cpp` (avec BestiaryEntry en struct)
- [x] Implementer le parseur CSV (dans GameManager)
- [x] Creer les fichiers CSV `items.csv` et `monsters.csv`
- [ ] ⚠️ **Corriger `monsters.csv`** : remplacer les IDs numeriques par les noms texte (FLATTER, CHANTER...)

### Phase 2 : Entites (Heritage et Polymorphisme) — ✅ FAIT (partiel)
- [x] Creer `Entity.h` abstraite
- [x] Creer `Player.h/.cpp` avec inventaire inline
- [x] Creer `Monster.h/.cpp` avec enum et clone()
- [x] ⚠️ **Rendre `Item` abstraite** → ajouter `virtual void applyEffect(Player&) = 0`
- [x] **Creer `HealItem.h/.cpp`** → implemente applyEffect() (restaure HP)

### Phase 3 : Menu Principal — ✅ FAIT (partiel)
- [x] Boucle principale du GameManager
- [x] Afficher Statistiques, Bestiaire, Inventaire
- [ ] **Brancher le combat** (appel a `Combat`)
- [ ] **Refactoring optionnel** : extraire `FileLoader` + `Inventory` + `ActCatalogue`

- [x] **Creer `Combat.h/.cpp`**
  - [x] Constructeur `Combat(Player&, Monster)`
  - [x] `run()` : boucle principale du combat
  - [x] `playerTurn()` : affiche menu FIGHT/ACT/ITEM/MERCY
  - [x] `monsterTurn()` : le monstre attaque le joueur (degats aleatoires)
  - [x] `calcDamage(int maxHp)` : `rand() % maxHp`
  - [x] `getResult()` : retourne `CombatResult`
  - [x] `displayStatus()` : affiche les HP et Mercy
- [x] Brancher `Combat` dans `GameManager::lancerCombat()`
- [x] Apres chaque combat : `addKill()` ou `addSpared()`, `addEntry(bestiary)`
- [x] **Affichage Victoire** : Cadre stylise avec stats du monstre vaincu.

- [x] Detecter 10 victoires dans la boucle de `GameManager`
- [x] `showEnding()` : Pacifiste (kills==0), Genocidaire (spared==0), Neutre (sinon)
- [x] Nettoyage du code et commentaires dans les .h

### Phase 6 : Bonus — ❌ OPTIONNEL
- [ ] `SaveManager.h/.cpp` : sauvegarde dans `Data/saves/slotX.sav`
- [ ] Integration de la sauvegarde dans `GameManager::showMainMenu()` (option 5)
- [x] Amelioration affichage console (ASCII art, separateurs, couleurs ANSI, clear screen)

---

## 5. Incohérences à corriger

| Probleme | Fichier | Correction |
|---|---|---|
| Attributs `protected` au lieu de `private` | `Entity.h` | Changer en `private`, passer par des accesseurs dans les sous-classes |
| `Item` non abstraite | `Item.h` | Ajouter `virtual void applyEffect(Player&) = 0` |
| IDs d'action numeriques dans le CSV | `monsters.csv` | Remplacer 1,2,3... par FLATTER, CHANTER, DANSER... |
| `addVictory()` inutile dans Player | `Player.h` | `addKill()` et `addSpared()` incrementent deja `m_victories` |
| `lancerCombat()` vide dans GameManager | `GameManager.cpp` | Implementer avec `Combat` |

---

## 6. Evaluation

| Etape         | Date  | Poids | Criteres |
|---|---|---|---|
| Mini-suivi    | 08/04 | 20%   | UML initial, structure du code, premieres classes |
| Soutenance P1 | 29/04 | 80%   | UML final, POO complete, gameplay fonctionnel |
| Soutenance P2 | 06/05 | --    | QR individuel sur la maitrise du code |

Bonus : Jusqu'a +4 points pour SaveManager + qualite generale.

### Checklist Mini-Suivi 08/04

- [x] `Entity.h` — classe abstraite avec virtuelles pures
- [x] `Player.h/.cpp` — herite de Entity
- [x] `Monster.h/.cpp` — herite de Entity, enum MonsterCategory
- [x] `Item.h/.cpp` — classe de base (a rendre abstraite avant 08/04)
- [x] `ActionAct.h/.cpp` — 10 actions dont 2 negatives
- [x] `Bestiary.h/.cpp` — journal des monstres vaincus
- [x] `csv/items.csv` + `csv/monsters.csv` — fichiers de donnees
- [x] `main.cpp` — point d'entree fonctionnel
- [x] `HealItem.h/.cpp` — sous-classe concrete de Item
- [x] **Corriger monsters.csv** (IDs texte au lieu de numeros)
- [ ] **UML** dessine (conforme a Projet.md, section 4)
- [x] Affichage initial : nom joueur + HP + inventaire apres chargement CSV
