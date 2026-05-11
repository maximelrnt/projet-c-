# 🎮 Guide de Révision — ALTERDUNE (C++ POO)
> Interrogation orale 30 minutes — Tout ce que tu dois savoir

---

## 🗺️ Vue d'ensemble du projet

**ALTERDUNE** est un RPG console inspiré d'Undertale. Le joueur affronte des monstres en choisissant entre les combattre (FIGHT), les amadouer (ACT), utiliser des objets (ITEM) ou les épargner (MERCY). Le but : **10 victoires** (tuer OU épargner compte).

### Architecture — Les 4 couches

```
main.cpp
  └── GameManager          ← chef d'orchestre
        ├── Player          ← le joueur (hérite Entity)
        │     └── Inventory ← composition (contient des Item*)
        │           └── HealItem ← hérite Item
        ├── Monster         ← hérite Entity
        ├── Bestiary        ← liste des monstres vaincus
        ├── FileLoader      ← lecture CSV (statique)
        ├── SaveManager     ← sauvegarde/chargement (statique)
        └── Combat          ← boucle de combat
              ├── ActCatalogue ← dictionnaire static des actions
              └── ActAction   ← une action pacifiste
```

---

## 🔥 Classes qui vont tomber — et comment les expliquer

---

### 1. `Entity` (classe abstraite) ⭐⭐⭐

**Concept clé : classe de base avec méthodes virtuelles pures**

```cpp
class Entity {
protected:
    string m_name;
    int m_hp, m_hpMax, m_atk, m_def;
public:
    virtual void display() const = 0;   // méthode virtuelle PURE
    virtual string getType() const = 0; // = 0 → oblige les sous-classes
    virtual ~Entity() {}               // destructeur virtuel !
};
```

**Comment l'expliquer :**
> « Entity est une classe abstraite — on ne peut pas l'instancier directement. Elle définit l'interface commune à Player et Monster. Les méthodes `display()` et `getType()` sont **pures** (`= 0`), ce qui force chaque sous-classe à les implémenter. Le destructeur est `virtual` pour que si on delete via un pointeur Entity*, le bon destructeur soit appelé. »

**Question piège possible :** *Pourquoi `protected` et pas `private` pour les attributs ?*
> « Parce que les sous-classes (Player, Monster) ont besoin d'y accéder directement. Si c'était `private`, même les classes filles ne pourraient pas les lire. »

---

### 2. `Player` vs `Monster` — Héritage ⭐⭐⭐

**Concept clé : héritage + composition**

```cpp
class Player : public Entity {
private:
    Inventory m_inventory;  // COMPOSITION : Player "possède" un Inventory
    int m_kills, m_spared, m_victories;
public:
    string getType() const override { return "PLAYER"; }
    bool hasWon() const { return m_victories >= 10; }
};

class Monster : public Entity {
private:
    MonsterCategory m_category;  // enum class
    int m_mercy, m_mercyGoal;
    vector<string> m_actIds;
public:
    Monster clone() const;       // retourne une copie fraiche
};
```

**Comment expliquer la différence :**
> « Player a un inventaire par **composition** (il en est propriétaire, si le Player est détruit, l'inventaire l'est aussi). Monster a une jauge `mercy` et des IDs d'actions. Les deux héritent de Entity. »

**Question piège :** *Pourquoi Monster a un `clone()` ?*
> « Parce que quand on lance un combat, on veut une **copie fraîche** du monstre (HP remis à max). Si le joueur affrontait le monstre original du pool, il le modifierait définitivement. Clone remet les HP à `m_hpMax`. »

**Enum class :**
```cpp
enum class MonsterCategory { NORMAL, MINIBOSS, BOSS };
// "class" interdit la conversion implicite vers int → plus sûr
```

---

### 3. `Item` (abstraite) → `HealItem` ⭐⭐⭐

**Concept clé : polymorphisme sur les items**

```cpp
class Item {
public:
    virtual void applyEffect(Player& player) = 0;  // pure
    bool use() {
        if (m_quantity <= 0) return false;
        m_quantity--;
        return true;
    }
};

class HealItem : public Item {
public:
    void applyEffect(Player& player) override {
        int nouveauHp = player.getHp() + m_value;
        if (nouveauHp > player.getHpMax()) nouveauHp = player.getHpMax();
        player.setHp(nouveauHp);
    }
};
```

