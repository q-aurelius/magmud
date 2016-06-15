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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"

/* command procedures needed */
DECLARE_DO_FUN(do_backstab      );
DECLARE_DO_FUN(do_emote         );
DECLARE_DO_FUN(do_berserk       );
DECLARE_DO_FUN(do_bash          );
DECLARE_DO_FUN(do_trip          );
DECLARE_DO_FUN(do_dirt          );
DECLARE_DO_FUN(do_flee          );
DECLARE_DO_FUN(do_fury          );
DECLARE_DO_FUN(do_destruction   );
DECLARE_DO_FUN(do_kick          );
DECLARE_DO_FUN(do_disarm        );
DECLARE_DO_FUN(do_get           );
DECLARE_DO_FUN(do_recall        );
DECLARE_DO_FUN(do_yell          );
DECLARE_DO_FUN(do_sacrifice     );
DECLARE_DO_FUN(do_echo          );
DECLARE_DO_FUN(do_look          );


/*
 * Local functions.
 */
void    check_assist    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    check_dodge     args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    check_killer    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    check_wanted    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    check_parry     args( ( CHAR_DATA *ch, CHAR_DATA *victim, bool parryable ) );
bool    check_shield_block     args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    check_evas     args( ( CHAR_DATA *ch, CHAR_DATA *victim, bool parryable, bool proj ) );
bool    check_i_evas     args( ( CHAR_DATA *ch, CHAR_DATA *victim, bool parryable, bool proj ) );
void    dam_message     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			    int dt, bool immune ) );
void    death_cry       args( ( CHAR_DATA *ch ) );
void    group_gain      args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int     xp_compute      args( ( CHAR_DATA *gch, CHAR_DATA *victim,
				int total_levels ) );
bool    is_safe         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    make_corpse     args( ( CHAR_DATA *ch ) );
void    one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool secondary ) );
void    mob_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    disarm          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );



/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	ch_next = ch->next;

	if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
	    continue;

	if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
	    multi_hit( ch, victim, TYPE_UNDEFINED );
	else
	    stop_fighting( ch, FALSE );

	if ( ( victim = ch->fighting ) == NULL )
	    continue;

	/*
	 * Fun for the whole family!
	 */
	check_assist(ch,victim);
    }

    return;
}

/* for auto assisting */
void check_assist(CHAR_DATA *ch,CHAR_DATA *victim)
{
    CHAR_DATA *rch, *rch_next;

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
	rch_next = rch->next_in_room;
	if (IS_AWAKE(rch) && rch->fighting == NULL && can_see(rch,victim))  //didnt check to see if can see before assist
	{

	    /* quick check for ASSIST_PLAYER */
	    if (!IS_NPC(ch) && IS_NPC(rch)
	    && IS_SET(rch->off_flags,ASSIST_PLAYERS)
	    &&  rch->level + 6 > victim->level)
	    {
		do_emote(rch,"screams and attacks!");
		multi_hit(rch,victim,TYPE_UNDEFINED);
		continue;
	    }

	    /* PCs next */
	    if (!IS_NPC(ch) || IS_AFFECTED(ch,AFF_CHARM))
	    {
		if ( ( (!IS_NPC(rch) && IS_SET(rch->act,PLR_AUTOASSIST))
		||     IS_AFFECTED(rch,AFF_CHARM))
		&&   is_same_group(ch,rch)
		&&   !is_safe(rch, victim))
		    multi_hit (rch,victim,TYPE_UNDEFINED);
		continue;
	    }

	    /* now check the NPC cases */

	    if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
	    {
		if ( (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALL))
		||   (IS_NPC(rch) && rch->group && rch->group == ch->group)
		||   (IS_NPC(rch) && rch->race == ch->race
		   && IS_SET(rch->off_flags,ASSIST_RACE))

		||   (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALIGN)
		   &&   ((IS_GOOD(rch)    && IS_GOOD(ch))
		     ||  (IS_EVIL(rch)    && IS_EVIL(ch))
		     ||  (IS_NEUTRAL(rch) && IS_NEUTRAL(ch))))
		||   (rch->pIndexData == ch->pIndexData
		   && IS_SET(rch->off_flags,ASSIST_VNUM)))

		{
		    CHAR_DATA *vch;
		    CHAR_DATA *target;
		    int number;

		    if (number_bits(1) == 0)
			continue;
		    target = NULL;
		    number = 0;
		    for (vch = ch->in_room->people; vch; vch = vch->next)
		    {
			if (can_see(rch,vch)
			&&  is_same_group(vch,victim)
			&&  number_range(0,number) == 0)
			{
			    target = vch;
			    number++;
			}
		    }

		    if (target != NULL)
		    {
			do_emote(rch,"screams and attacks!");
			multi_hit(rch,target,TYPE_UNDEFINED);
		    }
		}
	    }
	}
    }
}


/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int     chance;

    /* decrement the wait */
    if (ch->desc == NULL)
	ch->wait = UMAX(0,ch->wait - PULSE_VIOLENCE);

    if (ch->desc == NULL)
	ch->daze = UMAX(0,ch->daze - PULSE_VIOLENCE);

    /* no attacks for stunnies -- just a check */
    if (ch->position <= POS_RESTING)
	return;

    if (IS_NPC(ch))
    {
	mob_hit(ch,victim,dt);
	return;
    }


    /* First Attack (1)*/
    one_hit( ch, victim, dt, FALSE );
    if (ch->fighting != victim)
	return;


    /* First Dual Wield (2)*/
    if (get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_dual_wield,TRUE,5);
	  if ( ch->fighting != victim )
	    return;
      }
    }

    /* Haste (3) or (+1, +2) */
    if (IS_AFFECTED(ch,AFF_HASTE))
    {
	if (ch->level < 150)
		one_hit(ch,victim,dt,FALSE);
	else if (ch->level >= 150 && ch->level < 300)
	{
		one_hit(ch,victim,dt,FALSE);
		one_hit(ch,victim,dt,FALSE);
	}
	else if (ch->level >= 300)
	{
		one_hit(ch,victim,dt,FALSE);
		one_hit(ch,victim,dt,FALSE);
		one_hit(ch,victim,dt,FALSE);
	}
	else
		one_hit(ch,victim,dt,FALSE);
    }
    if ( ch->fighting != victim )
	return;


    /* Hasted Dual Wield (4) or (+1, +2) */
    if (get_eq_char(ch, WEAR_SECONDARY) && IS_AFFECTED(ch,AFF_HASTE))
    {

	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	   if (ch->level < 150)
		one_hit(ch,victim,dt,FALSE);
	   else if (ch->level >= 150 && ch->level < 300)
	   {
		one_hit(ch,victim,dt,FALSE);
		one_hit(ch,victim,dt,FALSE);
	   }
	   else if (ch->level >= 300)
	   {
		one_hit(ch,victim,dt,FALSE);
		one_hit(ch,victim,dt,FALSE);
		one_hit(ch,victim,dt,FALSE);
	   }
	   else
		one_hit(ch,victim,dt,FALSE);
	  check_improve(ch,gsn_dual_wield,TRUE,5);
	  if ( ch->fighting != victim  || dt == gsn_backstab )
	    return;
      }
    }

    /* Second Attack (5) */
    chance = get_skill(ch,gsn_second_attack)/2;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance /= 2;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_second_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }

    /* Second Attack Dual Wield (6) */

    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_dual_wield,TRUE,5);
	  check_improve(ch,gsn_second_attack,TRUE,5);
	  if ( ch->fighting != victim )
	    return;
      }
    }

    /* Third Attack (7) */
    chance = get_skill(ch,gsn_third_attack)/4;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_third_attack,TRUE,6);
	if ( ch->fighting != victim )
	    return;
    }

    /* Third Attack Dual Wield (8) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_third_attack,TRUE,5);
	  check_improve(ch,gsn_dual_wield,TRUE,5);
	  if ( ch->fighting != victim )
	    return;
      }
    }


    /* Fourth Attack (9) */
    chance = get_skill(ch,gsn_fourth_attack)/6;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_fourth_attack,TRUE,7);
	if ( ch->fighting != victim )
	    return;
    }

    /* Fourth Attack Dual Wield (10) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_fourth_attack,TRUE,5);
	  check_improve(ch,gsn_dual_wield,TRUE,5);
	  if ( ch->fighting != victim )
	    return;
      }
    }


    /* Fifth Attack (11) */
    chance = get_skill(ch,gsn_fifth_attack)/6;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_fifth_attack,TRUE,7);
	if ( ch->fighting != victim )
	    return;
    }


    /* Fifith Attack Dual Wield (12) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_fifth_attack,TRUE,5);
	  check_improve(ch,gsn_dual_wield,TRUE,5);
	  if ( ch->fighting != victim )
	    return;
      }
    }


    /* Sixth Attack (13) */
    chance = get_skill(ch,gsn_sixth_attack)/8;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_sixth_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }


    /* Sixth Attack Dual Wield (14) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_sixth_attack,TRUE,3);
	  check_improve(ch,gsn_dual_wield,TRUE,3);
	  if ( ch->fighting != victim )
	    return;
      }
    }

    /* Seventh Attack (15) */
    chance = get_skill(ch,gsn_seventh_attack)/8;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_seventh_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }


    /* Seventh Attack Dual Wield (16) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_seventh_attack,TRUE,3);
	  check_improve(ch,gsn_dual_wield,TRUE,3);
	  if ( ch->fighting != victim )
	    return;
      }
    }

    /* Eighth Attack (17) */
    chance = get_skill(ch,gsn_eighth_attack)/8;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_eighth_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }


    /* Eighth Attack Dual Wield (18) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_eighth_attack,TRUE,3);
	  check_improve(ch,gsn_dual_wield,TRUE,3);
	  if ( ch->fighting != victim )
	    return;
      }
    }

    /* Ninth Attack (19) */
    chance = get_skill(ch,gsn_ninth_attack)/10;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_ninth_attack,TRUE,3);
	if ( ch->fighting != victim )
	    return;
    }


    /* Ninth Attack Dual Wield (20) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_ninth_attack,TRUE,3);
	  check_improve(ch,gsn_dual_wield,TRUE,3);
	  if ( ch->fighting != victim )
	    return;
      }
    }

    /* Tenth Attack (21) */
    chance = get_skill(ch,gsn_tenth_attack)/10;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance = 0;;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_tenth_attack,TRUE,3);
	if ( ch->fighting != victim )
	    return;
    }
    /* Tenth Attack Dual Wield (22) */
    if (number_percent( ) < chance && get_eq_char(ch, WEAR_SECONDARY))
    {
	chance = get_skill(ch,gsn_dual_wield);
      if ( number_percent( ) < chance )
      {
	  one_hit( ch, victim, dt, TRUE );
	  check_improve(ch,gsn_tenth_attack,TRUE,3);
	  check_improve(ch,gsn_dual_wield,TRUE,3);
	  if ( ch->fighting != victim )
	    return;
      }
    }
    return;
}


/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    int chance,number;
    CHAR_DATA *vch, *vch_next;

    /* no attacks for stunnies -- just a check */
    if (ch->position <= POS_RESTING)
	return;

    /* Mob First Attack (1) */
    one_hit(ch,victim,dt,FALSE);
    if (ch->fighting != victim)
	return;

    /* Area attack -- BALLS nasty! */
    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch,dt,FALSE);
	}
    }


	/* Mob Dual Wield (2) */
    chance = get_skill(ch,gsn_dual_wield);
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_second_attack,TRUE,5);
	if ( ch->fighting != victim )
	  return;
    }

    /* Area attack -- BALLS nasty! */
    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance)
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch,dt,FALSE);
	}
    }


	/* Mob Haste (3) */
    if (IS_AFFECTED(ch,AFF_HASTE)
    ||  (IS_SET(ch->off_flags,OFF_FAST) && !IS_AFFECTED(ch,AFF_SLOW)))
	one_hit(ch,victim,dt,FALSE);
    if (ch->fighting != victim)
	return;

    /* Area attack -- BALLS nasty! */
    if ((IS_AFFECTED(ch,AFF_HASTE)
    ||  (IS_SET(ch->off_flags,OFF_FAST) && !IS_AFFECTED(ch,AFF_SLOW)))
    && IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch,dt,FALSE);
	}
    }

	/* Mob Hasted Dual Wield and Area Attack (4) */
    if (IS_AFFECTED(ch,AFF_HASTE)
    ||  (IS_SET(ch->off_flags,OFF_FAST) && !IS_AFFECTED(ch,AFF_SLOW)))
    {
	chance = get_skill(ch,gsn_dual_wield);
	if ( number_percent( ) < chance )
	{
	  one_hit( ch, victim, dt, FALSE );
	  if ( ch->fighting != victim )
	    return;
      }

	/* Area attack -- BALLS nasty! */
	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance)
	{
	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	  {
	    vch_next = vch->next;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch,dt,FALSE);
	  }
	}
    }

    if (ch->fighting != victim || dt == gsn_backstab)
	return;

	/* Mob Second Attack (5) */
    chance = get_skill(ch,gsn_second_attack)/2;
    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
	chance /= 2;
    if (number_percent() < chance)
    {
	one_hit(ch,victim,dt,FALSE);
	if (ch->fighting != victim)
	    return;
    }

    /* Area attack -- BALLS nasty! */
    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK)
	 && number_percent() < chance)
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch,dt,FALSE);
	}
    }



	/* Mob Second Dual Wield and Area Attack (6) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent() < chance)

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }





	/* Mob Third Attack (7) */

    chance = get_skill(ch,gsn_third_attack)/4;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK)

	 && number_percent() < chance)

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob Third Dual Wield and Area Attack (8) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }





	/* Mob Fourth Attack (9) */

    chance = get_skill(ch,gsn_fourth_attack)/4;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob fourth Dual Wield and Area Attack (10) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }





	/* Mob Fifth Attack (11) */

    chance = get_skill(ch,gsn_fifth_attack)/4;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }





	/* Mob fifth Dual Wield and Area Attack (12) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }





	/* Mob Sixth Attack (13) */

    chance = get_skill(ch,gsn_sixth_attack)/6;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob Sixth Dual Wield and Area Attack (14) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }



	/* Mob Seventh Attack (15) */

    chance = get_skill(ch,gsn_seventh_attack)/8;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob Seventh Dual Wield and Area Attack (16) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }



	/* Mob Eighth Attack (17) */

    chance = get_skill(ch,gsn_eighth_attack)/10;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob Eighth Dual Wield and Area Attack (18) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }



	/* Mob Ninth Attack (19) */

    chance = get_skill(ch,gsn_ninth_attack)/10;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob Ninth Dual Wield and Area Attack (20) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }



	/* Mob Tenth Attack (21) */

    chance = get_skill(ch,gsn_tenth_attack)/10;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))

	chance = 0;

    if (number_percent() < chance)

    {

	one_hit(ch,victim,dt,FALSE);

	if (ch->fighting != victim)

	    return;

    }



    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

    {

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	{

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	}

    }







	/* Mob Tenth Dual Wield and Area Attack (22) */

    if (number_percent() < chance)

    {

	chance = get_skill(ch,gsn_dual_wield);

	if ( number_percent( ) < chance )

	{

	  one_hit( ch, victim, dt, FALSE );

	  if ( ch->fighting != victim )

	    return;

      }



	/* Area attack -- BALLS nasty! */

	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK) && number_percent( ) < chance )

	{

	  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)

	  {

	    vch_next = vch->next;

	    if ((vch != victim && vch->fighting == ch))

		one_hit(ch,vch,dt,FALSE);

	  }

	}

    }





    /* oh boy!  Fun stuff! */



    if (ch->wait > 0)

	return;



/*

    number = number_range(0,2);



    if (number == 1 && IS_SET(ch->act,ACT_MAGE))

    {

	  { mob_cast_mage(ch,victim); return; };

    }



    if (number == 2 && IS_SET(ch->act,ACT_CLERIC))

    {

	{ mob_cast_cleric(ch,victim); return; };

    }

*/



    /* now for the skills */



    number = number_range(0,9);



    switch(number)

    {

    case (0) :

	if (IS_SET(ch->off_flags,OFF_BASH))

	    do_bash(ch,"");

	break;



    case (1) :

	if (IS_SET(ch->off_flags,OFF_BERSERK) && !IS_AFFECTED(ch,AFF_BERSERK))

	    do_berserk(ch,"");

	break;





    case (2) :

	if (IS_SET(ch->off_flags,OFF_DISARM)

	|| (get_weapon_sn(ch) != gsn_hand_to_hand

	&& (IS_SET(ch->act,ACT_WARRIOR)

	||  IS_SET(ch->act,ACT_THIEF))))

	    do_disarm(ch,"");

	break;



    case (3) :

	if (IS_SET(ch->off_flags,OFF_KICK))

	    do_kick(ch,"");

	break;



    case (4) :

	if (IS_SET(ch->off_flags,OFF_KICK_DIRT))

	    do_dirt(ch,"");

	break;



    case (5) :

	if (IS_SET(ch->off_flags,OFF_TAIL))

	{

	    /* do_tail(ch,""); */

	}

	break;



    case (6) :

	if (IS_SET(ch->off_flags,OFF_TRIP))

	    do_trip(ch,"");

	break;



    case (7) :

	if (IS_SET(ch->off_flags,OFF_CRUSH))

	{

	    /* do_crush(ch,""); */

	}

	break;

    case (8) :

	if (IS_SET(ch->off_flags,OFF_BACKSTAB))

	{

	    do_backstab(ch,"");

	}

    case (9) :

	if (IS_SET(ch->off_flags,OFF_FURY) && !IS_AFFECTED(ch,AFF_FURY))

	    do_fury(ch,"");

	break;

    }

}





/*

 * Hit one guy once.

 */

void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool secondary )
{
    OBJ_DATA *wield, *w_vic;
    OBJ_DATA *second_weapon;
    int victim_ac;
    int thac0;
    int thac0_00;
    int thac0_32;
    int dam;
    int diceroll;
    int sn,skill;
    int dam_type;
    int new_ac;
    bool result;
    char log_buf[MAX_STRING_LENGTH];
	bool parryable;

    sn = -1;

    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
	return;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
    return;

    /* no attacks for stunnies -- just a check */
    if (ch->position <= POS_RESTING)
	return;

    /* Guard against non-weapon secondary items */
    if ( (second_weapon = get_eq_char( ch, WEAR_SECONDARY )) != NULL )
    {
    	if ( second_weapon->item_type != ITEM_WEAPON )
    	{
		stop_fighting( ch, TRUE );
		send_to_char( "You are using an illegal weapon.\n\rThe Gods slay you!\n\r", ch);
		act("$n is using an illegal weapon!\n\rThe Gods slay $s!",ch,NULL,NULL,TO_ROOM);
		raw_kill( ch );
		wiznet("$N used an illegal weapon.",ch,NULL,WIZ_FLAGS,0,get_trust(ch));
		sprintf( log_buf, "%s used an illegal weapon, vnum: %d   name: %s",
			   ch->name, second_weapon->pIndexData->vnum, second_weapon->name );
		log_string( log_buf );
		if ( !IS_SET(ch->act, PLR_WANTED) )
		{
		    SET_BIT(ch->act, PLR_WANTED);
		    send_to_char( "*** You are now WANTED!! ***\n\r", ch );
		    save_char_obj( ch );
		    wiznet("$N is now a wanted man.",ch,NULL,WIZ_FLAGS,0,get_trust(ch));
		}
		return;
	}
    }


    /*
     * Figure out the type of damage message.
     * if secondary == true, use second weapon.
     */
    if (!secondary)
	wield = get_eq_char( ch, WEAR_WIELD );
    else
	wield = get_eq_char( ch, WEAR_SECONDARY );
	
	w_vic = get_eq_char(victim, WEAR_WIELD);

	if (wield!=NULL)
	{
		if (w_vic != NULL)
		{
			if (wield->value[0]==WEAPON_PROJ && !(w_vic->value[0]==WEAPON_LIGHTSABER &&
												   class_lookup("jedi") != victim->class) )
				parryable = FALSE;
			else
				parryable = TRUE;
		}
		else
		{
			if (wield->value[0]==WEAPON_PROJ)
				parryable=FALSE;
			else 
			parryable=TRUE;
		}
	}

    if ( dt == TYPE_UNDEFINED )
    {
	dt = TYPE_HIT;
	if ( wield != NULL && wield->item_type == ITEM_WEAPON )
	    dt += wield->value[3];
	else
	    dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
	if (wield != NULL)
	    dam_type = attack_table[wield->value[3]].damage;
	else
	    dam_type = attack_table[ch->dam_type].damage;
    else
	dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
	dam_type = DAM_BASH;

    /* get the weapon skill */
    sn = get_weapon_sn(ch);
    skill = 20 + get_weapon_skill(ch,sn);

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if ( IS_NPC(ch) )
    {
	thac0_00 = 20;
	thac0_32 = -4;   /* as good as a thief */
	if (IS_SET(ch->act,ACT_WARRIOR))
	    thac0_32 = -10;
	else if (IS_SET(ch->act,ACT_THIEF))
	    thac0_32 = -4;
	else if (IS_SET(ch->act,ACT_CLERIC))
	    thac0_32 = 2;
	else if (IS_SET(ch->act,ACT_MAGE))
	    thac0_32 = 6;
    }
    else
    {
	thac0_00 = class_table[ch->class].thac0_00;
	thac0_32 = class_table[ch->class].thac0_32;
    }
    thac0  = interpolate( ch->level, thac0_00, thac0_32 );

    if (thac0 < 0)
	thac0 = thac0/2;

    if (thac0 < -5)
	thac0 = -5 + (thac0 + 5) / 2;

    thac0 -= GET_HITROLL(ch) * skill/100;
    thac0 += 5 * (100 - skill) / 100;

    if (dt == gsn_backstab)
	thac0 -= 10 * (100 - get_skill(ch,gsn_backstab));

    switch(dam_type)
    {
	case(DAM_PIERCE)	:	victim_ac = GET_AC(victim,AC_PIERCE)/10;       break;
	case(DAM_BASH)	:	victim_ac = GET_AC(victim,AC_BASH)/10;         break;
	case(DAM_SLASH)	:	victim_ac = GET_AC(victim,AC_SLASH)/10;        break;
	default		:	victim_ac = GET_AC(victim,AC_EXOTIC)/10;       break;
    };

    if (victim_ac < -15)
	victim_ac = (victim_ac + 15) / 5 - 15;
    if ( !can_see( ch, victim ) )
	victim_ac -= 4;

    if ( victim->position < POS_FIGHTING)
	victim_ac += 4;
    if (victim->position < POS_RESTING)
	victim_ac += 6;

    /*
     * The moment of excitement!
     */
    while ( ( diceroll = number_bits( 5 ) ) >= 20 )
	;

    if ( diceroll == 0
    || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
    {
	/* Miss. */
	damage( ch, victim, 0, dt, dam_type, TRUE, parryable ); /* Grayson -- Parryable 4 May 2003 */
	tail_chain( );
	return;
    }

    /*
     * Hit.
     * Calc damage.
     */
    if ( IS_NPC(ch) && (!ch->pIndexData->new_format || wield == NULL))
	if (!ch->pIndexData->new_format)
	{
	    dam = number_range( ch->level / 2, ch->level * 3 / 2 );
	    if ( wield != NULL )
		dam += dam / 2;
	}
	else
	    dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
    else
    {
	if (sn != -1)
	    check_improve(ch,sn,TRUE,5);
	if ( wield != NULL )
	{
	    if (wield->pIndexData->new_format)
		dam = dice(wield->value[1],wield->value[2]) * skill/100;
	    else
		dam = number_range( wield->value[1] * skill/100,
				wield->value[2] * skill/100);

	    if (get_eq_char(ch,WEAR_SHIELD) == NULL)  /* no shield = more */
		dam = dam * 11/10;

	    /* sharpness! */
	    if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
	    {
		int percent;

		if ((percent = number_percent()) <= (skill / 8))
		    dam = 2 * dam + (dam * 2 * percent / 100);
	    }
	}
	else
	    dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
    }

    /*
     * Bonuses.
     */
    if ( get_skill(ch,gsn_enhanced_damage) > 0 )
    {
	diceroll = number_percent();
	if (diceroll <= get_skill(ch,gsn_enhanced_damage))
	{
	    check_improve(ch,gsn_enhanced_damage,TRUE,6);
	    dam += 2 * ( dam * diceroll/175);
	}
    }

    if ( !IS_AWAKE(victim) )
	dam *= 2;
     else if (victim->position < POS_FIGHTING)
	dam = dam * 3 / 2;
	/*
    if ( dt == gsn_backstab && wield != NULL)
	if ( wield->value[0] != 2 )
	//    dam *= 2 + (ch->level / 10);
		dam += dice(level, 8);
	else
	    //dam *= 2 + (ch->level / 8);
	    dam += dice(level,6);
	*/
    dam += GET_DAMROLL(ch) * UMIN(100,skill) /100;

    if (IS_IMMORTAL(ch))
    {
	dam = dam + (ch->level / 10);
    }

    switch(dam_type)
    {
	case(DAM_PIERCE)	:	new_ac = GET_AC(victim,AC_PIERCE);       break;
	case(DAM_BASH)		:	new_ac = GET_AC(victim,AC_BASH);         break;
	case(DAM_SLASH)		:	new_ac = GET_AC(victim,AC_SLASH);        break;
	default				:	new_ac = GET_AC(victim,AC_EXOTIC);       break;
    };
    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac");
    
    if (ch->level > 50 && victim->level > 50)
    {
    	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
    }						// Grayson 21 Dec 2003
    
    
    if ( dt == gsn_backstab && wield != NULL)
	if ( wield->value[0] != 2 )
	//    dam *= 2 + (ch->level / 10);
		dam += dice(ch->level, 8);
	else
	    //dam *= 2 + (ch->level / 8);
	    dam += dice(ch->level,6);
	
    
    if ( dam <= 0 )
	dam = 1;
		
	if (dt == gsn_backstab) // Made backstab tables do something -- Gray 21 Dec 03
	{
		dam = dam * get_combat_value("m_damage_stab");
		dam = dam / get_combat_value("d_damage_stab");
	}
		
    result = damage( ch, victim, dam, dt, dam_type, TRUE, parryable );
    /* but do we have a funky weapon? */
    
    if (result && wield != NULL)
    {
	int dam;

	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
	{
	    int level;
	    AFFECT_DATA *poison, af;

	    if ((poison = affect_find(wield->affected,gsn_poison)) == NULL)
		level = wield->level;
	    else
		level = poison->level;

	    if (!saves_spell(level / 2,victim,DAM_POISON))
	    {
		send_to_char("{GYou feel {gpoison{G coursing through your veins.{w",
		    victim);
		act("{G$n{G is {gpoisoned{G by the venom on $p{G.{w",
		    victim,wield,NULL,TO_ROOM);

		af.where     = TO_AFFECTS;
		af.type      = gsn_poison;
		af.level     = level * 3/4;
		af.duration  = level / 2;
		af.location  = APPLY_STR;
		af.modifier  = -1;
		af.bitvector = AFF_POISON;
		affect_join( victim, &af );
	    }

	    /* weaken the poison if it's temporary */
	    if (poison != NULL)
	    {
		poison->level = UMAX(0,poison->level - 2);
		poison->duration = UMAX(0,poison->duration - 1);
		if (poison->level == 0 || poison->duration == 0)
		    act("The poison on $p has worn off.",ch,wield,NULL,TO_CHAR);
	    }
	}


	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_VAMPIRIC))
	{
	    dam = number_range(1, wield->level / 5 + 1);
	    act("{W$p{W draws life from $n{W.{w",victim,wield,NULL,TO_ROOM);
	    act("{WYou feel $p{W drawing your life away.{w",
		victim,wield,NULL,TO_CHAR);
	    damage_old(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
	    ch->alignment = UMAX(-1000,ch->alignment - 1);
	    if (ch->hit < 30000)
	    {
		if ((ch->hit + dam/2) >= 30000 )
		{
			ch->hit = 30000;
		}
		else
		{
			ch->hit += dam/2;
		}
	    }
	}

	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FLAMING))
	{
	    dam = number_range(1,wield->level / 4 + 1);
	    act("{w$n{w is {rburned{w by $p{w.",victim,wield,NULL,TO_ROOM);
	    act("{r$p{r sears your flesh.{w",victim,wield,NULL,TO_CHAR);
	    fire_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
	    damage(ch,victim,dam,0,DAM_FIRE,FALSE, parryable); /* Grayson -- Parryable 4 May 2003 */
	}

	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FROST))
	{
	    dam = number_range(1,wield->level / 6 + 2);
	    act("{B$p{b freezes{B $n{B.{w",victim,wield,NULL,TO_ROOM);
	    act("{bThe cold touch of $p{b surrounds you with ice.{w",
		victim,wield,NULL,TO_CHAR);
	    cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
	    damage(ch,victim,dam,0,DAM_COLD,FALSE, parryable);/* Grayson -- Parryable 4 May 2003 */
	}

	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SHOCKING))
	{
	    dam = number_range(1,wield->level/5 + 2);
	    act("{Y$n{Y is struck by {ylightning{Y from $p{Y.{w",victim,wield,NULL,TO_ROOM);
	    act("{yYou are shocked by $p{y.{w",victim,wield,NULL,TO_CHAR);
	    shock_effect(victim,wield->level/2,dam,TARGET_CHAR);
	    damage(ch,victim,dam,0,DAM_LIGHTNING,FALSE, parryable);/* Grayson -- Parryable 4 May 2003 */
	}
    }
    
   	if (IS_AFFECTED2(victim, AFF_FORCE_FEAR))
		do_flee( victim, "" );
    
    tail_chain( );
    return;
}


