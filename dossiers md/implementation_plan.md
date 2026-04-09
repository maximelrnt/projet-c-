# Plan d'implementation : ALTERDUNE (Projet C++)

> Derniere mise a jour : 05/04/2026
> Source de verite du binome : Louis Le Forestier & Maxime

---

## REGLES D'OR DU PROJET (Binome)

1. **Un paragraphe par classe** : Au tout debut de chaque fichier .h, un bloc commentaire C++ explique en quelques phrases le role global de la classe, ce qu'elle represente dans le jeu et comment elle interagit avec les autres. Pas de commentaires ligne par ligne.
2. **Ce fichier fait reference** : implementation_plan.md est la source de verite. On y revient a chaque fois qu'on commence une nouvelle classe ou phase.
3. **Comprehension mutuelle** : Les deux membres du binome doivent pouvoir relire n'importe quelle partie du code et comprendre a quoi elle sert.

---

## ETAT ACTUEL DU CODE (05/04/2026)

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
| `Combat.h / .cpp`   | ✅ FAIT | Systeme de combat et écran de victoire complets (FIGHT/ACT/ITEM/MERCY). |
| `GameManager.h / .cpp`| ✅ Fait | Gere le menu, chargement CSV, combat et fins multiples. |
| `main.cpp` | ✅ Fait | Point d'entree, active les couleurs ANSI. |
| `csv/items.csv` | ✅ Fait | 3 objets de soin. |
| `csv/monsters.csv` | ✅ Fait | IDs d'actions corriges (FLATTER, CHANTER...). |
| `Data/*.txt` | ✅ Fait | Art ASCII pour chaque monstre (Dossier Data optionnel). |
| **Victoire UI** | ✅ Fait | Cadre "Victoire !!!" avec stats (Combat.cpp). |

### Ce qui est ABSENT et Optionnel (Refactoring / Bonus)

| Element | Priorite | Raison |
|---|---|---|
| Classe `Inventory` | ✅ Fait | Creation de la classe `Inventory` pour encapsuler et controler le `vector<Item*>`. Intégré dans Player. |
| Classe `ActCatalogue` | ✅ Fait | Création du registry et implémenté dans le Combat. |
| Classe `FileLoader` | ✅ Fait | Réfractorisation effectuée. Les méthodes ont été migrées. |
| `SaveManager.h / .cpp` | ✅ Fait | Systeme de `loadGame` et `saveGame` rajouté dans le Menu Principal ("Data/Save/save1.sav"). |

---

## 1. Architecture des Classes

### Architecture cible (conforme a `Projet.md`)

```
Entity  (classe abstraite)
├── Player
└── Monster

Item (classe abstraite)
└── HealItem

Inventory  (composition dans Player) - Optionnel
ActAction + ActCatalogue (fusionnées dans ActionAct pour le moment)
Bestiary (contains BestiaryEntry)
Combat
GameManager (= "Game" dans Projet.md)
FileLoader (utilitaire statique) - Optionnel
SaveManager (utilitaire statique, BONUS)
```

### Architecture actuelle (ce qui existe vraiment)

```
Entity  (classe abstraite)              ✅
├── Player (avec vector<Item*> inline)  ✅ (sans vrai Inventory)
└── Monster                             ✅

Item (abstraite)                        ✅
└── HealItem                            ✅
ActionAct (avec getCatalogue() static)  ✅ (sans vrai ActCatalogue)
Bestiary + BestiaryEntry (dans 1 .h)    ✅
GameManager                             ✅
Combat                                  ✅
main.cpp                                ✅
csv/items.csv + csv/monsters.csv        ✅ (IDs textes OK)
```

---

## 2. Description des classes

### Entity (abstraite) — ✅ FAIT
Represente tout ce qui est vivant dans le jeu. Regroupe les attributs communs (nom, HP, ATK, DEF) et declare les methodes virtuelles pures `display()` et `getType()`. Grace a cette classe, Player et Monster sont manipulables via un meme pointeur de type `Entity*`. La visibilité des attributs est `protected`.

### Player (herite de Entity) — ✅ FAIT
Represente le joueur. Possede un inventaire (vector<Item*>), des compteurs de kills/spared/victories. `hasWon()` retourne true a 10 victoires. Affiche ses statistiques détaillées.

### Monster (herite de Entity) — ✅ FAIT
Represente un ennemi. Categorie en enum class (NORMAL/MINIBOSS/BOSS), jauge Mercy, liste d'IDs d'actions ACT. `clone()` retourne une copie fraiche avec HP max remis.

### Item (abstraite) — ✅ FAIT
Classe de base des objets, avec `virtual void applyEffect(Player& player) = 0` pour forcer chaque sous-classe (comme HealItem) à posséder son effet propre.

### HealItem (herite de Item) — ✅ FAIT
Soigne le joueur de `m_value` HP. Implemente `applyEffect(Player& player)`.

