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
#include "tables.h"


/* for clans */

struct clan_type clan_table[MAX_CLAN] =
{
 /*   name,			who entry,					death-transfer room,	ind,		active,	PKSAFE 	Visible	*/
    { "\0",			"\0",						ROOM_VNUM_ALTAR,		TRUE,		FALSE,	TRUE,		TRUE	},
    { "loner", 		"{D-<L{wo{Wn{we{DR>-{w", 		ROOM_VNUM_ALTAR, 		TRUE, 	TRUE,		FALSE, 	TRUE 	},
    {	"emptyclan1",	"EmptyClan1",				ROOM_VNUM_ALTAR,		TRUE,		FALSE,	TRUE,		TRUE 	},
    {	"emptyclan2",	"EmptyClan2",				ROOM_VNUM_ALTAR,		TRUE,		FALSE,	TRUE,		TRUE 	},
    {	"emptyclan3",	"EmptyClan3",				ROOM_VNUM_ALTAR,		TRUE,		FALSE,	TRUE,		TRUE 	},
    {	NULL,			NULL,						ROOM_VNUM_ALTAR,		TRUE,		FALSE,	TRUE,		TRUE  }
};
                                                                       


/* for Tips */
const struct tip_type tip_table[MAX_TIPS] =
{
 { 
	"Use the helpers. They have a wealth of information to share with you.",
	"You're at a hard time leveling wise. Stay with it and don't worry about how long it takes to gain a level.", 
	"Explore the mud! Magus Castle and the Stone Covered Field aren't the only places to level during this time.", 
	"Leveling will be easier this time around, so use your knowledge and level.", 
	"Apply to be a Helper and share your knowledge with the newbie's.", 
	"Explore the mud as much as possible. Learn where everything is and what it does." 
 },
 { 
	"Try to see if a command has a help file before asking about it.", 
	"Join a clan if you haven't already. It is a wonderful way to meet new friends and share interests.", 
	"Get ready to remort and have some fun!", 
	"Don't bug IMMs to get reclanned. Have your Leader, GM, or Trainer send a note and it will get done.", 
	"Begin exploring Magrathea. This is a massive place, who knows what you might find.", 
	"Help your clan with gold and your knowledge" 
 },
 { 
	"The 'quit' command is the best way to exit the mud.", 
	"Explore new areas. There are other places besides hell to level during this time.", 
	"The choices you make on skills and spells here will affect you for a long time.", 
	"Don't bug IMMs period. You are a remort and should know the rules.", 
	"You should be a skilled player by now, act like it.", 
	"Apply to be a Helper and share your knowledge with the newbie's."
 },
 { 
	"If you have trouble playing, ask around for guidance.", 
	"Begin to develop your Role Play and practice it.", 
	"Your Role Play skills should be developed by now, use them.", 
	"This time through, Role Play as well as Level.", 
	"Keep your record clean. Not doing so could hurt if you plan on being a Helper or Builder.", 
	"You are among the elite of the mud, show your maturity, the Imms will accept no less."
 },
 {
	"HAVE FUN!",
	"Keep nosummon ON",
	"Experiment with different kinds of armor",
	"Find the Remort race that best suits you",
	"Never enter battle without Fury or Destruction",
	"Watch the arena's!",
 },
 {
	"Learn how to use the channels",
	"Explore, walk, try to walk even AFTER you get gate",
	"Refine your skills",
	"Try different mobs this time around",
	"Consider being a Builder",
	"Explore all those areas you never did before",
 },
 {
	"Turn on your auto's (and know what each one is for)",
	"Consider joining a clan",
	"Make sure you've mastered the basics of mudding",
	"Experiment with a new RP",
	"Ever wanted to be a helper?",
	"Master your Role Play",
 },
 {
	"Learn how and when to use your spells",
	"Beware of arena's",
	"EXPLORE!",
	"Time to go recollect those items people were holding while you remorted",
	"If you are in a clan, begin to show leadership within it",
	"Consider starting a clan",
 },
 {
	"Use the Newbie Kits and the equipment they have",
	"If you wish to PKill, begin to learn now",
	"Refine and practice your Role Playing Skills",
	"Your remort bonuses make it easier for you to kill harder mobs",
	"Role Play!",
	"Builders are needed!",
 },
/*
 {
	"",
	"",
	"",
	"",
	"",
	"",
 },
*/
 { NULL, NULL, NULL, NULL, NULL, NULL }
};


/* for the global economy and other global settings */
struct economy_type economy_table[MAX_ECON] =
{
 { "ghost_nocorpse_cash",	"New Body Cost without Corpse in Gold",		1000	},
 { "ghost_pccorpse_cash",	"New Body Cost with PC Corpse in Gold",		1000	},
 { "ghost_npccorpse_cash",	"New Body Cost with NPC Corpse in Gold",		5000	},
 { "ghost_healerrate_cash",	"Multiplier for Healer using bank",			2	},
 { "ghost_nocorpse_mana",	"New Body Cost without Corpse in Mana",		500	},
 { "ghost_pccorpse_mana",	"New Body Cost with PC Corpse in Mana",		150	},
 { "ghost_npccorpse_mana",	"New Body Cost with NPC Corpse in Mana",		250	},
 { "ghost_min_mana",		"Minimum Mana Cost for Reanimate",			100	},
 { "ghost_min_cash",		"Minimum Gold Cost for Reanimate",			500	},
 { "bank_wire_fee_percent",	"Bank Wire Transfer Fee Percentage",		50	},
 { "dblock_report",		"Consecutive DB Lockouts to report",		10	},
 { "spool_lock_report",		"Consecutive Spool Lockouts to report",		10	},
 { "m_sacrifice",			"Multiplyer applied to corpe sac",			1	},
 { "d_sacrifice",			"Divisor applied to sacrificing corpses",		2	},
 { "m_sell",			"Multiplyer applied to selling",			1	},
 { "d_sell",			"Divisor applied to selling of items",		2	},
 { "m_room_heal",			"Multiplyer to Room Heal Rates",			1	},
 { "d_room_heal",			"Divisor to Room Heal Rates",				1	},
 { "m_room_mana",			"Multiplyer to Room Mana Rates",			1	},
 { "d_room_mana",			"Divisor to Room Mana Rates",				1	},
 { "global_change",		"Time until global values changed",			5760	},
 { "wiz_time_count",		"Number of Ticks for Wiznet Time",			36	},
 { "healer_percentage",		"Percentage per Level surcharge",			10	},
 { "healer_light",		"Healer: cure light wounds",				10	},
 { "healer_serious",		"Healer: cure serious wounds",			15	},
 { "healer_critical",		"Healer: cure critical wounds",			25	},
 { "healer_renew",		"Healer: renew (partial restore)",			500	},
 { "healer_blind",		"Healer: cure blindness",				20	},
 { "healer_disease",		"Healer: cure disease",					15	},
 { "healer_poison",		"Healer: cure poison",					25	},
 { "healer_uncurse",		"Healer: remove curse",					50	},
 { "healer_refresh",		"Healer: restore movement",				5	},
 { "healer_mana",			"Healer: restore mana",					10	},
 { "healer_restore",		"Healer: full restore",					1000	},
 { "cyber_cost_proc",		"Cybernetics: processor socket",			100000	},
 { "cyber_cost_optic",		"Cybernetics: optical implant",			    100000	},
 { "cyber_cost_leg",		"Cybernetics: leg implant",			        100000	},
 { "cyber_cost_chest",		"Cybernetics: chest implant",			    100000	},
 { "cyber_cost_reflex",		"Cybernetics: reflex implant",			    100000	},
 { "cyber_cost_str",		"Cybernetics: strength implant",			100000	},
 { "cyber_cost_comm",		"Cybernetics: comm socket",			        100000	},
 { "race_skill_cost",		"Cost to Practice Race Skills",			3	},
 { "spam_count",			"Input number before spamming",			50	},
 { "auto_punish",			"Timer for automatic punishments",			5	},
 { "max_punish",			"Maximum timer for punishments",			1440	},
 { "fileload_clan",		"Load and Save Clan Table to File",			10	},
 { "fileload_econ",		"Load and Save Economy Table to File",		10	},
 { "fileload_curse",		"Load and Save Curse Table to File",		10	},
 { "fileload_combat",            "Load and Save Combat Table to File",            10      },
/*
 { "",				"",								0	},
*/
 { NULL, NULL, 0 }
};

