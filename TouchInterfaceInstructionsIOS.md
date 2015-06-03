#short summary of controls on iOS port

# Introduction #

A Short summary of controls on iOS port of Wagic

# Details #
README for iOS Touch Interface for Wagic

These instructions are very similar to the Android port but with a few differences.  Since there is no hardware button that can be used for the menu button.  Until a complete touch interface has been introduced, some of the controls will be non-intuitive as they map to specific buttons on the PSP.   This document will describe the different gestures for each section of the game.  Much of how the game works is described in the Wagic Instruction manual.  This document is not mean to replace it but have additions and changes.

# Controls #

most things work on the touch and release mechanism or tap mechanism.

## Main Menu ##

Touch the icon desired

## Deck selection menu. ##

1) Tapping the desired deck name should select it.
2) Touch and panning around the screen will highlight the closest deck name.  lifting your finger above the highlighted name will select it.
3) two finger swipe up will bring up the detailed stats on the deck if available.

## Game play ##
All interactions here are done via tapping or touch and panning mechanisms described above.

> Tapping the action wheel on the left side of the screen will advance the game to the  next game phase/step.

### Non-visible Interactions ###
These commands are done by doing something other than tapping or touch and release

#### Single Finger Long Press (press and hold one finger down for 2 seconds and release) ####
1) bring up the menu button

#### Two Finger Swipe Right ####
> Interrupt -> When prompted this action will allow the player to interrupt an action
> cancel Interrupt -> while interrupting an action, this action will cancel the interrupt request and return to display the Interrupt prompt

> Cancel viewing of player graveyard/library -> when viewing the player library/graveyard the screen is modal and will not allow other interactions until you close the view of the graveyard/library.

### Two finger swipe left ###
This is used to cancel the viewing of the AI player's library/graveyard/hand when it is displayed

### Double Swipe Up ###
Toggle images

### Special interactions (iOS only) ###
These three interactions are mapped onto the screen to make it easier to play due to the lack of hardware buttons on the devices.  All of these actions are mapped to outside the gaming board.

**Above the game board:** Brings up the "MENU"
**Lower left half below game board:** Advance to the next phase of the game.
**Lower right half below game board:** Show/Hide the modal view of the player's hand.


## Deck Editor ##

> Tapping to the left and right of the current card will advance the editor X cards.  X is the number of card edges the touch falls after.  Thus tapping on the right edge of the foremost displayed card will advance the editor to the right one card.

> Tapping on the inside of the card will push the card into/out of the current deck.  If you are viewing the collection, this will add one copy of the card to the currently selected deck.  Likewise, doing so while viewing the deck will move the card into the collection and remove it from the deck.

### Non-visible Interactions ###
These commands are done by doing something other than tapping or touch and release

### Single Finger Swipe Up/Down ###
Advance the card filtering between (All, Artifacts, Green, Blue, Red, Black and White)

### Single Finger Swipe Left/Right ###
Switch between your current deck and collection

### Double Swipe Right ###
Prompt to sell current card

### Double Swipe Up ###
Toggle images

### Double Swipe Left ###
Toggle between Deck / Collection Viewing.

### Double Swipe Down ###
Add/Remove current card from Deck.

### Two Finger Long Press (press and hold two fingers down for 2 seconds and release) ###
Brings up the card filter screen (see section on Card Filter Screen)

### Special interactions (iOS only) ###
These three interactions are mapped onto the screen to make it easier to play due to the lack of hardware buttons on the devices.  All of these actions are mapped to outside the gaming board.

**Above the game board:** Brings up the "MENU"
**Lower left half below game board:**  cycle backwards through the current deck statistics
**Lower right half below game board:** cycle forwards through the current deck statistics

## Shop ##

Tap a card: Prompt user to buy a copy of the card

#### Double Swipe Up ####
Toggle images

### Single Finger Swipe Right ###
cycle the current set of cards.  Brings up a random new set of cards

### Two Finger Long Press ###
Display the Filter Screen

### Single Finger Long Press ###
Display the Menu

### Special interactions (iOS only) ###
These three interactions are mapped onto the screen to make it easier to play due to the lack of hardware buttons on the devices.  All of these actions are mapped to outside the gaming board.

**Above the game board:** Hides/Displays  the "MENU"
**Lower left half below game board:**  dismiss Task Board
**Lower right half below game board:** display Task Board

## Card Filter Screen ##
This screen is seen in the Deck Editor as well as the Shop.  The controls are exactly the same in both instances.

The touch interface of this part of the game has not been fully developed.  as a result successfully interacting with this screen maps the gestures to a D-Pad.  Tapping will select the currently selected item.  Popup menus work as they do across the rest of the game.  The two finger swipe gestures have no effect on this screen.


### Special interactions (iOS only) ###
These three interactions are mapped onto the screen to make it easier to play due to the lack of hardware buttons on the devices.  All of these actions are mapped to outside the gaming board.

**Above the game board:** Hides/Displays  the "MENU"
**Lower left half below game board:**  dismiss Task Board
**Lower right half below game board:** display Task Board


## Trophy Room ##