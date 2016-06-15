/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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
#include "merc.h"

#ifndef GRAYSON_TABLES_H
#define GRAYSON_TABLES_H

struct clan_type
{
    char 	*name;
    char 	*who_name;
    sh_int 	hall;
    bool	independent; 	/* true for loners */
    bool	active;		/* true is clan is active */
    bool	pksafe;		/* true if clan is PKSafe */
    bool	visible;		/* true if clan is visible */
};

struct tip_type
{
    char	*newbie;
    char	*beginner;
    char	*intermediate;
    char	*remort;
    char	*experienced;
    char	*advanced;
};

struct economy_type
{
    char		*name;
    char		*description;
    long		value;
};

struct combatmod_type
{
    char		*name;
    char		*description;
    long		value;
};

struct position_type
{
    char *name;
    char *short_name;
};

struct sex_type
{
    char *name;
};

struct size_type
{
    char *name;
};

struct censor_type
{
    char		*word;
};

struct insult_type
{
    char		*insult;
};


/* game tables */
extern		struct	clan_type		clan_table[MAX_CLAN];
extern	const	struct	tip_type		tip_table[MAX_TIPS];
extern		struct	economy_type	economy_table[MAX_ECON];
extern      struct  combatmod_type  combatmod_table[MAX_COMBAT];
extern		struct	censor_type		censor_table[MAX_CENSOR];
extern		struct	insult_type		insult_table[MAX_INSULTS];
extern	const	struct	position_type	position_table[];
extern	const	struct	sex_type		sex_table[];
extern	const	struct	size_type		size_table[];

/* flag tables */
extern	const	struct	flag_type	act_flags[];
extern	const	struct	flag_type	plr_flags[];
extern	const	struct	flag_type	affect_flags[];
extern	const	struct	flag_type	off_flags[];
extern	const	struct	flag_type	imm_flags[];
extern	const	struct	flag_type	form_flags[];
extern	const	struct	flag_type	part_flags[];
extern	const	struct	flag_type	cyber_flags[];
extern	const	struct	flag_type	comm_flags[];
extern	const	struct	flag_type	info_flags[];
extern	const	struct	flag_type	extra_flags[];
extern	const	struct	flag_type	wear_flags[];
extern	const	struct	flag_type	weapon_flags[];
extern	const	struct	flag_type	container_flags[];
extern	const	struct	flag_type	portal_flags[];
extern	const	struct	flag_type	room_flags[];
extern	const	struct	flag_type	exit_flags[];
extern	const	struct	flag_type	olcs_flags[];
extern	const struct	flag_type	icf_her_flags[];
extern	const struct	flag_type	icf_stat_flags[];
extern	const struct	flag_type	icf_ang_flags[];
extern	const struct	flag_type	icf_sup_flags[];





#endif
