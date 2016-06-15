/**************************************************************************r
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
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "handler.h"

#ifndef GRAYSON_HANDLER_C
#define GRAYSON_HANDLER_C

/* command procedures needed */
//DECLARE_DO_FUN(do_return	);



/*
 * Local functions.
 */
/* moved to header
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
bool	is_ploaded 		args( ( CHAR_DATA *ch ) );
bool	is_linkdead 	args( ( CHAR_DATA *ch ) );
*/

/* friend stuff -- for NPC's mostly */
bool is_friend(CHAR_DATA *ch,CHAR_DATA *victim)
{
    if (is_same_group(ch,victim))
	return TRUE;


    if (!IS_NPC(ch))
	return FALSE;

    if (!IS_NPC(victim))
    {
	if (IS_SET(ch->off_flags,ASSIST_PLAYERS))
	    return TRUE;
	else
	    return FALSE;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if (IS_SET(ch->off_flags,ASSIST_ALL))
	return TRUE;

    if (ch->group && ch->group == victim->group)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_VNUM)
    &&  ch->pIndexData == victim->pIndexData)

	return TRUE;



    if (IS_SET(ch->off_flags,ASSIST_RACE) && ch->race == victim->race)

	return TRUE;



    if (IS_SET(ch->off_flags,ASSIST_ALIGN)

    &&  !IS_SET(ch->act,ACT_NOALIGN) && !IS_SET(victim->act,ACT_NOALIGN)

    &&  ((IS_GOOD(ch) && IS_GOOD(victim))

    ||	 (IS_EVIL(ch) && IS_EVIL(victim))

    ||   (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))))

	return TRUE;

    return FALSE;

}



/* returns number of people on an object */

int count_users(OBJ_DATA *obj)
{
    CHAR_DATA *fch;
    int count = 0;

    if (obj->in_room == NULL)
	return 0;

    for (fch = obj->in_room->people; fch != NULL; fch = fch->next_in_room)
	if (fch->on == obj)
	    count++;

    return count;
}


/* returns material number */
int material_lookup (const char *name)
{
    return 0;
}

/* returns race number */
int race_lookup (const char *name)
{
   int race;

   for ( race = 0; race_table[race].name != NULL; race++)
   {
	if (LOWER(name[0]) == LOWER(race_table[race].name[0])
	&&  !str_prefix( name,race_table[race].name))
	    return race;
   }

   return 0;
}


int liq_lookup (const char *name)

{

    int liq;



    for ( liq = 0; liq_table[liq].liq_name != NULL; liq++)

    {

	if (LOWER(name[0]) == LOWER(liq_table[liq].liq_name[0])

	&& !str_prefix(name,liq_table[liq].liq_name))

	    return liq;

    }



    return -1;

}



int weapon_lookup (const char *name)

{

    int type;



    for (type = 0; weapon_table[type].name != NULL; type++)

    {

	if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])

	&&  !str_prefix(name,weapon_table[type].name))

	    return type;

    }



    return -1;

}



int weapon_type (const char *name)

{

    int type;



    for (type = 0; weapon_table[type].name != NULL; type++)

    {

        if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])

        &&  !str_prefix(name,weapon_table[type].name))

            return weapon_table[type].type;

    }



    return WEAPON_EXOTIC;

}





int item_lookup(const char *name)

{

    int type;



    for (type = 0; item_table[type].name != NULL; type++)

    {

        if (LOWER(name[0]) == LOWER(item_table[type].name[0])

        &&  !str_prefix(name,item_table[type].name))

            return item_table[type].type;

    }



    return -1;

}



char *item_name(int item_type)

{

    int type;



    for (type = 0; item_table[type].name != NULL; type++)

	if (item_type == item_table[type].type)

	    return item_table[type].name;

    return "none";

}



char *weapon_name( int weapon_type)

{

    int type;



    for (type = 0; weapon_table[type].name != NULL; type++)

        if (weapon_type == weapon_table[type].type)

            return weapon_table[type].name;

    return "exotic";

}



int attack_lookup  (const char *name)

{

    int att;



    for ( att = 0; attack_table[att].name != NULL; att++)

    {

	if (LOWER(name[0]) == LOWER(attack_table[att].name[0])

	&&  !str_prefix(name,attack_table[att].name))

	    return att;

    }



    return 0;

}



/* returns a flag for wiznet */

long wiznet_lookup (const char *name)

{

    int flag;



    for (flag = 0; wiznet_table[flag].name != NULL; flag++)

    {

	if (LOWER(name[0]) == LOWER(wiznet_table[flag].name[0])

	&& !str_prefix(name,wiznet_table[flag].name))

	    return flag;

    }



    return -1;

}



/* returns class number */

int class_lookup (const char *name)

{

   int class;



   for ( class = 0; class_table[class].name != NULL; class++ )

   {

        if (LOWER(name[0]) == LOWER(class_table[class].name[0])

        &&  !str_prefix( name,class_table[class].name))

            return class;

   }



   return -1;

}



/* for immunity, vulnerabiltiy, and resistant

   the 'globals' (magic and weapons) may be overriden

   three other cases -- wood, silver, and iron -- are checked in fight.c */



int check_immune(CHAR_DATA *ch, int dam_type)

{

    int immune, def;

    int bit;

    char wnetbuf[MAX_STRING_LENGTH];



    immune = -1;

    def = IS_NORMAL;



    if (dam_type == DAM_NONE)

	return immune;



    if (dam_type <= 3)

    {

	if (IS_SET(ch->imm_flags,IMM_WEAPON))

	    def = IS_IMMUNE;

	else if (IS_SET(ch->res_flags,RES_WEAPON))

	    def = IS_RESISTANT;

	else if (IS_SET(ch->vuln_flags,VULN_WEAPON))

	    def = IS_VULNERABLE;

    }

    else /* magical attack */

    {

	if (IS_SET(ch->imm_flags,IMM_MAGIC))

	    def = IS_IMMUNE;

	else if (IS_SET(ch->res_flags,RES_MAGIC))

	    def = IS_RESISTANT;

	else if (IS_SET(ch->vuln_flags,VULN_MAGIC))

	    def = IS_VULNERABLE;

    }



    /* set bits to check -- VULN etc. must ALL be the same or this will fail */

    switch (dam_type)
    {
	case(DAM_BASH):		bit = IMM_BASH;		break;
	case(DAM_PIERCE):	bit = IMM_PIERCE;	break;
	case(DAM_SLASH):	bit = IMM_SLASH;	break;
	case(DAM_FIRE):		bit = IMM_FIRE;		break;
	case(DAM_COLD):		bit = IMM_COLD;		break;
	case(DAM_LIGHTNING):	bit = IMM_LIGHTNING;	break;
	case(DAM_ACID):		bit = IMM_ACID;		break;
	case(DAM_POISON):	bit = IMM_POISON;	break;
	case(DAM_NEGATIVE):	bit = IMM_NEGATIVE;	break;
	case(DAM_HOLY):		bit = IMM_HOLY;		break;
	case(DAM_ENERGY):	bit = IMM_ENERGY;	break;
	case(DAM_MENTAL):	bit = IMM_MENTAL;	break;
	case(DAM_DISEASE):	bit = IMM_DISEASE;	break;
	case(DAM_DROWNING):	bit = IMM_DROWNING;	break;
	case(DAM_LIGHT):	bit = IMM_LIGHT;	break;
	case(DAM_CHARM):	bit = IMM_CHARM;	break;
	case(DAM_SOUND):	bit = IMM_SOUND;	break;
	default:		return def;
    }



    if (IS_SET(ch->imm_flags,bit))

	immune = IS_IMMUNE;

    else if (IS_SET(ch->res_flags,bit) && immune != IS_IMMUNE)

	immune = IS_RESISTANT;

    else if (IS_SET(ch->vuln_flags,bit))

    {

	if (immune == IS_IMMUNE)

	    immune = IS_RESISTANT;

	else if (immune == IS_RESISTANT)

	    immune = IS_NORMAL;

	else

	    immune = IS_VULNERABLE;

    }



	if ( !IS_NPC(ch) )

	{

		if ( IS_HERO(ch) && ( immune == IS_NORMAL || immune == IS_VULNERABLE ) )

			immune = IS_RESISTANT;



		if ( IS_IMMORTAL(ch) )

			immune = IS_IMMUNE;



		if ( IS_SET(ch->info, INFO_GHOST) )

			immune = IS_IMMUNE;

	}



    if (immune == -1)

	return def;

    else

      return immune;

}



bool is_clan(CHAR_DATA *ch)

{

    return ch->clan;

}



bool is_same_clan(CHAR_DATA *ch, CHAR_DATA *victim)

{

    if (clan_table[ch->clan].independent)

	return FALSE;

    else

	return (ch->clan == victim->clan);

}



/* checks mob format */

bool is_old_mob(CHAR_DATA *ch)

{

    if (ch->pIndexData == NULL)

	return FALSE;

    else if (ch->pIndexData->new_format)

	return FALSE;

    return TRUE;

}



/* for returning skill information */

int get_skill(CHAR_DATA *ch, int sn)