/* global table for combat modifiers */
struct combatmod_type combatmod_table[MAX_COMBAT] =
{
 { "m_base_xp",			"Multiplyer applied to ALL XP",			2	},
 { "d_base_xp",			"Divisor applied to ALL XP Calculations",		3	},
 { "m_remort_xp",			"Multiplyer applied to Remort XP",			1	},
 { "d_remort_xp",			"Divisor applied to Remort XP",			1	},
 { "m_group_xp",			"Multiplyer applied to Group XP",			1	},
 { "d_group_xp",			"Divisor applied to Group XP",			1	},
 { "m_damage_pc_pc",		"Multiplyer Damage PC to PC",				1	},
 { "d_damage_pc_pc",		"Divisor Damage PC to PC",				8	},
 { "m_damage_pc_npc",		"Multiplyer Damage PC to NPC",			1	},
 { "d_damage_pc_npc",		"Divisor Damage PC to NPC",				2	},
 { "m_damage_npc_pc",		"Multiplyer Damage NPC to PC",			1	},
 { "d_damage_npc_pc",		"Divisor Damage NPC to PC",				1	},
 { "lag_bash_ch",			"Multiplyer for Attacker Bash Lag",			1	},
 { "lag_bash_victim",		"Multiplyer for Victim Bash Lag",			2	},
 { "lag_spinkick_ch",		"Multiplyer for Attacker Spinkick Lag",		1	},
 { "lag_spinkick_victim",	"Multiplyer for Victim Spinkick Lag",		4	},
 { "lag_trip_ch",			"Multiplyer for Attacker Trip Lag",			1	},
 { "lag_trip_victim",		"Multiplyer for Victim Trip Lag",			2	},
 { "lag_buffet_ch",		"Multiplyer for Attacker Buffet Lag",		1	},
 { "lag_buffet_victim",	"Multiplyer for Victim  Buffet Lag",		2	},
 { "lag_sweep_ch",		"Multiplyer for Attacker Sweep Lag",		1	},
 { "lag_sweep_victim",	"Multiplyer for Victim Sweep Lag",			2	}, 
 { "lag_sting_ch",		"Multiplyer for Attacker Sting Lag",		1	},
 { "lag_sting_victim",	"Multiplyer for Victim Sting Lag",			2	},
 { "lag_poetry_ch",		"Multiplyer for Attacker Poetry Lag",		1	},
 { "lag_poetry_victim",	"Multiplyer for Victim Poetry Lag",			1	},
 { "lag_drain_ch",		"Multiplyer for Attacker Drain Lag",		0	},
 { "lag_drain_victim",	"Multiplyer for Victim Drain Lag",			0	},
 { "m_damage_bash",		"Multiplyer for Bash Damage",				1	},
 { "d_damage_bash",		"Divisor for Bash Damage",					1	},
 { "m_damage_spinkick",		"Multiplyer for Spinkick Damage",		3	},
 { "d_damage_spinkick",		"Divisor for Spinkick Damage",			2	},
 { "m_damage_trip",		"Multiplyer for Trip Damage",				1	},
 { "d_damage_trip",			"Divisor for Trip Damage",				1	},
 { "m_damage_stab",		"Multiplyer for Back/Circle Stab",			1	},
 { "d_damage_stab",			"Divisor for Back/Circle Stab",			4	},
 { "m_damage_buffet",		"Multiplyer for Buffet Damage",			3	},
 { "d_damage_buffet",		"Divisor for Buffet Damage",			2	},
 { "m_damage_sweep",		"Multiplyer for Sweep Damage",			3	},
 { "d_damage_sweep",		"Divisor for Sweep Damage",				2	},
 { "m_damage_sting",		"Multiplyer for Sting Damage",			3	},
 { "d_damage_sting",		"Divisor for Sting Damage",				2	},
 { "m_damage_poetry",		"Multiplyer for Poetry Damage",			3	},
 { "d_damage_poetry",		"Divisor for Poetry Damage",			2	},
 { "m_damage_drain",		"Multiplyer for Drain Damage",			3	},
 { "d_damage_drain",		"Divisor for Drain Damage",				2	},
 { "m_ac",					"Multiplier for Armor Class",			1	},
 { "d_ac",					"Divisor for Armor Class",				4	},
 { "gouge_time_thief",		"Gouge Blindness if a Thief",			3	},
 { "gouge_time_other",		"Gouge Blindness if not Thief",			1	},
 { "suicide_exp",			"Base XP cost for a Suicide",				1000	},
 { "suicide_exp_limit",		"Limit on EXP to Level for Suicide",		1500	}, 
 { "m_dmfb_dam",		"Mult for Dragon Mage Fire Breath Damage",				1	}, 
 { "d_dmfb_dam",		"Div for Dragon Mage Fire Breath Damage",				1	}, 
 { "m_dwfb_dam",		"Mult for Dragon Warrior Fire Breath Damage",				1	}, 
 { "d_dwfb_dam",		"Div for Dragon Warrior Fire Breath Damage",				1	}, 
 { "m_dtfb_dam",		"Mult for Dragon Thief Fire Breath Damage",				1	}, 
 { "d_dtfb_dam",		"Div for Dragon Thief Fire Breath Damage",				1	}, 
 { "m_dcfb_dam",		"Mult for Dragon Cleric Fire Breath Damage",				1	}, 
 { "d_dcfb_dam",		"Div for Dragon Cleric Fire Breath Damage",				1	}, 
 { "m_ddfb_dam",		"Mult for Dragon Druid Fire Breath Damage",				1	}, 
 { "d_ddfb_dam",		"Div for Dragon Druid Fire Breath Damage",				1	}, 
 { "m_dmab_dam",		"Mult for Dragon Mage Acid Breath Damage",				1	}, 
 { "d_dmab_dam",		"Div for Dragon Mage Acid Breath Damage",				1	}, 
 { "m_dwab_dam",		"Mult for Dragon Warrior Acid Breath Damage",				1	}, 
 { "d_dwab_dam",		"Div for Dragon Warrior Acid Breath Damage",				1	}, 
 { "m_dtab_dam",		"Mult for Dragon Thief Acid Breath Damage",				1	}, 
 { "d_dtab_dam",		"Div for Dragon Thief Acid Breath Damage",				1	}, 
 { "m_dcab_dam",		"Mult for Dragon Cleric Acid Breath Damage",				1	}, 
 { "d_dcab_dam",		"Div for Dragon Cleric Acid Breath Damage",				1	}, 
 { "m_ddab_dam",		"Mult for Dragon Druid Acid Breath Damage",				1	}, 
 { "d_ddab_dam",		"Div for Dragon Druid Acid Breath Damage",				1	}, 
 { "m_dmfrb_dam",		"Mult for Dragon Mage Frost Breath Damage",				1	}, 
 { "d_dmfrb_dam",		"Div for Dragon Mage Frost Breath Damage",				1	}, 
 { "m_dwfrb_dam",		"Mult for Dragon Warrior Frost Breath Damage",				1	}, 
 { "d_dwfrb_dam",		"Div for Dragon Warrior Frost Breath Damage",				1	}, 
 { "m_dtfrb_dam",		"Mult for Dragon Thief Frost Breath Damage",				1	}, 
 { "d_dtfrb_dam",		"Div for Dragon Thief Frost Breath Damage",				1	}, 
 { "m_dcfrb_dam",		"Mult for Dragon Cleric Frost Breath Damage",				1	}, 
 { "d_dcfrb_dam",		"Div for Dragon Cleric Frost Breath Damage",				1	}, 
 { "m_ddfrb_dam",		"Mult for Dragon Druid Frost Breath Damage",				1	}, 
 { "d_ddfrb_dam",		"Div for Dragon Druid Frost Breath Damage",				1	}, 
 { "m_dmgb_dam",		"Mult for Dragon Mage Gas Breath Damage",				1	}, 
 { "d_dmgb_dam",		"Div for Dragon Mage Gas Breath Damage",				1	}, 
 { "m_dwgb_dam",		"Mult for Dragon Warrior Gas Breath Damage",				1	}, 
 { "d_dwgb_dam",		"Div for Dragon Warrior Gas Breath Damage",				1	}, 
 { "m_dtgb_dam",		"Mult for Dragon Thief Gas Breath Damage",				1	}, 
 { "d_dtgb_dam",		"Div for Dragon Thief Gas Breath Damage",				1	}, 
 { "m_dcgb_dam",		"Mult for Dragon Cleric Gas Breath Damage",				1	}, 
 { "d_dcgb_dam",		"Div for Dragon Cleric Gas Breath Damage",				1	}, 
 { "m_ddgb_dam",		"Mult for Dragon Druid Gas Breath Damage",				1	}, 
 { "d_ddgb_dam",		"Div for Dragon Druid Gas Breath Damage",				1	}, 
 { "m_dmlb_dam",		"Mult for Dragon Mage Lightning Breath Damage",				1	}, 
 { "d_dmlb_dam",		"Div for Dragon Mage Lightning Breath Damage",				1	}, 
 { "m_dwlb_dam",		"Mult for Dragon Warrior Lightning Breath Damage",			1	}, 
 { "d_dwlb_dam",		"Div for Dragon Warrior Lightning Breath Damage",			1	}, 
 { "m_dtlb_dam",		"Mult for Dragon Thief Lightning Breath Damage",			1	}, 
 { "d_dtlb_dam",		"Div for Dragon Thief Lightning Breath Damage",				1	}, 
 { "m_dclb_dam",		"Mult for Dragon Cleric Lightning Breath Damage",			1	}, 
 { "d_dclb_dam",		"Div for Dragon Cleric Lightning Breath Damage",			1	}, 
 { "m_ddlb_dam",		"Mult for Dragon Druid Lightning Breath Damage",			1	}, 
 { "d_ddlb_dam",		"Div for Dragon Druid Lightning Breath Damage",				1	},  
 { "m_damage_mage",		"Multiplyer for Mage Damage",				1	},
 { "d_damage_mage",		"Divisor for Mage Damage",				1	},
 { "m_damage_warrior",		"Multiplyer for Warrior Damage",				1	},
 { "d_damage_warrior",		"Divisor for Warrior Damage",				1	},
 { "m_damage_cleric",		"Multiplyer for Cleric Damage",				1	},
 { "d_damage_cleric",		"Divisor for Cleric Damage",				1	},
 { "m_damage_thief",		"Multiplyer for Thief Damage",				1	},
 { "d_damage_thief",		"Divisor for Thief Damage",				1	},
 { "m_damage_cyborg",		"Multiplyer for Cyborg Damage",				1	},
 { "d_damage_cyborg",		"Divisor for Cyborg Damage",				1	},
 { "m_damage_druid",		"Multiplyer for Druid Damage",				1	},
 { "d_damage_druid",		"Divisor for Druid Damage",				1	},
 { "m_damage_stab",		"Multiplyer for Back/Circle Stab",			1	},
 { "d_damage_stab",		"Divisor for Back/Circle Stab",			4	},
 { "gouge_time_thief",		"Gouge Blindness if a Thief",				3	},
 { "gouge_time_other",		"Gouge Blindness if not Thief",			1	}, 
/*
 { "",				"",								0	},
*/
 { NULL, NULL, 0 }
};

