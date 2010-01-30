#ifndef _MTGDEFINITION_H_
#define _MTGDEFINITION_H_

#define DEFAULT_MENU_FONT_SCALE 1.0
#define DEFAULT_MAIN_FONT_SCALE 1.0
#define DEFAULT_TEXT_FONT_SCALE 1.0

class Constants
{
 public:
  enum
  {
    MTG_COLOR_ARTIFACT = 0,
    MTG_COLOR_GREEN = 1,
    MTG_COLOR_BLUE = 2,
    MTG_COLOR_RED = 3,
    MTG_COLOR_BLACK = 4,
    MTG_COLOR_WHITE = 5,
    MTG_COLOR_LAND = 6,

    MTG_NB_COLORS = 7,


    MTG_UNCOLORED = 0,
    MTG_FOREST = 1,
    MTG_ISLAND = 2,
    MTG_MOUNTAIN = 3,
    MTG_SWAMP = 4,
    MTG_PLAIN = 5,


    MTG_TYPE_CREATURE = 10,
    MTG_TYPE_ARTIFACT = 11,
    MTG_TYPE_ENCHANTMENT = 12,
    MTG_TYPE_SORCERY = 13,
    MTG_TYPE_LAND = 14,
    MTG_TYPE_INSTANT = 15,


    MTG_PHASE_BEFORE_BEGIN = 0,
    MTG_PHASE_UNTAP = 1,
    MTG_PHASE_UPKEEP = 2,
    MTG_PHASE_DRAW = 3,
    MTG_PHASE_FIRSTMAIN = 4,
    MTG_PHASE_COMBATBEGIN = 5,
    MTG_PHASE_COMBATATTACKERS = 6,
    MTG_PHASE_COMBATBLOCKERS = 7,
    MTG_PHASE_COMBATDAMAGE = 8,
    MTG_PHASE_COMBATEND = 9,
    MTG_PHASE_SECONDMAIN = 10,
    MTG_PHASE_ENDOFTURN = 11,
    MTG_PHASE_EOT = 11,
    MTG_PHASE_CLEANUP = 12,
    MTG_PHASE_AFTER_EOT = 13,
    NB_MTG_PHASES = 14,

    TRAMPLE = 0,
    FORESTWALK = 1,
    ISLANDWALK = 2,
    MOUNTAINWALK = 3,
    SWAMPWALK = 4,
    PLAINSWALK = 5,
    FLYING = 6,
    FIRSTSTRIKE = 7,
    DOUBLESTRIKE = 8,
    FEAR = 9,
    FLASH = 10,
    HASTE = 11,
    LIFELINK = 12,
    REACH = 13,
    SHROUD = 14,
    VIGILANCE = 15,
    DEFENSER = 16,
    DEFENDER = 16,
    BANDING = 17,
    PROTECTIONGREEN = 18,
    PROTECTIONBLUE = 19,
    PROTECTIONRED = 20,
    PROTECTIONBLACK = 21,
    PROTECTIONWHITE = 22,
    UNBLOCKABLE = 23,
    WITHER = 24,
    PERSIST = 25,
    RETRACE = 26,
    EXALTED = 27,
    NOFIZZLE = 28,
    SHADOW = 29,
    REACHSHADOW = 30,
    FORESTHOME = 31,
    ISLANDHOME = 32,
    MOUNTAINHOME = 33,
    SWAMPHOME = 34,
    PLAINSHOME = 35,
    CLOUD = 36,
    CANTATTACK = 37,
    MUSTATTACK = 38,
    CANTBLOCK = 39,
	  DOESNOTUNTAP = 40,
	  OPPONENTSHROUD = 41,
	  INDESTRUCTIBLE = 42,
    INTIMIDATE = 43,
    DEATHTOUCH = 44,
    HORSEMANSHIP = 45,
    CANTREGENERATE = 46,

    NB_BASIC_ABILITIES = 47,


    RARITY_M = 'M',   //Mythics
    RARITY_R = 'R',   //Rares
    RARITY_U = 'U',   //Uncommons
    RARITY_C = 'C',   //Commons
    RARITY_L = 'L',   //Lands
    RARITY_T = 'T',   //Tokens

    //Price for singles
    PRICE_1M = 3000,
    PRICE_1R = 500,
    PRICE_1U = 100,
    PRICE_1C = 20,
    PRICE_1L = 5,

    //Price in booster
    PRICE_XM = 2500,
    PRICE_XR = 355,
    PRICE_XU = 88,
    PRICE_XC = 8,
    PRICE_XL = 1,

    MAIN_FONT = 0,
    MENU_FONT = 1,
    MAGIC_FONT = 2,
    OPTION_FONT = 1,

    GRADE_SUPPORTED = 1,
    GRADE_BORDERLINE = 2,
    GRADE_CRAPPY = 3,
    GRADE_UNSUPPORTED = 4,
    GRADE_DANGEROUS = 5,
  };

  static char MTGColorChars[];
  static const char* MTGColorStrings[];
  static int _r[], _g[], _b[];
  static const char* MTGBasicAbilities[];
  static const char* MTGPhaseNames[];
  static const char* MTGPhaseCodeNames[];
};

#endif
