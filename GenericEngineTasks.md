# Tasks #

  * [Zones](GenericEngineTasks#Zones.md)
  * [Phases](GenericEngineTasks#Phases.md)
    * Steps?
  * [Energies](GenericEngineTasks#Energies/Money/Mana.md) (mana)
  * [Card Values](GenericEngineTasks#Card_values.md) (ie name, type, subtype, power, toughness, text, cost, etc)
  * [Card Types](GenericEngineTasks#Card_Types/subtypes.md)
  * [Card Subtypes](GenericEngineTasks#Card_Types/subtypes.md) (I know these two overlap somewhat with values, but in most games there is a very specific list of Types, and then often each type has a specific list of allowed subtypes, ie lands can be Forest, Island, Plains, Mountain, or Swamp)
  * [Player Values](GenericEngineTasks#Player_Values_(Life,_counters).md) (Life being the first obvious example, although not all games have it. Also poison counters from mtg.)
  * [Combat](GenericEngineTasks#Combat.md) (What can attack, what can be attacked, when attacks can occur, how they are resolved, etc.)


## Zones ##
### Current State ###
(As of the SVN in 2011/05/11)

Zones are hardcoded as objects of the Player's "game" attribute.  Some zones (main example is "inplay") still have strong hardcoded stuff (for example, when a card moves from inplay to inplay, we avoid sending "moved to play" events, etc...).

### Todo ###
  * It should be relatively easy to create a generic "zone" object that has a "name", which would then make it easy to get rid of most hardcoded zone-related code.
  * Someone contacted me with a game concept where "geography" matters. The "battlefield" is a 20x20 grid, and where cards are on this grid is important for the game. Should those be 400 different zones? This sounds over the top... WotC's "eye of the judgement" or "Triple Triad" also come to mind, although those have only a 3x3 grid, which sounds much more reasonable...

#### Detailed "Generic Zones" system ####
  * List of zones for each player must be declared somewhere in the rules (mod rules?). A name is probably enough. Zone rules themselves (how they interact with each other) are declared somewhere else in the modrules. Graphical behavior of the zones should be declared somewhere else (to allow themes). The goal is to link a "zone" (defined by its id) to a graphical renderer
  * Zone rules (how a card can move to or from this zone, etc...)  need to be defined (replace "putIntoPlay" hardcoded rule, etc...
    * We already know that "moveTo" is not always good enough, because MTG requires for example to make a difference between something played "normally" and something moved on the battlefield by a "putintoplay" action... require an additional flag maybe ?
  * Once zones are described outside of the code, we need to become more flexible with card databases. "auto" won't mean anything once "inplay" is not a special zone anymore, "draw" is not precise enough anymore, same for "shuffle", etc.... In order for the card code to not become entirely unreadable, a system of "macros" should be designed (top of each primitives files sounds the best? Maybe an include?). example: _#define auto= autoinplay=_
    * Careful of not introducing infinite loops or recursion errors ?
  * Zones are attached to renderers/controllers. Those are hardcoded in C++ but need to be designed so that they can render "any" zone (== ordered group of card)
    * Which controller renders what zone is defined in a theme xml. The following is just an example, not sure of the details yet
```
<zone>
  <id>inplay1</id>
  <renderer name="PriorityListRenderer">
      <subzone>
          <filter>creature[attacking]</filter>
          <renderer name="CombatRenderer">
              <x>0</x>
              <y>SCREEN_WIDTH/2</x>
              <width>...</width>
              <height>SCREEN_WIDTH/4</height>
              <rows>1</rows>
              <order>LeftToRight</order>
          </renderer>
      </subzone>
      <subzone>
          <filter>creature</filter>
          <renderer name="DefaultCardsRenderer">
              <x>0</x>
              <y>SCREEN_WIDTH/2 + 100</x>
              <width>...</width>
              <height>SCREEN_WIDTH/4</height>
              <rows>1</rows>
              <order>LeftToRight</order>
          </renderer>
      </subzone>
      <subzone>
          <filter>land</filter>
          <renderer name="DefaultCardsRenderer">
              <x>0</x>
              <y>3 * SCREEN_WIDTH/4</x>
              <width>...</width>
              <height>SCREEN_WIDTH/4</height>
              <rows>1</rows>
              <order>LeftToRight</order>
          </renderer>
      </subzone>
      <subzone>
          <filter>enchantment,artifact</filter>
          <renderer name="DefaultCardsRenderer">
              <x>0</x>
              <y>SCREEN_WIDTH/2</x>
              <width>...</width>
              <height>SCREEN_WIDTH/4</height>
              <rows>3</rows> 
              <order>TopToBottom,LeftToRight</order>
          </renderer>
      </subzone>
  </renderer>
</zone>
<zone>
  <id>player1</id>
  <renderer name="AvatarContainerRenderer">
      <x>...</x>
      <y>...</y>
      <subzones>
          <id>graveyard1</id> <-- will render a card back for each, and those will be displayed when clicked on ?
          <id>Library1</id>
      </subzones>
  </renderer>
</zone>
```
    * Ideas:
      * RegularZoneRenderer (for inplay). Parameters: hide (0/1) see if cards need to be shown or hidden (could allow to render opponents hand easily), x, y (upper left corner), width, height (size of the component), columns, rows (number of colum/rows to render ?)
      * PriorityListRenderer: series of subrenderers, that render sub-parts of the zone, based on targetChooser definitions (see example above). Allows to have several renderers for inplay. The first renderer that matches a card gets to render that card.
      * AvatarContainer: renders Avatar, plus a list of "card backs", that once activated, activate the rendering of other zones (library, graveyard, exile, whatever...)
      * Remove/Add events already exist in GuiCombat, we will need to reuse similar things
      * t is ok to render outside of the limts of a zone (x/y/width/height) for the sake of animation. use actX,actY VS x,y from the Pos objects. Once an object is "marked" as belonging to graveyard, graveyard is in charge of rendering it, including the animation that brings it from inplay to graveyard. actX can be outside of the current zone, x shouldn't (assertion failure)
      * Will need a global "CardView" pool for continuity of the rendering
      * Render "Overlord" has a list of all renderers in order. ReceiveEvent becomes ReceiveEventMinus (loop all renderers) then ReceiveEventPlus (loop all renderers) <-- note, same for priorityList described above?

## Phases ##
### Current State ###
(As of the SVN in 2011/05/11)

rules/.txt allow to define a "customphasering", which allows to reorganize the existing phases "as you wish". It doesn't allow to create entirely new phases yet.

Limitations of what a player can/can't do during a phase is hardcoded

### Todo ###

## Energies/Money/Mana ##
### Current State ###
(As of the SVN in 2011/05/11)

  * Currently mana colors are deeply hardcoded
  * WotC have a patent on "energy producing cards"

### Todo ###

## Card values ##
### Current State ###
(As of the SVN in 2011/05/11)

### Todo ###
  * Rendering could be described in XML (what do we want to render on the card, where on the card do we want to render it)
  * Defining attributes themselves generically sounds like a hard task
  * May Want to take a look at how GCCG does it, although their documentation is horrible. [gccg.sourceforge.net]

## Card Types/subtypes ##
### Current State ###
(As of the SVN in 2011/05/11)

Types are hardcoded

Subtypes are already entirely generic, except for a few exceptions that need to be handled a bit better (lots of land subtypes for example...)

### Todo ###

## Player Values (Life, counters) ##
### Current State ###
(As of the SVN in 2011/05/11)

All of this is currently hardcoded.

### Todo ###
One easy step would be to create a map associating a string to an int ("life" => 20").

## Combat ##
### Current State ###
(As of the SVN in 2011/05/11)

This is completely hardcoded.

### Todo ###

# In Progress #

If anyone is currently working on any of these they should say so somewhere on this page so that we don't get any duplication of effort from multiple people working on the same. It would also be a good idea to list it on the [Projects](Projects.md) page.
- Erwan: working on zones as of 2011/06. Updated this wiki with my work in progress on the Todo list

## Wtactics support ##
  * How to choose north/South zone when a card comes into play? Define a new "putintoplay" rule ?
  * How to handle piles of face down cards for resources? How to allow creation of new piles?
    * Resource piles reconfiguration
  * Mulligan rules are slightly different from the ones we have <-- make an outside rule?
  * How to represent North/South? <-- See "zones" WIP above. I suggest to represent north and south with left/right, makes more sense visually to me than above/below
  * New card values (faction, etc...) -> How to graphically represent new printed values? (xml to define card components in mods rules + xml to define the rendering of those)
    * WTactics: color is associated to faction
  * Cost: only gold
  * Threshold?
  * Equipemt carry/type limitation rule (attacker must declare equipment at attack phase).
    * card goes to graveyard, still equipped? Rules are unclear
  * Events == instants, can be done with langugage file change
  * Magic == rituals, can be done with language file change
  * MTG Layer rule is simpified, "can VS can't" rule. How to move the layer rule outside of the code?
  * Resolve combat creature by creature (How to order creatures? Rules are not clear)

# Reference #
Firemox was an attempt at a generic game engine. They have an xml file describing lots of game components, we can take inspiration from that.
http://sourceforge.net/projects/firemox/files/