### Inventory — ✅ FAIT
Classe de composition dans Player. Encapsule le `vector<Item*>` et fournit `addItem()`, `useItem(index, player)`, `display()`, `isEmpty()`. Remplace le vecteur brut dans Player.

### ActionAct — ✅ FAIT
Represente une action non-violente (id, texte, impact Mercy).

### ActCatalogue — ✅ FAIT
Classe utilitaire statique avec `std::map<string, ActAction>`. Centralise la création des actions pacifistes.

### BestiaryEntry + Bestiary — ✅ FAIT
BestiaryEntry est un struct dans Bestiary.h. Parfaitement fonctionnel.

### Combat — ✅ FAIT
Gere un combat complet entre le joueur et un monstre. Boucle de tours Player/Monster. Menus FIGHT/ACT/ITEM/MERCY. Retourne un `CombatResult` et gère l'écran "Victoire !!!".

### GameManager — ✅ FAIT
Chef d'orchestre. Menu principal fonctionnel, instanciation des combats, vérification de la condition des 10 victoires, et menu pour la sauvegarde/chargement.

### FileLoader — ✅ FAIT
Classe statique. Gère le File IO vers le format CSV.

### SaveManager — ✅ FAIT
Sauvegarde et chargement dans `"Data/Save/save1.sav"`. Serialize/Deserialize l'inventaire, le Player, et le bestiaire.

---

## 3. Fichiers de Donnees

```
items.csv :    nom; type; valeur; quantite         ✅ OK
monsters.csv : categorie; nom; hp; atk; def; mercyGoal; act1; act2; [act3; act4] ✅ OK (IDs textes)
```

---

## 4. Feuille de Route (Phases)

### Phase 0 : Planification — ✅ DONE
- [x] Lire et comprendre l'enonce
- [x] Definir l'architecture des classes
- [x] Rediger Projet.md et implementation_plan.md

### Phase 1 : Structures de base et CSV — ✅ FAIT
- [x] Creer `Item.h/.cpp` (abstraite)
- [x] Creer `ActionAct.h/.cpp` + catalogue des actions
- [x] Creer `Bestiary.h/.cpp`
- [x] Implementer le parseur CSV (dans GameManager)
- [x] Creer les fichiers CSV `items.csv` et `monsters.csv` avec attributs corrects.

### Phase 2 : Entites (Heritage et Polymorphisme) — ✅ FAIT
- [x] Creer `Entity.h` abstraite (`protected` data)
- [x] Creer `Player.h/.cpp` avec inventaire inline
- [x] Creer `Monster.h/.cpp` avec enum et clone()
- [x] Rendre `Item` abstraite
- [x] Creer `HealItem.h/.cpp`

### Phase 3 : Combat et  Menu Principal — ✅ FAIT
- [x] Boucle principale du GameManager
- [x] Afficher Statistiques, Bestiaire, Inventaire
- [x] Brancher le combat (APPEL dans `GameManager::lancerCombat()`)
- [x] Creer `Combat.h/.cpp` complet avec menu FIGHT/ACT/ITEM/MERCY
- [x] Affichage Victoire (effacement d'écran `system("cls")`)
- [x] Gérer les conditions de victoire/défaite par les résultats de Combat
- [x] `showEnding()` : Pacifiste, Genocidaire, Neutre
- [x] Amélioration esthétique

### Phase 4 : Bonus & Refactoring — ✅ FAIT
- [x] `SaveManager.h/.cpp` : sauvegarde dans `Data/Save/save1.sav`
- [x] Integration de la sauvegarde dans `GameManager::showMainMenu()`
- [x] Refactoring : extraire `FileLoader` + `Inventory` + `ActCatalogue`

---

## 5. Incohérences à corriger (Historique)

| Probleme (Ancien) | Fichier | Statut / Correction Apportée |
|---|---|---|
| `Item` non abstraite | `Item.h` | ✅ Corrigé ( `applyEffect` virtuelle pure ajoutée) |
| IDs d'action numeriques | `monsters.csv` | ✅ Corrigé |
| `addVictory()` inutile | `Player.h` | ✅ Conservé/Adapté en incrémentation automatique via Kills/Spared |
| `lancerCombat()` vide | `GameManager.cpp` | ✅ Rempli et branché |

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
- [x] `Item.h/.cpp` — abstraite
- [x] `ActionAct.h/.cpp` — actions avec impact mercy
- [x] `Bestiary.h/.cpp` — journal des monstres vaincus
- [x] `csv/items.csv` + `csv/monsters.csv` — propres
- [x] `main.cpp` — point d'entree fonctionnel
- [x] `HealItem.h/.cpp` — sous-classe concrete
- [x] **Combat** implémenté et branché
- [x] **UML** dessiné (intégré dans rendu1.md mis en forme)
- [x] Affichage propre ANSI/system cls