/*
 * Inflict damage from a hit.
 */
bool damage(CHAR_DATA *ch,CHAR_DATA *victim,int dam,int dt,int dam_type, bool show, bool parryable)
{
    OBJ_DATA *corpse;
	bool immune;
    char log_buf[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int lvldiff;
    int clvl;
    int vlvl;
    int new_ac;

    if ( victim->position == POS_DEAD )
	return FALSE;

    /* Armor Class Get AC */
    switch(dam_type)
    {
	case(DAM_PIERCE)	:	new_ac = GET_AC(victim,AC_PIERCE);       break;
	case(DAM_BASH)		:	new_ac = GET_AC(victim,AC_BASH);         break;
	case(DAM_SLASH)		:	new_ac = GET_AC(victim,AC_SLASH);        break;
	default				:	new_ac = GET_AC(victim,AC_EXOTIC);       break;
    };
    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac");
	
	
	/* New Damage Adjustment for ALL attacks */

	clvl = 0;
	vlvl = 0;
	lvldiff = 0;

	if (ch->level >= LEVEL_HERO)
		clvl = 1000;
	else
		clvl = ch->level;

	if ( victim->level >= LEVEL_HERO )
		vlvl = 1000;
	else
		vlvl = victim->level;

	if (clvl >= 300)
		dam += clvl/10;

	lvldiff = clvl - vlvl;

	if ( lvldiff > 20 )
		dam += lvldiff - 20;
	else if ( lvldiff < -20 )
		dam -= lvldiff + 20;

	if ( dam >= 2500 && !IS_HERO(ch))
	    dam = 2500;

    /* Check for Armor Class 
    if (ch->level > 50 && victim->level > 50)
    {
    	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
    }						// Grayson 21 Dec 2003
	*/
	if ( dam <= 1 )
		dam = 1;

    /*
     * Modify Backstab Damage.
     */
    if ( (dt == gsn_backstab) && (ch->level < LEVEL_HERO) )
    {
	dam = dam * get_combat_value("m_damage_stab");
	dam = dam / get_combat_value("d_damage_stab");
    }

    if ( victim != ch )
    {
	/*
	 * Certain attacks are forbidden.
	 * Most other attacks are returned.
	 */
	if ( is_safe( ch, victim ) )
	    return FALSE;
	check_killer( ch, victim );

	if ( victim->position > POS_STUNNED )
	{
	    if ( victim->fighting == NULL )
		set_fighting( victim, ch );
	    if (victim->timer <= 4)
		victim->position = POS_FIGHTING;
	}

	if ( victim->position > POS_STUNNED )
	{
	    if ( ch->fighting == NULL )
		set_fighting( ch, victim );

	    /*
	     * If victim is charmed, ch might attack victim's master.
	     taken out by Russ! */
/*
	    if ( IS_NPC(ch)
	    &&   IS_NPC(victim)
	    &&   IS_AFFECTED(victim, AFF_CHARM)
	    &&   victim->master != NULL
	    &&   victim->master->in_room == ch->in_room
	    &&   number_bits( 3 ) == 0 )
	    {
		stop_fighting( ch, FALSE );
		multi_hit( ch, victim->master, TYPE_UNDEFINED );
		return FALSE;
	    }
*/
	}

	/*
	 * More charm stuff.
	 */
	if ( victim->master == ch )
	    stop_follower( victim );
    }

    /*
     * Inviso attacks ... not.
     */
    if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
    {
	affect_strip( ch, gsn_invis );
	affect_strip( ch, gsn_mass_invis );
	affect_strip( ch, gsn_improv_invis );
	REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
	REMOVE_BIT( ch->affected2_by, AFF_IMPROV_INVIS );
	act( "$n fades into existence.", ch, NULL, NULL, TO_ROOM );
    }

    /*
     * Damage modifiers.
     */

    if ( dam > 1 && !IS_NPC(victim)
    &&   victim->pcdata->condition[COND_DRUNK]  > 10 )
	dam = 9 * dam / 10;

    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) && dt!=TYPE_SUNLIGHT)
	dam /= 2;

    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) && dt!=TYPE_SUNLIGHT )
    ||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )))
	dam -= dam / 4;

    immune = FALSE;

    /*
     * Check for parry, and dodge.
     */
    if ( dt >= TYPE_HIT && ch != victim)
    {
	if ( check_parry( ch, victim, parryable ) )
		return FALSE;
	if ( check_dodge( ch, victim ) )
		return FALSE;
	if ( check_shield_block(ch,victim))
	    return FALSE;
	if ( check_evas(ch, victim, parryable, TRUE) )
		return FALSE;
	if ( check_i_evas(ch, victim, parryable, TRUE))
		return FALSE;
    }
    else //Check evasion - Grayson Evasion 9 July 2004
    {
    	if (dt != TYPE_SUNLIGHT &&
    		dt != TYPE_HUNGER &&
    		dt != TYPE_THIRST
    		)
    	{
    	if ( check_evas( ch, victim, FALSE, FALSE ) )
			return FALSE;
		if ( check_i_evas( ch, victim, FALSE, FALSE ) )
			return FALSE;	
    	}
    }


    /*
     * Check for Immunity and make sure no one is immune
     * to an attack by an Immortal
     */
    if( IS_IMMORTAL(ch) && !IS_IMMORTAL(victim) )
	dam = dam + (dam/2);
    else
    {
	switch(check_immune(victim,dam_type))
	{
	   case(IS_IMMUNE):
	    immune = TRUE;
	    dam = 0;
	    break;
	   case(IS_RESISTANT):
	    dam -= dam/3;
	    break;
	   case(IS_VULNERABLE):
	    dam += dam/2;
	    break;
    	}
    }

    /* Do Global Modifications */
    if ( !IS_NPC(ch) && !IS_NPC(victim) )
    {
	dam = dam * get_combat_value("m_damage_pc_pc");
	dam = dam / get_combat_value("d_damage_pc_pc");
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
	dam = dam * get_combat_value("m_damage_pc_npc");
	dam = dam / get_combat_value("d_damage_pc_npc");
    }

    if ( IS_NPC(ch) && !IS_NPC(victim) )
    {
	dam = dam * get_combat_value("m_damage_npc_pc");
	dam = dam / get_combat_value("d_damage_npc_pc");
    }

	    // Global damage modifiers for classes
	        if ( class_lookup("mage") == ch->class && !IS_NPC(ch) )
    	    {
	        	dam = dam * get_combat_value("m_damage_mage");
	        	dam = dam / get_combat_value("d_damage_mage");
        	    //sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
                //send_to_char( log_buf, ch );
            }
        	if ( class_lookup("warrior") == ch->class && !IS_NPC(ch) )
        	{
            	dam = dam * get_combat_value("m_damage_warrior");
            	dam = dam / get_combat_value("d_damage_warrior");
        	    //sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
                //send_to_char( log_buf, ch );
            }
        	if ( class_lookup("thief") == ch->class && !IS_NPC(ch) )
        	{
            	dam = dam * get_combat_value("m_damage_thief");
	            dam = dam / get_combat_value("d_damage_thief");
        	    //sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
                //send_to_char( log_buf, ch );
            }
            if ( class_lookup("cleric") == ch->class && !IS_NPC(ch) )
            {
	            dam = dam * get_combat_value("m_damage_cleric");
	            dam = dam / get_combat_value("d_damage_cleric");
        	    //sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
                //send_to_char( log_buf, ch );
            }
            if ( class_lookup("cyborg") == ch->class && !IS_NPC(ch) )
            {
	            dam = dam * get_combat_value("m_damage_cyborg");
	            dam = dam / get_combat_value("d_damage_cyborg");
        	    //sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
                //send_to_char( log_buf, ch );
            }
            if ( class_lookup("druid") == ch->class && !IS_NPC(ch) )
            {
	            dam = dam * get_combat_value("m_damage_druid");
	            dam = dam / get_combat_value("d_damage_druid");
        	    //sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
                //send_to_char( log_buf, ch );
            }

    /* Do Final Reductions */
    if (dam >= 15000 )
	dam = 15000;

    if ( (ch->level == MAX_LEVEL) && (get_trust(ch) == MAX_LEVEL) && !IS_NPC(ch) )
	dam = 20000;

    /* Randomize the Damage */
    dam = number_range (dam * 3/4, dam * 5/4);

    if (show)
	dam_message( ch, victim, dam, dt, immune );

    if (dam == 0)
	return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */

    sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
    if ( IS_IMMORTAL(ch) )
	send_to_char( log_buf, ch );
    sprintf( log_buf, "{rDAMAGE: {w%s does {y%d{w damage to you.{w\n\r", ch->name, dam );
    if ( IS_IMMORTAL(victim) )
	send_to_char( log_buf, victim );


    victim->hit -= dam;
    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 )
	victim->hit = 1;
    update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
	act( "{W$n is mortally wounded, and will die soon, if not aided.{w",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char("{WYou are mortally wounded, and will die soon, if not aided.{w\n\r",
	    victim );
	break;

    case POS_INCAP:
	act( "{C$n is incapacitated and will slowly die, if not aided.{w",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char(
	    "{CYou are incapacitated and will slowly die, if not aided.{w\n\r",
	    victim );
	break;

    case POS_STUNNED:
	act( "{Y$n is stunned, but will probably recover.{w",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char("{YYou are stunned, but will probably recover.{w\n\r",
	    victim );
	break;

    case POS_DEAD:
	act( "{y$n is {WDEAD{y!!{w", victim, 0, 0, TO_ROOM );
	send_to_char( "{yYou have been {WKILLED{y!!{w\n\r\n\r", victim );
	break;

    default:
	if ( dam > victim->max_hit / 4 )
	    send_to_char( "That really did HURT!\n\r", victim );
	if ( victim->hit < victim->max_hit / 4 )
	    send_to_char( "You sure are BLEEDING!\n\r", victim );
	break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) )
	stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
	group_gain( ch, victim );

	if ( !IS_NPC(victim) )
	{
	    sprintf( log_buf, "%s killed by %s at %d",
		victim->name,
		(IS_NPC(ch) ? ch->short_descr : ch->name),
		ch->in_room->vnum );
	    log_string( log_buf );

	    /*
	     * Dying penalty:
	     * 2/3 way back to previous level.
	     */
	    if ( victim->exp > exp_per_level(victim,victim->pcdata->points)
			       * victim->level )
	gain_exp( victim, (2 * (exp_per_level(victim,victim->pcdata->points)
				 * victim->level - victim->exp)/3) + 50 );
	}

	sprintf( log_buf, "%s got toasted by %s at %s [room %d]",
	    (IS_NPC(victim) ? victim->short_descr : victim->name),
	    (IS_NPC(ch) ? ch->short_descr : ch->name),
	    ch->in_room->name, ch->in_room->vnum);

	if (IS_NPC(victim))
	    wiznet(log_buf,NULL,NULL,WIZ_MOBDEATHS,0,0);
	else
	    wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);

      if (!IS_NPC(ch)
      && !IS_NPC(victim)
      && victim->pcdata->bounty > 0)
	{
	   sprintf(buf,"You recive a %d gold bounty, for killing %s.\n\r",
    	   victim->pcdata->bounty, victim->name);
    	   send_to_char(buf, ch);
    	   ch->gold += victim->pcdata->bounty;
    	   victim->pcdata->bounty =0;
	}

	raw_kill( victim );
	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */
	/* dump the flags */
	if (ch != victim && !IS_NPC(ch) && !is_same_clan(ch,victim))
	{
	    if (IS_SET(victim->act,PLR_KILLER))
		REMOVE_BIT(victim->act,PLR_KILLER);
	    else if (IS_SET(victim->act,PLR_THIEF))
		REMOVE_BIT(victim->act,PLR_THIEF);
	    else
		REMOVE_BIT(victim->act,PLR_WANTED);
	}

	/* RT new auto commands */

	if (!IS_NPC(ch)
	&&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
	&&  corpse->item_type == ITEM_CORPSE_NPC && can_see_obj(ch,corpse))
	{
	    OBJ_DATA *coins;

	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents );

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) /* exists and not empty */
		do_get( ch, "all corpse" );

	    if (IS_SET(ch->act,PLR_AUTOGOLD) &&
		corpse && corpse->contains  && /* exists and not empty */
		!IS_SET(ch->act,PLR_AUTOLOOT))
		if ((coins = get_obj_list(ch,"gcash",corpse->contains))
		     != NULL)
		    do_get(ch, "all.gcash corpse");

	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return TRUE;  /* leave if corpse has treasure */
	      else
		do_sacrifice( ch, "corpse" );
	}

	return TRUE;
    }

    if ( victim == ch )
	return TRUE;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	if ( number_range( 0, victim->wait ) == 0 )
	{
	    do_recall( victim, "" );
	    return TRUE;
	}
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
	if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
	&&   victim->hit < victim->max_hit / 5)
	||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
	&&     victim->master->in_room != victim->in_room ) )
	    do_flee( victim, "" );
    }

    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2 )
	do_flee( victim, "" );

    tail_chain( );
    return TRUE;
}


/*
 * Inflict damage from a hit.
 */

bool damage_old( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show )
{

    OBJ_DATA *corpse;
    bool immune;
    char log_buf[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int lvldiff;
    int clvl;
    int vlvl;


    if ( victim->position == POS_DEAD )
	return FALSE;


	/* New Damage Adjustment for ALL attacks */

	clvl = 0;
	vlvl = 0;
	lvldiff = 0;

	if (ch->level >= LEVEL_HERO)
		clvl = 1000;
	else
		clvl = ch->level;

	if ( victim->level >= LEVEL_HERO )
		vlvl = 1000;
	else
		vlvl = victim->level;

	if (clvl >= 300)
		dam += clvl/10;

	lvldiff = clvl - vlvl;

	if ( lvldiff > 20 )
		dam += lvldiff - 20;
	else if ( lvldiff < -20 )
		dam -= lvldiff + 20;

	if ( dam >= 2500 && !IS_HERO(ch))
	    dam = 2500;

	if ( dam <= 1 )
		dam = 1;

    /*
     * Modify Backstab Damage.
     */
    if ( (dt == gsn_backstab) && (ch->level < LEVEL_HERO) )
    {
	dam = dam * get_combat_value("m_damage_stab");
	dam = dam / get_combat_value("d_damage_stab");
    }


    if ( victim != ch )
    {
	/*
	 * Certain attacks are forbidden.
	 * Most other attacks are returned.
	 */
	if ( is_safe( ch, victim ) )
	    return FALSE;
	check_killer( ch, victim );

	if ( victim->position > POS_STUNNED )
	{
	    if ( victim->fighting == NULL )
		set_fighting( victim, ch );
	    if (victim->timer <= 4)
		victim->position = POS_FIGHTING;
	}

	if ( victim->position > POS_STUNNED )
	{
	    if ( ch->fighting == NULL )
		set_fighting( ch, victim );

	    /*
	     * If victim is charmed, ch might attack victim's master.
	     */
	    if ( IS_NPC(ch)
	    &&   IS_NPC(victim)
	    &&   IS_AFFECTED(victim, AFF_CHARM)
	    &&   victim->master != NULL
	    &&   victim->master->in_room == ch->in_room
	    &&   number_bits( 3 ) == 0 )
	    {
		stop_fighting( ch, FALSE );
		multi_hit( ch, victim->master, TYPE_UNDEFINED );
		return FALSE;
	    }
	}

	/*
	 * More charm stuff.
	 */
	if ( victim->master == ch )
	    stop_follower( victim );
    }

    /*
     * Inviso attacks ... not.
     */
    if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
    {
	affect_strip( ch, gsn_invis );
	affect_strip( ch, gsn_mass_invis );
	affect_strip( ch, gsn_improv_invis );
	REMOVE_BIT( ch->affected2_by, AFF_IMPROV_INVIS );
	REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
	act( "$n fades into existence.", ch, NULL, NULL, TO_ROOM );
    }

    /*
     * Damage modifiers.
     */

    if ( dam > 1 && !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK]  > 10 )
	dam = 9 * dam / 10;

    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) )
	dam /= 2;

    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
    ||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )))
	dam -= dam / 4;

    immune = FALSE;


    /*
     * Check for parry, and dodge.
     */
    if ( dt >= TYPE_HIT && ch != victim)
    {
	if ( check_parry( ch, victim, FALSE ) ) /* Grayson -- Parryable 4 May 2003 */
	    return FALSE;
	if ( check_dodge( ch, victim ) )
	    return FALSE;
	if ( check_shield_block(ch,victim))
	    return FALSE;

    }


    /*
     * Check for Immunity and make sure no one is immune
     * to an attack by an Immortal
     */
    if( IS_IMMORTAL(ch) && !IS_IMMORTAL(victim) )
	dam += dam/2;
    else
    {
	switch(check_immune(victim,dam_type))
	{
	   case(IS_IMMUNE):
	    immune = TRUE;
	    dam = 0;
	    break;
	   case(IS_RESISTANT):
	    dam -= dam/3;
	    break;
	   case(IS_VULNERABLE):
	    dam += dam/2;
	    break;
    	}
    }

    /* Do Global Modifications */
    if ( !IS_NPC(ch) && !IS_NPC(victim) )
    {
	dam = dam * get_combat_value("m_damage_pc_pc");
	dam = dam / get_combat_value("d_damage_pc_pc");
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
	dam = dam * get_combat_value("m_damage_pc_npc");
	dam = dam / get_combat_value("d_damage_pc_npc");
    }

    if ( IS_NPC(ch) && !IS_NPC(victim) )
    {
	dam = dam * get_combat_value("m_damage_npc_pc");
	dam = dam / get_combat_value("d_damage_npc_pc");
    }


    /* Do Final Reductions */
    if (dam >= 15000 )
	dam = 15000;

    if ( (ch->level == MAX_LEVEL) && (get_trust(ch) == MAX_LEVEL) && !IS_NPC(ch) )
	dam = 20000;


    /* Randomize the Damage */
    dam = number_range (dam * 3/4, dam * 5/4);

    if (show)
	dam_message( ch, victim, dam, dt, immune );

    if (dam == 0)
	return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */

    sprintf( log_buf, "{rDAMAGE:{w You do {y%d{w to %s.{w\n\r", dam, victim->name );
    if ( IS_IMMORTAL(ch) )
	send_to_char( log_buf, ch );
    sprintf( log_buf, "{rDAMAGE: {w%s does {y%d{w damage to you.{w\n\r", ch->name, dam );
    if ( IS_IMMORTAL(victim) )
	send_to_char( log_buf, victim );

    victim->hit -= dam;
    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 )
	victim->hit = 1;
    update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
	act( "{W$n is mortally wounded, and will die soon, if not aided.{w",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char(
	    "{WYou are mortally wounded, and will die soon, if not aided.{w\n\r",
	    victim );
	break;

    case POS_INCAP:
	act( "{C$n is incapacitated and will slowly die, if not aided.{w",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char(
	    "{CYou are incapacitated and will slowly die, if not aided.{w\n\r",
	    victim );
	break;

    case POS_STUNNED:
	act( "{Y$n is stunned, but will probably recover.{w",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char("{YYou are stunned, but will probably recover.{w\n\r",
	    victim );
	break;

    case POS_DEAD:
	act( "{y$n is {WDEAD{y!!{w", victim, 0, 0, TO_ROOM );
	send_to_char( "{yYou have been {WKILLED{y!!{w\n\r\n\r", victim );
	break;

    default:
	if ( dam > victim->max_hit / 4 )
	    send_to_char( "That really did HURT!\n\r", victim );
	if ( victim->hit < victim->max_hit / 4 )
	    send_to_char( "You sure are BLEEDING!\n\r", victim );
	break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) )
	stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
	group_gain( ch, victim );

	if ( !IS_NPC(victim) )
	{
	    sprintf( log_buf, "%s killed by %s at %d",
		victim->name,
		(IS_NPC(ch) ? ch->short_descr : ch->name),
		victim->in_room->vnum );
	    log_string( log_buf );

	    /*
	     * Dying penalty:
	     * 2/3 way back to previous level.
	     */
	    if ( victim->exp > exp_per_level(victim,victim->pcdata->points)
			       * victim->level )
	gain_exp( victim, (2 * (exp_per_level(victim,victim->pcdata->points)
				 * victim->level - victim->exp)/3) + 50 );
	}

	sprintf( log_buf, "%s got toasted by %s at %s [room %d]",
	    (IS_NPC(victim) ? victim->short_descr : victim->name),
	    (IS_NPC(ch) ? ch->short_descr : ch->name),
	    ch->in_room->name, ch->in_room->vnum);

	if (IS_NPC(victim))
	    wiznet(log_buf,NULL,NULL,WIZ_MOBDEATHS,0,0);
	else
	    wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);

      if (!IS_NPC(ch)
      && !IS_NPC(victim)
      && victim->pcdata->bounty > 0)
	{
	   sprintf(buf,"You recive a %d gold bounty, for killing %s.\n\r",
    	   victim->pcdata->bounty, victim->name);
    	   send_to_char(buf, ch);
    	   ch->gold += victim->pcdata->bounty;
    	   victim->pcdata->bounty =0;
	}

	raw_kill( victim );
	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */
	/* dump the flags */
	if (ch != victim && !IS_NPC(ch) && !is_same_clan(ch,victim))
	{
	    if (IS_SET(victim->act,PLR_KILLER))
		REMOVE_BIT(victim->act,PLR_KILLER);
	    else if (IS_SET(victim->act,PLR_THIEF))
		REMOVE_BIT(victim->act,PLR_THIEF);
	    else
		REMOVE_BIT(victim->act,PLR_WANTED);
	}
	/* RT new auto commands */

	if ( !IS_NPC(ch) && IS_NPC(victim) )
	{
	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents );

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) /* exists and not empty */
		do_get( ch, "all corpse" );

	    if (IS_SET(ch->act,PLR_AUTOGOLD) &&
		corpse && corpse->contains  && /* exists and not empty */
		!IS_SET(ch->act,PLR_AUTOLOOT))
	      do_get(ch, "gold corpse");

	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return TRUE;  /* leave if corpse has treasure */
	      else
		do_sacrifice( ch, "corpse" );
	}

	return TRUE;
    }

    if ( victim == ch )
	return TRUE;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	if ( number_range( 0, victim->wait ) == 0 )
	{
	    do_recall( victim, "" );
	    return TRUE;
	}
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
	if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
	&&   victim->hit < victim->max_hit / 5)
	||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
	&&     victim->master->in_room != victim->in_room ) )
	    do_flee( victim, "" );
    }

    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2 )
	do_flee( victim, "" );

    tail_chain( );
    return TRUE;
}

bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{

    /*

     * Immortals can fight anywhere

     * Safe rooms prevent fighting

     * Ghosts are always safe

     * Arena's allow anyone to fight

     * TotalWar makes it a clannie killing zone

     * Anyone can kill a killer

     * PK requires both parties to be clanned,

     * and the clannie not be in a PKSAFE clan,

     * and the attacker to be no more than 8 levels above the defender

     */



    /* char buf[MAX_STRING_LENGTH]; */



    extern bool totalwar;



    if (victim->in_room == NULL || ch->in_room == NULL)

	return TRUE;



    if (victim->fighting == ch || victim == ch)

	return FALSE;



    if (is_ploaded(ch))

	return TRUE;



    if (is_linkdead(ch))

	return TRUE;



    if (ch->level >= LEVEL_IMMORTAL)

	return FALSE;



    if (!IS_NPC(ch) && IS_SET(ch->info, INFO_GHOST) )

	return TRUE;



    if (!IS_NPC(victim) && IS_SET(victim->info, INFO_GHOST) )

	return TRUE;



    /* killing mobiles */

    if (IS_NPC(victim))

    {

	/* safe room? */

	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))

	{

	   send_to_char("This place is protected, killing is not allowed here.\n\r", ch);

	   return TRUE;

	}



	if (victim->pIndexData->pShop != NULL)

	{

	   send_to_char("The shopkeeper would not like that.\n\r",ch);

	   return TRUE;

	}



	/* no killing healers, trainers, etc */

	if (IS_SET(victim->act,ACT_TRAIN)

	||  IS_SET(victim->act,ACT_PRACTICE)

	||  IS_SET(victim->act,ACT_IS_HEALER)

	||  IS_SET(victim->act,ACT_BANKER)

	||  IS_SET(victim->act,ACT_ENGRAVER)

	||  IS_SET(victim->act,ACT_IS_CHANGER))

	{

	   send_to_char("Mota would not approve.\n\r", ch);

	   return TRUE;

	}





	if (!IS_NPC(ch))

	{

	    /* no pets */

	    if (IS_SET(victim->act,ACT_PET))

	    {

		act("But $N looks so cute and cuddly...",

		    ch,NULL,victim,TO_CHAR);

		return TRUE;

	    }



	    /* no charmed creatures unless owner */

	    if (IS_AFFECTED(victim,AFF_CHARM) && ch != victim->master)

	    {

		send_to_char("You don't own that monster.\n\r",ch);

		return TRUE;

	    }

	}

    }

    /* killing players */

    else

    {

	/* NPC doing the killing */

	if (IS_NPC(ch))

	{

	    /* safe room check */

	    if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))

	    {

		send_to_char("This place is protected, killing is not allowed here.\n\r", ch);

		return TRUE;

	    }



	    /* charmed mobs and pets cannot attack players while owned */

	    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL

	    &&  ch->master->fighting != victim)

	    {

		send_to_char("Players are your friends!\n\r",ch);

		return TRUE;

	    }

	}

	/* player doing the killing */

	else

	{

	    /* safe room check */

	    if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))

	    {

		send_to_char("This place is protected, killing is not allowed here.\n\r", ch);

		return TRUE;

	    }



	    /* PK Safe Room Check, only stops killing players */

	    if (IS_SET(victim->in_room->room_flags,ROOM_NOPK))

	    {

		send_to_char("Player killing is not allowed here.\n\r", ch);

		return TRUE;

	    }



	    /* is the victim a Killer or a Thief */

	    if (IS_SET(victim->act,PLR_KILLER) || IS_SET(victim->act,PLR_THIEF))

		return FALSE;



	    /* Arena's are free for all rooms, no restrictions */

	    if (IS_SET(victim->in_room->room_flags,ROOM_ARENA))

		return FALSE;



	    /* is the attacker in a clan */

	    if (!is_clan(ch))

	    {

		send_to_char("Join a clan if you want to kill players.\n\r",ch);

		return TRUE;

	    }



	    /* is the victim clanless */

	    if (!is_clan(victim))

	    {

		send_to_char("They aren't in a clan, leave them alone.\n\r",ch);

		return TRUE;

	    }



	    /* Total War makes it a killing field for clannies */

	    if ( totalwar )

		return FALSE;



	    /* Is it a PK Safe Clan */

	    if ( clan_table[ch->clan].pksafe )

	    {

		send_to_char( "You are in a non-PK clan.\n\r", ch );

		return TRUE;

	    }



	    if ( clan_table[victim->clan].pksafe )

	    {

		send_to_char( "They are in a non-PK clan.\n\r", ch );

		return TRUE;

	    }



	    /* Pick on someone your own size */

	    if (ch->level <= 100 && (ch->level > victim->level + 10))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 200 && (ch->level > victim->level + 20))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 300 && (ch->level > victim->level + 30))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 400 && (ch->level > victim->level + 40))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 500 && (ch->level > victim->level + 50))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else

		return FALSE;

	}

    }



    return FALSE;



}



bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area )

{

    /*

     * Immortals can fight anywhere

     * Safe rooms prevent fighting

     * Arena's allow anyone to fight

     * Total War makes it a clannie bloodbath

     * Anyone can kill a killer

     * PK requires both parties to be clanned,

     * and the attacker to be no more than 8 levels above the defender

     */



/*     char buf[MAX_STRING_LENGTH]; */



    extern bool totalwar;



    if (victim->in_room == NULL || ch->in_room == NULL)

	return TRUE;



    if (victim == ch && area)

	return TRUE;



    if (victim->fighting == ch || victim == ch)

	return FALSE;



    if (is_ploaded(ch))

	return TRUE;



    if (is_linkdead(ch))

	return TRUE;



    if (ch->level >= SUPREME)

	return FALSE;



    if (ch->level >= LEVEL_IMMORTAL && !area)

	return FALSE;



    if (!IS_NPC(ch) && IS_SET(ch->info,INFO_GHOST) )

	return TRUE;



    if (!IS_NPC(victim) && IS_SET(victim->info,INFO_GHOST) )

	return TRUE;



    /* killing mobiles */

    if (IS_NPC(victim))

    {

      /* safe room check */

	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))

		return TRUE;



	if (victim->pIndexData->pShop != NULL)

		return TRUE;



	/* no killing healers, trainers, etc */

	if (IS_SET(victim->act,ACT_TRAIN)

	||  IS_SET(victim->act,ACT_PRACTICE)

	||  IS_SET(victim->act,ACT_IS_HEALER)

	||  IS_SET(victim->act,ACT_BANKER)

	||  IS_SET(victim->act,ACT_ENGRAVER)

	||  IS_SET(victim->act,ACT_IS_CHANGER))

		return TRUE;



	if (!IS_NPC(ch))

	{

	    /* no pets */

	    if (IS_SET(victim->act,ACT_PET))

		return TRUE;



	    /* no charmed creatures unless owner */

	    if (IS_AFFECTED(victim,AFF_CHARM) && (area || ch != victim->master))

		return TRUE;



	    /* legal kill? -- cannot hit mob fighting non-group member */

	    if (victim->fighting != NULL && !is_same_group(ch,victim->fighting))

		return TRUE;

	}

	else

	{

	    /* area effect spells do not hit other mobs */

	    if (area && !is_same_group(victim,ch->fighting))

		return TRUE;

	}

    }

    /* killing players */

    else

    {

	/* NPC doing the killing */

	if (IS_NPC(ch))

	{

	    /* First off, mobs can't fry Immortals with Area Spells */

	    if ((ch->fighting != victim) && area && (victim->level >= LEVEL_IMMORTAL))

		return TRUE;



	    /* charmed mobs and pets cannot attack players while owned */

	    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL

	    &&  ch->master->fighting != victim)

		return TRUE;



	    /* safe room? */

	    if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))

		return TRUE;



	    /* legal kill? -- mobs only hit players grouped with opponent*/

	    if (ch->fighting != NULL && !is_same_group(ch->fighting,victim))

		return TRUE;

	}



	/* player doing the killing */

	else

	{

	    if (victim->level >=LEVEL_IMMORTAL && area && ch->level < LEVEL_HERO && ch->fighting != victim)

		return TRUE;



	    /* safe room? */

	    if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))

		return TRUE;



	    /* PK Safe Room Check, only stops killing players */

	    if (IS_SET(victim->in_room->room_flags,ROOM_NOPK))

		return TRUE;



	    /* No killing anyone in your group with an area spell */

	    if (area && is_same_group(ch,victim))

		return TRUE;



	    if (IS_SET(victim->act,PLR_KILLER) || IS_SET(victim->act,PLR_THIEF))

		return FALSE;



	    /* Arena's are free for all rooms, no restrictions */

	    if (IS_SET(victim->in_room->room_flags,ROOM_ARENA))

		return FALSE;



	    if (!is_clan(ch))

		return TRUE;



	    if (!is_clan(victim))

		return TRUE;



	    if ( totalwar )

		return FALSE;



	    if ( clan_table[ch->clan].pksafe )

		return TRUE;



	    if ( clan_table[victim->clan].pksafe )

		return TRUE;



	    if (ch->level <= 100 && (ch->level > victim->level + 10))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 200 && (ch->level > victim->level + 20))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 300 && (ch->level > victim->level + 30))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 400 && (ch->level > victim->level + 40))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else if (ch->level <= 500 && (ch->level > victim->level + 50))

	    {

		send_to_char("Pick on someone your own size.\n\r",ch);

		return TRUE;

	    }

	    else

		return FALSE;



	}



    }



    return FALSE;



}

/*

 * See if an attack justifies a KILLER flag.

 */

void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )

{

    char buf[MAX_STRING_LENGTH];

    /*

     * Follow charm thread to responsible character.

     * Attacking someone's charmed char is hostile!

     */

    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )

	victim = victim->master;



    /*

     * NPC's are fair game.

     * So are killers, thieves, or wanted people.

     */

    if ( IS_NPC(victim)

    ||   IS_SET(victim->act, PLR_WANTED)

    ||   IS_SET(victim->act, PLR_KILLER)

    ||   IS_SET(victim->act, PLR_THIEF))

	return;



    /*

     * Charm-o-rama.

     */

    if ( IS_SET(ch->affected_by, AFF_CHARM) )

    {

	if ( ch->master == NULL )

	{

	    char buf[MAX_STRING_LENGTH];



	    sprintf( buf, "Check_killer: %s bad AFF_CHARM",

		IS_NPC(ch) ? ch->short_descr : ch->name );

	    bug( buf, 0 );

	    affect_strip( ch, gsn_charm_person );

	    REMOVE_BIT( ch->affected_by, AFF_CHARM );

	    return;

	}

/*

	send_to_char( "*** You are now a KILLER!! ***\n\r", ch->master );

	SET_BIT(ch->master->act, PLR_KILLER);

*/



	stop_follower( ch );

	return;

    }



    /*

     * NPC's are cool of course (as long as not charmed).

     * Hitting yourself is cool too (bleeding).

     * So is being immortal (Alander's idea).

     * And current killers stay as they are.

     */

    if ( IS_NPC(ch)

    ||   ch == victim

    ||   ch->level >= LEVEL_IMMORTAL

    ||   IS_SET(ch->act, PLR_KILLER)

    ||   ch->fighting  == victim)

	return;



    send_to_char( "*** You are now a KILLER!! ***\n\r", ch );

    SET_BIT(ch->act, PLR_KILLER);

    sprintf(buf,"$N is attempting to murder %s",victim->name);

    wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);

    save_char_obj( ch );

    return;

}





/*

 * See if an attack justifies a WANTED flag.

 */

void check_wanted( CHAR_DATA *ch, CHAR_DATA *victim )

{

    char buf[MAX_STRING_LENGTH];



    /*

     * Follow charm thread to responsible character.

     * Attacking someone's charmed char is hostile!

     */

    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )

	victim = victim->master;



    /*

     * NPC's are fair game.

     * So are killers and thieves.

     */

    if ( IS_NPC(victim)

    ||   IS_SET(victim->act, PLR_WANTED)

    ||   IS_SET(victim->act, PLR_KILLER)

    ||   IS_SET(victim->act, PLR_THIEF))

	return;



    /*

     * Charm-o-rama.

     */

    if ( IS_SET(ch->affected_by, AFF_CHARM) )

    {

	if ( ch->master == NULL )

	{

	    sprintf( buf, "Check_wanted: %s bad AFF_CHARM",

		IS_NPC(ch) ? ch->short_descr : ch->name );

	    bug( buf, 0 );

	    affect_strip( ch, gsn_charm_person );

	    REMOVE_BIT( ch->affected_by, AFF_CHARM );

	    return;

	}



	send_to_char( "*** You are now a CRIMINAL!! ***\n\r", ch->master );

	SET_BIT(ch->master->act, PLR_WANTED);





	stop_follower( ch );

	return;

    }



    /*
     * NPC's are cool of course (as long as not charmed).
     * Hitting yourself is cool too (bleeding).
     * So is being immortal (Alander's idea).
     * And current killers stay as they are.
     */

    if ( IS_NPC(ch)
    ||   ch == victim
    ||   ch->level >= LEVEL_IMMORTAL
    ||   IS_SET(ch->act, PLR_WANTED)
    ||   ch->fighting  == victim)
	return;



    send_to_char( "*** You are now a CRIMINAL!! ***\n\r", ch );

    SET_BIT(ch->act, PLR_WANTED);

    wiznet("$N is now wanted!",ch,NULL,WIZ_FLAGS,0,0);

    save_char_obj( ch );

    return;

}







/*
 * Check for parry.
 */

bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, bool parryable )

{
    int chance;
	OBJ_DATA *wield;
	OBJ_DATA *second;

/* victim is parrying the ch */

    wield = get_eq_char(victim, WEAR_WIELD);
	second = get_eq_char(victim, WEAR_SECONDARY);

	if(wield!=NULL&&second!=NULL)
	{
		if ( wield->value[0]==WEAPON_PROJ && second->value[0]==WEAPON_PROJ)
			return FALSE;
	}

	if ( !IS_AWAKE(victim) )
	return FALSE;

    if (!parryable) return FALSE;

	chance = get_skill(victim,gsn_parry) / 2;

    if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
    {
	if (IS_NPC(victim))
	    chance /= 2;
	else
	    return FALSE;
    }

    if (!can_see(victim,ch))
	chance /= 2;

    /* modifiers */
    /* stats */
    chance -= get_curr_stat(ch,STAT_DEX);
    chance += get_curr_stat(victim,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance -= 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance += 15;

    /* level */
    if ( ch->level > (victim->level + 50) )
	chance -= (ch->level - victim->level);

	if ( victim->level > (ch->level + 50) )
		chance += (victim->level - ch->level);

	if (wield!=NULL)
	{
	if ( wield->value[0]==WEAPON_PROJ)
		chance /= 2;
	}
	if (second!=NULL)
	{
		if (second->value[0]==WEAPON_PROJ)
			chance /=2;
	}

    if ( number_percent( ) >= chance )
	return FALSE;

    act( "{WYou {yparry{W $n{W's attack.{w",  ch, NULL, victim, TO_VICT    );
    act( "{W$N{W {yparries{W your attack.{w", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_parry,TRUE,6);
    return TRUE;

}



/*

 * Check for shield block.

 */

bool check_shield_block( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;

/* Victim is blocking with their shield */

    if ( !IS_AWAKE(victim) )
	return FALSE;


    chance = get_skill(victim,gsn_shield_block) / 5 + 3;

    /* modifiers */

    if (!can_see(victim,ch))
	chance /= 2;

    /* stats */
    chance -= get_curr_stat(ch,STAT_DEX);
    chance += get_curr_stat(victim,STAT_DEX);
    chance -= get_curr_stat(ch,STAT_STR);
    chance += get_curr_stat(victim,STAT_STR);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance -= 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance += 15;

    /* level */
    if ( ch->level > (victim->level + 50) )
	chance -= (ch->level - victim->level);
    if ( victim->level > (ch->level + 50) )
	chance += (victim->level - ch->level);

    if ( get_eq_char( victim, WEAR_SHIELD ) == NULL )
	return FALSE;

    if ( number_percent( ) >= chance)
	return FALSE;

    act( "{cYou block $n{c's attack with your shield.{w",  ch, NULL, victim, TO_VICT    );
    act( "{c$N{c blocks your attack with a shield.{w", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_shield_block,TRUE,6);
    return TRUE;
}



/*
 * Check for dodge.
 */

bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;

/* victim is dodging ch */

    if ( !IS_AWAKE(victim) )
	return FALSE;

    chance = get_skill(victim,gsn_dodge) / 2;
	if (class_lookup("thief") == ch->class)
		chance = get_skill(victim,gsn_dodge) *3/5;
	
    /* modifiers */

    if (!can_see(victim,ch))
	chance /= 2;

    /* stats */
    chance -= get_curr_stat(ch,STAT_DEX);
    chance += get_curr_stat(victim,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance -= 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance += 15;

    /* level */
    if ( ch->level > (victim->level + 50) )
	chance -= (ch->level - victim->level);
    if ( victim->level > (ch->level + 50) )
	chance += (victim->level - ch->level);

    if ( number_percent( ) >= chance )
	return FALSE;

    act( "{WYou {mdodge{W $n{W's attack.{w", ch, NULL, victim, TO_VICT    );
    act( "{W$N{W {mdodges{W your attack.{w", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_dodge,TRUE,6);
    return TRUE;
}

bool check_evas( CHAR_DATA *ch, CHAR_DATA *victim, bool parryable, bool proj )
{
    int chance;
    int proj;

/* victim is evading ch */

    if ( !IS_AWAKE(victim) )
	return FALSE;
	
	if(parryable)
		return FALSE;
		
	if(IS_NPC(victim))
		return FALSE;
	
    chance = get_skill(victim,gsn_evasion) * .2;
    
    if (!parryable && proj)
    	chance = get_skill(victim,gsn_evasion) *.35;

    /* modifiers */

    if (!can_see(victim,ch))
	chance /= 2;

    /* stats */
    chance -= get_curr_stat(ch,STAT_AGI);
    chance += get_curr_stat(victim,STAT_AGI);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance -= 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance += 15;

    /* level */
    if ( ch->level > (victim->level + 50) )
	chance -= (ch->level - victim->level);
    if ( victim->level > (ch->level + 50) )
	chance += (victim->level - ch->level);

	if (class_lookup("thief") != victim->class)
		chance /= 2;

    if ( number_percent( ) >= chance )
	return FALSE;

    act( "{WYou deftly {cevade{W $n{W's attack.{w", ch, NULL, victim, TO_VICT    );
    act( "{W$N{W deftly {cevades{W your attack.{w", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_evasion,TRUE,6);
    return TRUE;
}

bool check_i_evas( CHAR_DATA *ch, CHAR_DATA *victim, bool parryable, bool proj )
{
    int chance;

/* victim is evading ch */

	if(parryable && proj)
		return FALSE;

    if ( !IS_AWAKE(victim) )
	return FALSE;

	if(IS_NPC(victim))
		return FALSE;

	//if(class_lookup("jedi") != victim->class)
	return FALSE;

    chance = get_skill(victim,gsn_improv_evasion) / 2;

	if (!parryable && proj)
    	chance = get_skill(victim,gsn_evasion) *.35;

    /* modifiers */

    if (!can_see(victim,ch))
	chance /= 2;

    /* stats */
    chance -= get_curr_stat(ch,STAT_AGI);
    chance += get_curr_stat(victim,STAT_AGI);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance -= 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance += 15;

    /* level */
    if ( ch->level > (victim->level + 50) )
	chance -= (ch->level - victim->level);
    if ( victim->level > (ch->level + 50) )
	chance += (victim->level - ch->level);

    if ( number_percent( ) >= chance )
	return FALSE;

    act( "{WYou deftly {cevade{W $n{W's attack with{D uncanny{W swiftness.{w", ch, NULL, victim, TO_VICT    );
    act( "{W$N{W deftly {cevades{W your attack with{D uncanny{W swiftness.{w", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_improv_evasion,TRUE,6);
    return TRUE;
}


/*
 * Set position of a victim.
 */

void update_pos( CHAR_DATA *victim )

{

    if ( victim->hit > 0 )

    {

	if ( victim->position <= POS_STUNNED )

	    victim->position = POS_STANDING;

	return;

    }



    if ( IS_NPC(victim) && victim->hit < 1 )

    {

	victim->position = POS_DEAD;

	return;

    }



    if ( victim->hit <= -11 )

    {

	victim->position = POS_DEAD;

	return;

    }



	 if ( victim->hit <= -6 ) victim->position = POS_MORTAL;

    else if ( victim->hit <= -3 ) victim->position = POS_INCAP;

    else                          victim->position = POS_STUNNED;



    return;

}







/*

 * Start fights.

 */

void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )

{

    if ( ch->fighting != NULL )

    {

	bug( "Set_fighting: already fighting", 0 );

	return;

    }



    if ( IS_AFFECTED(ch, AFF_SLEEP) )

	affect_strip( ch, gsn_sleep );



    ch->fighting = victim;

    ch->position = POS_FIGHTING;



    /*

     * Now - Let's frag the plevelers

     */

    if ( IS_NPC(victim) && (!is_same_group(ch, victim->was_fighting) || victim->was_fighting == NULL) )

    {

	affect_strip(victim,gsn_plague);

	affect_strip(victim,gsn_poison);

	affect_strip(victim,gsn_blindness);

	affect_strip(victim,gsn_sleep);

	affect_strip(victim,gsn_curse);

	affect_strip(victim,skill_lookup("fire breath"));

	affect_strip(victim,skill_lookup("calm"));

	affect_strip(victim,skill_lookup("change sex"));

	affect_strip(victim,skill_lookup("charm person"));

	affect_strip(victim,skill_lookup("chill touch"));

	affect_strip(victim,skill_lookup("faerie fire"));

	affect_strip(victim,skill_lookup("slow"));

	affect_strip(victim,skill_lookup("weaken"));



	victim->hit 	= victim->max_hit;

	victim->mana	= victim->max_mana;

	victim->move	= victim->max_move;

	update_pos( victim );

    }



    victim->was_fighting = ch;



    if (!IS_NPC(victim))

	save_char_obj( victim );



    return;

}







/*

 * Stop fights.

 */

void stop_fighting( CHAR_DATA *ch, bool fBoth )

{

    CHAR_DATA *fch;



    for ( fch = char_list; fch != NULL; fch = fch->next )

    {

	if ( fch == ch || ( fBoth && fch->fighting == ch ) )

	{

	    fch->fighting       = NULL;

	    fch->position       = IS_NPC(fch) ? fch->default_pos : POS_STANDING;

	    update_pos( fch );

	}

    }



    return;

}







/*

 * Make a corpse out of a character.

 */

void make_corpse( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;

    if ( IS_NPC(ch) )
    {
	name            = ch->short_descr;
	corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
	corpse->timer   = number_range( 3, 6 );
	if ( ch->gold > 0 )
	{
	    obj_to_obj( create_money( ch->gold, ch->silver ), corpse );
	    ch->gold = 0;
	    ch->silver = 0;
	}
	corpse->cost = 0;
    }
    else
    {
	if ( IS_SET(ch->info, INFO_GHOST) )
		return;

	name            = ch->name;
	corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
	corpse->timer   = number_range( 25, 40 );
	REMOVE_BIT(ch->act,PLR_CANLOOT);
	if (!is_clan(ch))
	    corpse->owner = str_dup(ch->name);
	else
	{
	    corpse->owner = NULL;
	    if (ch->gold > 1 || ch->silver > 1)
	    {
		obj_to_obj(create_money(ch->gold / 2, ch->silver/2), corpse);
		ch->gold -= ch->gold/2;
		ch->silver -= ch->silver/2;
	    }
	}

	corpse->cost = 0;
    }

    corpse->level = ch->level;

    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	bool floating = FALSE;

	obj_next = obj->next_content;
	if (obj->wear_loc == WEAR_FLOAT)
	    floating = TRUE;
	obj_from_char( obj );
	if (obj->item_type == ITEM_POTION)
	    obj->timer = number_range(500,1000);
	if (obj->item_type == ITEM_SCROLL)
	    obj->timer = number_range(1000,2500);
	if (IS_SET(obj->extra_flags,ITEM_ROT_DEATH) && !floating)
	{
	    obj->timer = number_range(5,10);
	    REMOVE_BIT(obj->extra_flags,ITEM_ROT_DEATH);
	}
	REMOVE_BIT(obj->extra_flags,ITEM_VIS_DEATH);

	if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	    extract_obj( obj );
	else if (floating)
	{
	    if (IS_OBJ_STAT(obj,ITEM_ROT_DEATH)) /* get rid of it! */
	    {
		if (obj->contains != NULL)
		{
		    OBJ_DATA *in, *in_next;

		    act("$p evaporates,scattering its contents.",
			ch,obj,NULL,TO_ROOM);
		    for (in = obj->contains; in != NULL; in = in_next)
		    {
			in_next = in->next_content;
			obj_from_obj(in);
			obj_to_room(in,ch->in_room);
		    }
		 }
		 else
		    act("$p evaporates.",
			ch,obj,NULL,TO_ROOM);
		 extract_obj(obj);
	    }
	    else
	    {
		act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
		obj_to_room(obj,ch->in_room);
	    }
	}
	else
	    obj_to_obj( obj, corpse );
    }

    obj_to_room( corpse, ch->in_room );
    return;
}




/*

 * Improved Death_cry contributed by Diavolo.

 */

void death_cry( CHAR_DATA *ch )

{

    ROOM_INDEX_DATA *was_in_room;

    char *msg;

    int door;

    int vnum;



    vnum = 0;

    msg = "You hear $n's death cry.";



    switch ( number_bits(5))

    {

    case  0: msg  = "$n hits the ground ... {WDEAD{w.";                     break;

    case  1:

	if (ch->material == 0)

	{

	    msg  = "$n splatters {rblood{w on your armor.";

	    break;

	}

    case  2:

	if (IS_SET(ch->parts,PART_GUTS))

	{

	    msg = "{m$n spills $s guts all over the floor.{w";

	    vnum = OBJ_VNUM_GUTS;

	}

	break;

    case  3:

	if (IS_SET(ch->parts,PART_HEAD))

	{

	    msg  = "{c$n's severed head plops on the ground.{w";

	    vnum = OBJ_VNUM_SEVERED_HEAD;

	}

	break;

    case  4:

	if (IS_SET(ch->parts,PART_HEART))

	{

	    msg  = "{r$n's heart is torn from $s chest.{w";

	    vnum = OBJ_VNUM_TORN_HEART;

	}

	break;

    case  5:

	if (IS_SET(ch->parts,PART_ARMS))

	{

	    msg  = "$n's arm is sliced from $s dead body.";

	    vnum = OBJ_VNUM_SLICED_ARM;

	}

	break;

    case  6:

	if (IS_SET(ch->parts,PART_LEGS))

	{

	    msg  = "$n's leg is sliced from $s dead body.";

	    vnum = OBJ_VNUM_SLICED_LEG;

	}

	break;

    case 7:

	if (IS_SET(ch->parts,PART_BRAINS))

	{

	    msg = "{W$n's head is {gshattered{W, and $s brains splash all over you.{w";

	    vnum = OBJ_VNUM_BRAINS;

	}
	break;
    case 8:
        if (IS_SET(ch->parts,PART_HANDS))
                {
                            msg = "{W$n's hand is {gsevered{W, and $s blood sprays you in the face.{w";
                                vnum = OBJ_VNUM_HANDS;
                            }
        break;
    case 9:
        if (IS_SET(ch->parts,PART_FEET))
                {
                            msg = "{W$n's foot is {ghacked off{W, and $s foot flies up into the air.{w";
                                vnum = OBJ_VNUM_FEET;
                            }
        break;
    case 10:
        if (IS_SET(ch->parts,PART_FINGERS))
                {
                            msg = "{W$n's finger is {gchopped off{W.{w";
                                vnum = OBJ_VNUM_FINGERS;
                            }
        break;
    case 11:
        if (IS_SET(ch->parts,PART_EAR))
                {
                            msg = "{W$n's ear is {gsevered{W from his head.{w";
                                vnum = OBJ_VNUM_EAR;
                            }
        break;
    case 12:
        if (IS_SET(ch->parts,PART_EYE))
                {
                            msg = "{W$n's eye is {ggouged out{W.{w";
                                vnum = OBJ_VNUM_EYE;
                            }
        break;
    case 13:
        if (IS_SET(ch->parts,PART_LONG_TONGUE))
                {
                            msg = "{W$n's tongue is {gripped out{W, and $s blood squirts out of the stump.{w";
                                vnum = OBJ_VNUM_LONG_TONGUE;
                            }
        break;
    case 14:
        if (IS_SET(ch->parts,PART_EYESTALKS))
                {
                            msg = "{W$n's eyestalk is {gchopped{W, and $s eyeball flies off.{w";
                                vnum = OBJ_VNUM_EYESTALKS;
                            }
        break;
    case 15:
        if (IS_SET(ch->parts,PART_TENTACLES))
                {
                            msg = "{W$n's tentacle is {ghacked off{W.{w";
                                vnum = OBJ_VNUM_TENTACLES;
                            }
        break;
    case 16:
        if (IS_SET(ch->parts,PART_FINS))
                {
                            msg = "{W$n's fin is {gsliced off{W.{w";
                                vnum = OBJ_VNUM_FINS;
                            }
        break;
    case 17:
        if (IS_SET(ch->parts,PART_WINGS))
                {
                            msg = "{W$n's wing is {gsevered{W, and $s blood squirts all over the place.{w";
                                vnum = OBJ_VNUM_WINGS;
                            }
        break;
    case 18:
        if (IS_SET(ch->parts,PART_TAIL))
                {
                            msg = "{W$n's tail is {gchopped off{W.{w";
                                vnum = OBJ_VNUM_TAIL;
                            }
        break;
    case 19:
        if (IS_SET(ch->parts,PART_CLAWS))
                {
                            msg = "{W$n's claw is {gsliced off{W.{w";
                                vnum = OBJ_VNUM_CLAWS;
                            }
        break;
    case 20:
        if (IS_SET(ch->parts,PART_FANGS))
                {
                            msg = "{W$n's fang is {gknocked out{W.{w";
                                vnum = OBJ_VNUM_FANGS;
                            }
        break;
    case 21:
        if (IS_SET(ch->parts,PART_HORNS))
                {
                            msg = "{W$n's horn is {gsevered{W, and $s blood sprays you in the face.{w";
                                vnum = OBJ_VNUM_HORNS;
                            }
        break;
    case 22:
        if (IS_SET(ch->parts,PART_SCALES))
                {
                            msg = "{WOne of $n's scales is {gknocked loose{W.{w";
                                vnum = OBJ_VNUM_SCALES;
                            }
        break;
    case 23:
        if (IS_SET(ch->parts,PART_TUSKS))
                {
                            msg = "{W$n's tusk is {gsliced off{W.{w";
                                vnum = OBJ_VNUM_TUSKS;
                            }

    }



    act( msg, ch, NULL, NULL, TO_ROOM );



    if ( vnum != 0 )

    {

	char buf[MAX_STRING_LENGTH];

	OBJ_DATA *obj;

	char *name;



	name            = IS_NPC(ch) ? ch->short_descr : ch->name;

	obj             = create_object( get_obj_index( vnum ), 0 );

	obj->timer      = number_range( 4, 7 );



	if (vnum == OBJ_VNUM_SEVERED_HEAD)
	    obj->timer	= -1;
    if (vnum == OBJ_VNUM_CLAWS )
        obj->timer   = -1;
    if (vnum == OBJ_VNUM_FANGS )
        obj->timer   = -1;
    if (vnum == OBJ_VNUM_HORNS )
        obj->timer   = -1;
    if (vnum == OBJ_VNUM_SCALES )
        obj->timer   = -1;
    if (vnum == OBJ_VNUM_TUSKS )
        obj->timer   = -1;

	sprintf( buf, obj->short_descr, name );

	free_string( obj->short_descr );

	obj->short_descr = str_dup( buf );



	sprintf( buf, obj->description, name );

	free_string( obj->description );

	obj->description = str_dup( buf );



	if (obj->item_type == ITEM_FOOD)

	{

	    if (IS_SET(ch->form,FORM_POISON))

		obj->value[3] = 1;

	    else if (!IS_SET(ch->form,FORM_EDIBLE))

		obj->item_type = ITEM_TRASH;

	}



	obj_to_room( obj, ch->in_room );

    }



    if ( IS_NPC(ch) )

	msg = "You hear something's death cry.";

    else

	msg = "You hear someone's death cry.";



    was_in_room = ch->in_room;

    for ( door = 0; door <= 5; door++ )

    {

	EXIT_DATA *pexit;



	if ( ( pexit = was_in_room->exit[door] ) != NULL

	&&   pexit->u1.to_room != NULL

	&&   pexit->u1.to_room != was_in_room )

	{

	    ch->in_room = pexit->u1.to_room;

	    act( msg, ch, NULL, NULL, TO_ROOM );

	}

    }

    ch->in_room = was_in_room;



    return;

}



/* URK_KILLCOUNTER */

void cnt_kill( CHAR_DATA *victim, CHAR_DATA *ch)

{



    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill++;

    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath++;

    if (!IS_NPC(victim) && !IS_NPC(ch) && ch != victim)

    {

        ch->pkill++;

        victim->pdeath++;

    }

    if (!IS_NPC(victim) && IS_IMMORTAL(ch) && ch != victim)
    {
        ch->slaykill++;
        victim->slaydeath++;
    }

    return;

}



void raw_kill( CHAR_DATA *victim )
{
    int i;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *looker;

    stop_fighting( victim, TRUE );

    death_cry( victim );
    make_corpse( victim );

    if ( IS_NPC(victim) )
    {
		victim->pIndexData->killed++;
		kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
		extract_char( victim, TRUE );
		return;
    }

    if (   !IS_NPC( victim )
	  && !IS_IMMORTAL(victim)
	  && victim->level > 50
	  && (victim->race != race_lookup("vampire") )
	   )
    {
		SET_BIT( victim->info, INFO_GHOST );
		victim->pcdata->timer_ghost = UMAX( 10, (victim->level/10) );
		victim->pcdata->died_in_room = victim->in_room;
    }

    extract_char( victim, FALSE );
    while ( victim->affected )
	affect_remove( victim, victim->affected );
    victim->affected_by = race_table[victim->race].aff;
    for (i = 0; i < 4; i++)
	victim->armor[i]= 100;
    victim->position    = POS_RESTING;
    victim->hit         = UMAX( 1, victim->hit  );
    victim->mana        = UMAX( 1, victim->mana );
    victim->move        = UMAX( 1, victim->move );

    if ( !IS_NPC( victim ) )
    {
	for ( looker = victim->in_room->people; looker; looker = looker->next_in_room )
	{
		if ( IS_AWAKE( looker ) && can_see(looker,victim) && (looker != victim) )
		{
			sprintf( buf, "Having fallen victim to death, %s returns once again.{w\n\r", victim->name );
			send_to_char( buf, looker );
		}
	}
	sprintf( buf, "Having fallen victim to death, you return once again to %s{w.\n\r", victim->in_room->name );
	send_to_char( buf, victim );
	do_look( victim, "auto" );
    }

    if ( !IS_NPC( victim ) )
	save_char_obj( victim );

    return;
}




void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )

{

    char buf[MAX_STRING_LENGTH];

    CHAR_DATA *gch;

    CHAR_DATA *lch;

    DESCRIPTOR_DATA *d;

    int xp;

    int members;

    int group_levels;

    bool imm_in_group;

    bool mort_in_group;

    int max_glvl;

    int min_glvl;



    /*

     * Monsters don't get kill xp's or alignment changes.

     * P-killing doesn't help either.

     * Dying of mortal wounds or poison doesn't give xp to anyone!

     * Having an Immortal in your group COSTS you xp!

     */

    if ( victim == ch )

	return;



    if (IS_SET(ch->act, PLR_QUESTOR)&& IS_NPC(victim))

    {

	if (ch->pcdata->questmob == victim->pIndexData->vnum)

	{

		send_to_char("You have almost completed your QUEST!\n\r",ch);

		send_to_char("Return to the questmaster before your time runs out!\n\r",ch);

		ch->pcdata->questmob = -1;

	}

    }



    imm_in_group 	= FALSE;

    mort_in_group	= FALSE;



    /*

     * First we test the group members throughout the mud

     * Looking for 2 things...

     * Are they an Immortal?

     * Was one of us the last one to attack the dead mob?

     * Once we have these answers, we deal with those players

     * then we go on to handle the exp awards...

     */



    if ( IS_IMMORTAL(ch) )

    {

	imm_in_group	= TRUE;

    }

    else

    {

	mort_in_group	= TRUE;

    }



    members = 0;

    group_levels = 0;

    max_glvl = 0;

    min_glvl = MAX_LEVEL;



    for ( d = descriptor_list; d; d = d->next )

    {

	gch = d->character;



	if ( is_same_group( ch, gch ) )

	{

	    members++;



	    group_levels += IS_NPC(gch) ? gch->level / 2 : gch->level;



	    if ( !IS_NPC(gch) )

	    {

		group_levels += gch->level;



		if ( gch->level < min_glvl )

		{

			min_glvl = gch->level;

		}



		if ( gch->level > max_glvl )

		{

			max_glvl = gch->level;

		}



		if ( IS_IMMORTAL(gch) )

		{

			imm_in_group 	= TRUE;

		}

		else

		{

			mort_in_group	= TRUE;

		}



		if ( gch->pet !=NULL )

		{

			group_levels += (gch->pet->level/2);

		}

	    }

	    else

	    {

		group_levels += (gch->level/2);

	    }

	}

    }





    if ( members == 0 )

    {

	bug( "Group_gain: members.", members );

	members = 1;

	group_levels = ch->level ;

    }



    lch = (ch->leader != NULL) ? ch->leader : ch;



    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )

    {

	OBJ_DATA *obj;

	OBJ_DATA *obj_next;



	if ( !is_same_group( gch, ch ) || IS_NPC(gch))

	    continue;





	/*

	 * Killing a Mob that someone else weakend for us - PowerLeveling - is a bad thing

	 * Also includes other PowerLeveling checks

	 *

	 * Modified to remove the PowerLeveling checks

	 */





	if (imm_in_group && (members > 1) && mort_in_group)

	{

		/* Deal with having an Immortal Grouped */



		xp = -1000;

		gain_exp( gch, xp );

		if ( gch->level >= LEVEL_HERO )

			send_to_char( "{rYou were illegally grouped with a mortal!{w\n\r", gch );

		else

			send_to_char( "You were grouped with an Immortal!\n\r", gch );

		sprintf( buf, "You receive {r%d{w experience points as punishment!\n\r", xp );

		send_to_char( buf, gch );

		wiznet("$N was in an illegal grouping between mortals and Immortals.",gch,NULL,WIZ_SECURE,0,get_trust(gch));

		sprintf( buf, "%s was in an illegal grouping between mortals and Immortals", ch->name );

		log_string( buf );

	}

	else if ( max_glvl < 50 )

	{

		xp = xp_compute( gch, victim, group_levels );

		if ( members > 1 )

		{

			xp = xp * get_combat_value( "m_group_xp" );

			xp = xp / get_combat_value( "d_group_xp" );

		}

		sprintf( buf, "You receive {y%d{w experience points.\n\r", xp );

		send_to_char( buf, gch );

		gain_exp( gch, xp );

	}

	else if ( max_glvl >= 50 )

	{

		xp = xp_compute( gch, victim, group_levels );

		if ( members > 1 )

		{

			xp = xp * get_combat_value( "m_group_xp" );

			xp = xp / get_combat_value( "d_group_xp" );

		}

		sprintf( buf, "You receive {y%d{w experience points.\n\r", xp );

		send_to_char( buf, gch );

		gain_exp( gch, xp );

	}

	else

	{

		xp = xp_compute( gch, victim, group_levels );

		if ( members > 1 )

		{

			xp = xp * get_combat_value( "m_group_xp" );

			xp = xp / get_combat_value( "d_group_xp" );

		}

		sprintf( buf, "You receive {y%d{w experience points.\n\r", xp );

		send_to_char( buf, gch );

		gain_exp( gch, xp );

	}



	for ( obj = ch->carrying; obj != NULL; obj = obj_next )

	{

	    obj_next = obj->next_content;

	    if ( obj->wear_loc == WEAR_NONE )

		continue;



	    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )

	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )

	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )

	    {

		act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );

		act( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );

		obj_from_char( obj );

		obj_to_room( obj, ch->in_room );

	    }

	}

    }



    victim->was_fighting = NULL;



    return;

}







