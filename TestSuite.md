# Introduction #

This page is a reference of all Test Suite commands. The Test Suite is Wagic's automated way of testing specific game situations, it's a great tool for debugging and development. We're also planning on a Test Suite Tutorial, but it isn't here yet.


# Details #
This page is a reference of all Test Suite commands. The Test Suite is Wagic's automated way of testing specific game situations, it's a great tool for debugging and development. We're also planning on a [[Wagic/TestSuite/Tutorial|Test Suite Tutorial]], but it isn't here yet.

This is a work in progress and not yet presentable.

## Test Suite Basics ##

The test suite enables you to create a specific game situation, let the game run a specific sequence of actions, and then check the results. This makes the test suite a powerful tool for testing and debugging.

To use the test suite, it's recommended to get the Windows download of Wagic - it comes with all the test scripts that the developers use. The PSP download doesn't have these.

After installing Wagic, look for a folder called "test". This folder contains three important types of files:
****test.txt - this file lists all tests that the test suite shall run, bye listing their filenames, one file per line.** (any name).txt - each of these files contains one test.
results.html - this file contains the detailed results of the last run of the test suite. It may not yet be present if you never ran the test suite.**

To run the test suite, make sure that the filename of the test(s) that you want to run is specified in _tests.txt, and start Wagic. Click on "Play", choose "Test Suite", and watch the tests. Open_results.html for detailed results.

Each test file is a script. the rest of this page explains the commands and syntax of this script.

## Test Stages ##

Each test script has four stages: [INIT](INIT.md), [DO](DO.md), [ASSERT](ASSERT.md) and [END](END.md). They always occur in this order, and each stage occurs only once.

[INIT](INIT.md) defines a game state - which cards are in which zone, how much life or mana a player has, etc.

[DO](DO.md) contains a sequence of actions to perform (like summoning Grizzly Bears, blocking the Lord of the Pit with a poor Scryb Sprites, etc.)

[ASSERT](ASSERT.md) describes how the game state should be after performing the actions in the [DO](DO.md) state. The [ASSERT](ASSERT.md) state uses exactly the same syntax and format as the [INIT](INIT.md) state.

[END](END.md) denotes the end of the script.

## [INIT](INIT.md) and [ASSERT](ASSERT.md) syntax ##

This stage has three parts:
**specification of game phase** specification of data for player1 (after tag [PLAYER1](PLAYER1.md))
**specification of data for player2 (after tag [PLAYER2](PLAYER2.md))**

All three parts must be present, and must be present in the sequence above.

### Game Phase ###

This specifies the game phase with which the test should start. Legal values are:

untap, upkeep, draw, firstmain, combatbegin, combatattackers, combatblockers, combatdamage, combatend, secondmain, endofturn, cleanup

You ''have to'' specify a game phase, in the [INIT](INIT.md) stage as well as in the [ASSERT](ASSERT.md) stage. If you don't, then the value will be undetermined, and the test will almost certainly fail.

### Player data ###

These two sections - one section for the first player, starting with the tag [PLAYER1](PLAYER1.md), and one for the second player, starting with the tag [PLAYER2](PLAYER2.md) - specify data for the two players and their cards. Each section may contain a list of definitions. Each of these definitions is optional - if it is not specified for a given player, then a default value is used. the definitions can appear in any order, but each definition must not appear more than once for the same player.

The following table lists the available definitions:

{| class="wikitable"
|-
! command
! meaning
! syntax
! example
! default
|-
| inplay:
| lists all cards in this player's battlefield
| inplay:[card1](card1.md)[,card2][...]
| inplay:Wall of Swords,Crusade,Plains
| empty (this player has no cards in the battlefield)
|-
| hand:
| lists all cards in this player's hand
| hand:[card1](card1.md)[,card2][,...]
| hand:Shock,Disenchant
| empty (this player has no cards in his hand)
|-
| library:
| lists all cards in this player's library
| library:[card1](card1.md)[,card2][,...]
| library:Mountain,Island,Zap
| empty (this player has no cards in his library)
|-
| graveyard:
| lists all cards in this player's graveyard
| graveyard:[card1](card1.md)[,card2][,...]
| graveyard:Swamp,Scathe Zombies
| empty (this player has no cards in his graveyard)
|-
| manapool:
| lists the mana currently in this player's manapool
| manapool:[{W|U|B|R|G|number}][{W|U|B|R|G|number}][...]
| manapool:{W}{3}{R}{R}
| empty (this player has no mana in his manapool)
|-
| life:
| lists the player's life points
| life:[number](number.md)
| life:17
| 20
|}