struct censor_type censor_table[MAX_CENSOR]=
{
	{ "fuck"	},
	{ NULL }
};

struct insult_type insult_table[MAX_INSULTS]=
{
	{ "what are you looking at you one eyed cockroach."	},
	{ "shut your mouth if I wanted to hear from an ass, I'd fart."	},
	{ "you really are a moron aren't you?"	},
	{ "is a coward and a fool!"	},
	{ "you have a face like a saint, a saint bernard." },
	{ "don't you need a license to be that ugly?" },
	{ "do you like nature, in spite of what it did to you?" },
	{ "everyone has the right to be ugly, but you abused it." },
	{ "I've seen people like you before, but I had to pay admission" },
	{ "are you always this stupid or are you making a special effort today?" },
	{ "are your parents siblings?" },
	{ "do you ever wonder what life would be like if you'd had enough oxygen at birth?" },
	{ "I bet your mother has a loud bark!" },
	{ "I'd like to leave you with one thought, but I don't think you have anywhere to put it." },
	{ "if your brain was chocolate it wouldn't fill an M&M." },
	{ "some day you will find yourself - and wish you hadn't." },
	{ "there is no vaccine against stupidity." },
	{ "you've such a striking face. So tell me, what was it that hit you?" },
	{ "boy if my dog were that ugly I would shave his ass and make him walk backwards." },
	{ "nice face...want a gun?" },
	{ "what are you going to do for a face when the babboon wants his ass back?" },
	{ "when it comes to I.Q. points, you lose them every time you go to the bathroom." },
	{ "no, a polygon is not a dead parrot." },
	{ "you remind me of opium, a slow working dope." },
	{ "you're so useless you couldn't pour water out of a boot if the instructions were written on the bottom of the heel." },
	{ "if I want any crap from you I'd squeeze your head." },
	{ "dont be ignorant all your life, take a day off why dont you?" },
	{ "you don't need to use an insult, you just use your breath." },
	{ "you're incompetence is an inspiration to morons everywhere." },
	{ "I'd smack the crap out of you if I didn't think it would fill up the room" },
	{ "Your mother was a hamster and your father smelt of elderberries!" },
	{ NULL }
};



/* for position */
const struct position_type position_table[] =
{
    {	"dead",			"dead"	},
    {	"mortally wounded",	"mort"	},
    {	"incapacitated",		"incap"	},
    {	"stunned",			"stun"	},
    {	"sleeping",			"sleep"	},
    {	"resting",			"rest"	},
    { "sitting",			"sit"   	},
    {	"fighting",			"fight"	},
    {	"standing",			"stand"	},
    {	NULL,				NULL	}
};

/* for sex */
const struct sex_type sex_table[] =
{
   {	"none"		},
   {	"male"		},
   {	"female"		},
   {	"either"		},
   {	NULL			}
};

/* for sizes */
const struct size_type size_table[] =
{ 
    {	"tiny"		},
    {	"small" 		},
    {	"medium"		},
    {	"large"		},
    {	"huge", 		},
    {	"giant" 		},
    {	NULL			}
};

/* various flag tables */
const struct flag_type act_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"sentinel",			B,	TRUE	},
    {	"scavenger",		C,	TRUE	},
    {	"aggressive",		F,	TRUE	},
    {	"stay_area",		G,	TRUE	},
    {	"wimpy",			H,	TRUE	},
    {	"pet",			I,	TRUE	},
    {	"train",			J,	TRUE	},
    {	"practice",			K,	TRUE	},
    {	"undead",			O,	TRUE	},
    {	"cleric",			Q,	TRUE	},
    {	"mage",			R,	TRUE	},
    {	"thief",			S,	TRUE	},
    {	"warrior",			T,	TRUE	},
    {	"noalign",			U,	TRUE	},
    {	"nopurge",			V,	TRUE	},
    {	"outdoors",			W,	TRUE	},
    {	"indoors",			Y,	TRUE	},
    {	"healer",			aa,	TRUE	},
    {	"gain",			bb,	TRUE	},
    {	"update_always",		cc,	TRUE	},
    {	"changer",			dd,	TRUE	},
    { "banker",			ee,	TRUE  },
    { "engraver",			ff,	TRUE	},
    {	NULL,				0,	FALSE	}
};

const struct flag_type plr_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"autoassist",		C,	TRUE	},
    {	"autoexit",			D,	TRUE	},
    {	"autoloot",			E,	TRUE	},
    {	"autosac",			F,	TRUE	},
    {	"autogold",			G,	TRUE	},
    {	"autosplit",		H,	TRUE	},
    { "autowizi",			I,	TRUE	},
    {	"holylight",		N,	TRUE	},
    {	"can_loot",			P,	TRUE	},
    {	"nosummon",			Q,	TRUE	},
    {	"nofollow",			R,	TRUE	},
    {	"colour",			T,	TRUE	},
    {	"permit",			U,	TRUE	},
    {	"log",			W,	TRUE	},
    {	"deny",			X,	TRUE	},
    {	"freeze",			Y,	TRUE	},
    {	"thief",			Z,	TRUE	},
    {	"killer",			aa,	TRUE	},
    { "helper",			bb,	TRUE	},
    { "wanted",			cc,	TRUE	},
    { "remort",			dd,	TRUE	},
    { "clan_leader",		ff,	TRUE	},
    {	NULL,				0,	0	}
};

const struct flag_type olcs_flags[] =

{

    { "room",		A,	TRUE	},

    { "mob",		B,	TRUE	},

    { "obj",		C,	TRUE	},

    { "reset",		D,	TRUE	},

    { "admin",		E,	TRUE	},

    { "builder",		F,	TRUE	},

    { "clanadmin",	G,	TRUE	},

    { "inspector",	H,	TRUE	},

    { "coder",		I,	TRUE  },		/* Storm */

    {	NULL,			0,	0	}

};



const struct flag_type affect_flags[] =

