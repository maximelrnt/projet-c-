# Justification de l'Architecture — ALTERDUNE

## Structure des dossiers

```
Projetcpp/
├── main.cpp
├── core/
│   ├── Entity.h
│   ├── Monster.h / Monster.cpp
│   └── Player.h / Player.cpp
├── items/
│   ├── Item.h / Item.cpp
│   ├── HealItem.h / HealItem.cpp
│   └── Inventory.h / Inventory.cpp
├── combat/
│   ├── Combat.h / Combat.cpp
│   ├── ActionAct.h / ActionAct.cpp
│   └── ActCatalogue.h / ActCatalogue.cpp
├── managers/
│   ├── GameManager.h / GameManager.cpp
│   ├── SaveManager.h / SaveManager.cpp
│   ├── FileLoader.h / FileLoader.cpp
│   └── Bestiary.h / Bestiary.cpp
├── Data/     ← ASCII art des monstres
└── csv/      ← Données des monstres et items
```

---

## Principes OOP appliqués

### 1. `core/` — Hiérarchie d'entités (Héritage & Polymorphisme)

Ce dossier regroupe la **classe abstraite `Entity`** et ses deux sous-classes concrètes `Player` et `Monster`.

- **Héritage** : `Player` et `Monster` héritent tous deux de `Entity`, partageant les attributs communs (`m_name`, `m_hp`, `m_atk`, `m_def`) et les méthodes virtuelles (`display()`, `getType()`).
- **Polymorphisme** : `Combat` manipule des entités via des références à `Entity`, sans connaître leur type exact au moment de la compilation.
- **Encapsulation** : Les attributs sont `protected` dans `Entity` pour être accessibles aux sous-classes uniquement.

> Regrouper ces trois classes ensemble reflète directement la relation d'héritage visible dans le diagramme UML.

---

### 2. `items/` — Système d'inventaire (Composition & Héritage)

Ce dossier contient la hiérarchie des objets du jeu.

- **Héritage** : `HealItem` hérite de la classe abstraite `Item`, qui définit l'interface commune (`use()`, `applyEffect()`, `display()`). D'autres types d'items peuvent être ajoutés ici facilement.
- **Composition** : `Player` **contient** un objet `Inventory` (composition forte via `m_inventory`). `Inventory` gère un `vector<Item*>` et est responsable de la destruction des items (RAII via le destructeur).
- **Encapsulation** : L'inventaire est completement encapsulé dans `Player`, accessible uniquement via `getInventory()`.

> Ce dossier regroupe tout ce qui touche aux objets possédés par le joueur, formant une unité cohérente et isolée.

---

### 3. `combat/` — Logique de combat (Single Responsibility Principle)

Ce dossier isole tout ce qui concerne le déroulement d'un combat.

- **SRP (Single Responsibility Principle)** : `Combat` est responsable uniquement de l'orchestration du tour de jeu (FIGHT, ACT, ITEM, MERCY). Aucune autre classe ne gère cette logique.
- **ActAction & ActCatalogue** : Les actions pacifistes (ACT) sont modélisées séparément. `ActCatalogue` est une classe utilitaire statique qui centralise le catalogue d'actions, et `ActAction` représente une action individuelle.
- **Association** : `Combat` référence un `Player&` et un `Monster` par valeur — il orchestre leur interaction sans les posséder.

> Séparer le combat dans son propre module évite que `GameManager` devienne une classe "god object".

---

### 4. `managers/` — Gestionnaires de services (Responsabilité unique, classes statiques)

Ce dossier regroupe les classes dont le rôle est de **gérer des ressources ou coordonner des opérations transversales**.

- **GameManager** : Chef d'orchestre global. Il possède le `Player`, le `Bestiary` et le pool de monstres. Il délègue le combat à `Combat` et la persistence à `SaveManager`. C'est le seul point d'entrée du jeu après `main()`.
- **SaveManager** : Classe utilitaire statique responsable uniquement de la lecture/écriture du fichier `.sav`. Elle respecte le SRP en ne faisant que de la sérialisation.
- **FileLoader** : Classe utilitaire statique dédiée au parsing des fichiers CSV. Elle sépare la logique de chargement des données du reste de la logique métier.
- **Bestiary** : Gère la liste des `BestiaryEntry`, un registre des monstres rencontrés. Il est naturellement aux côtés des managers car il est chargé/sauvegardé par `SaveManager` et utilisé par `GameManager`.

> Ces classes sont regroupées car elles coordonnent toutes des ressources du jeu, sans elles-mêmes être des entités du jeu.

---

## Tableau récapitulatif des dépendances

| Module | Dépend de |
|---|---|
| `core/` | Aucune dépendance interne |
| `items/` | `core/` (forward declaration de `Player`) |
| `combat/` | `core/` |
| `managers/` | `core/`, `items/`, `combat/` |
| `main.cpp` | `managers/` uniquement |

> Les dépendances sont **unidirectionnelles** et **du général vers le spécifique**, ce qui évite les dépendances circulaires et facilite la maintenance.

---

## Commande de compilation

```bash
g++ -std=c++17 \
  main.cpp \
  core/Monster.cpp core/Player.cpp \
  items/Item.cpp items/HealItem.cpp items/Inventory.cpp \
  combat/ActionAct.cpp combat/ActCatalogue.cpp combat/Combat.cpp \
  managers/Bestiary.cpp managers/FileLoader.cpp managers/SaveManager.cpp managers/GameManager.cpp \
  -o alterdune
```
