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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"


extern char last_command_who [MAX_STRING_LENGTH];
extern char last_command_loctime [MAX_STRING_LENGTH];
extern char last_command_what [MAX_STRING_LENGTH];


/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER		2



/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;



/*
 * Command table.
 */

/* const	struct	cmd_type	cmd_table	[] = (read only) */

struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,    	 	 0,  LOG_NEVER, 0,	TRUE,	TRUE },
    { "east",		do_east,	POS_STANDING,	 	 0,  LOG_NEVER, 0,	TRUE,	TRUE },
    { "south",		do_south,	POS_STANDING,	 	 0,  LOG_NEVER, 0,	TRUE,	TRUE },
    { "west",		do_west,	POS_STANDING,	 	 0,  LOG_NEVER, 0,	TRUE,	TRUE },
    { "up",			do_up,	POS_STANDING,	 	 0,  LOG_NEVER, 0,	TRUE,	TRUE },
    { "down",		do_down,	POS_STANDING,	 	 0,  LOG_NEVER, 0,	TRUE,	TRUE },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "at",			do_at,		POS_DEAD,		L6,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "cast",		do_cast,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "assemble",		do_assemble,		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "activate",		do_activate,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "auction",        do_auction,     	POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "bounty",		do_bounty,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "buy",		do_buy,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "channels",       do_channels,    	POS_DEAD,		 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "crecall",		do_clan_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "cyber",		do_cyber,	POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE, 	TRUE },
    { "exits",		do_exits,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "force",		do_force,		POS_DEAD,		L5,  LOG_ALWAYS, 1,	TRUE,	TRUE 	},
    { "forcepower",		do_forcepower,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "get",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "goto",		do_goto,		POS_DEAD,		L8,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "group",          do_group,       	POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "guild",		do_guild,		POS_DEAD,		 0,  LOG_ALWAYS, 1,	TRUE,	TRUE 	},
    { "hit",		do_kill,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,	TRUE,	FALSE },
    { "inventory",	do_inventory,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "kill",		do_kill,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "look",		do_look,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "clantalk",		do_clantalk,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "clanlist",		do_clanlist,	POS_DEAD,		10,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "clanrequest",	do_clanrequest,	POS_DEAD,		10,  LOG_NORMAL, 1,	TRUE, FALSE },
    { "music",          do_music,   	POS_SLEEPING,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "order",		do_order,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "practice",       do_practice,	POS_SLEEPING,    	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "quest",          do_quest,		POS_RESTING,     	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "rest",		do_rest,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "reanimate",	do_reanimate,	POS_DEAD,		10,  LOG_NORMAL, 1,	TRUE,	TRUE	},
    { "sit",		do_sit,		POS_SLEEPING,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "sockets",		do_sockets,		POS_DEAD,		L4,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "stand",		do_stand,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE	},
    { "tell",		do_tell,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE 	},
    { "unlock",         do_unlock,		POS_RESTING,       0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "wield",		do_wear,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "wizhelp",		do_wizhelp,		POS_DEAD,		HE,  LOG_NORMAL, 1,	TRUE,	TRUE 	},

    /*
     * Informational commands.
     */
    { "accept",		do_accept_vampire,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "affects",		do_affects,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "areas",		do_areas,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "bug",		do_bug,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "changes",		do_changes,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "commands",		do_commands,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "compare",		do_compare,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "consider",		do_consider,	POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "count",		do_count,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "credits",		do_credits,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "equipment",	do_equipment,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "examine",		do_examine,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "finger",		do_finger,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
/*  { "groups",		do_groups,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "help",		do_help,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "idea",		do_idea,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "motd",		do_motd,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "news",		do_news,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "nobounty",		do_nobounty,	POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "read",		do_read,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "reject",		do_reject_vampire,POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "rem",		do_remove,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "remor",		do_remor,		POS_DEAD,		 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "remort",		do_remort,		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "report",		do_report,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "rules",		do_rules,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "score",		do_score,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "skills",		do_skills,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "socials",		do_socials,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "show",		do_show,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "spells",		do_spells,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "story",		do_story,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "tabledump",	do_tabledump,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
	{ "turn",		do_turn_vampire,POS_STANDING, 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "version",		do_version,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "time",		do_time,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "typo",		do_typo,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "weather",		do_weather,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "who",		do_who,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "whois",		do_whois,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "wizlist",		do_wizlist,		POS_DEAD,        	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "worth",		do_worth,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "rpnote",		do_rpnote,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "clannote",		do_clannote,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },

    /*
     * Configuration commands.
     */
    { "alia",		do_alia,		POS_DEAD,	 	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "alias",		do_alias,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autolist",		do_autolist,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autoassist",	do_autoassist,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autoexit",		do_autoexit,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autogold",		do_autogold,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autoloot",		do_autoloot,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autosac",		do_autosac,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autosplit",	do_autosplit,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "autowizi",		do_autowizi,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "brief",		do_brief,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "channels",		do_channels,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "colour",		do_colour,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "combine",		do_combine,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "compact",		do_compact,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "description",	do_description,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "delet",		do_delet,		POS_DEAD,	 	 0,  LOG_ALWAYS, 0,	TRUE,	TRUE },
    { "delete",		do_delete,		POS_DEAD,		 0,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "nofollow",		do_nofollow,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "noloot",		do_noloot,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "nosummon",		do_nosummon,	POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "outfit",		do_outfit,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "password",		do_password,	POS_DEAD,	 	 0,  LOG_NEVER,  1,	TRUE,	TRUE },
    { "prompt",		do_prompt,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "scroll",		do_scroll,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "sound",		do_soundon,		POS_DEAD,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "clear",		do_clear,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "title",		do_title,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "unalias",		do_unalias,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "wimpy",		do_wimpy,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },

    /*
     * Communication commands.
     */
    { "afk",		do_afk,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "answer",		do_answer,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
/*  { "auction",		do_auction,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "coder",		do_codertalk,	POS_DEAD,		 0,  LOG_NORMAL, 1,	TRUE, TRUE }, /* Storm */
    { "deaf",		do_deaf,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "emote",		do_emote,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "pmote",		do_pmote,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { ".",			do_gossip,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "gossip",		do_gossip,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "flame",		do_flamechan,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "ooc",		do_oocchan,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "newbie",		do_newbietalk,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { ",",			do_emote,		POS_RESTING,	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "grats",		do_grats,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "gtell",		do_gtell,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { ";",			do_gtell,		POS_DEAD,	 	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "helpertalk",	do_helpertalk,	POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "info",           do_info,  		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
/*  { "music",		do_music,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "note",		do_note,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "pray",		do_pray,		POS_DEAD,		 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "pose",		do_pose,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "question",		do_question,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "quote",		do_quote,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "quiet",		do_quiet,		POS_DEAD, 	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "reply",		do_reply,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "replay",		do_replay,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "say",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "showtips",		do_tips,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "'",			do_say,		POS_RESTING,	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "shout",		do_shout,		POS_RESTING,	 3,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "unread",		do_unread,		POS_SLEEPING,    	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "yell",		do_yell,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },

    /*
     * Object manipulation commands.
     */
    { "brandish",		do_brandish,	POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "close",		do_close,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "drink",		do_drink,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "open",		do_open,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "drag",		do_drag,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "drop",		do_drop,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "eat",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "envenom",	do_envenom,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "fill",		do_fill,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "give",		do_give,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "heal",		do_heal,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "hold",		do_wear,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "list",		do_list,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "lock",		do_lock,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
//  { "lore",		do_lore,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "pick",		do_pick,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "pour",		do_pour,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "put",		do_put,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "quaff",		do_quaff,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "recite",		do_recite,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "toss",		do_toss,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "remove",		do_remove,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "sell",		do_sell,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "sacrifice",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "junk",           do_sacrifice,   	POS_RESTING,     	 0,  LOG_NORMAL, 0,	TRUE,	FALSE },
    { "tap",      	do_sacrifice,   	POS_RESTING,     	 0,  LOG_NORMAL, 0,	TRUE,	FALSE },
    { "unlock",		do_unlock,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "value",		do_value,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "wear",		do_wear,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "second",		do_second,  	POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "zap",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "balance",		do_balance,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "deposit",		do_deposit,		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "withdraw",		do_withdraw,	POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "wiretransfer",	do_wiretransfer,	POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE, FALSE	},
    { "restring",		do_restring,	POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },

    /*
     * Combat commands.
     */
    { "backstab",		do_backstab,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "bash",		do_bash,		POS_FIGHTING,      0,  LOG_NORMAL, 1,	TRUE, FALSE },
    { "stun",		do_stun,		POS_FIGHTING,      0,  LOG_NORMAL, 1,	TRUE, FALSE },
    { "berserk",		do_berserk,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "power",		do_power_attack,POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "circlestab",	do_circlestab,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "dirt",		do_dirt,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "disarm",		do_disarm,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "flee",		do_flee,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "fury",		do_fury,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "destruction",	do_destruction,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "kick",		do_kick,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "spinkick",		do_spin,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE, FALSE },
    { "murde",		do_murde,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,	TRUE,	FALSE },
    { "murder",		do_murder,		POS_FIGHTING,	10,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "rescue",		do_rescue,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,	TRUE,	FALSE },
    { "trip",		do_trip,		POS_FIGHTING,      0,  LOG_NORMAL, 1,	TRUE, FALSE },
    { "gouge",		do_gouge,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "buffet",  	do_buffet,		POS_FIGHTING,    0,  LOG_NORMAL, 1, 	TRUE,  FALSE },
    { "drain", 		do_drain,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "sting",		do_sting,		POS_FIGHTING,    0,  LOG_NORMAL, 1, 	TRUE,	FALSE },
    { "sweep",		do_sweep,		POS_FIGHTING,	 0,  LOG_NORMAL, 1, 	TRUE,   FALSE },
    { "poetry",		do_poetry,		POS_FIGHTING,    0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "stake",		do_stake,		POS_FIGHTING,    0,  LOG_NORMAL, 1,	TRUE,	FALSE },

    /*
     * Miscellaneous commands.
     */
    { "enter", 		do_enter, 		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "follow",		do_follow,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "gain",		do_gain,		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "go",			do_enter,		POS_STANDING,	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
/*  { "group",		do_group,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },  */
    { "groups",		do_groups,		POS_SLEEPING,      0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "hide",		do_hide,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "play",		do_play,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "practice",		do_practice,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "qui",		do_qui,		POS_DEAD,	 	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "quit",		do_quit,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "recall",		do_recall,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "/",			do_recall,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "rent",		do_rent,		POS_DEAD,	 	 0,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "roomin",		do_roomin,		POS_DEAD,	 	15,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "roomout",		do_roomout,		POS_DEAD,	 	15,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "save",		do_save,		POS_DEAD,	 	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "sleep",		do_sleep,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "sneak",		do_sneak,		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "split",		do_split,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "steal",		do_steal,		POS_STANDING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "suicid",		do_suicid,		POS_DEAD,		 0,  LOG_NORMAL, 0,	TRUE,	FALSE },
    { "suicide",		do_suicide,		POS_DEAD,		 0,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "trance",		do_trance,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "train",		do_train,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	FALSE },
    { "visible",		do_visible,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "wake",		do_wake,		POS_SLEEPING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "where",		do_where,		POS_RESTING,	 0,  LOG_NORMAL, 1,	TRUE,	TRUE },

    /*
     * Immortal commands.
     */
    { "addlag",		do_addlag,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "advance",		do_advance,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "arealinks",	do_arealinks,	POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "bonus",		do_bonus,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "cmdctrl",		do_cmdctrl,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "clanadmin",	do_clanadmin,	POS_DEAD,		IM,  LOG_ALWAYS, 1,	TRUE, TRUE },
    { "crash",	      do_crash,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "dump",		do_dump,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "trust",		do_trust,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "trustlist",	do_trustlist,	POS_DEAD,		ML,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "violate",		do_violate,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "hack",           do_hack,    	POS_RESTING,	L7,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "allow",		do_allow,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "ban",		do_ban,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "deny",		do_deny,		POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "disconnect",	do_disconnect,	POS_DEAD,		L3,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "flag",		do_flag,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "freeze",		do_freeze,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "nuke",		do_nuke,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
/*  { "pause",		do_pause,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE }, */
    { "permban",		do_permban,		POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "process",		do_process,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "protect",		do_protect,		POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "pload",		do_pload,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "punload",		do_punload,		POS_DEAD,		ML,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "reboo",		do_reboo,		POS_DEAD,		L1,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "reboot",		do_reboot,		POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "set",		do_set,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "shutdow",		do_shutdow,		POS_DEAD,		L1,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "shutdown",		do_shutdown,	POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "autorestart",	do_autorestart,	POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
/*  { "sockets",		do_sockets,		POS_DEAD,		L2,  LOG_NORMAL, 1,	TRUE,	TRUE },  */
    { "wizlock",		do_wizlock,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "betalock",		do_betalock,	POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "totalwar",		do_totalwar,	POS_DEAD,		L1,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "lockstat",		do_lockstat,	POS_DEAD,		IM,  LOG_ALWAYS, 1,	TRUE,	TRUE },
/*  { "force",		do_force,		POS_DEAD,		L5,  LOG_ALWAYS, 1,	TRUE,	TRUE },  */
    { "load",		do_load,		POS_DEAD,		L6,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "newlock",		do_newlock,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "nochannels",	do_nochannels,	POS_DEAD,		HE,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "noemote",		do_noemote,		POS_DEAD,		L8,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "noshout",		do_noshout,		POS_DEAD,		L8,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "notell",		do_notell,		POS_DEAD,		L8,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "pecho",		do_pecho,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "pardon",		do_pardon,		POS_DEAD,		L7,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "purge",		do_purge,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "restore",		do_restore,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "sla",		do_sla,		POS_DEAD,		L3,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "slay",		do_slay,		POS_DEAD,		L3,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "teleport",		do_transfer,	POS_DEAD,		L7,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "transfer",		do_transfer,	POS_DEAD,		L7,  LOG_ALWAYS, 1,	TRUE,	TRUE },
/*  { "at",			do_at,		POS_DEAD,		L6,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "poofin",		do_bamfin,		POS_DEAD,		L8,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "poofout",		do_bamfout,		POS_DEAD,		L8,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "gecho",		do_echo,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
/*  { "iecho",		do_iecho,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE }, */
/*  { "goto",		do_goto,		POS_DEAD,		L8,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "holylight",	do_holylight,	POS_DEAD,		HE,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "incognito",	do_incognito,	POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
/*  { "invis",		do_invis,		POS_DEAD,		L6,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "log",		do_log,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "memory",		do_memory,		POS_DEAD,		L5,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "mwhere",		do_mwhere,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "olclock",		do_olclock,		POS_DEAD,	 	L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "owhere",		do_owhere,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
/*  { "rwhere",		do_rwhere,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "peace",		do_peace,		POS_DEAD,		L5,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "penalty",		do_penalty,		POS_DEAD,		HE,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "immnote",		do_immnote,		POS_DEAD,		HE,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "echo",		do_recho,		POS_DEAD,		L6,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "return",         do_return,  	POS_DEAD,   	L4,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "snoop",		do_snoop,		POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "snooplist",	do_snooplist,	POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "stat",		do_stat,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "slist",		do_slist,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
/*  { "astat",		do_areastat,	POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE }, */
    { "slookup", 		do_slookup,		POS_DEAD,		L4,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "string",		do_string,		POS_DEAD,		L6,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "switch",		do_switch,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	FALSE,TRUE },
    { "unlockdb",		do_unlock_db,	POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "unlockspool",	do_unlock_spool,	POS_DEAD,		L2,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "wizinvis",		do_invis,		POS_DEAD,		L6,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "wiziin",			do_wiziin,		POS_DEAD,		L6,  LOG_NORMAL, 1, TRUE,  TRUE },   // Ferg-wizicust
    { "wiziout",		do_wiziout,		POS_DEAD,		L6,  LOG_NORMAL, 1, TRUE,  TRUE }, // Ferg-wizicust
    { "vnum",		do_vnum,		POS_DEAD,		L6,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "zecho",		do_zecho,		POS_DEAD,		L4,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "clone",		do_clone,		POS_DEAD,		L6,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "wiznet",		do_wiznet,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "immtalk",		do_immtalk,		POS_DEAD,		HE,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "angtalk",		do_angtalk,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "suptalk",		do_suptalk,		POS_DEAD,		L2,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "imotd", 		do_imotd,   	POS_DEAD,   	HE,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { ":",			do_angtalk,		POS_DEAD,		IM,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "|",			do_suptalk,		POS_DEAD,		L2,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "smote",		do_smote,		POS_DEAD,		IM,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "prefi",		do_prefi,		POS_DEAD,		HE,  LOG_NORMAL, 0,	TRUE,	TRUE },
    { "prefix",		do_prefix,		POS_DEAD,		HE,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "ipaddress",	do_ipaddress,	POS_DEAD,		L4,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "chkdupip",		do_chkdupip,	POS_DEAD,		L4,  LOG_NORMAL, 1,	TRUE, TRUE },
    { "jail",		do_jail,		POS_DEAD,		HE,  LOG_ALWAYS, 1,	TRUE,	TRUE },
    { "release",		do_release,		POS_DEAD,		HE,  LOG_ALWAYS, 1,	TRUE,	TRUE },


    /*
     * OLC
     */
    { "buildertalk",	do_buildtalk, 	POS_DEAD,		BD,  LOG_NORMAL, 1,	TRUE,	TRUE },
    { "edit",		do_olc,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "asave",          do_asave,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "alist",		do_alist,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "resets",		do_resets,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "redit",		do_redit,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "medit",		do_medit,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "aedit",		do_aedit,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },
    { "oedit",		do_oedit,		POS_DEAD,    	BD,  LOG_ALWAYS, 1,	TRUE,	FALSE },

    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0,	TRUE,	TRUE }
};




/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    int string_count;
    int perm_count;
    char cmd_copy[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int trust;
    bool found;

/*
    int nAllocString;
    int nAllocPerm;
*/

    string_count = nAllocString;
    perm_count = nAllocPerm;

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * No hiding.
     */
    REMOVE_BIT( ch->affected_by, AFF_HIDE );

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );

    /*
     * Check to see if we are ploaded or linkdead
     */
    if ( !IS_NPC(ch) )
    {
	if ( is_ploaded(ch) )
	{
	   sprintf( buf, "{rCommand{w :: {C[{cPLOAD{C]{w :: %s :: %s", ch->name, logline );
	   wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	   sprintf( buf, "Command :: PLOADED :: %s :: %s", ch->name, logline );
	   log_string ( buf );
	   return;
	}

	if ( is_linkdead(ch) )
	{
	   sprintf( buf, "{rCommand{w :: {r|{yLink Dead{r|{w :: %s :: %s", ch->name, logline );
	   wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	   sprintf( buf, "Command :: Link Dead :: %s :: %s", ch->name, logline );
	   log_string ( buf );
	   return;
	}
    }

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }

    /*Lets see who is doing what? -Ferric*/
    strcpy( buf, argument);
    sprintf(last_command_who,     "%s @ %s",
		ch->name,
		(!IS_NPC(ch) ? ch->desc->ipaddy : "NPC") );
    sprintf(last_command_what,    "%s", buf );
    sprintf(last_command_loctime, "Room %d at %s", ch->in_room->vnum, (char *) ctime( &current_time ) );

    strcpy(cmd_copy, argument) ;

    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }


    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "" );

    if ( cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "{cLogged Command{w: %s: %s", ch->name, logline );
	wiznet(log_buf,ch,NULL,WIZ_SECURE,0,get_trust(ch));
    }

    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
	  || fLogAll
	  || cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "%s: %s", ch->name, logline );
	wiznet(log_buf,ch,NULL,WIZ_LOG,0,get_trust(ch));
	sprintf( log_buf, "PLog: %s: %s", ch->name, logline );
	log_string( log_buf );
    }

    if ( !IS_NPC(ch) )
    {
	if ( ch->desc != NULL && ch->desc->snoop_by1 != NULL )
	{
		write_to_buffer( ch->desc->snoop_by1, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by1, logline, 0 );
		write_to_buffer( ch->desc->snoop_by1, "\n\r",  2 );
	}

	if ( ch->desc != NULL && ch->desc->snoop_by2 != NULL )
	{
		write_to_buffer( ch->desc->snoop_by2, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by2, logline, 0 );
		write_to_buffer( ch->desc->snoop_by2, "\n\r",  2 );
	}

	if ( ch->desc != NULL && ch->desc->snoop_by3 != NULL )
	{
		write_to_buffer( ch->desc->snoop_by3, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by3, logline, 0 );
		write_to_buffer( ch->desc->snoop_by3, "\n\r",  2 );
	}

	if ( ch->desc != NULL && ch->desc->snoop_by4 != NULL )
	{
		write_to_buffer( ch->desc->snoop_by4, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by4, logline, 0 );
		write_to_buffer( ch->desc->snoop_by4, "\n\r",  2 );
	}

	if ( ch->desc != NULL && ch->desc->snoop_by5 != NULL )
	{
		write_to_buffer( ch->desc->snoop_by5, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by5, logline, 0 );
		write_to_buffer( ch->desc->snoop_by5, "\n\r",  2 );
	}
    }

    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	    send_to_char( "Huh?\n\r", ch );
	return;
    }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_SITTING:
	    send_to_char( "Better stand up first.\n\r",ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /* Check if the Command is Active */
    if ( !cmd_table[cmd].active )
    {
	send_to_char("That command is no longer available.\n\r", ch);
	return;
    }

    /* Check and see if a Ghost can use the command */
    if ( !IS_NPC(ch) && !IS_IMMORTAL(ch) && IS_SET(ch->info, INFO_GHOST) && !cmd_table[cmd].ghost )
    {
	send_to_char("You are a ghost, you can not do that!\n\r", ch);
	return;
    }

    /*
     * Record this as the current process
     */

    if ( !IS_NPC(ch) )
    {
	sprintf( ch->desc->curcmd, "%s", cmd_table[cmd].name );
	sprintf( ch->desc->cmdline, "%s", logline );
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

	/*
	 * Memory Debugging & Notification
	 */
	if (string_count < nAllocString) /* Strings Increased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gIncrease in strings {w:: {y%d {w: {y%d {w:: {c%s {w: {c%s {w: {c%s{w",
				string_count, nAllocString, ch->name, cmd_table[cmd].name, cmd_copy) ;
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,get_trust(ch)) ;
		sprintf(buf,
			"MEMCHECK : Increase in strings :: %d : %d :: %s : %s : %s",
				string_count, nAllocString, ch->name, cmd_table[cmd].name, cmd_copy) ;
		log_string ( buf );
	}

	if (perm_count < nAllocPerm) /* Perms Increased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gIncrease in perms {w:: {y%d {w: {y%d {w:: {c%s {w: {c%s {w: {c%s{w",
				perm_count, nAllocPerm, ch->name, cmd_table[cmd].name, cmd_copy) ;
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,get_trust(ch)) ;
		sprintf(buf,
			"MEMCHECK : Increase in perms :: %d : %d :: %s : %s : %s",
				perm_count, nAllocPerm, ch->name, cmd_table[cmd].name, cmd_copy) ;
		log_string ( buf );
	}

	if (string_count > nAllocString) /* Strings Decreased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gDecrease in strings {w:: {y%d {w: {y%d {w:: {c%s {w: {c%s {w: {c%s{w",
				string_count, nAllocString, ch->name, cmd_table[cmd].name, cmd_copy) ;
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,get_trust(ch)) ;
		sprintf(buf,
			"MEMCHECK : Decrease in strings :: %d : %d :: %s : %s : %s",
				string_count, nAllocString, ch->name, cmd_table[cmd].name, cmd_copy) ;
		log_string ( buf );
	}

	if (perm_count > nAllocPerm) /* Perms Decreased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gDecrease in perms {w:: {y%d {w: {y%d {w:: {c%s {w: {c%s {w: {c%s{w",
				perm_count, nAllocPerm, ch->name, cmd_table[cmd].name, cmd_copy) ;
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,get_trust(ch)) ;
		sprintf(buf,
			"MEMCHECK : Decrease in perms :: %d : %d :: %s : %s : %s",
				perm_count, nAllocPerm, ch->name, cmd_table[cmd].name, cmd_copy) ;
		log_string ( buf );
	}


    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim)
	&&   victim->desc == NULL)
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( char *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument(char *argument, char *arg)
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '*' )
        {
            *pdot = '\0';
            number = atoi( argument );
            *pdot = '*';
            strcpy( arg, pdot+1 );
            return number;
        }
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level <  LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch )
	&&   cmd_table[cmd].show)
	{
	    if ( !cmd_table[cmd].active )
		sprintf( buf, "{r%-12s{w", cmd_table[cmd].name );
	    else if ( !cmd_table[cmd].ghost )
		sprintf( buf, "{c%-12s{w", cmd_table[cmd].name );
	    else
		sprintf( buf, "%-12s", cmd_table[cmd].name );

	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    send_to_char("Commands is {rred{w are inactive for all players\n\r", ch);
    send_to_char("Commands in {ccyan{w are inactive for Ghosts\n\r", ch);

    return;
}

void do_wizhelp_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

    col = 0;

    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
		if ( cmd_table[cmd].level >= LEVEL_HERO
		&&   cmd_table[cmd].level <= get_trust( ch )
		&&   cmd_table[cmd].show
		&&   cmd_table[cmd].level == lev_num(ch, argument) )
		{
			if ( !cmd_table[cmd].active )
				sprintf( buf, "{r%-12s{w", cmd_table[cmd].name );
			else if ( !cmd_table[cmd].ghost )
				sprintf( buf, "{c%-12s{w", cmd_table[cmd].name );
			else
				sprintf( buf, "%-12s", cmd_table[cmd].name );

			send_to_char( buf, ch );
			if ( ++col % 3 == 0 )
				send_to_char( "\n\r     ", ch );
		}
    }

    if ( col % 3 != 0 )
	send_to_char( "\n\r     ", ch );

    return;
}

void do_wizhelp_quicklist( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];
	int cmd;
	int col;

 	col = 0;
 	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
 	{
		if ( cmd_table[cmd].level >= LEVEL_HERO && cmd_table[cmd].show)
		{
		    if ( !cmd_table[cmd].active )
			sprintf( buf, "{W[ {c%-3s {W] {r%-12s{w     ", lev_name(cmd_table[cmd].level), cmd_table[cmd].name );
		    else if ( !cmd_table[cmd].ghost )
			sprintf( buf, "{W[ {c%-3s {W] {c%-12s{w     ", lev_name(cmd_table[cmd].level), cmd_table[cmd].name );
		    else
			sprintf( buf, "{W[ {c%-3s {W] {w%-12s     ", lev_name(cmd_table[cmd].level), cmd_table[cmd].name );

		    send_to_char( buf, ch );
		    if ( ++col % 3 == 0 )
		    send_to_char( "{w\n\r", ch );
		}
	}

	if ( col % 4 != 0 )
		send_to_char( "{w\n\r", ch );
	return;
}

void do_wizhelp( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_STRING_LENGTH];
	int sw_timer;

	one_argument( argument, arg );
	sw_timer = ch->desc->swcount;

	if ( arg[0] == '\0' )
		sprintf( arg, "%s", lev_name(get_trust(ch)) );

	if ( lev_num(ch, arg) != -1 )
	{
		send_to_char("\n\r{cWizhelp{w\n\r",ch);

		if ( lev_num(ch, arg) >= LEVEL_HERO && get_trust(ch) >= LEVEL_HERO && sw_timer == 0 )
		{
			send_to_char("\n\r{gHero:{w\n\r     ",ch);
			do_wizhelp_list(ch, "HER");
		}
		if ( lev_num(ch, arg) >= AVATAR && get_trust(ch) >= AVATAR && sw_timer == 0 )
		{
			send_to_char("\n\r{gAvatar:{w\n\r     ",ch);
			do_wizhelp_list(ch, "AVA");
			if ( lev_num(ch, arg) > AVATAR )
			{
				scroll_wait( ch, "wizhelp", arg, 1, TRUE );
				return;
			}
		}
		if ( lev_num(ch, arg) >= ANGEL && get_trust(ch) >= ANGEL && sw_timer == 1 )
		{
			send_to_char("\n\r{gAngel:{w\n\r     ",ch);
			do_wizhelp_list(ch, "ANG");
		}
		if ( lev_num(ch, arg) >= DEMI && get_trust(ch) >= DEMI && sw_timer == 1 )
		{
			send_to_char("\n\r{gDemigod:{w\n\r     ",ch);
			do_wizhelp_list(ch, "DEM");
		}
		if ( lev_num(ch, arg) >= IMMORTAL && get_trust(ch) >= IMMORTAL && sw_timer == 1 )
		{
			send_to_char("\n\r{gImmortal:{w\n\r     ",ch);
			do_wizhelp_list(ch, "IMM");
			if ( lev_num(ch, arg) > IMMORTAL )
			{
				scroll_wait( ch, "wizhelp", arg, 2, TRUE );
				return;
			}
		}
		if ( lev_num(ch, arg) >= GOD && get_trust(ch) >= GOD && sw_timer == 2 )
		{
			send_to_char("\n\r{gGod:{w\n\r     ",ch);
			do_wizhelp_list(ch, "GOD");
		}
		if ( lev_num(ch, arg) >= DEITY && get_trust(ch) >= DEITY && sw_timer == 2 )
		{
			send_to_char("\n\r{gDeity:{w\n\r     ",ch);
			do_wizhelp_list(ch, "DEI");
			if ( lev_num(ch, arg) > DEITY )
			{
				scroll_wait( ch, "wizhelp", arg, 3, TRUE );
				return;
			}
		}
		if ( lev_num(ch, arg) >= SUPREME && get_trust(ch) >= SUPREME && sw_timer == 3 )
		{
			send_to_char("\n\r{gSupremacy:{w\n\r     ",ch);
			do_wizhelp_list(ch, "SUP");
		}
		if ( lev_num(ch, arg) >= CREATOR && get_trust(ch) >= CREATOR && sw_timer == 3 )
		{
			send_to_char("\n\r{gCreator:{w\n\r     ",ch);
			do_wizhelp_list(ch, "CRE");
		}
		if ( lev_num(ch, arg) >= MAX_LEVEL && get_trust(ch) >= MAX_LEVEL && sw_timer == 3 )
		{
			send_to_char("\n\r{gImplementor:{w\n\r     ",ch);
			do_wizhelp_list(ch, "IMP");
		}
		scroll_wait( ch, "", "", 0, FALSE );
		send_to_char("\n\r",ch);
		send_to_char("Commands is {rred{w are inactive for all players\n\r", ch);
		send_to_char("Commands in {ccyan{w are inactive for Ghosts\n\r", ch);
		return;
	}
	else if ( !str_prefix(arg, "all") && get_trust(ch) >= SUPREME )
	{
		do_wizhelp_quicklist( ch, "" );
		send_to_char("\n\r",ch);
		send_to_char("Commands is {rred{w are inactive for all players\n\r", ch);
		send_to_char("Commands in {ccyan{w are inactive for Ghosts\n\r", ch);
		return;
	}
	else
	{
		send_to_char("Syntax is: WIZHELP <Immortal Level>                           \n\r", ch);
		send_to_char("The Immortal level can be the level number, or                \n\r", ch);
		send_to_char("a 3 letter Immortal Level (such as HER for Hero)              \n\r", ch);
		send_to_char("to show the commands at that level, or you may                \n\r", ch);
		send_to_char("leave it blank to show all comands available to you.          \n\r", ch);
		send_to_char("NOTE: You can only see commands at or below your trust level. \n\r", ch);
		return;
	}

	return;
}


void do_cmdctrl( CHAR_DATA *ch, char *argument )
{
	char chgtype[MAX_STRING_LENGTH];
	char chgset[MAX_STRING_LENGTH];
      char buf[MAX_INPUT_LENGTH];
      char buf2[MAX_INPUT_LENGTH];
	char log_buf1[MAX_STRING_LENGTH];
	char log_buf2[MAX_STRING_LENGTH];
	char log_buf3[MAX_STRING_LENGTH];
	char command[MAX_INPUT_LENGTH];
	int cmd;
	int trust;
	int newlvl;
	int line_count;
	bool found;

	argument = one_argument( argument, command );
	argument = one_argument( argument, chgtype );
	argument = one_argument( argument, chgset  );

	if (command[0] == '\0')
	{
		send_to_char("SYNTAX: cmdctrl <command> <level|log|all|ghost> <setting>     \n\r", ch);
		send_to_char("        <command>     = command to change                     \n\r", ch);
		send_to_char("        <level>       = Level of the command                  \n\r", ch);
		send_to_char("        <log>         = Logging of the command                \n\r", ch);
		send_to_char("        <all>         = Command active for all users          \n\r", ch);
		send_to_char("        <ghost>       = Command active for ghosts             \n\r", ch);
		send_to_char("        <settings>    =                                       \n\r", ch);
		send_to_char("                         \n\r", ch);
		send_to_char("SETTINGS: \n\r", ch);
		send_to_char("          Level       :   Level number (0-500) or Imm Abbreviation   \n\r", ch);
		send_to_char("          Log         :   Always  - Log all the time                 \n\r", ch);
		send_to_char("                          Normal  - Normal log setting               \n\r", ch);
		send_to_char("                          Never   - Never Log command                \n\r", ch);
		send_to_char("          All/Ghost   :   On    - Command is active                  \n\r", ch);
		send_to_char("                          Off   - Command is inactive                \n\r", ch);
		send_to_char("                         \n\r", ch);
		send_to_char("cmdctrl xfulldump     = Displays the full command table \n\r", ch);
		return;
	}

	if ( !str_prefix( command, "xfulldump" ) )
	{
		if ( ch->desc->iswait )
		{
			line_count = 0;
			for ( cmd = ch->desc->swcount; cmd_table[cmd].name[0] != '\0'; cmd++ )
			{
				sprintf( buf, "%-12s     {c%-3s{w     {y%-10s{w     %s     %s {w\n\r",
				   cmd_table[cmd].name,
				   lev_name(cmd_table[cmd].level),
				   ( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
				   ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" )),
				   ( cmd_table[cmd].active ? "{gTrue {w" : "{rFalse{w" ),
				   ( cmd_table[cmd].ghost ? "{gTrue {w" : "{rFalse{w" ));
				send_to_char( buf, ch );

				if ( line_count++ == (ch->lines - 2) )
				{
					scroll_wait( ch, "cmdctrl", "xfulldump", cmd++, TRUE );
					return;
				}
			}
			send_to_char( "\n\r", ch);
			scroll_wait( ch, "", "", 0, FALSE );
		}
		else
		{
			send_to_char( "Command          Level   Log             Active    Ghost    \n\r", ch);
			send_to_char( "------------------------------------------------------------\n\r", ch);

			line_count = 2;
			for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
			{
				sprintf( buf, "%-12s     {c%-3s{w     {y%-10s{w     %s     %s {w\n\r",
				   cmd_table[cmd].name,
				   lev_name(cmd_table[cmd].level),
				   ( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
				   ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" )),
				   ( cmd_table[cmd].active ? "{gTrue {w" : "{rFalse{w" ),
				   ( cmd_table[cmd].ghost ? "{gTrue {w" : "{rFalse{w" ));
				send_to_char( buf, ch );

				if ( line_count++ == (ch->lines - 2) )
				{
					scroll_wait( ch, "cmdctrl", "xfulldump", cmd++, TRUE );
					return;
				}
			}
			send_to_char( "\n\r", ch);
			scroll_wait( ch, "", "", 0, FALSE );
		}
		return;
	}

	/* First find the command */

	found = FALSE;
	trust = get_trust( ch );
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( command[0] == cmd_table[cmd].name[0]
		&&   !str_prefix( command, cmd_table[cmd].name )
		&&   cmd_table[cmd].level <= trust )
		{
		    found = TRUE;
		    break;
		}
	}

	if ( !found )
	{
		send_to_char("Command not found.\n\r", ch);
		return;
	}


	/* Now see what to do to it */

	if ( !str_prefix( chgtype, "level" ) )
	{
		if ( ( newlvl = lev_num( ch, chgset ) ) == -1 )
		{
			send_to_char("Command Levels must be a number between 0-500, or\n\r", ch);
			send_to_char("a three letter Imm level abbreviation (HER-IMP).\n\r", ch);
			return;
		}

		sprintf( buf, "%s{w changed the level of {r%s{w from %d to %d.",
				ch->name,
				cmd_table[cmd].name,
				cmd_table[cmd].level,
				newlvl );
		sprintf( buf2, "Command {r%s{w level changed from %d to %d.\n\r",
				cmd_table[cmd].name,
				cmd_table[cmd].level,
				newlvl );

		sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
		sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

		cmd_table[cmd].level = newlvl;

		sprintf( log_buf3, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
	}
	else if ( !str_prefix( chgtype, "log" ) )
	{
		if ( !str_prefix( chgset, "always" ) )
		{
			sprintf( buf, "%s{w changed the logging of {r%s{w from %s to LOG_ALWAYS.",
			ch->name,
			cmd_table[cmd].name,
			( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ));

			sprintf( buf2, "Logging of {r%s{w changed from %s to LOG_ALWAYS.\n\r",
			cmd_table[cmd].name,
			( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ));

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].log = LOG_ALWAYS;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else if ( !str_prefix( chgset, "normal" ) )
		{
			sprintf( buf, "%s{w changed the logging of {r%s{w from %s to LOG_NORMAL.",
			ch->name,
			cmd_table[cmd].name,
			( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ));

			sprintf( buf2, "Logging of {r%s{w changed from %s to LOG_ALWAYS.\n\r",
			cmd_table[cmd].name,
			( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ));

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].log = LOG_NORMAL;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else if ( !str_prefix( chgset, "never" ) )
		{
			sprintf( buf, "%s{w changed the logging of {r%s{w from %s to LOG_NEVER.",
			ch->name,
			cmd_table[cmd].name,
			( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ));

			sprintf( buf2, "Logging of {r%s{w changed from %s to LOG_ALWAYS.\n\r",
			cmd_table[cmd].name,
			( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  ( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ));

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].log = LOG_NEVER;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else
		{
			send_to_char( "Valid log values are {calways{w, {cnormal{w, and {cnever{w.\n\r", ch);
			return;
		}
	}
	else if ( !str_prefix( chgtype, "all" ) )
	{
		if ( !str_prefix( chgset, "on" ) )
		{
			sprintf( buf, "%s{w activated {r%s{w for all users.", ch->name, cmd_table[cmd].name );
			sprintf( buf2, "Command {r%s{w is now active for all users.\n\r", cmd_table[cmd].name );

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].active = TRUE;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else if ( !str_prefix( chgset, "off" ) )
		{
			sprintf( buf, "%s{w deactivated {r%s{w for all users.", ch->name, cmd_table[cmd].name );
			sprintf( buf2, "Command {r%s{w is now inactive for all users.\n\r", cmd_table[cmd].name );

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].active = FALSE;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else
		{
			send_to_char( "Valid options are {con{w and {coff{w.\n\r", ch);
			return;
		}
	}
	else if ( !str_prefix( chgtype, "ghost" ) )
	{
		if ( !str_prefix( chgset, "on" ) )
		{
			sprintf( buf, "%s{w activated {r%s{w for ghosts.", ch->name, cmd_table[cmd].name );
			sprintf( buf2, "Command {r%s{w is now active for ghosts.\n\r", cmd_table[cmd].name );

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].ghost = TRUE;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else if ( !str_prefix( chgset, "off" ) )
		{
			sprintf( buf, "%s{w deactivated {r%s{w for ghosts.", ch->name, cmd_table[cmd].name );
			sprintf( buf2, "Command {r%s{w is now inactive for ghosts.\n\r", cmd_table[cmd].name );

			sprintf( log_buf1, "Command %s changed by %s", cmd_table[cmd].name, ch->name );
			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );

			cmd_table[cmd].ghost = FALSE;

			sprintf( log_buf2, "From:   Lvl: %s   Log: %-11s   Active: %-5s   Ghost: %-5s",
					lev_name(cmd_table[cmd].level),
					( cmd_table[cmd].log == LOG_NORMAL ? "LOG_NORMAL" :
			  		( cmd_table[cmd].log == LOG_ALWAYS ? "LOG_ALWAYS" : "LOG_NEVER" ) ),
					( cmd_table[cmd].active ? "TRUE" : "FALSE" ),
					( cmd_table[cmd].ghost  ? "TRUE" : "FALSE" ) );
		}
		else
		{
			send_to_char( "Valid options are {con{w and {coff{w.\n\r", ch);
			return;
		}
	}
	else
	{
		send_to_char("SYNTAX: cmdctrl <command> <level|log|all|ghost> <setting>     \n\r", ch);
		send_to_char("        <command>     = command to change                     \n\r", ch);
		send_to_char("        <level>       = Level of the command                  \n\r", ch);
		send_to_char("        <log>         = Logging of the command                \n\r", ch);
		send_to_char("        <all>         = Command active for all users          \n\r", ch);
		send_to_char("        <ghost>       = Command active for ghosts             \n\r", ch);
		send_to_char("        <settings>    =                                       \n\r", ch);
		send_to_char("                         \n\r", ch);
		send_to_char("SETTINGS: \n\r", ch);
		send_to_char("          Level       :   Level number (0-500) or Imm Abbreviation   \n\r", ch);
		send_to_char("          Log         :   Always  - Log all the time                 \n\r", ch);
		send_to_char("                          Normal  - Normal log setting               \n\r", ch);
		send_to_char("                          Never   - Never Log command                \n\r", ch);
		send_to_char("          All/Ghost   :   On    - Command is active                  \n\r", ch);
		send_to_char("                          Off   - Command is inactive                \n\r", ch);
		send_to_char("                         \n\r", ch);
		send_to_char("cmdctrl xfulldump     = Displays the full command table \n\r", ch);
		return;
	}

    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	send_to_char(buf2, ch);

	log_string( log_buf1 );
	log_string( log_buf2 );
	log_string( log_buf3 );

	return;
}


void do_olclock( CHAR_DATA *ch, char *argument )
{
    extern bool olclock;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    int cmd;

    olclock = !olclock;

    if ( olclock )
    {
	wiznet("OLC has been locked.",NULL,NULL,0,0,0);
	send_to_char( "OLC has been locked.\n\r", ch );
	sprintf( buf, "%s has locked OLC", ch->name );
	log_string( buf );
	log_string( "GLOBAL VARIABLE : olclock set to TRUE" );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
		if ( d->connected == CON_PLAYING && IS_BUILDER(d->character) )
		{
			send_to_char( "OLC is now locked.  Please exit all editors.\n\r", d->character );
		}
	}
	/* Lock each OLC Command */
	/* AEdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "aedit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w deactivated {r%s{w for all users by OLC Lock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = FALSE;

	/* REdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "redit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w deactivated {r%s{w for all users by OLC Lock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = FALSE;

	/* MEdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "medit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w deactivated {r%s{w for all users by OLC Lock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = FALSE;

	/* OEdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "oedit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w deactivated {r%s{w for all users by OLC Lock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = FALSE;

	/* RESET */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "reset", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w deactivated {r%s{w for all users by OLC Lock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = FALSE;
    }
    else
    {
	wiznet("OLC has been unlocked.",NULL,NULL,0,0,0);
	send_to_char( "OLC has been unlocked.\n\r", ch );
	sprintf( buf, "%s has unlocked OLC", ch->name );
	log_string( buf );
	log_string( "GLOBAL VARIABLE : olclock set to FALSE" );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
		if ( d->connected == CON_PLAYING && IS_BUILDER(d->character) )
		{
			send_to_char( "OLC is now unlocked.  You may resume building.\n\r", d->character );
		}
	}

	/* Unlock each OLC Command */
	/* AEdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "aedit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w activated {r%s{w for all users by OLC Unlock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = TRUE;

	/* REdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "redit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w activated {r%s{w for all users by OLC Unlock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = TRUE;

	/* MEdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "medit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w activated {r%s{w for all users by OLC Unlock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = TRUE;

	/* OEdit */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "oedit", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w activated {r%s{w for all users by OLC Unlock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = TRUE;

	/* RESET */
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( !str_prefix( "reset", cmd_table[cmd].name ) )
		{
		    break;
		}
	}
	sprintf( buf, "%s{w activated {r%s{w for all users by OLC Unlock.", ch->name, cmd_table[cmd].name );
    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	cmd_table[cmd].active = TRUE;
    }

    return;
}


void scroll_wait( CHAR_DATA *ch, char *dofun, char *argument, int count, bool startwait )
{
	if ( startwait )
	{
		send_to_char( "\n\r[ Press ENTER to Continue... ]\n\r", ch );
		ch->desc->connected = CON_SCROLL_WAIT;
		ch->desc->iswait = TRUE;
		sprintf( ch->desc->swcmd, "%s %s", dofun, argument );
		ch->desc->swcount = count;
	}
	else
	{
		ch->desc->connected = CON_PLAYING;
		ch->desc->iswait = FALSE;
		strcpy( ch->desc->swcmd, "" );
		ch->desc->swcount = 0;
	}
	return;
}


void do_process( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *vch;
    DESCRIPTOR_DATA *d;
    char            buf  [ MAX_STRING_LENGTH ];
    char            buf2 [ MAX_STRING_LENGTH ];
    int             count;
    char            s[100];
    char            idle[10];

    count       = 0;
    buf[0]      = '\0';
    buf2[0]     = '\0';

    strcat( buf2, "\n\r[ ID Login@  Idle ] Player Name   Current Command Line\n\r" );
    strcat( buf2,
"{b-----------------------------------------------------------------------------------------\n\r");
    for ( d = descriptor_list; d; d = d->next )
    {
        if ( d->character != NULL && can_see( ch, d->character ) )
        {
           count++;

           /* Format "login" value... */
           vch = d->original ? d->original : d->character;
           strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );

           if ( vch->timer > 0 )
              sprintf( idle, "%4d", vch->timer );
           else
              sprintf( idle, "    " );

	     if ( get_trust(ch) > get_trust(d->character) )
           {
	           sprintf( buf, "{W[{g%3d {D%7s {y%s{W ]{c %-12s {w %-10s  :  %-40s \n\r",
	              d->descriptor,
	              s,
	              idle,
	              ( d->original ) ? d->original->name
	                              : ( d->character )  ? d->character->name : "{R({rNone!{R){w",
			  d->curcmd,
			  d->cmdline	);
	     }
	     else if ( get_trust(ch) == get_trust(d->character) )
	     {
	           sprintf( buf, "{W[{g%3d {D%7s {y%s{W ]{c %-12s {w %-10s  :  %-40s \n\r",
	              d->descriptor,
	              s,
	              idle,
	              ( d->original ) ? d->original->name
	                              : ( d->character )  ? d->character->name : "{R({rNone!{R){w",
			  d->curcmd,
			  "{R({rNo Data{R){w"	);
	     }
	     else
	     {
	           sprintf( buf, "{W[{g%3d {D%7s {y%s{W ]{c %-12s {w %-10s  :  %-40s \n\r",
	              d->descriptor,
	              s,
	              idle,
	              ( d->original ) ? d->original->name
	                              : ( d->character )  ? d->character->name : "{R({rNone!{R){w",
			  "{R({rNo Data{R){w",
			  "{R({rNo Data{R){w"	);
	     }

           strcat( buf2, buf );

        }
    }

    sprintf( buf, "\n\r%d user%s\n\r", count, count == 1 ? "" : "s" );
    strcat( buf2, buf );
    send_to_char( buf2, ch );
    return;
}

/*
 * wizhelp and advance need to be modified for the ICF flags
 */