{

    {	"blind",			A,	TRUE	},

    {	"invisible",		B,	TRUE	},

    {	"detect_evil",		C,	TRUE	},

    {	"detect_invis",		D,	TRUE	},

    {	"detect_magic",		E,	TRUE	},

    {	"detect_hidden",		F,	TRUE	},

    {	"detect_good",		G,	TRUE	},

    {	"sanctuary",		H,	TRUE	},

    {	"faerie_fire",		I,	TRUE	},

    {	"infrared",			J,	TRUE	},

    {	"curse",			K,	TRUE	},

    {	"poison",			M,	TRUE	},

    {	"protect_evil",		N,	TRUE	},

    {	"protect_good",		O,	TRUE	},

    {	"sneak",			P,	TRUE	},

    {	"hide",			Q,	TRUE	},

    {	"sleep",			R,	TRUE	},

    {	"charm",			S,	TRUE	},

    {	"flying",			T,	TRUE	},

    {	"pass_door",		U,	TRUE	},

    {	"haste",			V,	TRUE	},

    {	"calm",			W,	TRUE	},

    {	"plague",			X,	TRUE	},

    {	"weaken",			Y,	TRUE	},

    {	"dark_vision",		Z,	TRUE	},

    {	"berserk",			aa,	TRUE	},

    {	"swim",			bb,	TRUE	},

    {	"regeneration",		cc,	TRUE	},

    {	"slow",			dd,	TRUE	},

    {	"fury",			ee,	TRUE	},

    {	"destruction",		ff,	TRUE	},

    {	NULL,				0,	0	}

};



const struct flag_type off_flags[] =

{

    {	"area_attack",		A,	TRUE	},

    {	"backstab",			B,	TRUE	},

    {	"bash",			C,	TRUE	},

    {	"berserk",			D,	TRUE	},

    {	"disarm",			E,	TRUE	},

    {	"dodge",			F,	TRUE	},

    {	"fade",			G,	TRUE	},

    {	"fast",			H,	TRUE	},

    {	"kick",			I,	TRUE	},

    {	"dirt_kick",		J,	TRUE	},

    {	"parry",			K,	TRUE	},

    {	"rescue",			L,	TRUE	},

    {	"tail",			M,	TRUE	},

    {	"trip",			N,	TRUE	},

    {	"crush",			O,	TRUE	},

    {	"assist_all",		P,	TRUE	},

    {	"assist_align",		Q,	TRUE	},

    {	"assist_race",		R,	TRUE	},

    {	"assist_players",		S,	TRUE	},

    {	"assist_guard",		T,	TRUE	},

    {	"assist_vnum",		U,	TRUE	},

    {	"fury",			V,	TRUE	},

    {	"spinkick",			W,	TRUE	},

    {	"gouge",			X,	TRUE	},

    {	"destruction",		Y,	TRUE	},

    {	NULL,				0,	0	}

};



const struct flag_type imm_flags[] =

{

    {	"summon",		A,	TRUE	},

    {	"charm",		B,	TRUE	},

    {	"magic",		C,	TRUE	},

    {	"weapon",		D,	TRUE	},

    {	"bash",		E,	TRUE	},

    {	"pierce",		F,	TRUE	},

    {	"slash",		G,	TRUE	},

    {	"fire",		H,	TRUE	},

    {	"cold",		I,	TRUE	},

    {	"lightning",	J,	TRUE	},

    {	"acid",		K,	TRUE	},

    {	"poison",		L,	TRUE	},

    {	"negative",		M,	TRUE	},

    {	"holy",		N,	TRUE	},

    {	"energy",		O,	TRUE	},

    {	"mental",		P,	TRUE	},

    {	"disease",		Q,	TRUE	},

    {	"drowning",		R,	TRUE	},

    {	"light",		S,	TRUE	},

    {	"sound",		T,	TRUE	},

    {	"wood",		X,	TRUE	},

    {	"silver",		Y,	TRUE	},

    {	"iron",		Z,	TRUE	},

    {	"quests",		aa,	TRUE	},

    {	NULL,			0,	0	}

};



const struct flag_type form_flags[] =

{

    {	"edible",		FORM_EDIBLE,		TRUE	},

    {	"poison",		FORM_POISON,		TRUE	},

    {	"magical",		FORM_MAGICAL,		TRUE	},

    {	"instant_decay",	FORM_INSTANT_DECAY,	TRUE	},

    {	"other",		FORM_OTHER,			TRUE	},

    {	"animal",		FORM_ANIMAL,		TRUE	},

    {	"sentient",		FORM_SENTIENT,		TRUE	},

    {	"undead",		FORM_UNDEAD,		TRUE	},

    {	"construct",	FORM_CONSTRUCT,		TRUE	},

    {	"mist",		FORM_MIST,			TRUE	},

    {	"intangible",	FORM_INTANGIBLE,		TRUE	},

    {	"biped",		FORM_BIPED,			TRUE	},

    {	"centaur",		FORM_CENTAUR,		TRUE	},

    {	"insect",		FORM_INSECT,		TRUE	},

    {	"spider",		FORM_SPIDER,		TRUE	},

    {	"crustacean",	FORM_CRUSTACEAN,		TRUE	},

    {	"worm",		FORM_WORM,			TRUE	},

    {	"blob",		FORM_BLOB,			TRUE	},

    {	"mammal",		FORM_MAMMAL,		TRUE	},

    {	"bird",		FORM_BIRD,			TRUE	},

    {	"reptile",		FORM_REPTILE,		TRUE	},

    {	"snake",		FORM_SNAKE,			TRUE	},

    {	"dragon",		FORM_DRAGON,		TRUE	},

    {	"amphibian",	FORM_AMPHIBIAN,		TRUE	},

    {	"fish",		FORM_FISH ,			TRUE	},

    {	"cold_blood",	FORM_COLD_BLOOD,		TRUE	},

    {	NULL,			0,			0	}

};



const struct flag_type part_flags[] =

{

    {	"head",		PART_HEAD,		TRUE	},

    {	"arms",		PART_ARMS,		TRUE	},

    {	"legs",		PART_LEGS,		TRUE	},

    {	"heart",		PART_HEART,		TRUE	},

    {	"brains",		PART_BRAINS,	TRUE	},

    {	"guts",		PART_GUTS,		TRUE	},

    {	"hands",		PART_HANDS,		TRUE	},

    {	"feet",		PART_FEET,		TRUE	},

    {	"fingers",		PART_FINGERS,	TRUE	},

    {	"ear",		PART_EAR,		TRUE	},

    {	"eye",		PART_EYE,		TRUE	},

    {	"long_tongue",	PART_LONG_TONGUE,	TRUE	},

    {	"eyestalks",	PART_EYESTALKS,	TRUE	},

    {	"tentacles",	PART_TENTACLES,	TRUE	},

    {	"fins",		PART_FINS,		TRUE	},

    {	"wings",		PART_WINGS,		TRUE	},

    {	"tail",		PART_TAIL,		TRUE	},

    {	"claws",		PART_CLAWS,		TRUE	},

    {	"fangs",		PART_FANGS,		TRUE	},

    {	"horns",		PART_HORNS,		TRUE	},

    {	"scales",		PART_SCALES,	TRUE	},

    {	"tusks",		PART_TUSKS,		TRUE	},

    {	NULL,			0,			0	}

};



const struct flag_type comm_flags[] =

{

    {	  "quiet",			COMM_QUIET,			TRUE	},

    {   "deaf",			COMM_DEAF,			TRUE	},

    {   "nowiz",			COMM_NOWIZ,			TRUE	},

    {   "nosup",			COMM_NOSUP,			TRUE	},

    {   "nobuilder",		COMM_NOBUILDER,		TRUE  },

    {   "noclangossip",		COMM_NOAUCTION,		TRUE	},

    {   "noflame",		COMM_NOFLAME,		TRUE	},

    {   "noooc",			COMM_NOOOC,			TRUE	},

    {   "nogossip",		COMM_NOGOSSIP,		TRUE	},

    {   "noquestion",		COMM_NOQUESTION,		TRUE	},

    {   "nomusic",		COMM_NOMUSIC,		TRUE	},

    {   "noclan",			COMM_NOCLAN,		TRUE	},

    {   "noquote",		COMM_NOQUOTE,		TRUE	},

    {   "shoutsoff",		COMM_SHOUTSOFF,		TRUE	},

    {   "compact",		COMM_COMPACT,		TRUE	},

    {   "brief",			COMM_BRIEF,			TRUE	},

    {   "prompt",			COMM_PROMPT,		TRUE	},

    {   "combine",		COMM_COMBINE,		TRUE	},

    {   "telnet_ga",		COMM_TELNET_GA,		TRUE	},

    {   "show_affects",		COMM_SHOW_AFFECTS,	TRUE	},

    {   "nograts",		COMM_NOGRATS,		TRUE	},

    {   "noemote",		COMM_NOEMOTE,		TRUE	},

    {   "noshout",		COMM_NOSHOUT,		TRUE	},

    {   "notell",			COMM_NOTELL,		TRUE	},

    {   "nochannels",		COMM_NOCHANNELS,		TRUE	},

    {   "snoop_proof",		COMM_SNOOP_PROOF,		FALSE	},

    {   "afk",			COMM_AFK,			TRUE	},

    {	NULL,			0,			0	}

};