Synonyms: Instead of inplay:, you can also use battlefield:.

Noteworthy details:
**When specifying cards, do not uses spaces before or after the colon or the commas, or the parser will not recognize the names.** You can specify cards by their multiverse ID as well as by their name. You can mix both notations freely. Example: hand:3118,Shock,1373.
**If you specify a card by name, then (out of all cards in the database which have that name) the card with the lowest ID will be chosen.** You cannot specify names that contain a comma (for example Stonebrow, Krosan Hero). To specify such cards, use their IDs instead.
**You can specify tokens by using the negative ID of the card that created them. Example: the line inplay:1138,-1138 specifies "The Hive" (ID 1138) and one Wasp (the token that "The Hive" creates).** You should not use the same name or ID for two different cards in the same script. If you do, then you can only select the first card with that name/ID in the script, and if cards with identical names or IDs end up in different zones, then the checking algorithm will confuse them. If you need several instances of the same card for your test, use different IDs for each of them. For example, to put three Island in the battlefield, don't write inplay:Island,Island,Island. Instead, write inplay:1392,1393,1394 (or any other IDs of cards named Island).
**Of course, the above only works if the card has at least as many different print versions as you want instances of that card in your script. If this isn't the case, then you have to create a copy of the required card's code in any **cards.dat file, give it a yet unused ID, and use that ID in your test.** You cannot attach auras to permanents in the [INIT](INIT.md) stage. If you want an aura to be attached to another card, you have to cast the aura in the [DO](DO.md) stage and specify its target there.
**It is okay to leave the library empty in the [INIT](INIT.md) stage, even if the respective player will have a draw step later on. While the test suite is active, players cannot lose from having no cards to draw, they simply don't draw a card and then go on.** Note that the manapool will be emptied when the game proceeds to the next step. Thus, specifying a manapool in the [INIT](INIT.md) stage only makes sense if the mana will be used in the current step. If one of your players needs mana later on, you have to provide it by other meaning (for example, putting lands in his battlefield and tapping these later on).
**In the [ASSERT](ASSERT.md) stage, you can use the asterisk** instead of a card name. The asterisk has the meaning "any one single card".
_In the [ASSERT](ASSERT.md) stage, auras will be counted in the battlefield of their targets controller. If PLAYER1 casts Spirit Link on PLAYER2's Craw Wurm, then (during [ASSERT](ASSERT.md)) you have to specify both cards in the inplay: segment of PLAYER2._

## [DO](DO.md) syntax ##**

In the [DO](DO.md) stage, you specify a sequence of actions, which the test suite then performs.

### Clicking on cards ###

You specify the desired sequence of actions by specifying the cards to click on, one card per line. If you want to tap a Mountain for red mana and then cast Shock on a Bog Imp, you write:

<pre>
Mountain<br>
Shock<br>
Bog Imp<br>
</pre>

If you want to attack with a Craw Wurm and a Brass Man, you proceed to the combatattackers phase (see below), and then write:

<pre>
Craw Wurm<br>
Brass Man<br>
</pre>

If you want to block these two attackers with a Wall of Ice and a Wall of Earth, proceed to the combatblockers phase (see below), and then write:

<pre>
Wall of Ice<br>
Wall of Earth<br>
Wall of Earth<br>
</pre>

Note that you have to specify "Wall of Earth" twice if it should block the second attacker (Brass Man). The interface is just like in the regular game: If you click on Wall of Earth once, then it will be assigned as a blocker to the first attacker, you have to click on it again to move it to the second attacker.

