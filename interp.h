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
*       ROM 2.4 is copyright 1993-1996 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@efn.org)                                  *
*           Gabrielle Taylor                                               *
*           Brian Moore (zump@rom.org)                                     *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Rom24/doc/rom.license                     *
***************************************************************************/

/* this is a listing of all the commands and command related data */

/* for command types */
#define ML      MAX_LEVEL		/* implementor */
#define L1      CREATOR			/* creator */
#define L2      SUPREME			/* supreme being */
#define L3      DEITY			/* deity */
#define L4      GOD			/* god */
#define L5      IMMORTAL		/* immortal */
#define L6      DEMI			/* demigod */
#define L7      ANGEL			/* angel */
#define L8      AVATAR			/* avatar */
#define IM      ANGEL			/* angel */
#define HE      LEVEL_HERO		/* hero */

#define BD		1			/* Builder */

#define COM_INGORE      1


/*
 * Structure for a command in the command lookup table.
 */
struct  cmd_type
{
    char * const        name;
    DO_FUN *            do_fun;
    sh_int              position;
    sh_int              level;
    sh_int              log;
    sh_int              show;
    bool			active;	/* Is the command on-line */
    bool			ghost;	/* Can a Ghost use it */
};

/* the command table itself (read only)
extern  const   struct  cmd_type        cmd_table       []; */