const struct flag_type cyber_flags[] =
{
    {	"cyber_comm",        CYBER_COMM,		TRUE	},
    {	"cyber_eyes",        CYBER_EYES,		TRUE	},
    {	"cyber_leg",		 CYBER_LEGS,		TRUE	},
    {	"cyber_chest",		 CYBER_CHEST,	    TRUE	},
    {	"cyber_reflexes",	 CYBER_REFLEXES,	TRUE	},
    {	"cyber_processor",	 CYBER_MIND,		TRUE	},
    {	"cyber_muscles",	 CYBER_STRENGTH,	TRUE	},
    {   "cyber_surgeon",     CYBER_SURGEON,     TRUE    },
    {	NULL,			0,			0	}
};





const struct flag_type info_flags[] =

{

    {   "nomudwatch",		INFO_NOWATCH,		TRUE  },

    {   "nonewnotice",		INFO_NONEWNOTE,		TRUE	},

    {   "nobounty",		INFO_NOBOUNTY,		TRUE	},

    {   "nonewbie",		INFO_NONEWBIE,		TRUE	},

    {   "nohelper",		INFO_NOHELPER,		TRUE	},

    {   "noangel",		INFO_NOANGEL,		TRUE	},

    {   "notips",			INFO_NOTIPS,		TRUE	},

    {   "clangm",			INFO_CLANGM,		TRUE	},

    {   "clantrainer",		INFO_CLANTRAINER,		TRUE	},

    {   "clanenforcer",		INFO_CLANENFORCER,	TRUE	},

    {   "ghost",			INFO_GHOST,			TRUE	},

    {   "noinfo",			INFO_NOINFO,		TRUE	},

    {   "noidle",			INFO_NOIDLE,		TRUE	},

    {   "nocode",			INFO_NOCODE,		TRUE  },

    {   "immmulti",		INFO_IMMMULTI,		TRUE	},

    {   "bozo",			INFO_BOZO,			TRUE	},

    {   "sound",			INFO_SOUNDON,		TRUE	},

    {	NULL,			0,			0	}

};



const struct flag_type area_flags[] =

{

    {	"none",		    AREA_NONE,		FALSE	},

    {	"changed",		AREA_CHANGED,	TRUE	},

    {	"added",		AREA_ADDED,		TRUE	},

    {	"loading",		AREA_LOADING,	FALSE	},

    {   "closed",		AREA_CLOSED,	TRUE	},

    {   "OLC_Protect",	OLC_PROTECT,	TRUE	},
    
    {   "time_fantasy",	TIME_FANTASY,	TRUE	},
    {   "time_modern",	TIME_MODERN,	TRUE	},
    {   "time_future",	TIME_FUTURE,	TRUE	},
    {   "no_tech",	    AREA_NOTECH,	TRUE	},
    {   "no_magic",	    AREA_NOMAGIC,	TRUE	},
    {   "no_holy",	    AREA_NOHOLY,	TRUE	},
    {   "no_felon",	    AREA_NOFELON,	TRUE	},
    {   "anti_good",	AREA_NOGOOD,	TRUE	},
    {   "anti_evil",	AREA_NOEVIL,	TRUE	},
    {	NULL,			0,			0	}

};







const struct flag_type sex_flags[] =

{

    {	"male",		SEX_MALE,		TRUE	},

    {	"female",		SEX_FEMALE,		TRUE	},

    {	"neutral",		SEX_NEUTRAL,		TRUE	},

    { "random",         3,                      TRUE    },   /* ROM */

    {	"none",			SEX_NEUTRAL,		TRUE	},

    {	NULL,			0,			0	}

};







const struct flag_type exit_flags[] =

{

    {   "door",			EX_ISDOOR,		TRUE    },

    {	"closed",		EX_CLOSED,		TRUE	},

    {	"locked",		EX_LOCKED,		TRUE	},

    {	"pickproof",		EX_PICKPROOF,		TRUE	},

    {   "nopass",		EX_NOPASS,		TRUE	},

    {   "easy",			EX_EASY,		TRUE	},

    {   "hard",			EX_HARD,		TRUE	},

    {	"infuriating",		EX_INFURIATING,		TRUE	},

    {	"noclose",		EX_NOCLOSE,		TRUE	},

    {	"nolock",		EX_NOLOCK,		TRUE	},

    {	NULL,			0,			0	}

};







const struct flag_type door_resets[] =

{

    {	"open and unlocked",	0,		TRUE	},

    {	"closed and unlocked",	1,		TRUE	},

    {	"closed and locked",	2,		TRUE	},

    {	NULL,			0,		0	}

};







const struct flag_type room_flags[] =

{

    {	"dark",		ROOM_DARK,			TRUE	},

    {	"no_mob",		ROOM_NO_MOB,		TRUE	},

    {	"indoors",		ROOM_INDOORS,		TRUE	},

    {	"private",		ROOM_PRIVATE,		TRUE  },

    {	"safe",		ROOM_SAFE,			TRUE	},

    {	"solitary",		ROOM_SOLITARY,		TRUE	},

    {	"pet_shop",		ROOM_PET_SHOP,		TRUE	},

    {	"no_recall",	ROOM_NO_RECALL,		TRUE	},

    {	"imp_only",		ROOM_IMP_ONLY,		TRUE  },

    {	"gods_only",	ROOM_GODS_ONLY,		TRUE  },

    {	"heroes_only",	ROOM_HEROES_ONLY,		TRUE	},

    {	"newbies_only",	ROOM_NEWBIES_ONLY,	TRUE	},

    {	"law",		ROOM_LAW,			TRUE	},

    { "nowhere",		ROOM_NOWHERE,		TRUE	},

    { "bank",		ROOM_BANK,			TRUE	},

    { "engraving",	ROOM_ENGRAVING,		TRUE	},

    { "arena",		ROOM_ARENA,			TRUE	},

    { "sustain",		ROOM_SUSTAIN,		TRUE	},

    { "PK_Safe",		ROOM_NOPK,			TRUE	},

    { "helper",		ROOM_HELPER,		TRUE	},

    { "coder",		ROOM_CODER,			TRUE	},

    {	NULL,			0,			0	}

};







const struct flag_type sector_flags[] =

{

    {	"inside",	SECT_INSIDE,		TRUE	},

    {	"city",		SECT_CITY,		TRUE	},

    {	"field",	SECT_FIELD,		TRUE	},

    {	"forest",	SECT_FOREST,		TRUE	},

    {	"hills",	SECT_HILLS,		TRUE	},

    {	"mountain",	SECT_MOUNTAIN,		TRUE	},

    {	"swim",		SECT_WATER_SWIM,	TRUE	},

    {	"noswim",	SECT_WATER_NOSWIM,	TRUE	},

    {   "unused",	SECT_UNUSED,		TRUE	},

    {	"air",		SECT_AIR,		TRUE	},

    {	"desert",	SECT_DESERT,		TRUE	},

    {	NULL,		0,			0	}

};







const struct flag_type type_flags[] =

{

    {	"light",		ITEM_LIGHT,		TRUE	},

    {	"scroll",		ITEM_SCROLL,		TRUE	},

    {	"wand",			ITEM_WAND,		TRUE	},

    {	"clothing",		ITEM_CLOTHING,		TRUE	},

    {	"staff",		ITEM_STAFF,		TRUE	},

    {	"weapon",		ITEM_WEAPON,		TRUE	},

    {	"treasure",		ITEM_TREASURE,		TRUE	},

    {	"armor",		ITEM_ARMOR,		TRUE	},

    {	"potion",		ITEM_POTION,		TRUE	},

    {	"furniture",		ITEM_FURNITURE,		TRUE	},

    {	"trash",		ITEM_TRASH,		TRUE	},

    {	"container",		ITEM_CONTAINER,		TRUE	},

    {	"drinkcontainer",	ITEM_DRINK_CON,		TRUE	},

    {	"key",			ITEM_KEY,		TRUE	},

    {	"food",			ITEM_FOOD,		TRUE	},

    {	"money",		ITEM_MONEY,		TRUE	},

    {	"boat",			ITEM_BOAT,		TRUE	},

    {	"npccorpse",		ITEM_CORPSE_NPC,	TRUE	},

    {	"pc corpse",		ITEM_CORPSE_PC,		FALSE	},

    {	"fountain",		ITEM_FOUNTAIN,		TRUE	},

    {	"pill",			ITEM_PILL,		TRUE	},

    {	"protect",		ITEM_PROTECT,		TRUE	},

    {	"map",			ITEM_MAP,		TRUE	},

    {   "portal",		ITEM_PORTAL,		TRUE	},

    {   "warpstone",		ITEM_WARP_STONE,	TRUE	},

    {	"roomkey",		ITEM_ROOM_KEY,		TRUE	},

    { 	"gem",			ITEM_GEM,		TRUE	},
    
    { 	"grenade",			ITEM_GRENADE,		TRUE	},

    {	"jewelry",		ITEM_JEWELRY,		TRUE	},

    {	"jukebox",		ITEM_JUKEBOX,		TRUE	},

    {	"terminal",		ITEM_TERMINAL,		TRUE	},
    {	"saber_crystal",ITEM_SABER_CRYSTAL,	TRUE	},

    {	NULL,			0,			0	}

};





