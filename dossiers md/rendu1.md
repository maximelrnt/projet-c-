# Mini-suivi 1 - ALTERDUNE

**Binôme :** Louis Le Forestier & Maxime  
**Projet :** ALTERDUNE (Mini-RPG inspiré de Undertale)

## 1. UML Initial


![UML](UML2.png)

---

## 2. Avancées techniques

Nous avons pris une avance significative sur les attentes du mini-suivi. Les éléments suivants sont **déjà implémentés et fonctionnels** :

- **Architecture robuste en `.h` / `.cpp`** : L'ensemble de la base de code est scindée proprement. Nos premières classes principales `Entity` (classe mère abstraite), `Player` et `Monster` sont pleinement opérationnelles avec leurs getters/setters et méthodes associées.
- **Lecture de fichiers data (CSV) dynamique** : La classe utilitaire `FileLoader` permet de parser les fichiers externes `items.csv` (liste des objets) et `monsters.csv` (caractéristiques et actions des ennemis).
- **Système de menus et boucle de jeu** : La classe `GameManager` a été mise en place, offrant au joueur un menu principal pour naviguer vers ses statistiques, son inventaire ou le bestiaire.
- **Premiers Tests (Démarrage du Combat Complet)** : Le coeur du jeu, délégué à la classe `Combat`, a été modélisé. Les 4 options de l'inspiraton Undertale sont testables : attaquer (**FIGHT**), interagir avec le monstre (**ACT** via notre `ActCatalogue`), se soigner (**ITEM** de l'`Inventory`), ou tenter d'épargner (**MERCY**).
- **Polymorphisme et Collections** : Le système d'inventaire manipule un vecteur de pointeurs vers la classe abstraite parent (`Item*`), permettant l'utilisation de la méthode virtuelle pure `applyEffect()` en toute sécurité (et la désallocation mémoire est protégée par un destructeur redéfini).
- *(Bonus)* **Sauvegardes** : Un `SaveManager` a été intégré en avance pour pouvoir sauvegarder (_Serialize_) l'arbre du joueur et de ses actions passées, puis le recharger (_Deserialize_).

---

## 3. Clarté des explications : Justification de nos choix

Nous avons profité du démarrage pour asseoir de très bonnes bases de **Programmation Orientée Objet**.

- **Abstraction et Héritage**:`Entity` s’est imposé comme classe globale abstraite regroupant les comportements communs que partagent le Joueur et les Monstres (avoir des points de vie, de l'attaque, de la défense, subir des dégâts `takeDamage()`). Ses méthodes virtuelles pures (`display()` et `getType()`) posent le contrat de base que tout être vivant dans le jeu se doit de redéfinir, et assure la possibilité de gérer plusieurs entités d'origines différentes sous des pointeurs `Entity*` à l'avenir si besoin.
- **Composition et Sécurité mémoire** : La classe `Player` compose intimement avec `Inventory` (c'est une composition de cycle de vie : le joueur instancie son inventaire et il disparaît avec lui). L'inventaire lui, agrège des objets sous forme de pointeurs, de manière à permettre la destructuration d'entités concrètes variées sans _slicing_ via le destructeur.
- **Design Pattern (Stratégie & OCP de SOLID)** : `Inventory` ne sachant manipuler que des classes génériques `Item*`, nous garantissons d'avoir respecté le concept de classe _Ouverte à l'extension mais Fermée à la modification_ : demain nous ajouterons des `WeaponItem`, ni le Joueur ni l'Inventaire ne changeront.
- **Principe de Responsabilité unique (SRP)** : Au lieu de confier le parsing fastidieux des CSV ou des saves `.sav` au `GameManager` (qui aurait pu devenir une lourde "God Class"), nous avons isolé ces tâches complexes d'Entrée/Sortie dans les classes purement utilitaires statiques `FileLoader` et `SaveManager`. Le GameManager se concentre ainsi uniquement sur la boucle de menus.