{

    int skill;



    if (sn == -1) /* shorthand for level based skills */

    {

	skill = ch->level * 5 / 2;

    }



    else if (sn < -1 || sn > MAX_SKILL)

    {

	bug("Bad sn %d in get_skill.",sn);

	skill = 0;

    }



    else if (!IS_NPC(ch))

    {

	if ( can_use_skill(ch, sn) )

	    skill = ch->pcdata->learned[sn];

	else

	    skill = 0;

    }



    else /* mobiles */

    {

        if (skill_table[sn].spell_fun != spell_null)

	    skill = 40 + 2 * ch->level;



	else if (sn == gsn_sneak || sn == gsn_hide)

	    skill = ch->level * 2 + 20;



        else if ((sn == gsn_dodge && IS_SET(ch->off_flags,OFF_DODGE))

 	||       (sn == gsn_parry && IS_SET(ch->off_flags,OFF_PARRY)))

	    skill = ch->level * 2;



 	else if (sn == gsn_shield_block)

	    skill = 10 + 2 * ch->level;



	else if (sn == gsn_second_attack

	&& (IS_SET(ch->act,ACT_WARRIOR) || IS_SET(ch->act,ACT_THIEF)))

		skill = 40 + 3 * ch->level;



	else if (sn == gsn_second_attack

	&& !(IS_SET(ch->act,ACT_WARRIOR) || IS_SET(ch->act,ACT_THIEF)))

		skill = (20 + 3 * ch->level) - 120;



	else if (sn == gsn_third_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 4 * ch->level - 40;



	else if (sn == gsn_third_attack && !IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 120;



	else if (sn == gsn_fourth_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 60;



	else if (sn == gsn_fourth_attack && !IS_SET(ch->act,ACT_WARRIOR))

		skill = 2 * ch->level - 140;



	else if (sn == gsn_fifth_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 100;



	else if (sn == gsn_fifth_attack && !IS_SET(ch->act,ACT_WARRIOR))

		skill = 2 * ch->level - 200;



	else if (sn == gsn_sixth_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 150;



	else if (sn == gsn_sixth_attack && !IS_SET(ch->act,ACT_WARRIOR))

		skill = 2 * ch->level - 250;



	else if (sn == gsn_seventh_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 200;



	else if (sn == gsn_seventh_attack && !IS_SET(ch->act,ACT_WARRIOR))

		skill = 2 * ch->level - 350;



	else if (sn == gsn_eighth_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 250;



	else if (sn == gsn_eighth_attack && !IS_SET(ch->act,ACT_WARRIOR))

		skill = 2 * ch->level - 500;



	else if (sn == gsn_ninth_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 3 * ch->level - 300;



	else if (sn == gsn_tenth_attack && IS_SET(ch->act,ACT_WARRIOR))

		skill = 2 * ch->level - 350;



	else if (sn == gsn_dual_wield && IS_SET(ch->act,ACT_WARRIOR))

		skill = 20 + 3 * ch->level;



	else if (sn == gsn_dual_wield && !IS_SET(ch->act,ACT_WARRIOR))

		skill = (2 * ch->level) - 120;



	else if (sn == gsn_hand_to_hand)

	    skill = 40 + 2 * ch->level;



 	else if (sn == gsn_trip && IS_SET(ch->off_flags,OFF_TRIP))

	    skill = 10 + 3 * ch->level;



 	else if (sn == gsn_bash && IS_SET(ch->off_flags,OFF_BASH))

	    skill = 10 + 3 * ch->level;



	else if (sn == gsn_disarm

	     &&  (IS_SET(ch->off_flags,OFF_DISARM)

	     ||   IS_SET(ch->act,ACT_WARRIOR)

	     ||	  IS_SET(ch->act,ACT_THIEF)))

	    skill = 20 + 3 * ch->level;



	else if (sn == gsn_berserk && IS_SET(ch->off_flags,OFF_BERSERK))

	    skill = 3 * ch->level;



	else if (sn == gsn_fury && IS_SET(ch->off_flags,OFF_FURY))

	    skill = 2 * ch->level;



	else if (sn == gsn_destruction && IS_SET(ch->off_flags,OFF_DESTRUCTION))

	    skill = 2 * ch->level;



	else if (sn == gsn_kick)

	    skill = 10 + 3 * ch->level;



	else if (sn == gsn_spinkick)

	    skill = 10 + 3 * ch->level;



	else if (sn == gsn_gouge)

	    skill = 3 * ch->level;



	else if (sn == gsn_backstab && IS_SET(ch->act,ACT_THIEF))

	    skill = 20 + 2 * ch->level;



  	else if (sn == gsn_rescue)

	    skill = 40 + ch->level;



	else if (sn == gsn_recall)

	    skill = 40 + ch->level;



	else if (sn == gsn_sword

	||  sn == gsn_dagger

	||  sn == gsn_spear

	||  sn == gsn_mace

	||  sn == gsn_axe

	||  sn == gsn_flail

	||  sn == gsn_whip

	||  sn == gsn_polearm

	||  sn == gsn_projectile
	||  sn == gsn_battelh)

	    skill = 40 + 5 * ch->level / 2;



	else

	   skill = 0;



	/* Time to beef up the high level mobs */



	if (ch->level>=100 && ch->level<150 && skill>0)

		skill = skill + (skill/4) + ((ch->level-100)*2);



	if (ch->level>=150 && ch->level<200 && skill>0)

		skill = skill + (skill/3) + ((ch->level-100)*2) + ((ch->level-150)*5/4);



	if (ch->level>=200 && skill>0)

		skill = skill + (skill/3) + ((ch->level-100)*2) + ((ch->level-150)*5/4) + ((ch->level-200)*3/2);



    }



    if (ch->daze > 0)

    {

	if (skill_table[sn].spell_fun != spell_null)

	    skill /= 2;

	else

	    skill = 2 * skill / 3;

    }



    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )

	skill = 9 * skill / 10;



    if ( IS_ADMIN(ch) )

	skill	= 100;



    return URANGE(0,skill,100);

}



/* for returning weapon information */

int get_weapon_sn(CHAR_DATA *ch)

{

    OBJ_DATA *wield;

    int sn;



    wield = get_eq_char( ch, WEAR_WIELD );

    if (wield == NULL || wield->item_type != ITEM_WEAPON)

        sn = gsn_hand_to_hand;

    else switch (wield->value[0])
    {
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
		case(WEAPON_PROJ):      sn = gsn_projectile;    break;
		case(WEAPON_BTL):		sn = gsn_battelh;		break;
		case(WEAPON_LIGHTSABER): sn = gsn_lightsaber;	break;
		case(WEAPON_STAKE):		sn = gsn_stake;			break;
   }

   return sn;

}



int get_weapon_skill(CHAR_DATA *ch, int sn)

{

     int skill;



     /* -1 is exotic */

    if (IS_NPC(ch))

    {

	if (sn == -1)

	    skill = 3 * ch->level;

	else if (sn == gsn_hand_to_hand)

	    skill = 40 + 2 * ch->level;

	else

	    skill = 40 + 5 * ch->level / 2;

    }



    else

    {

	if (sn == -1)

	    skill = 3 * ch->level;

	else

	    skill = ch->pcdata->learned[sn];

    }



    return URANGE(0,skill,100);

}





/* used to de-screw characters */

void reset_char(CHAR_DATA *ch)

{

     int loc,mod,stat;

     OBJ_DATA *obj;

     AFFECT_DATA *af;

     int i;



     if (IS_NPC(ch))

	return;



    if (ch->pcdata->perm_hit == 0

    ||	ch->pcdata->perm_mana == 0

    ||  ch->pcdata->perm_move == 0

    ||	ch->pcdata->last_level == 0)

    {

    /* do a FULL reset */

	for (loc = 0; loc < MAX_WEAR; loc++)

	{

	    obj = get_eq_char(ch,loc);

	    if (obj == NULL)

		continue;

	    if (!obj->enchanted)

	    for ( af = obj->pIndexData->affected; af != NULL; af = af->next )

	    {

		mod = af->modifier;

		switch(af->location)

		{

		    case APPLY_SEX:	ch->sex		-= mod;

					if (ch->sex < 0 || ch->sex >2)

					    ch->sex = IS_NPC(ch) ?

						0 :

						ch->pcdata->true_sex;

									break;

		    case APPLY_MANA:	ch->max_mana	-= mod;		break;

		    case APPLY_HIT:	ch->max_hit	-= mod;		break;

		    case APPLY_MOVE:	ch->max_move	-= mod;		break;

		}

	    }



            for ( af = obj->affected; af != NULL; af = af->next )

            {

                mod = af->modifier;

                switch(af->location)

                {

                    case APPLY_SEX:     ch->sex         -= mod;         break;

                    case APPLY_MANA:    ch->max_mana    -= mod;         break;

                    case APPLY_HIT:     ch->max_hit     -= mod;         break;

                    case APPLY_MOVE:    ch->max_move    -= mod;         break;

                }

            }

	}

	/* now reset the permanent stats */

	ch->pcdata->perm_hit 	= ch->max_hit;

	ch->pcdata->perm_mana 	= ch->max_mana;

	ch->pcdata->perm_move	= ch->max_move;

	ch->pcdata->last_level	= ch->perm_played/3600;

	if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)

		if (ch->sex > 0 && ch->sex < 3)

	    	    ch->pcdata->true_sex	= ch->sex;

		else

		    ch->pcdata->true_sex 	= 0;



    }



    /* now restore the character to his/her true condition */

    for (stat = 0; stat < MAX_STATS; stat++)

	ch->mod_stat[stat] = 0;



    if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)

	ch->pcdata->true_sex = 0;

    ch->sex		= ch->pcdata->true_sex;

    ch->max_hit 	= ch->pcdata->perm_hit;

    ch->max_mana	= ch->pcdata->perm_mana;

    ch->max_move	= ch->pcdata->perm_move;



    for (i = 0; i < 4; i++)

    	ch->armor[i]	= 100;



    ch->hitroll		= 0;

    ch->damroll		= 0;

    ch->saving_throw	= 0;



    /* now start adding back the effects */

    for (loc = 0; loc < MAX_WEAR; loc++)

    {

        obj = get_eq_char(ch,loc);

        if (obj == NULL)

            continue;

	for (i = 0; i < 4; i++)

	    ch->armor[i] -= apply_ac( obj, loc, i );



        if (!obj->enchanted)

	for ( af = obj->pIndexData->affected; af != NULL; af = af->next )

        {

            mod = af->modifier;

            switch(af->location)
            {
		case APPLY_STR:		ch->mod_stat[STAT_STR]	+= mod; break;
		case APPLY_DEX:		ch->mod_stat[STAT_DEX]	+= mod; break;
		case APPLY_INT:		ch->mod_stat[STAT_INT]	+= mod; break;
		case APPLY_WIS:		ch->mod_stat[STAT_WIS]	+= mod; break;
		case APPLY_CON:		ch->mod_stat[STAT_CON]	+= mod; break;
		case APPLY_AGI:		ch->mod_stat[STAT_AGI]	+= mod; break;
		case APPLY_MCH:		ch->mod_stat[STAT_MCH]	+= mod; break;
		case APPLY_TCH:		ch->mod_stat[STAT_TCH]	+= mod; break;
		
		case APPLY_SEX:		ch->sex			+= mod; break;
		case APPLY_MANA:	ch->max_mana		+= mod; break;
		case APPLY_HIT:		ch->max_hit		+= mod; break;
		case APPLY_MOVE:	ch->max_move		+= mod; break;

		case APPLY_AC:
		    for (i = 0; i < 4; i ++)
			ch->armor[i] += mod;
		    break;
		case APPLY_HITROLL:	ch->hitroll		+= mod; break;
		case APPLY_DAMROLL:	ch->damroll		+= mod; break;
		case APPLY_DEFROLL:	ch->defroll		+= mod; break;

		case APPLY_SAVES:		ch->saving_throw += mod; break;
		case APPLY_SAVING_ROD: 		ch->saving_throw += mod; break;
		case APPLY_SAVING_PETRI:	ch->saving_throw += mod; break;
		case APPLY_SAVING_BREATH: 	ch->saving_throw += mod; break;
		case APPLY_SAVING_SPELL:	ch->saving_throw += mod; break;
	    }
        }

        for ( af = obj->affected; af != NULL; af = af->next )
        {
            mod = af->modifier;
            switch(af->location)
            {
                case APPLY_STR:         ch->mod_stat[STAT_STR]  += mod; break;
                case APPLY_DEX:         ch->mod_stat[STAT_DEX]  += mod; break;
                case APPLY_INT:         ch->mod_stat[STAT_INT]  += mod; break;
                case APPLY_WIS:         ch->mod_stat[STAT_WIS]  += mod; break;
                case APPLY_AGI:         ch->mod_stat[STAT_AGI]  += mod; break;
				case APPLY_MCH:         ch->mod_stat[STAT_MCH]  += mod; break;
				case APPLY_TCH:         ch->mod_stat[STAT_TCH]  += mod; break;
				
                case APPLY_SEX:         ch->sex                 += mod; break;
                case APPLY_MANA:        ch->max_mana            += mod; break;
                case APPLY_HIT:         ch->max_hit             += mod; break;
                case APPLY_MOVE:        ch->max_move            += mod; break;

                case APPLY_AC:
                    for (i = 0; i < 4; i ++)
                        ch->armor[i] += mod;
                    break;
				case APPLY_HITROLL:     ch->hitroll             += mod; break;
            	case APPLY_DAMROLL:     ch->damroll             += mod; break;
				case APPLY_DEFROLL:     ch->defroll             += mod; break;

                case APPLY_SAVES:         ch->saving_throw += mod; break;
                case APPLY_SAVING_ROD:          ch->saving_throw += mod; break;
                case APPLY_SAVING_PETRI:        ch->saving_throw += mod; break;
                case APPLY_SAVING_BREATH:       ch->saving_throw += mod; break;
                case APPLY_SAVING_SPELL:        ch->saving_throw += mod; break;
            }
	}
    }

    /* now add back spell effects */

    for (af = ch->affected; af != NULL; af = af->next)

    {

        mod = af->modifier;

        switch(af->location)
        {
                case APPLY_STR:         ch->mod_stat[STAT_STR]  += mod; break;
                case APPLY_DEX:         ch->mod_stat[STAT_DEX]  += mod; break;
                case APPLY_INT:         ch->mod_stat[STAT_INT]  += mod; break;
                case APPLY_WIS:         ch->mod_stat[STAT_WIS]  += mod; break;
                case APPLY_CON:         ch->mod_stat[STAT_CON]  += mod; break;
                case APPLY_AGI:         ch->mod_stat[STAT_AGI]  += mod; break;
                case APPLY_MCH:         ch->mod_stat[STAT_TCH]  += mod; break;
                case APPLY_TCH:         ch->mod_stat[STAT_MCH]  += mod; break;

                case APPLY_SEX:         ch->sex                 += mod; break;
                case APPLY_MANA:        ch->max_mana            += mod; break;
                case APPLY_HIT:         ch->max_hit             += mod; break;
                case APPLY_MOVE:        ch->max_move            += mod; break;

                case APPLY_AC:
                    for (i = 0; i < 4; i ++)
                        ch->armor[i] += mod;
                    break;
                case APPLY_HITROLL:     ch->hitroll             += mod; break;
                case APPLY_DAMROLL:     ch->damroll             += mod; break;
                case APPLY_DEFROLL:     ch->defroll             += mod; break;

                case APPLY_SAVES:         ch->saving_throw += mod; break;
                case APPLY_SAVING_ROD:          ch->saving_throw += mod; break;
                case APPLY_SAVING_PETRI:        ch->saving_throw += mod; break;
                case APPLY_SAVING_BREATH:       ch->saving_throw += mod; break;
                case APPLY_SAVING_SPELL:        ch->saving_throw += mod; break;
        }
    }

    /* make sure sex is RIGHT!!!! */
    if (ch->sex < 0 || ch->sex > 2)
	ch->sex = ch->pcdata->true_sex;
}





/*

 * Retrieve a character's trusted level for permission checking.

 */

int get_trust( CHAR_DATA *ch )

{

    if ( ch->desc != NULL && ch->desc->original != NULL )

	ch = ch->desc->original;



    if (ch->trust)

	return ch->trust;



    if ( IS_NPC(ch) && ch->level >= MAX_MORT_LEVEL )

	return MAX_MORT_LEVEL;

    else

	return ch->level;

}





/*

 * Retrieve a character's age.

 */

int get_age( CHAR_DATA *ch )

{

    return 17 + ( ch->played + (int) (current_time - ch->logon) ) / 72000;

}



/* command for retrieving stats */
int get_curr_stat( CHAR_DATA *ch, int stat )
{
    int max;

    if (IS_NPC(ch) || (ch->level >= LEVEL_IMMORTAL))
	max = 35; //Immortals can have stats up to 35, just like remorts.
    else
    {
	max = pc_race_table[ch->race].max_stats[stat] + 4;

	if (class_table[ch->class].attr_prime == stat)
	    max += 2;

	if ( ch->race == race_lookup("human"))
	    max += 1;

 	      if ( IS_REMORT(ch) )
          {
             max = UMIN(max,35);
          }
          else
          {
             max = UMIN(max,25);
          }
    }

    return URANGE(3,ch->perm_stat[stat] + ch->mod_stat[stat], max);
}


/* command for returning max training score */

int get_max_train( CHAR_DATA *ch, int stat )
{
    int max;

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	return 36;

    max = pc_race_table[ch->race].max_stats[stat];
    if (class_table[ch->class].attr_prime == stat)
	if (ch->race == race_lookup("human"))
	   max += 3;
	else
	   max += 2;
	//Grayson - new max stats for remorts with non-remort races
	if ((race_table[ch->race].remort_race == FALSE) && IS_REMORT(ch))
		max += 2; 
	//


    return UMIN(max,36);
}





/*

 * Retrieve a character's carry capacity.

 */

int can_carry_n( CHAR_DATA *ch )

{

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )

	return 10000;



    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )

	return 0;



    if ( IS_SET(ch->info, INFO_GHOST) )

	return 0;



    return MAX_WEAR +  2 * get_curr_stat(ch,STAT_DEX) + ch->level;

}







/*

 * Retrieve a character's carry capacity.

 */

int can_carry_w( CHAR_DATA *ch )

{

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )

	return 10000000;



    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )

	return 0;



    if ( IS_SET(ch->info, INFO_GHOST) )

	return 0;



    return str_app[get_curr_stat(ch,STAT_STR)].carry * 10 + ch->level * 25;

}







/*

 * See if a string is one of the names of an object.

 */



bool is_name ( char *str, char *namelist )

{

    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];

    char *list, *string;



    /* fix crash on NULL namelist */

    if (namelist == NULL || namelist[0] == '\0')

    	return FALSE;



    /* fixed to prevent is_name on "" returning TRUE */

    if (str[0] == '\0')

	return FALSE;



    string = str;

    /* we need ALL parts of string to match part of namelist */

    for ( ; ; )  /* start parsing string */

    {

	str = one_argument(str,part);



	if (part[0] == '\0' )

	    return TRUE;



	/* check to see if this is part of namelist */

	list = namelist;

	for ( ; ; )  /* start parsing namelist */

	{

	    list = one_argument(list,name);

	    if (name[0] == '\0')  /* this name was not found */

		return FALSE;



	    if (!str_prefix(string,name))

		return TRUE; /* full pattern match */



	    if (!str_prefix(part,name))

		break;

	}

    }

}



bool is_exact_name(char *str, char *namelist )

{

    char name[MAX_INPUT_LENGTH];



    if (namelist == NULL)

	return FALSE;



    for ( ; ; )

    {

	namelist = one_argument( namelist, name );

	if ( name[0] == '\0' )

	    return FALSE;

	if ( !str_cmp( str, name ) )

	    return TRUE;

    }

}



/* enchanted stuff for eq */

void affect_enchant(OBJ_DATA *obj)

{

    /* okay, move all the old flags into new vectors if we have to */

    if (!obj->enchanted)

    {

        AFFECT_DATA *paf, *af_new;

        obj->enchanted = TRUE;



        for (paf = obj->pIndexData->affected;

             paf != NULL; paf = paf->next)

        {

	    af_new = new_affect();



            af_new->next = obj->affected;

            obj->affected = af_new;



	    af_new->where	= paf->where;

            af_new->type        = UMAX(0,paf->type);

            af_new->level       = paf->level;

            af_new->duration    = paf->duration;

            af_new->location    = paf->location;

            af_new->modifier    = paf->modifier;

            af_new->bitvector   = paf->bitvector;

        }

    }

}





/*

 * Apply or remove an affect to a character.

 */

void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )

{

    OBJ_DATA *wield;

    int mod,i;



    mod = paf->modifier;



    if ( fAdd )

    {

	switch (paf->where)
	{
	case TO_AFFECTS:
	    SET_BIT(ch->affected_by, paf->bitvector);
	    break;
	case TO_AFFECTS2:  //Grayson new aff 9 March 2004
        SET_BIT(ch->affected2_by, paf->bitvector);
        break;
	case TO_IMMUNE:
	    SET_BIT(ch->imm_flags,paf->bitvector);
	    break;
	case TO_RESIST:
	    SET_BIT(ch->res_flags,paf->bitvector);
	    break;
	case TO_VULN:
	    SET_BIT(ch->vuln_flags,paf->bitvector);
	    break;
	}

    }

    else

    {

        switch (paf->where)

        {

        case TO_AFFECTS:

            REMOVE_BIT(ch->affected_by, paf->bitvector);

            break;
       case TO_AFFECTS2:  //Grayson new aff 9 March 2004
            REMOVE_BIT(ch->affected2_by, paf->bitvector);
            break;

        case TO_IMMUNE:

            REMOVE_BIT(ch->imm_flags,paf->bitvector);

            break;

        case TO_RESIST:

            REMOVE_BIT(ch->res_flags,paf->bitvector);

            break;

        case TO_VULN:

            REMOVE_BIT(ch->vuln_flags,paf->bitvector);

            break;

        }

	mod = 0 - mod;

    }



    switch ( paf->location )

    {

    default:

	bug( "Affect_modify: unknown location %d.", paf->location );

	return;



    case APPLY_NONE:						break;
    case APPLY_STR:           ch->mod_stat[STAT_STR]	+= mod;	break;
    case APPLY_DEX:           ch->mod_stat[STAT_DEX]	+= mod;	break;
    case APPLY_INT:           ch->mod_stat[STAT_INT]	+= mod;	break;
    case APPLY_WIS:           ch->mod_stat[STAT_WIS]	+= mod;	break;
    case APPLY_CON:           ch->mod_stat[STAT_CON]	+= mod;	break;
    case APPLY_AGI:           ch->mod_stat[STAT_AGI]	+= mod;	break;
    case APPLY_MCH:           ch->mod_stat[STAT_MCH]	+= mod;	break;
    case APPLY_TCH:           ch->mod_stat[STAT_TCH]	+= mod;	break;
            
    case APPLY_SEX:           ch->sex			+= mod;	break;

    case APPLY_CLASS:						break;

    case APPLY_LEVEL:						break;

    case APPLY_AGE:						break;

    case APPLY_HEIGHT:						break;

    case APPLY_WEIGHT:						break;

    case APPLY_MANA:          ch->max_mana		+= mod;	break;

    case APPLY_HIT:           ch->max_hit		+= mod;	break;

    case APPLY_MOVE:          ch->max_move		+= mod;	break;

    case APPLY_GOLD:						break;

    case APPLY_EXP:						break;

    case APPLY_AC:

        for (i = 0; i < 4; i ++)

            ch->armor[i] += mod;

        break;

    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;

    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    
    case APPLY_DEFROLL:       ch->defroll		+= mod;	break;

    case APPLY_SAVES:   ch->saving_throw		+= mod;	break;

    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;

    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;

    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;

    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;

    case APPLY_SPELL_AFFECT:  					break;

    }



    /*

     * Check for weapon wielding.

     * Guard against recursion (for weapons with affects).

     */

    if ( !IS_NPC(ch) && ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL

    &&   get_obj_weight(wield) > (str_app[get_curr_stat(ch,STAT_STR)].wield*10))

    {

	static int depth;



	if ( depth == 0 )

	{

	    depth++;

	    act( "You drop $p.", ch, wield, NULL, TO_CHAR );

	    act( "$n drops $p.", ch, wield, NULL, TO_ROOM );

	    obj_from_char( wield );

	    obj_to_room( wield, ch->in_room );

	    depth--;

	}

    }



    return;

}





/* find an effect in an affect list */

AFFECT_DATA  *affect_find(AFFECT_DATA *paf, int sn)

{

    AFFECT_DATA *paf_find;



    for ( paf_find = paf; paf_find != NULL; paf_find = paf_find->next )

    {

        if ( paf_find->type == sn )

	return paf_find;

    }



    return NULL;

}



/* fix object affects when removing one */

void affect_check(CHAR_DATA *ch,int where,int vector)

{

    AFFECT_DATA *paf;

    OBJ_DATA *obj;



    if (where == TO_OBJECT || where == TO_WEAPON || vector == 0)

	return;



    for (paf = ch->affected; paf != NULL; paf = paf->next)

	if (paf->where == where && paf->bitvector == vector)

	{

	    switch (where)

	    {

	        case TO_AFFECTS:

		    SET_BIT(ch->affected_by,vector);

		    break;
			case TO_AFFECTS2: //Grayson New Aff 9 March 2004
                 SET_BIT(ch->affected2_by, vector);
                 break;
	        case TO_IMMUNE:

		    SET_BIT(ch->imm_flags,vector);

		    break;

	        case TO_RESIST:

		    SET_BIT(ch->res_flags,vector);

		    break;

	        case TO_VULN:

		    SET_BIT(ch->vuln_flags,vector);

		    break;

	    }

	    return;

	}



    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)

    {

	if (obj->wear_loc == -1)

	    continue;



            for (paf = obj->affected; paf != NULL; paf = paf->next)

            if (paf->where == where && paf->bitvector == vector)

            {

                switch (where)

                {

                    case TO_AFFECTS:

                        SET_BIT(ch->affected_by,vector);

                        break;
					case TO_AFFECTS2:  //Grayson New Aff 9 March 2004
                    	SET_BIT(ch->affected2_by, vector);
	                    break;
                    case TO_IMMUNE:

                        SET_BIT(ch->imm_flags,vector);

                        break;

                    case TO_RESIST:

                        SET_BIT(ch->res_flags,vector);

                        break;

                    case TO_VULN:

                        SET_BIT(ch->vuln_flags,vector);



                }

                return;

            }



        if (obj->enchanted)

	    continue;



        for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)

            if (paf->where == where && paf->bitvector == vector)

            {

                switch (where)

                {

                    case TO_AFFECTS:

                        SET_BIT(ch->affected_by,vector);

                        break;
                    case TO_AFFECTS2:  //Grayson New AFF 9 March 2004
	                    SET_BIT(ch->affected2_by, vector);
    	                break;

                    case TO_IMMUNE:

                        SET_BIT(ch->imm_flags,vector);

                        break;

                    case TO_RESIST:

                        SET_BIT(ch->res_flags,vector);

                        break;

                    case TO_VULN:

                        SET_BIT(ch->vuln_flags,vector);

                        break;

                }

                return;

            }

    }

}



/*

 * Give an affect to a char.

 */

void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )

{

    AFFECT_DATA *paf_new;



    paf_new = new_affect();
    VALIDATE(paf_new); // When assigning an affect to a character/object, the memory allocated for the affect isn't validated and thus never freed when the affect is removed


    *paf_new		= *paf;



    paf_new->next	= ch->affected;

    ch->affected	= paf_new;



    affect_modify( ch, paf_new, TRUE );

    return;

}



/* give an affect to an object */

void affect_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)

{

    AFFECT_DATA *paf_new;



    paf_new = new_affect();



    *paf_new		= *paf;

    paf_new->next	= obj->affected;

    obj->affected	= paf_new;



    /* apply any affect vectors to the object's extra_flags */

    if (paf->bitvector)

        switch (paf->where)

        {

        case TO_OBJECT:

    	    SET_BIT(obj->extra_flags,paf->bitvector);

	    break;

        case TO_WEAPON:

	    if (obj->item_type == ITEM_WEAPON)

	        SET_BIT(obj->value[4],paf->bitvector);

	    break;

        }





    return;

}







/*

 * Remove an affect from a char.

 */

void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    int where;
    int vector;

    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE );
    where = paf->where;
    vector = paf->bitvector;

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }

    free_affect(paf);

    affect_check(ch,where,vector);
    return;
}



void affect_remove_obj( OBJ_DATA *obj, AFFECT_DATA *paf)

{

    int where, vector;

    if ( obj->affected == NULL )

    {

        bug( "Affect_remove_object: no affect.", 0 );

        return;

    }



    if (obj->carried_by != NULL && obj->wear_loc != -1)

	affect_modify( obj->carried_by, paf, FALSE );



    where = paf->where;

    vector = paf->bitvector;



    /* remove flags from the object if needed */

    if (paf->bitvector)

	switch( paf->where)

        {

        case TO_OBJECT:

            REMOVE_BIT(obj->extra_flags,paf->bitvector);

            break;

        case TO_WEAPON:

            if (obj->item_type == ITEM_WEAPON)

                REMOVE_BIT(obj->value[4],paf->bitvector);

            break;

        }



    if ( paf == obj->affected )

    {

        obj->affected    = paf->next;

    }

    else

    {

        AFFECT_DATA *prev;



        for ( prev = obj->affected; prev != NULL; prev = prev->next )

        {

            if ( prev->next == paf )

            {

                prev->next = paf->next;

                break;

            }

        }



        if ( prev == NULL )

        {

            bug( "Affect_remove_object: cannot find paf.", 0 );

            return;

        }

    }



    free_affect(paf);



    if (obj->carried_by != NULL && obj->wear_loc != -1)

	affect_check(obj->carried_by,where,vector);

    return;

}







/*

 * Strip all affects of a given sn.

 */

void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}







/*

 * Return true if a char is affected by a spell.

 */

bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}

int lvl_affected( CHAR_DATA *ch, int sn )
{						//Returns the level of a certain affect
    AFFECT_DATA *paf;	//Grayson 8 March 2004
						//Should be quite useful, if'n it works
    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return paf->level;
    }

    return -1;
}



/*

 * Add or enhance an affect.

 */

void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )

{

    AFFECT_DATA *paf_old;

    bool found;



    found = FALSE;

    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )

    {

	if ( paf_old->type == paf->type )

	{

	    paf->level = (paf->level += paf_old->level) / 2;

	    paf->duration += paf_old->duration;

	    paf->modifier += paf_old->modifier;

	    affect_remove( ch, paf_old );

	    break;

	}

    }



    affect_to_char( ch, paf );

    return;

}







/*

 * Move a char out of a room.

 */

void char_from_room( CHAR_DATA *ch )

{

    OBJ_DATA *obj;



    if ( ch->in_room == NULL )

    {

	bug( "Char_from_room: NULL.", 0 );

	return;

    }



    if ( !IS_NPC(ch) )

	--ch->in_room->area->nplayer;



    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL

    &&   obj->item_type == ITEM_LIGHT

    &&   obj->value[2] != 0

    &&   ch->in_room->light > 0 )

	--ch->in_room->light;



    if ( ch == ch->in_room->people )

    {

	ch->in_room->people = ch->next_in_room;

    }

    else

    {

	CHAR_DATA *prev;



	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )

	{

	    if ( prev->next_in_room == ch )

	    {

		prev->next_in_room = ch->next_in_room;

		break;

	    }

	}



	if ( prev == NULL )

	    bug( "Char_from_room: ch not found.", 0 );

    }



    ch->in_room      = NULL;

    ch->next_in_room = NULL;

    ch->on 	     = NULL;  /* sanity check! */

    return;

}







/*

 * Move a char into a room.

 */

void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )

{

    OBJ_DATA *obj;



    if ( pRoomIndex == NULL )

    {

	ROOM_INDEX_DATA *room;



	bug( "Char_to_room: NULL.", 0 );



	if ((room = get_room_index(ROOM_VNUM_TEMPLE)) != NULL)

	    char_to_room(ch,room);



	return;

    }



    ch->in_room		= pRoomIndex;

    ch->next_in_room	= pRoomIndex->people;

    pRoomIndex->people	= ch;



    if ( !IS_NPC(ch) )

    {

	if (ch->in_room->area->empty)

	{

	    ch->in_room->area->empty = FALSE;

	    ch->in_room->area->age = 0;

	}

	++ch->in_room->area->nplayer;

    }



    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL

    &&   obj->item_type == ITEM_LIGHT

    &&   obj->value[2] != 0 )

	++ch->in_room->light;



    if (IS_AFFECTED(ch,AFF_PLAGUE))

    {

        AFFECT_DATA *af, plague;

        CHAR_DATA *vch;



        for ( af = ch->affected; af != NULL; af = af->next )

        {

            if (af->type == gsn_plague)

                break;

        }



        if (af == NULL)

        {

            REMOVE_BIT(ch->affected_by,AFF_PLAGUE);

            return;

        }



	  if (ch->level >= LEVEL_IMMORTAL)

	  {

		REMOVE_BIT(ch->affected_by,AFF_PLAGUE);

		send_to_char( "The plague sores vanish!.\n\r",ch);

		return;

	  }



        if (af->level == 1)

            return;



	  plague.where		= TO_AFFECTS;

        plague.type 		= gsn_plague;

        plague.level 		= af->level - 1;

        plague.duration 	= number_range(1,2 * plague.level);

        plague.location		= APPLY_STR;

        plague.modifier 	= -5;

        plague.bitvector 	= AFF_PLAGUE;



        for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)

        {

            if (!saves_spell(plague.level - 2,vch,DAM_DISEASE)

	    &&  !IS_IMMORTAL(vch) &&

            	!IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(6) == 0)

            {

            	send_to_char("You feel hot and feverish.\n\r",vch);

            	act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);

            	affect_join(vch,&plague);

            }

        }

    }





    return;

}







/*

 * Give an obj to a char.

 */

void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )

{

    obj->next_content	 = ch->carrying;

    ch->carrying	 = obj;

    obj->carried_by	 = ch;

    obj->in_room	 = NULL;

    obj->in_obj		 = NULL;

    ch->carry_number	+= get_obj_number( obj );

    ch->carry_weight	+= get_obj_weight( obj );

}







/*

 * Take an obj from its character.

 */

void obj_from_char( OBJ_DATA *obj )

{

    CHAR_DATA *ch;



    if ( ( ch = obj->carried_by ) == NULL )

    {

	bug( "Obj_from_char: null ch.", 0 );

	return;

    }



    if ( obj->wear_loc != WEAR_NONE )

	unequip_char( ch, obj );



    if ( ch->carrying == obj )

    {

	ch->carrying = obj->next_content;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )

	{

	    if ( prev->next_content == obj )

	    {

		prev->next_content = obj->next_content;

		break;

	    }

	}



	if ( prev == NULL )

	    bug( "Obj_from_char: obj not in list.", 0 );

    }



    obj->carried_by	 = NULL;

    obj->next_content	 = NULL;

    ch->carry_number	-= get_obj_number( obj );

    ch->carry_weight	-= get_obj_weight( obj );

    return;

}







/*

 * Find the ac value of an obj, including position effect.

 */

int apply_ac( OBJ_DATA *obj, int iWear, int type )