const struct flag_type extra_flags[] =
{
    {	"glow",			ITEM_GLOW,			TRUE	},
    {	"hum",			ITEM_HUM,			TRUE	},
    {	"dark",			ITEM_DARK,			TRUE	},
    {	"lock",			ITEM_LOCK,			TRUE	},
    {	"evil",			ITEM_EVIL,			TRUE	},
    {	"invis",			ITEM_INVIS,			TRUE	},
    {	"magic",			ITEM_MAGIC,			TRUE	},
    {	"nodrop",			ITEM_NODROP,		TRUE	},
    {	"bless",			ITEM_BLESS,			TRUE	},
    {	"antigood",			ITEM_ANTI_GOOD,		TRUE	},
    {	"antievil",			ITEM_ANTI_EVIL,		TRUE	},
    {	"antineutral",		ITEM_ANTI_NEUTRAL,	TRUE	},
    {	"noremove",			ITEM_NOREMOVE,		TRUE	},
    {	"inventory",		ITEM_INVENTORY,		TRUE	},
    {	"nopurge",			ITEM_NOPURGE,		TRUE	},
    {	"rotdeath",			ITEM_ROT_DEATH,		TRUE	},
    {	"visdeath",			ITEM_VIS_DEATH,		TRUE	},
    { "nonmetal",			ITEM_NONMETAL,		TRUE	},
    {	"meltdrop",			ITEM_MELT_DROP,		TRUE	},
    {	"hadtimer",			ITEM_HAD_TIMER,		TRUE	},
    {	"sellextract",		ITEM_SELL_EXTRACT,	TRUE	},
    {	"burnproof",		ITEM_BURN_PROOF,	TRUE	},
    {	"nouncurse",		ITEM_NOUNCURSE,		TRUE	},
    { "nolocate",			ITEM_NOLOCATE,		TRUE	},
    {  "antimage",			ITEM_ANTI_MAG,		TRUE    },
    {  "anticleric",		ITEM_ANTI_CLE,      TRUE    },
	{  "antidruid",			ITEM_ANTI_DRU,		TRUE	},
	{  "antiwarrior",		ITEM_ANTI_WAR,		TRUE	},
	{  "anticyborg",		ITEM_ANTI_CYB,		TRUE	},
	{  "antithief",			ITEM_ANTI_THI,		TRUE	},
//	{  "ethereal",			ITEM_ETHEREAL,		TRUE	},
//	{  "huge",			ITEM_HUGE,		TRUE	},
    {	NULL,				0,				0	}
};







const struct flag_type wear_flags[] =

{

    {	"take",			ITEM_TAKE,		TRUE	},

    {	"draggable",		ITEM_DRAGGABLE,	TRUE	},

    {	"finger",		ITEM_WEAR_FINGER,	TRUE	},

    {	"neck",			ITEM_WEAR_NECK,		TRUE	},

    {	"body",			ITEM_WEAR_BODY,		TRUE	},

    {	"head",			ITEM_WEAR_HEAD,		TRUE	},

    {	"legs",			ITEM_WEAR_LEGS,		TRUE	},

    {	"feet",			ITEM_WEAR_FEET,		TRUE	},

    {	"hands",		ITEM_WEAR_HANDS,	TRUE	},

    {	"arms",			ITEM_WEAR_ARMS,		TRUE	},

    {	"shield",		ITEM_WEAR_SHIELD,	TRUE	},

    {	"about",		ITEM_WEAR_ABOUT,	TRUE	},

    {	"waist",		ITEM_WEAR_WAIST,	TRUE	},

    {	"wrist",		ITEM_WEAR_WRIST,	TRUE	},

    {	"wield",		ITEM_WIELD,		TRUE	},

    {	"hold",			ITEM_HOLD,		TRUE	},

    {   "nosac",		ITEM_NO_SAC,		TRUE	},

    {	"wearfloat",		ITEM_WEAR_FLOAT,	TRUE	},
    
    {   "cybereye",             ITEM_CYBER_EYE,         TRUE    },
    {   "cybercomm",            ITEM_CYBER_COMM,        TRUE    },
    {   "cyberleg",             ITEM_CYBER_LEGS,        TRUE    },
    {   "cyberchest",           ITEM_CYBER_CHEST,       TRUE    },
    {   "cyberreflex",          ITEM_CYBER_REFLEXES,    TRUE    },
    {   "cybermind",            ITEM_CYBER_MIND,        TRUE    },
    {   "cyberstrength",        ITEM_CYBER_STRENGTH,    TRUE    },    

    /*    {   "twohands",            ITEM_TWO_HANDS,         TRUE    }, */

    {	NULL,			0,			0	}

};



/*

 * Used when adding an affect to tell where it goes.

 * See addaffect and delaffect in act_olc.c

 */

const struct flag_type apply_flags[] =
{
    {	"none",			APPLY_NONE,		TRUE	},
    {	"strength",		APPLY_STR,		TRUE	},
    {	"dexterity",		APPLY_DEX,		TRUE	},
    {	"intelligence",		APPLY_INT,		TRUE	},
    {	"wisdom",		APPLY_WIS,		TRUE	},
    {	"constitution",		APPLY_CON,		TRUE	},
    {	"agility",		APPLY_AGI,		TRUE	},
    {	"mechanical",	APPLY_MCH,		TRUE	},
    {	"technical",	APPLY_TCH,		TRUE	},
    {	"sex",			APPLY_SEX,		TRUE	},
    {	"class",		APPLY_CLASS,		TRUE	},
    {	"level",		APPLY_LEVEL,		TRUE	},
    {	"age",			APPLY_AGE,		TRUE	},
    {	"height",		APPLY_HEIGHT,		TRUE	},
    {	"weight",		APPLY_WEIGHT,		TRUE	},
    {	"mana",			APPLY_MANA,		TRUE	},
    {	"hp",			APPLY_HIT,		TRUE	},
    {	"move",			APPLY_MOVE,		TRUE	},
    {	"gold",			APPLY_GOLD,		TRUE	},
    {	"experience",		APPLY_EXP,		TRUE	},
    {	"ac",			APPLY_AC,		TRUE	},
    {	"hitroll",		APPLY_HITROLL,		TRUE	},
    {	"damroll",		APPLY_DAMROLL,		TRUE	},
    {	"defroll",		APPLY_DEFROLL,		TRUE	},
    {	"saves",		APPLY_SAVES,		TRUE	},
    {	"savingpara",		APPLY_SAVING_PARA,	TRUE	},
    {	"savingrod",		APPLY_SAVING_ROD,	TRUE	},
    {	"savingpetri",		APPLY_SAVING_PETRI,	TRUE	},
    {	"savingbreath",		APPLY_SAVING_BREATH,	TRUE	},
    {	"savingspell",		APPLY_SAVING_SPELL,	TRUE	},
    {	"spellaffect",		APPLY_SPELL_AFFECT,	FALSE	},
    {	NULL,			0,			0	}
};







/*

 * What is seen.

 */

const struct flag_type wear_loc_strings[] =

{

    {	"in the inventory",	WEAR_NONE,	TRUE	},

    {	"as a light",		WEAR_LIGHT,	TRUE	},

    {	"on the left finger",	WEAR_FINGER_L,	TRUE	},

    {	"on the right finger",	WEAR_FINGER_R,	TRUE	},

    {	"around the neck (1)",	WEAR_NECK_1,	TRUE	},

    {	"around the neck (2)",	WEAR_NECK_2,	TRUE	},

    {	"on the body",		WEAR_BODY,	TRUE	},

    {	"over the head",	WEAR_HEAD,	TRUE	},

    {	"on the legs",		WEAR_LEGS,	TRUE	},

    {	"on the feet",		WEAR_FEET,	TRUE	},

    {	"on the hands",		WEAR_HANDS,	TRUE	},

    {	"on the arms",		WEAR_ARMS,	TRUE	},

    {	"as a shield",		WEAR_SHIELD,	TRUE	},

    {	"about the shoulders",	WEAR_ABOUT,	TRUE	},

    {	"around the waist",	WEAR_WAIST,	TRUE	},

    {	"on the left wrist",	WEAR_WRIST_L,	TRUE	},

    {	"on the right wrist",	WEAR_WRIST_R,	TRUE	},

    {	"wielded",		WEAR_WIELD,	TRUE	},

    {	"held in the hands",	WEAR_HOLD,	TRUE	},

    {	"floating nearby",	WEAR_FLOAT,	TRUE	},
    {   "in optical socket",    WEAR_CYBER_EYE,      TRUE    },
    {   "in comm socket",       WEAR_CYBER_COMM,     TRUE    },
    {   "in leg socket",        WEAR_CYBER_LEGS,     TRUE    },
    {   "in chest socket",      WEAR_CYBER_CHEST,    TRUE    },
    {   "in reflext socket",    WEAR_CYBER_REFLEXES, TRUE    },    
    {   "in processor socket",  WEAR_CYBER_MIND,     TRUE    },
    {   "in str socket",        WEAR_CYBER_STRENGTH, TRUE    },
    {	NULL,			0	      , 0	}
};