/*

 * Compute xp for a kill.

 * Also adjust alignment of killer.

 * Edit this function to change xp computations.

 */

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim, int total_levels )
{
    int xp,base_exp;
    int align;
    int level_range;
    int change;
    int EXP_Level;
    char wnetbuf[MAX_STRING_LENGTH];


/*    int hours_played; */

    /* If this was done through PK, return 0 */
    if (!IS_NPC(gch) && !IS_NPC(victim))
    {
	xp = 0;
	return xp;
    }

    level_range = victim->level - gch->level;

    /* compute the base exp */
    switch (level_range)
    {
	default :       base_exp =   0;         break;
	case -10:       base_exp =   1;         break;
	case -9 :       base_exp =   5;         break;
	case -8 :       base_exp =  10;         break;
	case -7 :       base_exp =  15;         break;
	case -6 :       base_exp =  20;         break;
	case -5 :       base_exp =  25;         break;
	case -4 :       base_exp =  30;         break;
	case -3 :       base_exp =  35;         break;
	case -2 :       base_exp =  40;         break;
	case -1 :       base_exp =  45;         break;
	case  0 :       base_exp =  50;         break;
	case  1 :       base_exp =  55;         break;
	case  2 :       base_exp =  60;         break;
	case  3 :       base_exp =  65;         break;
	case  4 :       base_exp =  70;         break;
	case	5 :	    base_exp =  75;	    break;
	case	6 :	    base_exp =  80;	    break;
	case	7 :	    base_exp =  85;	    break;
	case	8 :	    base_exp =  90;	    break;
	case	9 :	    base_exp =  95;	    break;
	case 10 :	    base_exp = 100;	    break;
    }

    if ( (level_range > 10) && (level_range <= 50) )
	base_exp = (level_range * 10);

    if ( level_range > 50 )
	base_exp = 500;

    /* do alignment computations */

    align = victim->alignment - gch->alignment;

    if (IS_SET(victim->act,ACT_NOALIGN)||gch->race==race_lookup("vampire"))
    {
	/* no change */
    }

    else if (align > 500) /* monster is more good than slayer */
    {
	change = (align - 500) * base_exp / 500 * gch->level/total_levels;
	change = UMAX(1,change);
	gch->alignment = UMAX(-1000,gch->alignment - change);
    }

    else if (align < -500) /* monster is more evil than slayer */
    {
	change =  ( -1 * align - 500) * base_exp/500 * gch->level/total_levels;
	change = UMAX(1,change);
	gch->alignment = UMIN(1000,gch->alignment + change);
    }

    else /* improve this someday */
    {
	change =  gch->alignment * base_exp/500 * gch->level/total_levels;
	gch->alignment -= change;
    }

    /*   Alignment based XP */

    /* calculate exp multiplier  */
    if (IS_SET(victim->act,ACT_NOALIGN))
	xp = base_exp;

    else if (gch->alignment > 500)         /* for goodie two shoes */
    {
	if (victim->alignment < -750)      /* We kill something real bad */
	    xp = (base_exp * 2);

	else if (victim->alignment < -500) /* We kill something sorta bad */
	    xp = (base_exp * 3)/2;

	else if (victim->alignment > 750)  /* We kill something real good */
	    xp = base_exp / 2;

	else if (victim->alignment > 500)  /* We kill something sorta good */
	    xp = (base_exp * 3)/4;

	else
	    xp = base_exp;
    }

    else if (gch->alignment < -500)        /* for baddies */
    {
	if (victim->alignment > 750)       /* We kill something real good */
	    xp = (base_exp * 2);

	else if (victim->alignment > 500)  /* We kill something kinda good */
	    xp = (base_exp * 3)/2;

	else if (victim->alignment < -750) /* We kill something real bad */
	    xp = base_exp / 2;

	else if (victim->alignment < -500) /* We kill something kinda bad */
	    xp = (base_exp * 3)/4;

	else
	    xp = base_exp;
    }

    else  /* neutral  */
    {
	if (victim->alignment > 750 || victim->alignment < -750)
	    xp = (base_exp * 2);

	else if (victim->alignment > 500 || victim->alignment < -500)
	    xp = (base_exp * 3)/2;

	else if (victim->alignment > 250 || victim->alignment < -250)
	    xp = base_exp;

	else
	    xp = (base_exp * 3)/4;
    }


    /* more exp at the low levels */
    if (gch->level < 50)
	xp = ( ( 40 * xp ) / ( gch->level + 4 ) );


    /* Add time based xp */

	/* 100 Hour Bonus of 10% */
	if (GET_HOURS(gch) >= 100 && level_range >= -10)
		xp = xp + xp/10;

	/* 200 Hours and remort or level 100 gives 10% bonus */
	if (GET_HOURS(gch) >= 200 && level_range >= -10)
	{
		if (IS_REMORT(gch))
			xp += GET_HOURS(gch)/100;
		/* for every 200 hrs, you get another 10% bonus */
		xp = xp + ( ( ( GET_HOURS(gch)/200 ) * (1/10) ) * xp );
	}

	/* At level 600 remorts get a bonus of 1% of their hours */
	if (IS_REMORT(gch) && GET_HOURS(gch) >= 600 && level_range >= -10)
		xp += GET_HOURS(gch)/100;

    /* randomize the rewards */
    xp = number_range (xp * 3/4, xp * 5/4);

    /* adjust for grouping */
    xp = xp * gch->level/( UMAX(1,total_levels -1) );

    /* 2/3 XP for having a pet tank for you */
    if (gch->pet != NULL)
	xp = xp*2/3;

    /* Global XP Modification */
    xp = xp * get_combat_value("m_base_xp");
    xp = xp / get_combat_value("d_base_xp");

    if (IS_REMORT(gch))
    {
	xp = xp * get_combat_value("m_remort_xp");
	xp = xp * get_combat_value("d_remort_xp");
    }

    /* Make sure we get at least 1 xp point */
    if ( xp < 1 )
	xp = 1;

    /* Ensure that Experience is not more than 10% over the Experience per Level */
    EXP_Level = exp_per_level(gch,gch->pcdata->points);

    if ( xp > (EXP_Level+(EXP_Level/10)) )
	xp = (EXP_Level+(EXP_Level/10));

    /* If you kill something more than 20 levels below you, you get 1 exp */
    if (level_range < -20)
    {
	send_to_char( "Wow! Look at you picking on little puny things!\n\r", gch );
	xp = 1;
    }

    /* If you kill something more than 50 levels below you, you get 0 exp */
    if (level_range < -50)
    {
	send_to_char( "Killing something that puny is not a challege!\n\r", gch );
	xp = 0;
    }

    /* If they're at or above MAX_MORT_LEVEL they get no XP */
    if (gch->level >= 500)
    {
	send_to_char( "You are at or above level 500, you get no experience.\n\r", gch );
	xp = 0;
    }

    /* If they are an invalid character (more than 24000 exp to level) they get nothing */
    if (exp_per_level(gch,gch->pcdata->points) > 24000)
    {
	send_to_char( "{rYou are an invalid character created with more than 100 Creation Points.\n\rYou receive no experience points!{w\n\r", gch );
	sprintf( wnetbuf, "Invalid Character: %s has more than 100 Creation Points.  No EXP awarded.", gch->name );
	wiznet(wnetbuf,NULL,NULL,WIZ_SECURE,0,get_trust(gch));
	log_string( wnetbuf );
	xp = 0;
    }

    return xp;
}



void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune )
{
    char buf1[256];
    char buf2[256];
    char buf3[256];
    char buf4[256];
    char buf5[256];
    const char * vs;
    const char * vp;
    const char * attack;
    char punct;

    if (ch == NULL || victim == NULL)
	return;

    if      ( dam ==   0 ) { vs = "miss";       vp = "misses";          }
    else if ( dam <=   5 ) { vs = "scratch";    vp = "scratches";       }
    else if ( dam <=  10 ) { vs = "graze";      vp = "grazes";          }
    else if ( dam <=  15 ) { vs = "hit";        vp = "hits";            }
    else if ( dam <=  20 ) { vs = "pound";      vp = "pounds";          }
    else if ( dam <=  25 ) { vs = "hurt";       vp = "hurts";           }
    else if ( dam <=  30 ) { vs = "grind";      vp = "grinds";          }
    else if ( dam <=  35 ) { vs = "injure";     vp = "injures";         }
    else if ( dam <=  40 ) { vs = "wound";      vp = "wounds";          }
    else if ( dam <=  45 ) { vs = "maul";       vp = "mauls";           }
    else if ( dam <=  50 ) { vs = "decimate";   vp = "decimates";       }
    else if ( dam <=  55 ) { vs = "devastate";  vp = "devastates";      }
    else if ( dam <=  60 ) { vs = "maim";       vp = "maims";           }
    else if ( dam <=  65 ) { vs = "MUTILATE";   vp = "MUTILATES";       }
    else if ( dam <=  70 ) { vs = "DISEMBOWEL"; vp = "DISEMBOWELS";     }
    else if ( dam <=  75 ) { vs = "DISMEMBER";  vp = "DISMEMBERS";      }

    else if ( dam <=  100 ) { vs = "{RMAS{rSA{RCRE{w";         vp = "{RMAS{rSA{RCRES{w";        }
    else if ( dam <=  125 ) { vs = "{RM{BA{RN{BG{RL{BE{w";     vp = "{RM{BA{RN{BG{RL{BE{RS{w";  }
    else if ( dam <=  150 ) { vs = "{rG{Dr{rI{Dn{rD{Ds{w";     vp = "{rG{Dr{rI{Dn{rD{Ds{w";     }

    else if ( dam <=  175 ) { vs = "{C*** {cDE{CMO{cLISH {C***{w";
			            vp = "{C*** {cDE{CMO{cLISHES {C***{w";                         }
    else if ( dam <=  200 ) { vs = "{D*** {MDEVASTATE {D***{w";
			            vp = "{D*** {MDEVASTATES {D***{w";                             }
    else if ( dam <=  225 ) { vs = "{y=== {YOBLITERATE {y==={w";
			            vp = "{y=== {YOBLITERATES {y==={w";                            }
    else if ( dam <=  250 ) { vs = "{c>>> {GANNIHILATE {c<<<{w";
			            vp = "{c>>> {GANNIHILATES {c<<<{w";                            }
    else if ( dam <=  275 ) { vs = "{m<<< {MERADICATE {m>>>{w";
			            vp = "{m<<< {MERADICATES {m>>>{w";                             }
    else if ( dam <=  300 ) { vs = "{y({Y={y*{Y={y){r VAPORIZES {y({Y={y*{Y={y){w";
			            vp = "{y({Y={y*{Y={y){r VAPORIZES {y({Y={y*{Y={y){w";          }
    else if ( dam <=  325 ) { vs = "{M<{m^{M>{W ATOMIZES {M<{m^{M>{w";
			            vp = "{M<{m^{M>{W ATOMIZES {M<{m^{M>{w";                       }
    else if ( dam <=  350 ) { vs = "{G={g+{G={rKick {cthe {Wshit {wout {bof {G={g+{G={w";
			            vp = "{G={g+{G={rKicks {cthe {Wshit {wout {bof {G={g+{G={w";   }
    else if ( dam <=  375 ) { vs = "{C*** {cPOUND{w to{W MUSH {C***{w";
			            vp = "{C*** {cPOUND{w to{W MUSH {C***{w";                      }
    else if ( dam <=  400 ) { vs = "{yHurts {rReally {WBadly{w";
			            vp = "{yHurts {rReally {WBadly{w{w";                           }
    else if ( dam <=  425 ) { vs = "do {rUNSPEAKABLE{w things to";
			            vp = "does {rUNSPEAKABLE{w things to";                         }
    else if ( dam <=  450 ) { vs = "{g))) {GCASTRATES{g ((({w";
			            vp = "{g))) {GCASTRATES{g ((({w";                              }
    else if ( dam <=  475 ) { vs = "{R<{D({R<{DKACKS{R>{D){R>{w";
			            vp = "{R<{D({R<{DKACKS{R>{D){R>{w";                            }
    else if ( dam <=  500 ) { vs = "{R<{Y({r<{yDISINTIGRATES{r>{Y){R>{w";
			            vp = "{R<{Y({r<{yDISINTIGRATES{r>{Y){R>{w";                    }
    else if ( dam <=  550 ) { vs = "{C==> {cRIPS TO SHREDS{C <=={w";
			            vp = "{C==> {cRIPS TO SHREDS{C <=={w";                        }
    else if ( dam <=  600 ) { vs = "{M=*= {mMAKE A GRUESOME MESS{M =*={w of";
			            vp = "{M=*= {mMAKES A GRUESOME MESS{M =*={w of";               }
    else if ( dam <= 1000 ) { vs = "{y!!! {rDO REALLY MEAN THINGS{y !!!{w to";
			            vp = "{y!!! {rDOES REALLY MEAN THINGS{y !!!{w to";             }
    else if ( dam <= 2000 ) { vs = "{B=={b={B=={b={B> {RtEa{rRs {RA n{rEw A{RsSh{rOlE {B<{b=={B={b=={B={w into";
			            vp = "{B=={b={B=={b={B> {RtEa{rRs {RA n{rEw A{RsSh{rOlE {B<{b=={B={b=={B={w into";             }
    else if ( dam <= 5000 ) { vs = "{r=(!)= {yTOTALLY FUCKING DESTROYS{r =(!)={w";
			            vp = "{r=(!)= {yTOTALLY FUCKING DESTROYS{r =(!)={w";           }
    else { vs = "{D >>>> {WSMITES {D<<<< {w";
			            vp = "{D >>>> {WSMITES {D<<<< {w";           }

    punct   = (dam <= 36) ? '.' : '!';

    if ( dt == TYPE_HIT )
    {
		if (ch  == victim)
		{
		    sprintf( buf1, "$n %s $melf%c",vp,punct);
		    sprintf( buf2, "You %s yourself%c",vs,punct);
		}
		else
		{
		    sprintf( buf1, "$n %s $N%c",  vp, punct );
		    sprintf( buf2, "You %s $N%c", vs, punct );
		    sprintf( buf3, "$n %s you%c", vp, punct );
		}
    }
    else
    {
	    if(ch==victim && dt == TYPE_SUNLIGHT)//Grayson Vampire 29 June 2004
	    {
			sprintf( buf1, "{YSunlight{w %s $n%c",vp,punct);
			sprintf( buf2, "{YSunlight{w %s you%c",vp,punct);		    	
	    }
	    else
		{	
		    if(ch==victim && dt == TYPE_HUNGER)//Grayson Vampire 29 June 2004
		    {
				sprintf( buf1, "$n's hunger %s $m%c",vp,punct);
				sprintf( buf2, "Your hunger %s you%c",vp,punct);	    	
		    }
		    else
		    {
				if(ch==victim && dt == TYPE_THIRST)//Grayson Vampire 29 June 2004
		    	{
					sprintf( buf1, "$n's thirst %s $m%c",vp,punct);
					sprintf( buf2, "Your thirst %s you%c",vp,punct);	    	
		    	}
		    	else
		    	{
					if ( dt >= 0 && dt < MAX_SKILL )
					    attack      = skill_table[dt].noun_damage;
					else if ( dt >= TYPE_HIT
					&& dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
					    attack      = attack_table[dt - TYPE_HIT].noun;
					else
					{
					    bug( "Dam_message: bad dt %d.", dt );
					    dt  = TYPE_HIT;
					    attack  = attack_table[0].name;
					}
				
					if (immune)
					{
					    if (ch == victim)
					    {
							sprintf(buf1,"{c$n is unaffected by $s own %s.{w",attack);
							sprintf(buf2,"{cLuckily, you are immune to that.{w");
					    }
					    else
					    {
							sprintf(buf1,"{c$N is unaffected by $n's %s!{w",attack);
							sprintf(buf2,"{c$N is unaffected by your %s!{w",attack);
							sprintf(buf3,"{c$n's %s is powerless against you.{w",attack);
					    }
					}
					else
					{
					    if (ch == victim)
					    {
							sprintf( buf1, "$n's %s %s $m%c",attack,vp,punct);
							sprintf( buf2, "Your %s %s you%c",attack,vp,punct);
					    }
					    else
					    {
							sprintf( buf1, "$n's %s %s $N%c",  attack, vp, punct );
							sprintf( buf2, "Your %s %s $N%c",  attack, vp, punct );
							sprintf( buf3, "$n's %s %s you%c", attack, vp, punct );
					    }
					}
		    	}
		    }
		}
    }

    sprintf( buf4, "{cOriginal {rDAMAGE:{w You do {y%d{w to %s.{w", dam, victim->name );
    sprintf( buf5, "{cOriginal {rDAMAGE: {w%s does {y%d{w damage to you.{w", ch->name, dam );

    if (ch == victim)
    {
	act(buf1,ch,NULL,NULL,TO_ROOM);
	act(buf2,ch,NULL,NULL,TO_CHAR);
	if (IS_HERO(ch))
	{
		act(buf4,ch,NULL,NULL,TO_CHAR);
	}
    }
    else
    {
	act( buf1, ch, NULL, victim, TO_NOTVICT );
	act( buf2, ch, NULL, victim, TO_CHAR    );
	act( buf3, ch, NULL, victim, TO_VICT    );
	if (IS_HERO(ch))
	{
	   act( buf4, ch, NULL, victim, TO_CHAR   );
	}
	if (IS_IMMORTAL(victim))
	{
	   act( buf5, ch, NULL, victim, TO_VICT );
	}
    }

    return;
}




/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */

void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    bool weap1, weap2;

    if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
	weap1 = FALSE;
    else
	weap1 = TRUE;

    if ( get_eq_char( victim, WEAR_SECONDARY ) == NULL )
	weap2 = FALSE;
    else
	weap2 = TRUE;

    if ( !weap1 && !weap2 )
	return;

    if ( weap2  )  /*  Grayson Disarm Dual Wield 12 April 2003 */
	obj = get_eq_char( victim, WEAR_SECONDARY );
    else
	obj = get_eq_char( victim, WEAR_WIELD );

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE))
    {
	act("{g$S{g weapon won't budge!{w",ch,NULL,victim,TO_CHAR);
	act("{g$n{g tries to disarm you, but your weapon won't budge!{w",
	    ch,NULL,victim,TO_VICT);
	act("{g$n{g tries to disarm $N{g, but fails.{w",ch,NULL,victim,TO_NOTVICT);
	return;
    }

    act( "{c$n {rDISARMS{c you and sends your weapon flying!{w",
	 ch, NULL, victim, TO_VICT    );
    act( "{cYou {rDISARM{c $N{c!{w",  ch, NULL, victim, TO_CHAR    );
    act( "{c$n disarms $N{c!{w",  ch, NULL, victim, TO_NOTVICT );



    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY) )
	obj_to_char( obj, victim );
    else
    {
	obj_to_room( obj, victim->in_room );
	if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
	    get_obj(victim,obj,NULL);
    }

    return;
}



void do_berserk( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill(ch,gsn_berserk)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch) && !can_use_skill(ch, gsn_berserk)))
    {
	send_to_char("You turn red in the face, but nothing happens.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    ||  is_affected(ch,skill_lookup("frenzy")))
    {
	send_to_char("You get a little madder.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
	send_to_char("You're feeling too mellow to berserk.\n\r",ch);
	return;
    }

    if (ch->mana < 50)
    {
	send_to_char("You can't get up enough energy.\n\r",ch);
	return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
	chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 50;
	ch->move /= 2;

	/* heal a little damage */
	ch->hit += ch->level * 2;
	ch->hit = UMIN(ch->hit,ch->max_hit);

	send_to_char("Your pulse races as you are consumed by rage!\n\r",ch);
	act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_berserk,TRUE,2);

	af.where        = TO_AFFECTS;
	af.type         = gsn_berserk;
	af.level        = ch->level;
	af.duration     = number_fuzzy(ch->level / 8);
	af.modifier     = UMAX(1,ch->level/4);
	af.bitvector    = AFF_BERSERK;

	af.location     = APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location     = APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier     = UMAX(10,10 * (ch->level/4));
	af.location     = APPLY_AC;
	affect_to_char(ch,&af);
    }

    else

    {

	WAIT_STATE(ch,3 * PULSE_VIOLENCE);

	ch->mana -= 25;

	ch->move /= 2;



	send_to_char("Your pulse speeds up, but nothing happens.\n\r",ch);

	check_improve(ch,gsn_berserk,FALSE,2);

    }

}



void do_fury( CHAR_DATA *ch, char *argument)

{

    int chance, hp_percent;



    if ((chance = get_skill(ch,gsn_fury)) == 0

    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_FURY))

    ||  (!IS_NPC(ch) && !can_use_skill(ch, gsn_fury)))

    {

	send_to_char("You aren't angry enough.\n\r",ch);

	return;

    }
//don't stack fury and destruction

	 if ((IS_AFFECTED(ch,AFF_FURY)) || is_affected(ch,gsn_fury)

    ||  is_affected(ch,skill_lookup("frenzy")) || is_affected(ch,gsn_destruction)
    || (IS_AFFECTED(ch,AFF_DESTRUCTION)))

    {

	send_to_char("You become more furious.\n\r",ch);

	return;

    }



    if (IS_AFFECTED(ch,AFF_CALM))

    {

	send_to_char("You're feeling too mellow to get angry.\n\r",ch);

	return;

    }



    if (ch->mana < 250)

    {

	send_to_char("You can't get up enough energy.\n\r",ch);

	return;

    }



    /* modifiers */



    /* fighting */

    if (ch->position == POS_FIGHTING)

	chance += 10;



    /* damage -- below 50% of hp helps, above hurts */

    hp_percent = 100 * ch->hit/ch->max_hit;

    chance += 25 - hp_percent/2;



    if (number_percent() < chance)

    {

	AFFECT_DATA af;



	WAIT_STATE(ch,PULSE_VIOLENCE);

	ch->mana -= 150;

	ch->move /= 2;



	/* heal a little damage */

	ch->hit += ch->level * 3;

	ch->hit = UMIN(ch->hit,ch->max_hit);

	if IS_HERO(ch)

		ch->hit = ch->max_hit;



	send_to_char("You become so furious you blood boils and your eyes glow!\n\r",ch);

	act("$n growls with anger and $s eyes glow {rred{w.",ch,NULL,NULL,TO_ROOM);

	check_improve(ch,gsn_fury,TRUE,2);



	af.where        = TO_AFFECTS;

	af.type         = gsn_fury;

	af.level        = ch->level;

	af.duration     = number_fuzzy(ch->level / 10);



	if (ch->level < 100)

		af.modifier     = UMAX(1,ch->level/2);

	else if ((ch->level >= 100) && (ch->level < 200))

		af.modifier     = UMAX(1,ch->level);

	else if ((ch->level >= 200) && (ch->level < 300))

		af.modifier     = UMAX(1,(ch->level*3)/2);

	else if ((ch->level >= 300) && (ch->level < 500))

		af.modifier     = UMAX(1,ch->level*2);

	else

		af.modifier     = 5000;



	af.bitvector    = AFF_FURY;



	af.location     = APPLY_HITROLL;

	affect_to_char(ch,&af);



	af.location     = APPLY_DAMROLL;

	affect_to_char(ch,&af);



    }



    else

    {

	WAIT_STATE(ch,4 * PULSE_VIOLENCE);

	ch->mana -= 100;

	ch->move /= 2;



	send_to_char("You start to get angry but begin to calm down.\n\r",ch);

	check_improve(ch,gsn_fury,FALSE,2);

    }

}





void do_destruction( CHAR_DATA *ch, char *argument)

{

    int chance, hp_percent;



    if ((chance = get_skill(ch,gsn_destruction)) == 0

    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DESTRUCTION))

    ||  (!IS_NPC(ch) && !can_use_skill(ch, gsn_destruction)))

    {

	send_to_char("{g<CCP>:DESTRUCTION PROGRAM ALREADY INITIALIZED.{w\n\r",ch);

	return;

    }



    if (IS_AFFECTED(ch,AFF_DESTRUCTION) || is_affected(ch,gsn_destruction)

    ||  is_affected(ch,skill_lookup("frenzy"))||(IS_AFFECTED(ch,AFF_FURY))
    || is_affected(ch,gsn_fury))

    {

	send_to_char("{g<CCP>:PROCESSOR OVERLOAD!{w\n\r",ch);

	return;

    }



    if (IS_AFFECTED(ch,AFF_CALM))

    {

	send_to_char("{g<CCP>:UNABLE TO LOAD PROGRAM.{w\n\r",ch);

	return;

    }



    if (ch->mana < 250)

    {

	send_to_char("{g<CCP>:SYSTEM MEMORY UNAVAILABLE.{w\n\r",ch);

	return;

    }



    /* modifiers */



    /* fighting */

    if (ch->position == POS_FIGHTING)

	chance += 10;



    /* damage -- below 50% of hp helps, above hurts */

    hp_percent = 100 * ch->hit/ch->max_hit;

    chance += 25 - hp_percent/2;



    if (number_percent() < chance)

    {

	AFFECT_DATA af;



	WAIT_STATE(ch,PULSE_VIOLENCE);

	ch->mana -= 150;

	ch->move /= 2;



	/* heal a little damage */

	ch->hit += ch->level * 3;

	ch->hit = UMIN(ch->hit,ch->max_hit);

	if IS_HERO(ch)

		ch->hit = ch->max_hit;



	send_to_char("{g<CCP>:DESTRUCTION PROGRAM INITIALIZED SYSTEM VOLTAGE OVERLOAD!{w\n\r",ch);

	act("$n becomes surrounded by lightning and $s eyes glow {ggreen{w.",ch,NULL,NULL,TO_ROOM);

	check_improve(ch,gsn_berserk,TRUE,2);



	af.where        = TO_AFFECTS;

	af.type         = gsn_destruction;

	af.level        = ch->level;

	af.duration     = number_fuzzy(ch->level / 10);



	if (ch->level < 100)

		af.modifier     = UMAX(1,ch->level/2);

	else if ((ch->level >= 100) && (ch->level < 200))

		af.modifier     = UMAX(1,ch->level);

	else if ((ch->level >= 200) && (ch->level < 300))

		af.modifier     = UMAX(1,(ch->level*3)/2);

	else if ((ch->level >= 300) && (ch->level < 500))

		af.modifier     = UMAX(1,ch->level*2);

	else

		af.modifier     = 5000;



	af.bitvector    = AFF_DESTRUCTION;



	af.location     = APPLY_HITROLL;

	affect_to_char(ch,&af);



	af.location     = APPLY_DAMROLL;

	affect_to_char(ch,&af);



    }



    else

    {

	WAIT_STATE(ch,4 * PULSE_VIOLENCE);

	ch->mana -= 100;

	ch->move /= 2;



	send_to_char("{g<CCP>:DESTRUCTION PROGRAM HALTED.{w\n\r",ch);

	check_improve(ch,gsn_destruction,FALSE,2);

    }

}







void do_bash( CHAR_DATA *ch, char *argument )