{

    if ( obj->item_type != ITEM_ARMOR )

	return 0;



    switch ( iWear )

    {

    case WEAR_BODY:	return 3 * obj->value[type];

    case WEAR_HEAD:	return 2 * obj->value[type];

    case WEAR_LEGS:	return 2 * obj->value[type];

    case WEAR_FINGER_L: return     obj->value[type];

    case WEAR_FINGER_R: return     obj->value[type];

    case WEAR_FEET:	return     obj->value[type];

    case WEAR_HANDS:	return     obj->value[type];

    case WEAR_ARMS:	return     obj->value[type];

    case WEAR_SHIELD:	return     obj->value[type];

    case WEAR_NECK_1:	return     obj->value[type];

    case WEAR_NECK_2:	return     obj->value[type];

    case WEAR_ABOUT:	return 2 * obj->value[type];

    case WEAR_WAIST:	return     obj->value[type];

    case WEAR_WRIST_L:	return     obj->value[type];

    case WEAR_WRIST_R:	return     obj->value[type];

    case WEAR_HOLD:	return     obj->value[type];

    case WEAR_CYBER_EYE:         return     obj->value[type];
    case WEAR_CYBER_COMM:        return     obj->value[type];
    case WEAR_CYBER_LEGS:        return     obj->value[type];
    case WEAR_CYBER_CHEST:       return     obj->value[type];
    case WEAR_CYBER_REFLEXES:    return     obj->value[type];
    case WEAR_CYBER_MIND:        return     obj->value[type];
    case WEAR_CYBER_STRENGTH:    return     obj->value[type];

    }



    return 0;

}







/*

 * Find a piece of eq on a character.

 */

OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )

{

    OBJ_DATA *obj;



    if (ch == NULL)

	return NULL;



    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )

    {

	if ( obj->wear_loc == iWear )

	    return obj;

    }



    return NULL;

}







/*

 * Equip a char with an obj.

 */

void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
	char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *name;
    int i;

    if ( get_eq_char( ch, iWear ) != NULL )
    {
	sprintf( buf, "Mob %d (%s) slot %d already loaded %d.", ch->pIndexData->vnum, ch->name, iWear, obj->pIndexData->vnum );
	bug( buf, iWear );
	return;
    }

	if ( /*Checks For Class Flags */
	((class_lookup("mage") == ch->class) && (IS_OBJ_STAT(obj, ITEM_ANTI_MAG)))||
	((class_lookup("thief") == ch->class) && (IS_OBJ_STAT(obj, ITEM_ANTI_THI)))||
	((class_lookup("warrior") == ch->class) && (IS_OBJ_STAT(obj, ITEM_ANTI_WAR)))||
	((class_lookup("druid") == ch->class) && (IS_OBJ_STAT(obj, ITEM_ANTI_DRU)))||
	((class_lookup("cyborg") == ch->class) && (IS_OBJ_STAT(obj, ITEM_ANTI_CYB)))||
	((class_lookup("cleric") == ch->class) && (IS_OBJ_STAT(obj, ITEM_ANTI_CLE)))
	)
	{
		act( "You cannot use $p.", ch, obj, NULL, TO_CHAR );
		return;
	}

    if ( IS_SET(ch->info, INFO_GHOST) )
    {
	act( "You are a Ghost, $p falls through you to the floor.", ch, obj, NULL, TO_CHAR );
	act( "$p falls through $n to the floor.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	return;
    }



    if ( ch->level < obj->level )
    {
	act( "You are to inexperienced to use $p and drop it.", ch, obj, NULL, TO_CHAR );
	act( "$n is to inexperienced to use $p and drops it.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	return;
    }



    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
	act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
	act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	return;
    }



    for (i = 0; i < 4; i++)

    	ch->armor[i]      	-= apply_ac( obj, iWear,i );

    obj->wear_loc	 = iWear;



    if (!obj->enchanted)

	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )

	    if ( paf->location != APPLY_SPELL_AFFECT )

	        affect_modify( ch, paf, TRUE );

    for ( paf = obj->affected; paf != NULL; paf = paf->next )

	if ( paf->location == APPLY_SPELL_AFFECT )

    	    affect_to_char ( ch, paf );

	else

	    affect_modify( ch, paf, TRUE );



    if ( obj->item_type == ITEM_LIGHT

    &&   obj->value[2] != 0

    &&   ch->in_room != NULL )

	++ch->in_room->light;



    return;

}







/*

 * Unequip a char with an obj.

 */

void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )

{

    AFFECT_DATA *paf = NULL;

    AFFECT_DATA *lpaf = NULL;

    AFFECT_DATA *lpaf_next = NULL;

    int i;



    if ( obj->wear_loc == WEAR_NONE )

    {

	bug( "Unequip_char: already unequipped.", 0 );

	return;

    }



    for (i = 0; i < 4; i++)

    	ch->armor[i]	+= apply_ac( obj, obj->wear_loc,i );

    obj->wear_loc	 = -1;



    if (!obj->enchanted)

	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )

	    if ( paf->location == APPLY_SPELL_AFFECT )

	    {

	        for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )

	        {

		    lpaf_next = lpaf->next;

		    if ((lpaf->type == paf->type) &&

		        (lpaf->level == paf->level) &&

		        (lpaf->location == APPLY_SPELL_AFFECT))

		    {

		        affect_remove( ch, lpaf );

			lpaf_next = NULL;

		    }

	        }

	    }

	    else

	    {

	        affect_modify( ch, paf, FALSE );

		affect_check(ch,paf->where,paf->bitvector);

	    }



    for ( paf = obj->affected; paf != NULL; paf = paf->next )

	if ( paf->location == APPLY_SPELL_AFFECT )

	{

	    bug ( "Norm-Apply: %d", 0 );

	    for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )

	    {

		lpaf_next = lpaf->next;

		if ((lpaf->type == paf->type) &&

		    (lpaf->level == paf->level) &&

		    (lpaf->location == APPLY_SPELL_AFFECT))

		{

		    bug ( "location = %d", lpaf->location );

		    bug ( "type = %d", lpaf->type );

		    affect_remove( ch, lpaf );

		    lpaf_next = NULL;

		}

	    }

	}

	else

	{

	    affect_modify( ch, paf, FALSE );

	    affect_check(ch,paf->where,paf->bitvector);

	}



    if ( obj->item_type == ITEM_LIGHT

    &&   obj->value[2] != 0

    &&   ch->in_room != NULL

    &&   ch->in_room->light > 0 )

	--ch->in_room->light;



    return;

}







/*

 * Count occurrences of an obj in a list.

 */

int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )

{

    OBJ_DATA *obj;

    int nMatch;



    nMatch = 0;

    for ( obj = list; obj != NULL; obj = obj->next_content )

    {

	if ( obj->pIndexData == pObjIndex )

	    nMatch++;

    }



    return nMatch;

}







/*

 * Move an obj out of a room.

 */

void obj_from_room( OBJ_DATA *obj )

{

    ROOM_INDEX_DATA *in_room;

    CHAR_DATA *ch;



    if ( ( in_room = obj->in_room ) == NULL )

    {

	bug( "obj_from_room: NULL.", 0 );

	return;

    }



    for (ch = in_room->people; ch != NULL; ch = ch->next_in_room)

	if (ch->on == obj)

	    ch->on = NULL;



    if ( obj == in_room->contents )

    {

	in_room->contents = obj->next_content;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = in_room->contents; prev; prev = prev->next_content )

	{

	    if ( prev->next_content == obj )

	    {

		prev->next_content = obj->next_content;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Obj_from_room: obj not found.", 0 );

	    return;

	}

    }



    obj->in_room      = NULL;

    obj->next_content = NULL;

    return;

}







/*

 * Move an obj into a room.

 */

void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )

{

    obj->next_content		= pRoomIndex->contents;

    pRoomIndex->contents	= obj;

    obj->in_room		= pRoomIndex;

    obj->carried_by		= NULL;

    obj->in_obj			= NULL;

    return;

}







/*

 * Move an object into an object.

 */

void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )

{

    obj->next_content		= obj_to->contains;

    obj_to->contains		= obj;

    obj->in_obj			= obj_to;

    obj->in_room		= NULL;

    obj->carried_by		= NULL;

    if (obj_to->pIndexData->vnum == OBJ_VNUM_PIT)

        obj->cost = 0;



    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )

    {

	if ( obj_to->carried_by != NULL )

	{

	    obj_to->carried_by->carry_number += get_obj_number( obj );

	    obj_to->carried_by->carry_weight += get_obj_weight( obj )

		* WEIGHT_MULT(obj_to) / 100;

	}

    }



    return;

}







/*

 * Move an object out of an object.

 */

void obj_from_obj( OBJ_DATA *obj )

{

    OBJ_DATA *obj_from;



    if ( ( obj_from = obj->in_obj ) == NULL )

    {

	bug( "Obj_from_obj: null obj_from.", 0 );

	return;

    }



    if ( obj == obj_from->contains )

    {

	obj_from->contains = obj->next_content;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = obj_from->contains; prev; prev = prev->next_content )

	{

	    if ( prev->next_content == obj )

	    {

		prev->next_content = obj->next_content;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Obj_from_obj: obj not found.", 0 );

	    return;

	}

    }



    obj->next_content = NULL;

    obj->in_obj       = NULL;



    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )

    {

	if ( obj_from->carried_by != NULL )

	{

	    obj_from->carried_by->carry_number -= get_obj_number( obj );

	    obj_from->carried_by->carry_weight -= get_obj_weight( obj )

		* WEIGHT_MULT(obj_from) / 100;

	}

    }



    return;

}







/*

 * Extract an obj from the world.

 */

void extract_obj( OBJ_DATA *obj )

{

    OBJ_DATA *obj_content;

    OBJ_DATA *obj_next;



    if ( obj->in_room != NULL )

	obj_from_room( obj );

    else if ( obj->carried_by != NULL )

	obj_from_char( obj );

    else if ( obj->in_obj != NULL )

	obj_from_obj( obj );



    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )

    {

	obj_next = obj_content->next_content;

	extract_obj( obj_content );

    }



    if ( object_list == obj )

    {

	object_list = obj->next;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = object_list; prev != NULL; prev = prev->next )

	{

	    if ( prev->next == obj )

	    {

		prev->next = obj->next;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );

	    return;

	}

    }



    --obj->pIndexData->count;

    free_obj(obj);

    return;

}







/*

 * Extract a char from the world.

 */

void extract_char( CHAR_DATA *ch, bool fPull )

{

    CHAR_DATA *wch;

    OBJ_DATA *obj;

    OBJ_DATA *obj_next;



    /* doesn't seem to be necessary

    if ( ch->in_room == NULL )

    {

	bug( "Extract_char: NULL.", 0 );

	return;

    }

    */



    nuke_pets(ch);

    ch->pet = NULL; /* just in case */



    if ( fPull )



	die_follower( ch );



    stop_fighting( ch, TRUE );



    for ( obj = ch->carrying; obj != NULL; obj = obj_next )

    {

	obj_next = obj->next_content;

	extract_obj( obj );

    }



    if (ch->in_room != NULL)

        char_from_room( ch );



    /* Death room is set in the clan table now */

    if ( !fPull )

    {

	if ( IS_HERO(ch) )

		char_to_room( ch, get_room_index( ROOM_VNUM_CHAPEL ) );

	else

		char_to_room(ch,get_room_index(clan_table[ch->clan].hall));

	return;

    }



    if ( IS_NPC(ch) )

	--ch->pIndexData->count;



    if ( ch->desc != NULL && ch->desc->original != NULL )

    {

	do_return( ch, "" );

	ch->desc = NULL;

    }



    for ( wch = char_list; wch != NULL; wch = wch->next )

    {

	if ( wch->reply == ch )

	    wch->reply = NULL;

    }



    if ( ch == char_list )

    {

       char_list = ch->next;

    }

    else

    {

	CHAR_DATA *prev;



	for ( prev = char_list; prev != NULL; prev = prev->next )

	{

	    if ( prev->next == ch )

	    {

		prev->next = ch->next;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Extract_char: char not found.", 0 );

	    return;

	}

    }



    if ( ch->desc != NULL )

	ch->desc->character = NULL;

    free_char( ch );

    return;

}







/*

 * Find a char in the room.

 */

CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *rch;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    if ( !str_cmp( arg, "self" ) )

	return ch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )

    {

	if ( !can_see( ch, rch ) || !is_name( arg, rch->name ) )

	    continue;

	if ( ++count == number )

	    return rch;

    }



    return NULL;

}









/*

 * Find a char in the world.

 */

CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *wch;

    int number;

    int count;



    if ( ( wch = get_char_room( ch, argument ) ) != NULL )

	return wch;



    number = number_argument( argument, arg );

    count  = 0;

    for ( wch = char_list; wch != NULL ; wch = wch->next )

    {

	if ( wch->in_room == NULL || !can_see( ch, wch )

	||   !is_name( arg, wch->name ) )

	    continue;

	if ( ++count == number )

	    return wch;

    }



    return NULL;

}







/*

 * Find some object with a given index data.

 * Used by area-reset 'P' command.

 */

OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )

{

    OBJ_DATA *obj;



    for ( obj = object_list; obj != NULL; obj = obj->next )

    {

	if ( obj->pIndexData == pObjIndex )

	    return obj;

    }



    return NULL;

}





/*

 * Find an obj in a list.

 */

OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = list; obj != NULL; obj = obj->next_content )

    {

	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Find an obj in player's inventory.

 */

OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )

    {

	if ( obj->wear_loc == WEAR_NONE

	&&   (can_see_obj( viewer, obj ) )

	&&   is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Find an obj in player's equipment.

 */

OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )

    {

	if ( obj->wear_loc != WEAR_NONE

	&&   can_see_obj( ch, obj )

	&&   is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Find an obj in the room or in inventory.

 */

OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )

{

    OBJ_DATA *obj;



    obj = get_obj_list( ch, argument, ch->in_room->contents );

    if ( obj != NULL )

	return obj;



    if ( ( obj = get_obj_carry( ch, argument, ch ) ) != NULL )

	return obj;



    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )

	return obj;



    return NULL;

}







/*

 * Find an obj in the world.

 */

OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )

	return obj;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = object_list; obj != NULL; obj = obj->next )

    {

	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}



/* deduct cost from a character */



void deduct_cost(CHAR_DATA *ch, int cost)

{

    int silver = 0, gold = 0;



    silver = UMIN(ch->silver,cost);



    if (silver < cost)

    {

	gold = ((cost - silver + 99) / 100);

	silver = cost - 100 * gold;

    }



    ch->gold -= gold;

    ch->silver -= silver;



    if (ch->gold < 0)

    {

	bug("deduct costs: gold %d < 0",ch->gold);

	ch->gold = 0;

    }

    if (ch->silver < 0)

    {

	bug("deduct costs: silver %d < 0",ch->silver);

	ch->silver = 0;

    }

}



void deduct_long_cost(CHAR_DATA *ch, long cost)

{

    long silver = 0, gold = 0;



    silver = UMIN(ch->silver,cost);



    if (silver < cost)

    {

	gold = ((cost - silver + 99) / 100);

	silver = cost - 100 * gold;

    }



    ch->gold -= gold;

    ch->silver -= silver;



    if (ch->gold < 0)

    {

	bug("deduct costs: gold %d < 0",ch->gold);

	ch->gold = 0;

    }

    if (ch->silver < 0)

    {

	bug("deduct costs: silver %d < 0",ch->silver);

	ch->silver = 0;

    }

}