const struct flag_type wear_loc_flags[] =
{
    {	"none",		WEAR_NONE,	TRUE	},
    {	"light",	WEAR_LIGHT,	TRUE	},
    {	"lfinger",	WEAR_FINGER_L,	TRUE	},
    {	"rfinger",	WEAR_FINGER_R,	TRUE	},
    {	"neck1",	WEAR_NECK_1,	TRUE	},
    {	"neck2",	WEAR_NECK_2,	TRUE	},
    {	"body",		WEAR_BODY,	TRUE	},
    {	"head",		WEAR_HEAD,	TRUE	},
    {	"legs",		WEAR_LEGS,	TRUE	},
    {	"feet",		WEAR_FEET,	TRUE	},
    {	"hands",	WEAR_HANDS,	TRUE	},
    {	"arms",		WEAR_ARMS,	TRUE	},
    {	"shield",	WEAR_SHIELD,	TRUE	},
    {	"about",	WEAR_ABOUT,	TRUE	},
    {	"waist",	WEAR_WAIST,	TRUE	},
    {	"lwrist",	WEAR_WRIST_L,	TRUE	},
    {	"rwrist",	WEAR_WRIST_R,	TRUE	},
    {	"wielded",	WEAR_WIELD,	TRUE	},
    {	"hold",		WEAR_HOLD,	TRUE	},
    {	"floating",	WEAR_FLOAT,	TRUE	},
    {   "cybereye",         WEAR_CYBER_EYE,         TRUE    },  
    {   "cybercomm",        WEAR_CYBER_COMM,        TRUE    },
    {   "cyberleg",         WEAR_CYBER_LEGS,        TRUE    },
    {   "cyberchest",       WEAR_CYBER_CHEST,       TRUE    },
    {   "cyberreflex",      WEAR_CYBER_REFLEXES,    TRUE    },
    {   "cyberproc",        WEAR_CYBER_MIND,        TRUE    },
    {   "cyberstrength",    WEAR_CYBER_STRENGTH,    TRUE    },
    {	NULL,		0,		0	}
};



const 	struct flag_type weapon_flags[]=

{

    { 	"hit",		0,		TRUE	},  /*  0 */

    {	"slice", 	1,		TRUE	},	

    {   "stab",		2,		TRUE	},

    {	"slash",	3,		TRUE	},

    {	"whip",		4,		TRUE	},

    {   "claw",		5,		TRUE	},  /*  5 */

    {	"blast",	6,		TRUE	},

    {   "pound",	7,		TRUE	},

    {	"crush",	8,		TRUE	},

    {   "grep",		9,		TRUE	},

    {	"bite",		10,		TRUE	},  /* 10 */

    {   "pierce",	11,		TRUE	},

    {   "suction",	12,		TRUE	},

    {	"beating",	13,		TRUE	},

    {   "digestion",	14,		TRUE	},

    {	"charge",	15,		TRUE	},  /* 15 */

    { 	"slap",		16,		TRUE	},

    {	"punch",	17,		TRUE	},

    {	"wrath",	18,		TRUE	},

    {	"magic",	19,		TRUE	},

    {   "divinepower",	20,		TRUE	},  /* 20 */

    {	"cleave",	21,		TRUE	},

    {	"scratch",	22,		TRUE	},

    {   "peckpierce",	23,		TRUE	},

    {   "peckbash",	24,		TRUE	},

    {   "chop",		25,		TRUE	},  /* 25 */

    {   "sting",	26,		TRUE	},

    {   "smash",	27,		TRUE	},

    {   "shockingbite",28,		TRUE	},

    {	"flamingbite", 29,		TRUE	},

    {	"freezingbite", 30,		TRUE	},  /* 30 */

    {	"acidicbite", 	31,		TRUE	},

    {	"chomp",	32,		TRUE	},

    {  	"lifedrain",	33,		TRUE	},

    {   "thrust",	34,		TRUE	},

    {   "slime",	35,		TRUE	},

    {	"shock",	36,		TRUE	},

    {   "thwack",	37,		TRUE	},

    {   "flame",	38,		TRUE	},

    {   "chill",	39,		TRUE	},

    {   NULL,		0,		0	}

};



const struct flag_type container_flags[] =

{

    {	"closeable",		1,		TRUE	},

    {	"pickproof",		2,		TRUE	},

    {	"closed",		4,		TRUE	},

    {	"locked",		8,		TRUE	},

    {	"puton",		16,		TRUE	},

    {	NULL,			0,		0	}

};



/*****************************************************************************

                      ROM - specific tables:

 ****************************************************************************/









const struct flag_type ac_type[] =

{

    {   "pierce",        AC_PIERCE,            TRUE    },

    {   "bash",          AC_BASH,              TRUE    },

    {   "slash",         AC_SLASH,             TRUE    },

    {   "exotic",        AC_EXOTIC,            TRUE    },

    {   NULL,              0,                    0       }

};





const struct flag_type size_flags[] =

{

    {   "tiny",          SIZE_TINY,            TRUE    },

    {   "small",         SIZE_SMALL,           TRUE    },

    {   "medium",        SIZE_MEDIUM,          TRUE    },

    {   "large",         SIZE_LARGE,           TRUE    },

    {   "huge",          SIZE_HUGE,            TRUE    },

    {   "giant",         SIZE_GIANT,           TRUE    },

    {   NULL,              0,                    0       },

};





const struct flag_type weapon_class[] =
{
    {   "exotic",        0,                    TRUE    },
    {   "sword",         1,                    TRUE    },
    {   "dagger",        2,                    TRUE    },
    {   "spear",         3,                    TRUE    },
    {   "mace",          4,                    TRUE    },
    {   "axe",           5,                    TRUE    },
    {   "flail",         6,                    TRUE    },
    {   "whip",          7,                    TRUE    },
    {   "polearm",       8,                    TRUE    },
	{	"projectile",	 9,					   TRUE	   },
	{	"battelh",	    10,					   TRUE	   },
	{	"lightsaber",   11,					   TRUE	   },
	{	"stake",		12,					   TRUE	   },
    {   NULL,            0,                    0       }
};





const struct flag_type weapon_type2[] =

{

    {   "none",		 0,			TRUE   },

    {   "flaming",       WEAPON_FLAMING,       TRUE    },

    {   "frost",         WEAPON_FROST,         TRUE    },

    {   "vampiric",      WEAPON_VAMPIRIC,      TRUE    },

    {   "sharp",         WEAPON_SHARP,         TRUE    },

    {   "vorpal",        WEAPON_VORPAL,        TRUE    },

    {   "twohands",     WEAPON_TWO_HANDS,     TRUE    },

    {	"shocking",	 WEAPON_SHOCKING,      TRUE    },

    {	"poison",	WEAPON_POISON,		TRUE	},

    {   NULL,              0,                    0       }

};



const struct flag_type res_flags[] =

{

    {	"summon",	 RES_SUMMON,		TRUE	},

    {   "charm",         RES_CHARM,            TRUE    },

    {   "magic",         RES_MAGIC,            TRUE    },

    {   "weapon",        RES_WEAPON,           TRUE    },

    {   "bash",          RES_BASH,             TRUE    },

    {   "pierce",        RES_PIERCE,           TRUE    },

    {   "slash",         RES_SLASH,            TRUE    },

    {   "fire",          RES_FIRE,             TRUE    },

    {   "cold",          RES_COLD,             TRUE    },

    {   "lightning",     RES_LIGHTNING,        TRUE    },

    {   "acid",          RES_ACID,             TRUE    },

    {   "poison",        RES_POISON,           TRUE    },

    {   "negative",      RES_NEGATIVE,         TRUE    },

    {   "holy",          RES_HOLY,             TRUE    },

    {   "energy",        RES_ENERGY,           TRUE    },

    {   "mental",        RES_MENTAL,           TRUE    },

    {   "disease",       RES_DISEASE,          TRUE    },

    {   "drowning",      RES_DROWNING,         TRUE    },

    {   "light",         RES_LIGHT,            TRUE    },

    {	"sound",	RES_SOUND,		TRUE	},

    {	"wood",		RES_WOOD,		TRUE	},

    {	"silver",	RES_SILVER,		TRUE	},

    {	"iron",		RES_IRON,		TRUE	},

    {   NULL,          0,            0    }

};





