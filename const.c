/***************************************************************************
*                                                                          *
*  Magrathea is copyright (C) 1997, 1998 by Scott Baker and Jason LeBlanc  *
*                                                                          *
*    Authors must be notified, and permission granted, before using or     *
*    altering this software.                                               *
*                                                                          *
****************************************************************************/

/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,		"light"		},
    {	ITEM_SCROLL,	"scroll"		},
    {	ITEM_WAND,		"wand"		},
    { ITEM_STAFF,		"staff"		},
    { ITEM_GRENADE,		"grenade"	},
    { ITEM_SABER_CRYSTAL,	"lightsaber_crystal"	},
    { ITEM_WEAPON,	"weapon"		},
    { ITEM_TREASURE,	"treasure"		},
    { ITEM_ARMOR,		"armor"		},
    {	ITEM_POTION,	"potion"		},
    {	ITEM_CLOTHING,	"clothing"		},
    { ITEM_FURNITURE,	"furniture"		},
    {	ITEM_TRASH,		"trash"		},
    {	ITEM_CONTAINER,	"container"		},
    {	ITEM_DRINK_CON, 	"drink"		},
    {	ITEM_KEY,		"key"			},
    {	ITEM_FOOD,		"food"		},
    {	ITEM_MONEY,		"money"		},
    {	ITEM_BOAT,		"boat"		},
    {	ITEM_CORPSE_NPC,	"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"		},
    { ITEM_FOUNTAIN,	"fountain"		},
    {	ITEM_PILL,		"pill"		},
    {	ITEM_PROTECT,	"protect"		},
    {	ITEM_MAP,		"map"			},
    {	ITEM_PORTAL,	"portal"		},
    {	ITEM_WARP_STONE,	"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"		},
    {	ITEM_GEM,		"gem"			},
    {	ITEM_GRENADE,	"grenade"		},
    {	ITEM_JEWELRY,	"jewelry"		},
    { ITEM_JUKEBOX,	"jukebox"		},
    {   0,			NULL			}
};




/* weapon selection table */