/* the command table itself (changeable) */
extern          struct  cmd_type        cmd_table       [];

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN( do_addlag       );
DECLARE_DO_FUN( do_advance      );
DECLARE_DO_FUN( do_affects      );
DECLARE_DO_FUN( do_afk          );
DECLARE_DO_FUN( do_alia         );
DECLARE_DO_FUN( do_alias        );
DECLARE_DO_FUN( do_allow        );
DECLARE_DO_FUN( do_angtalk      );
DECLARE_DO_FUN( do_answer       );
DECLARE_DO_FUN( do_areas        );
DECLARE_DO_FUN( do_at           );
DECLARE_DO_FUN( do_auction      );
DECLARE_DO_FUN( do_autoassist   );
DECLARE_DO_FUN( do_autoexit     );
DECLARE_DO_FUN( do_autogold     );
DECLARE_DO_FUN( do_autolist     );
DECLARE_DO_FUN( do_autoloot     );
DECLARE_DO_FUN( do_autosac      );
DECLARE_DO_FUN( do_autosplit    );
DECLARE_DO_FUN( do_autowizi     );
DECLARE_DO_FUN( do_backstab     );
DECLARE_DO_FUN( do_bamfin       );
DECLARE_DO_FUN( do_bamfout      );
DECLARE_DO_FUN( do_ban          );
DECLARE_DO_FUN( do_bash         );
DECLARE_DO_FUN( do_berserk      );
DECLARE_DO_FUN( do_bonus        );
DECLARE_DO_FUN( do_bounty       );
DECLARE_DO_FUN( do_brandish     );
DECLARE_DO_FUN( do_brief        );
DECLARE_DO_FUN( do_bug          );
DECLARE_DO_FUN( do_buy          );
DECLARE_DO_FUN( do_cast         );
DECLARE_DO_FUN( do_activate     );
DECLARE_DO_FUN( do_changes      );
DECLARE_DO_FUN( do_channels     );
DECLARE_DO_FUN( do_chkdupip     );
DECLARE_DO_FUN( do_circlestab   );
DECLARE_DO_FUN( do_clanadmin    );
DECLARE_DO_FUN( do_clan_recall  );  /* Froth of Dyre MUD */
DECLARE_DO_FUN( do_clear        );
DECLARE_DO_FUN( do_clone        );
DECLARE_DO_FUN( do_close        );
DECLARE_DO_FUN( do_cmdctrl      );
DECLARE_DO_FUN( do_codertalk    );	    /* Storm */
DECLARE_DO_FUN( do_colour       );      /* Colour Command By Lope */
DECLARE_DO_FUN( do_commands     );
DECLARE_DO_FUN( do_combine      );
DECLARE_DO_FUN( do_compact      );
DECLARE_DO_FUN( do_compare      );
DECLARE_DO_FUN( do_consider     );
DECLARE_DO_FUN( do_count        );
DECLARE_DO_FUN( do_crash        );
DECLARE_DO_FUN( do_credits      );
DECLARE_DO_FUN(	do_cyber	);
DECLARE_DO_FUN( do_deaf         );
DECLARE_DO_FUN( do_delet        );
DECLARE_DO_FUN( do_delete       );
DECLARE_DO_FUN( do_deny         );
DECLARE_DO_FUN( do_description  );
DECLARE_DO_FUN( do_dirt         );
DECLARE_DO_FUN( do_disarm       );
DECLARE_DO_FUN( do_disconnect   );
DECLARE_DO_FUN( do_down         );
DECLARE_DO_FUN( do_drag         );
DECLARE_DO_FUN( do_drink        );
DECLARE_DO_FUN( do_drop         );
DECLARE_DO_FUN( do_dump         );
DECLARE_DO_FUN( do_east         );
DECLARE_DO_FUN( do_eat          );
DECLARE_DO_FUN( do_echo         );
DECLARE_DO_FUN( do_emote        );
DECLARE_DO_FUN( do_enter        );
DECLARE_DO_FUN( do_envenom      );
DECLARE_DO_FUN( do_equipment    );
DECLARE_DO_FUN( do_examine      );
DECLARE_DO_FUN( do_exits        );
DECLARE_DO_FUN( do_fill         );
DECLARE_DO_FUN( do_finger       );
DECLARE_DO_FUN( do_flag         );
DECLARE_DO_FUN( do_flee         );
DECLARE_DO_FUN( do_follow       );
DECLARE_DO_FUN( do_force        );
DECLARE_DO_FUN( do_freeze       );
DECLARE_DO_FUN( do_fury      	  );
DECLARE_DO_FUN( do_destruction  );
DECLARE_DO_FUN( do_gain         );
DECLARE_DO_FUN( do_get          );
DECLARE_DO_FUN( do_give         );
DECLARE_DO_FUN( do_gossip       );
DECLARE_DO_FUN( do_hack         );
DECLARE_DO_FUN( do_flamechan    );
DECLARE_DO_FUN( do_olclock      );
DECLARE_DO_FUN( do_oocchan      );
DECLARE_DO_FUN( do_newbietalk   );
DECLARE_DO_FUN( do_nuke         );
DECLARE_DO_FUN( do_goto         );
DECLARE_DO_FUN( do_grats        );
DECLARE_DO_FUN( do_group        );
DECLARE_DO_FUN( do_groups       );
DECLARE_DO_FUN( do_gtell        );
DECLARE_DO_FUN( do_guild        );
DECLARE_DO_FUN( do_heal         );
DECLARE_DO_FUN( do_help         );
DECLARE_DO_FUN( do_helpertalk   );
DECLARE_DO_FUN( do_hide         );
DECLARE_DO_FUN( do_holylight    );
DECLARE_DO_FUN( do_idea         );
DECLARE_DO_FUN( do_immtalk      );
DECLARE_DO_FUN( do_ipaddress    );
DECLARE_DO_FUN( do_suptalk      );
DECLARE_DO_FUN( do_suicid       );
DECLARE_DO_FUN( do_suicide      );
DECLARE_DO_FUN( do_buildtalk    );
DECLARE_DO_FUN( do_incognito    );
DECLARE_DO_FUN( do_clantalk     );
DECLARE_DO_FUN( do_clanlist     );
DECLARE_DO_FUN( do_clanrequest  );
DECLARE_DO_FUN( do_imotd        );
DECLARE_DO_FUN( do_info         );
DECLARE_DO_FUN( do_inventory    );
DECLARE_DO_FUN( do_invis        );
DECLARE_DO_FUN( do_kick         );
DECLARE_DO_FUN( do_spin         );
DECLARE_DO_FUN( do_gouge        );
DECLARE_DO_FUN( do_buffet       );
DECLARE_DO_FUN( do_sting	);
DECLARE_DO_FUN( do_drain	);
DECLARE_DO_FUN( do_sweep	);
DECLARE_DO_FUN( do_poetry	);