const struct flag_type vuln_flags[] =

{

    {	"summon",	 VULN_SUMMON,		TRUE	},

    {	"charm",	VULN_CHARM,		TRUE	},

    {   "magic",         VULN_MAGIC,           TRUE    },

    {   "weapon",        VULN_WEAPON,          TRUE    },

    {   "bash",          VULN_BASH,            TRUE    },

    {   "pierce",        VULN_PIERCE,          TRUE    },

    {   "slash",         VULN_SLASH,           TRUE    },

    {   "fire",          VULN_FIRE,            TRUE    },

    {   "cold",          VULN_COLD,            TRUE    },

    {   "lightning",     VULN_LIGHTNING,       TRUE    },

    {   "acid",          VULN_ACID,            TRUE    },

    {   "poison",        VULN_POISON,          TRUE    },

    {   "negative",      VULN_NEGATIVE,        TRUE    },

    {   "holy",          VULN_HOLY,            TRUE    },

    {   "energy",        VULN_ENERGY,          TRUE    },

    {   "mental",        VULN_MENTAL,          TRUE    },

    {   "disease",       VULN_DISEASE,         TRUE    },

    {   "drowning",      VULN_DROWNING,        TRUE    },

    {   "light",         VULN_LIGHT,           TRUE    },

    {	"sound",	 VULN_SOUND,		TRUE	},

    {   "wood",          VULN_WOOD,            TRUE    },

    {   "silver",        VULN_SILVER,          TRUE    },

    {   "iron",          VULN_IRON,            TRUE    },

    {   NULL,              0,                    0       }

};





const struct flag_type material_type[] =    /* not yet implemented */

{

    {   "none",          0,            TRUE    },

    {   NULL,              0,            0       }

};





const struct flag_type position_flags[] =

{

    {   "dead",           POS_DEAD,            FALSE   },

    {   "mortal",         POS_MORTAL,          FALSE   },

    {   "incap",          POS_INCAP,           FALSE   },

    {   "stunned",        POS_STUNNED,         TRUE	 },

    {   "sleeping",       POS_SLEEPING,        TRUE    },

    {   "resting",        POS_RESTING,         TRUE    },

    {   "sitting",        POS_SITTING,         TRUE    },

    {   "fighting",       POS_FIGHTING,        FALSE   },

    {   "standing",       POS_STANDING,        TRUE    },

    {   NULL,              0,                    0       }

};



const struct flag_type portal_flags[]=

{

    {   "normal_exit",	  GATE_NORMAL_EXIT,	TRUE	},

    {	"no_curse",	  GATE_NOCURSE,		TRUE	},

    {   "go_with",	  GATE_GOWITH,		TRUE	},

    {   "buggy",	  GATE_BUGGY,		TRUE	},

    {	"random",	  GATE_RANDOM,		TRUE	},

    {   NULL,		  0,			0	}

};



const struct flag_type furniture_flags[]=

{

    {	"none",		  0,			TRUE	},

    {   "stand_at",	  STAND_AT,		TRUE	},

    {	"stand_on",	  STAND_ON,		TRUE	},

    {	"stand_in",	  STAND_IN,		TRUE	},

    {	"sit_at",	  SIT_AT,		TRUE	},

    {	"sit_on",	  SIT_ON,		TRUE	},

    {	"sit_in",	  SIT_IN,		TRUE	},

    {	"rest_at",	  REST_AT,		TRUE	},

    {	"rest_on",	  REST_ON,		TRUE	},

    {	"rest_in",	  REST_IN,		TRUE	},

    {	"sleep_at",	  SLEEP_AT,		TRUE	},

    {	"sleep_on",	  SLEEP_ON,		TRUE	},

    {	"sleep_in",	  SLEEP_IN,		TRUE	},

    {	"put_at",	  PUT_AT,		TRUE	},

    {	"put_on",	  PUT_ON,		TRUE	},

    {	"put_in",	  PUT_IN,		TRUE	},

    {	"put_inside",	  PUT_INSIDE,		TRUE	},

    {	NULL,		  0,			0	}

};



const struct flag_type icf_her_flags[] =

{

	{	"nochannels",	HER_NOCHANNELS,	TRUE	},

	{	"noemote",		HER_NOEMOTE,	TRUE	},

	{	"noshout",		HER_NOSHOUT,	TRUE	},

	{	"notell",		HER_NOTELL,		TRUE	},

	{	NULL,			0,			0	}

};



const struct flag_type icf_ang_flags[] =

{

	{	"clanadmin",	ANG_CLANADMIN,	TRUE	},

	{	"disconnect",	ANG_DISCONNECT,	TRUE	},

	{	"hack",		ANG_HACK,		TRUE	},

	{	"freeze",		ANG_FREEZE,		TRUE	},

	{	"force",		ANG_FORCE,		TRUE	},

	{	"load",		ANG_LOAD,		TRUE	},

	{	"newlock",		ANG_NEWLOCK,	TRUE	},

	{	"pecho",		ANG_PECHO,		TRUE	},

	{	"pardon",		ANG_PARDON,		TRUE	},

	{	"purge",		ANG_PURGE,		TRUE	},

	{	"restore",		ANG_RESTORE,	TRUE	},

	{	"transfer",		ANG_TRANSFER,	TRUE	},

	{	"at",			ANG_AT,		TRUE	},

	{	"gecho",		ANG_GECHO,		TRUE	},

	{	"log",		ANG_LOG,		TRUE	},

	{	"smote",		ANG_SMOTE,		TRUE	},

	{	"peace",		ANG_PEACE,		TRUE	},

	{	"echo",		ANG_ECHO,		TRUE	},

	{	"return",		ANG_RETURN,		TRUE	},

	{	"string",		ANG_STRING,		TRUE	},

	{	"switch",		ANG_SWITCH,		TRUE	},

	{	"zecho",		ANG_ZECHO,		TRUE	},

	{	"clone",		ANG_CLONE,		TRUE	},

	{	"slay",		ANG_SLAY,		TRUE	},

	{	NULL,			0,			0	}

};



const struct flag_type icf_stat_flags[] =

{

	{	"memory",		STAT_MEMORY,	TRUE	},

	{	"mwhere",		STAT_MWHERE,	TRUE	},

	{	"owhere",		STAT_OWHERE,	TRUE	},

	{	"rwhere",		STAT_RWHERE,	TRUE	},

	{	"stat",		STAT_STAT,		TRUE	},

	{	"astat",		STAT_ASTAT,		TRUE	},

	{	"slookup",		STAT_SLOOKUP,	TRUE	},

	{	"vnum",		STAT_VNUM,		TRUE	},

	{	NULL,			0,			0	}

};



const struct flag_type icf_sup_flags[] =

{

	{	"permban",		SUP_PERMBAN,	TRUE	},

	{	"process",		SUP_PROCESS,	TRUE	},

	{	"protect",		SUP_PROTECT,	TRUE	},

	{	"reboot",		SUP_REBOOT,		TRUE	},

	{	"set",		SUP_SET,		TRUE	},

	{	"shutdown",		SUP_SHUTDOWN,	TRUE	},

	{	"sockets",		SUP_SOCKETS,	TRUE	},

	{	"wizlock",		SUP_WIZLOCK,	TRUE	},

	{	"betalock",		SUP_BETALOCK,	TRUE	},

	{	"totalwar",		SUP_TOTALWAR,	TRUE	},

	{	"addlag",		SUP_ADDLAG,		TRUE	},

	{	"bonus",		SUP_BONUS,		TRUE	},

	{	"allow",		SUP_ALLOW,		TRUE	},

	{	"ban",		SUP_BAN,		TRUE	},

	{	"deny",		SUP_DENY,		TRUE	},

	{	"flag",		SUP_FLAG,		TRUE	},

	{	"olclock",		SUP_OLCLOCK,	TRUE	},

	{	"ipaddress",	SUP_IPADDRESS,	TRUE	},

	{	"unlockdb",		SUP_UNLOCKDB,	TRUE	},

	{	"unlockspool",	SUP_UNLOCKSPOOL,	TRUE	},

	{	"snoop",		SUP_SNOOP,		TRUE	},

	{	"snooplist",	SUP_SNOOPLIST,	TRUE	},

	{	NULL,			0,			0	}

};