const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
   { "staff",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
   { "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm},
   { "projectile", OBJ_VNUM_SCHOOL_PROJ, WEAPON_PROJ,   &gsn_projectile},
   { "battelh",  OBJ_VNUM_SCHOOL_BTL,	WEAPON_BTL,		&gsn_battelh },
   { "lightsaber",  OBJ_VNUM_SCHOOL_SWORD,	WEAPON_LIGHTSABER,	&gsn_lightsaber },
   { "stake", 	OBJ_VNUM_SCHOOL_STAFF,  WEAPON_STAKE,	&gsn_stake },
   { NULL,	0,				0,	NULL		}
};


/* wiznet table and prototype for future flag setting */

const   struct wiznet_type      wiznet_table    []              =
{
   {  "On",           	WIZ_ON,     	IM },
   {  "Prefix",		WIZ_PREFIX,		IM },
   {  "Ticks",        	WIZ_TICKS,      	IM },
   {  "Logout",       	WIZ_LOGOUT,     	IM },
   {	"Newbies",		WIZ_NEWBIE,		IM },
   {	"Levels",		WIZ_LEVELS,		IM },
   {  "Deaths",       	WIZ_DEATHS,     	IM },
   {	"PLog",		WIZ_LOG,		IM },
   {	"Time",		WIZ_TIME,		IM },
   {  "Sites",        	WIZ_SITES,      	L5 },
   {	"Spam",		WIZ_SPAM,		L5 },
   {  "Resets",       	WIZ_RESETS,     	L5 },
   {  "MobDeaths",    	WIZ_MOBDEATHS,  	L5 },
   {  "Flags",		WIZ_FLAGS,		L5 },
   {	"Penalties",	WIZ_PENALTIES,	L5 },
   {	"Saccing",		WIZ_SACCING,	L5 },
   {  "Links",        	WIZ_LINKS,      	L5 },
   {	"Load",		WIZ_LOAD,		L2 },
   {	"Restore",		WIZ_RESTORE,	L2 },
   {	"Snoops",		WIZ_SNOOPS,		L2 },
   {	"Switches",		WIZ_SWITCHES,	L2 },
   {	"Secure",		WIZ_SECURE,		L2 },
   {	"OLC",		WIZ_OLC,		L2 },
   {  "Debug",		WIZ_DEBUG,		L2 },
   {  "Debug_Exp",	WIZ_DEBUG_EXP,	L2 },
   {  "Debug_Fight",	WIZ_DEBUG_FIGHT,	L2 },
   {  "Debug_Channels",	WIZ_DEBUG_CHAN,	L2 },
   {  "Debug_Magic",	WIZ_DEBUG_MAGIC,	L2 },
   {	"Debug_Other",	WIZ_DEBUG_OTHER,	L2 },
   {	"Debug_Memory",	WIZ_DEBUG_MEMORY,	L2 },
   {	NULL,			0,			0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[MAX_DAMAGE_MESSAGE]	=
{
    { "none",		"hit",		-1		},  /*  0 */
    {	"slice",		"slice", 		DAM_SLASH	},
    { "stab",		"stab",		DAM_PIERCE	},
    {	"slash",		"slash",		DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    { "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",		"blast",		DAM_BASH	},
    { "pound",		"pound",		DAM_BASH	},
    {	"crush",		"crush",		DAM_BASH	},
    { "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    { "pierce",		"pierce",		DAM_PIERCE	},
    { "suction",		"suction",		DAM_BASH	},
    {	"beating",		"beating",		DAM_BASH	},
    { "digestion",	"digestion",	DAM_ACID	},
    {	"charge",		"charge",		DAM_BASH	},  /* 15 */
    { "slap",		"slap",		DAM_BASH	},
    {	"punch",		"punch",		DAM_BASH	},
    {	"wrath",		"wrath",		DAM_ENERGY	},
    {	"magic",		"magic",		DAM_ENERGY	},
    { "divine",		"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",		"cleave",		DAM_SLASH	},
    {	"scratch",		"scratch",		DAM_PIERCE	},
    { "peck",		"peck",		DAM_PIERCE	},
    { "peckb",		"peck",		DAM_BASH	},
    { "chop",		"chop",		DAM_SLASH	},  /* 25 */
    { "sting",		"sting",		DAM_PIERCE	},
    { "smash",		"smash",		DAM_BASH	},
    { "shbite",		"shocking bite",	DAM_LIGHTNING	},
    {	"flbite",		"flaming bite", 	DAM_FIRE	},
    {	"frbite",		"freezing bite", 	DAM_COLD	},  /* 30 */
    {	"acbite",		"acidic bite", 	DAM_ACID	},
    {	"chomp",		"chomp",		DAM_PIERCE	},
    { "drain",		"life drain",	DAM_NEGATIVE	},
    { "thrust",		"thrust",		DAM_PIERCE	},
    { "slime",		"slime",		DAM_ACID	},
    {	"shock",		"shock",		DAM_LIGHTNING	},
    { "thwack",		"thwack",		DAM_BASH	},
    { "flame",		"flame",		DAM_FIRE	},
    { "chill",		"chill",		DAM_COLD	},
    {   NULL,		NULL,			0		}
};


/* race table */

const 	struct	race_type	race_table	[]		=
{
/*
    {
	name,		pc_race?,
	act bits,	aff_by bits, aff2,	off bits,
	imm,		res,		vuln,
	form,		parts,	    remort?, remort2?
    },
*/
    { "unique",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0,0, FALSE, FALSE },

    {
	"human",		TRUE,
	0,		0, 		0, 0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {
	"draconian", 		TRUE,
	0, 			AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,	0,
	0,			RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, FALSE
    },

    {
	"red dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS,AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },

	{
	"blue dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_LIGHTNING|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },

	{
	"green dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	IMM_POISON|IMM_DISEASE,		RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    
    {
	"black dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_ACID|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    
    {
	"white dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_COLD|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_FIRE,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    
    {
	"gold dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    
    {
	"silver dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_COLD|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_FIRE,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    

    {
	"brass dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    
    {
	"bronze dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_LIGHTNING|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },
    
    {
	"copper dragon", 		TRUE,
	0, 			AFF_DARK_VISION|AFF_INFRARED|AFF_FLYING|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, AFF_DRAGON_ARMOR|AFF_DRAGON_POWER,	0,
	0,			RES_ACID|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, TRUE, TRUE
    },

    {
	"drow",			TRUE,
	0,		AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,	0,
	0,	RES_CHARM|RES_MAGIC,	VULN_IRON|VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"elf",			TRUE,
	0,		AFF_INFRARED, 0,	0,
	0,		RES_CHARM,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {
	"dwarf",		TRUE,
	0,		AFF_INFRARED, 0,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {
	"giant",		TRUE,
	0,		0,	 0,	0,
	0,		RES_FIRE|RES_COLD,	VULN_MENTAL|VULN_LIGHTNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {   "ferengi",		TRUE,
	0,		AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,	0,
	IMM_DISEASE|IMM_POISON,		0,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y, TRUE, FALSE
    },

    {
	"goblin",		TRUE,
	0,		AFF_INFRARED, 0,	0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {
	"hobbit",			TRUE,
	0,		AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,	0,
	0,		RES_CHARM,	VULN_ACID,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {
	"klingon",		TRUE,
	0,		AFF_REGENERATION|AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,	OFF_FAST,
	0,		RES_POISON|RES_DISEASE|RES_BASH,	VULN_MENTAL|VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"kobold",		TRUE,
	0,		AFF_INFRARED, 0,	0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q, FALSE, FALSE
    },

    {
	"orc",			TRUE,
	0,		AFF_INFRARED, 0,	0,
	0,		RES_DISEASE,	VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, FALSE, FALSE
    },

    {
	"troll",		TRUE,
	0,		AFF_REGENERATION|AFF_INFRARED|AFF_DETECT_HIDDEN, 0,
	OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V, FALSE, FALSE
    },

    {
	"vogon",		TRUE,
	0,		AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,		0,
	IMM_CHARM,		RES_POISON|RES_DISEASE,	VULN_MENTAL|VULN_MAGIC|VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"wyvern",		TRUE,
	0,		AFF_FLYING|AFF_DETECT_INVIS|AFF_DETECT_HIDDEN, 0,
	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X, TRUE, FALSE
    },

    {
	"sprite",   TRUE,
	0,  AFF_INFRARED|AFF_FLYING|AFF_HASTE|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0,
	0,
	0,  RES_MAGIC, VULN_BASH,
	A|H|M|V, A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"wraith",   TRUE,
	0,
	AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS|AFF_FLYING|AFF_PASS_DOOR, 0, 0,
	IMM_DISEASE|IMM_POISON,  RES_NEGATIVE|RES_WEAPON, VULN_HOLY,
	A|H|M|V, A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"minbari",   TRUE,
	0,  AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0, 0,
	0, RES_CHARM|RES_BASH|RES_MAGIC, 0,
	A|H|M|V, A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"high elf",   TRUE,
	0,  AFF_INFRARED|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS, 0, 0,
	IMM_DISEASE|IMM_POISON, RES_CHARM|RES_MAGIC, VULN_IRON|VULN_NEGATIVE,
	A|H|M|V, A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {
	"numenor",  TRUE,
	0,  AFF_DETECT_INVIS|AFF_DETECT_HIDDEN, 0,   0,
	0,   RES_CHARM,  0,
	A|H|M|V, A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },


    {
	"vampire",  TRUE,
	0,  AFF_REGENERATION|AFF_DETECT_INVIS|AFF_DETECT_HIDDEN|AFF_DARK_VISION, 0,  0,
	IMM_DISEASE|IMM_POISON,   0,  VULN_HOLY|VULN_FIRE,
	A|H|M|V, A|B|C|D|E|F|G|H|I|J|K, TRUE, FALSE
    },

    {   "bat",			FALSE,
	0,		AFF_FLYING|AFF_DARK_VISION,0,	OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P, FALSE, FALSE
    },

    {
	"bear",			FALSE,
	0,		0, 0,		OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V, FALSE, FALSE
    },

    {
	"cat",			FALSE,
	0,		AFF_DARK_VISION,0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, FALSE, FALSE
    },

    {
	"centipede",		FALSE,
	0,		AFF_DARK_VISION,0,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K	, FALSE, FALSE
    },

    {
	"dog",			FALSE,
	0,		0,0,		OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V, FALSE, FALSE
    },

    {
	"doll",			FALSE,
	0,		0,0,		0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K, FALSE, FALSE
    },

    {
	"fido",			FALSE,
	0,		0,0,		OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V, FALSE, FALSE
    },

    {
	"fox",			FALSE,
	0,		AFF_DARK_VISION,0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V, FALSE, FALSE
    },

    {
	"hobgoblin",		FALSE,
	0,		AFF_INFRARED, 0,	0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y, FALSE, FALSE
    },

    {
	"lizard",		FALSE,
	0,		0, 0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V, FALSE, FALSE
    },

    {
	"modron",		FALSE,
	0,		AFF_INFRARED, 0,		ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K, FALSE, FALSE
    },

    {
	"pig",			FALSE,
	0,		0, 0,		0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K, FALSE, FALSE
    },

    {
	"rabbit",		FALSE,
	0,		0, 0,		OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K, FALSE, FALSE
    },

    {
	"school monster",	FALSE,
	ACT_NOALIGN,		0, 0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U, FALSE, FALSE
    },

    {
	"snake",		FALSE,
	0,		0, 0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X, FALSE, FALSE
    },

    {
	"song bird",		FALSE,
	0,		AFF_FLYING, 0,		OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P, FALSE, FALSE
    },

    {
	"water fowl",		FALSE,
	0,		AFF_SWIM|AFF_FLYING, 0,	0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P, FALSE, FALSE
    },

    {
	"wolf",			FALSE,
	0,		AFF_DARK_VISION, 0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|J|K|Q|V, FALSE, FALSE
    },

    {
	"unique",		FALSE,
	0,		0, 0,		0,
	0,		0,		0,
	0,		0, FALSE, FALSE
    },


    {
	NULL, 0, 0, 0, 0, 0, 0, 0, FALSE
    }
};

const	struct	pc_race_type	pc_race_table	[MAX_PC_RACE]	=
{
    {
	 NULL, "", 0, { 100, 100, 100, 100 },
      { "" }, { 13, 13, 13, 13, 13 }, { 18, 18, 18, 18, 18 }, 0, 40, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 50
    },

/*
    DO NOT SET any class multiplyer less than 100 or the exp_per_level
    calculation will result in the class needing over 10000 exp_per_levl
    {
	"race name", 	short name, 	points,	{ class multipliers },
	{ bonus skills },
	{ base stats },		{ max stats },		size
	old_age, { STR, INT, WIS, DEX, CON, HITROLL, DAMROLL, HIT, MANA, MOVE, AC }, remort_age
    },
*/

  /* --- Grayson Druid --- */
    {
	"human",	"    Human    ",	0,	{ 100, 100, 100, 100, 100, 100, 100, 100, 200 },
	{ "" },
	{ 13, 13, 13, 13, 13, 13, 13, 13 },	{ 18, 18, 18, 18, 18, 18, 18, 18 },	SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {				 
	"draconian",	"  Draconian  ",	14,	{ 120, 120, 200, 120, 150, 130, 140, 150, 250 },
	{ "gate", "summon", "buffet", "sweep" },
	{ 18, 12, 12, 11, 15, 12, 12, 10 },	{ 25, 20, 20, 17, 23, 17, 17, 14, 100, 100, 100 }, SIZE_LARGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"red dragon",	" Red Dragon  ",	42,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "fire breath", "buffet", "sweep" },
	{ 18, 12, 12, 7, 15, 10, 10, 10 },	{ 34, 24, 25, 13, 33, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"blue dragon",	" Blue Dragon ",	38,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "lightning breath", "buffet", "sweep" },
	{ 18, 12, 12, 7, 15, 10, 10, 10 },	{ 31, 23, 22, 13, 30, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },
    
    {				 
	"green dragon",	"Green Dragon ",	38,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "gas breath", "buffet", "sweep" },
	{ 18, 12, 12, 7, 15, 10, 10, 10 },	{ 31, 23, 24, 13, 30, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },
    
    {				 
	"black dragon",	"Black Dragon ",	34,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "acid breath", "buffet", "sweep" },
	{ 18, 12, 12, 7, 15, 10, 10, 10 },	{ 29, 22, 24, 13, 30, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"white dragon",	"White Dragon ",	33,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "frost breath", "buffet", "sweep" },
	{ 18, 12, 12, 7, 15, 10, 10, 10 },	{ 30, 20, 21, 13, 30, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"gold dragon",	" Gold Dragon ",	45,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "fire breath", "buffet", "sweep" },
	{ 18, 12, 12, 11, 15, 10, 10, 10 },	{ 34, 27, 27, 13, 33, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"silver dragon",	"Silver Dragon",	44,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "frost breath", "buffet", "sweep" },
	{ 18, 12, 12, 11, 15, 10, 10, 10 },	{ 32, 26, 26, 13, 31, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"brass dragon",	" Brass Dragon",	40,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "fire breath", "buffet", "sweep" },
	{ 18, 12, 12, 11, 15, 10, 10, 10 },	{ 29, 22, 23, 13, 30, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"bronze dragon",	"Bronze Dragon",	40,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "lightning breath", "buffet", "sweep" },
	{ 18, 12, 12, 11, 15, 10, 10, 10 },	{ 31, 24, 25, 13, 31, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

	{				 
	"copper dragon",	"Copper Dragon",	40,	{ 220, 220, 300, 220, 450, 230, 230, 800, 500 },
	{ "gate", "summon", "acid breath", "buffet", "sweep" },
	{ 18, 12, 12, 11, 15, 10, 10, 10 },	{ 30, 23, 24, 13, 30, 13, 14, 12 }, SIZE_HUGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },


    {
	"drow",		"     Drow    ",	10,	{ 100, 100, 120, 150, 200, 140, 160, 140, 220 },
	{ "sneak", "hide", "identify", "farsight", "gate", "summon" },
	{ 14, 19, 16, 17, 14, 15, 13, 13 },	{ 18, 23, 19, 23, 16, 21, 18, 18 }, SIZE_SMALL,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"elf",		"     Elf     ",	5,	{ 100, 125,  100, 120, 200, 100, 120, 100, 210 },
	{ "sneak", "hide" },
	{ 12, 14, 13, 15, 11, 15, 13, 10 },	{ 16, 20, 18, 21, 15, 21, 17, 15 }, SIZE_SMALL,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"dwarf",	"    Dwarf   ",	6,	{ 150, 100, 125, 100, 115, 130, 110, 100, 210 },
	{ "berserk" },
	{ 14, 12, 14, 10, 15, 10, 16, 12 },	{ 20, 16, 19, 14, 21, 14, 20, 16 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"giant",	"    Giant    ",	6,	{ 200, 150, 150, 100, 100, 150, 120, 130, 280 },
	{ "bash", "fast healing" },
	{ 16, 11, 13, 11, 14, 11, 8, 8 },	{ 22, 15, 18, 15, 20, 15, 13, 13 }, SIZE_LARGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {	         
	"ferengi",	"   Ferengi   ",		8,	{ 120, 120, 100, 200, 150, 200, 130, 140, 300 },
	{ "haggle", "farsight", "steal", "haste", "gate", "summon" },
	{ 14, 13, 14, 11, 12, 15, 15, 18 },	{ 19, 17, 19, 22, 18, 20, 20, 23 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"goblin",	"   Goblin    ",	4,	{ 200, 150, 100, 150, 150, 150, 120, 140, 270 },
	{ "infravision", "detect hidden" },
	{ 12, 11, 13, 12, 12, 12, 10, 10 },	{ 18, 15, 18, 20, 17, 17, 14, 12 }, SIZE_SMALL,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"hobbit",		"   Hobbit   ",	7,	{ 100, 100,  100, 120, 150, 110, 120, 130, 240 },
	{ "sneak", "hide", "meditation" },
	{ 12, 16, 16, 15, 12, 12, 12, 12 },	{ 15, 22, 19, 21, 15, 17, 16, 15 }, SIZE_SMALL,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"klingon",	"   Klingon   ",	9,	{ 200, 120, 200, 100, 100, 150, 120, 120, 270 },
	{ "berserk", "fast healing", "battelh", "gate", "summon"  },
	{ 17, 12, 14, 12, 15, 16, 16, 18 },	{ 26, 18, 18, 18, 23, 21, 21, 24 }, SIZE_LARGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"kobold",	"   Kobold    ",	5,	{ 150, 120, 120, 100, 120, 130, 120, 130, 270 },
	{ "bash", "fast healing" },
	{ 14, 11, 13, 11, 12, 14, 10, 9 },	{ 19, 15, 18, 16, 18, 19, 14, 12 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"orc",	"     Orc     ",	6,	{ 170, 100, 200, 110, 150, 150, 120, 130, 280 },
	{ "bash" },
	{ 12, 13, 13, 12, 13, 12, 8, 8 },	{ 20, 18, 18, 15, 18, 17, 15, 13 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"troll",	"    Troll    ",	4,	{ 160, 120, 110, 100, 120, 150, 130, 150, 350 },
	{ "fast healing" },
	{ 14, 11, 13, 11, 13, 10, 8, 8 },	{ 20, 15, 18, 13, 19, 15, 11, 11 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"vogon",	"    Vogon    ",	7,	{ 250, 150, 150, 100, 100, 200, 150, 160, 350 },
	{ "bash", "fast healing", "sanctuary", "gate", "summon", "poetry" },
	{ 14, 11, 13, 11, 17, 10, 13, 15 },	{ 23, 15, 18, 13, 23, 15, 18, 22 }, SIZE_LARGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"wyvern",	"    Wyvern   ",	6,	{ 100, 100, 200, 100, 150, 150, 130, 140, 300 },
	{ "haste", "fast healing", "gate", "summon", "sweep", "sting", "buffet" },
	{ 16, 11, 13, 12, 17, 14, 8, 8 },	{ 22, 16, 21, 15, 18, 19, 12, 12 }, SIZE_LARGE,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
    },

    {
	"sprite", "    Sprite   ",  10, { 110, 180, 100, 250, 250, 140, 110, 100, 210 },
	{ "sneak", "hide", "gate", "summon" },
	{ 10, 15, 14, 18, 10, 20, 17, 12 }, { 15, 23, 19, 25, 15, 28, 19, 17 }, SIZE_TINY,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
   },

   {
	"wraith",  "    Wraith   ", 10, { 100, 120, 100, 150, 300, 200, 350, 150, 300 },
	{ "gate", "summon", "curse", "chill touch", "drain" },
	{ 14, 19, 16, 16, 14, 15, 13, 13 }, { 20, 24, 25, 19, 17, 22, 18, 18 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
   },

   {
	"minbari",  "   Minbari   ", 12, { 130, 100, 200, 110, 200, 100, 110, 100, 200 },
	{ "gate", "summon", "spinkick" },
	{ 16, 15, 16, 12, 14, 15, 14, 15 }, { 22, 23, 21, 18, 22, 20, 22, 26}, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
   },

   {
	"high elf",  "  High Elf   ", 10, { 100, 100, 120, 150, 200, 110, 110, 100, 210 },
	{ "sneak", "hide", "identify", "farsight", "gate", "summon" },
	{ 14, 19, 16, 17, 14, 15, 12, 12 }, { 18, 23, 19, 23, 16, 22, 18, 18 }, SIZE_SMALL,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
   },

   {
	"numenor", "   Numenor   ", 5, { 100, 100, 100, 100, 100, 100, 100, 100, 200 },
	{ "gate", "summon", "fast healing" },
	{ 15, 15, 15, 15, 15, 15, 15, 15 }, { 22, 22, 22, 22, 22, 22, 22, 22 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
   },

   {
	"vampire", "   Vampire   ", 10, { 180, 180, 180, 180, 480, 280, 400, 100, 500 },
	{ "gate", "summon", "fast healing", "haste" },
	{ 13, 13, 13, 13, 13, 13, 13, 13 }, { 26, 21, 24, 21, 24, 22, 21, 18 }, SIZE_MEDIUM,
	40, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 50
   }
};

/* --- Grayson Druid --- */







/*
 * Class table.
 */

const	struct	class_type	class_table	[MAX_CLASS]	=
{

/*
    {
	"name", "who",  Primary Attribute,  First Weapon,
	{ Guild Rooms },  Max_Skill, THAC0_0, THAC0_32, hp_min, hp_max, Gain_Mana,
	"base skills", "default skills", FALSE
    },
*/

    {
	"mage", "Mag",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3018, 9618, 12221 },  75,  20, 6,  6,  8, TRUE,
	"mage basics", "mage default", FALSE
    },

    {
	"cleric", "Cle",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
	{ 3003, 9619, 12200 },  75,  20, 2,  7, 10, TRUE,
	"cleric basics", "cleric default", FALSE
    },

    {
	"thief", "Thi",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3028, 9639, 12231 },  75,  20,  -4,  8, 13, FALSE,
	"thief basics", "thief default", FALSE
    },

    {
	"warrior", "War",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3022, 9633, 12210 },  75,  20,  -10,  11, 18, FALSE,
	"warrior basics", "warrior default", FALSE
    },

    {
	"cyborg", "Cyb",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 1212, 1212, 12203 },  75,  20,  -8,  8, 12, FALSE,
	"cyborg basics", "cyborg default", FALSE
    },
/* --- Grayson Druid --- */
    {
        "druid", "Dru",  STAT_WIS,  OBJ_VNUM_SCHOOL_SPEAR,
        { 1212, 1212, 12215 },  75,  20, 2,  7, 14, TRUE,
        "druid basics", "druid default", FALSE
    },
    
    {
        "demon hunter", "Hnt",  STAT_AGI,  OBJ_VNUM_SCHOOL_SPEAR,
        { 1212, 1212, 1212 },  75,  20, 2,  7, 14, TRUE,
        "hunter basics", "hunter default", FALSE
    },


    {
        "monk", "Mnk",  STAT_WIS,  OBJ_VNUM_SCHOOL_SPEAR,
        { 1212, 1212, 12215 },  75,  20, 2,  7, 14, TRUE,
        "monk basics", "monk default", FALSE

    },


    {
        "jedi", "Jdi",  STAT_AGI,  OBJ_VNUM_SCHOOL_SPEAR,
        { 1212, 1212, 1212 },  75,  20, 2,  7, 16, TRUE,
        "jedi basics", "jedi default", FALSE
    },


    {
	NULL, "", 	0,		0,
	{ 1212, 1212, 1212 },	0,	0,	0,	0,	0,	FALSE,
	NULL,	NULL,	FALSE
    }
};


/*
 * Titles.
 */

char *	const			title_table	[MAX_CLASS][MAX_LEVEL+1][2] =
{
    {
	{ "Man",			"Woman"				},

	{ "Apprentice of Magic",	"Apprentice of Magic"		},
	{ "Spell Student",		"Spell Student"			},
	{ "Scholar of Magic",		"Scholar of Magic"		},
	{ "Delver in Spells",		"Delveress in Spells"		},
	{ "Medium of Magic",		"Medium of Magic"		},

	{ "Scribe of Magic",		"Scribess of Magic"		},
	{ "Seer",			"Seeress"			},
	{ "Sage",			"Sage"				},
	{ "Illusionist",		"Illusionist"			},
	{ "Abjurer",			"Abjuress"			},

	{ "Invoker",			"Invoker"			},
	{ "Enchanter",			"Enchantress"			},
	{ "Conjurer",			"Conjuress"			},
	{ "Magician",			"Witch"				},
	{ "Creator",			"Creator"			},

	{ "Savant",			"Savant"			},
	{ "Magus",			"Craftess"			},
	{ "Wizard",			"Wizard"			},
	{ "Warlock",			"War Witch"			},
	{ "Sorcerer",			"Sorceress"			},

	{ "Elder Sorcerer",		"Elder Sorceress"		},
	{ "Grand Sorcerer",		"Grand Sorceress"		},
	{ "Great Sorcerer",		"Great Sorceress"		},
	{ "Golem Maker",		"Golem Maker"			},
	{ "Greater Golem Maker",	"Greater Golem Maker"		},

	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},


	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Master of all Magic",	"Master of all Magic"		},

 	{ "Master Mage",		"Master Mage"			},
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },

        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },

        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },

	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},

	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},

	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},
	{ "Monk",			"Nun"				},
	{ "Healer",			"Healess"			},

	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},

	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},

	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},

	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},

	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},

	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},

	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},

	{ "Holy Hero",			"Holy Heroine"			},
	{ "Holy Avatar",		"Holy Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },

        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },

        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },

	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "Avatar of Death",		"Avatar of Death",		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Demigod of Assassins",	"Demigoddess of Assassins"	},
	{ "Immortal Assasin",		"Immortal Assassin"		},
	{ "God of Assassins",		"God of Assassins",		},
	{ "Deity of Assassins",		"Deity of Assassins"		},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "Master Warrior",		"Master Warrior"		},
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },

        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },

        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },

	{ "Knight Hero",		"Knight Heroine"		},
	{ "Avatar of War",		"Avatar of War"			},
	{ "Angel of War",		"Angel of War"			},
	{ "Demigod of War",		"Demigoddess of War"		},
	{ "Immortal Warlord",		"Immortal Warlord"		},
	{ "God of War",			"God of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    }
};





/*
 * Attribute bonus tables.
 */

const	struct	str_app_type	str_app		[36]		=

{

    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 130, 12 },
    {  0,  0, 130, 13 }, /* 13  */
    {  0,  1, 140, 14 },
    {  1,  1, 150, 15 }, /* 15  */
    {  2,  2, 165, 16 },
    {  3,  3, 180, 22 },
    {  4,  3, 200, 25 }, /* 18  */
    {  5,  4, 225, 30 },
    {  6,  5, 250, 35 }, /* 20  */
    {  7,  6, 300, 40 },
    {  8,  6, 350, 45 },
    {  5,  7, 400, 50 },
    {  5,  8, 450, 55 },
    {  6,  9, 500, 60 },  /* 25   */
    {  6,  9, 550, 65 },
    {  6,  9, 600, 70 },
    {  7,  10, 650, 75 },
    {  9,  10, 700, 80 },
    {  9,  10, 750, 85 },
    {  10,  10, 800, 90 },
    {  10,  11, 850, 95 },
    {  11,  11, 900, 95 },
    {  11,  11, 950, 95 },
    {  12,  11, 999, 95 }

};







const	struct	int_app_type	int_app		[36]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 80 },
    { 85 },	/* 25 */
    { 85 },
    { 85 },
    { 85 },
    { 90 },
    { 90 },	/* 30 */
    { 90 },
    { 90 },
    { 90 },
    { 90 },
    { 95 }	/* 35 */

};



const	struct	wis_app_type	wis_app		[36]		=
{
    { 0, 0 },	/*  0 */
    { 0, 0 },	/*  1 */
    { 0, 0 },
    { 0, 0 },	/*  3 */
    { 0, 0 },
    { 1, 0 },	/*  5 */
    { 1, 0 },
    { 1, 0 },
    { 1, 0 },
    { 1, 0 },
    { 1, 0 },	/* 10 */
    { 1, 0 },
    { 1, 0 },
    { 1, 0 },
    { 1, 0 },
    { 2, 0 },	/* 15 */
    { 2, 0 },
    { 2, 0 },
    { 3, 1 },	/* 18 */
    { 3, 1 },
    { 3, 2 },	/* 20 */
    { 3, 2 },
    { 4, 3 },
    { 4, 3 },
    { 4, 4 },
    { 5, 4 },	/* 25 */
    { 5, 5 },
    { 5, 5 },
    { 6, 6 },
    { 6, 6 },
    { 6, 7 },	/* 30 */
    { 7, 7 },
    { 7, 8 },
    { 7, 8 },
    { 8, 9 },
    { 9, 9 }	/* 35 */

};



const	struct	dex_app_type	dex_app		[36]		=

{

    {  60,  -5,  -5 },   /* 0 */
    {  50,  -5,  -5 },   /* 1 */
    {  50,  -4,  -4 },
    {  40,  -4,  -4 },
    {  30,  -3,  -3 },
    {  20,  -3,  -3 },   /* 5 */
    {  10,  -2,  -2 },
    {  0,  -2,  -2 },
    {  0,  -1,  -1 },
    {  0,  -1,  -1 },
    {  0,  0,  0 },   /* 10 */
    {  0,  0,  0 },
    {  0,  0,  0 },
    {  0,  0,  1 },
    {   0,  1,  1 },
    { - 10,  1,  1 },   /* 15 */
    { - 15,  1,  1 },
    { - 20,  1,  1 },
    { - 30,  1,  1 },
    { - 40,  1,  1 },
    { - 50,  2,  2 },   /* 20 */
    { - 60,  2,  2 },
    { - 75,  2,  2 },
    { - 90,  2,  2 },
    { -105,  3,  3 },
    { -120,  3,  3 },    /* 25 */
    { -125,  3,  3 },
    { -130,  3,  3 },
    { -135,  3,  4 },
    { -140,  3,  4 },
    { -144,  3,  4 },
    { -150,  3,  4 },
    { -155,  4,  5 },
    { -160,  4,  5 },
    { -165,  4,  5 },
    { -170,  4,  6 }

};





const	struct	con_app_type	con_app		[36]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 },    /* 25 */
    {  9, 99 },
    {  10, 99 },
    {  11, 99 },
    {  12, 99 },
    {  12, 99 },	/* 30 */
    {  13, 99 },
    {  13, 99 },
    {  14, 99 },
    {  14, 99 },
    {  15, 99 }		/* 35 */
};


const	struct	agi_app_type	agi_app		[36]		=
{

    { -5, -4 },  /* 0  */
    { -5, -4 },  /* 1  */
    { -3, -2 },
    { -3, -1 },  /* 3  */
    { -2, -1 },
    { -2, -1 },  /* 5  */
    { -1,  0 },
    { -1,  0 },
    {  0,  0 },
    {  0,  0 },
    {  0,  0 }, /* 10  */
    {  0,  0 },
    {  0,  0 },
    {  1,  0 }, /* 13  */
    {  1,  1 },
    {  2,  1 }, /* 15  */
    {  2,  2 },
    {  2,  3 },
    {  3,  3 }, /* 18  */
    {  3,  4 },
    {  4,  5 }, /* 20  */
    {  4,  6 },
    {  5,  6 },
    {  5,  7 },
    {  6,  8 },
    {  6,  9 },  /* 25   */
    {  7,  9 },
    {  7,  9 },
    {  8,  10 },
    {  8,  10 },
    {  9,  10 }, // *30*
    {  9,  10 },
    {  10,  11 },
    {  10,  11 },
    {  11,  11 },
    {  12,  11 }    // *35*

};




/*

 * Liquid properties.

 * Used in world.obj.

 */

const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	},
    { "dr pepper",		"brown",	{   0, 2,  9, 2, 12 }	},
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   },
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { "blood wine",             "red",          { 200, 2,  9, 2, 12 }   },
    { "pepsi",                  "brown",        {   0, 2,  9, 2, 12 }   },
    { "mountain dew",		"{Ggreen{w",    {   0, 2,  9, 2, 12 }   },
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};

/* --- Grayson Druid --- */

/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n



const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
	"reserved",		{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },	{ 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			"",		""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99 },
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_str,		SLOT(201),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99 },
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_int,		SLOT(202),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99 },
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_wis,		SLOT(203),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_dex,		SLOT(204),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99 },
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_con,		SLOT(205),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_hitroll,		SLOT(206),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_damroll,		SLOT(207),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_hit,		SLOT(208),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_mana,		SLOT(209),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_move,		SLOT(210),	0,	0,
	"old age",		"",	""
    },

    {
	"old age",	{ 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099, 20099 },     { 99,  99,  99,  99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_DEAD,
	&gsn_ageaffect_ac,		SLOT(211),	0,	0,
	"old age",		"",	""
    },

    {
	"armor",		{  7,  47, 1000, 1000, 1000, 1000, 53, 1000, 1000 },     { 1,  1,  99,  99, 99, 99,  1, 99, 99},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less armored.",	""
    },

    {
	"bless",		{ 53,  7, 1000, 1000, 1000, 1000, 33, 9, 1000 },     { 1,  1,  99,  99, 99, 99,  1, 1, 99},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous.",
	"$p's holy aura fades."
    },

    {
	"blindness",		{  12,  8, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99,  99, 99, 99},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again.",	""
    },

    {
	"burning hands",	{  7, 53, 1000, 1000, 1000, 15, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 1,  99, 99, 99 },
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	""
    },

    {
	"call lightning",	{ 26, 18, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99,  99, 99, 99},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!",	""
    },

    {   "calm",			{ 48, 16, 1000, 1000, 1000, 25, 1000, 26, 38 },     { 1,  1,  99,  99, 99, 1,  99, 99, 1},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.",	""
    },

    {
	"cancellation",		{ 18, 26, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99,  99, 99, 99},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	""			"!cancellation!",	""
    },

    {
	"cause critical",	{ 53,  13, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99,  99, 99, 99},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	""
    },

    {
	"cause light",		{ 53,  1, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	""
    },

    {
	"cause serious",	{ 53,  7, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	""
    },

    {
	"chain lightning",	{ 33, 85, 1000, 1000, 97, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 2, 99,  99, 99, 99},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	""
    },

    {
	"change sex",		{ 100, 125, 150, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 99, 99, 99, 99},
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(82),	15,	12,
	"",			"Your body feels familiar again.",	""
    },

    {
	"charm person",		{ 20, 90, 70, 1000, 1000, 35, 1000, 1000, 72 },     { 1,  1,  2,  99, 99, 1, 99, 99, 1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",			"You feel more self-confident.",	""
    },

    {
	"chill touch",		{  4, 53, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	""
    },


    {

	"colour spray",		{ 16, 53, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(10),	15,	12,

	"colour spray",		"!Colour Spray!",	""

    },



    {

	"continual light",	{  6,  4, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_continual_light,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(57),	 7,	12,

	"",			"!Continual Light!",	""

    },



    {

	"control weather",	{ 15, 19, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_control_weather,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(11),	25,	12,

	"",			"!Control Weather!",	""

    },



    {

	"create food",		{ 10, 5, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_create_food,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(12),	 5,	12,

	"",			"!Create Food!",	""

    },



    {

	"create rose",		{ 16, 11, 1000, 1000, 1000, 5, 1000, 1000, 1000 }, 	{ 1, 1, 99, 99, 99, 1, 99, 99, 99 },

	spell_create_rose,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(511),	30, 	12,

	"",			"!Create Rose!",	""

    },



    {

	"create spring",	{ 14, 17, 1000, 1000, 1000, 10, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 1, 99, 99, 99},

	spell_create_spring,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(80),	20,	12,

	"",			"!Create Spring!",	""

    },



    {

	"create water",		{ 8,  3, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,

	NULL,			SLOT(13),	 5,	12,

	"",			"!Create Water!",	""

    },



    {

	"cure blindness",	{ 1000,  6, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(14),	 5,	12,

	"",			"!Cure Blindness!",	""

    },



    {

	"cure critical",	{ 1000,  13, 1000, 1000, 1000, 61, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 1, 99, 99, 99},

	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(15),	20,	12,

	"",			"!Cure Critical!",	""

    },



    {

	"cure disease",		{ 1000, 13, 1000, 1000, 1000, 20, 1000, 15, 13 },     { 99,  1,  99,  99, 99, 1, 99, 1, 1},

	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,

	NULL,			SLOT(501),	20,	12,

	"",			"!Cure Disease!",	""

    },



    {

	"cure light",		{ 1000,  1, 1000, 1000, 1000, 25, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 1, 99, 99, 99},

	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(16),	10,	12,

	"",			"!Cure Light!",		""

    },



    {

	"cure poison",		{ 1000,  14, 1000, 1000, 1000, 10, 1000, 1000, 18 },     { 99,  1,  99,  99, 99, 1, 99, 99, 1},

	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,

	NULL,			SLOT(43),	 5,	12,

	"",			"!Cure Poison!",	""

    },



    {

	"cure serious",		{ 1000,  7, 1000, 1000, 1000, 50, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 1, 99, 99, 99},

	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(61),	15,	12,

	"",			"!Cure Serious!",	""

    },



    {

	"curse",		{ 18, 18, 26, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 99, 99, 99, 99},

	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,

	&gsn_curse,		SLOT(17),	20,	12,

	"curse",		"The curse wears off.",

	"$p is no longer impure."

    },



    {

	"demonfire",		{ 1000, 30, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(505),	20,	12,

	"torments",		"!Demonfire!",		""

    },



    {

	"detect evil",		{ 11,  4, 12, 1000, 1000, 1000, 15, 18, 10 },     { 1,  1,  2,  99, 99, 99, 1, 1, 1},

	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,

	NULL,			SLOT(18),	 5,	12,

	"",			"The red in your vision disappears.",	""

    },



    {
        "detect good",          { 11,  4, 12, 1000, 1000, 1000, 1000, 18, 10 },     { 1,  1,  2,  99, 99, 99, 99, 1, 1},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
        "",                     "The gold in your vision disappears.",	""
    },



    {

	"detect hidden",	{ 15, 11, 12, 1000, 1000, 35, 1000, 1000, 1000 },     { 1,  1,  2,  99, 2, 1, 99, 99, 1},

	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,

	NULL,			SLOT(44),	 5,	12,

	"",			"You feel less aware of your surroundings.",

	""

    },



    {

	"detect invis",		{  3,  8, 6, 1000, 10, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 3, 99, 99, 99, 99},

	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,

	NULL,			SLOT(19),	 5,	12,

	"",			"You no longer see invisible objects.",

	""

    },



    {

	"detect magic",		{  2,  6, 5, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 99, 99, 99, 99},

	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,

	NULL,			SLOT(20),	 5,	12,

	"",			"The detect magic wears off.",	""

    },



    {

	"detect poison",	{ 15,  7, 9, 1000, 1000, 1000, 1000, 1000, 10 },     { 1,  1,  2,  99, 99, 99, 99, 99, 99},

	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,

	NULL,			SLOT(21),	 5,	12,

	"",			"!Detect Poison!",	""

    },



    {

	"dispel evil",		{ 1000, 15, 1000, 1000, 1000, 1000, 36, 1000, 1000 },     { 99,  1,  99,  99, 99, 1, 99, 99, 99},

	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(22),	15,	12,

	"dispel evil",		"!Dispel Evil!",	""

    },



    {

        "dispel good",          { 1000, 15, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        NULL,                   SLOT(512),      15,     12,

        "dispel good",          "!Dispel Good!",	""

    },



    {

	"dispel magic",		{ 16, 24, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(59),	15,	12,

	"",			"!Dispel Magic!",	""

    },



    {
	"earthquake",		{ 53,  10, 1000, 1000, 1000, 30, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 1, 99, 99, 99},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		""
    },

    {
	"holy signet",		{ 1000,  355, 1000, 1000, 1000, 1000, 100, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 1, 99, 99},
	spell_holy_signet,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(561),	55,	20,
	"holy signet",		"!Holy Signet!",		""
    },

    {

	"enchant armor",	{ 16, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 2,  99,  99,  99, 99, 99, 99, 99, 99 },

	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,

	NULL,			SLOT(510),	100,	24,

	"",			"!Enchant Armor!",	""

    },



    {

	"enchant weapon",	{ 17, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 2,  99, 99,  99, 99, 99, 99, 99, 99},

	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,

	NULL,			SLOT(24),	100,	24,

	"",			"!Enchant Weapon!",	""

    },



    {
	"energy drain",		{ 19, 22, 1000, 1000, 27, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 2, 99, 99, 99, 99},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
	"energy drain",		"!Energy Drain!",	""
    },

 	{
        "entangle",    { 1000, 1000, 1000, 1000, 1000, 81, 1000, 1000, 1000 },     { 99,  99,  99,  99, 99, 1, 99, 99, 99},
        spell_entangle,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(26),      50,     25,
        "",                     "You are released from the vines.",	""
    },

    {
	"faerie fire",		{  6,  3, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	""
    },

    {
	"faerie fog",		{ 14, 21, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"!Faerie Fog!",		""
    },

    {
	"farsight",		{ 14, 16, 16, 1000, 31, 15, 1000, 1000, 21 },	{ 1, 1, 2, 99, 3, 1, 99, 99, 1},
	spell_farsight,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(521),	36,	20,
	"farsight",		"!Farsight!",		""
    },

    {
	"fireball",		{ 22, 1000, 30, 1000, 1000, 55, 1000, 1000, 1000 },     { 1,  99,  2,  99, 99, 2, 99, 99, 99},
	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!",		""
    },

    {
	"fireproof",		{ 13, 12, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 1, 1, 99, 99, 99, 99, 99, 99, 99},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",			"",	"$p's protective aura fades."
    },

    {
	"flamestrike",		{ 1000, 20, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		""
    },

    {
	"fly",			{ 10, 1000, 1000, 1000, 7, 1000, 1000, 1000, 1000 },     { 1,  99,  99,  99, 2, 99, 99, 99, 99},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(56),	10,	18,
	"",			"You slowly float to the ground.",	""
    },

    {
	"floating disc",	{  4, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 1, 99, 99, 99, 99, 99, 99, 99, 99},
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	"",			"!Floating disc!",	""
    },

    {
        "frenzy",               { 53, 63, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Your rage ebbs.",	""
    },

    {
	"gate",			{ 27, 31, 38, 42, 43, 42, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 3, 99, 99, 99},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		""
    },

    {
	"giant strength",	{  11, 1000, 1000, 1000, 24, 18, 1000, 1000, 1000 },     { 1,  99,  99,  99, 3, 2, 99, 99, 99},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(39),	20,	12,
	"",			"You feel weaker.",	""
    },

    {
	"harm",			{ 53, 23, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!,		"""
    },

    {
	"haste",		{ 21, 25, 28, 35, 25, 28, 1000, 22, 38 },     { 1,  1,  2,  3, 3, 3, 99, 1, 1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	""
    },



    {

	"heal",			{ 37, 21, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(28),	50,	12,

	"",			"!Heal!",		""

    },



    {

	"heat metal",		{ 125, 16, 1000, 1000, 98, 1000, 1000, 1000, 1000 },	{ 1, 1, 99, 99, 3, 99, 99, 99, 99 },

	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(516), 	25,	18,

	"spell",		"!Heat Metal!",		""

    },



    {

	"holy word",		{ 1000, 36, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 99,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,

	NULL,			SLOT(506), 	200,	24,

	"divine wrath",		"!Holy Word!",		""

    },



    {

	"identify",		{ 15, 16, 18, 23, 22, 17, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 3, 99, 99, 99},

	spell_identify,		TAR_OBJ_INV,		POS_STANDING,

	NULL,			SLOT(53),	12,	24,

	"",			"!Identify!",		""

    },



    {

	"infravision",		{  9,  13, 10, 1000, 26, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 3, 99, 99, 99, 99},

	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,

	NULL,			SLOT(77),	 5,	18,

	"",			"You no longer see in the dark.",	""

    },



    {

	"invisibility",		{  5, 53, 9, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 99, 99, 99, 99},

	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,

	&gsn_invis,		SLOT(29),	 5,	12,

	"",			"You are no longer invisible.",

	"$p fades into view."

    },
    
    //Grayson improved invis 9 March 2004
    {

	"improved invisibility",		{  150, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  99,  99,  99, 99, 99, 99, 99, 99},

	spell_improv_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,

	&gsn_improv_invis,		SLOT(117),	 50,	12,

	"",			"You are no longer invisible.",

	"$p fades into view."

    },
    
    {

	"improved detection",		{  150, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  99,  99,  99, 99, 99, 99, 99, 99},

	spell_improv_detect,		TAR_CHAR_SELF,	POS_STANDING,

	&gsn_improv_detect,		SLOT(118),	 50,	12,

	"",			"You no longer see invisible things.",

	""

    },    

    //Grayson improved invis 9 March 2004

    {

	"know alignment",	{  12,  9, 20, 1000, 1000, 1000, 1000, 1000, 10 },     { 1,  1,  2,  99, 99, 99, 99, 99, 1},

	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(58),	 9,	12,

	"",			"!Know Alignment!",	""

    },

    {
	"lightning",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 82 }, 	{ 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(78),	75,	20,
	"{Blightning{w",		"!Lightning!"
    },

    {

	"lightning bolt",	{  13, 23, 1000, 1000, 1000, 30, 1000, 1000, 55 },     { 1,  1,  99,  99, 99, 1, 99, 99, 1},

	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(30),	15,	12,

	"lightning bolt",	"!Lightning Bolt!",	""

    },



    {

	"locate object",	{  9, 15, 1000, 1000, 55, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 3, 99, 99, 99, 99},

	spell_locate_object,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(31),	20,	18,

	"",			"!Locate Object!",	""

    },



    {

	"magic missile",	{  1, 1000, 1000, 1000, 5, 1000, 1000, 1000, 1000 },     { 1,  99,  99,  99, 3, 99, 99, 99, 99},

	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(32),	15,	12,

	"magic missile",	"!Magic Missile!",	""

    },



    {

	"mass healing",		{ 1000, 38, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 99,  2,  99,  99, 99, 99, 99, 99, 99},

	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(508),	100,	36,

	"",			"!Mass Healing!",	""

    },



    {

	"mass invis",		{ 42, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  99,  99,  99, 99, 99, 99, 99, 99},

	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,

	&gsn_mass_invis,	SLOT(69),	20,	24,

	"",			"You are no longer invisible.",		""

    },



    {

        "nexus",                { 110, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 2,  99,  99,  99, 99, 99, 99, 99, 99},

        spell_nexus,            TAR_IGNORE,             POS_STANDING,

        NULL,                   SLOT(520),       150,   36,

        "",                     "!Nexus!",		""

    },



    {

	"pass door",		{ 34, 127, 65, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 99, 99, 99, 99},

	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,

	NULL,			SLOT(74),	20,	12,

	"",			"You feel solid again.",	""

    },



    {

	"plague",		{ 37, 17, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	&gsn_plague,		SLOT(503),	20,	12,

	"sickness",		"Your sores vanish.",	""

    },



    {

	"poison",		{ 37,  12, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,

	&gsn_poison,		SLOT(33),	10,	12,

	"poison",		"You feel less sick.",

	"The poison on $p dries up."

    },



    {

        "portal",               { 55, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 2,  99,  99,  99, 99, 99, 99, 99, 99},

        spell_portal,           TAR_IGNORE,             POS_STANDING,

        NULL,                   SLOT(519),       100,     24,

        "",                     "!Portal!",		""

    },

    {
	"protection",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 62 },     { 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_protection,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	50,	18,
	"",			"The protection of {WThe Force{w fades away.",	""
    },

    {
	"protection evil",	{ 12,  9, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	""
    },



    {

        "protection good",      { 12,  9, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

        spell_protection_good,  TAR_CHAR_SELF,          POS_STANDING,

        NULL,                   SLOT(514),       5,     12,

        "",                     "You feel less protected.",	""

    },



    {

        "ray of truth",         { 1000, 30, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        NULL,                   SLOT(518),      20,     12,

        "ray of truth",         "!Ray of Truth!",	""

    },



    {

	"recharge",		{ 29, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 1, 99, 99, 99, 99, 99, 99, 99, 99 },

	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,

	NULL,			SLOT(517),	60,	24,

	"",			"!Recharge!",		""

    },



    {

	"refresh",		{  8,  5, 1000, 1000, 1000, 8, 1000, 1000, 1000 },      { 1,  1,  99,  99, 99, 1, 99, 99, 99},

	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,

	NULL,			SLOT(81),	12,	18,

	"refresh",		"!Refresh!",		""

    },



    {

	"remove curse",		{ 1000, 18, 1000, 1000, 1000, 1000, 28, 20, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,

	NULL,			SLOT(35),	 5,	12,

	"",			"!Remove Curse!",	""

    },



    {

	"renew",		{ 1000, 97, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_renew,	TAR_OBJ_CHAR_DEF,	POS_STANDING,

	NULL,			SLOT(600),	 300,	24,

	"",			"!Renew!",	""

    },



    {
	"sanctuary",		{ 36, 41, 55, 63, 43, 58, 1000, 1000, 142 },     { 1,  1,  2,  3, 3, 3, 99, 99, 1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	""
    },



    {

	"shield",		{ 20, 60, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,

	NULL,			SLOT(67),	12,	18,

	"",			"Your force shield shimmers then fades away.",

	""

    },



    {

	"shocking grasp",	{  10, 101, 1000, 1000, 15, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 2, 99, 99, 99, 99},

	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(53),	15,	12,

	"shocking grasp",	"!Shocking Grasp!",	""

    },



    {

	"sleep",		{ 10, 85, 31, 1000, 1000, 28, 1000, 1000, 1000 },     { 1,  1,  3,  99, 99, 1, 99, 99, 99},

	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,

	&gsn_sleep,		SLOT(38),	15,	12,

	"",			"You feel less tired.",	""

    },



    {

        "slow",                 { 23, 30, 29, 1000, 1000, 32, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 1, 99, 99, 99},

        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        NULL,                   SLOT(515),      30,     12,

        "",                     "You feel yourself speed up.",	""

    },



    {

	"stone skin",		{ 25, 97, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,

	NULL,			SLOT(66),	12,	18,

	"",			"Your skin feels soft again.",	""

    },



    {

	"summon",		{ 24, 28, 31, 42, 33, 29, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 2, 99, 99, 99},

	spell_summon,		TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(40),	50,	12,

	"",			"!Summon!",		""

    },



    {

	"teleport",		{  13, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  99,  99,  99, 99, 99, 99, 99, 99},

	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,

	NULL,	 		SLOT( 2),	35,	12,

	"",			"!Teleport!",		""

    },



    {

	"ventriloquate",	{  1, 1000, 2, 1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  99,  2,  99, 99, 99, 99, 99, 99},

	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,

	NULL,			SLOT(41),	 5,	12,

	"",			"!Ventriloquate!",	""

    },



    {

	"weaken",		{  11, 14, 16, 1000, 1000, 48, 1000, 1000, 1000 },     { 1,  1,  2,  99, 99, 2, 99, 99, 99},

	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(68),	20,	12,

	"spell",		"You feel stronger.",	""

    },



    {

	"word of recall",	{ 32, 28, 1000,  1000, 1000, 1000, 1000, 1000, 1000 },     { 1,  1,  99,  99, 99, 99, 99, 99, 99},

	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,

	NULL,			SLOT(42),	 5,	12,

	"",			"!Word of Recall!",	""

    },

/*Grenades*/
	{
	"flash grenade",		{ 1000, 1000, 18, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 99, 99, 4, 99, 99, 99, 99, 99, 99 },
	gren_flash_grenade,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(515),	0, 	0,
	"",			"!Flash Grenade!",	""
    },
    
    {
	"fire bomb",		{ 1000, 1000, 68, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 99, 99, 6, 99, 99, 99, 99, 99, 99 },
	gren_fire_bomb,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(516),	0, 	0,
	"",			"!Fire Bomb!",	""
    },
/*Grenades*/

   {
	"saber",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 42 }, 	{ 99, 99, 99, 99, 99, 99, 99, 99, 1 },
	ls_lightsaber,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(535),	0, 	0,
	"",			"!Lightsaber!",	""
    },

//Cyborg Programs
    {

	"optical blast",		{ 1000, 1000, 1000, 1000, 35, 1000, 1000, 1000, 1000 }, 	{ 99,  99,  99,  99, 1, 99, 99, 99, 99},
	prog_optical_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(78),	20,	12,
	"optical blast",		"!Optical Blast!"

    },
//Cyborg Programs


// Force Powers
    {
	"push",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 38 }, 	{ 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_push,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(320),	50,	24,
	"Force push",		"!Push!"
    },

    {
	"pull weapon",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 17 }, 	{ 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_pull_weapon,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(321),	35,	24,
	"pull weapon",		"!Pull Weapon!"
    },

    {
	"choke",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 35 }, 	{ 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_choke,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(322),	35,	20,
	"Force choke",		"!Choke!"
    },

    {
	"rage",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 71 }, 	{ 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_rage,	TAR_CHAR_SELF,     POS_STANDING,
	NULL,			SLOT(323),	35,	20,
	"rage",		"!Rage!"
    },

    {
	"inflict pain",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 91 },     { 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_pain,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	NULL,		SLOT(324),	45,	12,
	"inflict pain",		"The pain begins to die away.",
	""
    },

	{
	"drain life",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 174 },     { 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_drain_life,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(325),	175,	20,
	"Force life-drain",		"!Drain Life!",	""
    },

    
    {
	"absorb",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 192 },     { 99,  99,  99,  99, 99, 99, 99, 99, 1},
	force_absorb,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(326),	100,	20,
	"",			"The defense of{W The Force{w fades away.",	""
    },


/* Blast Spells */



    {

	"acid blast",		{ 28, 112, 1000, 1000, 1000, 164, 1000, 1000, 1000 }, 	{ 1,  2,  99,  99, 99, 2, 99, 99, 99},

	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(70),	20,	12,

	"acid blast",		"!Acid Blast!"

    },



    {

	"frost blast",		{ 30, 114, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 1,  2,  99,  99, 99, 99, 99, 99, 99},

	spell_frost_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(112),	20,	12,

	"frost blast",		"!Frost Blast!"

    },





    {

	"fire blast",		{ 32, 116, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 1,  2,  99,  99, 99, 99, 99, 99, 99},

	spell_fire_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(113),	20,	12,

	"fire blast",		"!Fire Blast!"

    },



    {

	"lightning blast",	{ 32, 118, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 1,  2,  99,  99, 99, 99, 99, 99, 99},

	spell_lightning_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(114),	20,	12,

	"lightning blast",		"!Lightning Blast!"

    },



    {

	"mage blast",		{ 34, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 1,  99,  99,  99, 99, 99, 99, 99, 99},

	spell_mage_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(115),	20,	12,

	"mage blast",		"!Mage Blast!"

    },



    {

	"magic bash",		{ 50, 100, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }, 	{ 1,  2,  99,  99, 99, 99, 99, 99, 99},

	spell_magic_bash,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(116),	50,	24,

	"magic bash",		"!Magic Bash!"

    },



/*

 * Dragon breath

 */

    {

	"acid breath",		{ 31, 38, 54, 36, 83, 42, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 2, 99, 99, 99},

	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(200),	100,	20,

	"blast of acid",	"!Acid Breath!",	""

    },



    {

	"fire breath",		{ 40, 49, 69, 48, 78, 50, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 2, 99, 99, 99},

	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(201),	200,	20,

	"blast of flame",	"The smoke leaves your eyes.",	""

    },



    {

	"frost breath",		{ 50, 61, 81, 61, 65, 60, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 2, 99, 99, 99},

	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(202),	125,	20,

	"blast of frost",	"!Frost Breath!",	""

    },



    {

	"gas breath",		{ 60, 72, 93, 76, 54, 70, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 2, 99, 99, 99},

	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,

	NULL,			SLOT(203),	175,	20,

	"blast of gas",		"!Gas Breath!",		""

    },



    {

	"lightning breath",	{ 55, 83, 101, 84, 41, 75, 1000, 1000, 1000 },     { 1,  1,  2,  3, 3, 2, 99, 99, 99},

	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,

	NULL,			SLOT(204),	150,	20,

	"blast of lightning",	"!Lightning Breath!",	""

    },



/*

 * Spells for mega1.are from Glop/Erkenbrand.

 */

    {

        "general purpose",      { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 99, 99, 99, 99, 99, 99, 99 },

        spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        NULL,                   SLOT(401),      0,      12,

        "general purpose ammo", "!General Purpose Ammo!",	""

    },



    {

        "high explosive",       { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 99, 99, 99, 99, 99, 99, 99 },

        spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        NULL,                   SLOT(402),      0,      12,

        "high explosive ammo",  "!High Explosive Ammo!",	""

    },





/* combat and weapons skills */


    {
	"axe",			{ 1000, 1000, 1000,  1, 10, 1, 1, 1000, 1000 },	{ 99, 99, 99, 2, 4, 2, 2, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
        "",                     "!Axe!",		""
    },

    {
        "dagger",               {  1,  1,  1,  1, 1, 1, 1, 1000, 1 },     { 2, 3, 2, 2, 2, 2, 2, 99, 3},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
        "",                     "!Dagger!",		""
    },

    {
	"flail",		{ 1,  1, 1,  1, 1000, 1, 1, 1000, 1000 },	{ 6, 3, 6, 4, 99, 5, 3, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
        "",                     "!Flail!",		""
    },

    {
	"mace",			{ 1,  1,  1,  1, 5, 1, 1, 1000, 1000 },	{ 5, 2, 3, 3, 5, 3, 4, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
        "",                     "!Mace!",		""
    },

    {
	"polearm",		{ 1000, 1000, 1,  1, 25, 15, 1000, 1000, 1000 },	{ 99, 99, 6, 4, 6, 5, 99, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
        "",                     "!Polearm!",		""
    },
	{
	"projectile",		{ 1000, 1000, 1,  1, 1, 1, 1, 1000, 1 },	{ 99, 99, 6, 4, 6, 5, 5, 99, 6},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_projectile,        SLOT( 0),       0,      0,
        "",                     "!Projectile!",		""
    },
   
    {
	"battelh",		{ 1000, 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 99, 99, 99, 99, 99, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_battelh,        SLOT( 0),       0,      0,
        "",                     "!Battelh!",		""
    },

    {
	"lightsaber",		{ 1000, 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1 },	{ 99, 99, 99, 99, 99, 99, 99, 99, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_lightsaber,        SLOT( 0),       0,      0,
        "",                     "!Lightsaber!",		""
    },
    
    {
	"stake",		{ 1000, 1000, 1000,  1000, 1000, 1000, 1, 1000, 1000 },	{ 99, 99, 99, 99, 99, 99, 1, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_stake,        SLOT( 0),       0,      0,
        "stake",                     "!Stake!",		""
    },


    {
	"shield block",		{ 1,  1, 1,  1, 1, 1, 1000, 1000, 1000 },	{ 6, 4, 6, 2, 2, 3, 99, 99, 99},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		""
    },



    {

	"spear",		{  1,  1,  1000,  1, 1000, 1, 1, 1 },	{ 5, 5, 99, 3, 99, 2, 3, 2, 2},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_spear,            	SLOT( 0),       0,      0,

        "",                     "!Spear!",		""

    },



    {

	"sword",		{ 1000, 1000,  1,  1, 1, 1, 1000, 1000, 1000},	{ 99, 99, 0, 2, 3, 4, 99, 99, 99 },

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_sword,            	SLOT( 0),       0,      0,

        "",                     "!sword!",		""

    },



    {

	"whip",			{ 1, 1,  1,  1, 30, 1, 1, 1000, 1000},	{ 6, 5, 5, 4, 6, 5, 1, 99, 99},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_whip,            	SLOT( 0),       0,      0,

        "",                     "!Whip!",	""

    },



    {

        "backstab",             { 1000, 1000,  1, 1000, 1000, 1000, 1000, 1000, 1000 },     { 99, 99, 5, 99, 99, 99, 99, 99, 99},

        spell_null,             TAR_IGNORE,             POS_STANDING,

        &gsn_backstab,          SLOT( 0),        0,     12,

        "backstab",             "!Backstab!",		""

    },



    {
	"bash",			{ 1000, 1000, 1000,  1, 3, 90, 1000, 1000, 1000 },	{ 99, 99, 99, 4, 5, 15, 99, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      12,
        "bash",                 "!Bash!",		""
    },

    {
	"stun",			{ 1000, 1000, 1000,  1000, 1000, 1000, 1000, 41, 1000 },	{ 99, 99, 99, 99, 99, 99, 99, 5, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_stun,            	SLOT( 0),       0,      12,
        "stunning blow",                 "!Stun!",		""
    },

    {
	"power attack",		{ 1000, 1000, 1000, 1000, 1000, 1000, 1000, 54, 1000 },	{ 99, 99, 99, 99, 99, 99, 99, 5, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_power_attack,        	SLOT( 0),       0,      24,
        "",                     "Your attacks become more precise.",	""
    },

    {
	"berserk",		{ 1000, 1000, 1000, 18, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 99, 5, 99, 99, 99, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        "",                     "You feel your pulse slow down.",	""
    },



    {

	"dirt kicking",		{ 1000, 1000,  3,  3, 5, 8, 10, 1000, 1000 },	{ 99, 99, 4, 4, 4, 4, 4, 99, 99},

	spell_null,		TAR_IGNORE,		POS_FIGHTING,

	&gsn_dirt,		SLOT( 0),	0,	24,

	"kicked dirt",		"You rub the dirt out of your eyes.",	""

    },



    {

        "disarm",               { 1000, 1000, 12, 11, 18, 26, 30, 21, 16 },     { 99, 99, 6, 4, 5, 4, 6, 3, 5},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_disarm,            SLOT( 0),        0,     24,

        "",                     "!Disarm!",		""

    },



    {
        "dodge",                { 20, 22,  1, 13, 18, 8, 14, 2, 10 },     { 8, 8, 4, 6, 8, 5, 6, 4, 4},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
        "",                     "!Dodge!",		""
    },



    {

        "enhanced damage",      { 1000, 1000, 1000,  1, 11, 6, 19, 13, 16 },     { 99, 99, 99, 3, 6, 4, 5, 4, 4},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_enhanced_damage,   SLOT( 0),        0,     0,

        "",                     "!Enhanced Damage!",	""

    },



    {

	"envenom",		{ 1000, 1000, 10, 1000, 1000, 14, 23, 1000, 1000 },	{ 99, 99, 4, 99, 99, 6, 5, 99, 99},

	spell_null,		TAR_IGNORE,	  	POS_RESTING,

	&gsn_envenom,		SLOT(0),	0,	36,

	"",			"!Envenom!",		""

    },

    {
        "evasion",                { 1000, 1000,  50, 1000, 1000, 1000, 1000, 30, 38 },     { 99, 99, 5, 99, 99, 99, 99, 4, 4},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_evasion,             SLOT( 0),        0,     0,
        "",                     "!Evasion!",		""
    },

    {
        "improved evasion",       { 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1000, 122 },     { 99, 99, 99, 99, 99, 99, 99, 99, 6},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_improv_evasion,    SLOT( 0),        0,     0,
        "",                     "!Improved Evasion!",		""
    },



    {

	"fury",		{ 1000, 1000, 1000, 112, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 99, 5, 99, 99, 99, 99, 99},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_fury,        	SLOT( 0),       0,      24,

        "",                     "Your hatred and rage eases.",	""

    },



    {
	"destruction",		{ 1000, 1000, 1000, 1000, 86, 1000, 1000, 1000, 1000 },	{ 99, 99, 99, 99, 5, 99, 99, 99, 99},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_destruction,        	SLOT( 0),       0,      24,
        "",                     "{g<CCP>:DESTRUCTION PROGRAM TERMINATED.{w",	""
    },


    {
        "gouge",                 { 1000, 92, 75,  27, 45, 37, 1000, 1000, 1000 },     { 99, 6, 5, 2, 2, 4, 99, 99, 99},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_gouge,              SLOT( 0),        0,     12,
        "gouge to the eye",                 "!Gouge!",		""
    },

    {
        "spinkick",              { 1000, 1000, 1000,  1000, 1000, 1000, 1000, 135, 1000 },     { 99, 99, 99, 99, 99, 99, 1000, 8, 99},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_spinkick,              SLOT( 0),        0,     12,
        "spinkick",                 "!SpinKick!",		""
    },

    {

        "buffet",                 { 1000, 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1000 },     { 99, 99, 99, 99, 99, 99, 99, 99, 99},

        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        &gsn_buffet,              SLOT( 0),        0,     12,

        "buffet",                 "!Buffet!",		""

    },

	{

        "sweep",                 { 1000, 1000, 1000,  1000, 1000, 80, 1000, 1000, 1000 },     { 99, 99, 99, 99, 99, 5, 99, 99, 99},

        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        &gsn_sweep,              SLOT( 0),        0,     12,

        "sweep",                 "!Sweep!",		""

    },

	{

        "poetry",                 { 1000, 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1000 },     { 99, 99, 99, 99, 99, 99, 99, 99, 99},

        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        &gsn_poetry,              SLOT( 0),        0,     12,

        "poetry",                 "!Poetry!",		""

    },

	{

        "sting",                 { 1000, 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1000 },     { 99, 99, 99, 99, 99, 99, 99, 99, 99},

        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        &gsn_sting,              SLOT( 0),        0,     12,

        "sting",                 "!Sting!",		""

    },

    {

        "drain",                 { 1000, 1000, 1000,  1000, 1000, 1000, 1000, 1000, 1000 },     { 99, 99, 99, 99, 99, 99, 99, 99, 99},

        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        &gsn_drain,              SLOT( 0),        0,     12,

        "drain",                 "!Drain!",		""

    },



    {

	"hand to hand",		{ 15,  22, 15, 6, 11, 10, 13, 1, 9 },	{ 4, 4, 3, 2, 2, 2, 3, 1, 2},

	spell_null,		TAR_IGNORE,		POS_FIGHTING,

	&gsn_hand_to_hand,	SLOT( 0),	0,	0,

	"",			"!Hand to Hand!",	""

    },



    {

        "kick",                 { 1000, 12, 14,  8, 21, 26, 31, 3, 13 },     { 99, 4, 6, 3, 4, 5, 4, 3, 4},

        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,

        &gsn_kick,              SLOT( 0),        0,     6,

        "kick",                 "!Kick!",		""

    },



    {

        "parry",                { 1000, 1000, 1000,  1, 1, 1, 1, 1, 1 },     { 99, 99, 99, 4, 4, 4, 4, 4, 4},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_parry,             SLOT( 0),        0,     0,

        "",                     "!Parry!",		""

    },



    {

        "rescue",               { 1000, 1000, 1000,  1, 9, 31, 18, 16, 9 },     { 99, 99, 99, 4, 5, 7, 5, 4, 2},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_rescue,            SLOT( 0),        0,     12,

        "",                     "!Rescue!",		""

    },



    {

	"trip",			{ 1000, 1000,  1, 15, 19, 21, 11, 1, 14 },	{ 99, 99, 4, 8, 4, 6, 4, 3, 5},

	spell_null,		TAR_IGNORE,		POS_FIGHTING,

	&gsn_trip,		SLOT( 0),	0,	24,

	"trip",			"!Trip!",		""

    },



    {

        "second attack",        { 1000, 50, 12,  5, 31, 8, 7, 6, 6 },     { 99, 12, 5, 3, 5, 4, 4, 3, 4},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_second_attack,     SLOT( 0),        0,     0,

        "",                     "!Second Attack!",	""

    },



    {

        "third attack",         { 1000, 100, 24, 12, 100, 27, 19, 15, 17 },     { 99, 12, 6, 4, 6, 5, 5, 3, 4 },

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_third_attack,      SLOT( 0),        0,     0,

        "",                     "!Third Attack!",	""

    },



    {

        "fourth attack",        { 1000, 150, 100, 50, 125, 99, 65, 38, 57 },     { 99, 12, 8, 5, 7, 6, 6, 4, 5},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_fourth_attack,      SLOT( 0),        0,     0,

        "",                     "!Fourth Attack!",	""

    },



    {

        "fifth attack",        { 1000, 1000, 150, 100, 150, 200, 120, 85, 115 },     { 99, 99, 10, 6, 8, 10, 7, 5, 6},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_fifth_attack,      SLOT( 0),        0,     0,

        "",                     "!Fifth Attack!",	""

    },



    {

        "sixth attack",        { 1000, 1000, 250, 150, 200, 300, 168, 135, 160 },     { 99, 99, 12, 6, 8, 12, 8, 5, 7},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_sixth_attack,      SLOT( 0),        0,     0,

        "",                     "!Sixth Attack!",	""

    },



    {

        "seventh attack",        { 1000, 1000, 1000, 200, 250, 1000, 300, 250, 280 },     { 99, 99, 99, 8, 10, 99, 12, 10, 10},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_seventh_attack,      SLOT( 0),        0,     0,

        "",                     "!Seventh Attack!",	""

    },



    {

        "eighth attack",        { 1000, 1000, 1000, 250, 300, 1000, 1000, 275, 310 },     { 99, 99, 99, 8, 10, 99, 14, 10, 10},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_eighth_attack,      SLOT( 0),        0,     0,

        "",                     "!Eighth Attack!",	""

    },



    {

        "ninth attack",        { 1000, 1000, 1000, 300, 350, 1000, 1000, 1000, 1000 },     { 99, 99, 99, 10, 12, 99, 99, 99, 99},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_ninth_attack,      SLOT( 0),        0,     0,

        "",                     "!Ninth Attack!",	""

    },



    {

        "tenth attack",        { 1000, 1000, 1000, 350, 400, 1000, 1000, 1000, 1000 },     { 99, 99, 99, 10, 12, 99, 99, 99, 99},

        spell_null,             TAR_IGNORE,             POS_FIGHTING,

        &gsn_tenth_attack,      SLOT( 0),        0,     0,

        "",                     "!Tenth Attack!",	""

    },



/* non-combat skills */



    {

	"dual wield",		{ 1000, 125, 75, 25, 72, 35, 1000, 1000, 1000 },	{ 99, 12, 7, 5, 10, 8, 99, 99, 99 },

	spell_null,			TAR_IGNORE,			POS_STANDING,

	&gsn_dual_wield,		SLOT( 0),	0,	0,

	"",				"!Dual Wield!",		""

    },



    {

	"fast healing",		{ 15, 9, 16,  6, 18, 3, 12, 4, 5 },	{ 8, 5, 6, 4, 5, 3, 5, 4, 3},

	spell_null,		TAR_IGNORE,		POS_SLEEPING,

	&gsn_fast_healing,	SLOT( 0),	0,	0,

	"",			"!Fast Healing!",	""

    },



    {

	"haggle",		{ 1000, 1000,  1, 1000, 1000, 1000, 1000, 1000, 31 },	{ 99, 99, 3, 99, 99, 99, 99, 99, 5},

	spell_null,		TAR_IGNORE,		POS_RESTING,

	&gsn_haggle,		SLOT( 0),	0,	0,

	"",			"!Haggle!",		""

    },



    {

	"hide",			{ 1000, 1000,  1,  1000, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 4, 99, 99, 99, 99, 99, 99},

	spell_null,		TAR_IGNORE,		POS_RESTING,

	&gsn_hide,		SLOT( 0),	 0,	12,

	"",			"!Hide!",		""

    },



    {

	"lore",			{ 10, 10,  6, 1000, 17, 3, 24, 14, 1000 },	{ 6, 6, 4, 99, 9, 3, 4, 99, 99},

	spell_null,		TAR_IGNORE,		POS_RESTING,

	&gsn_lore,		SLOT( 0),	0,	36,

	"",			"!Lore!",		""

    },



    {

	"meditation",		{  6,  6, 1000, 1000, 1000, 200, 1000, 6, 17 },	{ 5, 5, 99, 99, 99, 10, 99, 3, 5},

	spell_null,		TAR_IGNORE,		POS_SLEEPING,

	&gsn_meditation,	SLOT( 0),	0,	0,

	"",			"Meditation",		""

    },



    {

	"peek",			{  1000, 1000,  1, 1000, 32, 1000, 1000, 1000, 1000 },	{ 99, 99, 3, 99, 8, 99, 99, 99, 99},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_peek,		SLOT( 0),	 0,	 0,

	"",			"!Peek!",		""

    },



    {

	"pick lock",		{ 1000, 1000,  7, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 4, 99, 99, 99, 99, 99, 99},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_pick_lock,		SLOT( 0),	 0,	12,

	"",			"!Pick!",		""

    },



    {

	"sneak",		{ 1000, 1000,  4,  1000, 1000, 72, 32, 1000, 39 },	{ 99, 99, 4, 99, 99, 8, 6, 99, 5},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_sneak,		SLOT( 0),	 0,	12,

	"",			"You no longer feel stealthy.",	""

    },



    {

	"steal",		{ 1000, 1000,  5, 1000, 1000, 1000, 1000, 1000, 1000 },	{ 99, 99, 4, 99, 99, 99, 99, 99, 99},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_steal,		SLOT( 0),	 0,	24,

	"",			"!Steal!",		""

    },



    {

	"scrolls",		{  1,  1,  1,  1, 1, 1, 1, 1, 1 },	{ 2, 3, 5, 8, 7, 7, 4, 4, 4},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_scrolls,		SLOT( 0),	0,	24,

	"",			"!Scrolls!",		""

    },



    {

	"staves",		{  1,  1,  1,  1, 1, 1, 1, 1, 1 },	{ 2, 3, 5, 8, 9, 3, 5, 4, 5},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_staves,		SLOT( 0),	0,	12,

	"",			"!Staves!",		""

    },

    {
	"trance",		{  99,  99,  99,  99, 99, 99, 99, 99, 99 },	{ 99, 99, 99, 99, 99, 99, 99, 99, 4},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_trance,		SLOT( 0),	0,	12,
	"",			"!Trance!",		""
    },

	{

	"grenades",		{  1,  1,  1,  1, 1, 1, 1, 1, 1 },	{ 10, 5, 3, 8, 7, 7, 6, 10, 4},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_toss,		SLOT( 0),	0,	24,

	"",			"!Grenades!",		""

    },

    {

	"wands",		{  1,  1,  1000,  1000, 1000, 1000, 1, 1000, 1000 },	{ 2, 3, 99, 99, 99, 99, 5, 99, 99},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_wands,		SLOT( 0),	0,	12,

	"",			"!Wands!",		""

    },



    {

	"recall",		{  1,  1,  1,  1, 1, 1, 1, 1, 1 },	{ 2, 2, 2, 2, 2, 2, 2, 2, 2},

	spell_null,		TAR_IGNORE,		POS_STANDING,

	&gsn_recall,		SLOT( 0),	0,	12,

	"",			"!Recall!",		""

    }

};

/* --- Grayson Druid --- */





const   struct  group_type      group_table     [MAX_GROUP]     =
{

    {
	"rom basics",		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ "scrolls", "staves", "wands", "recall"  }
    },

    {
	"mage basics",		{ 0, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger" }
    },

    {
	"cleric basics",	{ -1, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ "mace" }
    },

    {
	"thief basics",		{ -1, -1, 0, -1, -1, -1, -1, -1, -1 },
	{ "dagger", "steal" }
    },

    {
	"warrior basics",	{ -1, -1, -1, 0, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },

    {
	"cyborg basics",	{ -1, -1, -1, -1, 0, -1, -1, -1, -1},
	{ "sword", "enhanced damage" }
    },
/* --- Grayson Druid --- */
    {
        "druid basics",        { -1, -1, -1, -1, -1, 0, -1, -1, -1 },
        { "spear", "dodge" }
    },

    {
        "hunter basics",        { -1, -1, -1, -1, -1, -1, 0, -1, -1 },
        { "whip", "dodge", "stake" }
    },

    {
        "monk basics",        { -1, -1, -1, -1, -1, -1, -1, 0, -1 },
        { "spear", "dodge", "hand to hand" }
    },

    {
        "jedi basics",        { -1, -1, -1, -1, -1, -1, -1, -1, 0 },
        { "spear", "lightsaber", "saber" }
    },

    {
	"mage default",		{ 40, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"cleric default",	{ -1, 40, -1, -1, -1, -1, -1, -1, -1 },
	{ "flail", "attack", "creation", "curative",  "benedictions",
	  "detection", "healing", "maladictions", "protective", "shield block",
	  "transportation", "weather" }
    },

    {
	"thief default",	{ -1, -1, 40, -1, -1, -1, -1, -1, -1 },
	{ "mace", "backstab", "disarm", "dodge", "second attack",
	  "trip", "hide", "peek", "pick lock", "sneak" }
    },

    {
	"warrior default",	{ -1, -1, -1, 40, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "shield block", "bash", "disarm", "enhanced damage",
	  "parry", "rescue", "third attack" }
    },

    {
	"cyborg default",	{ -1, -1, -1, -1, 40, -1, -1, -1, -1 },
	{ "disarm", "enhanced damage", "cyborg programs",
	  "parry", "rescue", "second attack" }
    },
 /* --- Grayson Druid --- */
    {
	 "druid default",       { -1, -1, -1, -1, -1, 40, -1, -1, -1 },
	 { "sneak", "enhanced damage", "beguiling", "druid benedictions",
	   "parry", "druid curative", "second attack" }
    },
	
	{
	 "hunter default",       { -1, -1, -1, -1, -1, -1, 40, -1, -1 },
	 { "sneak", "stake", "axe", "staff", "demon hunter spells" }
    },
	
	{
	 "monk default",       { -1, -1, -1, -1, -1, -1, -1, 40, -1 },
	 { "enhanced damage", "parry", "second attack", "disarm", "third attack",
	 	"power attack", "stun", "fourth attack", "fifth attack", "evasion", "spinkick" }
    },

	{
	 "jedi default",       { -1, -1, -1, -1, -1, -1, -1, -1, 40 },
	 { "jedi neutral", "second attack", "third attack", "sneak",
	   "evasion", "improved evasion", "fourth attack", "enhanced damage",
	   "fast healing", "saber" }
    },

    {
	"weaponsmaster",	{ -1, -1, -1, 20, 20, 20, -1, -1, -1 },
	{ "axe", "dagger", "flail", "mace", "polearm", "spear", "sword","whip" }
    },


    {
	"attack",		{ -1, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ "demonfire", "dispel evil", "dispel good", "earthquake",
	  "flamestrike", "heat metal", "ray of truth" }
    },

    {
	"beguiling",		{ 4, -1, 6, -1, -1, 4, -1, -1, -1 },
	{ "calm", "charm person", "sleep" }
    },

    {
	"benedictions",		{ -1, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ "bless", "calm", "frenzy", "holy word", "remove curse",
	  "protection good", "sanctuary" }
    },

    {
	"combat",		{ 6, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ "acid blast", "burning hands", "chain lightning", "chill touch",
	  "colour spray", "fireball", "lightning bolt", "magic missile",
	  "shocking grasp", "fire blast", "frost blast", "lightning blast",
	  "mage blast", "magic bash"  }
    },

    {
	"creation",		{ 4, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ "continual light", "create food", "create spring", "create water",
	  "create rose", "floating disc" }
    },

    {
	"curative",		{ -1, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ "cure blindness", "cure disease", "cure poison" }
    },

    {
	"detection",		{ 4, 3, 6, -1, -1, -1, -1, -1, -1 },
 	{ "detect evil", "detect good", "detect hidden", "detect invis",
	  "detect magic", "detect poison", "farsight", "identify",
	  "know alignment", "locate object", "improved detection" }
    },

    {
	"draconian",		{ -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "acid breath", "fire breath", "frost breath", "gas breath",
	  "lightning breath"  }
    },
/* --- Grayson Druid --- */
    {
        "druid benedictions",       { -1, -1, -1, -1, -1, 5, -1, -1, -1 },
        { "haste", "giant strength", "create spring", "create rose",
	  "detect hidden", "identify", "farsight" }
    },
/* --- Grayson Druid --- */
    {
        "druid curative",       { -1, -1, -1, -1, -1, 5, -1, -1, -1 },
        { "cure serious", "cure poison", "cure critical",
           "cure disease", "cure light", "refresh" }
    },
/* --- Grayson Druid --- */
    {
        "druid maladictions",       { -1, -1, -1, -1, -1, 6, -1, -1, -1 },
	{ "acid blast", "fireball", "entangle", "slow",
	  "burning hands", "weaken", "lightning bolt" }
    },

    {
        "demolitions",       { -1, -1, 12, -1, -1, -1, -1, -1, -1 },
	{ "fire bomb", "flash grenade" }
    },

	{
		"demon hunter spells",  {-1, -1, -1, -1, -1, -1, 8, -1, -1 },
		{ "bless", "detect evil", "dispel evil", "weaken", "protection evil" }	
	},

	{	"jedi light", {-1, -1, -1, -1, -1, -1, -1, -1, 10 },
		{ "charm", "absorb", "protection", "sanctuary", "focus", "calm" }
	},
	
	{	"jedi dark", {-1, -1, -1, -1, -1, -1, -1, -1, 10 },
		{ "lightning", "choke", "rage", "inflict pain", "fear",
		  "drain life"											}
	},
	
	{	"jedi neutral", {-1, -1, -1, -1, -1, -1, -1, -1, 8 },
		{ "haste", "pull weapon", "know alignment", "detect good",
		  "detect evil", "push", "saber"						}
	},
	
	{
		"monk spells",  {-1, -1, -1, -1, -1, -1, -1, 6, -1 },
		{ "detect good", "detect evil", "haste", "cure disease" }	
	},
	
    {
	"enchantment",		{ 6, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "enchant armor", "enchant weapon", "fireproof", "recharge" }
    },

    {
	"enhancement",		{ 5, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "giant strength", "haste", "infravision", "refresh" }
    },

    {
	"harmful",		{ -1, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ "cause critical", "cause light", "cause serious", "harm" }
    },

    {
	"healing",		{ -1, 3, -1, -1, -1, -1, -1, -1, -1 },
 	{ "cure critical", "cure light", "cure serious", "heal",
	  "mass healing", "refresh", "renew" }
    },

    {
	"illusion",		{ 4, -1, 7, -1, -1, -1, -1, -1, -1 },
	{ "invis", "mass invis", "ventriloquate" , "improved invisibility" }
    },

    {
	"maladictions",		{ 5, 8, 9, -1, -1, -1, -1, -1, -1 },
	{ "blindness", "change sex", "curse", "energy drain", "plague",
	  "poison", "slow", "weaken" }
    },

    {
	"protective",		{ 4, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ "armor", "cancellation", "dispel magic", "fireproof",
	  "protection evil", "protection good", "sanctuary", "shield",
	  "stone skin" }
    },

    {
	"transportation",	{ 4, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ "fly", "gate", "nexus", "pass door", "portal", "summon", "teleport",
	  "word of recall" }
    },

    {
	"cyborg programs",		{ -1, -1, -1, -1, 20, -1, -1, -1, -1 },
	{ "chain lightning", "detect hidden", "detect invis", "farsight",
	  "fly", "gate", "giant strength", "haste", "heat metal",
	  "identify", "infravision", "locate object", "shocking grasp",
	  "optical blast"
	}
    },

    {
	"weather",		{ 4, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ "call lightning", "control weather", "faerie fire", "faerie fog",
	  "lightning bolt" }
    }

};