If the test suite cannot click on a given card (e.g. because the card is not a legal target for the current action, or isn't available any more, or cannot perform any action in the current situation), then the test suite will simply skip that action, just as if you had clicked on such a card in the game. This does not count as a test failure.

### Other commands ###

**choice [number](number.md) - this command is used to choose an action in a menu. Choices are numbered from 0 (for the first option in the menu) onwards. For example, if you want to attack with Llanowar Elves, you have to write:**

<pre>
Llanowar Elves<br>
choice 0<br>
</pre>

... because when you click on the Llanowar Elves, the game will ask you whether you want to attack with them, or use their mana ability. The "Attacker" option is the first in the list, so you have to write "choice 0". To use the mana ability, you'd have to write "choice 1".

**p1 and p2 are used to target the players. To cast Lightning Bolt on player 1, you write:**

<pre>
Lightning Bolt<br>
p1<br>
</pre>

Note that the players are numbered "1" and "2" in the test script, but labeled "Player0" and "Player1" (respectively) in _results.txt._

### Progressing through game phases/steps ###

Two commands are used to advance the game steps:

**next - goes to the next step** eot - "end of turn", goes to the next untap step

Notable details:

1. You do not have to switch players manually (unless you want to interrupt an action), the test suite does this automatically for you. If you want PLAYER1 to attack with a Craw Wurm, and PLAYER2 to block with a Wall of Ice, you simply write (assuming that you start in the "combatattackers" step):

<pre>
Craw Wurm<br>
next<br>
Wall of Ice<br>
</pre>

The test suite automatically gives PLAYER2 priority in the combatblockers step.

2. The sequence of steps/phases depends on the circumstances. If you declare no attackers, then there will be no combatblockers or combatdamage step. If creatures are dealing first strike damage, or if multiple blockers need to be orderer (or have damage amounts assigned) by the attacker, then additional steps will be inserted. You may have to experiment a little to match the actions to the correct game steps. When in doubt, run the test and check _results.txt, the output will tell you which step the game expected at the end of the [DO](DO.md) stage._

### Interruptions ###

The test suite uses three commands for interrupts:
**no** yes
**endinterruption**

By default, there will be no interrupts. If you want an action to be interrupted, you need to write "yes". Afterwards you specify the actions to be done during the interruption, and then you return to the normal gameflow by writing "endinterruption".

Note that the active player gets to interrupt first. Thus, if you want PLAYER1 to cast a Craw Wurm, and PLAYER2 to counter that spell, you write:

<pre>
Craw Wurm<br>
no<br>
yes<br>
Counterspell<br>
Craw Wurm<br>
endinterruption<br>
</pre>

The "no" tells the game that PLAYER1 does not want to interrupt his own spell. Then, the "yes" tells the game that PLAYER2 does want to interrupt that spell. That player then casts Counterspell and targets the Craw Wurm. (This example assumes that both players have enough mana for their actions in their manapools.)

### AI testing and user input ###

There are two commands to give control to the human player (i.e. you) or to the AI:

**human** ai

The "human" command can be used for tests that currently cannot be automated. It is also useful to simulate the steps of a test by hand if it isn't working, or if you want to know how a specific menu looks like, to find the right number for a choice command.

The "ai" command is intended for AI testing. Note that returning control from the AI to the test suite is currently tricky. Due to technical limitations you cannot be sure in which turn step the game will be in when control returns to the test suite. You can try to determine this step experimentally, but the value may be different for people using other hardware and running Wagic at different speeds.

## Comments ##

Every line that starts with a # sign is treated as a comment.

Commenting test scripts is highly recommended. Otherwise, it will be difficult to determine (just from reading the script) in which step the game will be exactly after a number of "next" commands. Also, a line like "Craw Wurm" can have very different meanings depending on the context: "attack with Craw Wurm", "Block Craw Wurm", "target my spell at Craw Wurm", "sacrifice Craw Wurm to pay ability X", etc. This can make reading scripts difficult, especially if they have been written by someone else, and commenting them thoroughly can make that easier.

## Results ##

(ToDo: Explanation of lines in _results.txt)_

## Policy ##

Smaller is better: Instead of writing one "huge" script that tests several conditions at once, it's usually better to write several small scripts. The reason for this is maintainability: Smaller scripts are much easier to understand, maintain, and adapt.

## Common Questions ##

Some variables, like the toughness of the creature, cannot be accessed directly by the test suite. The test suite can only check whether a card is in a given zone, plus the contents of the manapools and the amount of life of both players. So, if you want to test anything beyond these values, you have to write your test in a way that these accessible values lead to different results depending on the variables you want to test. You cannot access the toughness of a creature directly, so you write your test in a way that the creature ends up in a different zone if it had a different toughness than you expected.

**Q: How can I test whether my creature has received a toughness boost?** A: Cast a direct damage spell on it which does enough damage to kill the creature if it has not received the boost.

**Q: How can I test whether my creature has received a power boost?** A: Attack the opponent with the creature and check the opponent's life.

**Q: How can I check whether a creature is tapped?** A: Cast Assassinate on it. Assassinate can only be cast on a tapped creature (and sends it to the graveyard).

**Q: How can I check whether a creature has received Flying?** A: Choose it as an attacker while Moat is in play, or block a flying attacker with it.