**Forward declaration dans Item.h :**
```cpp
class Player;  // au lieu de #include "Player.h"
```
> « On utilise une **forward declaration** pour éviter les inclusions circulaires : Item.h inclut Player.h qui inclurait Item.h... Comme Item.h n'a besoin que d'une référence `Player&`, une déclaration suffit. »

---

### 4. `Inventory` — Gestion mémoire ⭐⭐⭐

**Concept clé : tableau de pointeurs + RAII**

```cpp
class Inventory {
private:
    vector<Item*> m_items;  // pointeurs → polymorphisme possible
public:
    ~Inventory() {
        // le destructeur libère la mémoire !
        for (Item* it : m_items) delete it;
    }
};
```

**Pourquoi des pointeurs ?**
> « Si on stockait des `Item` par valeur, on perdrait le polymorphisme (slicing). Avec des `Item*`, quand on appelle `applyEffect()`, C++ appelle la bonne version (HealItem::applyEffect) grâce au dispatch virtuel. »

**Question piège :** *Qu'est-ce qui se passe si on copie un Inventory ?*
> « C'est un problème ! La copie par défaut copierait les pointeurs (shallow copy), et les deux Inventory pointeraient sur les mêmes objets — double delete à la destruction. Ici on a pas implémenté le copy constructor, donc attention. »

---

### 5. `Combat` — Cœur du jeu ⭐⭐⭐

**La boucle principale :**
```cpp
void Combat::run() {
    afficherMonstreApparition();
    while (m_player.isAlive() && m_monster.isAlive()) {
        bool continuer = tourJoueur();
        if (!continuer) break;
        if (!m_monster.isAlive()) { m_result = MONSTER_KILLED; break; }
        bool joueurVivant = tourMonstre();
        if (!joueurVivant) { m_result = PLAYER_DEAD; break; }
    }
}
```

**Distinction référence vs copie :**
```cpp
class Combat {
    Player& m_player;  // RÉFÉRENCE : on modifie le vrai joueur
    Monster m_monster; // COPIE : on ne touche pas l'original du pool
};
```
> « Player est une **référence** car on doit modifier ses HP réels. Monster est une **copie** car on a déjà cloné le monstre avant de créer le Combat — ça protège le pool de monstres. »

**Les 4 actions du joueur :**
| Touche | Action | Effet |
|--------|--------|-------|
| 1 | FIGHT | Inflige des dégâts, réduit aussi la mercy |
| 2 | ACT | Lance une action pacifiste (via ActCatalogue) |
| 3 | ITEM | Utilise un objet de l'inventaire |
| 4 | MERCY | Épargne si mercy pleine, sinon message |

**Mécanique mercy lors d'une attaque (FIGHT) :**
```cpp
// si on attaque, la mercy diminue aussi !
int penalty = dmg + (oldMercy / 5);
m_monster.addMercy(-penalty);
```
> « Attaquer est contradictoire avec épargner — si le joueur frappe, il perd de la mercy accumulée. »

**Réduction dégâts monstre selon mercy :**
```cpp
// Plus la mercy est haute, moins le monstre attaque fort
double pourcentage = (double)mercy / mercyGoal;
atk = atk - (int)(atk * pourcentage);
if (atk < 1) atk = 1;
```

**Calcul dégâts aléatoire :**
```cpp
int Combat::calculerDegats(int atk) {
    return rand() % atk + 1;  // entre 1 et atk
}
```

---

### 6. `ActCatalogue` — Patron Registre Statique ⭐⭐

**Concept clé : classe utilitaire avec `static`**

```cpp
class ActCatalogue {
private:
    static map<string, ActAction> s_actions;  // unique, partagée
public:
    static void init();
    static const ActAction* getAction(const string& id);
};
```

> « ActCatalogue est une **classe utilitaire** — on ne l'instancie jamais. La `map` est `static`, donc partagée par tout le programme. `init()` est appelé une seule fois au démarrage. On utilise une `map<string, ActAction>` pour récupérer une action en O(log n) par son identifiant comme `"FLATTER"`. »

**`getAction` retourne un pointeur :**
```cpp
const ActAction* getAction(const string& id) {
    auto it = s_actions.find(id);
    if (it != s_actions.end()) return &(it->second);
    return nullptr;  // pas trouvé → nullptr
}
```
> « On retourne un **pointeur const** (pas une copie) pour éviter de copier l'objet. Si l'ID n'existe pas, on retourne `nullptr` — d'où l'importance de vérifier avant d'utiliser. »

