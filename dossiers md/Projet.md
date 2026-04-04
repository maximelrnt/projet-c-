# ALTERDUNE — Plan Complet du Projet C++

> **ESILV – A3 – Programmation Orientée Objet en C++**  
> Auteur du sujet : Daniel Wladdimiro  
> Binôme : Louis Le Forestier & Maxime …  
> Soutenance mini-suivi : **08/04** | Soutenance P1 : **29/04** | P2 : **06/05**

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
9. [Checklist mini-suivi 08/04](#9-checklist-mini-suivi-0804)
10. [🆕 Système de sauvegarde (BONUS)](#10-système-de-sauvegarde-bonus)

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
├── main.cpp                  ← Point d'entrée, boucle principale du jeu
│
├── Entity/
│   ├── Entity.h              ← Classe de base abstraite
│   ├── Entity.cpp
│   ├── Player.h              ← Hérite de Entity
│   ├── Player.cpp
│   ├── Monster.h             ← Hérite de Entity
│   └── Monster.cpp
│
├── Items/
│   ├── Item.h                ← Classe de base des objets
│   ├── Item.cpp
│   ├── HealItem.h            ← Hérite de Item (type HEAL)
│   └── HealItem.cpp
│
├── Inventory/
│   ├── Inventory.h           ← Composition : contient des Item*
│   └── Inventory.cpp
│
├── Act/
│   ├── ActAction.h           ← Représente une action ACT (id, texte, mercyImpact)
│   ├── ActAction.cpp
│   ├── ActCatalogue.h        ← Catalogue global des actions ACT (≥ 8)
│   └── ActCatalogue.cpp
│
├── Combat/
│   ├── Combat.h              ← Gère un combat entre Player et Monster
│   └── Combat.cpp
│
├── Bestiary/
│   ├── BestiaryEntry.h       ← Une entrée du bestiaire (monstre + résultat)
│   ├── BestiaryEntry.cpp
│   ├── Bestiary.h            ← Liste des entrées + affichage
│   └── Bestiary.cpp
│
├── Game/
│   ├── Game.h                ← Orchestre tout : menus, stats, boucle
│   └── Game.cpp
│
├── Utils/
│   ├── FileLoader.h          ← Lecture des fichiers CSV
│   ├── FileLoader.cpp
│   ├── SaveManager.h         ← 🆕 Sauvegarde / chargement de partie
│   └── SaveManager.cpp
│
└── Data/
    ├── items.csv             ← Inventaire initial
    ├── monsters.csv          ← Monstres disponibles
    └── saves/
        ├── slot1.sav         ← 🆕 Slot de sauvegarde 1
        ├── slot2.sav         ← 🆕 Slot de sauvegarde 2
        └── slot3.sav         ← 🆕 Slot de sauvegarde 3
```

---

## 3. Description détaillée de chaque classe

---

### 3.1 `Entity` (Entity/Entity.h + Entity.cpp)

**Rôle :** Classe de base abstraite représentant toute entité vivante du jeu.

**Attributs (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_name` | `std::string` | Nom de l'entité |
| `m_hp` | `int` | Points de vie actuels |
| `m_hpMax` | `int` | Points de vie maximum |
| `m_atk` | `int` | Valeur d'attaque (pour calcul de rand) |
| `m_def` | `int` | Valeur de défense |

**Méthodes :**
- Constructeur : `Entity(string name, int hp, int atk, int def)`
- Accesseurs/modificateurs : `getName()`, `getHp()`, `getHpMax()`, `getAtk()`, `getDef()`, `setHp(int)`
- `bool isAlive() const` → HP > 0
- `int takeDamage(int dmg)` → applique les dégâts (min 0), retourne les dégâts réels
- `virtual void display() const = 0` → **méthode virtuelle pure** → polymorphisme
- `virtual std::string getType() const = 0` → retourne "PLAYER" ou "MONSTER"

---

### 3.2 `Player` (Entity/Player.h + Player.cpp)

**Rôle :** Représente le joueur. Hérite de `Entity`.

**Attributs supplémentaires (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_inventory` | `Inventory` | Composition avec l'inventaire |
| `m_kills` | `int` | Nombre de monstres tués |
| `m_spared` | `int` | Nombre de monstres épargnés |
| `m_victories` | `int` | Nombre de victoires totales |

**Méthodes :**
- `Player(string name, int hp, int atk, int def)`
- `void display() const override`
- `std::string getType() const override` → "PLAYER"
- `Inventory& getInventory()`
- `void addKill()` / `void addSpared()` / `void addVictory()`
- `int getKills()` / `int getSpared()` / `int getVictories()`
- `void displayStats() const` → affiche un résumé complet
- `bool hasWon() const` → victories >= 10

---

### 3.3 `Monster` (Entity/Monster.h + Monster.cpp)

**Rôle :** Représente un monstre. Hérite de `Entity`.

**Type (enum class) :**
```cpp
enum class MonsterCategory { NORMAL, MINIBOSS, BOSS };
```

**Attributs supplémentaires (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_category` | `MonsterCategory` | Catégorie (NORMAL/MINIBOSS/BOSS) |
| `m_mercy` | `int` | Jauge Mercy actuelle (0-100) |
| `m_mercyGoal` | `int` | Seuil de Mercy pour épargner |
| `m_actIds` | `std::vector<std::string>` | IDs des actions ACT disponibles |

**Méthodes :**
- `Monster(string name, MonsterCategory cat, int hp, int atk, int def, int mercyGoal, vector<string> actIds)`
- `void display() const override`
- `std::string getType() const override` → "MONSTER"
- `MonsterCategory getCategory() const`
- `std::string getCategoryStr() const` → "NORMAL", "MINIBOSS", "BOSS"
- `int getMercy() const` / `int getMercyGoal() const`
- `void addMercy(int amount)` → borne entre 0 et mercyGoal
- `bool isMercyFull() const` → mercy >= mercyGoal
- `const std::vector<std::string>& getActIds() const`
- `int getActCount() const` → 2, 3 ou 4 selon catégorie
- `Monster clone() const` → retourne une copie fraîche du monstre (HP remis à max)

---

### 3.4 `Item` (Items/Item.h + Item.cpp)

**Rôle :** Classe de base abstraite pour les objets.

**Attributs (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_name` | `std::string` | Nom de l'objet |
| `m_type` | `std::string` | Type ("HEAL") |
| `m_value` | `int` | Valeur (HP rendus) |
| `m_quantity` | `int` | Quantité en stock |

**Méthodes :**
- `Item(string name, string type, int value, int quantity)`
- Accesseurs : `getName()`, `getType()`, `getValue()`, `getQuantity()`
- `bool use()` → diminue quantité de 1 si >0, retourne true si succès
- `virtual void applyEffect(Player& player) = 0` → **virtuelle pure**
- `virtual void display() const`

---

### 3.5 `HealItem` (Items/HealItem.h + HealItem.cpp)

**Rôle :** Objet de soin. Hérite de `Item`.

**Méthodes :**
- `HealItem(string name, int value, int quantity)`
- `void applyEffect(Player& player) override` → restaure `value` HP (max = hpMax)
- `void display() const override`

---

### 3.6 `Inventory` (Inventory/Inventory.h + Inventory.cpp)

**Rôle :** Composition dans Player. Gère la liste d'items.

**Attributs (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_items` | `std::vector<Item*>` | Liste des items (pointeurs) |

**Méthodes :**
- `~Inventory()` → destructeur libère la mémoire (delete)
- `void addItem(Item* item)`
- `void display() const` → liste tous les items
- `bool useItem(int index, Player& player)` → utilise l'item à l'index donné
- `int size() const`
- `Item* getItem(int index)`
- `bool isEmpty() const`

---

### 3.7 `ActAction` (Act/ActAction.h + ActAction.cpp)

**Rôle :** Représente une action ACT du catalogue.

**Attributs (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_id` | `std::string` | Identifiant unique (ex: "FLATTER") |
| `m_displayText` | `std::string` | Texte affiché au joueur |
| `m_mercyImpact` | `int` | Impact sur Mercy (+, -, ou 0) |

**Méthodes :**
- `ActAction(string id, string text, int mercyImpact)`
- `std::string getId()` / `std::string getDisplayText()` / `int getMercyImpact()`

---

### 3.8 `ActCatalogue` (Act/ActCatalogue.h + ActCatalogue.cpp)

**Rôle :** Catalogue global de toutes les actions ACT (≥ 8, dont ≥ 2 négatives).

**Attributs (privé statique) :**
```cpp
static std::map<std::string, ActAction> s_actions;
```

**Catalogue (minimum 8 actions) :**
| ID | Texte affiché | Impact Mercy |
|---|---|---|
| `FLATTER` | "Tu flattes le monstre avec éloquence." | +20 |
| `CHANTER` | "Tu chantes une mélodie apaisante." | +25 |
| `DANSER` | "Tu exécutes une danse bizarre." | +15 |
| `ENCOURAGER` | "Tu encourages le monstre." | +10 |
| `INTERROGER` | "Tu poses des questions sur sa vie." | +5 |
| `REGARDER` | "Tu observes le monstre en silence." | 0 |
| `INSULTER` | "Tu insultes sauvagement le monstre !" | -20 |
| `PROVOQUER` | "Tu provoques le monstre avec arrogance." | -30 |
| `MIMER` | "Tu mimes ses attaques de façon comique." | +10 |
| `PLEURER` | "Tu fonds en larmes devant le monstre." | +5 |

**Méthodes statiques :**
- `static void init()` → initialise la map
- `static const ActAction* getAction(const std::string& id)`
- `static void displayAvailable(const std::vector<std::string>& ids)` → affiche les actions disponibles

---

### 3.9 `BestiaryEntry` (Bestiary/BestiaryEntry.h + BestiaryEntry.cpp)

**Rôle :** Enregistre les données d'un monstre vaincu.

**Attributs (privés) :**
| Attribut | Type | Description |
|---|---|---|
| `m_monsterName` | `std::string` | Nom du monstre |
| `m_category` | `std::string` | Catégorie |
| `m_hp` | `int` | HP du monstre |
| `m_atk` | `int` | ATK du monstre |
| `m_def` | `int` | DEF du monstre |
| `m_wasKilled` | `bool` | true = tué, false = épargné |

**Méthodes :**
- Constructeur depuis un Monster + bool
- `void display() const`
- `bool wasKilled() const`

---

### 3.10 `Bestiary` (Bestiary/Bestiary.h + Bestiary.cpp)

**Rôle :** Collection de BestiaryEntry, affichée dans le menu.

**Attributs (privés) :**
```cpp
std::vector<BestiaryEntry> m_entries;
```

**Méthodes :**
- `void addEntry(const Monster& m, bool killed)`
- `void display() const`
- `int totalKilled() const`
- `int totalSpared() const`

---

### 3.11 `Combat` (Combat/Combat.h + Combat.cpp)

**Rôle :** Gère un combat complet entre le joueur et un monstre.

**Attributs (privés) :**
```cpp
Player& m_player;
Monster m_monster;   // copie fraîche du monstre
```

**Méthodes :**
- `Combat(Player& player, Monster monster)`
- `void run()` → boucle principale du combat
- `bool playerTurn()` → retourne false si combat terminé
- `bool monsterTurn()` → retourne false si joueur mort
- `int calcDamage(int defenderMaxHp)` → `rand(0, defenderMaxHp)`
- `void displaySeparator() const`
- `void displayStatus() const` → affiche HP joueur + HP monstre + Mercy
- `bool m_monsterKilled` / `bool m_monsterSpared` → résultat

**Résultat :**
```cpp
enum class CombatResult { PLAYER_DEAD, MONSTER_KILLED, MONSTER_SPARED };
CombatResult getResult() const;
```

---

### 3.12 `FileLoader` (Utils/FileLoader.h + FileLoader.cpp)

**Rôle :** Lecture des fichiers CSV.

**Méthodes statiques :**
```cpp
static std::vector<Item*> loadItems(const std::string& filepath);
static std::vector<Monster> loadMonsters(const std::string& filepath);
```

**Parsing `items.csv` :** `nom; type; valeur; quantite`
**Parsing `monsters.csv` :** `categorie; nom; hp; atk; def; mercyGoal; act1; act2; [act3]; [act4]`

---

### 3.12b 🆕 `SaveManager` (Utils/SaveManager.h + SaveManager.cpp)

**Rôle :** Gère la sérialisation et désérialisation de l'état complet du jeu dans des fichiers `.sav`. Classe **utilitaire statique** (pas d'instance).

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

**Méthodes statiques :**
```cpp
// Sauvegarde
static bool saveGame(const Player& player,
                     const Bestiary& bestiary,
                     int slot);            // slot : 1, 2 ou 3

// Chargement
static bool loadGame(Player& player,
                     Bestiary& bestiary,
                     int slot);

// Utilitaires
static bool saveExists(int slot);          // vérifie si Data/saves/slotX.sav existe
static void displaySaveSlots();            // affiche les 3 slots avec leur état
static std::string getSlotPath(int slot);  // → "Data/saves/slot1.sav"

// Parsing interne (privé)
static std::string serializeInventory(const Inventory& inv);
static void        deserializeInventory(Inventory& inv, const std::string& data);
static std::string serializeBestiary(const Bestiary& bestiary);
static void        deserializeBestiary(Bestiary& bestiary, const std::string& data);
```

**Méthodes à ajouter dans `Player` pour le chargement :**
```cpp
void setHp(int hp);
void setKills(int k);
void setSpared(int s);
void setVictories(int v);
```

**Méthodes à ajouter dans `Bestiary` pour le chargement :**
```cpp
void addEntryRaw(const std::string& name, const std::string& category,
                 int hp, int atk, int def, bool wasKilled);
void clear();
```

---

### 3.13 `Game` (Game/Game.h + Game.cpp)

**Rôle :** Chef d'orchestre. Contient le Player, le Bestiary, les monstres chargés et gère tous les menus.

**Attributs (privés) :**
```cpp
Player m_player;
Bestiary m_bestiary;
std::vector<Monster> m_monsterPool;
int m_roundResult; // pour les fins multiples
```

**Méthodes :**
- `Game()`
- `void init()` → charge les CSV, propose de charger une sauvegarde, crée le joueur
- `void run()` → boucle du menu principal
- `void showMainMenu()`
- `void showBestiary()`
- `void showStats()`
- `void showItems()`
- `void startCombat()`
- `void showEnding()` → affiche la fin selon kills/spared
- `Monster pickRandomMonster()` → tire un monstre au hasard dans le pool
- `void saveGame()` → 🆕 affiche les slots, demande le slot, appelle `SaveManager::saveGame()`
- `void loadGame()` → 🆕 affiche les slots, demande le slot, appelle `SaveManager::loadGame()`

---

## 4. Schéma UML

```
╔══════════════════════════════════════════════════════════════════════╗
║                          ALTERDUNE — UML                             ║
╚══════════════════════════════════════════════════════════════════════╝

┌──────────────────────────────┐
│         <<abstract>>         │
│           Entity             │
├──────────────────────────────┤
│ - m_name : string            │
│ - m_hp : int                 │
│ - m_hpMax : int              │
│ - m_atk : int                │
│ - m_def : int                │
├──────────────────────────────┤
│ + getName() : string         │
│ + getHp() / getHpMax() : int │
│ + isAlive() : bool           │
│ + takeDamage(int) : int      │
│ + display() = 0              │
│ + getType() = 0              │
└──────────────┬───────────────┘
               │ hérite
       ┌───────┴───────┐
       ▼               ▼
┌─────────────┐  ┌────────────────────────────┐
│   Player    │  │          Monster           │
├─────────────┤  ├────────────────────────────┤
│ - inventory │  │ - m_category : enum        │
│ - m_kills   │  │ - m_mercy : int            │
│ - m_spared  │  │ - m_mercyGoal : int        │
│ - m_victories│ │ - m_actIds : vector<str>   │
├─────────────┤  ├────────────────────────────┤
│ + display() │  │ + display()                │
│ + getInv()  │  │ + addMercy(int)            │
│ + addKill() │  │ + isMercyFull() : bool     │
│ + hasWon()  │  │ + getActIds()              │
└──────┬──────┘  │ + clone() : Monster        │
       │ compose └────────────────────────────┘
       ▼
┌─────────────────────────┐
│        Inventory        │
├─────────────────────────┤
│ - m_items : vector<Item*>│
├─────────────────────────┤
│ + addItem(Item*)         │
│ + useItem(int, Player&)  │
│ + display()              │
└─────────────────────────┘
       │ contient (composition)
       ▼
┌────────────────────────┐
│      <<abstract>>      │
│          Item          │
├────────────────────────┤
│ - m_name : string      │
│ - m_type : string      │
│ - m_value : int        │
│ - m_quantity : int     │
├────────────────────────┤
│ + use() : bool         │
│ + applyEffect() = 0    │
│ + display()            │
└───────────┬────────────┘
            │ hérite
            ▼
┌────────────────────────┐
│        HealItem        │
├────────────────────────┤
│ (pas d'attrib. supp.)  │
├────────────────────────┤
│ + applyEffect(Player&) │
└────────────────────────┘

┌───────────────────────────────┐
│         ActCatalogue          │
├───────────────────────────────┤
│ - s_actions:map<str,ActAction>│
├───────────────────────────────┤
│ + init()                      │
│ + getAction(id) : ActAction*  │
│ + displayAvailable(ids)       │
└───────────────────────────────┘
            │ contient
            ▼
┌────────────────────────┐
│       ActAction        │
├────────────────────────┤
│ - m_id : string        │
│ - m_displayText : str  │
│ - m_mercyImpact : int  │
├────────────────────────┤
│ + getId()              │
│ + getDisplayText()     │
│ + getMercyImpact()     │
└────────────────────────┘

┌──────────────────────────────┐
│           Combat             │
├──────────────────────────────┤
│ - m_player : Player&         │
│ - m_monster : Monster        │
│ - m_result : CombatResult    │
├──────────────────────────────┤
│ + run()                      │
│ + playerTurn() : bool        │
│ + monsterTurn() : bool       │
│ + calcDamage(int) : int      │
│ + getResult() : CombatResult │
└──────────────────────────────┘

┌──────────────────────────────┐
│           Bestiary           │
├──────────────────────────────┤
│ - m_entries : vector<Entry>  │
├──────────────────────────────┤
│ + addEntry(Monster, bool)    │
│ + display()                  │
│ + totalKilled()              │
│ + totalSpared()              │
└──────────────────────────────┘
            │ contient
            ▼
┌──────────────────────────────┐
│        BestiaryEntry         │
├──────────────────────────────┤
│ - m_monsterName : string     │
│ - m_category : string        │
│ - m_hp / m_atk / m_def       │
│ - m_wasKilled : bool         │
├──────────────────────────────┤
│ + display()                  │
│ + wasKilled() : bool         │
└──────────────────────────────┘

┌──────────────────────────────┐
│           FileLoader         │
├──────────────────────────────┤
│ (classe utilitaire statique) │
├──────────────────────────────┤
│ + loadItems(path) : vec<I*>  │
│ + loadMonsters(path): vec<M> │
└──────────────────────────────┘

┌──────────────────────────────┐
│       🆕 SaveManager         │
├──────────────────────────────┤
│ (classe utilitaire statique) │
├──────────────────────────────┤
│ + saveGame(Player,Bestiary,  │
│            slot) : bool      │
│ + loadGame(Player&,Bestiary&,│
│            slot) : bool      │
│ + saveExists(slot) : bool    │
│ + displaySaveSlots()         │
│ + getSlotPath(slot) : string │
└──────────────────────────────┘

┌──────────────────────────────┐
│             Game             │
├──────────────────────────────┤
│ - m_player : Player          │
│ - m_bestiary : Bestiary      │
│ - m_monsterPool: vec<Monster>│
├──────────────────────────────┤
│ + init()                     │
│ + run()                      │
│ + showMainMenu()             │
│ + startCombat()              │
│ + showEnding()               │
│ + saveGame() 🆕              │
│ + loadGame() 🆕              │
└──────────────────────────────┘

Relations :
  Game ──────composé────▶ Player
  Game ──────composé────▶ Bestiary
  Game ──────utilisé────▶ FileLoader
  Game ──────utilisé────▶ SaveManager 🆕
  Combat ───référence───▶ Player
  Combat ───possède─────▶ Monster (copie)
  Player ───composé─────▶ Inventory
  Inventory ─composé────▶ Item* (polymorphe)
```

---

## 5. Fichiers de données CSV

### `Data/items.csv`
```
nom; type; valeur; quantite
Potion de vie; HEAL; 30; 3
Grande Potion; HEAL; 60; 1
Elixir; HEAL; 100; 1
```

### `Data/monsters.csv`
```
categorie; nom; hp; atk; def; mercyGoal; act1; act2; act3; act4
NORMAL; Sableron; 50; 15; 5; 100; FLATTER; CHANTER;;
NORMAL; Dunecrok; 40; 20; 3; 100; DANSER; INTERROGER;;
NORMAL; Glisseur; 60; 10; 8; 100; ENCOURAGER; MIMER;;
MINIBOSS; Sablier Géant; 100; 25; 10; 100; FLATTER; CHANTER; DANSER;
MINIBOSS; Vipersand; 90; 30; 8; 100; INTERROGER; PLEURER; ENCOURAGER;
BOSS; Le Grand Dune; 200; 40; 15; 100; FLATTER; CHANTER; DANSER; INSULTER
BOSS; OmbreDesert; 180; 45; 12; 100; ENCOURAGER; MIMER; PLEURER; PROVOQUER
```

---

## 6. Flux de jeu (gameplay loop)

```
[DÉMARRAGE]
  └─▶ Saisie du nom du joueur
  └─▶ Chargement items.csv + monsters.csv
  └─▶ Affichage du résumé initial (nom, HP, inventaire)
  └─▶ ActCatalogue::init()

[MENU PRINCIPAL] (boucle tant que victories < 10 et player.isAlive())
  ├─▶ [1] Bestiaire
  │     └─▶ Affiche monstres vaincus (tués / épargnés)
  ├─▶ [2] Statistiques
  │     └─▶ Nom, HP, kills, spared, victories
  ├─▶ [3] Items
  │     └─▶ Affiche inventaire, option d'utiliser un objet
  ├─▶ [4] Démarrer un combat
  │     └─▶ Pick monstre aléatoire
  │     └─▶ [COMBAT] ──────────────────────────┐
  │           ├─▶ [FIGHT]   → damage → monstre │
  │           ├─▶ [ACT]     → modifier Mercy   │
  │           ├─▶ [ITEM]    → utiliser objet   │
  │           └─▶ [MERCY]   → épargner si ok   │
  │                │                           │
  │           Tour monstre : attaque joueur    │
  │                │                           │
  │           Fin combat :                      │
  │           ├─ Player mort → GAME OVER       │
  │           ├─ Monstre tué → addKill()       │
  │           └─ Monstre épargné → addSpared() │
  │     └─▶ addVictory() + addEntry(bestiary)  │
  ├─▶ [5] 🆕 Sauvegarder
  │     └─▶ Affiche les 3 slots (état + nom joueur si rempli)
  │     └─▶ Joueur choisit 1, 2 ou 3
  │     └─▶ SaveManager::saveGame() → écrit Data/saves/slotX.sav
  │     └─▶ Confirmation : "Partie sauvegardée dans le slot X !"
  └─▶ [6] Quitter ◀──────────────────────────┘

[FIN] (10 victoires atteintes)
  ├─▶ kills == 0  → FIN PACIFISTE
  ├─▶ spared == 0 → FIN GÉNOCIDAIRE
  └─▶ sinon       → FIN NEUTRE
```

**Au lancement (dans `init()`) 🆕 :**
```
[LANCEMENT]
  └─▶ Charge items.csv + monsters.csv + ActCatalogue::init()
  └─▶ Vérifie si au moins 1 slot de sauvegarde existe (SaveManager::saveExists)
  ├─▶ Si oui → "Une sauvegarde existe. Continuer une partie ? [O/N]"
  │     ├─▶ [O] → Affiche les slots disponibles → Joueur choisit → loadGame()
  │     └─▶ [N] → Nouvelle partie (saisie du nom)
  └─▶ Si non → Nouvelle partie directement (saisie du nom)
```

---

## 7. Plan d'implémentation (ordre de développement)

| Étape | Fichiers | Priorité |
|---|---|---|
| 1 | `ActAction.h/.cpp` + `ActCatalogue.h/.cpp` | 🔴 Premier |
| 2 | `Entity.h/.cpp` | 🔴 Premier |
| 3 | `Item.h/.cpp` + `HealItem.h/.cpp` | 🟠 Deuxième |
| 4 | `Inventory.h/.cpp` | 🟠 Deuxième |
| 5 | `Player.h/.cpp` | 🟠 Deuxième |
| 6 | `Monster.h/.cpp` | 🟠 Deuxième |
| 7 | `FileLoader.h/.cpp` + CSV | 🟡 Troisième |
| 8 | `BestiaryEntry.h/.cpp` + `Bestiary.h/.cpp` | 🟡 Troisième |
| 9 | `Combat.h/.cpp` | 🟢 Quatrième |
| 10 | `Game.h/.cpp` | 🟢 Quatrième |
| 11 | `SaveManager.h/.cpp` + dossier `Data/saves/` | 🔵 Bonus (après le jeu de base) |
| 12 | `main.cpp` | ✅ Final |

---

## 8. Points clés POO à démontrer

| Concept | Où | Comment |
|---|---|---|
| **Encapsulation** | Toutes les classes | Tous les attributs en `private`, accesseurs/modifieurs publics |
| **Héritage** | `Player` et `Monster` héritent de `Entity` | `HealItem` hérite de `Item` |
| **Polymorphisme** | `Entity::display()` virtuelle pure | `Item::applyEffect()` virtuelle pure |
| **Composition** | `Player` contient `Inventory` | `Inventory` contient `vector<Item*>` |
| **Lecture fichiers** | `FileLoader` | Parsing de `items.csv` et `monsters.csv` avec `ifstream` |
| **Gestion mémoire** | `Inventory` | Destructeur (`delete`) pour les `Item*` |
| **Enum class** | `MonsterCategory`, `CombatResult` | Typage fort |
| **Map/Collections** | `ActCatalogue` | `std::map<string, ActAction>` |
| **Fichiers (écriture)** | `SaveManager` | `ofstream` pour écrire les `.sav` |
| **Fichiers (lecture)** | `SaveManager` | `ifstream` + parsing clé=valeur pour charger |

---

## 9. Checklist mini-suivi 08/04

- [ ] `Entity.h` + `Entity.cpp` — classe abstraite avec attributs et méthodes virtuelles
- [ ] `Player.h` + `Player.cpp` — hérite de Entity, intègre Inventory
- [ ] `Monster.h` + `Monster.cpp` — hérite de Entity, enum MonsterCategory
- [ ] `Item.h` + `Item.cpp` — classe abstraite
- [ ] `HealItem.h` + `HealItem.cpp` — implémente applyEffect()
- [ ] `Inventory.h` + `Inventory.cpp` — composition, vector<Item*>
- [ ] `ActAction.h` + `ActCatalogue.h` — min 8 actions dont 2 négatives
- [ ] `FileLoader.h` — chargement CSV fonctionnel
- [ ] `Data/items.csv` + `Data/monsters.csv` — fichiers de données prêts
- [ ] **UML** dessiné sur papier/draw.io (conforme à ce document)
- [ ] `main.cpp` — affiche résumé initial (nom, HP, inventaire) après chargement CSV

---

## Notes et conventions de code

```cpp
// Conventions à respecter :
// - Noms de classes : PascalCase        → class Monster
// - Attributs privés : préfixe m_       → m_name, m_hp
// - Méthodes : camelCase                → getHp(), addKill()
// - Constantes/enum : UPPER_CASE        → MonsterCategory::BOSS
// - Fichiers : même nom que la classe   → Monster.h, Monster.cpp
// - Guards : #pragma once dans les .h

// Includes à utiliser :
#include <iostream>   // cout, cin
#include <fstream>    // ifstream
#include <sstream>    // stringstream (parsing CSV)
#include <vector>     // vector
#include <map>        // map (ActCatalogue)
#include <string>     // string
#include <cstdlib>    // rand(), srand()
#include <ctime>      // time() pour srand
#include <ofstream>   // ofstream (sauvegarde)
#include <ifstream>   // ifstream (chargement)
```

---

## 10. Système de sauvegarde (BONUS)

> 🎯 **Objectif :** Rapporter les +4 points bonus. Peu de binômes implémenteront ça.

### Format complet du fichier `.sav`

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
INVENTORY=Potion de vie;HEAL;30;2|Grande Potion;HEAL;60;1|Elixir;HEAL;100;0
BESTIARY=Sableron;NORMAL;50;15;5;KILLED|Dunecrok;NORMAL;40;20;3;SPARED
```

**Règles de format :**
- Chaque ligne = `CLE=VALEUR`
- `INVENTORY` : items séparés par `|`, champs séparés par `;` → `nom;type;valeur;quantite`
- `BESTIARY` : entrées séparées par `|`, champs séparés par `;` → `nom;categorie;hp;atk;def;KILLED|SPARED`
- Les lignes commençant par `#` sont des commentaires (ignorées à la lecture)

### Algorithme de sauvegarde (`saveGame`)

```cpp
bool SaveManager::saveGame(const Player& player, const Bestiary& bestiary, int slot) {
    std::ofstream file(getSlotPath(slot));
    if (!file.is_open()) return false;

    file << "# ALTERDUNE SAVE FILE v1\n";
    file << "PLAYER_NAME=" << player.getName() << "\n";
    file << "PLAYER_HP="   << player.getHp()   << "\n";
    file << "PLAYER_HP_MAX=" << player.getHpMax() << "\n";
    file << "PLAYER_ATK=" << player.getAtk() << "\n";
    file << "PLAYER_DEF=" << player.getDef() << "\n";
    file << "VICTORIES="  << player.getVictories() << "\n";
    file << "KILLS="      << player.getKills()     << "\n";
    file << "SPARED="     << player.getSpared()    << "\n";
    file << "INVENTORY="  << serializeInventory(player.getInventory()) << "\n";
    file << "BESTIARY="   << serializeBestiary(bestiary) << "\n";

    file.close();
    return true;
}
```

### Algorithme de chargement (`loadGame`)

```cpp
bool SaveManager::loadGame(Player& player, Bestiary& bestiary, int slot) {
    std::ifstream file(getSlotPath(slot));
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        // Séparer clé et valeur sur '='
        size_t sep = line.find('=');
        std::string key   = line.substr(0, sep);
        std::string value = line.substr(sep + 1);

        if      (key == "PLAYER_NAME")   player.setName(value);
        else if (key == "PLAYER_HP")     player.setHp(std::stoi(value));
        else if (key == "PLAYER_HP_MAX") player.setHpMax(std::stoi(value));
        else if (key == "PLAYER_ATK")    player.setAtk(std::stoi(value));
        else if (key == "PLAYER_DEF")    player.setDef(std::stoi(value));
        else if (key == "VICTORIES")     player.setVictories(std::stoi(value));
        else if (key == "KILLS")         player.setKills(std::stoi(value));
        else if (key == "SPARED")        player.setSpared(std::stoi(value));
        else if (key == "INVENTORY")     deserializeInventory(player.getInventory(), value);
        else if (key == "BESTIARY")      deserializeBestiary(bestiary, value);
    }
    file.close();
    return true;
}
```

### Affichage des slots (`displaySaveSlots`)

```
╔══════════════════════════════════╗
║       SAUVEGARDES ALTERDUNE      ║
╠══════════════════════════════════╣
║ [1] Louis      - 4 victoires     ║
║ [2] Maxime     - 7 victoires     ║
║ [3] [VIDE]                       ║
╚══════════════════════════════════╝
Choisissez un slot (1-3) :
```

### Points d'implémentation à ne pas oublier

- [ ] Créer le dossier `Data/saves/` au premier lancement s'il n'existe pas
- [ ] Gérer le cas où un slot est corrompu (erreur de parsing → message d'erreur propre)
- [ ] Ajouter `setName()`, `setAtk()`, `setDef()`, `setHpMax()` dans `Entity`
- [ ] Ajouter `setKills()`, `setSpared()`, `setVictories()` dans `Player`
- [ ] Ajouter `clear()` + `addEntryRaw()` dans `Bestiary`
- [ ] Option dans le menu principal : `[5] Sauvegarder`
- [ ] Proposition de chargement au démarrage si un slot existe

### Ajout dans le menu principal (résumé)

| Option | Action |
|---|---|
| `[5] Sauvegarder` | Affiche les 3 slots, écrit dans le slot choisi |
| Au lancement | Détecte les saves existantes, propose de charger |