{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    int hitchance;
    int dam;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bash)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||   (!IS_NPC(ch) && !can_use_skill(ch, gsn_bash)))
    {
	send_to_char("Bashing? What's that?\n\r",ch);
	return;
    }

    if ( ch->fighting == NULL )
	chance /= 2;

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You try to bash your brains out, but fail.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( IS_NPC(victim) &&
	victim->fighting != NULL &&
	!is_same_group(ch,victim->fighting))
    {
	send_to_char("Kill stealing is not permitted.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 200;
    chance -= victim->carry_weight / 250;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 20;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= (get_curr_stat(victim,STAT_DEX) * 4)/3;
    chance -= GET_AC(victim,AC_BASH) /25;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance -= 25;

    /* level */
    if ( ch->level > (victim->level + 50) )
	chance += (ch->level - victim->level);
    if ( victim->level > (ch->level + 50) )
	chance -= (victim->level - ch->level);

    /* Are they Flying? */
    if (IS_AFFECTED(victim,AFF_FLYING))
	chance -= 50;

    if (!IS_NPC(victim)
	&& chance < get_skill(victim,gsn_dodge) )
    	chance -= 3 * (get_skill(victim,gsn_dodge) - chance);

    switch(check_immune(victim,DAM_BASH))
    {
	case(IS_IMMUNE):
	    chance = 0;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 3/2;
	    break;
    }
	hitchance = number_percent();

    /* now the attack */
    if (hitchance < chance )
    {

	if (IS_AFFECTED(victim,AFF_FLYING))
	{
	    	act("{y$n{y knocks you out of the sky with a powerful bash!{w",
			ch,NULL,victim,TO_VICT);
		act("{yYou slam into $N{y, and knock $M{y out of the sky!{w",ch,NULL,victim,TO_CHAR);
		act("{y$n{y knocks $N{y out of the sky with a powerful bash.{w",
			ch,NULL,victim,TO_NOTVICT);
		check_improve(ch,gsn_bash,TRUE,1);
		affect_strip(victim,skill_lookup("fly"));
		WAIT_STATE(victim, (get_combat_value("lag_bash_victim")*PULSE_VIOLENCE) );
		WAIT_STATE(ch, (skill_table[gsn_bash].beats*get_combat_value("lag_bash_ch")) );
		dam = number_range(ch->level*4, ch->level*12);
		dam = dam * get_combat_value("m_damage_bash");
		dam = dam / get_combat_value("d_damage_bash");
		damage(ch,victim,dam,gsn_bash,DAM_BASH,FALSE, FALSE);/* Grayson -- Parryable 4 May 2003 */
	      stop_fighting( victim, TRUE );
		victim->position = POS_STANDING;
	}
	else
	{
	    	act("{y$n{y sends you sprawling with a powerful bash!{w",
			ch,NULL,victim,TO_VICT);
		act("{yYou slam into $N{y, and send $M{y flying!{w",ch,NULL,victim,TO_CHAR);
		act("{y$n{y sends $N{y sprawling with a powerful bash.{w",
			ch,NULL,victim,TO_NOTVICT);
		check_improve(ch,gsn_bash,TRUE,1);
		WAIT_STATE(victim, (get_combat_value("lag_bash_victim")*PULSE_VIOLENCE) );
		WAIT_STATE(ch, (skill_table[gsn_bash].beats*get_combat_value("lag_bash_ch")) );
		dam = number_range(ch->level, ch->level*4);
		dam = dam * get_combat_value("m_damage_bash");
		dam = dam / get_combat_value("d_damage_bash");
		damage(ch,victim,dam,gsn_bash,DAM_BASH,FALSE, FALSE); /* Grayson -- Parryable 4 May 2003 */
	      stop_fighting( victim, TRUE );
		victim->position = POS_RESTING;
	}
    }
    else
    {
	damage(ch,victim,0,gsn_bash,DAM_BASH,FALSE, FALSE); /* Grayson -- Parryable 4 May 2003 */
	act("You fall flat on your face!",
	    ch,NULL,victim,TO_CHAR);
	act("$n falls flat on $s face.",
	    ch,NULL,victim,TO_NOTVICT);
	act("You evade $n's bash, causing $m to fall flat on $s face.",
	    ch,NULL,victim,TO_VICT);
	if (IS_AFFECTED(victim,AFF_FLYING))
		damage(ch,ch,number_range(ch->level, ch->level*4),gsn_bash,DAM_BASH,FALSE, FALSE); /* Grayson -- Parryable 4 May 2003 */
	check_improve(ch,gsn_bash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch, ((skill_table[gsn_bash].beats*get_combat_value("lag_bash_ch"))/2) );
    }
	check_killer(ch,victim);
	/*  // Working on Stand-up code
	if (IS_NPC(victim) && victim->position==POS_RESTING)
	{
		victim->position = POS_STANDING;
		act( "$n stands up.", victim, NULL, NULL, TO_ROOM);

	}*/
	return;
}

void do_dirt( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    int chance;



    one_argument(argument,arg);



    if ( (chance = get_skill(ch,gsn_dirt)) == 0

    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK_DIRT))

    ||   (!IS_NPC(ch) && !can_use_skill(ch, gsn_dirt)))

    {

	send_to_char("You get your feet dirty.\n\r",ch);

	return;

    }



    if (arg[0] == '\0')

    {

	victim = ch->fighting;

	if (victim == NULL)

	{

	    send_to_char("But you aren't in combat!\n\r",ch);

	    return;

	}

    }



    else if ((victim = get_char_room(ch,arg)) == NULL)

    {

	send_to_char("They aren't here.\n\r",ch);

	return;

    }



    if (IS_AFFECTED(victim,AFF_BLIND))

    {

	act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);

	return;

    }



    if (victim == ch)

    {

	send_to_char("Very funny.\n\r",ch);

	return;

    }



    if (is_safe(ch,victim))

	return;



    if (IS_NPC(victim) &&

	 victim->fighting != NULL &&

	!is_same_group(ch,victim->fighting))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)

    {

	act("But $N is such a good friend!",ch,NULL,victim,TO_CHAR);

	return;

    }



    /* modifiers */



    /* dexterity */

    chance += get_curr_stat(ch,STAT_DEX);

    chance -= 2 * get_curr_stat(victim,STAT_DEX);



    /* speed  */

    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))

	chance += 10;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))

	chance -= 25;



    /* sloppy hack to prevent false zeroes */

    if (chance % 5 == 0)

	chance += 1;



    /* terrain */



    switch(ch->in_room->sector_type)

    {

	case(SECT_INSIDE):              chance -= 20;   break;

	case(SECT_CITY):                chance -= 10;   break;

	case(SECT_FIELD):               chance +=  5;   break;

	case(SECT_FOREST):                              break;

	case(SECT_HILLS):                               break;

	case(SECT_MOUNTAIN):            chance -= 10;   break;

	case(SECT_WATER_SWIM):          chance  =  0;   break;

	case(SECT_WATER_NOSWIM):        chance  =  0;   break;

	case(SECT_AIR):                 chance  =  0;   break;

	case(SECT_DESERT):              chance += 10;   break;

    }



    if (chance == 0)

    {

	send_to_char("There isn't any dirt to kick.\n\r",ch);

	return;

    }



    /* now the attack */

    if (number_percent() < chance)

    {

	AFFECT_DATA af;

	act("{Y$n{Y is blinded by the dirt in $s{Y eyes!{w",victim,NULL,NULL,TO_ROOM);

	act("{Y$n{Y kicks dirt in your eyes!{w",ch,NULL,victim,TO_VICT);

	damage(ch,victim,number_range(2,5),gsn_dirt,DAM_NONE,FALSE, FALSE); /* Grayson -- Parryable 4 May 2003 */

	send_to_char("You can't see a thing!\n\r",victim);

	check_improve(ch,gsn_dirt,TRUE,2);

	WAIT_STATE(ch,skill_table[gsn_dirt].beats);



	af.where        = TO_AFFECTS;

	af.type         = gsn_dirt;

	af.level        = ch->level;

	af.duration     = 0;

	af.location     = APPLY_HITROLL;

	af.modifier     = -4;

	af.bitvector    = AFF_BLIND;



	affect_to_char(victim,&af);

    }

    else

    {

	damage(ch,victim,0,gsn_dirt,DAM_NONE,TRUE, FALSE);/* Grayson -- Parryable 4 May 2003 */

	check_improve(ch,gsn_dirt,FALSE,2);

	WAIT_STATE(ch,skill_table[gsn_dirt].beats);

    }

	check_killer(ch,victim);

	return;

}



void do_trip( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    int chance;

    int dam;



    one_argument(argument,arg);



    if ( (chance = get_skill(ch,gsn_trip)) == 0

    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP))

    ||   (!IS_NPC(ch) && !can_use_skill(ch, gsn_trip)))

    {

	send_to_char("Tripping?  What's that?\n\r",ch);

	return;

    }





    if (arg[0] == '\0')

    {

	victim = ch->fighting;

	if (victim == NULL)

	{

	    send_to_char("But you aren't fighting anyone!\n\r",ch);

	    return;

	}

    }



    else if ((victim = get_char_room(ch,arg)) == NULL)

    {

	send_to_char("They aren't here.\n\r",ch);

	return;

    }



    if (is_safe(ch,victim))

	return;



    if (IS_NPC(victim) &&

	 victim->fighting != NULL &&

	!is_same_group(ch,victim->fighting))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    if (IS_AFFECTED(victim,AFF_FLYING))

    {

	act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);

	return;

    }



    if (victim->position < POS_FIGHTING)

    {

	act("$N is already down.",ch,NULL,victim,TO_CHAR);

	return;

    }



    if (victim == ch)

    {

	send_to_char("{GYou fall flat on your face!{w\n\r",ch);

	WAIT_STATE(ch,skill_table[gsn_trip].beats*get_combat_value("lag_trip_ch"));

	act("{G$n{G trips over $s{G own feet!{w",ch,NULL,NULL,TO_ROOM);

	return;

    }



    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)

    {

	act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);

	return;

    }



    /* modifiers */



    /* size */

    if (ch->size < victim->size)

	chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */



    /* dex */

    chance += get_curr_stat(ch,STAT_DEX);

    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;



    /* speed */

    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))

	chance += 10;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))

	chance -= 20;



    /* now the attack */

    if (number_percent() < chance)

    {

	act("{G$n{G trips you and you go down!{w",ch,NULL,victim,TO_VICT);

	act("{GYou trip $N{G and $N{G goes down!{w",ch,NULL,victim,TO_CHAR);

	act("{G$n{G trips $N{G, sending $M{G to the ground.{w",ch,NULL,victim,TO_NOTVICT);

	check_improve(ch,gsn_trip,TRUE,1);



	DAZE_STATE(victim,get_combat_value("lag_trip_victim") * PULSE_VIOLENCE);

	WAIT_STATE(ch,skill_table[gsn_trip].beats*get_combat_value("lag_trip_ch"));

	victim->position = POS_RESTING;

	dam = number_range(2, 2 +  2 * victim->size);

	dam = dam * get_combat_value("m_damage_trip");

	dam = dam / get_combat_value("d_damage_trip");

	damage(ch,victim,dam,gsn_trip,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */

    }

    else

    {

	damage(ch,victim,0,gsn_trip,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */

	WAIT_STATE(ch,skill_table[gsn_trip].beats*get_combat_value("lag_trip_ch")*2/3);

	check_improve(ch,gsn_trip,FALSE,1);

    }

	check_killer(ch,victim);

	return;

}







void do_kill( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Kill whom?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_room( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( !IS_NPC(ch) && !IS_NPC(victim) && !IS_IMMORTAL(ch) )

    {

       send_to_char( "You must MURDER a player.\n\r", ch );

       return;

    }



    if ( victim == ch )

    {

	send_to_char( "{bYou hit yourself.  Ouch!{w\n\r", ch );

	multi_hit( ch, ch, TYPE_UNDEFINED );

	return;

    }



    if ( is_safe( ch, victim ) )

	return;



    if ( victim->fighting != NULL &&

	!is_same_group(ch,victim->fighting))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )

    {

	act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );

	return;

    }



    if ( ch->position == POS_FIGHTING )

    {

	send_to_char( "You do the best you can!\n\r", ch );

	return;

    }



    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );

    check_killer( ch, victim );

    multi_hit( ch, victim, TYPE_UNDEFINED );

    return;

}







void do_murde( CHAR_DATA *ch, char *argument )

{

    send_to_char( "If you want to MURDER, spell it out.\n\r", ch );

    return;

}







void do_murder( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Murder whom?\n\r", ch );

	return;

    }



    if (IS_AFFECTED(ch,AFF_CHARM) || (IS_NPC(ch) && IS_SET(ch->act,ACT_PET)))

    {

	send_to_char("Tell your master to do it themself.\n\r", ch);

	return;

    }



    if ( ( victim = get_char_room( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( victim == ch )

    {

	send_to_char( "Suicide is a mortal sin.\n\r", ch );

	return;

    }



    if ( is_safe( ch, victim ) )

	return;



    if (IS_NPC(victim))

    {

	send_to_char( "You must KILL a mobile.\n\r", ch );

	return;

    }



    if (IS_NPC(victim) &&

	 victim->fighting != NULL &&

	!is_same_group(ch,victim->fighting))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )

    {

	act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );

	return;

    }



    if ( ch->position == POS_FIGHTING )

    {

	send_to_char( "You do the best you can!\n\r", ch );

	return;

    }



    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );

    check_killer( ch, victim );

    multi_hit( ch, victim, TYPE_UNDEFINED );

    return;

}







void do_backstab( CHAR_DATA *ch, char *argument )

{
    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if (arg[0] == '\0')
    {
	send_to_char("Backstab whom?\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("You're facing the wrong end.\n\r",ch);
	return;
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if (IS_NPC(victim) &&
	 victim->fighting != NULL &&
	!is_same_group(ch,victim->fighting))
    {
	send_to_char("Kill stealing is not permitted.\n\r",ch);
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "You need to wield a weapon to backstab.\n\r", ch );
	return;
    }

    if ( victim->hit < victim->max_hit / 3)
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if (IS_NPC(victim) && (victim->hit < victim->max_hit * 6/10 ))
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if ( number_percent( ) < get_skill(ch,gsn_backstab)
    || ( get_skill(ch,gsn_backstab) >= 2 && !IS_AWAKE(victim) ) )
    {
	check_improve(ch,gsn_backstab,TRUE,1);
	multi_hit( ch, victim, gsn_backstab );
    }
    else
    {
	check_improve(ch,gsn_backstab,FALSE,1);
	damage( ch, victim, 0, gsn_backstab,DAM_NONE,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
    }
    return;

}



void do_circlestab( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    OBJ_DATA *obj;

    int chance;

    int hitchance;



    chance = get_skill(ch,gsn_backstab);



    if ( chance < 75 )

    {

	send_to_char("You are not skilled enough to Circle and Backstab ANYONE!\n\r", ch);

	return;

    }



    if (ch->fighting == NULL)

    {

	send_to_char("You are not fighting anyone!\n\r",ch);

	return;

    }



    if ((victim = ch->fighting) == NULL)

    {

	send_to_char("You are not fighting anyone!\n\r",ch);

	return;

    }



    if ( victim == ch )

    {

	send_to_char( "How can you circle around and backstab yourself?\n\r", ch );

	return;

    }



    if ( is_safe( ch, victim ) )

      return;



    if (IS_NPC(victim) &&

	 victim->fighting != NULL &&

	!is_same_group(ch,victim->fighting))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)

    {

	send_to_char( "You need to wield a weapon to backstab.\n\r", ch );

	return;

    }



    chance /= 2;



    if (ch->size < victim->size)

	chance += 15;



    chance -= get_curr_stat(victim,STAT_DEX)/2;

    chance += get_curr_stat(victim,STAT_DEX)/2;



    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))

	chance += 15;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))

	chance -= 15;



    /* level */

    if ( ch->level > (victim->level + 50) )

	chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )

	chance -= (victim->level - ch->level);



	hitchance = number_percent( );



    check_killer( ch, victim );

    WAIT_STATE( ch, skill_table[gsn_backstab].beats );

    if ( (hitchance < chance)

    || !IS_AWAKE(victim) )

    {

	check_improve(ch,gsn_backstab,TRUE,1);

	multi_hit( ch, victim, gsn_backstab );

	if (IS_AFFECTED(victim,AFF_FLYING))

	{

		affect_strip(victim,skill_lookup("fly"));

	}

    }

    else

    {

	check_improve(ch,gsn_backstab,FALSE,1);

	damage( ch, victim, 0, gsn_backstab,DAM_NONE,TRUE, FALSE);/* Grayson -- Parryable 4 May 2003 */

    }



    return;

}



void do_flee( CHAR_DATA *ch, char *argument )

{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;

    if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ch->position == POS_FIGHTING )
	    ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    was_in = ch->in_room;
    for ( attempt = 0; attempt < 6; attempt++ )
    {
	EXIT_DATA *pexit;
	int door;

	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->u1.to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	||   number_range(0,ch->daze) != 0
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	    continue;

	move_char( ch, door, FALSE );
	if ( ( now_in = ch->in_room ) == was_in )
	    continue;

	ch->in_room = was_in;
	act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;

	if ( !IS_NPC(ch) )
	{
	    send_to_char( "You flee from combat!\n\r", ch );
	if( (ch->class == 2)
	    && (number_percent() < 3*(ch->level/2) ) )
		send_to_char( "You snuck away safely.\n\r", ch);
	else
	    {
	    send_to_char( "You lost 10 exp.\n\r", ch);
	    gain_exp( ch, -10 );
	    }
	}

	stop_fighting( ch, TRUE );
	return;
    }

    send_to_char( "PANIC! You couldn't escape!\n\r", ch );
    return;
}