**ActAction — impact aléatoire :**
```cpp
int getMercyImpact() const {
    if (m_mercyImpact > 0) {
        int variance = (rand() % 31) - 10;  // entre -10 et +20
        int resultat = m_mercyImpact + variance;
        if (resultat < 5) resultat = 5;      // plancher à 5
        return resultat;
    }
    // impact négatif → malus supplémentaire aléatoire
}
```

---

### 7. `FileLoader` — Lecture CSV Statique ⭐⭐

**Concept clé : parsing ligne par ligne**

```cpp
// Toutes les méthodes sont static → pas besoin d'instance
class FileLoader {
public:
    static void loadItems(const string& filepath, Player& player);
    static vector<Monster> loadMonsters(const string& filepath);
private:
    static string trim(const string& s);  // enlève les espaces
};
```

**Comment le parsing fonctionne :**
```cpp
stringstream ss(ligne);
string cat, nom, hp;
getline(ss, cat, ';');   // sépare par ';'
getline(ss, nom, ';');
getline(ss, hp,  ';');
// ...
Monster m(nom, mc, stoi(hp), stoi(atk), stoi(def), stoi(mGoal), acts);
pool.push_back(m);
```
> « On lit le CSV ligne par ligne avec `getline`. Chaque ligne est parsée dans un `stringstream`, et on extrait les champs séparés par `;`. `stoi()` convertit les strings en int. `trim()` nettoie les espaces parasites. »

---

### 8. `SaveManager` — Sérialisation ⭐⭐

**Format du fichier :**
```
# ALTERDUNE SAVE FILE v1
PLAYER_NAME=Maxime
PLAYER_HP=85
VICTORIES=3
INVENTORY=Potion;HEAL;30;2|Elixir;HEAL;60;1
BESTIARY=Garfield;NORMAL;80;15;5;KILLED|Shrek;BOSS;200;40;20;SPARED
```

**Logique de sauvegarde :**
```cpp
// inventaire : items séparés par | , champs par ;
for (int i = 0; i < items.size(); i++) {
    file << items[i]->getName() << ";" << items[i]->getType() 
         << ";" << items[i]->getValue() << ";" << items[i]->getQuantity();
    if (i < items.size() - 1) file << "|";
}
```

**Logique de chargement :**
```cpp
// on sépare clé=valeur
size_t pos = line.find('=');
string key = line.substr(0, pos);
string val = line.substr(pos + 1);

if (key == "PLAYER_NAME") player.setName(val);
else if (key == "INVENTORY") { /* parsing nested avec | et ; */ }
```

> « La sauvegarde est **textuelle** (pas binaire) — ce qui est lisible et portable. On utilise deux niveaux de séparateurs : `|` entre les items, `;` entre leurs champs. Au chargement on vérifie d'abord le header `# ALTERDUNE SAVE FILE v1` pour valider le format. »

---

### 9. `GameManager` — Chef d'orchestre ⭐⭐

**Cycle de vie du jeu :**
```
demarrer()
  → crée Player (new)
  → ActCatalogue::init()
  → charge save OU FileLoader::loadItems + loadMonsters
  → showMainMenu()  ← boucle principale
       [1] lancerCombat() → clone monstre → Combat::run() → traite résultat
       [2] afficher inventaire
       [3] afficher bestiaire
       [4] afficher stats
       [5] SaveManager::saveGame()
       [6] quitter
  → showEnding() (3 fins possibles)
```

**Gestion mémoire du Player :**
```cpp
class GameManager {
    Player* m_player;  // POINTEUR (créé dynamiquement)
public:
    GameManager() : m_player(nullptr), jeuEnCours(true) {}
    ~GameManager() { delete m_player; }  // libéré dans le destructeur
};
```

**Sélection aléatoire du monstre :**
```cpp
static std::mt19937 rng(std::random_device{}());  // Mersenne Twister
std::uniform_int_distribution<int> uni(0, pool.size() - 1);
int index = uni(rng);
Monster monstre = m_monsterPool[index].clone();  // copie fraîche !
```

**3 fins selon les stats :**
| Condition | Fin |
|-----------|-----|
| `kills == 0` | 🔵 FIN PACIFISTE |
| `spared == 0` | 🔴 FIN GÉNOCIDAIRE |
| Mixte | 🟡 FIN NEUTRE |

---

## ❓ Questions pièges à préparer

### Sur l'héritage / polymorphisme
- **Q : Qu'est-ce qu'une classe abstraite ?**
  > Une classe avec au moins une méthode virtuelle pure (`= 0`). On ne peut pas l'instancier. Elle sert à définir une interface commune.