/*

 * Create a 'money' obj.

 */

OBJ_DATA *create_money( int gold, int silver )

{

    char buf[MAX_STRING_LENGTH];

    OBJ_DATA *obj;



    if ( gold < 0 || silver < 0 || (gold == 0 && silver == 0) )

    {

	bug( "Create_money: zero or negative money.",UMIN(gold,silver));

	gold = UMAX(1,gold);

	silver = UMAX(1,silver);

    }



    if (gold == 0 && silver == 1)

    {

	obj = create_object( get_obj_index( OBJ_VNUM_SILVER_ONE ), 0 );

    }

    else if (gold == 1 && silver == 0)

    {

	obj = create_object( get_obj_index( OBJ_VNUM_GOLD_ONE), 0 );

    }

    else if (silver == 0)

    {

        obj = create_object( get_obj_index( OBJ_VNUM_GOLD_SOME ), 0 );

        sprintf( buf, obj->short_descr, gold );

        free_string( obj->short_descr );

        obj->short_descr        = str_dup( buf );

        obj->value[1]           = gold;

        obj->cost               = gold;

	obj->weight		= gold/5;

    }

    else if (gold == 0)

    {

        obj = create_object( get_obj_index( OBJ_VNUM_SILVER_SOME ), 0 );

        sprintf( buf, obj->short_descr, silver );

        free_string( obj->short_descr );

        obj->short_descr        = str_dup( buf );

        obj->value[0]           = silver;

        obj->cost               = silver;

	obj->weight		= silver/20;

    }



    else

    {

	obj = create_object( get_obj_index( OBJ_VNUM_COINS ), 0 );

	sprintf( buf, obj->short_descr, silver, gold );

	free_string( obj->short_descr );

	obj->short_descr	= str_dup( buf );

	obj->value[0]		= silver;

	obj->value[1]		= gold;

	obj->cost		= 100 * gold + silver;

	obj->weight		= gold / 5 + silver / 20;

    }



    return obj;

}







/*

 * Return # of objects which an object counts as.

 * Thanks to Tony Chamberlain for the correct recursive code here.

 */

int get_obj_number( OBJ_DATA *obj )

{

    int number;



    if (obj->item_type == ITEM_CONTAINER || obj->item_type == ITEM_MONEY

    ||  obj->item_type == ITEM_GEM || obj->item_type == ITEM_JEWELRY)

        number = 0;

    else

        number = 1;



    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )

        number += get_obj_number( obj );



    return number;

}





/*

 * Return weight of an object, including weight of contents.

 */

int get_obj_weight( OBJ_DATA *obj )

{

    int weight;

    OBJ_DATA *tobj;



    weight = obj->weight;

    for ( tobj = obj->contains; tobj != NULL; tobj = tobj->next_content )

	weight += get_obj_weight( tobj ) * WEIGHT_MULT(obj) / 100;



    return weight;

}



int get_true_weight(OBJ_DATA *obj)

{

    int weight;



    weight = obj->weight;

    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )

        weight += get_obj_weight( obj );



    return weight;

}



/*

 * True if room is dark.

 */

bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex )
{
    if ( pRoomIndex->light > 0 )
	return FALSE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
	return FALSE;

    if ( weather_info.sunlight == SUN_SET
    ||   weather_info.sunlight == SUN_DARK )
	return TRUE;

    return FALSE;
}





bool is_room_owner(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
    if (ch->level >= MAX_LEVEL)
	return TRUE;

    if (room->owner == NULL || room->owner[0] == '\0')
	return FALSE;

    return is_name(ch->name,room->owner);
}



/*

 * True if room is private.

 */

bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )

{

    CHAR_DATA *rch;

    int count;





    if (pRoomIndex->owner != NULL && pRoomIndex->owner[0] != '\0')

	return TRUE;



    count = 0;

    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )

	count++;



    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )

	return TRUE;



    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )

	return TRUE;



    if ( IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) )

	return TRUE;



    return FALSE;

}



/* visibility on a room -- for entering and exits */

bool can_see_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    extern bool totalwar;

    if (IS_NPC(ch) && ch->master != NULL)
	return TRUE;

    if (IS_SET(pRoomIndex->area->area_flags, AREA_CLOSED)
    && !can_build(ch, pRoomIndex->area))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY)
    &&  get_trust(ch) < MAX_LEVEL)
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_HEROES_ONLY)
    &&  !IS_HERO(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_NEWBIES_ONLY)
    && !(ch->level < 10 || IS_HERO(ch) || IS_SET(ch->act, PLR_HELPER)))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_HELPER)
    && !(IS_HERO(ch) || IS_SET(ch->act, PLR_HELPER)))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_CODER)
    && !(( (ch->level >= SUPREME) && (get_trust(ch) >= SUPREME) ) || IS_SET(ch->olcs, OLC_CODER)))
	return FALSE;


    if (!IS_IMMORTAL(ch) && !totalwar && pRoomIndex->clan && ch->clan != pRoomIndex->clan)
	return FALSE;

    return TRUE;
}

/* Coded by Storm

 * True is a char is flagged OLC_CODER or is SUP+

 */

bool is_coder( CHAR_DATA *ch)

{

	if ((IS_SET(ch->olcs , OLC_CODER)) && (IS_SET(ch->olcs, OLC_BUILDER)) && (ch->pcdata->security > 0))

		return TRUE;



	if ((ch->level >=SUPREME) && get_trust(ch)>=SUPREME)

		return TRUE;



	return FALSE;

}



/*

 * True if char can see victim.

 */

bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
	AFFECT_DATA *pac_ch, *pac_vic;
	int invislvl, detectlvl;
/* RT changed so that WIZ_INVIS has levels */
    if ( ch == victim )
	return TRUE;

    if ( get_trust(ch) < victim->invis_level)
	return FALSE;


    if (get_trust(ch) < victim->incog_level && ch->in_room != victim->in_room)
	return FALSE;

    if ( (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
    ||   (IS_NPC(ch) && IS_IMMORTAL(ch)))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
	return FALSE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED) )
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE)
    &&   !(IS_AFFECTED(ch, AFF_DETECT_INVIS) 
			||IS_AFFECTED2(ch, AFF_IMPROV_DETECT)
			||ch->class == class_lookup("jedi") ) 
	  )
	return FALSE;

    if ( IS_AFFECTED2(victim, AFF_IMPROV_INVIS)  //Grayson New Invis 19 July 2003
    &&   !IS_AFFECTED2(ch, AFF_IMPROV_DETECT)    )  //modified 8 March 2004
    	return FALSE;

    if ( IS_AFFECTED2(victim, AFF_IMPROV_INVIS)  //Grayson New Invis 19 July 2003
    &&   (IS_AFFECTED2(ch, AFF_IMPROV_DETECT)    //Jedi can detect them 2 August 2004
    		||ch->class == class_lookup("jedi"))   )
	{
		invislvl = lvl_affected(victim, gsn_improv_invis);
		if (IS_AFFECTED2(ch, AFF_IMPROV_DETECT))
			detectlvl = lvl_affected(ch, gsn_improv_detect);
		else if (ch->class == class_lookup("jedi"))
			detectlvl = ch->level;
		if (invislvl > detectlvl)
			return FALSE;
	}


   /* sneaking */
    if ( IS_AFFECTED(victim, AFF_SNEAK)
    &&   !IS_AFFECTED(ch,AFF_DETECT_HIDDEN)
    &&   victim->fighting == NULL
    &&   !IS_IMMORTAL(ch)
    &&   ch->class != class_lookup("jedi"))
    {
	int chance;
	chance = get_skill(victim,gsn_sneak);
	chance += get_curr_stat(victim,STAT_AGI) * 3/2;
 	chance -= get_curr_stat(ch,STAT_INT) * 2;
	chance -= ch->level - victim->level * 3/2;

	if (number_percent() < chance)
	    return FALSE;
    }


    if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_HIDDEN)
    &&   victim->fighting == NULL)
	return FALSE;

    return TRUE;
}


/*

 * True if char can see obj.

 */

bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )

{

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )

	return TRUE;



    if ( IS_SET(obj->extra_flags,ITEM_VIS_DEATH))

	return FALSE;



    if ( IS_AFFECTED( ch, AFF_BLIND ) && obj->item_type != ITEM_POTION)

	return FALSE;



    /* if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )

	return TRUE; */



    if ( IS_SET(obj->extra_flags, ITEM_INVIS)

    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )

        return FALSE;



    if ( IS_OBJ_STAT(obj,ITEM_GLOW))

	return TRUE;



    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED) )

        return FALSE;


/* Removed by Urk until I can remember what I was doing here.
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_EYE) && !IS_SET(ch->cyber, CYBER_EYES))
        return FALSE;
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_COMM) && !IS_SET(ch->cyber, CYBER_COMM))
        return FALSE;
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_LEGS) && !IS_SET(ch->cyber, CYBER_LEGS))
        return FALSE;
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_CHEST) && !IS_SET(ch->cyber, CYBER_CHEST))
        return FALSE;
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_REFLEXES) && !IS_SET(ch->cyber, CYBER_REFLEXES))
        return FALSE;
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_MIND) && !IS_SET(ch->cyber, CYBER_MIND))
        return FALSE;
    if ( IS_SET(obj->wear_flags, WEAR_CYBER_STRENGTH) && !IS_SET(ch->cyber, CYBER_STRENGTH))
        return FALSE; */
    return TRUE;

}







/*

 * True if char can drop obj.

 */

bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )

{

    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )

	return TRUE;



    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )

	return TRUE;



    return FALSE;

}





/*

 * Return ascii name of an affect location.

 */

char *affect_loc_name( int location )

{

    switch ( location )
    {
    case APPLY_NONE:		return "none";
    case APPLY_STR:		return "strength";
    case APPLY_DEX:		return "dexterity";
    case APPLY_INT:		return "intelligence";
    case APPLY_WIS:		return "wisdom";
    case APPLY_CON:		return "constitution";
    case APPLY_AGI:		return "agility";
    case APPLY_MCH:		return "mechanical aptitude";
    case APPLY_TCH:		return "technical aptitude";

    case APPLY_SEX:		return "sex";

    case APPLY_CLASS:		return "class";

    case APPLY_LEVEL:		return "level";

    case APPLY_AGE:		return "age";

    case APPLY_MANA:		return "mana";

    case APPLY_HIT:		return "hp";

    case APPLY_MOVE:		return "moves";

    case APPLY_GOLD:		return "gold";

    case APPLY_EXP:		return "experience";

    case APPLY_AC:		return "armor class";

    case APPLY_HITROLL:		return "hit roll";

    case APPLY_DAMROLL:		return "damage roll";
    
    case APPLY_DEFROLL:		return "defense roll";

    case APPLY_SAVES:		return "saves";

    case APPLY_SAVING_ROD:	return "save vs rod";

    case APPLY_SAVING_PETRI:	return "save vs petrification";

    case APPLY_SAVING_BREATH:	return "save vs breath";

    case APPLY_SAVING_SPELL:	return "save vs spell";

    case APPLY_SPELL_AFFECT:	return "none";

    }



    bug( "Affect_location_name: unknown location %d.", location );

    return "(unknown)";

}







/*

 * Return ascii name of an affect bit vector.

 */

char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) strcat( buf, " invisible"     );
    if ( vector & AFF_DETECT_EVIL   ) strcat( buf, " detect_evil"   );
    if ( vector & AFF_DETECT_GOOD   ) strcat( buf, " detect_good"   );
    if ( vector & AFF_DETECT_INVIS  ) strcat( buf, " detect_invis"  );
    if ( vector & AFF_DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_PROTECT_EVIL  ) strcat( buf, " prot_evil"     );
    if ( vector & AFF_PROTECT_GOOD  ) strcat( buf, " prot_good"     );
    if ( vector & AFF_SLEEP         ) strcat( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CHARM         ) strcat( buf, " charm"         );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) strcat( buf, " pass_door"     );
    if ( vector & AFF_BERSERK	    ) strcat( buf, " berserk"	    );
    if ( vector & AFF_CALM	    ) strcat( buf, " calm"	    );
    if ( vector & AFF_HASTE	    ) strcat( buf, " haste"	    );
    if ( vector & AFF_SLOW          ) strcat( buf, " slow"          );
    if ( vector & AFF_PLAGUE	    ) strcat( buf, " plague" 	    );
    if ( vector & AFF_DARK_VISION   ) strcat( buf, " dark_vision"   );
    if ( vector & AFF_FURY	    ) strcat( buf, " fury"	    );
    if ( vector & AFF_DESTRUCTION	) strcat( buf, " destruction"   );
	if ( vector & AFF_REGENERATION	) strcat( buf, " regeneration"   );
    return ( buf[0] != '\0' ) ? buf+1 : "none";

}


/*
 * Return ascii name of an affect bit vector.
 */

char *affect2_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';				//Have I ever mentioned how much I hate ROM?
// new flag declarations here   //Grayson New Affects 9 March 2004
	// if (vector & AFF_TEST_FLAG) strcat(buf, " test_flag");
	if ( vector & AFF_IMPROV_INVIS	) strcat( buf, " improv_invis"   );
	if ( vector & AFF_IMPROV_DETECT	) strcat( buf, " improv_detect"   );
	if ( vector & AFF_DRAGON_POWER	) strcat( buf, " dragon_power"   );
	if ( vector & AFF_DRAGON_ARMOR	) strcat( buf, " dragon_armor"   ); 
	if ( vector & AFF_ENTANGLE	) 	  strcat( buf, " entangle"   );
	if ( vector & AFF_FORCE_ABSORB	) strcat( buf, " force_absorb"   );
	if ( vector & AFF_FORCE_PAIN	) strcat( buf, " force_pain"   );
	if ( vector & AFF_FORCE_RAGE	) strcat( buf, " force_rage"   );
	if ( vector & AFF_FORCE_TRANCE	) strcat( buf, " force_trance"   );
	if ( vector & AFF_FORCE_PAIN	) strcat( buf, " force_pain"   );
	if ( vector & AFF_FORCE_PROTECTION	) strcat( buf, " force_protect"   );
	if ( vector & AFF_STUN			) strcat( buf, " stun"   );
	if ( vector & AFF_PWR_ATTACK	) strcat( buf, " power_attack"   );
	if ( vector & AFF_BASH	) 	  strcat( buf, " bash"   );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}                 

/*
 * Return ascii name of extra flags vector.
 */

char *extra_bit_name( int extra_flags )