void do_rescue( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    CHAR_DATA *fch;



    one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	send_to_char( "Rescue whom?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_room( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( victim == ch )

    {

	send_to_char( "What about fleeing instead?\n\r", ch );

	return;

    }



    if ( !IS_NPC(ch) && IS_NPC(victim) )

    {

	send_to_char( "Doesn't need your help!\n\r", ch );

	return;

    }



    if ( ch->fighting == victim )

    {

	send_to_char( "Too late.\n\r", ch );

	return;

    }



    if ( ( fch = victim->fighting ) == NULL )

    {

	send_to_char( "That person is not fighting right now.\n\r", ch );

	return;

    }



    if ( IS_NPC(fch) && !is_same_group(ch,victim))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    WAIT_STATE( ch, skill_table[gsn_rescue].beats );

    if ( number_percent( ) > get_skill(ch,gsn_rescue))

    {

	send_to_char( "You fail the rescue.\n\r", ch );

	check_improve(ch,gsn_rescue,FALSE,1);

	return;

    }



    act( "{yYou rescue $N{y!{w",  ch, NULL, victim, TO_CHAR    );

    act( "{y$n{y rescues you!{w", ch, NULL, victim, TO_VICT    );

    act( "{y$n{y rescues $N{y!{w",  ch, NULL, victim, TO_NOTVICT );

    check_improve(ch,gsn_rescue,TRUE,1);



    stop_fighting( fch, FALSE );

    stop_fighting( victim, FALSE );



    check_killer( ch, fch );

    set_fighting( ch, fch );

    set_fighting( fch, ch );

    return;

}





void do_spin( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;
    int chance;
    int hitchance;
    char arg[MAX_INPUT_LENGTH];
    int dam;
   	int new_ac;
	

    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_spinkick) )
    {
	send_to_char("You'll just get dizzy if you try to spin kick.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_SPINKICK))
	return;

    if ( IS_IMMORTAL(ch) )
    {
		one_argument(argument,arg);
		if (arg[0] == '\0')
		{
			victim = ch->fighting;
			if (victim == NULL)
			{
			    send_to_char("But you aren't fighting anyone!\n\r",ch);
			    return;
			}
		}
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r",ch);
			return;
		}
		if (victim->position < POS_FIGHTING)
		{
			act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
			return;
		}
		if (victim == ch)
		{
			send_to_char("You spin in circles.\n\r",ch);
			return;
		}
		if (is_safe(ch,victim))
			return;
		if (  IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(ch,victim->fighting))
		{
			send_to_char("Kill stealing is not permitted.\n\r",ch);
			return;
		}
		if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
		{
			act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
			return;
		}
    }
    else
    {
		if ( (( victim = ch->fighting ) == NULL) )
		{
			send_to_char( "It is not honourable to kick without warning.\n\r", ch );
			return;
		}
    }

    chance = get_skill(ch,gsn_spinkick);
    hitchance = number_percent();

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 200;
    chance -= victim->carry_weight / 250;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX)*2;
    chance += get_curr_stat(ch,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 15;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 25;

    /* Are they Flying? */

    if (IS_AFFECTED(victim,AFF_FLYING))
		chance -= 50;

    /* level */
    if ( ch->level > (victim->level + 50) )
		chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )
		chance -= (victim->level - ch->level);

    switch(check_immune(victim,DAM_BASH))
    {
	case(IS_IMMUNE):
	    chance /= 4;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 3/2;
	    break;
    }
	new_ac = GET_AC(victim,AC_BASH);        
    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac");

    WAIT_STATE( ch, skill_table[gsn_spinkick].beats*get_combat_value("lag_spinkick_ch") );

    if ( chance > hitchance)
    {
		if (IS_AFFECTED(victim,AFF_FLYING))
		{
			act("$n spins fluidly and smashes their heel into $N, knocking them from the sky.",ch,NULL,victim,TO_NOTVICT);
	    	act("$n spins smashes you with their heel, knocking you from the sky.",ch,NULL,victim,TO_VICT);
	    	act("You smash your heel into $N, knocking them from the sky.",ch,NULL,victim,TO_CHAR);
			if(ch->level<=100)
			{
				dam = number_range( ch->level/2, ch->level*3/2 );
				dam = dam * get_combat_value("m_damage_spinkick");
				dam = dam / get_combat_value("d_damage_spinkick");
			}
			else
			{
				dam = number_range( ch->level*8, ch->level*16 );
				dam = dam * get_combat_value("m_damage_spinkick");
				dam = dam / get_combat_value("d_damage_spinkick");
			}
   			if (ch->level > 50 && victim->level > 50)
			{
			   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
			}	
			damage(ch,victim,dam,gsn_spinkick,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_spinkick,TRUE,1);
			affect_strip(victim,skill_lookup("fly"));
			WAIT_STATE(victim,skill_table[gsn_spinkick].beats*get_combat_value("lag_spinkick_victim"));
	        stop_fighting( victim, TRUE );
			victim->position = POS_STANDING;
		}
		else
		{
			act("$n spins fluidly and smashes their heel into $N.",ch,NULL,victim,TO_NOTVICT);
	    	act("$n spins smashes you with their heel.",ch,NULL,victim,TO_VICT);
	    	act("You smash your heel into $N.",ch,NULL,victim,TO_CHAR);
			if(ch->level<=100)
			{
				dam = number_range( ch->level/2, ch->level );
				dam = dam * get_combat_value("m_damage_spinkick");
				dam = dam / get_combat_value("d_damage_spinkick");
			}
			else
			{
				dam = number_range( ch->level*2, ch->level*8 );
				dam = dam * get_combat_value("m_damage_spinkick");
				dam = dam / get_combat_value("d_damage_spinkick");
			}
   			if (ch->level > 50 && victim->level > 50)
			{
			   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
			}	
			damage(ch,victim,dam,gsn_spinkick,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_spinkick,TRUE,1);
			WAIT_STATE(victim, get_combat_value("lag_spinkick_victim") * PULSE_VIOLENCE);
            stop_fighting( victim, TRUE );
			victim->position = POS_RESTING;
		}
    }
    else
    {
		damage( ch, victim, 0, gsn_spinkick,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_spinkick,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}


void do_buffet( CHAR_DATA *ch, char *argument )
{

    CHAR_DATA *victim;
    int chance;
    int hitchance;
    char arg[MAX_INPUT_LENGTH];
    int dam;
	int new_ac;
	bool hit;
	

    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_buffet) )
    {
	send_to_char("You don't have wings.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BUFFET))
	return;

    if ( IS_IMMORTAL(ch) )
    {
		one_argument(argument,arg);
		if (arg[0] == '\0')
		{
			victim = ch->fighting;
			if (victim == NULL)
			{
			    send_to_char("But you aren't fighting anyone!\n\r",ch);
			    return;
			}
		}
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r",ch);
			return;
		}
		if (victim->position < POS_FIGHTING)
		{
			act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
			return;
		}
		if (victim == ch)
		{
			send_to_char("You spin in circles.\n\r",ch);
			return;
		}
		if (is_safe(ch,victim))
			return;
		if (  IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(ch,victim->fighting))
		{
			send_to_char("Kill stealing is not permitted.\n\r",ch);
			return;
		}
		if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
		{
			act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
			return;
		}
    }
    else
    {
		if ( (( victim = ch->fighting ) == NULL) )
		{
			send_to_char( "You can't just sneak up on someone and flap your wings at them.\n\r", ch );
			return;
		}
    }

    chance = get_skill(ch,gsn_buffet);
    hitchance = number_percent();

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 200;
    chance -= victim->carry_weight / 250;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX)*2;
    chance += get_curr_stat(ch,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 15;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 25;

    /* Are they Flying? */

    if (IS_AFFECTED(victim,AFF_FLYING))
		chance += 15;

    /* level */
    if ( ch->level > (victim->level + 50) )
		chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )
		chance -= (victim->level - ch->level);

    switch(check_immune(victim,DAM_BASH))
    {
	case(IS_IMMUNE):
	    chance /= 4;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 3/2;
	    break;
    }
   	new_ac = GET_AC(victim,AC_BASH);        
    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac");

    WAIT_STATE( ch, skill_table[gsn_buffet].beats*get_combat_value("lag_buffet_ch") );

    if ( chance > hitchance)
    {
		if (IS_AFFECTED(victim,AFF_FLYING))
		{
			if(ch->level<100)
			{
				dam = number_range(ch->level/2, ch->level*3/2);
				dam = dam * get_combat_value("m_damage_buffet");
				dam = dam / get_combat_value("d_damage_buffet");
			}
			else
			{
				dam = number_range( ch->level*6, ch->level*12 );
				dam = dam * get_combat_value("m_damage_buffet");
				dam = dam / get_combat_value("d_damage_buffet");
			}
   			if (ch->level > 50 && victim->level > 50)
			{
			   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
			}			
			hit = damage(ch,victim,dam,gsn_buffet,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_buffet,TRUE,1);
			affect_strip(victim,skill_lookup("fly"));
			WAIT_STATE(victim,skill_table[gsn_buffet].beats*get_combat_value("lag_buffet_victim"));
	        stop_fighting( victim, TRUE );
			victim->position = POS_STANDING;
			if(hit)
			{
				act("$n flaps $s wings, knocking $N from the sky with a gust of air.",ch,NULL,victim,TO_NOTVICT);
		    	act("$n flaps $s wings, knocking you from the sky with a gust of air.",ch,NULL,victim,TO_VICT);
		    	act("You flap your wings, knocking $N from the sky, with a gust of air",ch,NULL,victim,TO_CHAR);
			}
		}
		else
		{
			if (ch->level<100)
			{
				dam = number_range(ch->level/2, ch->level);
				dam = dam * get_combat_value("m_damage_buffet");
				dam = dam / get_combat_value("d_damage_buffet");
			}
			else
			{
				dam = number_range( ch->level*4, ch->level*10 );
				dam = dam * get_combat_value("m_damage_buffet");
				dam = dam / get_combat_value("d_damage_buffet");
			}
   			if (ch->level > 50 && victim->level > 50)
			{
			   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
			}	
			hit = damage(ch,victim,dam,gsn_buffet,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_buffet,TRUE,1);
			WAIT_STATE(victim, get_combat_value("lag_buffet_victim") * PULSE_VIOLENCE);
            stop_fighting( victim, TRUE );
			victim->position = POS_RESTING;
			if (hit)
			{
				act("$n flaps $s wings, pushing $N to the ground with a gust of air.",ch,NULL,victim,TO_NOTVICT);
		    	act("$n flaps $s wings, pushing you to the ground with a gust of air.",ch,NULL,victim,TO_VICT);
		    	act("You flap your wings, pushing $N to the ground.",ch,NULL,victim,TO_CHAR);
			}
		}
    }
    else
    {
		damage( ch, victim, 0, gsn_buffet,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_buffet,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}

void do_sweep( CHAR_DATA *ch, char *argument )
{

    CHAR_DATA *victim;
    int chance;
    int hitchance;
    char arg[MAX_INPUT_LENGTH];
    int dam;
   	int new_ac;
   	bool hit;
	
	
    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_sweep) )
    {
	send_to_char("You can't do that.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_SWEEP))
	return;

    if ( IS_IMMORTAL(ch) )
    {
		one_argument(argument,arg);
		if (arg[0] == '\0')
		{
			victim = ch->fighting;
			if (victim == NULL)
			{
			    send_to_char("But you aren't fighting anyone!\n\r",ch);
			    return;
			}
		}
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r",ch);
			return;
		}
		if (victim->position < POS_FIGHTING)
		{
			act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
			return;
		}
		if (victim == ch)
		{
			send_to_char("You fall on your ass.\n\r",ch);
			return;
		}
		if (is_safe(ch,victim))
			return;
		if (  IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(ch,victim->fighting))
		{
			send_to_char("Kill stealing is not permitted.\n\r",ch);
			return;
		}
		if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
		{
			act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
			return;
		}
    }
    else
    {
		if ( (( victim = ch->fighting ) == NULL) )
		{
			send_to_char( "That's not exactly fair, is it?\n\r", ch );
			return;
		}
    }

    chance = get_skill(ch,gsn_sweep);
    hitchance = number_percent();

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 200;
    chance -= victim->carry_weight / 250;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX)*2;
    chance += get_curr_stat(ch,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 15;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 25;

     /* level */
    if ( ch->level > (victim->level + 50) )
		chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )
		chance -= (victim->level - ch->level);

    switch(check_immune(victim,DAM_BASH))
    {
	case(IS_IMMUNE):
	    chance /= 4;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 3/2;
	    break;
    }
    
	new_ac = GET_AC(victim,AC_BASH);        
    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac");

    WAIT_STATE( ch, skill_table[gsn_sweep].beats*get_combat_value("lag_sweep_ch") );

    if ( chance > hitchance)
    {
		if (IS_AFFECTED(victim,AFF_FLYING))
		{
			act("$n attempts to knock $N's feet out from under $M, but $N is flying!",ch,NULL,victim,TO_NOTVICT);
	    	act("$n attempts to knock your feet out from under you.  Luckily, you are flying!",ch,NULL,victim,TO_VICT);
	    	act("You attempt to knock $N's feet out from under $M.  Unfortunately, $E is flying.",ch,NULL,victim,TO_CHAR);
		}
		else
		{
			if(ch->level < 100)
			{
				dam = number_range(ch->level*2/3, ch->level);
				dam = dam * get_combat_value("m_damage_sweep");
				dam = dam / get_combat_value("d_damage_sweep");
			}
			else
			{
				dam = number_range( ch->level*4, ch->level*10 );
				dam = dam * get_combat_value("m_damage_sweep");
				dam = dam / get_combat_value("d_damage_sweep");
			}
   			if (ch->level > 50 && victim->level > 50)
			{
			   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
			}	
			hit = damage(ch,victim,dam,gsn_sweep,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_sweep,TRUE,1);
			WAIT_STATE(victim, get_combat_value("lag_sweep_victim") * PULSE_VIOLENCE);
            stop_fighting( victim, TRUE );
			victim->position = POS_RESTING;
			if (hit)
			{
				if (class_lookup("druid") == ch->class )
				{
					act("$n whips $s tail around, knocking $N's legs out from under $M.",ch,NULL,victim,TO_NOTVICT);
		    		act("$n whips $s tail around, knocking your legs out from under you.",ch,NULL,victim,TO_VICT);
		    		act("You whip your tail around, and knock $N's legs out from under $M.",ch,NULL,victim,TO_CHAR);
				}
				else
				{
					act("$n knocks $N's legs out from under $M.",ch,NULL,victim,TO_NOTVICT);
		    		act("$n knocks legs out from under you.",ch,NULL,victim,TO_VICT);
		    		act("You lash out with your staff and knock $N's legs out from under $M.",ch,NULL,victim,TO_CHAR);
				}
			}
		}
    }
    else
    {
		damage( ch, victim, 0, gsn_sweep,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_sweep,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}

void do_poetry( CHAR_DATA *ch, char *argument )
{

    CHAR_DATA *victim;
    int chance;
    int hitchance;
    char arg[MAX_INPUT_LENGTH];
    int dam;
    int new_ac;
    

    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_poetry) )
    {
	send_to_char("You don't know anything about Vogon poetry.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_POETRY))
	return;

    if ( IS_IMMORTAL(ch) )
    {
		one_argument(argument,arg);
		if (arg[0] == '\0')
		{
			victim = ch->fighting;
			if (victim == NULL)
			{
			    send_to_char("But you aren't fighting anyone!\n\r",ch);
			    return;
			}
		}
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r",ch);
			return;
		}
		if (victim->position < POS_FIGHTING)
		{
			act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
			return;
		}
		if (victim == ch)
		{
			send_to_char("You enjoy your own poetry.\n\r",ch);
			return;
		}
		if (is_safe(ch,victim))
			return;
		if (  IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(ch,victim->fighting))
		{
			send_to_char("Kill stealing is not permitted.\n\r",ch);
			return;
		}
		if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
		{
			act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
			return;
		}
    }
    else
    {
		if ( (( victim = ch->fighting ) == NULL) )
		{
			send_to_char( "You want to recite Vogon poetry without warning? That's just too cruel...\n\r", ch );
			return;
		}
		if ( victim->race == (race_lookup("vogon")) )
		{
			act("{g$n says 'Oh freddled gruntbuggly, thy micturations are to me as plurdled gabbleblotchits on a lurgid bee.'{w",ch,NULL,victim,TO_NOTVICT);
    		act("$N sneers and applaud's $n's poetry sarcastically.",ch,NULL,victim,TO_NOTVICT);
			act("{g$n says 'Oh freddled gruntbuggly, thy micturations are to me as plurdled gabbleblotchits on a lurgid bee.'{w",ch,NULL,victim,TO_VICT);
			act("You sneer and applaud $n's poetry sarcastically.",ch,NULL,victim,TO_VICT);
    		act("{gYou say 'Oh freddled gruntbuggly, thy micturations are to me as plurdled gabbleblotchits on a lurgid bee.'{w",ch,NULL,victim,TO_CHAR);
			act("$N sneers and sarcastically applauds your poetry.",ch,NULL,victim,TO_CHAR);	
			return;
		}
    }

    chance = get_skill(ch,gsn_poetry);
    hitchance = number_percent();

    /* modifiers */

    /* size  and weight */
    //chance += ch->carry_weight / 200;
    //chance -= victim->carry_weight / 250;

    /* stats */
    chance += get_curr_stat(victim,STAT_INT);
    chance -= get_curr_stat(ch,STAT_INT)*2;
    //chance += get_curr_stat(ch,STAT_DEX);


    /* level */
    if ( ch->level > (victim->level + 50) )
		chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )
		chance -= (victim->level - ch->level);

    switch(check_immune(victim,DAM_OTHER))
    {
	case(IS_IMMUNE):
	    chance /= 4;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 3/2;
	    break;
    }
    
	new_ac = GET_AC(victim,AC_EXOTIC);      

    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac");

    WAIT_STATE( ch, skill_table[gsn_poetry].beats*get_combat_value("lag_poetry_ch") );

    if ( chance > hitchance)
    {
		switch(number_range(0,3))
		{
			case 0 :
				act("{g$n says 'Oh freddled gruntbuggly, thy micturations are to me as plurdled gabbleblotchits on a lurgid bee.'{w",ch,NULL,victim,TO_NOTVICT);
    			act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_NOTVICT);
				act("{g$n says 'Oh freddled gruntbuggly, thy micturations are to me as plurdled gabbleblotchits on a lurgid bee.'{w",ch,NULL,victim,TO_VICT);
				act("You grimace in pain, covering your ears with your hands.",ch,NULL,victim,TO_VICT);
    			act("{gYou say 'Oh freddled gruntbuggly, thy micturations are to me as plurdled gabbleblotchits on a lurgid bee.'{w",ch,NULL,victim,TO_CHAR);
				act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_CHAR);
				break;
			case 1 :
				act("{g$n says 'Groop I implore thee, my foonting turlingdromes.'{w",ch,NULL,victim,TO_NOTVICT);
    			act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_NOTVICT);
				act("{g$n says 'Groop I implore thee, my foonting turlingdromes.'{w",ch,NULL,victim,TO_VICT);
				act("You grimace in pain, covering your ears with your hands.",ch,NULL,victim,TO_VICT);
    			act("{gYou say 'Groop I implore thee, my foonting turlingdromes.'{w",ch,NULL,victim,TO_CHAR);
				act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_CHAR);
				break;
			case 2 :
				act("{g$n says 'And hooptiously drangle me with crinkly brindlewurdles'{w",ch,NULL,victim,TO_NOTVICT);
    			act("$N grimaces in pain, covering $S ears with $S hands.{w",ch,NULL,victim,TO_NOTVICT);
				act("{g$n says 'And hooptiously drangle me with crinkly brindlewurdles'{w",ch,NULL,victim,TO_VICT);
				act("You grimace in pain, covering your ears with your hands.",ch,NULL,victim,TO_VICT);
    			act("{gYou say 'And hooptiously drangle me with crinkly brindlewurdles'{w",ch,NULL,victim,TO_CHAR);
				act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_CHAR);
				break;
			case 3 :
				act("{g$n says 'Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!'{w",ch,NULL,victim,TO_NOTVICT);
    			act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_NOTVICT);
				act("{g$n says 'Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!'{w",ch,NULL,victim,TO_VICT);
				act("You grimace in pain, covering your ears with your hands.",ch,NULL,victim,TO_VICT);
    			act("{gYou say 'Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!'{w",ch,NULL,victim,TO_CHAR);
				act("$N grimaces in pain, covering $S ears with $S hands.",ch,NULL,victim,TO_CHAR);
				break;
		}
		dam = number_range( ch->level, ch->level*5 );
		dam = dam * get_combat_value("m_damage_poetry");
		dam = dam / get_combat_value("d_damage_poetry");
		if (ch->level > 50 && victim->level > 50)
		{
		   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
		}	
		damage(ch,victim,dam,gsn_poetry,DAM_OTHER,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_poetry,TRUE,1);
		WAIT_STATE(victim, get_combat_value("lag_poetry_victim") * PULSE_VIOLENCE);
//        stop_fighting( victim, TRUE );
		victim->position = POS_RESTING;
    }
    else
    {
		damage( ch, victim, 0, gsn_poetry,DAM_OTHER,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_poetry,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}

void do_sting( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;
    int chance;
    int hitchance;
    char arg[MAX_INPUT_LENGTH];
    int dam;
    int level;
    AFFECT_DATA *poison, af;
	int new_ac;
  

    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_sting) )
    {
	send_to_char("You can't do that.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_STING))
	return;

    if ( IS_IMMORTAL(ch) )
    {
		one_argument(argument,arg);
		if (arg[0] == '\0')
		{
			victim = ch->fighting;
			if (victim == NULL)
			{
			    send_to_char("But you aren't fighting anyone!\n\r",ch);
			    return;
			}
		}
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r",ch);
			return;
		}
		if (victim->position < POS_FIGHTING)
		{
			act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
			return;
		}
		if (victim == ch)
		{
			send_to_char("You fall on your ass.\n\r",ch);
			return;
		}
		if (is_safe(ch,victim))
			return;
		if (  IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(ch,victim->fighting))
		{
			send_to_char("Kill stealing is not permitted.\n\r",ch);
			return;
		}
		if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
		{
			act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
			return;
		}
    }
    else
    {
		if ( (( victim = ch->fighting ) == NULL) )
		{
			send_to_char( "That's not exactly fair, is it?\n\r", ch );
			return;
		}
    }

    chance = get_skill(ch,gsn_sting);
    hitchance = number_percent();

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 200;
    chance -= victim->carry_weight / 250;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX)*2;
    chance += get_curr_stat(ch,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 15;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 25;

        /* level */
    if ( ch->level > (victim->level + 50) )
		chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )
		chance -= (victim->level - ch->level);

    switch(check_immune(victim,DAM_PIERCE))
    {
	case(IS_IMMUNE):
	    chance /= 4;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 3/2;
	    break;
    }
    	
	new_ac = GET_AC(victim,AC_PIERCE); 
	    
    new_ac = new_ac * get_combat_value("m_ac");
    new_ac = new_ac / get_combat_value("d_ac"); 

    WAIT_STATE( ch, skill_table[gsn_sting].beats*get_combat_value("lag_sting_ch") );

    if ( chance > hitchance)
    {
		act("$n stings $N, plunging $S venmous tail into $S chest.",ch,NULL,victim,TO_NOTVICT);
    	act("$n stings you, plunging $S tail into your chest.",ch,NULL,victim,TO_VICT);
    	act("You sting $N with your tail.",ch,NULL,victim,TO_CHAR);
		dam = number_range( ch->level*4, ch->level*10 );
		dam = dam * get_combat_value("m_damage_sting");
		dam = dam / get_combat_value("d_damage_sting");
		if (ch->level > 50 && victim->level > 50)
		{
		   	dam = dam + new_ac;	 // For above 50 chars, armor now absorbs damage
		}			
		damage(ch,victim,dam,gsn_sting,DAM_PIERCE,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_sting,TRUE,1);
		level =	ch->level;
	    if (!saves_spell(level / 2,victim,DAM_POISON))
	    {
			send_to_char("{GYou feel {gpoison{G coursing through your veins.{w",victim);
			act("{G$N{G is {gpoisoned{G by $n's venom.{w", ch,NULL,victim,TO_ROOM);

			af.where     = TO_AFFECTS;
			af.type      = gsn_poison;
			af.level     = level;
			af.duration  = level / 2;
			af.location  = APPLY_STR;
			af.modifier  = -2;
			af.bitvector = AFF_POISON;
			affect_join( victim, &af );
	    }
		WAIT_STATE(victim, get_combat_value("lag_sting_victim") * PULSE_VIOLENCE);
		victim->position = POS_SITTING;
    }
    else
    {
		damage( ch, victim, 0, gsn_sting,DAM_PIERCE,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_sting,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}

void do_drain( CHAR_DATA *ch, char *argument )
{

    CHAR_DATA *victim;
    int chance;
    int hitchance;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int dam;
    int condition;
    OBJ_DATA *obj;

	if (race_lookup("vampire") == ch->race)
	{
		one_argument( argument, arg2 );
		obj = get_obj_list( ch, arg2, ch->in_room->contents );
	    if ( obj == NULL )
	    {
			send_to_char( "You can't find it.\n\r", ch );
			return;
	    }
	    if ( obj->item_type == ITEM_CORPSE_PC )
    	{
 		    send_to_char("You draw the body closer to you and sink your teeth into it.\n\r",ch);
 		    send_to_char("Your gnawing hunger is finally sated as you drink the {rblood{w flowing into your mouth.\n\r",ch); 		    
 		    condition = ch->pcdata->condition[COND_HUNGER];
		    gain_condition( ch, COND_FULL, 30 );
		    gain_condition( ch, COND_HUNGER, 60 );
		    if ( condition == 0 && ch->pcdata->condition[COND_HUNGER] > 0 )
			send_to_char( "You are no longer hungry.\n\r", ch );
		    else if ( ch->pcdata->condition[COND_FULL] > 40 )
			send_to_char( "You are full.\n\r", ch );
			return;   		
    	}
    	else if ( obj->item_type == ITEM_CORPSE_NPC)
    	{
		    send_to_char("You sink your teeth into the neck of the corpse drawing forth its precious blood.\n\r",ch);
		    condition = ch->pcdata->condition[COND_HUNGER];
		    gain_condition( ch, COND_FULL, 3 );
		    gain_condition( ch, COND_HUNGER, 5);
		    if ( condition == 0 && ch->pcdata->condition[COND_HUNGER] > 0 )
			send_to_char( "You are no longer hungry.\n\r", ch );
		    else if ( ch->pcdata->condition[COND_FULL] > 40 )
			send_to_char( "You are full.\n\r", ch );
    		extract_obj( obj );
    		return;
    	}
    	else 
    	{
    		send_to_char("You can't eat that.\n\r",ch);
			return;	
    	}
	}
    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_drain) )
    {
	send_to_char("You can't do that.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DRAIN))
	return;

    if ( IS_IMMORTAL(ch) )
    {
		one_argument(argument,arg);
		if (arg[0] == '\0')
		{
			victim = ch->fighting;
			if (victim == NULL)
			{
			    send_to_char("But you aren't fighting anyone!\n\r",ch);
			    return;
			}
		}
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r",ch);
			return;
		}
		if (victim->position < POS_FIGHTING)
		{
			act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
			return;
		}
		if (victim == ch)
		{
			send_to_char("That doesn't even make sense.\n\r",ch);
			return;
		}
		if (is_safe(ch,victim))
			return;
		if (  IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(ch,victim->fighting))
		{
			send_to_char("Kill stealing is not permitted.\n\r",ch);
			return;
		}
		if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
		{
			act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
			return;
		}
    }
    else
    {
		if ( (( victim = ch->fighting ) == NULL) )
		{
			send_to_char( "That just isn't fair.\n\r", ch );
			return;
		}
    }

    chance = get_skill(ch,gsn_drain);
    hitchance = number_percent();

    /* modifiers */


    /* size  and weight */
/*  chance += ch->carry_weight / 200;
    chance -= victim->carry_weight / 250;
*/

    /* stats */
    //chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX)*2;
    chance += get_curr_stat(ch,STAT_DEX);

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 15;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 25;

    /* level */
    if ( ch->level > (victim->level + 50) )
		chance += (ch->level - victim->level);

    if ( victim->level > (ch->level + 50) )
		chance -= (victim->level - ch->level);

    switch(check_immune(victim,DAM_NEGATIVE))
    {
	case(IS_IMMUNE):
	    chance = 0;
	    break;
	case(IS_RESISTANT):
	    chance /= 2;
	    break;
	case(IS_VULNERABLE):
	    chance *= 2;
	    break;
    }

    WAIT_STATE( ch, skill_table[gsn_drain].beats*get_combat_value("lag_drain_ch") );

    if ( chance > hitchance)
    {
		if (IS_AFFECTED(victim,AFF_FLYING))
		{
			act("{D$n{W levitates and reaches into $N's chest, draining $S life away.{w",ch,NULL,victim,TO_NOTVICT);
	    	act("{D$n{W levitates up to you, and reaches into your chest, draining your life away.{w",ch,NULL,victim,TO_VICT);
	    	act("{WYou levitate up to $N{W, and reach into $S chest, draining $S life away.{w",ch,NULL,victim,TO_CHAR);
			if(ch->level<100)
			{
				dam = number_range( ch->level/4, ch->level*3/2);
				dam = dam * get_combat_value("m_damage_drain");
				dam = dam / get_combat_value("d_damage_drain");
			}
			else
			{
				dam = number_range( ch->level*4, ch->level*10 );
				dam = dam * get_combat_value("m_damage_drain");
				dam = dam / get_combat_value("d_damage_drain");
			}
			//dam = number_range(1, wield->level / 5 + 1);
			damage_old(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
			//ch->alignment = UMAX(-1000,ch->alignment - 1);
			if (ch->hit < 30000)
			{
				if ((ch->hit + dam*2/3) >= 30000 )
				{
					ch->hit = 30000;
				}
				else
				{
					ch->hit += dam*2/3;
				}
			}
			//damage(ch,victim,dam,gsn_buffet,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_drain,TRUE,1);
			affect_strip(victim,skill_lookup("fly"));
			//WAIT_STATE(victim,skill_table[gsn_buffet].beats*get_combat_value("lag_buffet_victim"));
	        //stop_fighting( victim, TRUE );
			victim->position = POS_STANDING;
		}
		else
		{
			act("{D$n{W reaches into $N's chest, draining $S life away.{w",ch,NULL,victim,TO_NOTVICT);
	    	act("{D$n{W reaches into your chest, draining your life away.{w",ch,NULL,victim,TO_VICT);
	    	act("{WYou reach into $N{W's chest, draining $S life away.{w",ch,NULL,victim,TO_CHAR);
			if(ch->level<100)
			{
				dam = number_range( ch->level/4, ch->level*3/2);
				dam = dam * get_combat_value("m_damage_drain");
				dam = dam / get_combat_value("d_damage_drain");
			}
			else
			{
				dam = number_range( ch->level*4, ch->level*10 );
				dam = dam * get_combat_value("m_damage_drain");
				dam = dam / get_combat_value("d_damage_drain");
			}
			damage_old(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
			//ch->alignment = UMAX(-1000,ch->alignment - 1);
			if (ch->hit < 30000)
			{
				if ((ch->hit + dam*2/3) >= 30000 )
				{
					ch->hit = 30000;
				}
				else
				{
					ch->hit += dam*2/3;
				}
			}
			//damage(ch,victim,dam,gsn_buffet,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
			check_improve(ch,gsn_drain,TRUE,1);
			WAIT_STATE(victim, get_combat_value("lag_drain_victim") * PULSE_VIOLENCE);
            //stop_fighting( victim, TRUE );
			victim->position = POS_RESTING;
		}
    }
    else
    {
		damage( ch, victim, 0, gsn_drain,DAM_NEGATIVE,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
		check_improve(ch,gsn_drain,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}


void do_gouge( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    int chance;



    one_argument(argument,arg);



    if ( (chance = get_skill(ch,gsn_gouge)) == 0

    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_GOUGE))

    ||   (!IS_NPC(ch) && !can_use_skill(ch, gsn_gouge)))

    {

	send_to_char("You miss completely.\n\r",ch);

	return;

    }



    if (arg[0] == '\0')

    {

	victim = ch->fighting;

	if (victim == NULL)

	{

	    send_to_char("But you aren't in combat!\n\r",ch);

	    return;

	}

    }



    else if ((victim = get_char_room(ch,arg)) == NULL)

    {

	send_to_char("They aren't here.\n\r",ch);

	return;

    }



    if (IS_AFFECTED(victim,AFF_BLIND))

    {

	act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);

	return;

    }



    if (victim == ch)

    {

	send_to_char("Very funny.\n\r",ch);

	return;

    }



    if (is_safe(ch,victim))

	return;



    if (IS_NPC(victim) &&

	 victim->fighting != NULL &&

	!is_same_group(ch,victim->fighting))

    {

	send_to_char("Kill stealing is not permitted.\n\r",ch);

	return;

    }



    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)

    {

	act("But $N is such a good friend!",ch,NULL,victim,TO_CHAR);

	return;

    }



    /* modifiers */



    /* dexterity */

    chance += get_curr_stat(ch,STAT_DEX);

    chance -= 2 * get_curr_stat(victim,STAT_DEX);



    /* speed  */

    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))

	chance += 10;

    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))

	chance -= 25;



    /* level */

    chance += (ch->level - victim->level) * 2;



    /* Thief Gets a better chance */

    if ( ch->class == 2 )

	chance += ch->level;



    /* sloppy hack to prevent false zeroes */

    if (chance % 5 == 0)

	chance += 1;



    /* terrain */



    switch(ch->in_room->sector_type)

    {

	case(SECT_INSIDE):              chance -= 20;   break;

	case(SECT_CITY):                chance -= 10;   break;

	case(SECT_FIELD):               chance +=  5;   break;

	case(SECT_FOREST):                              break;

	case(SECT_HILLS):                               break;

	case(SECT_MOUNTAIN):            chance -= 10;   break;

	case(SECT_WATER_SWIM):          chance  =  0;   break;

	case(SECT_WATER_NOSWIM):        chance  =  0;   break;

	case(SECT_AIR):                 chance  =  0;   break;

	case(SECT_DESERT):              chance += 10;   break;

    }



    if (chance == 0)

    {

	send_to_char("You can't seem to move fast enough.\n\r",ch);

	return;

    }



    /* now the attack */

    if (number_percent() < chance)

    {

	AFFECT_DATA af;

	act("{W$n gets $s eye poked ruthlessly!{w",victim,NULL,NULL,TO_ROOM);

	act("{Y$n{Y ruthlessly gouges you in the eye!{w",ch,NULL,victim,TO_VICT);

	damage(ch,victim,number_range(2,5),gsn_gouge,DAM_NONE,FALSE, FALSE); /* Grayson -- Parryable 4 May 2003 */

	send_to_char("You flinch and squint as your eyes tear up!\n\r",victim);

	check_improve(ch,gsn_gouge,TRUE,2);

	WAIT_STATE(ch,skill_table[gsn_gouge].beats);



	af.where        = TO_AFFECTS;

	af.type         = gsn_gouge;

	af.level        = ch->level;



	if ( ch->class == 2 )

		af.duration     = get_combat_value( "gouge_time_thief" );

	else

		af.duration     = get_combat_value( "gouge_time_other" );



	af.location     = APPLY_HITROLL;

	af.modifier     = -4;

	af.bitvector    = AFF_BLIND;



	affect_to_char(victim,&af);

    }

    else

    {

	damage(ch,victim,0,gsn_gouge,DAM_NONE,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */

	check_improve(ch,gsn_gouge,FALSE,2);

	WAIT_STATE(ch,skill_table[gsn_gouge].beats);

    }

	check_killer(ch,victim);

}