- **Q : Pourquoi `virtual ~Entity()` ?**
  > Sans ça, si on fait `Entity* e = new Player(...)` puis `delete e`, seul le destructeur de Entity est appelé → fuite mémoire sur les ressources de Player.

- **Q : C'est quoi le slicing ?**
  > Si on stocke `Player p = *entity_ptr`, on perd la partie "Player" et on garde seulement la partie "Entity". C'est pour ça qu'on utilise des pointeurs/références pour le polymorphisme.

### Sur la mémoire
- **Q : Qui libère les Item* dans l'Inventory ?**
  > Le destructeur d'Inventory fait `delete it` pour chaque Item*. Inventory "possède" ses items (composition).

- **Q : Pourquoi `m_player` est un pointeur dans GameManager ?**
  > Pour pouvoir le créer *après* la construction de GameManager (on a besoin du nom saisi par l'utilisateur). Avec une valeur, il faudrait le construire dans la liste d'initialisation.

### Sur le design
- **Q : Pourquoi FileLoader et SaveManager sont statiques ?**
  > Ce sont des classes utilitaires sans état. Pas besoin d'en créer une instance — elles n'ont pas d'attributs membres. C'est plus propre et lisible que des fonctions libres.

- **Q : Pourquoi une `map` dans ActCatalogue et pas un `vector` ?**
  > La `map` permet une recherche par clé (`"FLATTER"`) en O(log n). Avec un vector, il faudrait parcourir toute la liste. Et le code est plus lisible : `s_actions["FLATTER"]` plutôt qu'un index.

- **Q : Qu'est-ce que la composition vs l'héritage ?**
  > Héritage : "est-un" (Player est une Entity). Composition : "a-un" (Player a un Inventory). L'inventaire fait partie du joueur — il est créé et détruit avec lui.

- **Q : Pourquoi `#pragma once` au lieu de include guards ?**
  > `#pragma once` est une extension non-standard mais supportée par tous les compilateurs modernes. Elle évite les inclusions multiples du même header. Plus concise que `#ifndef`.

### Sur le combat
- **Q : Comment fonctionne la mercy ?**
  > Chaque monstre a une jauge `mercy` (0..mercyGoal). Les actions ACT l'augmentent (avec aléatoire). Attaquer (FIGHT) la diminue. Quand elle est pleine, MERCY devient disponible pour épargner le monstre.

- **Q : Que retourne `tourJoueur()` ?**
  > Un `bool` : `false` si le combat doit s'arrêter immédiatement (monstre tué ou épargné), `true` pour continuer la boucle et passer au tour du monstre.

- **Q : Pourquoi `CombatResult` est une `enum class` et pas juste un int ?**
  > `enum class` est fortement typé — `PLAYER_DEAD` ne peut pas être comparé par erreur à un int ou à un autre enum. Plus sûr, plus lisible.

---

## 📊 Résumé des concepts POO utilisés

| Concept | Où c'est utilisé |
|---------|-----------------|
| **Classe abstraite** | Entity, Item |
| **Héritage** | Player : Entity, Monster : Entity, HealItem : Item |
| **Polymorphisme** | `display()`, `getType()`, `applyEffect()` via pointeurs |
| **Composition** | Player a un Inventory |
| **Destructeur virtuel** | Entity::~Entity() |
| **Méthodes statiques** | FileLoader, SaveManager, ActCatalogue |
| **Enum class** | MonsterCategory, CombatResult |
| **Forward declaration** | `class Player;` dans Item.h |
| **Références** | `Player& m_player` dans Combat, `Player& player` dans applyEffect |
| **Pointeurs & new/delete** | Inventory (Item*), GameManager (Player*) |
| **std::map** | ActCatalogue::s_actions |
| **std::mt19937** | Sélection aléatoire du monstre |

---

## 🚀 Flux complet d'un combat (à réciter)

1. `GameManager::lancerCombat()` tire un monstre au hasard dans le pool
2. Il appelle `clone()` pour avoir une copie fraîche
3. Il crée `Combat combat(*m_player, monstre)` et appelle `combat.run()`
4. `run()` affiche l'apparition, puis boucle tant que les deux sont vivants
5. `tourJoueur()` : le joueur choisit FIGHT/ACT/ITEM/MERCY
6. `tourMonstre()` : le monstre attaque (réduit selon mercy)
7. Le combat se termine : `CombatResult` indique qui a gagné
8. `GameManager` traite le résultat : `addKill()` ou `addSpared()`, ajoute au bestiaire
9. Si 10 victoires → `showEnding()` avec la fin appropriée