{

    static char buf[512];



    buf[0] = '\0';

    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );

    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );

    if ( extra_flags & ITEM_DARK         ) strcat( buf, " dark"         );

    if ( extra_flags & ITEM_LOCK         ) strcat( buf, " lock"         );

    if ( extra_flags & ITEM_EVIL         ) strcat( buf, " evil"         );

    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );

    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );

    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );

    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );

    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );

    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );

    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );

    if ( extra_flags & ITEM_ANTI_MAG ) strcat( buf, " anti-mage" );
    if ( extra_flags & ITEM_ANTI_CLE ) strcat( buf, " anti-cleric" );
    if ( extra_flags & ITEM_ANTI_DRU ) strcat( buf, " anti-druid" );
    if ( extra_flags & ITEM_ANTI_WAR ) strcat( buf, " anti-warrior" );
    if ( extra_flags & ITEM_ANTI_CYB ) strcat( buf, " anti-cyborg" );
    if ( extra_flags & ITEM_ANTI_THI ) strcat( buf, " anti-thief" );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );

    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );

    if ( extra_flags & ITEM_NOPURGE	 ) strcat( buf, " nopurge"	);

    if ( extra_flags & ITEM_VIS_DEATH	 ) strcat( buf, " vis_death"	);

    if ( extra_flags & ITEM_ROT_DEATH	 ) strcat( buf, " rot_death"	);

    if ( extra_flags & ITEM_NOLOCATE	 ) strcat( buf, " no_locate"	);

    if ( extra_flags & ITEM_SELL_EXTRACT ) strcat( buf, " sell_extract" );

    if ( extra_flags & ITEM_BURN_PROOF	 ) strcat( buf, " burn_proof"	);

    if ( extra_flags & ITEM_NOUNCURSE	 ) strcat( buf, " no_uncurse"	);

    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



/* return ascii name of an act vector */

char *act_bit_name( int act_flags )

