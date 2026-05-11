---
config:
  layout: elk
---
classDiagram
direction TB
    class Entity {
	    #m_name : string
	    #m_hp : int
	    #m_hpMax : int
	    #m_atk : int
	    #m_def : int
	    +getName() string
	    +getHp() int
	    +getHpMax() int
	    +getAtk() int
	    +getDef() int
	    +setHp(hp: int) void
	    +isAlive() bool
	    +takeDamage(dmg: int) int
	    +display() void*
	    +getType() string*
    }

    class Player {
	    -m_inventory : Inventory
	    -m_kills : int
	    -m_spared : int
	    -m_victories : int
	    +display() void
	    +getType() string
	    +getInventory() Inventory&
	    +addKill() void
	    +addSpared() void
	    +getKills() int
	    +getSpared() int
	    +getVictories() int
	    +setKills(k: int) void
	    +setSpared(s: int) void
	    +setVictories(v: int) void
	    +setName(n: string) void
	    +loadStateHp(hp: int, maxHp: int) void
	    +setStats(atk: int, def: int) void
	    +displayStats() void
	    +hasWon() bool
    }

    class Monster {
	    -m_category : MonsterCategory
	    -m_mercy : int
	    -m_mercyGoal : int
	    -m_actIds : vector~string~
	    +display() void
	    +getType() string
	    +getCategory() MonsterCategory
	    +getCategoryStr() string
	    +getMercy() int
	    +getMercyGoal() int
	    +addMercy(amount: int) void
	    +isMercyFull() bool
	    +getActIds() vector~string~
	    +getActCount() int
	    +clone() Monster
    }

    class Inventory {
	    -m_items : vector~Item*~
	    +~Inventory()
	    +getItems() vector~Item*~
	    +addItem(item: Item*) void
	    +useItem(index: int, player: Player&) bool
	    +display() void
	    +isEmpty() bool
	    +hasUsableItems() bool
	    +size() int
    }

    class Item {
	    #m_name : string
	    #m_type : string
	    #m_value : int
	    #m_quantity : int
	    +getName() string
	    +getType() string
	    +getValue() int
	    +getQuantity() int
	    +use() bool
	    +applyEffect(player: Player&) void*
	    +display() void
    }

    class HealItem {
	    +applyEffect(player: Player&) void
	    +display() void
    }

    class ActAction {
	    -m_id : string
	    -m_displayText : string
	    -m_mercyImpact : int
	    +getId() string
	    +getDisplayText() string
	    +getMercyImpact() int
	    +display() void
    }

    class ActCatalogue {
	    -s_actions : map~string, ActAction~$
	    +init() void$
	    +getAction(id: string) ActAction*$
	    +displayAvailable(ids: vector~string~, out: vector~ActAction~&) void$
    }

    class Combat {
	    -m_player : Player&
	    -m_monster : Monster
	    -m_result : CombatResult
	    +Combat(player: Player&, monster: Monster)
	    +run() void
	    +getResult() CombatResult
	    -tourJoueur() bool
	    -tourMonstre() bool
	    -calculerDegats(atk: int) int
	    -afficherEcranCombat(msg: string) void
	    -afficherApparitionPokemon() void
	    -afficherVictoire() void
	    -makeHpBar(hp: int, hpMax: int, barLen: int) string
	    -lireAsciiLines() vector~string~
	    -getCouleurMonstre() string
	    -getAsciiFilePath() string
    }

    class BestiaryEntry {
	    +m_monsterName : string
	    +m_category : string
	    +m_hp : int
	    +m_atk : int
	    +m_def : int
	    +m_wasKilled : bool
	    +display() void
	    +wasKilled() bool
    }

    class Bestiary {
	    -m_entries : vector~BestiaryEntry~
	    +addEntry(m: Monster, killed: bool) void
	    +addRawEntry(name: string, cat: string, hp: int, atk: int, def: int, killed: bool) void
	    +display() void
	    +clear() void
	    +getEntries() vector~BestiaryEntry~
	    +totalKilled() int
	    +totalSpared() int
    }

    class GameManager {
	    -m_player : Player*
	    -m_bestiary : Bestiary
	    -m_monsterPool : vector~Monster~
	    -jeuEnCours : bool
	    +GameManager()
	    +~GameManager()
	    +demarrer() void
	    +showMainMenu() void
	    +lancerCombat() void
	    +showEnding() void
    }

    class FileLoader {
	    +loadItems(filepath: string, player: Player&) void$
	    +loadMonsters(filepath: string) vector~Monster~$
	    -trim(s: string) string$
    }

    class SaveManager {
	    +saveGame(filename: string, player: Player, bestiary: Bestiary) bool$
	    +loadGame(filename: string, player: Player&, bestiary: Bestiary&) bool$
	    -trim(s: string) string$
    }

	<<abstract>> Entity
	<<abstract>> Item
	<<struct>> BestiaryEntry

    Entity <|-- Player
    Entity <|-- Monster
    Item <|-- HealItem
    Player "1" *-- "1" Inventory
    Inventory "1" *-- "*" Item
    Bestiary "1" *-- "*" BestiaryEntry
    GameManager "1" o-- "1" Player
    GameManager "1" *-- "1" Bestiary
    Combat "1" --> "1" Player : utilise&
    Combat "1" *-- "1" Monster : possede (copie)
    ActCatalogue "1" *-- "*" ActAction : contient
    GameManager ..> FileLoader : appelle
    GameManager ..> SaveManager : appelle
    GameManager ..> Combat : instancie
    GameManager ..> ActCatalogue : init()