void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( !IS_NPC(ch) && !can_use_skill(ch, gsn_kick) )
    {
	send_to_char(
	    "You better leave the martial arts to fighters.\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( get_skill(ch,gsn_kick) > number_percent())
    {
	damage(ch,victim,number_range( 1, ch->level ), gsn_kick,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
	check_improve(ch,gsn_kick,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_kick,DAM_BASH,TRUE, FALSE); /* Grayson -- Parryable 4 May 2003 */
	check_improve(ch,gsn_kick,FALSE,1);
    }
	check_killer(ch,victim);
    return;
}








void do_disarm( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    OBJ_DATA *obj;

    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    bool weap1, weap2;



    hth = 0;



    if ((chance = get_skill(ch,gsn_disarm)) == 0)

    {

	send_to_char( "You don't know how to disarm opponents.\n\r", ch );

	return;

    }



    if ( get_eq_char( ch, WEAR_WIELD ) == NULL

    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0

    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))

    {

	send_to_char( "You must wield a weapon to disarm.\n\r", ch );

	return;

    }



    if ( ( victim = ch->fighting ) == NULL )

    {

	send_to_char( "You aren't fighting anyone.\n\r", ch );

	return;

    }



    if ( get_eq_char( victim, WEAR_WIELD ) != NULL )

	weap1 = TRUE;

    else

	weap1 = FALSE;



    if ( get_eq_char( victim, WEAR_SECONDARY ) != NULL )  /*  Grayson Dual Wield/Disarm 12 April 2003 */

	weap2 = TRUE;

    else

	weap2 = FALSE;



    if ( !weap1 && !weap2 )

    {

	send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );

	return;

    }



    if ( weap2 )

	obj = get_eq_char( victim, WEAR_SECONDARY );

    else

	obj = get_eq_char( victim, WEAR_WIELD );



    /* find weapon skills */

    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));

    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));

    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));



    /* modifiers */



    /* skill */

    if ( get_eq_char(ch,WEAR_WIELD) == NULL)

	chance = chance * hth/150;

    else

	chance = chance * ch_weapon/100;



    chance += (ch_vict_weapon/2 - vict_weapon) / 2;



    /* dex vs. strength */

    chance += get_curr_stat(ch,STAT_DEX);

    chance -= 2 * get_curr_stat(victim,STAT_STR);



    /* level */

    chance += (ch->level - victim->level) * 2;



    /* Secondary Weapon */

    if ( weap2 )

    {

	chance += get_curr_stat(ch,STAT_DEX);

	chance += get_curr_stat(ch,STAT_STR);

    }



    /* and now the attack */

    if (number_percent() < chance)

    {

	WAIT_STATE( ch, skill_table[gsn_disarm].beats );

	disarm( ch, victim );

	check_improve(ch,gsn_disarm,TRUE,1);

    }

    else

    {

	WAIT_STATE(ch,skill_table[gsn_disarm].beats);

	act("You fail to disarm $N.",ch,NULL,victim,TO_CHAR);

	act("$n tries to disarm you, but fails.",ch,NULL,victim,TO_VICT);

	act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);

	check_improve(ch,gsn_disarm,FALSE,1);

    }

    check_killer(ch,victim);

    return;

}



void do_suicid( CHAR_DATA *ch, char *argument )

{

	send_to_char("If you wish to kill yourself, you must type it out.\n\r", ch);

	return;

}



void do_suicide( CHAR_DATA *ch, char *argument )

{

	char buf[MAX_STRING_LENGTH];

	int  exp;

	bool can_die;





	can_die	= TRUE;





	/* Calculate EXP to lose */

	exp		= get_combat_value( "suicide_exp" );

	if ( ch->hit > 1 )

	{

		exp += ch->hit;

	}

	exp *= -1;





	/* People who can not kill themselves */

	if (IS_NPC(ch))

	{

		can_die	= FALSE;

		return;

	}



	/* Fighting */

	if (ch->fighting != NULL)

	{

		can_die	= FALSE;

		send_to_char( "Not while fighting!\n\r", ch);

		return;

	}



	/* Ghosts */

	if (IS_SET(ch->info, INFO_GHOST))

	{

		can_die	= FALSE;

		send_to_char( "You're already dead!\n\r", ch);

		return;

	}



	/* Immortal */

	if ( IS_IMMORTAL(ch) )

	{

		can_die	= FALSE;

		send_to_char( "You are an Immortal, you can not kill yourself!\n\r", ch );

		return;

	}



	/* To much EXP to Level */

	if ( ((ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp) >= get_combat_value( "suicide_exp_limit" ) )

	{

		can_die	= FALSE;

		send_to_char( "You require to many experience points for your next level!\n\r", ch );

		send_to_char( "The Immortals deny you a chance to die!\n\r", ch );

		return;

	}





	/* Now to look at the rooms */

	/* special room */

	if (	IS_SET(ch->in_room->room_flags,ROOM_SAFE)

		|| IS_SET(ch->in_room->room_flags,ROOM_NOPK)

		|| IS_SET(ch->in_room->room_flags,ROOM_ARENA) )

	{

	   can_die	= FALSE;

	}



	/* Rooms that we will let them out of, eventually */

	if (	ch->in_room->vnum == ROOM_VNUM_IMMINT

		|| IS_SET(ch->in_room->room_flags,ROOM_NO_RECALL) )

	{

		can_die	= TRUE;

		if ( ch->hit > 0 )

		{

			send_to_char( "You are not near enough to death.\n\r", ch );

			can_die	= FALSE;

		}

	}



	/* restricted room */

	if ( 	ch->in_room->vnum == ROOM_VNUM_JAIL

		|| ch->in_room->vnum == ROOM_VNUM_PRISON

		|| ch->in_room->vnum == ROOM_VNUM_SC01

		|| ch->in_room->vnum == ROOM_VNUM_SC02

		|| ch->in_room->vnum == ROOM_VNUM_SC03

		|| ch->in_room->vnum == ROOM_VNUM_SC04

		|| ch->in_room->vnum == ROOM_VNUM_SC05 )

	{

		send_to_char( "You can not escape by killing yourself!\n\r", ch );

		gain_exp( ch, exp );

		sprintf ( buf, "You lose %d EXP for trying to comitting suicide.\n\r", exp );

		send_to_char( buf, ch );

		sprintf(buf, "SUICIDE :: %s tried to kill themself in a prison room at %ld", ch->name, ch->in_room->vnum );

		log_string( buf );

		wiznet(buf,NULL,NULL,WIZ_DEATHS,0,0);

		can_die	= FALSE;

	}





	/* So, can we die or not */

	if ( !can_die )

	{

		send_to_char( "The Immortals deny you a chance to die!\n\r", ch );

		return;

	}



	send_to_char("You kill yourself!\n\r", ch);



	act( "$n has comitted suicide!",  ch, NULL, NULL, TO_ROOM );



	sprintf(buf, "SUICIDE :: %s has killed themself at %ld", ch->name, ch->in_room->vnum );

	log_string( buf );

	wiznet(buf,NULL,NULL,WIZ_DEATHS,0,0);



	gain_exp( ch, exp );

	sprintf( buf, "You lose %d EXP for commiting suicide.\n\r", exp );

	send_to_char( buf, ch );



	raw_kill( ch );



	return;

}





void do_sla( CHAR_DATA *ch, char *argument )

{

    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );

    return;

}







void do_slay( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    char arg[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];

    DESCRIPTOR_DATA *d;

    extern  bool        global_trident;

    extern  CHAR_DATA  *global_trident_target;

    extern  int         global_trident_timer;

    extern  ROOM_INDEX_DATA *global_trident_room;



    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	if (!IS_IMMORTAL(ch))

		send_to_char( "Slay whom?\n\r", ch );

	else

	{

		send_to_char( "SYNTAX: Slay <victim> <modifyer>                        \n\r", ch);

		send_to_char( "If modifyer is blank, the victim is slayed.             \n\r", ch );

		send_to_char( "Modifyer choices are FIRE, LIGHTNING, BLAST, HANG,      \n\r", ch );

		send_to_char( "ASS, FROST, BRICK, SHATTER, BLADES, BUGS, PIANO,        \n\r", ch );

		send_to_char( "WRATH, ANVIL, EAT, and TRIDENT \n\r", ch );

	}



	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( !IS_NPC(victim) && (get_trust(ch) < victim->level) )

    {

	send_to_char( "That was a bad idea.\n\r", ch );

	return;

    }



    one_argument( argument, arg2 );



    if ( arg2[0] == '\0' )

    {

	act( "You slay $M in cold blood!",  ch, NULL, victim, TO_CHAR    );

	act( "$n slays you in cold blood!", ch, NULL, victim, TO_VICT    );

	act( "$n has slain $N in cold blood!",  ch, NULL, victim, TO_NOTVICT );

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUTNER */

	return;

    }

    else if ( !str_prefix( arg2, "fire" ) && IS_IMMORTAL(ch) )

    {

	act( "You toast $M in a column of fire!",  ch, NULL, victim, TO_CHAR    );

	act( "$n toasts you in a column of fire!", ch, NULL, victim, TO_VICT    );

	act( "$n toasts $N in a column of fire!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s gets {Wtoasted{w in a column of {rFIRE{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "lightning" ) && IS_IMMORTAL(ch) )

    {

	act( "You slay $M with a bolt of lightning!",  ch, NULL, victim, TO_CHAR    );

	act( "$n kills you with a bolt of lightning!", ch, NULL, victim, TO_VICT    );

	act( "$n kills $N with a bolt of lightning!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "A {cBOLT{w of {yLIGHTNING{w blows %s to {Wcharred{w bits!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	return;

    }

    else if ( !str_prefix( arg2, "blast" ) && IS_IMMORTAL(ch) )

    {

	act( "You blast $M into oblivion.",  ch, NULL, victim, TO_CHAR    );

	act( "$n blasts you into oblivion!", ch, NULL, victim, TO_VICT    );

	act( "$n blasts $N into oblivion!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s is {gBLASTED{w into {bOBLIVION{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "frost" ) && IS_IMMORTAL(ch) )

    {

	act( "You crystalize $M in a sheet of ice!",  ch, NULL, victim, TO_CHAR    );

	act( "$n crystalizes you in a sheet of ice!", ch, NULL, victim, TO_VICT    );

	act( "$n crystalizes $N in a sheet of ice!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s gets {bcrystalized{w in a sheet of {cICE{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "brick" ) && IS_IMMORTAL(ch) )

    {

	act( "You crush $M under a ton of bricks!",  ch, NULL, victim, TO_CHAR    );

	act( "$n crushes you under a ton of bricks!", ch, NULL, victim, TO_VICT    );

	act( "$n drops a ton of bricks on $N and crushes them!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s {wgets crushed under a ton of {rbricks{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "shatter" ) && IS_IMMORTAL(ch) )

    {

	act( "You shatter $M into thousands of pieces!",  ch, NULL, victim, TO_CHAR    );

	act( "$n shatters you into a thousand pieces!", ch, NULL, victim, TO_VICT    );

	act( "$n shatters $N into a thousand pieces!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s gets {gshattered{w into a thousand pieces{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "blades" ) && IS_IMMORTAL(ch) )

    {

	act( "Blades of steel and cutting chains fly forth from you and rip $M to shreds!",  ch, NULL, victim, TO_CHAR    );

	act( "Blades of steel and cutting chains fly forth from $n and rip you to shreds!", ch, NULL, victim, TO_VICT    );

	act( "Blades of steel and cutting chains fly forth from $n and rip $N to shreds!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "{wBlades of {Wsteel{w and cutting chains {rrip {w%s to shreds!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "piano" ) && IS_IMMORTAL(ch) )

    {

	act( "You crush $M under a {WBlack Grand Piano{w!",  ch, NULL, victim, TO_CHAR    );

	act( "$n crushes you under a {WBlack Grand Piano{w!", ch, NULL, victim, TO_VICT    );

	act( "$n drops a {WBlack Grand Piano{w on $N and crushes them!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s {wgets crushed under a {WBlack Grand Piano{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "wrath" ) && IS_IMMORTAL(ch) )

    {

	act( "You slay $M with your wrath!",  ch, NULL, victim, TO_CHAR    );

	act( "The wrath of $n slays you!", ch, NULL, victim, TO_VICT    );

	act( "The wrath of $n slays $N!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s is slayed by the {Runholy{C WRATH{w of {yThe Immortals{w!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "anvil" ) && IS_IMMORTAL(ch) )

    {

	act( "You crush $M under an anvil!",  ch, NULL, victim, TO_CHAR    );

	act( "$n crushes you under an anvil!", ch, NULL, victim, TO_VICT    );

	act( "$n drops an anvil on $N and crushes them!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "An {Danvil{w falls from the sky and crushes %s!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "hang" ) && IS_IMMORTAL(ch) )

    {

	act( "You hang $M from the gallows!",  ch, NULL, victim, TO_CHAR    );

	act( "$n hangs you from the gallows!", ch, NULL, victim, TO_VICT    );

	act( "$n hangs $N from the gallows!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "%s{w is hanged from the gallows!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "bugs" ) && IS_IMMORTAL(ch) )

    {

	act( "You release a horde of bugs and rats to eat $M!",  ch, NULL, victim, TO_CHAR    );

	act( "$n releases a horde of bugs and rats to eat you alive!", ch, NULL, victim, TO_VICT    );

	act( "$n releases a horde of bugs and rats to eat $N alive!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "{wA horde of bugs and rats eats %s{w alive!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else if ( !str_prefix( arg2, "eat" ) && IS_IMMORTAL(ch) )

    {

	/* Let the slayer and the room the victim was in see them die... */

	act( "You stretch open your mouth and swallow $M whole!",  ch, NULL, victim, TO_CHAR );

	act( "$n stretches $s mouth open wide and swallows $N whole!",  ch, NULL, victim, TO_ROOM );

	act( "$n slays you in cold blood!", ch, NULL, victim, TO_VICT );



	/* Move them to the room */

	char_from_room( victim );

	char_to_room( victim, get_room_index( ROOM_VNUM_IMMINT ) );



	/* Kill them */

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */





	/* Are they a mob, or a player */

	if ( !IS_NPC( victim ) )

	{

		char_from_room( victim );

		char_to_room( victim, get_room_index( ROOM_VNUM_IMMINT ) );



		/* Now... Kill them... slowly... */

		send_to_char( "{w\n\r\n\r", victim );
		act( "$n appears before you, stretches $s mouth open wide and swallows you whole!", ch, NULL, victim, TO_VICT );
		send_to_char( "\n\r\n\rYou find yourself being torn to shreads by teeth as strong as steel...\n\r", victim );
		send_to_char( "You feel yourself being pulled downward to a pit of darkness...\n\r", victim );
		send_to_char( "The odor smells of dead, dying and dammed things as you fall into...\n\r", victim );
		send_to_char( "\n\r\n\r{rAn {yImmortal's{r Stomach!!!{w\n\r\n\r", victim );
		do_look( victim, "auto" );
	}

	sprintf(buf,"%s has slayed and eaten %s", ch->name, victim->name );
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	return;
    }

    else if ( !str_prefix( arg2, "trident" ) && IS_IMMORTAL(ch) )

    {

	if ( ch->level < SUPREME || get_trust(ch) < SUPREME )

	{

		send_to_char( "Only Admins's may use the Trident Slay.\n\r", ch );

		return;

	}



	if ( IS_NPC(victim) )

	{

		send_to_char( "Trident can not be used on Mobs.\n\r", ch );

		return;

	}



	if ( IS_IMMORTAL( victim ) && (ch->level < MAX_LEVEL || get_trust(ch) < MAX_LEVEL) )

	{

		send_to_char( "Only IMP's may use the Trident on other Immortals.\n\r", ch );

		return;

	}



	if ( global_trident )

	{

		send_to_char( "Trident already active!\n\r", ch );

		sprintf( buf, "A trident has already been launched at %s.\n\r", global_trident_target->name );

		send_to_char( buf, ch );

		return;

	}



	/* Begin Slay and Freeze Target */

	send_to_char( "Initiating Trident Slay... Please stand by...\n\r", ch );

	sprintf( buf, "%s fired a trident on %s", ch->name, victim->name );

	wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

	log_string( buf );



	global_trident_target	= victim;

	global_trident_room	= victim->in_room;

	global_trident_timer 	= 0;

	global_trident		= TRUE;



	if ( !IS_IMMORTAL(victim) )

	{

		SET_BIT(victim->act, PLR_FREEZE);

		send_to_char( "You have been targeted by the Imms for a Trident Slay!\n\r", victim );

		act( "$n has been targeted by the Imms!", victim, NULL, NULL, TO_ROOM );

		char_from_room( victim );

		char_to_room( victim, get_room_index( ROOM_VNUM_TROPHY ) );

		act( "$n has been targeted by the Imms!", victim, NULL, NULL, TO_ROOM );

		do_look( victim, "auto" );

	}

	else

	{

		SET_BIT(victim->act, PLR_FREEZE);

		char_from_room( victim );

		char_to_room( victim, get_room_index( ROOM_VNUM_TROPHY ) );

		do_look( victim, "auto" );

	}



	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING && IS_HERO(d->character) )

	   {

		sprintf( buf, "{RIMM-INFO{w: %s{W has been targeted by a Trident Slay.\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}



	++global_trident_timer;



	return;

    }

    else if ( !str_prefix( arg2, "ass" ) && IS_IMMORTAL(ch) )

    {

	act( "You slay $M with a big ass bolt of lightning!",  ch, NULL, victim, TO_CHAR    );

	act( "You are slayed by $n with a big ass bolt of lightning!", ch, NULL, victim, TO_VICT    );

	act( "$n slays $N with a big ass bolt of lightning!",  ch, NULL, victim, TO_NOTVICT );

	for ( d = descriptor_list; d; d = d->next )

	{

	   if ( d->connected == CON_PLAYING )

	   {

		sprintf( buf, "A {cBIG ASS BOLT{w of {yLIGHTNING{w strikes %s{w dead!\n\r", PERS(victim, d->character) );

		send_to_char( buf, d->character );

	   }

	}

	raw_kill( victim );

	cnt_kill( victim, ch ); /* URK_KILLCOUNTER */

	return;

    }

    else

    {

	if (!IS_IMMORTAL(ch))

		send_to_char( "Slay whom?\n\r", ch );

	else

		do_slay( ch, "" );



	return;

    }



    do_slay( ch, "" );



    return;

}