{

    static char buf[512];



    buf[0] = '\0';



    if (IS_SET(act_flags,ACT_IS_NPC))

    {

 	strcat(buf," npc");

    	if (act_flags & ACT_SENTINEL 	) strcat(buf, " sentinel");

    	if (act_flags & ACT_SCAVENGER	) strcat(buf, " scavenger");

	if (act_flags & ACT_AGGRESSIVE	) strcat(buf, " aggressive");

	if (act_flags & ACT_STAY_AREA	) strcat(buf, " stay_area");

	if (act_flags & ACT_WIMPY	) strcat(buf, " wimpy");

	if (act_flags & ACT_PET		) strcat(buf, " pet");

	if (act_flags & ACT_TRAIN	) strcat(buf, " train");

	if (act_flags & ACT_PRACTICE	) strcat(buf, " practice");

	if (act_flags & ACT_UNDEAD	) strcat(buf, " undead");

	if (act_flags & ACT_CLERIC	) strcat(buf, " cleric");

	if (act_flags & ACT_MAGE	) strcat(buf, " mage");

	if (act_flags & ACT_THIEF	) strcat(buf, " thief");

	if (act_flags & ACT_WARRIOR	) strcat(buf, " warrior");

	if (act_flags & ACT_NOALIGN	) strcat(buf, " no_align");

	if (act_flags & ACT_NOPURGE	) strcat(buf, " no_purge");

	if (act_flags & ACT_IS_HEALER	) strcat(buf, " healer");

	if (act_flags & ACT_IS_CHANGER  ) strcat(buf, " changer");

	if (act_flags & ACT_GAIN	) strcat(buf, " skill_train");

	if (act_flags & ACT_UPDATE_ALWAYS) strcat(buf," update_always");

    }

    else

    {

	strcat(buf," player");

	if (act_flags & PLR_AUTOASSIST	) strcat(buf, " autoassist");

	if (act_flags & PLR_AUTOEXIT		) strcat(buf, " autoexit");

	if (act_flags & PLR_AUTOLOOT		) strcat(buf, " autoloot");

	if (act_flags & PLR_AUTOSAC		) strcat(buf, " autosac");

	if (act_flags & PLR_AUTOGOLD		) strcat(buf, " autogold");

	if (act_flags & PLR_AUTOSPLIT		) strcat(buf, " autosplit");

	if (act_flags & PLR_AUTOWIZI		) strcat(buf, " autowizi");

	if (act_flags & PLR_HOLYLIGHT		) strcat(buf, " holy_light");

	if (act_flags & PLR_CANLOOT		) strcat(buf, " loot_corpse");

	if (act_flags & PLR_NOSUMMON		) strcat(buf, " no_summon");

	if (act_flags & PLR_NOFOLLOW		) strcat(buf, " no_follow");

	if (act_flags & PLR_FREEZE		) strcat(buf, " frozen");

	if (act_flags & PLR_COLOUR		) strcat(buf, " colour");

	if (act_flags & PLR_THIEF		) strcat(buf, " thief");

	if (act_flags & PLR_KILLER		) strcat(buf, " killer");

	if (act_flags & PLR_WANTED		) strcat(buf, " wanted");

	if (act_flags & PLR_CLANLEADER	) strcat(buf, " clan_leader");



    }

    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *comm_bit_name(int comm_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (comm_flags & COMM_QUIET		) strcat(buf, " quiet");

    if (comm_flags & COMM_DEAF		) strcat(buf, " deaf");

    if (comm_flags & COMM_NOWIZ		) strcat(buf, " no_wiz");

    if (comm_flags & COMM_NOSUP		) strcat(buf, " no_sup");

    if (comm_flags & COMM_NOBUILDER		) strcat(buf, " no_builder");

    if (comm_flags & COMM_NOAUCTION		) strcat(buf, " no_auction");

    if (comm_flags & COMM_NOFLAME		) strcat(buf, " no_flame");

    if (comm_flags & COMM_NOOOC		) strcat(buf, " no_ooc");

    if (comm_flags & COMM_NOGOSSIP		) strcat(buf, " no_gossip");

    if (comm_flags & COMM_NOQUESTION	) strcat(buf, " no_question");

    if (comm_flags & COMM_NOMUSIC		) strcat(buf, " no_music");

    if (comm_flags & COMM_NOQUOTE		) strcat(buf, " no_quote");

    if (comm_flags & COMM_SHOUTSOFF		) strcat(buf, " shout_off");

    if (comm_flags & COMM_COMPACT		) strcat(buf, " compact");

    if (comm_flags & COMM_BRIEF		) strcat(buf, " brief");

    if (comm_flags & COMM_PROMPT		) strcat(buf, " prompt");

    if (comm_flags & COMM_COMBINE		) strcat(buf, " combine");

    if (comm_flags & COMM_TELNET_GA		) strcat(buf, " telnet_ga");

    if (comm_flags & COMM_SHOW_AFFECTS	) strcat(buf, " show_affects");

    if (comm_flags & COMM_NOGRATS		) strcat(buf, " no_grats");

    if (comm_flags & COMM_NOEMOTE		) strcat(buf, " no_emote");

    if (comm_flags & COMM_NOSHOUT		) strcat(buf, " no_shout");

    if (comm_flags & COMM_NOTELL		) strcat(buf, " no_tell");

    if (comm_flags & COMM_NOCHANNELS	) strcat(buf, " no_channels");

    if (comm_flags & COMM_SNOOP_PROOF	) strcat(buf, " snoop_proof");

    if (comm_flags & COMM_AFK			) strcat(buf, " afk");





    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *info_bit_name(int info_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (info_flags & INFO_NOWATCH		) strcat(buf, " no_watch");

    if (info_flags & INFO_NONEWNOTE		) strcat(buf, " no_new_note");

    if (info_flags & INFO_NOBOUNTY		) strcat(buf, " no_bounty");

    if (info_flags & INFO_NONEWBIE		) strcat(buf, " no_newbie");

    if (info_flags & INFO_NOANGEL		) strcat(buf, " no_angel");

    if (info_flags & INFO_NOHELPER		) strcat(buf, " no_helper");

    if (info_flags & INFO_NOTIPS		) strcat(buf, " no_tips");

    if (info_flags & INFO_CLANGM		) strcat(buf, " clan_gm");

    if (info_flags & INFO_CLANTRAINER	) strcat(buf, " clan_trainer");

    if (info_flags & INFO_CLANENFORCER	) strcat(buf, " clan_enforcer");

    if (info_flags & INFO_GHOST		) strcat(buf, " ghost");

    if (info_flags & INFO_NOINFO          ) strcat(buf, " no_info");

    if (info_flags & INFO_NOIDLE		) strcat(buf, " no_idle");

    if (info_flags & INFO_NOCODE		) strcat(buf, " no_code");	/* Storm */

    if (info_flags & INFO_IMMMULTI		) strcat(buf, " imm_multi");

    if (info_flags & INFO_BOZO		) strcat(buf, " bozo");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *olcs_bit_name(int olcs_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (olcs_flags & OLC_BUILDER		) strcat(buf, " builder");

    if (olcs_flags & OLC_ROOM			) strcat(buf, " rooms");

    if (olcs_flags & OLC_MOB			) strcat(buf, " mobs");

    if (olcs_flags & OLC_OBJ			) strcat(buf, " objects");

    if (olcs_flags & OLC_RESET		) strcat(buf, " resets");

    if (olcs_flags & OLC_ADMIN		) strcat(buf, " admin");

    if (olcs_flags & OLC_CLANADMIN		) strcat(buf, " clan_admin");

    if (olcs_flags & OLC_INSPECTOR		) strcat(buf, " inspector");

    if (olcs_flags & OLC_CODER 		) strcat(buf, " coder");		/* Storm */



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *imm_bit_name(int imm_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (imm_flags & IMM_SUMMON		) strcat(buf, " summon");

    if (imm_flags & IMM_CHARM		) strcat(buf, " charm");

    if (imm_flags & IMM_MAGIC		) strcat(buf, " magic");

    if (imm_flags & IMM_WEAPON		) strcat(buf, " weapon");

    if (imm_flags & IMM_BASH		) strcat(buf, " blunt");

    if (imm_flags & IMM_PIERCE		) strcat(buf, " piercing");

    if (imm_flags & IMM_SLASH		) strcat(buf, " slashing");

    if (imm_flags & IMM_FIRE		) strcat(buf, " fire");

    if (imm_flags & IMM_COLD		) strcat(buf, " cold");

    if (imm_flags & IMM_LIGHTNING	) strcat(buf, " lightning");

    if (imm_flags & IMM_ACID		) strcat(buf, " acid");

    if (imm_flags & IMM_POISON		) strcat(buf, " poison");

    if (imm_flags & IMM_NEGATIVE	) strcat(buf, " negative");

    if (imm_flags & IMM_HOLY		) strcat(buf, " holy");

    if (imm_flags & IMM_ENERGY		) strcat(buf, " energy");

    if (imm_flags & IMM_MENTAL		) strcat(buf, " mental");

    if (imm_flags & IMM_DISEASE	) strcat(buf, " disease");

    if (imm_flags & IMM_DROWNING	) strcat(buf, " drowning");

    if (imm_flags & IMM_LIGHT		) strcat(buf, " light");

    if (imm_flags & VULN_IRON		) strcat(buf, " iron");

    if (imm_flags & VULN_WOOD		) strcat(buf, " wood");

    if (imm_flags & VULN_SILVER	) strcat(buf, " silver");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *wear_bit_name(int wear_flags)

{

    static char buf[512];



    buf [0] = '\0';

    if (wear_flags & ITEM_TAKE		) strcat(buf, " take");

    if (wear_flags & ITEM_WEAR_FINGER	) strcat(buf, " finger");

    if (wear_flags & ITEM_WEAR_NECK	) strcat(buf, " neck");

    if (wear_flags & ITEM_WEAR_BODY	) strcat(buf, " torso");

    if (wear_flags & ITEM_WEAR_HEAD	) strcat(buf, " head");

    if (wear_flags & ITEM_WEAR_LEGS	) strcat(buf, " legs");

    if (wear_flags & ITEM_WEAR_FEET	) strcat(buf, " feet");

    if (wear_flags & ITEM_WEAR_HANDS	) strcat(buf, " hands");

    if (wear_flags & ITEM_WEAR_ARMS	) strcat(buf, " arms");

    if (wear_flags & ITEM_WEAR_SHIELD	) strcat(buf, " shield");

    if (wear_flags & ITEM_WEAR_ABOUT	) strcat(buf, " body");

    if (wear_flags & ITEM_WEAR_WAIST	) strcat(buf, " waist");

    if (wear_flags & ITEM_WEAR_WRIST	) strcat(buf, " wrist");

    if (wear_flags & ITEM_WIELD		) strcat(buf, " wield");

    if (wear_flags & ITEM_HOLD		) strcat(buf, " hold");

    if (wear_flags & ITEM_NO_SAC	) strcat(buf, " nosac");

    if (wear_flags & ITEM_WEAR_FLOAT	) strcat(buf, " float");

    if (wear_flags & ITEM_CYBER_EYE      ) strcat(buf, " cybereye");
    if (wear_flags & ITEM_CYBER_COMM     ) strcat(buf, " cybercomm");
    if (wear_flags & ITEM_CYBER_LEGS     ) strcat(buf, " cyberleg");
    if (wear_flags & ITEM_CYBER_CHEST    ) strcat(buf, " cyberchest");
    if (wear_flags & ITEM_CYBER_REFLEXES ) strcat(buf, " cyberreflex");
    if (wear_flags & ITEM_CYBER_MIND     ) strcat(buf, " cybercomp");
    if (wear_flags & ITEM_CYBER_STRENGTH ) strcat(buf, " cyberstr");

    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *form_bit_name(int form_flags)

{

    static char buf[512];



    buf[0] = '\0';

    if (form_flags & FORM_POISON	) strcat(buf, " poison");

    else if (form_flags & FORM_EDIBLE	) strcat(buf, " edible");

    if (form_flags & FORM_MAGICAL	) strcat(buf, " magical");

    if (form_flags & FORM_INSTANT_DECAY	) strcat(buf, " instant_rot");

    if (form_flags & FORM_OTHER		) strcat(buf, " other");

    if (form_flags & FORM_ANIMAL	) strcat(buf, " animal");

    if (form_flags & FORM_SENTIENT	) strcat(buf, " sentient");

    if (form_flags & FORM_UNDEAD	) strcat(buf, " undead");

    if (form_flags & FORM_CONSTRUCT	) strcat(buf, " construct");

    if (form_flags & FORM_MIST		) strcat(buf, " mist");

    if (form_flags & FORM_INTANGIBLE	) strcat(buf, " intangible");

    if (form_flags & FORM_BIPED		) strcat(buf, " biped");

    if (form_flags & FORM_CENTAUR	) strcat(buf, " centaur");

    if (form_flags & FORM_INSECT	) strcat(buf, " insect");

    if (form_flags & FORM_SPIDER	) strcat(buf, " spider");

    if (form_flags & FORM_CRUSTACEAN	) strcat(buf, " crustacean");

    if (form_flags & FORM_WORM		) strcat(buf, " worm");

    if (form_flags & FORM_BLOB		) strcat(buf, " blob");

    if (form_flags & FORM_MAMMAL	) strcat(buf, " mammal");

    if (form_flags & FORM_BIRD		) strcat(buf, " bird");

    if (form_flags & FORM_REPTILE	) strcat(buf, " reptile");

    if (form_flags & FORM_SNAKE		) strcat(buf, " snake");

    if (form_flags & FORM_DRAGON	) strcat(buf, " dragon");

    if (form_flags & FORM_AMPHIBIAN	) strcat(buf, " amphibian");

    if (form_flags & FORM_FISH		) strcat(buf, " fish");

    if (form_flags & FORM_COLD_BLOOD 	) strcat(buf, " cold_blooded");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *part_bit_name(int part_flags)

{

    static char buf[512];



    buf[0] = '\0';

    if (part_flags & PART_HEAD		) strcat(buf, " head");

    if (part_flags & PART_ARMS		) strcat(buf, " arms");

    if (part_flags & PART_LEGS		) strcat(buf, " legs");

    if (part_flags & PART_HEART		) strcat(buf, " heart");

    if (part_flags & PART_BRAINS	) strcat(buf, " brains");

    if (part_flags & PART_GUTS		) strcat(buf, " guts");

    if (part_flags & PART_HANDS		) strcat(buf, " hands");

    if (part_flags & PART_FEET		) strcat(buf, " feet");

    if (part_flags & PART_FINGERS	) strcat(buf, " fingers");

    if (part_flags & PART_EAR		) strcat(buf, " ears");

    if (part_flags & PART_EYE		) strcat(buf, " eyes");

    if (part_flags & PART_LONG_TONGUE	) strcat(buf, " long_tongue");

    if (part_flags & PART_EYESTALKS	) strcat(buf, " eyestalks");

    if (part_flags & PART_TENTACLES	) strcat(buf, " tentacles");

    if (part_flags & PART_FINS		) strcat(buf, " fins");

    if (part_flags & PART_WINGS		) strcat(buf, " wings");

    if (part_flags & PART_TAIL		) strcat(buf, " tail");

    if (part_flags & PART_CLAWS		) strcat(buf, " claws");

    if (part_flags & PART_FANGS		) strcat(buf, " fangs");

    if (part_flags & PART_HORNS		) strcat(buf, " horns");

    if (part_flags & PART_SCALES	) strcat(buf, " scales");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *cyber_bit_name(int cyber_flags)

{

    static char buf[512];



    buf[0] = '\0';

    if (cyber_flags & CYBER_COMM	) strcat(buf, " cyber_comm");

    if (cyber_flags & CYBER_EYES	) strcat(buf, " cyber_optic");

    if (cyber_flags & CYBER_LEGS	) strcat(buf, " cyber_legs");

    if (cyber_flags & CYBER_CHEST	) strcat(buf, " cyber_chest");

    if (cyber_flags & CYBER_REFLEXES	) strcat(buf, " cyber_reflex");

    if (cyber_flags & CYBER_MIND	) strcat(buf, " cyber_proc");

    if (cyber_flags & CYBER_STRENGTH	) strcat(buf, " cyber_str");

    if (cyber_flags & CYBER_SURGEON    ) strcat(buf, " cyber_surgeon");

    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *weapon_bit_name(int weapon_flags)

{

    static char buf[512];



    buf[0] = '\0';

    if (weapon_flags & WEAPON_FLAMING	) strcat(buf, " flaming");

    if (weapon_flags & WEAPON_FROST	) strcat(buf, " frost");

    if (weapon_flags & WEAPON_VAMPIRIC	) strcat(buf, " vampiric");

    if (weapon_flags & WEAPON_SHARP	) strcat(buf, " sharp");

    if (weapon_flags & WEAPON_VORPAL	) strcat(buf, " vorpal");

    if (weapon_flags & WEAPON_TWO_HANDS ) strcat(buf, " two-handed");

    if (weapon_flags & WEAPON_SHOCKING 	) strcat(buf, " shocking");

    if (weapon_flags & WEAPON_POISON	) strcat(buf, " poison");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



char *cont_bit_name( int cont_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (cont_flags & CONT_CLOSEABLE	) strcat(buf, " closable");

    if (cont_flags & CONT_PICKPROOF	) strcat(buf, " pickproof");

    if (cont_flags & CONT_CLOSED	) strcat(buf, " closed");

    if (cont_flags & CONT_LOCKED	) strcat(buf, " locked");



    return (buf[0] != '\0' ) ? buf+1 : "none";

}





char *off_bit_name(int off_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (off_flags & OFF_AREA_ATTACK	) strcat(buf, " area attack");

    if (off_flags & OFF_BACKSTAB	) strcat(buf, " backstab");

    if (off_flags & OFF_BASH		) strcat(buf, " bash");

    if (off_flags & OFF_BERSERK		) strcat(buf, " berserk");

    if (off_flags & OFF_DISARM		) strcat(buf, " disarm");

    if (off_flags & OFF_DODGE		) strcat(buf, " dodge");

    if (off_flags & OFF_FADE		) strcat(buf, " fade");

    if (off_flags & OFF_FAST		) strcat(buf, " fast");

    if (off_flags & OFF_KICK		) strcat(buf, " kick");

    if (off_flags & OFF_SPINKICK		) strcat(buf, " spinkick");

    if (off_flags & OFF_GOUGE		) strcat(buf, " gouge");

    if (off_flags & OFF_KICK_DIRT	) strcat(buf, " kick_dirt");

    if (off_flags & OFF_PARRY		) strcat(buf, " parry");

    if (off_flags & OFF_RESCUE		) strcat(buf, " rescue");

    if (off_flags & OFF_TAIL		) strcat(buf, " tail");

    if (off_flags & OFF_TRIP		) strcat(buf, " trip");

    if (off_flags & OFF_CRUSH		) strcat(buf, " crush");

    if (off_flags & OFF_FURY		) strcat(buf, " fury");

    if (off_flags & ASSIST_ALL		) strcat(buf, " assist_all");

    if (off_flags & ASSIST_ALIGN	) strcat(buf, " assist_align");

    if (off_flags & ASSIST_RACE		) strcat(buf, " assist_race");

    if (off_flags & ASSIST_PLAYERS	) strcat(buf, " assist_players");

    if (off_flags & ASSIST_GUARD	) strcat(buf, " assist_guard");

    if (off_flags & ASSIST_VNUM		) strcat(buf, " assist_vnum");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}

/* -Ferg
 * OLCINSPECTORS can_build an area if its Security is < 9, the max.
 * Security 9 areas are finished areas.
 * Admins can_build any area. All others must either have a security
 * greater or equal than area security, or be that area's specific builder.
 * Ferg-
 */

bool can_build( CHAR_DATA *ch, AREA_DATA *area )

{

	if ( !IS_BUILDER(ch) )						return FALSE;

	if ( IS_SWITCHED(ch) )						return FALSE;

	if ( IS_OLCADMIN(ch) )						return TRUE;

	if ( IS_OLCINSPECTOR(ch)  && area->security < 9 )					return TRUE;

	if ( ch->pcdata->security >= area->security )		return TRUE;

	if ( strstr( area->builders, ch->name ) )			return TRUE;

	if ( strstr( area->builders, "All" ) )			return TRUE;



	return FALSE;

}


/* -Ferg
 * IS_OLCINSPECTOR in the following 3 functions was IS_OLCADMIN. I'm trying to
 * make a reason for this abiguous inspector flag to exist IS_OLCINSPECTOR is true
 * if the ch is an inspector or an admin, so this should limit admin access. I see
 * Inspectors as being sort of "admin helpers" who are able to go through and inspect
 * an area for an admin before the admin gives out reset abilities.
 * Ferg-
 */
bool can_redit( CHAR_DATA *ch, AREA_DATA *area )

{

	if ( !can_build(ch, area) )						return FALSE;

	if ( IS_OLCINSPECTOR(ch) )							return TRUE;

	if ( IS_SET(ch->in_room->area->area_flags, OLC_PROTECT) )	return FALSE;

	if ( IS_SET(ch->olcs, OLC_ROOM) )					return TRUE;



	return FALSE;

}



bool can_medit( CHAR_DATA *ch, AREA_DATA *area )

{

	if ( !can_build(ch, area) )						return FALSE;

	if ( IS_OLCINSPECTOR(ch) )							return TRUE;

	if ( IS_SET(ch->in_room->area->area_flags, OLC_PROTECT) )	return FALSE;

	if ( IS_SET(ch->olcs, OLC_MOB) )					return TRUE;



	return FALSE;

}



bool can_oedit( CHAR_DATA *ch, AREA_DATA *area )

{

	if ( !can_build(ch, area) )						return FALSE;

	if ( IS_OLCINSPECTOR(ch) )							return TRUE;

	if ( IS_SET(ch->in_room->area->area_flags, OLC_PROTECT) )	return FALSE;

	if ( IS_SET(ch->olcs, OLC_OBJ) )					return TRUE;



	return FALSE;

}



bool can_reset( CHAR_DATA *ch, AREA_DATA *area )

{

	if ( !can_build(ch, area) )						return FALSE;

	if ( IS_OLCADMIN(ch) )							return TRUE;

	if ( IS_SET(ch->in_room->area->area_flags, OLC_PROTECT) )	return FALSE;

	if ( IS_SET(ch->olcs, OLC_RESET) )					return TRUE;



	return FALSE;

}



bool can_aedit( CHAR_DATA *ch, AREA_DATA *area )

{
	if ( !can_build(ch, area) )						return FALSE;

	if ( IS_OLCADMIN(ch) )							return TRUE;

	if ( IS_SET(ch->in_room->area->area_flags, OLC_PROTECT) )	return FALSE;

	/*if ( can_build(ch, area) )						return TRUE;*/  /* Commented out to disallow anyone but admins to aedit -Ferg */



	return FALSE;

}



char *PERS( CHAR_DATA *ch, CHAR_DATA *looker )

{

	char	buf		[MAX_STRING_LENGTH];



	if ( IS_NPC(ch) )

	{

		if ( can_see( looker, ch ) )

		{

			return ch->short_descr;

		}

		else

		{

			return "someone";

		}

	}

	else

	{

		if ( can_see( looker, ch ) )

		{

			sprintf( buf, "%s%s%s%s%s{w",

				( is_ploaded( ch )		? "{C[{cPload{C]{w " 		: "" ),

				( is_linkdead( ch )		? "{r[{yLink Dead{r]{w "	: "" ),

				( IS_SET(ch->info,INFO_GHOST) ? "The Ghost of " 		: "" ),

				( IS_IMMORTAL(ch)			? "{y"				: "" ),

				ch->name );



			return buf;

		}

		else

		{

			if ( IS_IMMORTAL(ch) )

			{

				return "{yAn Immortal{w";

			}

			else

			{

				return "someone";

			}

		}

	}



	return "someone";

}



bool is_aff_age( CHAR_DATA *ch )

{

	if (		is_affected(ch, gsn_ageaffect_str)

		|| 	is_affected(ch, gsn_ageaffect_int)

		|| 	is_affected(ch, gsn_ageaffect_wis)

		|| 	is_affected(ch, gsn_ageaffect_dex)

		|| 	is_affected(ch, gsn_ageaffect_con)

		|| 	is_affected(ch, gsn_ageaffect_hitroll)

		|| 	is_affected(ch, gsn_ageaffect_damroll)

		|| 	is_affected(ch, gsn_ageaffect_hit)

		|| 	is_affected(ch, gsn_ageaffect_mana)

		|| 	is_affected(ch, gsn_ageaffect_move)

		|| 	is_affected(ch, gsn_ageaffect_ac) )

	{

		return TRUE;

	}

	else

	{

		return FALSE;

	}



	return FALSE;

}



bool is_aff_cur_age( CHAR_DATA *ch )

{

	AFFECT_DATA *afp;

	bool aff_age;



	aff_age	= FALSE;



	for ( afp = ch->affected; afp != NULL; afp = afp->next )

	{

		if ( 		((afp->type == gsn_ageaffect_str) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_int) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_dex) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_con) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_wis) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_hitroll) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_damroll) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_mana) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_hit) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_move) && (afp->level == get_age(ch)))

			|| 	((afp->type == gsn_ageaffect_ac) && (afp->level == get_age(ch))) )

		{

			aff_age	= TRUE;

		}

	}



	if ( aff_age )

		return TRUE;

	else

		return FALSE;



	return FALSE;

}





char *icf_her_bit_name(int icf_her_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (icf_her_flags & HER_NOCHANNELS	) strcat(buf, " no_channels");

    if (icf_her_flags & HER_NOEMOTE		) strcat(buf, " no_emote");

    if (icf_her_flags & HER_NOSHOUT		) strcat(buf, " no_shout");

    if (icf_her_flags & HER_NOTELL		) strcat(buf, " no_tell");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}





char *icf_ang_bit_name(int icf_ang_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (icf_ang_flags & ANG_CLANADMIN		) strcat(buf, " clanadmin");

    if (icf_ang_flags & ANG_DISCONNECT		) strcat(buf, " disconnect");

    if (icf_ang_flags & ANG_HACK			) strcat(buf, " hack");

    if (icf_ang_flags & ANG_FREEZE			) strcat(buf, " freeze");

    if (icf_ang_flags & ANG_FORCE			) strcat(buf, " force");

    if (icf_ang_flags & ANG_LOAD			) strcat(buf, " load");

    if (icf_ang_flags & ANG_NEWLOCK			) strcat(buf, " newlock");

    if (icf_ang_flags & ANG_PECHO			) strcat(buf, " pecho");

    if (icf_ang_flags & ANG_PARDON			) strcat(buf, " pardon");

    if (icf_ang_flags & ANG_PURGE			) strcat(buf, " purge");

    if (icf_ang_flags & ANG_RESTORE			) strcat(buf, " restore");

    if (icf_ang_flags & ANG_TRANSFER		) strcat(buf, " transfer");

    if (icf_ang_flags & ANG_AT			) strcat(buf, " at");

    if (icf_ang_flags & ANG_GECHO			) strcat(buf, " gecho");

    if (icf_ang_flags & ANG_LOG			) strcat(buf, " log");

    if (icf_ang_flags & ANG_SMOTE			) strcat(buf, " smote");

    if (icf_ang_flags & ANG_PEACE			) strcat(buf, " peace");

    if (icf_ang_flags & ANG_ECHO			) strcat(buf, " echo");

    if (icf_ang_flags & ANG_RETURN			) strcat(buf, " return");

    if (icf_ang_flags & ANG_STRING			) strcat(buf, " string");

    if (icf_ang_flags & ANG_SWITCH			) strcat(buf, " switch");

    if (icf_ang_flags & ANG_ZECHO			) strcat(buf, " zecho");

    if (icf_ang_flags & ANG_CLONE			) strcat(buf, " clone");

    if (icf_ang_flags & ANG_SLAY			) strcat(buf, " slay");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}





char *icf_stat_bit_name(int icf_stat_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (icf_stat_flags & STAT_MEMORY		) strcat(buf, " memory");

    if (icf_stat_flags & STAT_MWHERE		) strcat(buf, " mwhere");

    if (icf_stat_flags & STAT_OWHERE		) strcat(buf, " owhere");

    if (icf_stat_flags & STAT_RWHERE		) strcat(buf, " rwhere");

    if (icf_stat_flags & STAT_STAT			) strcat(buf, " stat");

    if (icf_stat_flags & STAT_ASTAT			) strcat(buf, " astat");

    if (icf_stat_flags & STAT_SLOOKUP		) strcat(buf, " slookup");

    if (icf_stat_flags & STAT_VNUM			) strcat(buf, " vnum");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}





char *icf_sup_bit_name(int icf_sup_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (icf_sup_flags & SUP_PERMBAN			) strcat(buf, " permban");

    if (icf_sup_flags & SUP_PROCESS			) strcat(buf, " process");

    if (icf_sup_flags & SUP_PROTECT			) strcat(buf, " protect");

    if (icf_sup_flags & SUP_REBOOT			) strcat(buf, " reboot");

    if (icf_sup_flags & SUP_SET			) strcat(buf, " set");

    if (icf_sup_flags & SUP_SHUTDOWN		) strcat(buf, " shutdown");

    if (icf_sup_flags & SUP_SOCKETS			) strcat(buf, " sockets");

    if (icf_sup_flags & SUP_WIZLOCK			) strcat(buf, " wizlock");

    if (icf_sup_flags & SUP_BETALOCK		) strcat(buf, " betalock");

    if (icf_sup_flags & SUP_TOTALWAR		) strcat(buf, " totalwar");

    if (icf_sup_flags & SUP_ADDLAG			) strcat(buf, " addlag");

    if (icf_sup_flags & SUP_BONUS			) strcat(buf, " bonus");

    if (icf_sup_flags & SUP_ALLOW			) strcat(buf, " allow");

    if (icf_sup_flags & SUP_BAN			) strcat(buf, " ban");

    if (icf_sup_flags & SUP_DENY			) strcat(buf, " deny");

    if (icf_sup_flags & SUP_FLAG			) strcat(buf, " flag");

    if (icf_sup_flags & SUP_OLCLOCK			) strcat(buf, " olclock");

    if (icf_sup_flags & SUP_IPADDRESS		) strcat(buf, " ipaddress");

    if (icf_sup_flags & SUP_UNLOCKDB		) strcat(buf, " unlockdb");

    if (icf_sup_flags & SUP_UNLOCKSPOOL		) strcat(buf, " unlockspool");

    if (icf_sup_flags & SUP_SNOOP			) strcat(buf, " snoop");

    if (icf_sup_flags & SUP_SNOOPLIST		) strcat(buf, " snooplist");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



int get_econ_value( char *argument )

{

	int count;

	int econ_value;

	char wnetbuf[MAX_STRING_LENGTH];

	char arg[MAX_INPUT_LENGTH];



	econ_value = 1;

	argument = one_argument( argument, arg );



	for ( count = 0; economy_table[count].name != NULL; ++count)

	{

		if ( !str_prefix( economy_table[count].name , arg) )

		{

			econ_value = economy_table[count].value;

		}

	}



	/* DEBUG */

	sprintf( wnetbuf, "get_econ_value :: {cArgument{w : {y%s {w:: {gValue{w : {y%d{w",

		arg, econ_value  );

	wiznet(wnetbuf,NULL,NULL,WIZ_DEBUG,0,0);



	return econ_value;

}

int get_combat_value( char *argument )

{

	int count;

	int combat_value;

	char wnetbuf[MAX_STRING_LENGTH];

	char arg[MAX_INPUT_LENGTH];



	combat_value = 1;

	argument = one_argument( argument, arg );



	for ( count = 0; combatmod_table[count].name != NULL; ++count)

	{

		if ( !str_prefix( combatmod_table[count].name , arg) )

		{

			combat_value = combatmod_table[count].value;

		}

	}



	/* DEBUG */

	sprintf( wnetbuf, "get_combat_value :: {cArgument{w : {y%s {w:: {gValue{w : {y%d{w",

		arg, combat_value  );

	wiznet(wnetbuf,NULL,NULL,WIZ_DEBUG,0,0);



	return combat_value;

}

char *room_bit_name(int room_flags)

{

    static char buf[512];



    buf[0] = '\0';



    if (room_flags & ROOM_DARK               ) strcat(buf, " dark");

    if (room_flags & ROOM_NO_MOB             ) strcat(buf, " no_mob");

    if (room_flags & ROOM_INDOORS            ) strcat(buf, " indoors");

    if (room_flags & ROOM_PRIVATE            ) strcat(buf, " private");

    if (room_flags & ROOM_SAFE               ) strcat(buf, " safe");

    if (room_flags & ROOM_SOLITARY           ) strcat(buf, " solitary");

    if (room_flags & ROOM_PET_SHOP           ) strcat(buf, " pet_shop");

    if (room_flags & ROOM_NO_RECALL          ) strcat(buf, " no_recall");

    if (room_flags & ROOM_IMP_ONLY           ) strcat(buf, " imp");

    if (room_flags & ROOM_GODS_ONLY          ) strcat(buf, " gods");

    if (room_flags & ROOM_HEROES_ONLY        ) strcat(buf, " heroes");

    if (room_flags & ROOM_NEWBIES_ONLY       ) strcat(buf, " newbies");

    if (room_flags & ROOM_LAW                ) strcat(buf, " law");

    if (room_flags & ROOM_NOWHERE            ) strcat(buf, " nowhere");

    if (room_flags & ROOM_BANK		   ) strcat(buf, " bank");

    if (room_flags & ROOM_ARENA		   ) strcat(buf, " arena");

    if (room_flags & ROOM_SUSTAIN		   ) strcat(buf, " sustain");

    if (room_flags & ROOM_NOPK		   ) strcat(buf, " pk_safe");

    if (room_flags & ROOM_ENGRAVING		   ) strcat(buf, " engraver");

    if (room_flags & ROOM_HELPER		   ) strcat(buf, " helper");

    if (room_flags & ROOM_CODER		   ) strcat(buf, " coder");



    return ( buf[0] != '\0' ) ? buf+1 : "none";

}





bool is_ploaded ( CHAR_DATA *ch )

{

	if ( IS_NPC(ch) )

	{

		return FALSE;

	}



	if ( ch->pcdata->ploaded )

	{

		return TRUE;

	}



	return FALSE;

}





bool is_linkdead ( CHAR_DATA *ch )

{

	char buf[MAX_STRING_LENGTH];



	if ( IS_NPC(ch) )

	{

		return FALSE;

	}



	if ( is_ploaded( ch ) )

	{

		if ( !ch->pcdata->linkdead )

		{

			ch->pcdata->linkdead	= TRUE;

			sprintf( buf, "%s had PLOAD set to TRUE without LINKDEAD set to TRUE", ch->name );

			bug( buf, 0 );

		}

		return TRUE;

	}



	if ( ch->desc == NULL )

	{

		if ( !ch->pcdata->linkdead )

		{

			ch->pcdata->linkdead	= TRUE;

			sprintf( buf, "%s had a NULL Descriptor without LINKDEAD set to TRUE", ch->name );

			bug( buf, 0 );

		}

		return TRUE;

	}



	if ( ch->pcdata->linkdead )

	{

		return TRUE;

	}



	return FALSE;

}



bool    can_use_skill( CHAR_DATA *ch, int sn )

{

	bool	can_do;

	int	cnt;



	if (sn < -1 || sn > MAX_SKILL)

	{

		bug("Bad sn %d in can_use_skill.",sn);

		return FALSE;

	}



	if ( skill_table[sn].name == NULL )

	{

		return FALSE;

	}



	if ( IS_NPC(ch) )

	{

		/*

		 * NPC can always use a skil

		 * we leave it up to the builders to control

		 * what skills/spells they have

		 */



		return TRUE;

	}



	if ( IS_IMMORTAL(ch) )

	{

		/*

		 * Immortals can use all skills/spells

		 */

		return TRUE;

	}



	if ( IS_SET(ch->info, INFO_IMMMULTI) )

	{

		/*

		 * Immortal Multi's are exempted

		 */

		return TRUE;

	}



	if (	(ch->level >= skill_table[sn].skill_level[ch->class])

		&& (ch->pcdata->learned[sn] >= 1) )

	{

		return TRUE;		/* Standard Test, we are allowed by our class, and we have learned it */

	}





	/*

	 * Now the special cases, we'll assume it's false, and walk through our list

	 */



	can_do		= FALSE;			/* Default to NO, we'll test and make this yes    */



	if ( ch->level < skill_table[sn].skill_level[ch->class] )

		can_do	= FALSE;



	if ( ch->pcdata->learned[sn] < 1 )

		can_do	= FALSE;



	if ( skill_table[sn].rating[ch->class] == 0 )

		can_do	= FALSE;



	if ( skill_table[sn].rating[ch->class] == 99 )

		can_do	= FALSE;



	if ( skill_table[sn].skill_level[ch->class] >= 1000 )

		can_do	= FALSE;



	/*

	 * We've gone through all the normal NO reasons

	 * Now we need to check for racial skills

	 */



	for (cnt = 0; cnt < MAX_PC_RACE_SKILLS; cnt++)

	{

		if (pc_race_table[ch->race].skills[cnt] == skill_table[sn].name)

			can_do	= TRUE;

	}



	if ( can_do )

		return TRUE;

	else

		return FALSE;



	return FALSE;		/* Final Debug */

}



bool    is_race_skill( CHAR_DATA *ch, int sn )

{

	int	cnt;



	if (sn < -1 || sn > MAX_SKILL)

	{

		bug("Bad sn %d in is_race_skill.",sn);

		return FALSE;

	}



	if ( skill_table[sn].name == NULL )

	{

		return FALSE;

	}





	if (!can_use_skill(ch,sn))

		return FALSE;



	for (cnt = 0; cnt < MAX_PC_RACE_SKILLS; cnt++)

	{

		if (pc_race_table[ch->race].skills[cnt] == skill_table[sn].name)

			return TRUE;

	}



	return FALSE;

}



bool censor_text( CHAR_DATA *ch, char *argument )
{
	int 	cnt;
	char	buf[MAX_STRING_LENGTH];
	bool	found;

	found = FALSE;

	if ( IS_NPC(ch) )
		return FALSE;

	if ( IS_IMMORTAL(ch) )
		return FALSE;

	for( cnt = 0 ; censor_table[cnt].word != NULL ; cnt++ )
	{
		if ( !str_infix( censor_table[cnt].word, argument ) )
		{
			sprintf( buf, "%s has been nochanneled for swearing (%s) on a public channel for %d ticks",
				ch->name,
				censor_table[cnt].word,
				get_econ_value( "auto_punish" ) );
			log_string( buf );

			send_to_char( "Swearing is not allowed on public channels.\n\r", ch );
			send_to_char( "The gods have revoked your channel priviliges.\n\r", ch );

			if ( !IS_SET(ch->comm, COMM_NOCHANNELS) )
			{
				wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);
				free_string( ch->pcdata->title );
				ch->pcdata->title 	= str_dup( "the {rNo Channeled{w." );
				free_string( ch->pcdata->roomin );
				ch->pcdata->roomin 	= str_dup( "" );
				free_string( ch->pcdata->roomout );
				ch->pcdata->roomout 	= str_dup( "" );
				free_string( ch->pcdata->bamfin );
				ch->pcdata->bamfin 	= str_dup( "" );
				free_string( ch->pcdata->bamfout );
				ch->pcdata->bamfout 	= str_dup( "" );
			}

			found = TRUE;
			SET_BIT(ch->comm, COMM_NOCHANNELS);
			ch->pcdata->timer_punished[PUNISH_NOCHAN] = get_econ_value( "auto_punish" );
		}

		if ( found )
			break;
	}

	return found;
}

bool is_in_prison( CHAR_DATA *ch )
{
	if ( IS_NPC(ch) )
		return FALSE;

	if ( IS_IMMORTAL(ch) )
		return FALSE;

	if (		(ch->in_room == get_room_index(ROOM_VNUM_JAIL))
		||	(ch->in_room == get_room_index(ROOM_VNUM_PRISON))
		||	(ch->in_room == get_room_index(ROOM_VNUM_SC01))
		||	(ch->in_room == get_room_index(ROOM_VNUM_SC02))
		||	(ch->in_room == get_room_index(ROOM_VNUM_SC03))
		||	(ch->in_room == get_room_index(ROOM_VNUM_SC04))
		||	(ch->in_room == get_room_index(ROOM_VNUM_SC05))
		||	(ch->in_room == get_room_index(ROOM_VNUM_TROPHY))
	   )
		return TRUE;
	else
		return FALSE;
}


bool is_punished( CHAR_DATA *ch )

{

	if ( IS_NPC(ch) )

		return FALSE;



	if ( !IS_IMMORTAL(ch) && (ch->in_room == get_room_index(ROOM_VNUM_IMMINT)) )

		return TRUE;



	if (		IS_SET(ch->comm, COMM_NOCHANNELS)

		||	IS_SET(ch->comm, COMM_NOEMOTE)

		||	IS_SET(ch->comm, COMM_NOTELL)

		||	IS_SET(ch->comm, COMM_NOSHOUT)

		||	IS_SET(ch->info, INFO_BOZO)

		||	IS_SET(ch->act,  PLR_WANTED)

		||	IS_SET(ch->act,  PLR_FREEZE)

		||	is_in_prison(ch)

	   )

		return TRUE;

	else

		return FALSE;

}



char *punished_bit_name( CHAR_DATA *ch )

{

	extern CHAR_DATA *global_trident_target;

	char buf[MAX_STRING_LENGTH];



	if ( is_punished(ch) )

	{

		sprintf( buf, "%s%s%s%s%s%s%s%s%s%s{w",

			(IS_SET(ch->comm, COMM_NOCHANNELS) 						? " NoChannel" 	: ""),

			(IS_SET(ch->comm, COMM_NOEMOTE) 						? " NoEmote" 	: ""),

			(IS_SET(ch->comm, COMM_NOTELL) 						? " NoTell" 	: ""),

			(IS_SET(ch->comm, COMM_NOSHOUT) 						? " NoShout" 	: ""),

			(IS_SET(ch->info, INFO_BOZO) 							? " Bozo" 		: ""),

			(IS_SET(ch->act,  PLR_WANTED) 						? " Wanted" 	: ""),

			(IS_SET(ch->act,  PLR_FREEZE) 						? " Freeze" 	: ""),

			((	(ch->in_room == get_room_index(ROOM_VNUM_JAIL))

			||	(ch->in_room == get_room_index(ROOM_VNUM_PRISON))

			||	(ch->in_room == get_room_index(ROOM_VNUM_SC01))

			||	(ch->in_room == get_room_index(ROOM_VNUM_SC02))

			||	(ch->in_room == get_room_index(ROOM_VNUM_SC03))

			||	(ch->in_room == get_room_index(ROOM_VNUM_SC04))

			||	(ch->in_room == get_room_index(ROOM_VNUM_SC05))

			||	(ch->in_room == get_room_index(ROOM_VNUM_TROPHY))	) 	? " Prison" 	: ""),

			(	(ch->in_room == get_room_index(ROOM_VNUM_IMMINT))		? " Eat" 		: ""),

			((global_trident_target == ch) 						? " Trident" 	: "") );

	}

	else

	{

		sprintf( buf, "{w" );

	}



	return buf;

}

#endif