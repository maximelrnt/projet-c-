# Plan d'implementation : ALTERDUNE (Projet C++)

## REGLES D'OR DU PROJET (Binome)

1. **Un paragraphe par classe** : Au tout debut de chaque fichier .h, un bloc commentaire C++ explique en quelques phrases le role global de la classe, ce qu'elle represente dans le jeu et comment elle interagit avec les autres. Pas de commentaires ligne par ligne.
2. **Ce fichier fait reference** : implementation_plan.md est la source de verite. On y revient a chaque fois qu'on commence une nouvelle classe ou phase.
3. **Comprehension mutuelle** : Les deux membres du binome doivent pouvoir relire n'importe quelle partie du code et comprendre a quoi elle sert.

---

## 1. Architecture des Classes

Entity  (classe abstraite)
- Player
- Monster  (classe abstraite)
  - NormalMonster
  - MiniBossMonster
  - BossMonster

### Description des classes

#### Entity (Classe abstraite de base)
Represente tout ce qui est vivant dans le jeu. Elle regroupe les attributs communs a tous les personnages (nom, HP actuels, HP max) et declare les methodes virtuelles pures que toutes les sous-classes devront implementer (attaquer, afficherInfos). Grace a cette classe, on peut manipuler un Player et un Monster via le meme type de pointeur.

#### Player (Herite de Entity)
Represente le joueur humain. Il possede un inventaire d'objets (composition avec Item), un compteur de monstres tues et de monstres epargnes, et un compteur de victoires globales. C'est cet objet qui determine quelle fin sera debloquee a la 10e victoire.

#### Monster (Herite de Entity, abstraite)
Represente un ennemi generique. Chaque monstre possede une jauge de Mercy, un identifiant de categorie, et une liste d'identifiants d'actions ACT disponibles. Les sous-classes (NormalMonster, MiniBossMonster, BossMonster) heritent de cette classe pour valider le polymorphisme demande par l'enonce.

#### Item
Represente un objet de l'inventaire du joueur. Seul le type HEAL existe pour l'instant. L'objet connait son nom, sa valeur de soin et sa quantite disponible.

#### ActionAct
Represente une action non-violente que le joueur peut effectuer pendant son tour (inspire d'Undertale). Chaque action possede un identifiant, un texte affiche, et un impact sur la jauge Mercy du monstre. Le catalogue (minimum 8 actions) est defini en dur dans le code.

#### Bestiary
Sert de journal de bord des monstres rencontres. Il enregistre pour chaque monstre vaincu ses caracteristiques et s'il a ete tue ou epargne. S'affiche dans le menu principal.

#### GameManager
Chef d'orchestre du jeu. Il gere le demarrage (saisie du nom, chargement CSV), la boucle principale du menu simulation, et la verification de la condition de fin (10 victoires).

#### CombatSystem
Gere le deroulement complet d'un combat. Il alterne les tours joueur/monstre, gere les choix (FIGHT, ACT, ITEM, MERCY), calcule les degats aleatoires et retourne le resultat du combat.

---

## 2. Fichiers de Donnees

items.csv :  nom; type; valeur; quantite
monsters.csv : categorie; nom; hp; atk; def; mercyGoal; act1; act2; [act3; act4]

---

## 3. Feuille de Route (Phases)

### Phase 0 : Planification (DONE)
- [x] Lire et comprendre l'enonce
- [x] Definir l'architecture des classes
- [x] Rediger ce fichier

### Phase 1 : Structures de base et CSV
- [ ] Creer Item
- [ ] Creer ActionAct + catalogue des 8 actions
- [ ] Creer Bestiary
- [ ] Implementer le parseur CSV
- [ ] Creer les fichiers CSV de test

### Phase 2 : Entites (Heritage et Polymorphisme)
- [ ] Creer Entity (classe abstraite)
- [ ] Creer Player avec inventaire (composition)
- [ ] Creer Monster et les 3 sous-classes

### Phase 3 : Menu Principal
- [ ] Boucle principale du GameManager
- [ ] Afficher Statistiques, Bestiaire, Inventaire
- [ ] Lancer un combat aleatoire

### Phase 4 : Systeme de Combat
- [ ] Menu FIGHT / ACT / ITEM / MERCY
- [ ] Formule de degats aleatoires
- [ ] Tour du monstre
- [ ] Conditions de fin de combat

### Phase 5 : Fins Multiples et Finitions
- [ ] Declencher la fin apres 10 victoires
- [ ] Logique Genocidaire / Pacifiste / Neutre
- [ ] Nettoyage du code

### Phase 6 : Interface Visuelle (Bonus)
- [ ] Amelioration affichage console (ASCII art, couleurs)
- [ ] Optionnel : Lib graphique (ncurses, SFML)

---

## 4. Evaluation

| Etape         | Date  | Poids | Criteres |
|---|---|---|---|
| Mini-suivi    | 08/04 | 20%   | UML initial, structure du code, premieres classes |
| Soutenance P1 | 29/04 | 80%   | UML final, POO complete, gameplay fonctionnel |
| Soutenance P2 | 06/05 | --    | QR individuel sur la maitrise du code |

Bonus : Jusqu'a +4 points pour un projet sur-saillant.
