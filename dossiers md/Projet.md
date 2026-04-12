# ALTERDUNE — Plan Complet du Projet C++

> **ESILV – A3 – Programmation Orientée Objet en C++**  
> Auteur du sujet : Daniel Wladdimiro  
> Binôme : Louis Le Forestier & Maxime  
> Soutenance mini-suivi : **08/04** | Soutenance P1 : **29/04** | P2 : **06/05**  
> **Dernière mise à jour : 12/04/2026 — Musique (PlaySound winmm), amélioration menus ANSI, fix warnings Unicode**

---

## Table des matières

1. [Vue d'ensemble du projet](#1-vue-densemble-du-projet)
2. [Architecture des fichiers](#2-architecture-des-fichiers)
3. [Description détaillée de chaque classe](#3-description-détaillée-de-chaque-classe)
4. [Schéma UML](#4-schéma-uml)
5. [Fichiers de données CSV](#5-fichiers-de-données-csv)
6. [Flux de jeu (gameplay loop)](#6-flux-de-jeu-gameplay-loop)
7. [Plan d'implémentation (ordre de développement)](#7-plan-dimplémentation-ordre-de-développement)
8. [Points clés POO à démontrer](#8-points-clés-poo-à-démontrer)
9. [Checklist finale](#9-checklist-finale)
10. [Système de sauvegarde (BONUS)](#10-système-de-sauvegarde-bonus)
11. [Musique (BONUS)](#11-musique-bonus)

---

## 1. Vue d'ensemble du projet

**ALTERDUNE** est un mini-RPG console au tour par tour inspiré de la mécanique d'*Undertale*. Le joueur affronte des monstres chargés depuis un fichier CSV. À chaque combat, il peut :
- **FIGHT** → attaquer directement
- **ACT** → interagir pour remplir la jauge Mercy du monstre
- **ITEM** → utiliser un objet de l'inventaire
- **MERCY** → épargner le monstre si Mercy ≥ 100

La partie se termine après **10 victoires**. Le type de fin (Génocidaire / Pacifiste / Neutre) dépend du ratio tués / épargnés.

---

## 2. Architecture des fichiers

```
Projetcpp/
│
├── main.cpp                        ✅ Active ANSI, init rand, lance GameManager
├── main.h                          ✅ Guard
│
├── core/
│   ├── Entity.h                    ✅ Classe abstraite (protected: attributs)
│   ├── Player.h / Player.cpp       ✅ Hérite de Entity, Inventory en composition
│   ├── Monster.h / Monster.cpp     ✅ Hérite de Entity, enum MonsterCategory, clone()
│
├── items/
│   ├── Item.h / Item.cpp           ✅ Classe abstraite (applyEffect = 0)
│   ├── HealItem.h / HealItem.cpp   ✅ Sous-classe concrète de Item
│   ├── Inventory.h / Inventory.cpp ✅ Composition dans Player, vector<Item*>
│
├── combat/
│   ├── ActionAct.h / ActionAct.cpp ✅ Une action ACT (id, texte, mercyImpact + variance)
│   ├── ActCatalogue.h / .cpp       ✅ Catalogue statique des 10 actions (map<string, ActAction>)
│   ├── Combat.h / Combat.cpp       ✅ Boucle de combat, UI style pokemon, ASCII art
│
├── managers/
│   ├── Bestiary.h / Bestiary.cpp   ✅ Contient BestiaryEntry (struct) + Bestiary (class)
│   ├── FileLoader.h / .cpp         ✅ Lecture CSV (items.csv, monsters.csv)
│   ├── SaveManager.h / .cpp        ✅ Sauvegarde/chargement .sav
│   ├── GameManager.h / .cpp        ✅ Chef d'orchestre: menus, combat, musique
│
├── Data/
│   ├── garfield.txt                ✅ ASCII art Garfield
│   ├── bob-leponge.txt             ✅ ASCII art Bob l'Éponge
│   ├── homer-simpson.txt           ✅ ASCII art Homer Simpson
│   ├── bugs-bunny.txt              ✅ ASCII art Bugs Bunny
│   ├── shrek.txt                   ✅ ASCII art Shrek
│   ├── eric-cartman.txt            ✅ ASCII art Eric Cartman
│   ├── musiques/
│   │   ├── menu.wav                ✅ Musique du menu (boucle)
│   │   └── combat.wav              ✅ Musique de combat (boucle)
│   └── Save/
│       └── save1.sav               (créé au runtime)
│
└── csv/
    ├── items.csv                   ✅ 3 objets de soin
    └── monsters.csv                ✅ 7 monstres avec IDs d'actions
```

---

## 3. Description détaillée de chaque classe

---

### 3.1 `Entity` (core/Entity.h)

**Rôle :** Classe de base abstraite représentant toute entité vivante du jeu.

**Attributs (protected) :**
| Attribut | Type | Description |
|---|---|---|
| `m_name` | `std::string` | Nom de l'entité |
| `m_hp` | `int` | Points de vie actuels |
| `m_hpMax` | `int` | Points de vie maximum |
| `m_atk` | `int` | Valeur d'attaque |
| `m_def` | `int` | Valeur de défense |

**Méthodes :**
- Constructeur : `Entity(string name, int hp, int atk, int def)`
- Accesseurs : `getName()`, `getHp()`, `getHpMax()`, `getAtk()`, `getDef()`
- `setHp(int hp)` → borne entre 0 et hpMax
- `bool isAlive() const` → HP > 0
- `int takeDamage(int dmg)` → soustrait les dégâts (min 0)
- `virtual void display() const = 0` → **méthode virtuelle pure**
- `virtual std::string getType() const = 0` → "PLAYER" ou "MONSTER"

---

### 3.2 `Player` (core/Player.h + Player.cpp)

**Rôle :** Représente le joueur. Hérite de `Entity`.

**Attributs supplémentaires (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_inventory` | `Inventory` | Composition : le joueur possède un inventaire |
| `m_kills` | `int` | Nombre de monstres tués |
| `m_spared` | `int` | Nombre de monstres épargnés |
| `m_victories` | `int` | Kills + Spared (total victoires) |

**Méthodes :**
- `void display() const override` / `string getType() const override`
- `Inventory& getInventory()` / `const Inventory& getInventory() const`
- `void addKill()` → kills++ et victories++
- `void addSpared()` → spared++ et victories++
- Getters : `getKills()`, `getSpared()`, `getVictories()`
- Setters load : `setKills()`, `setSpared()`, `setVictories()`, `setName()`, `loadStateHp()`, `setStats()`
- `void displayStats() const`
- `bool hasWon() const` → victories >= 10

---

### 3.3 `Monster` (core/Monster.h + Monster.cpp)

**Rôle :** Représente un monstre. Hérite de `Entity`.

**Type (enum class) :**
```cpp
enum class MonsterCategory { NORMAL, MINIBOSS, BOSS };
```

**Attributs supplémentaires (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_category` | `MonsterCategory` | Catégorie NORMAL/MINIBOSS/BOSS |
| `m_mercy` | `int` | Jauge Mercy actuelle |
| `m_mercyGoal` | `int` | Seuil pour épargner |
| `m_actIds` | `vector<string>` | IDs des actions ACT disponibles |

**Méthodes :**
- `string getCategoryStr() const` → "NORMAL", "MINIBOSS", "BOSS"
- `void addMercy(int amount)` → borne entre 0 et mercyGoal
- `bool isMercyFull() const`
- `const vector<string>& getActIds() const`
- `int getActCount() const`
- `Monster clone() const` → copie fraîche (HP remis à max, mercy remis à 0)

---

### 3.4 `Item` (items/Item.h + Item.cpp)

**Rôle :** Classe de base abstraite pour les objets.

**Attributs (protected) :**
| Attribut | Type | Description |
|---|---|---|
| `m_name` | `string` | Nom de l'objet |
| `m_type` | `string` | Type ("HEAL") |
| `m_value` | `int` | Valeur de l'effet |
| `m_quantity` | `int` | Quantité restante |

**Méthodes :**
- `bool use()` → diminue la quantité de 1
- `virtual void applyEffect(Player& player) = 0` → **virtuelle pure**
- `virtual void display() const`

---

### 3.5 `HealItem` (items/HealItem.h + HealItem.cpp)

**Rôle :** Objet de soin. Hérite de `Item`.

- `void applyEffect(Player& player) override` → restaure `value` HP (max = hpMax)

---

### 3.6 `Inventory` (items/Inventory.h + Inventory.cpp)

**Rôle :** Composition dans Player. Gère la liste d'items.

| Attribut | Type | Description |
|---|---|---|
| `m_items` | `vector<Item*>` | Liste des items (pointeurs) |

**Méthodes :**
- `~Inventory()` → destructeur libère la mémoire (`delete`)
- `void addItem(Item* item)`
- `bool useItem(int index, Player& player)`
- `void display() const`
- `bool isEmpty() const` / `bool hasUsableItems() const` / `int size() const`
- `const vector<Item*>& getItems() const` / `vector<Item*>& getItemsRaw()`

---

### 3.7 `ActAction` (combat/ActionAct.h + ActionAct.cpp)

**Rôle :** Représente une action ACT du catalogue.

| Attribut | Type | Description |
|---|---|---|
| `m_id` | `string` | Identifiant unique (ex: "FLATTER") |
| `m_displayText` | `string` | Texte affiché au joueur |
| `m_mercyImpact` | `int` | Impact de base sur Mercy |

**Note :** `getMercyImpact()` ajoute une variance aléatoire (±10/20) pour que l'impact ne soit jamais identique.

---

### 3.8 `ActCatalogue` (combat/ActCatalogue.h + ActCatalogue.cpp)

**Rôle :** Catalogue global de toutes les actions ACT (10 actions dont 2 négatives).

```cpp
static map<string, ActAction> s_actions;
```

| ID | Impact Mercy |
|---|---|
| `FLATTER` | +20 |
| `CHANTER` | +25 |
| `DANSER` | +15 |
| `ENCOURAGER` | +10 |
| `INTERROGER` | +5 |
| `REGARDER` | 0 |
| `INSULTER` | -20 |
| `PROVOQUER` | -30 |
| `MIMER` | +10 |
| `PLEURER` | +5 |

**Méthodes statiques :**
- `static void init()`
- `static const ActAction* getAction(const string& id)`
- `static void displayAvailable(const vector<string>& ids, vector<ActAction>& outAvailable)`

---

### 3.9 `BestiaryEntry` (struct dans managers/Bestiary.h)

**Rôle :** Enregistre les données d'un monstre vaincu.

Champs publics : `m_monsterName`, `m_category`, `m_hp`, `m_atk`, `m_def`, `m_wasKilled`  
Deux constructeurs : depuis `Monster` ou depuis données brutes (pour la sauvegarde)

---

### 3.10 `Bestiary` (managers/Bestiary.h + Bestiary.cpp)

**Rôle :** Collection de BestiaryEntry, affichée dans le menu.

```cpp
vector<BestiaryEntry> m_entries;
```

**Méthodes :**
- `void addEntry(const Monster& m, bool killed)`
- `void addRawEntry(string name, string cat, int hp, int atk, int def, bool killed)`
- `void display() const`
- `void clear()`
- `const vector<BestiaryEntry>& getEntries() const`
- `int totalKilled() const` / `int totalSpared() const`

---

### 3.11 `Combat` (combat/Combat.h + Combat.cpp)

**Rôle :** Gère un combat complet entre le joueur et un monstre.

```cpp
Player& m_player;
Monster m_monster;   // copie fraîche
CombatResult m_result;
```

**Résultat :**
```cpp
enum class CombatResult { PLAYER_DEAD, MONSTER_KILLED, MONSTER_SPARED };
```

**Interface style Pokemon :**
- `afficherEcranCombat()` → boîte ANSI avec ASCII art + barres HP `[###...]`
- `afficherApparitionPokemon()` → écran d'apparition du monstre
- `afficherVictoire()` → écran de victoire avec stats
- `makeHpBar()` → barre colorée verte/jaune/rouge selon %

**Bordures :** caractères ASCII simples (`#`, `=`, `+`, `-`) pour compatibilité Windows cp1252.

---

### 3.12 `FileLoader` (managers/FileLoader.h + FileLoader.cpp)

**Rôle :** Classe utilitaire statique, lit les fichiers CSV.

```cpp
static void loadItems(const string& filepath, Player& player);
static vector<Monster> loadMonsters(const string& filepath);
```

**Formats CSV :**
- `items.csv` : `nom; type; valeur; quantite`
- `monsters.csv` : `categorie; nom; hp; atk; def; mercyGoal; act1; act2; [act3]; [act4]`

---

### 3.13 `SaveManager` (managers/SaveManager.h + SaveManager.cpp)

**Rôle :** Sérialise/désérialise l'état complet du jeu dans un fichier `.sav`.

```cpp
static bool saveGame(const string& filename, const Player& player, const Bestiary& bestiary);
static bool loadGame(const string& filename, Player& player, Bestiary& bestiary);
```

**Format du fichier :**
```
# ALTERDUNE SAVE FILE v1
PLAYER_NAME=Louis
PLAYER_HP=85
PLAYER_HP_MAX=100
PLAYER_ATK=20
PLAYER_DEF=10
VICTORIES=4
KILLS=3
SPARED=1
INVENTORY=Potion de vie;HEAL;30;2|Grande Potion;HEAL;60;1
BESTIARY=Sableron;NORMAL;50;15;5;KILLED|Dunecrok;NORMAL;40;20;3;SPARED
```

---

### 3.14 `GameManager` (managers/GameManager.h + GameManager.cpp)

**Rôle :** Chef d'orchestre du jeu. Gère tout.

```cpp
Player* m_player;
Bestiary m_bestiary;
vector<Monster> m_monsterPool;
bool jeuEnCours;
```

**Méthodes :**
- `void demarrer()` → titre, saisie nom, chargement CSV, proposition de sauvegarde, **lance musique menu**
- `void showMainMenu()` → boucle principale avec barre de progression, options colorées
- `void lancerCombat()` → tire un monstre au hasard, **switch musique combat/menu**, lance `Combat::run()`
- `void showEnding()` → fin colorée selon kills/spared (PACIFISTE/NEUTRE/GENOCIDAIRE), **arrête musique**

**Points visuels :** 
- Cadre ASCII art aligné pour le titre `ALTERDUNE`
- Barre de progression `[###.......]` victoires
- Menus colorés ANSI (or, blanc, rouge, cyan, etc.)
- Écrans Game Over et Ending avec cadres colorés

---

## 4. Schéma UML

Voir le fichier [`Untitled-1.mmd`](uml/Untitled-1.mmd) (format Mermaid, viewable sur mermaid.live).

Résumé des relations :

| Relation | De | Vers | Type |
|---|---|---|---|
| Héritage | Player | Entity | `<|--` |
| Héritage | Monster | Entity | `<|--` |
| Héritage | HealItem | Item | `<|--` |
| Composition | Player | Inventory | `*--` |
| Composition | Inventory | Item | `*--` |
| Composition | Bestiary | BestiaryEntry | `*--` |
| Composition | ActCatalogue | ActAction | `*--` |
| Agrégation | GameManager | Player* | `o--` |
| Composition | GameManager | Bestiary | `*--` |
| Association | Combat | Player& | `-->` |
| Composition | Combat | Monster(copie) | `*--` |
| Dépendance | GameManager | FileLoader | `..>` |
| Dépendance | GameManager | SaveManager | `..>` |
| Dépendance | GameManager | Combat | `..>` |
| Dépendance | GameManager | ActCatalogue | `..>` |

---

## 5. Fichiers de données CSV

### `csv/items.csv`
```
nom; type; valeur; quantite
Potion de vie; HEAL; 30; 3
Grande Potion; HEAL; 60; 1
Elixir; HEAL; 100; 1
```

### `csv/monsters.csv`
```
categorie; nom; hp; atk; def; mercyGoal; act1; act2; act3; act4
NORMAL; Garfield; 50; 15; 5; 100; FLATTER; CHANTER;;
NORMAL; Bob l Eponge; 40; 20; 3; 100; DANSER; INTERROGER;;
NORMAL; Bugs Bunny; 60; 10; 8; 100; ENCOURAGER; MIMER;;
MINIBOSS; Homer Simpson; 100; 25; 10; 100; FLATTER; CHANTER; DANSER;
MINIBOSS; Eric Cartman; 90; 30; 8; 100; INTERROGER; PLEURER; ENCOURAGER;
BOSS; Shrek; 200; 40; 15; 100; FLATTER; CHANTER; DANSER; INSULTER
```

---

## 6. Flux de jeu (gameplay loop)

```
[DÉMARRAGE]
  └─▶ Lance musique menu (menu.wav en boucle)
  └─▶ Saisie du nom du joueur
  └─▶ Proposition de charger une sauvegarde (Data/Save/save1.sav)
  └─▶ Chargement items.csv + monsters.csv
  └─▶ ActCatalogue::init()
  └─▶ Affichage du résumé initial

[MENU PRINCIPAL] (boucle tant que victories < 10 et player.isAlive())
  ├─▶ [1] Demarrer un combat
  │     └─▶ Switch musique → combat.wav
  │     └─▶ [COMBAT] ──────────────────────────
  │           ├─▶ Apparition du monstre (Pokemon style)
  │           ├─▶ [FIGHT]  → damage + perte mercy
  │           ├─▶ [ACT]    → modifier Mercy (avec variance)
  │           ├─▶ [ITEM]   → utiliser objet
  │           └─▶ [MERCY]  → épargner si mercy pleine
  │           │
  │           └─▶ Tour monstre : attaque joueur
  │     └─▶ Switch musique → menu.wav
  │     └─▶ Résultat : addKill() ou addSpared() + addEntry(bestiary)
  ├─▶ [2] Voir l'inventaire
  ├─▶ [3] Voir le bestiaire
  ├─▶ [4] Voir mes statistiques
  ├─▶ [5] Sauvegarder → SaveManager::saveGame()
  └─▶ [6] Quitter

[FIN] (10 victoires)
  └─▶ Arrête la musique
  ├─▶ kills == 0   → FIN PACIFISTE (cyan)
  ├─▶ spared == 0  → FIN GÉNOCIDAIRE (rouge)
  └─▶ sinon        → FIN NEUTRE (jaune)
```

---

## 7. Plan d'implémentation (ordre de développement)

| Étape | Fichiers | Statut |
|---|---|---|
| 1 | `Entity.h` | ✅ Fait |
| 2 | `Item.h/.cpp` + `HealItem.h/.cpp` | ✅ Fait |
| 3 | `Inventory.h/.cpp` | ✅ Fait |
| 4 | `Player.h/.cpp` | ✅ Fait |
| 5 | `Monster.h/.cpp` | ✅ Fait |
| 6 | `ActionAct.h/.cpp` | ✅ Fait |
| 7 | `ActCatalogue.h/.cpp` | ✅ Fait |
| 8 | `FileLoader.h/.cpp` + CSV | ✅ Fait |
| 9 | `Bestiary.h/.cpp` | ✅ Fait |
| 10 | `Combat.h/.cpp` | ✅ Fait |
| 11 | `SaveManager.h/.cpp` | ✅ Fait (BONUS) |
| 12 | `GameManager.h/.cpp` | ✅ Fait |
| 13 | `main.cpp` | ✅ Fait |
| 14 | ASCII art monstres (.txt) | ✅ Fait |
| 15 | Musique (.wav via PlaySound) | ✅ Fait (BONUS) |

---

## 8. Points clés POO à démontrer

| Concept | Où | Comment |
|---|---|---|
| **Encapsulation** | Toutes les classes | Attributs `private`/`protected`, accesseurs publics |
| **Héritage** | `Player`, `Monster` héritent de `Entity` | `HealItem` hérite de `Item` |
| **Polymorphisme** | `Entity::display()` virtuelle pure | `Item::applyEffect()` virtuelle pure |
| **Composition** | `Player` contient `Inventory` | `Inventory` contient `vector<Item*>` |
| **Struct** | `BestiaryEntry` dans `Bestiary.h` | Données agrégées sans comportement propre |
| **Enum class** | `MonsterCategory`, `CombatResult` | Typage fort, pas de conversion implicite |
| **Map** | `ActCatalogue` | `map<string, ActAction>` pour accès par ID |
| **Fichiers (lecture)** | `FileLoader`, `SaveManager` | `ifstream`, parsing CSV et clé=valeur |
| **Fichiers (écriture)** | `SaveManager` | `ofstream` pour les `.sav` |
| **Gestion mémoire** | `Inventory` | Destructeur `delete` pour les `Item*` |
| **Random** | `Combat`, `ActAction` | `rand()`, `mt19937` pour les dégâts et variance ACT |
| **Classe statique** | `ActCatalogue`, `FileLoader`, `SaveManager` | Méthodes `static`, pas d'instance |

---

## 9. Checklist finale

> **Légende :** ✅ Fait | ⚠️ Partiel | ❌ À faire

### Classes et fichiers
- ✅ `Entity.h` — abstraite, `display()` et `getType()` pures
- ✅ `Player.h/.cpp` — hérite de Entity, kills/spared/victories, setters load
- ✅ `Monster.h/.cpp` — hérite de Entity, enum `MonsterCategory`, `clone()`
- ✅ `Item.h/.cpp` — **abstraite** avec `applyEffect()=0`
- ✅ `HealItem.h/.cpp` — sous-classe concrète
- ✅ `ActionAct.h/.cpp` — 10 actions dont 2 négatives, variance aléatoire
- ✅ `ActCatalogue.h/.cpp` — catalogue statique `map<string, ActAction>`
- ✅ `Inventory.h/.cpp` — composition, destructeur libère la mémoire
- ✅ `Bestiary.h/.cpp` — `BestiaryEntry` (struct) + `Bestiary` (class)
- ✅ `FileLoader.h/.cpp` — lecture CSV items + monstres
- ✅ `SaveManager.h/.cpp` — sauvegarde/chargement `.sav` (**BONUS**)
- ✅ `Combat.h/.cpp` — boucle combat, UI ANSI style pokemon
- ✅ `GameManager.h/.cpp` — menus, musique, fins
- ✅ `main.cpp` — active ANSI Windows, init rand

### Données
- ✅ `csv/items.csv` — 3 objets de soin
- ✅ `csv/monsters.csv` — 6+ monstres avec IDs d'actions
- ✅ ASCII art des 6 monstres (fichiers `.txt`)
- ✅ `Data/musiques/menu.wav` + `combat.wav` (**BONUS**)

### Fonctionnalités
- ✅ Chargement CSV → inventaire + pool monstres
- ✅ Menu principal coloré avec barre de progression
- ✅ Combat complet (FIGHT / ACT / ITEM / MERCY)
- ✅ Jauge Mercy avec variance aléatoire
- ✅ ASCII art monstres affiché en combat (style Pokémon)
- ✅ Barres HP colorées (vert/jaune/rouge)
- ✅ 3 fins différentes (Pacifiste / Neutre / Génocidaire)
- ✅ Sauvegarde / Chargement (**BONUS**)
- ✅ Musique menu + combat avec `PlaySound` winmm (**BONUS**)
- ✅ Warnings Unicode C4566 corrigés (ASCII cp1252 compatible)
- ✅ Warning C4267 size_t→int corrigé

---

## 10. Système de sauvegarde (BONUS)

**Format du fichier `.sav` :**
```
# ALTERDUNE SAVE FILE v1
PLAYER_NAME=Louis
PLAYER_HP=85
PLAYER_HP_MAX=100
PLAYER_ATK=20
PLAYER_DEF=10
VICTORIES=4
KILLS=3
SPARED=1
INVENTORY=Potion de vie;HEAL;30;2|Grande Potion;HEAL;60;1
BESTIARY=Sableron;NORMAL;50;15;5;KILLED|Dunecrok;NORMAL;40;20;3;SPARED
```

**Règles :**
- Chaque ligne = `CLE=VALEUR`
- `INVENTORY` : items séparés par `|`, champs par `;`
- `BESTIARY` : entrées séparées par `|`, champs par `;`
- Lignes `#` = commentaires (ignorées)

---

## 11. Musique (BONUS)

**Implémentation via l'API Win32 Multimedia (`winmm.lib`) :**

```cpp
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Démarrer la musique en boucle
PlaySoundA("Data/musiques/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

// Arrêter la musique
PlaySoundA(NULL, NULL, 0);
```

**Comportement :**
| Moment | Musique |
|---|---|
| Démarrage du jeu (`demarrer()`) | `menu.wav` démarre en boucle |
| Menu principal | `menu.wav` continue |
| Avant `Combat::run()` | Switch vers `combat.wav` |
| Après `Combat::run()` | Switch retour vers `menu.wav` |
| `showEnding()` | Musique arrêtée |

**Avantages :** Aucune dépendance externe, natif Windows, `.wav` nativement supporté.
