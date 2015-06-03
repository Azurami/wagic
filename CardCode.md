# Create your own cards! #

**This is an explanation on how to add your own cards to Wagic. Please try to create your own sets and share them with everyone! If you have questions, feel free to ask them in our [Cards/Sets creation forum](http://wololo.net/forum/viewforum.php?f=8).**

Information in <font color='#008000'>green writing</font> pertains only to the SVN version of Wagic, these features have been added _after_ the latest release version.

Information is current as of 02/20/2011.

This is a work in progress. Please report any errors, ambiguities, or incomprehensible passages to our forum. Some statements in this document still require checking, these have been marked with (??).

# File structure #

The cards used in Wagic can be found in the _Res/sets/_ folder. This folder contains several subfolders, one for every set.

Each of these subfolders contains a "cards.dat_" file. As long as this file is included in a subdirectory of the "sets" folder, the cards it describes will be automatically loaded into the game._

For example: Having a file **_Res/sets/MYSet/_cards.dat**_will automatically create a set called MYSet, which contains all cards specified in this_cards.dat file, whenever you start the game.

So what does this _cards.dat file look like? Well, you can open one of those with any text editor (Notepad, for example), and you'll see that it is a list of:_

<pre>
[card]<br>
(content)<br>
[/card]<br>
</pre>

So what can be put in contents?

Contents is a list of keys and values spearated by a "=" sign. the keys can be the following:

*** [[#name|name]]
  * [[#primitive|primitive]]
  * [[#grade|grade]]
  * [[#id|id]]
  * [[#rarity|rarity]]
  * [[#mana|mana]]
  * [[(cost)|kicker](#kicker.md)]
  * [[#color|color]]
  * [[#type|type]]
  * [[#subtype|subtype]]
  * [[#power|power]]
  * [[#toughness|toughness]]
  * [[#text|text]]
  * [[#target|target]]
  * [[#abilities|abilities]]
  * [[#auto|auto]]
  * [[#autohand|autohand]]
  * [[#autograveyard|autograveyard]]
  * [[#alias|alias]]**

**name, id, rarity, and type are compulsory**. The other ones, in some circumstances, are needed as well (such as toughness and power for "regular" creatures). Text is compulsory if you do not plan to add pictures (or plan to add pictures that do not have that text) to your set.

# name #

**Description:** name denotes the name of the card

**Example:** name=Grizzly Bears

**Further information:** The engine currently adds names to the list of types, to make cards like "Plague Rats" work. This shouldn't affect anything else, but if you're giving a card a name that is also a type (for example, naming a card "Goblin"), we advise thorough testing.

# primitive #

**Description:** primitive tells the engine to read all information for that card from a list of "primitives", which already contains an entry of this name.

**Example:** primitive=Grizzly Bears

**Usage:** The idea of this feature is to allow you to add "reprints" of cards into your set very easily. For example, if you want to add a reprint of "Royal Assassin" to your set, you only have to specify its id, its rarity, and the line "primitive=Royal Assassin". Since Royal Assassin has already been coded (and inlcuded in the primitives file), all other data for the card will be taken from there.

This also means that you don't need to update your set when wechange the coding of Royal Assassin - your set will always load the information from the primitives file, which in this case will contain the updated version of Royal Assassin.


# grade #
**Description:** gives a hint on the quality of the code of the card, how well the card respects the actual rules printed on it

**Example:** grade=Borderline

**Usage:** Currently 5 values are accepted for the grade: **Supported, Borderline, Unofficial, Crappy, Unsupported, Dangerous**. When this line is not specified, the default grade is "Supported".
Here are the meanings of these values:
  * **Supported**: Wagic handles this card 100% correctly, or the issues encountered with it are minor. This card can go into an AI Deck
  * **Borderline**: Wagic handles this card correctly in most cases. In some edge cases, the card will not work as expected but this shouldn't be too much of an issue (example: Drain Life can use non black mana). Please think about it twice before putting such a card in an AI Deck
  * **Unofficial**: The person who coded this card claims that you can use it, and according to this person, this card works as advertised. The only difference with such a card and a "Borderline" card is that the Wagic team hasn't had the time to review this card yet and mark it as "Supported" or "Borderline".
  * **Crappy**: Wagic handles this card, but there are some side effects that can make it completely unbalanced. For example, an effect supposed to end at the end of the turn will last as long as the card is in play... DO NOT add a card marked as "Crappy" into an AI Deck
  * **Unsupported**: The card doesn't work, mostly. For example for a creature, it can mean that its special abilities are not supported at all, and it will just be an expensive Vanilla creature. This is just for people who want to play with the card because they like the art, for example. Do not put these cards in an AI Deck
  * **Dangerous**: This card can potentially crash Wagic. Even if it works 99% of the time, that's not our concern here. Of course, do not put those into an AI Deck!

Additionally, Wagic players have an option to choose which cards they want to have in their database. By default, this option is set to "Borderline", which means that both Supported and Borderline cards will be loaded at startup. Players who want to experiment with less safe cards can change this value. Next time the game is restarted, it will load the additional cards.

It is the role of the cards creators and developers to determine if a card goes to "Supported", "Borderline", or "Crappy". The line between these 3 categories is extremely thin, and the decision can take into account the popularity of the card, the inconvenience caused by the bugs, the difficulty involved with solving the bugs, etc... As a policy, the Wagic team will not accept bug reports for cards that are marked as "Crappy", "Unsupported", or "Dangerous". This can also be used as a factor in the decision to rank a card (especially between crappy and Borderline).

Note: it is possible to assign a grade to an entire primitive file by setting the grade at the very top of your file, BEFORE the first [card](card.md) tag.

# id #

**Description:** id is a _unique integer_ identifying the card. This id cannot be the same as any other id used in this set or in any other set.

**Example:** id=174908

**Usage:** To keep ids unique, it's best (for Magic cards) to use the id referenced at the Gatherer site. For example:
http://ww2.wizards.com/gatherer/CardDetails.aspx?&id=49  (Bad Moon alpha), the id is 49, as you can see from the number at the end of the URL.

For your own sets, try to choose an id range that no other sets use. Official sets use ids between 1 and about 250,000, so for your own sets, we suggest using values between 1,000,000 (one million) and 2,000,000,000 (two billion).

The engine stores collections and decks as lists of card ids. If a card is part of a collection, and its id gets changed, then the engine will notice that it does not have a card of that id in its database, and remove it from the collection.

The id will also be used to retrieve the picture for the card. For the card with the id 1,234,567 in set MySet, the engine will look for a card picture with the path and filename Res/sets/MySet/1234567.jpg, and a thumbnail picture Res/sets/MySet/thumbnails/1234567.jpg

Thumbnails must have a size of 45x64 pixels, and pictures must be 200x285.

# rarity #

**Description:** rarity denotes the probability of a card to be found in a booster pack. Valid values are _C, U, R, M, L, T_ and respectively mean Common, Uncommon, Rare, Mythic Rare, Land, Token.

**Example:** rarity=U _(for an Uncommon card)_

**Notes:** Currently, a booster pack will have 1 card with rarity L, 10 cards with rarity C, 3 cards with rarity U, one card with rarity R (which has a 1/8 chance to be of of rarity M instead), and no cards of rarity T.

The "T" rarity can be used to create tokens with complex rules (see below).

# mana #

**Description:** mana denotes the cost needed to play the card from the hand. Each part of the cost is enclosed in braces {}. Valid values are _{W}, {U}, {B}, {R},_ and _{G}_ (meaning (W)hite, bl(U)e, (B)lack, (R)ed, and (G)reen respectively), and also integers like _{3}, {16},_ or _{0}_, which denote costs that can be paid with mana of any color.

To represent _hybrid mana_, you put two mana symbols inside the same braces.

Additional valid values:
  * {D} _(Discarding a random card)_
  * {E}/{E(target|zone)} _(Exiles something)_
  * {H}/{H(target|zone)} _(Returns something to your hand)_
  * {L} _(life loss)_
  * {M} _(Puts a card from the top of you library)_
  * {s2l} _(put   **target|whereever**  on top of your library)_
  * {X} or {X}{X} _(now 2 {X} are possible! (Card example:Decree of Justice))_
  * {l2e} _(Exiles the top card of you library )_

**Examples:**
  * mana={W}{R}{R}   _(one white and two red mana)_
  * mana={3}{G}    _(1 green mana, and 3 mana of any color)_
  * mana={U}{WB}   _(1 blue mana, and one mana which can be either white or black)_
  * mana={2R}{2R}{2R}   _(3 red mana, but each one of those could also be paid by 2 mana of any other colors)_

**Limitation:** While it is possible in kicker costs (see [[(cost)|kicker (cost)](#kicker.md)] for these), alternate costs, buyback or flashback costs, it is currently not possible to specify any additional or alternative costs (like sacrificing or tapping cards) in the mana= line. Some kinds of additional costs can be snuck in by other means, see [[cost|cost of activated abilities](#auto.md)].

# kicker (cost) #

**Description:** kicker denotes an additional cost that may be paid when playing the card. It uses the same format as [[cost|cost of activated abilities](#auto.md)]. You can specify effects for a card which apply only if its kicker cost was paid (see [[(condition)|kicker (condition)](#kicker.md)] for these).

**Example:** A card that gives you 1 life if you pay its normal cost of {W}, but an additional 4 life if you pay an additional cost of {1}{W}, could be coded like this:

<pre>
mana={W}<br>
kicker={1}{W}<br>
auto=life:1 controller<br>
auto=kicker life:4 controller<br>
</pre>

# other (cost) #

**Description:** other denotes an alternate cost that may be paid instead of the mana cost, to play the card. It uses the same format as [[cost|cost of activated abilities](#auto.md)]. You can specify effects for a card which apply only if its "other" cost was paid (see [[(condition)|alternative (condition)](#alternative.md)] for these).

# flashback (cost) #

**Description:** flashback denotes an alternate cost that may be paid to play the card from a graveyard instead of playing it from your hand.It uses the same format as [[cost|cost of activated abilities](#auto.md)]. Such a card will be moved to your "exile" zone after being played through it flashback cost. Therefore this cost makes sense only for sorceries and instants

# buyback (cost) #

**Description:** buyback denotes an alternate cost that may be paid when playing the card. It uses the same format as [[cost|cost of activated abilities](#auto.md)]. If the buyback cost was paid, the card will go back to your hand instead of going to the graveyard once it resolves. Therefore buyback only makes sense for sorceries and instants. Note that buyback is an additional cost in MTG, but is described as an alternate cost in Wagic. So you need to include the original mana cost as well

**Example:** A card that gives you 1 life if you pay its normal cost of {W}, but goes back to your hand if you pay an additional cost of {1}{W}, could be coded like this:

<pre>
mana={W}<br>
buyback={1}{W}{W}<br>
auto=life:1 controller<br>
</pre>

# color #

**Description:** color denotes the color of the card. Usually colors get determined automatically from the [[#mana|mana]] cost, but some cards have special rules which determine their color(s). For these, the color key is provided. Valid values are _white, blue, black, red, green, artifact_

**Example:** color=red

**Limitation:** You can only set one color this way. It is not possible to specify several different colors.

# type #

**Description:** type denotes the type(s) and supertype(s) of the card, space separated. Although anything's fine in there, most used values are: _Sorcery, Instant, Creature, Artifact, Artifact Creature, Enchantment, Land_. These can be prefixed by the following: "Legendary", "Snow", "Basic",...

**Examples:**
  * type=Enchantment
  * type=Legendary Creature
  * type=Basic Land

**Limitations:**
  * While you can specify types and subtypes on two separate lines, <font color='green'>the engine currently treats only a few specific hardcoded values as "types" or "supertypes", the rest is treated as subtypes</font>.

# subtype #

**Description:** subtype denotes all subtypes of the card, separated from each other by a space character.

**Examples:**
  * subtype=goblin
  * subtype=human wizard ally
  * subtype=aura

**Usage:** Just as [[#type|type]]s, you can specify a list of several subtypes.

# power #

**Description:** power is an integer describing the power of the card. It is used for creatures only.

**Example:** power=5

**Usage:** Some cards have a power of "  `*` ", or "  `*` +1". In this case, specify the value that the term would have if the "  `*` " was a zero - e.g. "power=0" and "power=1" for the two examples above. The functionality of the "  `*` " can be coded by a later [[#auto|auto]] rule.

# toughness #

**Description:** toughness is an integer describing the toughness of the card. It is used for creatures only.

**Example:** toughness=5

**Usage:** Some cards have a toughness of "  ```* ", or "  ``**` +1". In this case, specify the value that the term would have if the "  `**` " was a zero - e.g. "toughness=0" and "toughness=1" for the two examples above. The functionality of the "  `**` " can be coded by a later [[#auto|auto]] rule.**

# text #

**Description:** text is a text describing the effects of the card.

**Example:** text=Flying. First Strike. Whenever this creature attacks, draw a card.

# abilities #

**Description:** abilities denotes those special abilities of a card that can be defined with a single keyword, i.e which don't need any values specified for them.

You can specify several abilities by separating them with commas.

The following abilities are supported:

  * absorb
  * affinityartifacts / affinityforests / affinitygreencreatures / affinityislands / affinitymountains / affinityplains / affinityswamps
  * cantattack
  * cantblock
  * cantlifelose _(controller can't lose by getting to 0 life or below)_
  * cantlose _(controller can't lose - Card example:Platinum Angel)_
  * cantmillose _(controller can't lose by getting milled)_
  * cantregen _(creature can't regnerate)_
  * cantwin _(controller cant win the game / opponent cant lose)_
  * changeling
  * cloud _(can only block creatures with flying)_
  * controllershroud _(Card example:True Believer)_
  * deathtouch
  * defender
  * doesnotuntap _(... during it's controller's untap phase)_
  * double strike
  * exalted
  * exiledeath _(will send the card to exile instead of to graveyard opon death. Treated as a replacement effect.)_
  * fear
  * first strike
  * flash
  * flanking
  * flying
  * foresthome / islandhome / mountainhome / plainshome / swamphome
  * forestwalk / islandwalk / mountainwalk / plainswalk / swampwalk
  * frozen _(card/target will not untap on its controllers next untap phase. Card example:Tangle)_
  * haste
  * horsemanship
  * indestructible
  * infect
  * intimidate
  * leyline _Enables the coding of the Leyline cycles_
  * lifelink
  * mustattack
  * nofizzle _(cannot be countered)_
  * nomaxhand _(controller has no maximum max hand)_
  * opponentshroud _(cannot be targeted by spells or abilities that the opponent controls)_
  * persist
  * phantom _(prevents damage and remove one +1/+1 counter. Card examples:Phantom Centaur,Phantom Nishoba)_
  * playershroud _???_
  * poisontoxic / posiontwotoxic / poisonthreetoxis _(card adds 1,2 or 3 poison counters to the player if damaged)_
  * protection from white / protection from blue / protection from black / protection from red / protection from green _(note that there's also "protection from(target\_type)" available as an auto rule)_
  * rampage (??) _(note that there's also "rampage(p/t,n)" available as an auto rule)_
  * reach
  * reachshadow _(can block creatures with shadow)_
  * retrace
  * shadow
  * shroud
  * storm
  * sunburst
  * trample
  * treason _(card with abilities=treason will be sacrificed at the end of the turn. Card example:Sneak Attack)_
  * unblockable _(note that there's also "cantbeblockedby(target\_type)" available as an auto rule)_
  * vigilance
  * vigor _(instead of taking damage the source gains +1/+1 counters. Card examples:Phytohydra)_
  * wilting _(source takes damage in the form of -1/-1 counters)_
  * wither
  * legendarylandwalk
  * desertlandwalk
  * snowforestlandwalk
  * snowplainslandwalk
  * snowmountainlandwalk
  * snowislandlandwalk
  * snowswamplandwalk
  * snowlandwalk
  * nonbasiclandwalk
  * strong _(cant be blocked by creature with less power)_
  * weak _(cant block creatures with more power)_
  * phasing
  * split second


Note that despite being supported, some of the abilities don't work like you would expect them to, due to some limitations in the engine.

**Example:** abilities=flying,first strike,shadow

## deprecated abilities ##
As wagic evolves, we simplify some of the abilities, make them more flexible, or get rid of abilities that don't work correctly. We try to stay backwards compatible, but sometimes there's no choice...
  * <strike>bothnocreature <i>(no player can cast creatures)</i></strike> (deprecated since version 0.15, use maxCast instead)
  * <strike>bothcantcast <i>(no player can cast spell)</i></strike> (deprecated since version 0.15, use maxCast instead)
  * <strike>bothonecast <i>(no player can cast more than one spell per turn)</i></strike> (deprecated since version 0.15, use maxCast instead)
  * <strike>cantcreaturecast <i>(card controller cant cast creature)</i></strike> (deprecated since version 0.15, use maxCast instead)
  * <strike>cantspellcast <i>(card controller can't cast spells)</i></strike> (deprecated since version 0.15, use maxCast instead)
The following abilities were once available, but never worked right and have been removed for the time being: _banding_

# target #

**Description:** target defines the potential targets for the card.

There is an important distinction between instants/sorceries and other cards. _Only_ instants/sorceries can use the target= key. All other cards specify targets in their [[#auto|auto]] rules.

The syntax for the target specification is the same though, no matter whether a target is specified with a target= key or inside an auto= rule. Therefore, the complete syntax is explained here.

The syntax is the following:

target=target\_type\_1[s](s.md)[,target\_type\_2[s](s.md)][|zone1[,zone2]]

## target types ##
Valid keywords for target\_type are:
  * any word that can be found in the [[#type|type]] or [[#subtype|subtype]] key of any card
  * player (meaning "a player")
  * the   `*`  symbol (asterisk), meaning "a card of any type"
  * this (meaning: the one card that has this rule)
  * mytgt (meaning: the target of the card that has this rule. Mytgt uses a very broad definition of "target". For example, an aura can always use mytgt to reference the permanent it enchants as "mytgt", even though - according to the rules - it isn't technically targeting it anymore.)

Adding a "s" to a target means "1 target or more". Otherwise, it is 1 target. Note: This may not work as expected, depending on the effect applied to the targets. (??)

**Examples:**
| creature | _(targets one creature)_ |
|:---------|:-------------------------|
| goblin,player | _(targets one goblin OR one player)_ |
| wall,land,cat | _(targets one wall or land or cat)_ |
|  `*`     | _(targets one card of any type)_ |
| elfs     | _(targets one or more elves - note that the parser doesn't understand irregular plurals, so "elves" would not work)_ |
| `*`s     | _(targets one or more carda of any type) (??)_  |

**Limitations:** Although you can use "player" as a target, you cannot specify anything about this player. For example, you cannot specify "opponent" (which would be useful for rules like "Whenever this card deals damage to an opponent ...").

## target restrictions ##
A target can be followed by a string inside brackets (**[.md](.md)**) to give more specific information on it. Keywords allowed inside the brackets are:
  * _attacking_
  * _blocking_
  * _tapped_
  * one or more [[#color|colors]] (_white, blue, black, red, green, artifact_)
  * one or more [[#abilities|abilities]], such as _flying_.
  * _power_, _toughness_, and/or _manacost_
  * _gear_ - looks if a card is equipped ["this(gear !=0) effect"] or how many artifacts are on a card ["thisforeach(equip) effect"]
  * _fresh_ - [fresh](fresh.md)|zone donates a card that was placed into the determined zone this turn. [fesh](fesh.md) works for the zones "battlefield and "graveyard".
  * _multicolor_ denotes multicolored target
  * _leveler_ -checks if the card is a level up card
  * _enchanted_ -checks if the card is enchanted
- the following checks if both the colors are present on the target -
  * _blackandgreen_
  * _blackandwhite_
  * _redandblue_
  * _blueandgreen_
  * _redandwhite_


Unknown end tag for &lt;/font&gt;




Keywords inside brackets can be prefixed by a minus (**-**) sign that means "not". For example, -tapped means "not tapped", i.e. untapped.

Keywords inside brackets are separated by a semicolon (**;**). Currently, the relation between these restrictions is "or" if there is no minus sign inside the brackets and if the restrictions are of the same kind (for example, both are colors, or both are abilities), and "and" otherwise. (??) This has led to confusion and will probably change in the future.

Each of the three keywords "power", "toughness", and "manacost", can be followed by one of three comparison operators: _=_ (equal to), _<=_ (less than or equal), _<=_ (greater than or equal); and then by an integer, to which the value will be compared.

**Examples:**
| creature[blue](blue.md) | _(targets one blue creature)_ |
|:------------------------|:------------------------------|
| artifact[-tapped]       | _(targets one untapped artifact)_ |
| creature[-black;-artifact] | _(targets one creature that is not black AND is not an artifact)_ |
| creature[elf;warrior]   | _(targets one creature that is an elf OR a warrior OR both)_ |
| cleric[toughness<=4]    | _(targets one cleric whose toughness is 4 or less)_ |
| aura[manacost>=3]       | _(targets one aura which has a converted manacost of 3 or more)_ |
| creature[fresh](fresh.md) | _(targets a creature which was a put into the determined zone this turn)_ |

**Limitations:** Target resrictions are currently not available for targets using the keywords _player_, _this_ or _mytgt_. So, unfortunately, it is not yet possible to code cards with rules like "When this card attacks ...", because _aslongas(this[attacking](attacking.md))_ cannot be parsed.

## target zones ##
You can limit the targeting of your card to specific zones. The following zones are available:

_battlefield, graveyard, library, hand, stack, exile,   `*`_

The asterisk ( `*` ) means "all zones". You can also use the term "inplay" synonymous for "battlefield", and "removedfromgame" synonymous for "exile".

The default value for zones is "inplay".

"Battlefield" encompasses the whole battlefield, "graveyard" encompasses all graveyards, "library" encompasses all libraries, etc. This is often not desired, so you can limit the zone further by preceding its name with one of the following terms:

_my, opponent, owner, targetcontroller, targetowner_

The prefix and the zone name form a single word, e.g. "mybattlefield" (for: anything on the battlefield under my control) or "opponentgraveyard".

These prefixes are always seen from the perspective of the player who controls the card that uses them in its code. Let's say I'm playing against "Sheila", and I control a creature called "Painter", which lets me target a graveyard and change the color of all cards inside to green. The following table shows which graveyard would be affected by this ability, depending on the prefix used before "graveyard":

| Prefix | Meaning | "Painter" affects ... |
|:-------|:--------|:----------------------|
| (none) | _(the whole zone)_ | ... all graveyards of all players |
| my     | _(zone/cards controlled by the controller of the targeting card)_ | ... my graveyard, because I'm controlling Painter |
| opponent | _(zone/cards controlled by the opponent of the targeting card's controller)_ | ... Sheila's graveyard, because she's the opponent of Painter |
| owner  | _(zone/cards controlled by the owner of the targeting card)_ | ... usually my graveyard, assuming that Painter was originally part of my deck. If Painter was originally part of Sheila's deck, and I merely took control of it during play, then Sheila is Painter's owner, and her graveyard would be targeted. |
| targetcontroller | _(zone/cards controlled by the controller of the targeted card)_ | If target is a player, targetcontroller will be that player. If target is a card, targetcontroller will be that card's controller |
| targetowner | _(zone/cards controlled by the owner of the targeted card)_ | If target is a player, targetcontroller will be that player. If target is a card, targetcontroller will be that card's owner |

**Limitations:**
  * Anything that allows a player to look into a library will cause the library to be shuffled after the effect has resolved.

  * Some zones are not supported by the interface (although the engine can handle them). For example, the "exile" zone cannot be displayed at all, although the engine can address it and move cards to it.

  * "mybattlefield" really means "the zone formed by all permanents on the battlefield which I control". It does _not_ mean "my half of the duel screen". This is important for auras cast on opponent creatures: these are displayed in their opponent's half of the screen, but nevertheless belong to their controller's _battlefield_.

## example targets ##
All this sounds pretty complex, but it's not. Let's see some working examples:
| creature | _(any creature in play)_ |
|:---------|:-------------------------|
| artifact,enchantment | _(any artifact or enchantment in play)_ |
|  `*` `|`graveyard | _(any card in any graveyard)_ |
| creature,player | _(any creature or player in play)_ |
| mountains | _(one or more mountains in play)_ |
| artifact,enchantment`|`mygraveyard | _(any artifact or enchantment in your graveyard)_ |
| creature[-black;-artifact] | _(any non black, non artifact creature in play (that's what actually used for Terror)_ |
|  `*` `|`stack | _(any spell on the stack)_ |

If you cannot describe the target with the available keywords, then you're doomed, you cannot add the card to the game without coding.

# auto #

auto is the most complex key, as it allows to program the behavior of the cards to some extent. If you cannot add what you want with auto, it means you cannot add the card to the game without recompiling it.

Unlike other parameters, auto can appear several times in one card description.

Please note that the "auto" parameter is changing very often, and we cannot guarantee that future releases will be backward compatible!

## auto syntax ##

Values for "auto" often consist of several parts. An (approximated) general syntax for auto values is this (values inside brackets [.md](.md) are optional):

**`[`[[cost|cost:](#auto.md)]|[[triggers|trigger:](#auto.md)]`][`[[conditions|conditions](#auto.md)]`]`[[effects|effect](#auto.md)] [&& [[effects|further effects](#auto.md)]`] [`[[target|target](#auto.md)]|[[playertarget|playertarget](#auto.md)]`][`[[restrictions|restrictions](#auto.md)]`]`**

The individual parts and their meaning:
  * [[cost|cost](#auto.md)] - the cost for activating this effect
  * [[triggers|trigger](#auto.md)] - an event that triggers this effect
  * [[conditions|conditions](#auto.md)] - special conditions which need to be fulfilled for this effect to take place
  * [[effects|effect](#auto.md)] - the effect itself
  * [[effects|further effects](#auto.md)] - additional effects
  * [[target|target](#auto.md)]|[[playertarget|playertarget](#auto.md)] - the target(s) of the effect(s)
  * [[restrictions|restrictions](#auto.md)] - restrictions that apply to this rule, e.g. when and how often it can be activated

The order of elements if often interchangeable, but not always. The order used in this reference will give you maximal safety that your code can be parsed correctly. This can be important because the parser is a bit stingy with error messages (you'll only get any if you compile the game in debug mode, and even then they are sparse).

Spaces are necessary in some places, forbidden in some, and optional in some other places. The easiest rule to prevent problems is to _never_ put a space in front of a "(" character.

Colons are important signposts for the parser, make sure that you use them correctly (look at the examples provided here).

## auto cost ##

For most effects, cost is an optional parameter. If an auto rule has no cost, then it will be executed whenever the card is played or enters the battlefield. For example, a creature coded with _auto=life:2_ would give its controller 2 life whenever it enters the battlefield. The effects of most instants and sorceries are coded without a cost as well, since their cost is being paid when casting the spell.

If an auto rule has a cost, then this denotes an activated ability of this card. In an auto rule, cost may contain any of the following elements:

  * any kind and number of mana costs, specified with the same syntax as for [[#mana|mana]] keys
  * one {T} symbol, for "tap this permanent", or {T([[#target|target]])}, where target is anything that could be represented in a [[#target|target]] key.
  * one sacrifice, which can be specified either as {S} (for "sacrifice this permanent"), or {S([[#target|target]])}, where target is anything that could be represented in a [[#target|target]] key.
  * {E}/{E(target|zone)} _(Exiles something as additional casting/activation cost)_
  * {H}/{H(target|zone)} _(Returns something to your hand as additional casting/activation cost)_
  * {L} _(life loss as additional casting/activation cost)_
  * {D} _(Discarding a random card as additional casting/activation cost)_
  * {M} _(Puts a card from the top of you library as additional casting/activation cost)_
  * {N} _(cost of returning an unblocked attacker to your hand)_
  * {discard(target|zone)} _targeted discard cost
Note:
You have to type in one {L} for each single point of life to pay for the autoline's activation (see "Examples"). The same is true for {D} and {M}._

  * {s2l} _(put   **target|whereever**  on top of your library as additional casting/activation cost)_
  * {l2e} _(Exiles the top card of you library as additional casting/activation cost.Card example:Arc-Slogger)_


**Examples:**
  * {T}: - _(tap this card)_
  * {G}{T}: - _(pay one green mana and tap this card)_
  * {1}{S(goblin|mybattlefield)}: - _(pay 1 mana and sacrifice a goblin you control)_
  * {S(  `*` |myhand)}: - _(choose and discard a card - discarding is technically very similar to "sacrificing from the hand")_ (??)
  * {0}: - _(This ability can be activated without any cost, but it needs to be activated, it does not fire automatically or continuously)_
  * {L}{L}{L}{B}{S(creature|mybattlefield)}: - _(pay 3 life, 1 black mana and sacrifice a creature you control)_
  * {E(  `*` |mygraveyard)}{T}: - _(exile a card from your graveyard and tap this card)_
  * {T(elf|mybattlefield)}{T(elf|mybattlefield)}: - _(tap 2 elves you control)_
As you can see, there are no limitations in combining the different auto costs.

**Limitations:**
  * previously only one targeted cost was possible, this is no longer the case. it is now possible to denote as many targeted cost in a single cost line as you wish.

  * Note that for targeted sacrifice costs, you always have to add a target zone (usually _|myinplay_), otherwise the player will be able to sacrifice cards controlled by the opponent.

## auto triggers ##

Triggers are effects that are executed not immediately, but when a certain event occurs. Each trigger defines the event that triggers it.

Triggers are declared with the keyword _@_, and end with a colon _:_. The most basic trigger syntax is:

**@[trigger](trigger.md):`[`[[effects|effect](#auto.md)]`]`**

The following triggers are available in Wagic:
  * [[triggers|(various names)](#phase-based.md)]: trigger on phase start
  * [[#@movedto|@movedto]]: trigger on a card having changed zones
  * [[#@attacking|@attacking]]:trigger on a card declared as an attacker
  * [[#@blocking|@blocking]]:trigger on a card declared as an blocker
  * [[#@blocked|@blocked]]:trigger on an attacking card being blocked
  * [[#@notblocked|@notblocked]]:trigger on an unblocked attacking card
  * [[#@damaged|@damaged]]: trigger on a card or player being damaged
  * [[#@combatdamaged|@combatdamaged]]:trigger on a card or player being combat damaged
  * [[#@noncombatdamaged|@noncombatdamaged]]:trigger on a card or player being damaged by a noncombat source.
  * [[#@tapped|@tapped]]: trigger on a card becoming tapped
  * [[#@tappedformana|@tappedformana]]:trigger on a card becoming tapped for mana
  * [[#@untapped|@untapped]]:trigger on a card becoming untapped
  * [[#@drawn|@drawn]]:trigger related to each card draw
  * [[#@vampired|@vampired]]:triggers when a card is put in graveyard this turn that was damaged by source
  * [[#@lifed|@lifed]]:trigger related to life gain
  * [[#@lifeloss|@lifeloss]]:trigger related to lifeloss
  * [[#@targeted|@targeted]]:trigger related to a card being targeted
  * [[#@discarded|@discarded]]:trigger related to cards being discarded
  * [[#@sacrificed|@sacrificed]]:trigger related to cards being sacrificed
  * [[#@attackedalone|@attackedalone]]:trigger related to when a card attacked alone

**Limitation:**
  * It is currently not possible to pay a [[cost|cost](#auto.md)] during a triggered effect.

### phase-based triggers ###

**@[next|each] [my|opponent|targetcontroller] (beginofturn|untap|upkeep|draw|firstmain|combatbegins|attackers|blockers|combatdamage|combatends|secondmain|endofturn|cleanup|beforenextturn): [[effect|effect](#auto.md)]**

The effect will trigger at the beginning of the respective phase or step.

The keywords "next" and "each" determine whether the effect will trigger only the "next" time that the given phase begins, or "each" time that a phase of this name begins (as long as the rule is active).

The keywords "my", "opponent", and "targetcontroller"(??) determine whether only phases of the card's controller are counted ("my"), or only phases of the card controller's opponent ("opponent"). If neither of the three is specified, then both players' turns can fire the trigger.

Examples:
| @each my upkeep:life:2 controller | _(gives the card's controller 2 life at each of his upkeep phases)_ |
|:----------------------------------|:--------------------------------------------------------------------|
| @each combatbegins:life:2 controller | _(gives the card's controller 2 life at each of his upkeep phases)_ |
| @next opponent end:draw:2 opponent | _(lets the opponent draw 2 cards at the end of his next turn)_      |

**Limitations:**

It is curently not possible to trigger an effect at the _end_ of a phase or step. A workaround is to let the effect trigger at the beginning of the following step (for example, instead of trying to trigger at the end of upkeep, let the effect trigger at the begin of the draw phase phase). This will work as long as Wagic has no cards which skip steps. (In the above example, skipping the draw phase would skip the trigger, which wouldn't be the desired outcome).

Some phases are currently "messy", and it's not guaranteed that triggers upon them will work. Specifically, the phase "beginofturn" may be over before all triggers had a chance to fire. The phases from "draw" to "secondmain" (including the two themselves) should be safe; for all others, thorough testing is advised.

### @movedto ###

**@movedTo([[#target|target]]) [from([[zones|zone](#target.md)])]: [[effects|effect](#auto.md)]**

The effect will be executed whenever a card matching the target specifications comes from "zone". The "from" parameter is optional and defaults to "anywhere".

Note that the first parameter is a target specification, not just a zone name. This means you can code rules like "Whenever a goblin comes into play ..." with a trigger _@movedTo(goblin|battlefield)_.

### @damaged ###

**@damaged([[#target|target1]]) [from([[#target|target2]])]: [[effects|effect](#auto.md)]**

The effect will be executed whenever a card (or player) matching the target1 specifications is damaged by a card matching the target2 specifications. The "from" part is optional and defaults to "any source".

This trigger will fire on any kind of damage being dealt, not only combat damage. An activated ability "Tap this card to deal 1 damage to target player" would cause the trigger to fire as well.

### @tapped ###

**@tapped([[#target|target]]): [[effects|effect](#auto.md)]**

The effect will be executed whenever a card matching the target specifications get tapped.

## Special case trigger restrictions ##
  * note that not all of the triggers can use all of the restrictions, these were added on a "needed" basis.

  * [trigger](trigger.md) once: -this trigger will only fire once in its life time then become inactive
  * [trigger](trigger.md) sourcenottap: -this trigger will only fire if the source is not tapped.
  * [trigger](trigger.md) sourcetap: -this trigger will only fire if the source is tapped.
  * [trigger](trigger.md) opponentpoisoned:-this trigger will only fire if the opponent is poisoned.
  * [trigger](trigger.md) turnlimited:-this trigger will only fire once per turn.

## auto conditions ##

The conditions in this section govern whether or not an effect will be executed. Currently Wagic knows two special conditions, [[(condition)|kicker](#kicker.md)] and [[#may|may]].

### kicker (condition) ###

An effect that uses the term "kicker" in its auto= rule will only be executed if this card's [[(cost)|kicker cost](#kicker.md)] was paid. Usually "kicker" is the first word in the auto= rule. See [[(cost)|kicker (cost)](#kicker.md)] for an example.

kicker payment can be checked through restrictions using the following restriction check
auto=if paid(kicker) then **ability**
auto=ifnot paid(kicker) then **ability**
this allows you a very flexible way of coding effects, even those that replace a cards original form.

MultiKicker is also supported, simply add the word "multi" before the cost to denote that we will try to pay it more than once.
kicker=multi{b}{b}
you can access the variable for how many times payment was made by using the variable "kicked"

### Dredge ###
dredge is supported with no extra code accept
dredge=dredge(NUMBER)
dredge=dredge(3) will automatically give you the choice to draw or dredge when a draw event occurs, it than handles the depletion on it's own.

### draw replacement effects ###
using the following lines allows you to denote that draw abilities will be replaced by a specified ability.
auto=replacedraw ABILITY
or for your opponent
auto=opponentreplacedraw ABILITY

it is important that the draw replacement be the first thing in the string to parse. it can be used in other abilities such as transforms subkeyword newability[, however it still has to be the first thing parsed, the replacement ability is parsed after the white space.

LIMITATION:if you replace a draw with a draw ability, you need to use a subkeyword of draw:X , draw:2 noreplace. noreplace denotes that the draw was located in an ability that replaces draw effects, it runs the run ability a second time without sending a draw event.
draw events and drawn card events are separate.
### may ###

The _may_ keyword is used to represent the text "When ... comes into play, you may ... ". When this rule is executed, then player will see a menu pop up, which will allow him to choose the rule's effect or cancel. The effect following the "may" keyword will only be executed if the player chose to do so.

**Example:**
  * auto=may life:2 - _(When this comes into play, you may gain 2 life.)_
  * auto=may bury target(creature) - _(When this comes into play, you may bury target creature.)_
  * auto=@movedTo(  `*` [white](white.md)|stack): may life:1 controller - _(Whenever a white spell is cast, you may gain 1 life.)_

**Limitations:**
  * "May" was designed for "comes into play" effects. It also works for triggers like the one in the last example. It may not work in other circumstances (e.g. in activated abilities), so thorough testing is advised for these cases.

  * If a card has more than one "may" rule active in a given situation, then these rules will compete with each other (see [[effects|competing effects](#competing.md)]). The player may choose one of them, but not more.

## auto effects ##

A wide (and ever-increasing) range of effects is available for auto= rules. The following effects are currently supported:

**`[`[[cost|cost](#auto.md)]:]bury `[`[[target|target](#auto.md)]`]`** - destroys target, it cannot be regenerated

**`[`[[cost|cost](#auto.md)]:]destroy `[`[[target|target](#auto.md)]`]`** - destroys target

**`[`[[cost|cost](#auto.md)]:]damage:n `[`[[target|target](#auto.md)]|[[playertarget|playertarget](#auto.md)]`]`** - does n damage to target. n may be a [[parsed integers|variable](#auto.md)].

**`[`[[cost|cost](#auto.md)]:]regenerate `[`[[target|target](#auto.md)]`]`** - regenerates target creature

**`[`[[cost|cost](#auto.md)]:]tap `[`[[target|target](#auto.md)]`]`** - taps a target. If no target is available, taps the card itself

**`[`[[cost|cost](#auto.md)]:]untap `[`[[target|target](#auto.md)]`]`** - untaps a target. If no target is available, untaps the card itself

**`[`[[cost|cost](#auto.md)]:]moveTo([[zones|zone](#target.md)]) `[`[[target|target](#auto.md)]`]`** - moves target card into specified zone. Allowed keywords for zone are those listed under [[zones|target zones](#target.md)].

**`[`[[cost|cost](#auto.md)]:][-][[#abilities|ability]] `[`[[target|target](#auto.md)]`]`** - gives ability to target. "ability" can be any of the keywords described in the ability section previously. For auras, instants and sorceries, cost is usually empty. For instants, sorceries, and activated abilities, the effect lasts until end of turn. For auras, it lasts as long as the aura is present. If a minus sign is used, this removes the ability from the target.

**`[`[[cost|cost](#auto.md)]:]n/m `[`[[target|target](#auto.md)]`]`** - gives +n/+m (+n power and +m toughness) to the target creature. n and m can be negative, or zero, or [[parsed integers|variables](#auto.md)]. For auras, instants and sorceries, cost is usually empty. For instants, sorceries, and activated abilities, the effect lasts until end of turn. For auras, it lasts as long as the aura is present.

**`[`[[cost|cost](#auto.md)]:]counter(p/t,n,name) `[`[[target|target](#auto.md)]`]`** - put n p/t counters on target creature. If n is negative, remove -n p/t counters from target creature. n defaults to 1. Name can be any name you can imagine acommon example counters with the name "Charge": counter(0/0,1,Charge).

**`[`[[cost|cost](#auto.md)]:]add{[[#mana|mana]]}** - Mana producer. Adds mana if the user pays [[cost|cost](#auto.md)]. [[cost|Cost](#auto.md)] can be emtpy, or contain only {t}, which is the case for basic lands.

**`[`[[cost|cost](#auto.md)]:]token([[#name|name]],[[#subtype|types]], p/t,[[#abilities|abilities]] [[#color|colors]])[\* n](.md)** - puts n tokens with name "name", types "types", power p, toughness t, abilities "abilities" and colors "colors" into play. Default value for n is 1. n may also be a [[parsed integers|variable](#auto.md)]. An example (Dragon Roost (10E)):

_auto={5}{R}{R}:token(Dragon,creature dragon, 5/5,flying red)_.

Note: If you have specified the token in any _cards.dat file as its own card (with [[#rarity|rarity]] T and [[#id|id]] i), then you can simplify the command to:_`[`[[cost|cost](#auto.md)]:]token([[#id|id]])[\* n](.md)_. It is recommended to use the negative id of the token-creating card as the id for the token, to prevent incompatibilities._

**`[`[[cost|cost](#auto.md)]:]cycling** - sends the card to the graveyard and draws a new one from the library

**`[`[[cost|cost](#auto.md)]:]fizzle `[`[[target|target](#auto.md)]`]`** - cancel a spell on the stack. The [[zones|target zone](#target.md)] defaults to _stack_.

**`[`[[cost|cost](#auto.md)]:]copy `[`[[target|target](#auto.md)]`]`** - become a copy of target (used for clone)

**`[`[[cost|cost](#auto.md)]:]becomes([[#subtype|types]], p/t,[[#abilities|abilities(??)]] [[#color|colors(??)]])`[`[[target|target](#auto.md)]`]`** - target becomes a card of types "types", power p, toughness t, abilities "abilities" and colors "colors". p and t may be [[parsed integers|variables](#auto.md)]. Cannot be used for activated abilities yet. Note: This feature is still experimental and needs thorough testing.

**`[`[[cost|cost](#auto.md)]:]transforms(types or subtypes,color,ability)** - similar to **becomes**. The main difference is that you can change single parameters of a permanent like only color or type. -- (Card examples:Memnarch,Wild Mongrel)

**`[`[[cost|cost](#auto.md)]:]equip `[`[[target|target(??)](#auto.md)]`]`** - equips target with the card. target defaults to creature|myBattlefield. Can only be activated in the controller's main phases.

**`[`[[cost|cost](#auto.md)]:]attach `[`[[target|target(??)](#auto.md)]`]`** - same as equip, but can be used anytime its controller has priority.

**protection from([[target|target](#auto.md)])** - gives the card protection from all cards that match the criteria specified in target. Cannot be used in activated abilities or sorceries/instants. Can be used as continuous effect (as a permanent ability of the card, or transferred via aura / lord / etc.)

**cantbeblockedby([[target|target](#auto.md)])** - makes the card unblockable by creatures that match the criteria specified in target. Can
be used in anything if you use the ueot ability wrapper.

**cantbeblockeof([[target|target](#auto.md)])** - makes the target unable to block creatures that match the criteria specified in target. Can
be used in anything if you use the ueot ability wrapper.

**cantbeblockerof(this)** - makes the source card unblockable by the targeted creatures. Can
be used in anything if you use the ueot ability wrapper.

**`[`[[cost|cost](#auto.md)]:]maxCast([[target|target](#auto.md)])n** - sets the Maximum number of spells (defined by target) a player can cast (move to the stack) per turn

**`[`[[cost|cost](#auto.md)]:]maxPlay([[target|target](#auto.md)])n** - sets the Maximum number of cards (defined by target) a player can put into play (move to the battlefield) per turn. The difference with maxCast is that maxPlay can be used on cards that don't use the stack, such as lands.

**`[`[[cost|cost](#auto.md)]:]preventallcombatdamage [to([[target|target1](#auto.md)])] [from([[target|target2](#auto.md)])]** - no combat damage can be dealt from creatures matching the target2 specification to targets matching the target1 specification, for the duration of this effect. from() and to() are both optional and default to "anyone". This is a fairly recent feature, thorough testing is advised.

**`[`[[cost|cost](#auto.md)]:]fog** - This is similar to "preventallcombatdamage", which only worked for instants and sorceries. "Fog" works only for permanents and is used in combination with **oneshot**

<font color='#008000'><b><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]preventallnoncombatdamage [to([<a href='#auto.md'>target|target1</a>])] [from([<a href='#auto.md'>target|target2</a>])]</b> - Extension of preventallcombatdamage, prevents all noncombat damage <a href='from.md'>a target to a target</a>.</font>

<font color='#008000'><b><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]preventalldamage [to([<a href='#auto.md'>target|target1</a>])] [from([<a href='#auto.md'>target|target2</a>])]</b> - Extension of preventallcombatdamage, prevents all damage (=combat damage + noncombat damage) <a href='from.md'>a target to a target</a>.</font>

**`[`[[cost|cost](#auto.md)]:]prevent:x** - damage prevention to single creatures and players. (Card examples:Healing Salve,Battlefield Medic)

**`[`[[cost|cost](#auto.md)]:]rampage(p/t,n) `[`[[target|target](#auto.md)]`]`** - Whenever target creature gets blocked, it gets +p/+t for each blocker beyond the nth. n defaults to 1.

**`[`[[cost|cost](#auto.md)]:]draw:n `[`[[playertarget|playertarget](#auto.md)]`]`** - draw n cards. n may be a [[parsed integers|variable](#auto.md)].

**`[`[[cost|cost](#auto.md)]:]life:n `[`[[playertarget|playertarget](#auto.md)]`]`** - gain n life. If n is negative, lose -n life. n may be a [[parsed integers|variable](#auto.md)].

**`[`[[cost|cost](#auto.md)]:]deplete:n `[`[[playertarget|playertarget](#auto.md)]`]`** - move n cards from the top of the library to the graveyard.

**`[`[[cost|cost](#auto.md)]:]discard:n `[`[[playertarget|playertarget](#auto.md)]`]`** - randomly discard n cards

**`[`[[cost|cost](#auto.md)]:]shuffle `[`[[playertarget|playertarget](#auto.md)]`]`** - shuffles the library of playertarget. Usually used in conjunction with other effect that affect the library without shuffling it by themselves. For example, "Pay {U}: Shuffle this card back into your library" can be coded as _{U}:moveto(this|mylibrary) && shuffle_

**`[`[[cost|cost](#auto.md)]:]lord([[target|targets](#auto.md)]) [[effects|effect](#auto.md)] [other](other.md)** - Continuously applies effect to all cards matching the target desciption. effect is any effect that would work in an "auto=" line. other is used if the ability should not be applied the lord card itself. (Example: Lord of Atlantis is _lord(merfolk) islandwalk_.)

**`[`[[cost|cost](#auto.md)]:]all([[target|targets](#auto.md)]) [[effects|effect](#auto.md)] [other](other.md)** - all() works identical to lord(), but guarantees that the action will occur only once. Whenever you have problems with lord(), try to use all() instead. Example: _bury all(creature)_

**`[`[[cost|cost](#auto.md)]:]foreach([[target|targets](#auto.md)]) [[effects|effect](#auto.md)] [other](other.md) [criterion](criterion.md)**
  * Applies effect as many times as the amount of cards matching the target description.
  * effect is any effect that would work in an "auto=" line.
  * other is used if the ability should not be applied the lord card itself.
  * criterion is an optional element consisting of a space character, then a "<" or ">" sign, and then a number n. If a "<n" criterion is specified, then effect is only applied if the number of matching targets m is smaller than n, and only n-m times. If a ">n" criterion is specified, then effect is only applied if the number of matching targets m is greater than n, and only m-n times. This is a fairly new feature, testing is advised.
  * (Example: Keldon Warlord is _foreach([creature[-wall]|myinplay) 1/1_)
  * (Example 2: Black Vise uses **foreach(  _|opponenthand) damage:1 opponent >4_)**

**`[`[[cost|cost](#auto.md)]:]aslongas([[target|targets](#auto.md)]) [[effects|effect](#auto.md)] [other](other.md) [criterion](criterion.md)**
  * Applies effect once, but only if there is at least one card matching the target description.
  * effect is any effect that would work in an "auto=" line.
  * other is used if the card itslf should not be counted as a match.
  * criterion is an optional element consisting of a space character, then a "<" or ">" sign, and then a number n. If a "<n" criterion is specified, then effect is only applied if the number of matching targets m is smaller than n. If a ">n" criterion is specified, then effect is only applied if the number of matching targets m is greater than n. This is a fairly new feature, testing is advised.
  * (Example: Kird Ape uses _aslongas(forest|myinplay) 1/2_)

**`[`[[cost|cost](#auto.md)]:]bloodthirst:n** - if an opponent was dealt damage this turn, this creature enters the battlefield with n +1/+1 counters on it.

<font color='#008000'><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]bushido(n/n)**- When this blocks or becomes blocked, it gets +n/+n until end of turn.</font>**

**`[`[[cost|cost](#auto.md)]:]land:n** - target player can play n additional lands this turn.

Note: Land:n does only work on instants and sorceries right now!

<font color='#008000'><code>[{N</code>}[<a href='#auto.md'>cost|cost</a>]:]ninjutsu<b>- Return an unblocked attacker you control to hand: Put the card with ninjutsu onto the battlefield from your hand tapped and attacking.</b>

Note: For a card with Ninjutsu, you basically need 2 things: {N} as cost which means <i>return an unblocked attacker as a cost</i> + the keyword ninjutsu.<br>
<ul><li>(Example: Higure, the Still Wind uses <i>{N}{2}{U}{U}:ninjutsu</i>)</font></li></ul>

<font color='#008000'><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]twist <code>[</code>[<a href='#auto.md'>target|target</a>]<code>]</code>**- switches a creatures current power and toughness with each other.</font>**

<font color='#008000'>removemana(<code>[</code>  <b><code>]</code><code>[</code>[<a href='#auto.md'>cost|cost</a>]<code>]</code></b> - Removes mana from target player's manapool. if "  <b>" is set, all mana matching the "cost" is removed. If</b>  is set alone the manapool is emptied. If cost is set alone, this exact cost gets removed from the manapool<br>
<br>
<b>resetDamage</b> - Removes all damage received so far by a creature<br>
<br>
<b>loseAbilities</b> - Target Card loses all the abilities printed on it (BETA, needs testing)<br>
<br>
<b>loseSubtypesOf([#type|type])</b> - Target Card loses its subtypes of a given type. Valid values for the type are currently hardcoded. e.g: <i>loseSubtypesOf(land)</i>
</font>

### Cost Reduction/Manapool munipulation ###

**mypoolsave([| red | white | black | blue | green](colorless.md))
is a static ability for perminents that tells the game that while this is in play, the manapool is not to empty the listed color, the parameter
only allows listing one color at a time. multiple lines might be needed for some cards.**opponentpoolsave([| red | white | black | blue | green](colorless.md))
same as above but for opponent manapool.

add{MANA} doesntempty
doesntempty tag on mana producers tells the game that until end of turn this mana is not to be emptied from the manapool, it will empty all other untagged mana as normal.

**altercost( [| red | white | black | blue | green](colorless.md) , -x )** - reduces the target card casting cost by x. Cannot be used in activated abilities or sorceries/instants. Can be used as continuous effect (as a permanent ability of the card, or transferred via aura / lord / etc.)

  * (Example: Edgewalker "auto=lord(cleric|myhand) altercost( white, -1)
auto=lord(cleric|myhand) altercost( black, -1)"

The follow 4 lines have to be added in order to reset the cost if the coded permanent leaves play.

autoexile=all(  **|myhand) resetcost**

autograveyard=all(  **|myhand) resetcost**

autohand=all(  **|myhand) resetcost**

autolibrary=all(  **|myhand) resetcost**

### Cost Increase ###

**altercost( [| red | white | black | blue | green](colorless.md) , +x )** - increases the target card casting cost by x. Cannot be used in activated abilities or sorceries/instants. Can be used as continuous effect (as a permanent ability of the card, or transferred via aura / lord / etc.).

  * (Example: chill" auto=lord(  **[red](red.md)|myhand) altercost( colorless, +2 )"
auto=lord(** [red](red.md)|opponenthand) altercost( colorless, +2 )
)

Likewise, the follow 4 lines have to be added in order to reset the cost if the coded permanent leaves play.

autoexile=all(  **|myhand) resetcost**

autograveyard=all(  **|myhand) resetcost**

autohand=all(  **|myhand) resetcost**

autolibrary=all(  **|myhand) resetcost**

### Misc ###
**name(...) && effect** - When used in an autoline it replaces an autoline's "ability" text with a custom ability name.

### Deprecated keywords ###
<strike><b><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]nocreatures</b> - target player cant play creatures this turn.</strike> (Please use maxCast instead)

<strike><b><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]nospells</b> - target player cant play spells this turn.</strike> (Please use maxCast instead)

<strike><b><code>[</code>[<a href='#auto.md'>cost|cost</a>]:]onlyonecast</b> - target player can only play 1 spell this turn</strike> (Please use maxCast instead)

## multiple effects ##

You can (and often need to) specify multiple effects for a card. You can code alternative [[effects|competing effects](#competing.md)], or you can code [[effects for one cost|multiple effects for one cost](#multiple.md)].

### competing effects ###

If several "auto" effects compete with each other (i.e. if the game cannot determine automatically which action the player wants to take), then the game will automatically create a "choice" menu.

Effects compete with each other if they have a cost and if that cost can be paid in the given situation. Having more than one effect with a [[#may|may]] condition active will cause those to compete as well(??). Effects without a cost never compete, instead the engine simply executes all of them.

**Examples:**

The following two effects (from a dual land) will compete. When the land is tapped for mana, the game will open a menu, which lets the player choose between the two effects:
<pre>
auto={T}:add{B}<br>
auto={T}:add{G}<br>
</pre>

The following two effects would also compete, since the cost for both could be paid, so the game cannot know which effect the player wants to activate:
<pre>
auto={G}:damage:1 target(goblin)<br>
auto={G}:untap target(elf)<br>
</pre>

The following two effects _would_ compete if the player has green mana in his manapool when selecting the card (because then both effects could be paid for, but _not_ if he has no green mana available (in that case there's only one effect available, and the game will execute this one automatically):
<pre>
auto={1}:tap target(artifact)<br>
auto={G}:untap target(elf)<br>
</pre>

The following two effects would _not_ compete because they have no cost - both of them would be executed (although this would make the card a bit silly). The effects contradict each other, but that does _not_ cause them to compete. Effects only compete under the conditions described above.
<pre>
auto=1/1<br>
auto=-1/-1<br>
</pre>

### multiple effects for one cost ###

You may want one cost to activate several actions. This is done by putting the keyword **&&** between two effects.

**Examples:**
  * auto={T}:add{W} && damage:1 controller - _(Tap this card: You gain 1 white mana and receive 1 damage.)_
  * auto={2}:bury target(ooze) && regenerate all(treefolk) - _(Pay 2 mana: bury target ooze, and regenerate all treefolk.)_

**Limitations:**
  * The && keyword is only available for activated abilities, i.e. for auto= rules that actually do have a [[cost|cost](#auto.md)] segment.

  * You can only specify one explicit target (i.e. a target that requires a choice by the player) per auto= line. So, a rule like "Tap: Bury target elf, then bury target goblin" cannot be coded yet - you would need code like _bury target(elf) && bury target(goblin)_, and you cannot have two "target()" specifications in the same auto= line.

  * The engine cannot easily handle auto= lines which combine an effect on a target with an effect on the card itself. This happens because the usual way to tell an engine to apply an effect to the card itself is to simply not specify a target, like this: _{2}:flying_. If you add a targeted effect to this line, like this: _{2}:flying **&& -1/-1 target(wall)**_, then the "no target specified" condition isn't fulfilled anymore, and the game will give the "flying" ability to the wall. You can get around this problem by specifying the card itself as a target for the first effect (but without using a [[target|target()](#auto.md)] command), like this: _{2}:flying **all(this)** && -1/-1 target(wall)_.

  * The above workaround might work with all(mytgt) too if required, but this has not been tested yet. (There's a risk that "mytgt" and target() confuse each other.)

  * The sequence of actions is not always from left to right. The game requires you to specify your target before any effects are executed, even when the target specification is the last element in your auto= line. This is according to the rules - the target needs to be specified when the ability is put on the stack, not when it resolves.

## auto target ##

**target** defines a group of cards that share some characteristics (for example, all black or artifact creatures on the battlefield). Wagic uses such targets in four ways:

# If the target is part of another command, then it defines a group of cards that this command operates on. For example, in _lord(elf|mybattlefield) 1/1_, the target elf|mybattlefield defines a group of cards (all elves on the player's battlefield) that the command operates upon (i.e. every card belonging to this group will receive a 1/1 bonus). In the syntax definitions of this reference, such targets are _always_ enclosed in parantheses and attached directly to the command to which they belong. For example, the target for lord commands is written like this: **lord([[target|targets](#auto.md)]) [[effects|effect](#auto.md)]**
# If the target is **not** part of another command, then it defines a group of cards from which the player needs to choose one (or more). For example, in _bury target(goblin|mybattlefield)_, the target goblin|mybattlefield defines a group of cards (goblins on the player's battlefield), and the player must then choose one of those, which will be buried. In the syntax definitions of this reference, such targets are _never_ enclosed in parantheses and are preceded by a space. For example, the target for bury commands is written like this: **bury `[`[[target|target](#auto.md)]`]`**. In card code, such targets explicitly need the term "target()" written out, they are coded like this: **target(goblin|mybattlefield)**
# As an alternative to the notation in the paragraph above, Wagic also allows you to define targets with the keyword "notatarget()", e.g. _copy notatarget(goblin|mybattlefield)_. The only difference between target() and notatarget() is that target() excludes cards which cannot be legally targeted (e.b. because they have the "shroud" ability), while notatarget() includes those. This is useful for cards like Clone, which allow the player to choose a card without technically targeting it.
# A fourth usage of "target" is the way how targets appear in a [[#target|target=]] line. This line always defines targets of the second kind, but does not need the term "target" or parantheses around it. Example: _target=zombie|graveyard_.

Which values a target can have and how they are specified is explained in the [[#target|target]] section.

If an auto= line requires a target, but none is specified, and a target= line is present, then the value of that target= line will be used as target for the auto= line. This is how instants and sorceries define their targets and effects.

## auto playertarget ##

For some effects, you can specify a special kind of target: a playertarget. This is an optional value for effects that indirectly target a player. For example, if an effect specifies to draw cards, then the playertarget defines _which_ player does that.

Valid values for playertarget are:
  * _controller_ - (the player who controls the card that has this rule)
  * _opponent_ - (the player who does _not_ control the card that has this rule)
  * _targetcontroller_ - (the player who controls the [[target|target](#auto.md)] of this auto= rule)

When playertarget is not specified, Wagic tries to use the most logical value for it: if a "target=" line is specified for an effect, Wagic will use this target if it's a player, or the target's controller if it is a card.

## auto restrictions ##

For activated abilities (i.e. [[#auto|auto]] rules with a [[cost|cost](#auto.md)] segment) you can restrict when or how often they can be activated or how long they last. The following keywords are available:

  * [[restrictions|phase restrictions](#phase.md)] - allow the effect to be activated only at certain times
  * [[#limit|limit]] - limits how often the ability can be activated per turn
  * [[#ueot|ueot]] - "until end of turn", forces the effect to end when the turn ends

### phase restrictions ###
<font color='#008000'>
<b>Description:</b> Non-activated restrictions allows the effect to be active only at certain times. The following variable tags are available:<br>
<br>
<ul><li>controllerturn - <i>(only active in the card controller's turn)</i>
</li><li>opponentturn - <i>(only active in the card opponent's turn)</i></li></ul>

<b>Examples:</b>
<ul><li>auto=this(variable{controllerturn}) 1/5 - <i>(During your turn, the 1/5 effect is always active)</i>
</li><li>auto=this(variable{opponentturn}) 5/1 - <i>(During opponents turn, the 5/1 effect is always active)</i></li></ul>

<b>Limitations:</b>
<ul><li>These restrictions were designed for continuous effects only.<br>
</font>
<b>Description:</b> Activated phase restrictions allow the effect to be activated only at certain times. The following keywords are available:</li></ul>

  * myturnonly - _(only usable in the card controller's turn)_
  * assorcery - _(only usable when a sorcery could be played, i.e. in the card controller's first and second main phase)_
  * my[phasename](phasename.md)only - _(only usable in the respective phase in the card controller's turn. [phasename](phasename.md) is written without the square brackets and can be any phase name. You can see a list of available phase names in the section for [[triggers|phase-based triggers](#phase-based.md)])_

**Examples:**
  * {T}:add{B} asSorcery - _(Tap this card: Add one black mana. Use this ability only when you could play a sorcery.)_
  * {3}:untap myupkeeponly - _(In your upkeep phase, you may pay 3 mana to untap this card.)_

**Limitations:**
  * These restrictions were designed for activated abilities so they probably won't work for continuous effects.

  * You can't (yet) restrict activation to other combinations of phases, like "only during combat" or "only before attachers have been declared".

### limit ###

**Description:** limit limits how often the ability can be activated per turn

**Example:**
  * {B}:add{W} limit:2 - _(activating this card lets the player turn 1 black mana into 1 white mana, but no more than 2 times per turn)_

### ueot ###

**Description:** ueot (meaning "until end of turn"), forces the effect to end when the turn ends.

Usually, Wagic guesses whether an effect is meant to last until end of turn, or indefinitely. For example, it assumes that any activated abilities only last until end of turn, and that the effects of enchantments last as long as the enchantment is in play. This guess is correct in the vast majority of cases.

Sometimes, however, the guess may be wrong, and you may want to explicitly tell Wagic that an effect is only meant to least until end of turn. In this case, use the "ueot" keyword.

**Example**:

An Enchantment that gives all cats +1/+1 (as long as it is in play), and also makes those cats unblockable for the turn in which it entered the battlefield, can be coded like this:
<pre>
auto=lord(cat) 1/1<br>
auto=lord(cat) unblockable ueot<br>
</pre>

## auto parsed integers ##

Many [[effects|effects](#auto.md)] in [[#auto|auto]] rules have numbers as parameters: how many cards to draw, how much life to give, how much power to add, etc. Usually, you simply write a fixed number into the rule. In many places, you can also use a variable instead of a fixed number. The following variables are available:

  * X - _(the amount of mana paid for the "X" of the spell cost of this card)_
  * p - _(the power of this card's target)_
  * t - _(the toughness of this card's target)_
  * manacost - _(the converted manacost of this card's target)_
  * lifetotal
  * opponentlifetotal

**Examples**:
  * auto=draw:X controller - _(draw one card for each mana paid for the "X" cost of this spell)_
  * auto=bury target(goblin) && life:p targetcontroller - _(bury target goblin, and give its controller an amount of life equal to that goblin's power)_
  * auto={T}:0/manacost target(elf) - _(target elf gains an amount of toughness equal to his converted manacost)_

**Limitations:**
  * Some [[effects|effects](#auto.md)] can use this functionality, others can't. Thorough testing is advised.
  * For at least the following effects, this feature has been implemented: becomes, damage, draw, life, power/toughness modifiers (excluding counters), token (for the number of tokens)

## Auto Macros ##
<font color='#008000'>
With time, the Wagic syntax for auto lines has evolved from a very simple series of keywords to a convoluted system, close to a programming language, with lots of exceptions and tribal knowledge.<br>
In an attempt to simplify this, we introduced auto macros, a system that allows to "summarize" a complex auto line into a simple keyword, actually a function accepting parameters. This is useful especially if you plan to code dozens of cards that share a complex ability.<br>
<br>
Let's for example look at the code for cycling (as of 2011/11/12): <i>autohand={2}{cycle}:name(cycling) draw:1</i>. It is not super long to type, but ideally we would prefer to type cycling({2}) or something similar. The macros system allows to simplify things by writing the complex ability once, and associating it to a keyword.<br>
<br>
Here's and example:<br>
<br>
<b>#AUTO_DEFINE CYCLING($cost) $cost{cycle}:name(cycling) draw:1</b>

The syntax is simple: #AUTO_DEFINE <a href='function.md'>name and parameters</a> <a href='actual.md'>ability code</a>.<br>
<br>
In our example, we define the keyword <i>CYCLING</i> which takes a variable <i>$cost</i>. We then tell the engine to "rewrite" that keyword and parameter into <i>$cost{cycle}:name(cycling) draw:1</i> (note the $cost variable in there).<br>
<br>
This then allows card coders to write:<br>
<br>
<b>autohand=CYCLING({2})</b>

instead of<br>
<br>
<b>autohand={2}{cycle}:name(cycling) draw:1</b>

<h3>Important Notes</h3>
<ul><li>The keyword used for a macro cannot contain any space<br>
</li><li>it is recommended to prefix parameters for the macros with the symbol "$", to avoid any weird replacement<br>
</li><li>Macros are global. Wherever you define them in your card files, they will apply everywhere<br>
</li><li><b>We suggest to write all the macros in a single text file, inside the sets/primitives folder<br>
</li><li>Macros are simply "replaced" with the code they describe, through a simple text replacement algorithm.<br>
</li><li></b>  For this reason, it is important to use unique macro names that cannot "conflict". For example, having a macro named "MACRO" and another one named "MACRO2" is a bad idea, because the string "MACRO" exists in the string "MACRO2". As a convention, we start and end all our macro names by "", and we guarantee that we don't use "" inside of the macro name anyplace else.<br>
</font></li></ul>

# autohand #

**Description:** autohand is a recently introduced key that allows you to specify effects which can be activated while the card is in your hand.

Regular auto= rules are only active when the card is on the battlefield, they are ignored if the card is in any other zone. However, you may want to code effects which can be activated from the hand (without actually playing them), for example to code Cycling effects. For such effects, autohand= has been introduced.

**Example:** autohand={2}:cycling

**Limitations:** Currently, autohand= only works for activated abilities (i.e. rules with a [[cost|cost](#auto.md)] segment). Continuous effects or triggers cannot be coded in autohand= lines.

This is a new feature, thorough testing is advised.

# autograveyard #

**Description:** autograveyard is a key that allows you to specify effects which can be activated while the card is in your graveyard.

**Example:** autograveyard={B}{B}:moveTo(myBattlefield)

**Limitations:** Currently, autograveyard= only works for activated abilities (i.e. rules with a [[cost|cost](#auto.md)] segment). Continuous effects or triggers cannot be coded in autohand= lines.

# alias #

**Description:** alias is used to access existing hardcoded routines which were originally coded for other cards. Its main use is to create a new card that has the same hardcoded effects as an existing one.

For example, the effect of Control Magic (id 1194) currently cannot be coded with any auto= rule. Therefore, the card's functionality was hardcoded, i.e. directly programmed into the engine. The engine knows that whenever a card with the id 1194 is played, it needs to execute this hardcoded effect.

There's another card called Persuasion which has exactly the same effect as Control Magic. So we want to make Persuasion behave just like Control Magic. Usually we could just copy any rules and effects over from Control Magic's card data to make Persuasion work, but in this case there's nothing to copy, since the code is stored in the engine, not in the card's data. So, to make Persuasion work, we have to tell it that it needs to access the hardcoded functions that were meant for the card with id 1194 (Control Magic).

This is exactly what the "alias=" key does. So to make Persuasion work, we would add the line "alias=1194" to its code. Note that we still need to specify _all_ other data for Persuasion (type, mana, even the target) - the "alias"   **only**  makes the Persuasion access a hardcoded function, it copies no information over from the aliased card.

**Limitations:** Use alias with care. The only time you can be 100% sure that it's correct to use alias is when the card is a reprint of an existing card, or functionally identical.

The alias= key has led to confusion and may be removed from the parser in the future.

# Final Words #

Now, the best piece of advice we can give you is to look at the existing _cards.dat files and get inspiration from there. If you have question or run into problems, ask for help in our [Cards/Sets creation forum](http://wololo.net/forum/viewforum.php?f=8)._

If you're planning to code many cards, then you will appreciate a comfortable way of testing their functions. Check the [[Wagic/TestSuite|Test Suite Reference]] for that.

If you want to dig deeper, you can have a look at TargetChooser.cpp and MTGAbility.cpp in the source code. See our [[Wagic/Compiling|Compilation Guide]] for help on obtaining Wagic's source code and compiling it.

Good luck!