# Introduction #

As we spend time working towards release date while under feature lock, I think it might be a good idea to set aside this area for planning and organization of future projects. This could be a good way to keep on top of issues like, "Who do I talk to about...?" or "Is anyone working on..." without sending out multitudes of email. The emails and issue tracker are great for discussion, but as a quick overview of what's what this could be useful.

This is definitely voluntary, and there are no real guidelines here... but I think anything that can't be summed up in a single sentence should probably be explained in detail elsewhere. Think of this as the project organization "cheat sheet".

If this turns out to be more trouble than it's worth, we'll of course abandon it :)

# Project listing #


---


## Jeck ##
**Current Projects**
  * Complex themes
  * Restricted sets & format restriction
  * Better mod support
  * Wagic manual update to latest rev.


---


## Psyringe / Psyyringe ##
(One y on the forums, two ys on Google. Don't ask y.)

**Current Projects**
  * **Creating AI decks**
  * Creating tools and test modes for AI deck creators
  * Evaluating AI / AI deck performance
  * Learning C++ and Wagic's code structure
  * User support in the forum

**Proposed Projects**
  * Nothing is really _proposed_, I'm not in a position to, with my first piece of potentially submit-worthy code still waiting for review (btw, it's waiting here: http://wololo.net/forum/viewtopic.php?f=6&t=730 ;D ). However, here's a list of things I'm interested in. Some of these - though certainly not all - _may_ become future projects if I learn C++ coding quickly enough, and stay with the project long enough. Some of these have a draft for a design doc written for them, so if you're interested, just ask. :)

**Interests / Potential Projects**
  * **Match mode** (several games per match; already implemented, but with ugly code)
  * Flexible **Tournament** mode (managing knockout or swiss-system championships as well as round-robin-leagues from the same codebase)
  * **Story** mode (Branching quests with many decisions to make, strategies to work out, and secrets to discover. Quests are parsed from a text file and can be written by users.)
  * **Format enforcement** (Vintage, Highlander, Prismatic, etc.)
  * **Economic Difficulty** option. (Make starting games / entering tournaments (or quests) require a fee. Link all costs and rewards in the game to a single variable ecoDifficulty, which can be changed from the option screen to scale the whole in-game economy.)
  * **Shopkeeper sympathy** (Shopkeeper will raise prices if you keep requesting new selections, will set prices back to normal due to being impressed after a victory of yours. Will discourage endless refreshing of shop screen while retaining the ability to endlessly browse.)
  * **Realistic prices** (enable import of Crystal Keep price files)
  * **Card win/loss** (players can win opponent's cards, or lose cards from their own deck, if winning or losing high. Perhaps ante mode and cards.)
  * **Battle for Booster** mode (Player pays half price for a booster, then plays against an AI. Match result determines how many and which cards from the booster the player can choose.)
  * **Set unlock choice** (offer players a choice between three sets to unlock)
  * Improved **set presentation** (Allow set-specific booster graphics instead of always displaying the general card background. Allow booster packs to be displayed in Alternate Render mode, where the "Big card" will list info about the set - no of cards, main colors or types, main keywords, etc.)
  * **Card Avatars** (Display a card, or cropped graphics from a card, as avatar. Add an #AVATAR: option to AI deck files which can specify the card to be displayed.)
  * **Current card state** display (third display mode for big card, card displays its _current_ P/T, abilities, etc.)
  * **Safe mode** (Option to only load officially supported sets. After a crash, offer to switch to safe mode and turn music and image loading off before initializing the system.)
  * **Safe deck writing** (rename deck and collection files before overwriting them, only delete the files if save operation was successful)
  * **Game Speed** option (set in game menu to turbo / normal / slow / stepwise)
  * **Inspect mode** (query AI information in-game, display AI's hand, AIs target ranking, etc.)
  * **Expanded menu system** (allow to add menu items on top of the list, removal of menu items, changing of item text or description, querying a menu item's data)
  * **More efficient statistics** (read statistics in at application start and update them on memory, instead of re-reading them from the drive each time)
  * AI Improvements: **Cumulative Effects** (Teach AI the difference between cumulative and non-cumulative effects when it chooses targets. Default to non-cumulative.)
  * **Game recordings** (record games to show them as a game movie later. Might work as an extension of the Test Suite, would need the random seed to be setz and saved at game start though.)
  * **Conditional debugging output** (wrap OutputDebugString into something conditional, like an optional ID parameter and some functionality to only display debug strings which pass a specific ID to the function)
  * **Custom set** (create a custom set with graphics that can be used (and distributed) under Wagic's BSD license.

_(Argh. I was afraid that the list would get that long. I'd like to state again that it's unlikely that I'll be able to implement more than 10% of those. Usually I wouldn't even make these ideas public with so little to show yet. On the other hand I wanted to tell you what I'm interested in, so there it is.)_


---


## Jean ##
**Current Projects**
  * Internationalization : allow for accented characters, chinese, japanese.
  * Startup speed up : bytecompilation of collections.
  * Network : unification library PC/PSP
  * Effects : add graphical eyecandy to the inplay phase
  * Packaging : create a .deb for linux (ultimately, aim for integration in mainstream Debian/Ubuntu repositories), a .dmg for mac, create the windows binary under linux, all of this automatically
  * Remove fmod dependancy