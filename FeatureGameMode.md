# Game Modes #

This is the first of hopefully many FeatureDiscussion pages. The idea behind these pages  is to open up discussion on a feature we plan to introduce in a future release. Feel free to edit or comment on this document with ideas, criticism, etc.

This particular page deals with game modes.

This is just a rough draft, not a strict design doc. I've been working around ideas based on two main features for game modes. First, support for **custom rules**, and second, support for **scripted campaigns**.

---

# Custom Rules #
## Restricted sets ##
First, the ability to restrict cards and sets, using a combined whitelist/blacklist file, which would read rules sequentially. This would be something like this...
```
name:Crucible Commons
-set: * #Remove all sets
set: 10E #Add set 10E
-card: * rare
-card: * uncommon
+129480
```

Lines starting with a minus would blacklist the cards, lines without would whitelist, and lines with a plus would represent something required in every deck. There'd be some minor support for filtering based on card attributes (if we could copy or adapt the "targetting" code from cards, that'd be awesome).

## Modify base rules ##
I'd also like to add support for modes to have permanent "world enchantment" style effects. Codewise, this could be as simple as adding an invisble, invulnerable card to the battlefield. With a little tweaking, this could also lead to a Planechase mode (which would need to be at least partially hardcode).
  * Agreed: These could be loaded with the same "auto=" lines that we have in cards, but loaded from a "rules" or "mods" specific directory. --Erwan

## Different rules per player ##
I'd definitely want to add support for separate rules per player, so there might be game modes where one side has to use only walls, but also has a permanent "Rolling Stones" in play, or something similar. This would also include changing life counts, etc.

---

# Scripted Campaigns #
Campaigns would be a separate but complementary aspect of game modes. A campaign file would include a number of branching duels, each interspersed with an arbitrary number of "Story" scenes and possibly rewards.

Each duel could potentially use a different custom rules file. This is how I envision a campaign file:
```
[scene]
id=1
background=outside.jpg
title="The tournament begins..."
text="You stand outside a massive convention center, waiting for the game to begin. Press O to continue"
text="Suddenly, you are ambushed by an intimidating bouncer"
overlay=bouncer.png
chat="Hey, you! You've got to meet the minimum requirements to enter!"
chat="Prove that you're strong enough!"
next=2
[/scene]
[duel]
id=2
ai=deck1
rules=vintage
lose=3
win=4
[/duel]
[scene]
id=3
title="Game over, man!"
background=outside.jpg
overlay=bouncer_victory.png
chat="Go home, kid. You ain't got what it takes."
text="You have lost the game."
[/scene]
[scene]
id=4
title="Bouncer Trouncer!"
background=outside.jpg
overlay=bouncer_defeat.png
chat="Un... unbelievable!"
chat="Here, kid, take this. You've earned it!"
reward=400 credits
reward=Baneslayer Angel
[/scene]
```


  * I had something more "ambitious" in mind: Campaigns would be scripted through an XML files, that describes not only the deck of opponents, but also some maps you would have to travel on. This requires months of work so we should start with your idea, keeping the "maps" idea in mind -- Erwan