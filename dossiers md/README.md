# Projet : Programmation Orientée Objet en C++ - ALTERDUNE

**Auteur :** Daniel Wladdimiro  
**Institution :** ESILV  
**Contact :** daniel.wladdimiro@devinci.fr  
**Période :** Second semestre 2025-2026  

---

## 1. Introduction
La programmation orientée objet (POO) est un paradigme fondamental pour le développement de logiciels structurés et modulaires. Ce projet doit être réalisé en binôme. Il propose la création d'un mini-jeu de type RPG nommé ALTERDUNE, où les entités (joueur, monstres, objets) sont modélisées sous forme de classes. 

## 2. Description du Projet
Le projet consiste à développer un jeu par console structuré autour des éléments suivants : 
* Un menu principal hors combat. 
* Des combats au tour par tour. 
* Un système de fins multiples basé sur les choix du joueur. 
* Le chargement de données depuis deux fichiers obligatoires. 

### 2.1 Objectif du projet
L'objectif est de mettre en pratique les concepts suivants :
* **L'encapsulation** (attributs privés, accesseurs/modificateurs). 
* **L'héritage** (joueur/monstres). 
* **Le polymorphisme** (comportements spécifiques selon la catégorie de monstre). 
* **La composition** (inventaire, actions ACT, bestiaire). 
* **La lecture de fichiers** pour initialiser les objets. 

---

## 3. Démarrage d'une partie
Au lancement du programme : 
1. Le joueur saisit le nom de son personnage. 
2. Le jeu charge `items.csv` (inventaire initial) et `monsters.csv` (ennemis possibles). 
3. Le jeu affiche un résumé incluant le nom, les HP (actuel et maximum) et la liste des items avec leurs quantités. 

---

## 4. Condition de fin de partie et fins multiples
Une partie se termine lorsque le joueur obtient 10 victoires. Le jeu affiche alors une fin selon l'historique :
* **Fin Génocidaire :** Le joueur a tué tous les monstres vaincus (0 épargné).
* **Fin Pacifiste :** Le joueur a épargné tous les monstres vaincus (0 tué).
* **Fin Neutre :** Le joueur a à la fois tué et épargné des monstres.

---

## 5. Monstres, catégories et actions ACT

### 5.1 Catégories de monstres
La catégorie détermine le nombre d'actions ACT disponibles :
* **NORMAL :** 2 actions.
* **MINIBOSS :** 3 actions.
* **BOSS :** 4 actions. 

### 5.2 Système Mercy
Chaque monstre possède une jauge Mercy (de 0 à 100). Si la jauge est $\ge 100$, le joueur peut épargner le monstre pour gagner le combat sans le tuer. 

### 5.3 Catalogue d'actions ACT
Les actions sont pré-définies dans le code (map ou vector) et possèdent un identifiant, un texte affiché et un impact sur la jauge Mercy (positif, négatif ou nul).
**Contraintes :**
* Minimum 8 actions différentes dans le catalogue.
* Au moins 2 actions avec un impact négatif.
* La jauge Mercy est bornée entre 0 et l'objectif (ex. 100).

---

## 6. Menu de Simulation (Menu principal)
Options disponibles :
* **Bestiaire :** Affiche les monstres vaincus avec leurs caractéristiques (nom, catégorie, statistiques) et le résultat (tué ou épargné).
* **Statistiques :** Affiche le nom, les HP, le nombre de tués/épargnés et le nombre de victoires.
* **Items :** Affiche l'inventaire et permet d'utiliser un objet (soin) même hors combat.
* **Démarrer un combat :** Lance un combat aléatoire contre un ennemi du fichier CSV.
* **Quitter.**

---

## 7. Combat
Le combat oppose le joueur à un monstre jusqu'à ce que l'un des deux soit vaincu.

### 7.1 Menu de combat
À chaque tour, le joueur choisit entre : **FIGHT, ACT, ITEM, MERCY**.

### 7.2 Règles de dégâts
Les dégâts sont calculés aléatoirement à chaque attaque :
$$deg\hat{a}ts = rand(0, HP_{defenseur}^{max})$$
* Si dégâts $= 0$, l'attaque est ratée.
* Les HP ne peuvent pas descendre sous 0.

### 7.3 Actions du joueur
* **FIGHT :** Attaque directe. Si HP du monstre $= 0$, il est tué.
* **ACT :** Choix d'une action selon la catégorie du monstre pour modifier la jauge Mercy.
* **ITEM :** Utilise un objet et consomme le tour.
* **MERCY :** Épargne le monstre si Mercy $\ge 100$.

### 7.4 Tour du monstre
Le monstre attaque le joueur selon la même formule. Si le joueur tombe à 0 HP, la partie est perdue immédiatement.

---

## 8. Chargement des Données

### 8.1 items.csv
Format : `nom; type; valeur; quantite`
* `type` : Limité à `HEAL`.
* `valeur` : Points de vie rendus.

### 8.2 monsters.csv
Format : `categorie; nom; hp; atk; def; mercyGoal; act1; act2; act3; act4`
* Les catégories `NORMAL`, `MINIBOSS` et `BOSS` utilisent respectivement 2, 3 ou 4 actions.

---

## 9. Évaluation

| Étape | Poids | Critères principaux |
| :--- | :--- | :--- |
| **Mini-suivi (08/04)** | 20% | UML initial, structure du code, premières classes. |
| **Soutenance P1 (29/04)** | 80% | UML final, POO (héritage, encapsulation, polymorphisme), lecture fichiers, gameplay. |
| **Soutenance P2 (06/05)** | - | Session Q&R sur la maîtrise individuelle du code. |

**Bonus :** Jusqu'à +4 points pour un projet "sur-salient".  
**Formule :** $Note = 0.2 \times Note_{mini} + 0.8 \times Note_{soutenance} + bonus - penalite$