DECLARE_DO_FUN( do_kill         );
DECLARE_DO_FUN( do_list         );
DECLARE_DO_FUN( do_load         );
DECLARE_DO_FUN( do_lock         );
DECLARE_DO_FUN( do_log          );
DECLARE_DO_FUN( do_look         );
DECLARE_DO_FUN( do_memory       );
DECLARE_DO_FUN( do_mfind        );
DECLARE_DO_FUN( do_mload        );
DECLARE_DO_FUN( do_mset         );
DECLARE_DO_FUN( do_mstat        );
DECLARE_DO_FUN( do_mwhere       );
DECLARE_DO_FUN( do_motd         );
DECLARE_DO_FUN( do_murde        );
DECLARE_DO_FUN( do_murder       );
DECLARE_DO_FUN( do_music        );
DECLARE_DO_FUN( do_newlock      );
DECLARE_DO_FUN( do_news         );
DECLARE_DO_FUN( do_nobounty     );
DECLARE_DO_FUN( do_nochannels   );
DECLARE_DO_FUN( do_noemote      );
DECLARE_DO_FUN( do_nofollow     );
DECLARE_DO_FUN( do_noloot       );
DECLARE_DO_FUN( do_north        );
DECLARE_DO_FUN( do_noshout      );
DECLARE_DO_FUN( do_nosummon     );
DECLARE_DO_FUN( do_note         );
DECLARE_DO_FUN( do_notell       );
DECLARE_DO_FUN( do_ofind        );
DECLARE_DO_FUN( do_oload        );
DECLARE_DO_FUN( do_open         );
DECLARE_DO_FUN( do_order        );
DECLARE_DO_FUN( do_oset         );
DECLARE_DO_FUN( do_ostat        );
DECLARE_DO_FUN( do_outfit       );
DECLARE_DO_FUN( do_owhere       );
DECLARE_DO_FUN( do_pardon       );
DECLARE_DO_FUN( do_process      );
DECLARE_DO_FUN( do_password     );
DECLARE_DO_FUN( do_peace        );
DECLARE_DO_FUN( do_pecho        );
DECLARE_DO_FUN( do_penalty      );
DECLARE_DO_FUN( do_permban      );
DECLARE_DO_FUN( do_pick         );
DECLARE_DO_FUN( do_play         );
DECLARE_DO_FUN( do_pmote        );
DECLARE_DO_FUN( do_pose         );
DECLARE_DO_FUN( do_pour         );
DECLARE_DO_FUN( do_practice     );
DECLARE_DO_FUN( do_pray         );
DECLARE_DO_FUN( do_prefi        );
DECLARE_DO_FUN( do_prefix       );
DECLARE_DO_FUN( do_prompt       );
DECLARE_DO_FUN( do_protect      );
DECLARE_DO_FUN( do_purge        );
DECLARE_DO_FUN( do_put          );
DECLARE_DO_FUN( do_pload        );
DECLARE_DO_FUN( do_punload      );
DECLARE_DO_FUN( do_quaff        );
DECLARE_DO_FUN( do_quest        );
DECLARE_DO_FUN( do_question     );
DECLARE_DO_FUN( do_qui          );
DECLARE_DO_FUN( do_quiet        );
DECLARE_DO_FUN( do_quit         );
DECLARE_DO_FUN( do_quote        );
DECLARE_DO_FUN( do_read         );
DECLARE_DO_FUN( do_reanimate    );
DECLARE_DO_FUN( do_reboo        );
DECLARE_DO_FUN( do_reboot       );
DECLARE_DO_FUN( do_recall       );
DECLARE_DO_FUN( do_recho        );
DECLARE_DO_FUN( do_recite       );
DECLARE_DO_FUN( do_remor        );
DECLARE_DO_FUN( do_remort       );
DECLARE_DO_FUN( do_remove       );
DECLARE_DO_FUN( do_rent         );
DECLARE_DO_FUN( do_replay       );
DECLARE_DO_FUN( do_reply        );
DECLARE_DO_FUN( do_report       );
DECLARE_DO_FUN( do_rescue       );
DECLARE_DO_FUN( do_rest         );
DECLARE_DO_FUN( do_restore      );
DECLARE_DO_FUN( do_restring     );
DECLARE_DO_FUN( do_return       );
DECLARE_DO_FUN( do_roomin       );
DECLARE_DO_FUN( do_roomout      );
DECLARE_DO_FUN( do_rset         );
DECLARE_DO_FUN( do_rstat        );
DECLARE_DO_FUN( do_rules        );
DECLARE_DO_FUN( do_sacrifice    );
DECLARE_DO_FUN( do_save         );
DECLARE_DO_FUN( do_say          );
DECLARE_DO_FUN( do_score        );
DECLARE_DO_FUN( do_scroll       );
DECLARE_DO_FUN( do_sell         );
DECLARE_DO_FUN( do_set          );
DECLARE_DO_FUN( do_shout        );
DECLARE_DO_FUN( do_show         );
DECLARE_DO_FUN( do_showtips     );
DECLARE_DO_FUN( do_shutdow      );
DECLARE_DO_FUN( do_shutdown     );
DECLARE_DO_FUN( do_sit          );
DECLARE_DO_FUN( do_skills       );
DECLARE_DO_FUN( do_sla          );
DECLARE_DO_FUN( do_slay         );
DECLARE_DO_FUN( do_sleep        );
DECLARE_DO_FUN( do_trance        );
DECLARE_DO_FUN( do_slookup      );
DECLARE_DO_FUN( do_smote        );
DECLARE_DO_FUN( do_sneak        );
DECLARE_DO_FUN( do_snoop        );
DECLARE_DO_FUN( do_snooplist    );
DECLARE_DO_FUN( do_socials      );
DECLARE_DO_FUN( do_south        );
DECLARE_DO_FUN( do_sockets      );
DECLARE_DO_FUN( do_spells       );
DECLARE_DO_FUN( do_split        );
DECLARE_DO_FUN( do_sset         );
DECLARE_DO_FUN( do_stand        );
DECLARE_DO_FUN( do_stat         );
DECLARE_DO_FUN( do_steal        );
DECLARE_DO_FUN( do_story        );
DECLARE_DO_FUN( do_string       );
DECLARE_DO_FUN( do_switch       );
DECLARE_DO_FUN( do_tabledump    );
DECLARE_DO_FUN( do_tell         );
DECLARE_DO_FUN( do_time         );
DECLARE_DO_FUN( do_tips         );
DECLARE_DO_FUN( do_title        );
DECLARE_DO_FUN( do_train        );
DECLARE_DO_FUN( do_transfer     );
DECLARE_DO_FUN( do_trip         );
DECLARE_DO_FUN( do_trust        );
DECLARE_DO_FUN( do_typo         );
DECLARE_DO_FUN( do_unalias      );
DECLARE_DO_FUN( do_unlock       );
DECLARE_DO_FUN( do_unlock_spool );
DECLARE_DO_FUN( do_unlock_db    );
DECLARE_DO_FUN( do_unread       );
DECLARE_DO_FUN( do_up           );
DECLARE_DO_FUN( do_value        );
DECLARE_DO_FUN( do_visible      );
DECLARE_DO_FUN( do_violate      );
DECLARE_DO_FUN( do_vnum         );
DECLARE_DO_FUN( do_wake         );
DECLARE_DO_FUN( do_wear         );
DECLARE_DO_FUN( do_weather      );
DECLARE_DO_FUN( do_west         );
DECLARE_DO_FUN( do_where        );
DECLARE_DO_FUN( do_who          );
DECLARE_DO_FUN( do_whois        );
DECLARE_DO_FUN( do_wimpy        );
DECLARE_DO_FUN( do_wizhelp      );
DECLARE_DO_FUN( do_wiziin		); /* Ferg-wizicust */
DECLARE_DO_FUN( do_wiziout		); /* Ferg-wizicust */
DECLARE_DO_FUN( do_wizlock      );
DECLARE_DO_FUN( do_wizlist      );
DECLARE_DO_FUN( do_wiznet       );
DECLARE_DO_FUN( do_worth        );
DECLARE_DO_FUN( do_yell         );
DECLARE_DO_FUN( do_zap          );
DECLARE_DO_FUN( do_zecho        );
DECLARE_DO_FUN( do_version      );
DECLARE_DO_FUN( do_trustlist    );
DECLARE_DO_FUN( do_olc          );
DECLARE_DO_FUN( do_asave        );
DECLARE_DO_FUN( do_alist        );
DECLARE_DO_FUN( do_resets       );
DECLARE_DO_FUN( do_redit        );
DECLARE_DO_FUN( do_aedit        );
DECLARE_DO_FUN( do_medit        );
DECLARE_DO_FUN( do_oedit        );
DECLARE_DO_FUN( do_second	  );
DECLARE_DO_FUN( do_balance      );
DECLARE_DO_FUN( do_deposit      );
DECLARE_DO_FUN( do_withdraw     );
DECLARE_DO_FUN( do_lockstat     );
DECLARE_DO_FUN( do_betalock     );
DECLARE_DO_FUN( do_totalwar     );
DECLARE_DO_FUN( do_wiretransfer );
DECLARE_DO_FUN( do_immnote      );
DECLARE_DO_FUN( do_rpnote       );
DECLARE_DO_FUN( do_clannote     );
DECLARE_DO_FUN( do_autorestart  );
DECLARE_DO_FUN( do_slist        );
DECLARE_DO_FUN( do_soundon      );
DECLARE_DO_FUN( do_arealinks    );
DECLARE_DO_FUN( do_jail         );
DECLARE_DO_FUN( do_release      );
DECLARE_DO_FUN( do_toss		    );
DECLARE_DO_FUN( do_assemble     );
DECLARE_DO_FUN( do_turn_vampire );
DECLARE_DO_FUN( do_reject_vampire);
DECLARE_DO_FUN( do_accept_vampire);
DECLARE_DO_FUN( do_trance	    );
DECLARE_DO_FUN( do_stun		    );
DECLARE_DO_FUN( do_power_attack );
DECLARE_DO_FUN( do_stake	    );
DECLARE_DO_FUN( do_forcepower	    );
