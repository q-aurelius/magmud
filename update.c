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
#include <string.h>
#include <time.h>
#include "merc.h"
#include "music.h"
#include "tables.h"


/* command procedures needed */
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_look        );
DECLARE_DO_FUN(do_gossip      );
DECLARE_DO_FUN(do_flamechan	);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_save		);
DECLARE_DO_FUN(do_say		);

void do_remort_engine args(( CHAR_DATA *ch, bool IsRemort));


/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void  quest_update    args( ( void ) ); /* Vassago - quest.c */
void  who_html_update args( ( void ) );
void	autodown_update args( ( void ) );

/* used for saving */

int	save_number = 0;



/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA *ch, bool hide )
{
    char buf[MAX_STRING_LENGTH];
    int add_hp;
    int add_mana;
    int add_move;
    int add_prac;
    int hours;
    OBJ_DATA *prize, *wield;
    int numdice;

    ch->pcdata->last_level =
	( ch->perm_played + (int) (current_time - ch->logon) ) / 3600;

    /* Removed Title Changes
    sprintf( buf, "the %s",
	title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
    set_title( ch, buf );
    */

	hours = GET_HOURS(ch);
	
	
    add_hp	= con_app[get_curr_stat(ch,STAT_CON)].hitp + number_range(
		    class_table[ch->class].hp_min,
		    class_table[ch->class].hp_max );
    add_mana 	= number_range(2,(2*get_curr_stat(ch,STAT_INT)
				  + get_curr_stat(ch,STAT_WIS))/5);
    if (!class_table[ch->class].fMana)
	add_mana /= 2;
    add_move	= number_range( 1, (get_curr_stat(ch,STAT_CON)
				  + get_curr_stat(ch,STAT_DEX))/6 );
    add_prac	= wis_app[get_curr_stat(ch,STAT_WIS)].practice;

    add_hp = add_hp * 9/10;
    add_mana = add_mana * 9/10;
    add_move = add_move * 9/10;
	wield = get_eq_char( ch, WEAR_WIELD );
	
	if (wield!=NULL)
	{	
		if (ch->class == class_lookup("jedi") &&
		    wield->value[0] == WEAPON_LIGHTSABER)
		{
			 numdice = .8*ch->level;
			 wield->level = ch->level;
	   		 wield->value[1] = numdice;
	    	 wield->value[2] = 2;
		}	    
	}

    if ( hours >= 600 )
    {
		hours -= 500; // Grayson New Hour Bonuses 21 June 2004
		hours = hours/100;
		
		add_hp	+= hours;
		add_mana	+= hours;
		add_move	+= hours;
    }

    add_hp		= UMAX(  2, add_hp   );
    add_mana	= UMAX(  2, add_mana );
    add_move	= UMAX(  6, add_move );

    ch->max_hit 	+= add_hp;
    ch->max_mana	+= add_mana;
    ch->max_move	+= add_move;
    ch->practice	+= add_prac;
    ch->train		+= 1;

    ch->pcdata->perm_hit	+= add_hp;
    ch->pcdata->perm_mana	+= add_mana;
    ch->pcdata->perm_move	+= add_move;

    if (!hide)
    {
    	sprintf(buf,
	    "You gain %d hit point%s, %d mana, %d move, and %d practice%s.\n\r",
	    add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
	    add_prac, add_prac == 1 ? "" : "s");
	send_to_char( buf, ch );
	send_to_char( "\n\rThe {yImmortals{w congratulate you on your new level!\n\r", ch );
	switch ( number_range( 0, 10 ) )
	{
		default:		break;
		case 3:
		{
	            affect_strip(ch,gsn_plague);
	            affect_strip(ch,gsn_poison);
	            affect_strip(ch,gsn_blindness);
	            affect_strip(ch,gsn_sleep);
	            affect_strip(ch,gsn_curse);
			affect_strip(ch,skill_lookup("fire breath"));
			affect_strip(ch,skill_lookup("calm"));
			affect_strip(ch,skill_lookup("change sex"));
			affect_strip(ch,skill_lookup("charm person"));
			affect_strip(ch,skill_lookup("chill touch"));
			affect_strip(ch,skill_lookup("faerie fire"));
			affect_strip(ch,skill_lookup("slow"));
			affect_strip(ch,skill_lookup("weaken"));

      	      ch->hit 	= ch->max_hit;
      	      ch->mana	= ch->max_mana;
      	      ch->move	= ch->max_move;
      	      update_pos( ch);
			send_to_char( "As a gift, the {yImmortals{w have restored you to full health!\n\r", ch );
			break;
		}
        case 5:
        {
                prize = create_object(get_obj_index(OBJ_VNUM_RESTRING_PRIZE), 0);
                obj_to_char(prize, ch); //put the prize in target's inventory rather than on the ground at their feet - Ferg
                send_to_char( "As a gift, the {yImmortals{w have given you a {grestring prize{w!\n\r", ch );
                break;
        }
		case 7:
		{
			gain_exp( ch, 150 );
			send_to_char( "As a gift, the {yImmortals{w have given you {g150{w experience points!\n\r", ch );
			break;
		}
	}
    }
    return;
}

void gain_exp( CHAR_DATA *ch, int gain )
{
    char buf[MAX_STRING_LENGTH];
    char wnetbuf[MAX_STRING_LENGTH];
    int levels_gained;
    int exp_lvl;

    if ( IS_NPC(ch) || ch->level >= LEVEL_HERO )
	return;

    if ( ch->level >= 500 )
    {
	  send_to_char("{cYou have reached the Maximum Allowed Mortal Level.\n\r", ch);
	  send_to_char("Perhaps you should consider remorting.{w\n\r", ch);
	return;
    }

    /* Check out our current Experience points to see if we're legal */

    exp_lvl  = exp_per_level(ch,ch->pcdata->points);

    if  ( (ch->exp < ((ch->level - 1) * exp_lvl)) || (ch->exp > ((ch->level +1)*exp_lvl)) )
    {
	sprintf( wnetbuf, "Invalid Experience: %s, level %d has %d Experience, with a range of %d to %d", ch->name, ch->level, ch->exp, (ch->level * exp_lvl), ((ch->level +1)*exp_lvl) );
	wiznet(wnetbuf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	log_string( wnetbuf );
	send_to_char("You're experience was invalid but is now corrected.\n\r", ch);
	ch->exp = ((exp_lvl*ch->level)-(exp_lvl/2));
	gain = 0;
	save_char_obj(ch);
    }

    levels_gained = 0;
    ch->exp = UMAX( exp_lvl, ( ch->exp + gain ) );

    while ( ch->exp >= (exp_lvl*(ch->level+1)) )
    {
	send_to_char( "You raise a level!!  ", ch );
	ch->level += 1;
	levels_gained += 1;
	sprintf(buf,"%s gained level %d",ch->name,ch->level);
	log_string(buf);
	sprintf(buf,"$N has attained level %d!",ch->level);
	wiznet(buf,ch,NULL,WIZ_LEVELS,0,get_trust(ch));
	advance_level(ch,FALSE);
/*	ch->exp  = UMIN( ch->exp, (((ch->level+1)*exp_lvl)-1) );  Saftey Cap */
    }

    save_char_obj(ch);

    return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain =  5 + ch->level;
 	if (IS_AFFECTED(ch,AFF_REGENERATION))
	    gain *= 2;

	switch(ch->position)
	{
	    default : 			gain /= 2;			break;
	    case POS_SLEEPING: 		gain = 3 * gain/2;	break;
	    case POS_RESTING:  						break;
	    case POS_FIGHTING:		gain /= 3;		 	break;
 	}


    }
    else
    {
	gain = UMAX(3,get_curr_stat(ch,STAT_CON) - 3 + ch->level/2);
	gain += class_table[ch->class].hp_max - 10;
 	number = number_percent();
	if (number < get_skill(ch,gsn_fast_healing))
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_fast_healing,TRUE,8);
	}

	switch ( ch->position )
	{
	    default:	   		gain /= 4;			break;
	    case POS_SLEEPING: 						break;
	    case POS_RESTING:  		gain /= 2;			break;
	    case POS_FIGHTING: 		gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;
	if (IS_AFFECTED(ch,AFF_REGENERATION))
		gain *= 2;
	
	if (IS_AFFECTED2(ch, AFF_FORCE_TRANCE) && ch->position == POS_SLEEPING)
		gain *= 2;
    }

    gain = gain * ch->in_room->heal_rate / 100;
    gain *= get_econ_value("m_room_heal");
    gain /= get_econ_value("d_room_heal");

    if (IS_SET(ch->info, INFO_IMMMULTI))
	gain += ch->level;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
	gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
	gain /=2 ;

    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = 5 + ch->level;
	switch (ch->position)
	{
	    default:		gain /= 2;		break;
	    case POS_SLEEPING:	gain = 3 * gain/2;	break;
   	    case POS_RESTING:				break;
	    case POS_FIGHTING:	gain /= 3;		break;
    	}
    }
    else
    {
	gain = (get_curr_stat(ch,STAT_WIS)
	      + get_curr_stat(ch,STAT_INT) + ch->level) / 2;
	number = number_percent();
	if (number < get_skill(ch,gsn_meditation))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_meditation,TRUE,8);
	}
	if (!class_table[ch->class].fMana)
	    gain /= 2;

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;

    }

    gain = gain * ch->in_room->mana_rate / 100;
    gain *= get_econ_value("m_room_mana");
    gain /= get_econ_value("d_room_mana");

    if (IS_SET(ch->info, INFO_IMMMULTI))
	gain += ch->level;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[4] / 100;

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain /=2 ;

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = UMAX( 15, ch->level );

	switch ( ch->position )
	{
	case POS_SLEEPING: gain += get_curr_stat(ch,STAT_DEX);		break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_DEX) / 2;	break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;
    }

    gain = gain * ch->in_room->heal_rate/100;

    if (IS_SET(ch->info, INFO_IMMMULTI))
	gain += ch->level;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;


    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain /=2 ;

    return UMIN(gain, ch->max_move - ch->move);
}




void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( 	value == 0
		|| IS_NPC(ch)
		|| ch->level >= LEVEL_HERO
		|| IS_SET(ch->info, INFO_GHOST)
		|| IS_SET(ch->info, INFO_IMMMULTI)  )
	return;

    condition				= ch->pcdata->condition[iCond];
    if (condition == -1)
        return;

    ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );

    if ( ch->pcdata->condition[iCond] <= 3 && !IS_SET(ch->in_room->room_flags,ROOM_SUSTAIN) )
    {
	switch ( iCond )
	{
	case COND_HUNGER:
	    send_to_char( "You are hungry.\n\r", ch);
	    if ( condition == 0 && ch->level >= 10 )
	    {
			send_to_char( "You become faint with fatigue and hunger.\n\r", ch );
			
			if (race_lookup("vampire") == ch->race)
			{
				if (ch->mana > 100)
				   ch->mana = ch->mana - 100;
				else if (ch->mana > 50)
				   ch->mana = ch->mana - 25;
				   else ch->mana = ch->mana - 15;
				if (ch->hit > 100)
					damage(ch,ch,100,TYPE_HUNGER,DAM_OTHER,FALSE, FALSE);
				else if (ch->hit > 50)
					damage(ch,ch,25,TYPE_HUNGER,DAM_OTHER,FALSE, FALSE);
				  else 
				  damage(ch,ch,15,TYPE_HUNGER,DAM_OTHER,FALSE, FALSE);
				if (ch->move > 100)
				   ch->move = ch->move - 100;
				else if (ch->move > 50)
				   ch->move = ch->move - 25;
				   else ch->move = ch->move - 15;
			}
			else
			{
				if (ch->mana > 10)
				   ch->mana = ch->mana - 10;
				if (ch->hit > 20)
				  damage(ch,ch,15,TYPE_THIRST,DAM_OTHER,FALSE, FALSE);
				  else
				  damage(ch,ch,5,TYPE_THIRST,DAM_OTHER,FALSE, FALSE);
				if (ch->move > 10)
				   ch->move = ch->move - 5;		
			}
	    }
	    break;

	case COND_THIRST:
	    send_to_char( "You are thirsty.\n\r", ch);
	    if ( condition == 0 && ch->level >= 20 )
	    {
		send_to_char( "You become faint with fatigue and thirst.\n\r", ch );
		if (ch->mana > 100)
		   ch->mana = ch->mana - 100;
		else if (ch->mana > 50)
		   ch->mana = ch->mana - 25;
		   else ch->mana = ch->mana - 15;
		if (ch->hit > 100)
			damage(ch,ch,100,TYPE_THIRST,DAM_OTHER,FALSE, FALSE);
		else if (ch->hit > 50)
			damage(ch,ch,25,TYPE_THIRST,DAM_OTHER,FALSE, FALSE);
		  else damage(ch,ch,15,TYPE_THIRST,DAM_OTHER,FALSE, FALSE);
		if (ch->move > 100)
		   ch->move = ch->move - 100;
		else if (ch->move > 50)
		   ch->move = ch->move - 25;
		   else ch->move = ch->move - 15;

	    }
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }

    return;
}






/*

 * Mob autonomous action.

 * This function takes 25% to 35% of ALL Merc cpu time.

 * -- Furey

 */

void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( !IS_NPC(ch) || ch->in_room == NULL || IS_AFFECTED(ch,AFF_CHARM))
	    continue;

	if (ch->in_room->area->empty && !IS_SET(ch->act,ACT_UPDATE_ALWAYS))
	    continue;

	/* Make sure we haven't scrwed up a Mob */
      if ( IS_NPC(ch) && IS_SET(ch->info, INFO_GHOST) )
	    REMOVE_BIT(ch->info, INFO_GHOST);

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}

	if (ch->pIndexData->pShop != NULL) /* give him some gold */
	    if ((ch->gold * 100 + ch->silver) < ch->pIndexData->wealth)
	    {
		ch->gold += ch->pIndexData->wealth * number_range(1,20)/5000000;
		ch->silver += ch->pIndexData->wealth * number_range(1,20)/50000;
	    }


	/* That's all for sleeping / busy monster, and empty zones */
	if ( ch->position == POS_RESTING && ch->in_room->area->empty)
		do_stand(ch,"");
	
	if ( ch->position != POS_STANDING )
	    continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 6 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj)
		     && obj->cost > max  && obj->cost > 0)
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL)
	&& number_bits(3) == 0
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
	&& ( !IS_SET(ch->act, ACT_STAY_AREA)
	||   pexit->u1.to_room->area == ch->in_room->area )
	&& ( !IS_SET(ch->act, ACT_OUTDOORS)
	||   !IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS))
	&& ( !IS_SET(ch->act, ACT_INDOORS)
	||   IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)))
	{
	    move_char( ch, door, FALSE );
	}
    }

    return;
}






/*

 * Update the weather.

 */

void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int diff;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "The day has begun.\n\r" );
	break;

    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The sun rises in the east.\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The sun slowly disappears in the west.\n\r" );
	break;

    case 20:
	weather_info.sunlight = SUN_DARK;
	strcat( buf, "The night has begun.\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default:
	bug( "Weather_update: bad sky %d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The sky is getting cloudy.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "It starts to rain.\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "The clouds disappear.\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "Lightning flashes in the sky.\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The rain stopped.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The lightning has stopped.\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}




/*

 * Update all chars, including mobs.

*/

void char_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;
    DESCRIPTOR_DATA *d;
    extern  bool        betalock;
    extern 	int 	  	gtip_count;
    extern  bool        global_trident;
    extern  CHAR_DATA  *global_trident_target;
    extern  int         global_trident_timer;
    extern  ROOM_INDEX_DATA *global_trident_room;
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char buf[MAX_STRING_LENGTH];
    int string_count;
    int perm_count;

    string_count = nAllocString;
    perm_count = nAllocPerm;

    corpse 		= NULL;
    obj		= NULL;
    obj_next	= NULL;
    ch_quit		= NULL;

    /* update save counter */
    save_number++;

    if (save_number > 29)
	save_number = 0;

    if ( gtip_count > PULSE_TIP )
	gtip_count = 1;
    else
	gtip_count++;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	ch_next = ch->next;

        /*  if ( !IS_IMMORTAL(ch) && ( ch->timer >= 15 ) )
            ch_quit = ch;   */

	if ( ch->position >= POS_STUNNED )
	{
            /* check to see if we need to go home */
            if (IS_NPC(ch) && ch->zone != NULL && ch->zone != ch->in_room->area
            && ch->desc == NULL &&  ch->fighting == NULL
	    && !IS_AFFECTED(ch,AFF_CHARM) && number_percent() < 5)
            {
            	act("$n wanders on home.",ch,NULL,NULL,TO_ROOM);
            	extract_char(ch,TRUE);
            	continue;
            }

	    /* Old Gain Loop
	    if ( ch->hit  < ch->max_hit )
		ch->hit  += hit_gain(ch);
	    else
		ch->hit = ch->max_hit;

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);
	    else
		ch->mana = ch->max_mana;

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	    else
		ch->move = ch->max_move;
	    */

		ch->hit   +=   hit_gain(ch);
		if ( ch->hit > ch->max_hit )
		{
			ch->hit = ch->max_hit;
		}

		ch->mana   +=   mana_gain(ch);
		if ( ch->mana > ch->max_mana )
		{
			ch->mana = ch->max_mana;
		}

		ch->move   +=   move_gain(ch);
		if ( ch->move > ch->max_move )
		{
			ch->move = ch->max_move;
		}

		/* Take Care of Immortals */
		if ( IS_IMMORTAL(ch) )
		{
			ch->hit    = ch->max_hit;
			ch->mana   = ch->max_mana;
			ch->move   = ch->max_move;
		}
	}

	if ( !IS_NPC(ch) )
	{
	    OBJ_DATA *obj;
	    int 	sn, oSkillValue, cnt;

	    ++ch->timer;

	   /*
          * Check for Valid Character Traits
	    */


	   /* Start with Skills */
	   for ( sn = 0; sn < MAX_SKILL; sn++ )
	   {
		if (	(skill_table[sn].name != NULL)
			&& !can_use_skill(ch, sn)
			&& (ch->pcdata->learned[sn] > 1) )	/* They aren't supposed to use it, but they have it */
		{
			oSkillValue		= ch->pcdata->learned[sn];

			ch->pcdata->learned[sn] = 0;
			save_char_obj( ch );

			sprintf( buf, "You should not have been taught {g%s{w.\n\r", skill_table[sn].name );
			send_to_char( buf, ch );
			send_to_char( "Please contact the {yImmortals{w and you may\n\r", ch );
			send_to_char( "receive a refund of your {cPractices{w and/or {cTrains{w.\n\r", ch );

			sprintf(buf, "{gInvalid Skill{w : %-12s{w : %-20s{w at %-3d :   {gLvl/Sk{w: %-4d/%d",
				ch->name,
				skill_table[sn].name,
				oSkillValue,
				skill_table[sn].skill_level[ch->class],
				get_skill(ch,sn) );
			wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

			sprintf(buf, "Invalid Skill : %-12s : %-20s at %-3d :   Lvl/Sk: %-4d/%d",
				ch->name,
				skill_table[sn].name,
				oSkillValue,
				skill_table[sn].skill_level[ch->class],
				get_skill(ch,sn) );
			log_string( buf );
		}

		if ((skill_table[sn].name != NULL) && can_use_skill(ch,sn) && (ch->pcdata->learned[sn] > 100))
		{
			oSkillValue		= ch->pcdata->learned[sn];

			ch->pcdata->learned[sn] = 100;
			save_char_obj( ch );

			sprintf(buf, "{gInvalid Skill{w : {gSkill Value Reset{w : %-12s : %-20s at %-3d : %-3d",
				ch->name,
				skill_table[sn].name,
				oSkillValue,
				get_skill(ch,sn) );
			wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

			sprintf(buf, "Invalid Skill : Skill Value Reset : %-12s : %-20s at %-3d : %-3d",
				ch->name,
				skill_table[sn].name,
				oSkillValue,
				get_skill(ch,sn) );
			log_string( buf );
		}
	   }

	   /*
	    * Now lets check for bad second weapons
	    */

	   if ( get_eq_char( ch, WEAR_SECONDARY ) != NULL )
	   {
		OBJ_DATA *weap1, *weap2;
		char buf[MAX_STRING_LENGTH];
		bool valid;
/*  BOZO marking valid weapons invalid, temporary removal Grayson, 12 April 2003 */
/*		valid = TRUE;
		weap1 = get_eq_char(ch, WEAR_WIELD);
		weap2 = get_eq_char( ch, WEAR_SECONDARY );

		if ( weap1 == NULL )
			valid = FALSE;

		if ( weap2 == NULL )
			valid = FALSE;

		if (weap1->item_type != ITEM_WEAPON)
			valid = FALSE;

		if (weap2->item_type != ITEM_WEAPON)
			valid = FALSE;

		if (get_eq_char(ch,WEAR_SHIELD) != NULL)
			valid = FALSE;

		if (get_eq_char(ch,WEAR_HOLD) != NULL)
			valid = FALSE;

		if (ch->level < weap1->level)
			valid = FALSE;

		if (ch->level < weap2->level)
			valid = FALSE;

		if (get_obj_weight(weap1) > (str_app[get_curr_stat(ch,STAT_STR)].wield * 10))
			valid = FALSE;

		if (get_obj_weight(weap2) > (str_app[get_curr_stat(ch,STAT_STR)].wield * 10))
			valid = FALSE;

		if (get_obj_weight(weap2) > (get_obj_weight(weap1)*2/3))
			valid = FALSE;
*/
		if (IS_IMMORTAL(ch))
			valid = TRUE;

		if (!valid)
		{
			if (weap2 != NULL)
			{
				sprintf(buf, "%s was wielding an invalid second weapon (%s)",
					ch->name,
					weap2->short_descr );
				wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
				log_string( buf );
				remove_obj(ch, WEAR_SECONDARY, TRUE);
			}

			SET_BIT(ch->info, INFO_BOZO);
			ch->pcdata->timer_punished[PUNISH_BOZO] = get_econ_value( "auto_punish" );
			sprintf( buf, "%s has been flagged as a BOZO", ch->name );
			wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
			log_string( buf );
		}
	    }


	    /*
	     * BOZO Stuff
	     */

	    if ( !(IS_IMMORTAL(ch) || ch->in_room == NULL || ch->in_room->area->empty) )
	    {
		CHAR_DATA *mob;
		CHAR_DATA *mob_next;

		for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
		{
			mob_next	= mob->next_in_room;

			if ( IS_NPC(mob) && IS_SET(ch->info, INFO_BOZO) && can_see( mob, ch ) )
			{
				/*
			 	* PC is 'ch', NPC is 'mob', Player is flagged BOZO and the NPC can see the PC
			 	*/

				switch( number_range( 1,10 ) )
				{
					default:	check_social( mob, "laugh", ch->name );	break;
					case 1:	check_social( mob, "laugh", ch->name );	break;
					case 2:	check_social( mob, "poke", ch->name );	break;
					case 3:	check_social( mob, "hug", ch->name );	break;
					case 4:	check_social( mob, "dance", ch->name );	break;
					case 5:	check_social( mob, "redeye", ch->name );	break;
					case 6:	check_social( mob, "slap", ch->name );	break;
					case 7:	check_social( mob, "glove", ch->name );	break;
					case 8:	multi_hit( mob, ch, TYPE_UNDEFINED );	break;
					case 9:
					{
						do_gossip( ch, "I am such a BOZO, oh yes I am!" );
						break;
					}
					case 10:	multi_hit( ch, mob, TYPE_UNDEFINED );	break;
				}

				if ( IS_SET(mob->comm, COMM_NOCHANNELS) )
					REMOVE_BIT(mob->comm, COMM_NOCHANNELS);

				sprintf( buf, "%s, %s",
					ch->name,
					insult_table[number_range(0,(MAX_INSULTS-2))].insult );

				switch(number_range(1,8))
				{
					default:	do_gossip(		mob,buf);	break;
					case 1:	do_say(		mob,buf);	break;
					case 2:	do_gossip(		mob,buf);	break;
					case 3:	do_flamechan(	mob,buf);	break;
					case 4:	do_yell(		mob,buf);	break;
					case 5:	do_say(		mob,buf);	break;
					case 6:	do_gossip(		mob,buf);	break;
					case 7:	do_flamechan(	mob,buf);	break;
					case 8:	do_flamechan(	mob,buf);	break;
				}
			}
		}
	    }

	    /*
           * Punishments, The Trident, and Ghosts
 	     */


	    /* Punishment Timers */
	    if ( is_punished(ch) )
	    {
		cnt = 0;
		while ( cnt < PUNISH_COUNT )
		{
			/*
			 * If the timer was orignally set to 0, then it is an indefinite punishment.
			 * So, we have to  check and see if it is above 0 before we decrement it.
			 * Once it's decremented, then we can see if it has expired and, if so
			 * we'll remove the effect.  We've defined each positionn in merc.h so we
			 * can use that value in a switch statement to tell us what to fix.
			 * We can also use our switch to make sure we have a valid punishment.
			 */

			if ( ch->pcdata->timer_punished[cnt] > 0 )
			{
				--ch->pcdata->timer_punished[cnt];

				switch(cnt)
				{
					default:	log_string( "Bad Autopunish Update" );	break;
					case PUNISH_NOCHAN:
					{
						if ( !IS_SET(ch->comm, COMM_NOCHANNELS) )
						{
							REMOVE_BIT(ch->comm, COMM_NOCHANNELS);
							ch->pcdata->timer_punished[PUNISH_NOCHAN] = 0;
						}

						if ( ch->pcdata->timer_punished[PUNISH_NOCHAN] == 0 )
						{
							REMOVE_BIT(ch->comm, COMM_NOCHANNELS);
							ch->pcdata->timer_punished[PUNISH_NOCHAN] = 0;
							send_to_char( "The gods have restored your channel priviliges.\n\r", ch );
							sprintf(buf, "PUNISH_NOCHAN timer for %s has expired : Channels have been restored", ch->name);
							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);
							log_string( buf );
						}
						break;
					}
					case PUNISH_NOEMOTE:
					{
						if ( !IS_SET(ch->comm, COMM_NOEMOTE) )
						{
							REMOVE_BIT(ch->comm, COMM_NOEMOTE);
							ch->pcdata->timer_punished[PUNISH_NOEMOTE] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_NOEMOTE] == 0 )

						{

							REMOVE_BIT(ch->comm, COMM_NOEMOTE);

							ch->pcdata->timer_punished[PUNISH_NOEMOTE] = 0;

							send_to_char( "The gods have restored your ability to emote.\n\r", ch );

							sprintf(buf, "PUNISH_NOEMOTE timer for %s has expired : Emotes have been restored", ch->name);

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_NOTELL:

					{

						if ( !IS_SET(ch->comm, COMM_NOTELL) )

						{

							REMOVE_BIT(ch->comm, COMM_NOTELL);

							ch->pcdata->timer_punished[PUNISH_NOTELL] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_NOTELL] == 0 )

						{

							REMOVE_BIT(ch->comm, COMM_NOTELL);

							ch->pcdata->timer_punished[PUNISH_NOTELL] = 0;

							send_to_char( "The gods have restored your ability to send tells.\n\r", ch );

							sprintf(buf, "PUNISH_NOTELL timer for %s has expired : Tells have been restored", ch->name);

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_NOSHOUT:

					{

						if ( !IS_SET(ch->comm, COMM_NOSHOUT) )

						{

							REMOVE_BIT(ch->comm, COMM_NOSHOUT);

							ch->pcdata->timer_punished[PUNISH_NOSHOUT] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_NOSHOUT] == 0 )

						{

							REMOVE_BIT(ch->comm, COMM_NOSHOUT);

							ch->pcdata->timer_punished[PUNISH_NOSHOUT] = 0;

							send_to_char( "The gods have restored your ability to shout.\n\r", ch );

							sprintf(buf, "PUNISH_NOSHOUT timer for %s has expired : Shouts have been restored", ch->name);

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_BOZO:

					{

						if ( !IS_SET(ch->info, INFO_BOZO) )

						{

							REMOVE_BIT(ch->info, INFO_BOZO);

							ch->pcdata->timer_punished[PUNISH_BOZO] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_BOZO] == 0 )

						{

							REMOVE_BIT(ch->info, INFO_BOZO);

							ch->pcdata->timer_punished[PUNISH_BOZO] = 0;

							send_to_char( "You are no longer a {R({GB{YO{GZ{YO{R){w.\n\r", ch );

							sprintf(buf, "PUNISH_BOZO timer for %s has expired : BOZO flag has been removed", ch->name);

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_WANTED:

					{

						if ( !IS_SET(ch->act, PLR_WANTED) )

						{

							REMOVE_BIT(ch->act, PLR_WANTED);

							ch->pcdata->timer_punished[PUNISH_WANTED] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_WANTED] == 0 )

						{

							REMOVE_BIT(ch->act, PLR_WANTED);

							ch->pcdata->timer_punished[PUNISH_WANTED] = 0;

							send_to_char( "You are no longer wanted.\n\r", ch );

							sprintf(buf, "PUNISH_WANTED timer for %s has expired : Wanted flag has been removed", ch->name);

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_FREEZE:

					{

						if ( !IS_SET(ch->act, PLR_FREEZE) )

						{

							REMOVE_BIT(ch->act, PLR_FREEZE);

							ch->pcdata->timer_punished[PUNISH_FREEZE] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_FREEZE] == 0 )

						{

							REMOVE_BIT(ch->act, PLR_FREEZE);

							ch->pcdata->timer_punished[PUNISH_FREEZE] = 0;

							send_to_char( "You have been thawed and may now play again.\n\r", ch );

							sprintf(buf, "PUNISH_FREEZE timer for %s has expired : Freeze flag has been removed", ch->name);

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_PRISON:

					{

						if ( IS_IMMORTAL(ch) )

							ch->pcdata->timer_punished[PUNISH_PRISON] = 0;



						if ( !is_in_prison(ch) )

						{

							char_from_room( ch );

							char_to_room( ch, get_room_index( clan_table[ch->clan].hall ) );

							ch->pcdata->timer_punished[PUNISH_PRISON] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_PRISON] == 0 )

						{

							ch->pcdata->timer_punished[PUNISH_PRISON] = 0;

							for ( d = descriptor_list; d; d = d->next )

							{

								if ( d->connected == CON_PLAYING

								&&   d->character->in_room == ch->in_room

							      &&   can_see(d->character, ch)

								&&   d->character != ch )

								{

									sprintf(buf, "%s is released from chains and vanishes in a flash of light.\n\r", ch->name );

									send_to_char( buf, d->character );

								}

							}

							char_from_room( ch );

							char_to_room( ch, get_room_index( clan_table[ch->clan].hall ) );

							for ( d = descriptor_list; d; d = d->next )

							{

								if ( d->connected == CON_PLAYING

								&&   d->character->in_room == ch->in_room

							      &&   can_see(d->character, ch)

								&&   d->character != ch )

								{

									sprintf(buf, "%s arrives from a flash of light released from their chains.\n\r", ch->name );

									send_to_char( buf, d->character );

								}

							}

							do_look( ch, "auto" );

							ch->position = POS_STANDING;

							sprintf(buf,"PUNISH_PRISON timer for %s has expired : %s has been released from Prison", ch->name, ch->name );

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );

						}

						break;

					}

					case PUNISH_EAT:

					{

						if ( IS_IMMORTAL(ch) )

							ch->pcdata->timer_punished[PUNISH_PRISON] = 0;



						if (	!(ch->in_room == get_room_index(ROOM_VNUM_IMMINT)) )

						{

							char_from_room( ch );

							char_to_room( ch, get_room_index( clan_table[ch->clan].hall ) );

							ch->pcdata->timer_punished[PUNISH_EAT] = 0;

						}



						if ( ch->pcdata->timer_punished[PUNISH_EAT] == 0 )

						{

							ch->pcdata->timer_punished[PUNISH_EAT] = 0;

							for ( d = descriptor_list; d; d = d->next )

							{

								if ( d->connected == CON_PLAYING

								&&   d->character->in_room == ch->in_room

							      &&   can_see(d->character, ch)

								&&   d->character != ch )

								{

									sprintf(buf, "%s is released from the {yImmortal{r Stomach{w.\n\r", ch->name );

									send_to_char( buf, d->character );

								}

							}

							char_from_room( ch );

							char_to_room( ch, get_room_index( clan_table[ch->clan].hall ) );

							for ( d = descriptor_list; d; d = d->next )

							{

								if ( d->connected == CON_PLAYING

								&&   d->character->in_room == ch->in_room

							      &&   can_see(d->character, ch)

								&&   d->character != ch )

								{

									sprintf(buf, "%s arrives from a flash of light released from the {yImmortal{r Stomach{w.\n\r", ch->name );

									send_to_char( buf, d->character );

								}

							}

							do_look( ch, "auto" );

							ch->position = POS_STANDING;

							sprintf(buf,"PUNISH_EAT timer for %s has expired : %s has been released from the Immortal Stomach", ch->name, ch->name );

							wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,0);

							log_string( buf );



						}

						break;

					}

				}

			}

			cnt++;

		}

	    }



	    if (IS_SET(ch->act, PLR_FREEZE) && ch->in_room != get_room_index( ROOM_VNUM_TROPHY ) )

	    {

		send_to_char( "You are frozen!\n\rYou will now be placed in the Immortal Trophy Room.\n\r", ch );

		act( "$n fades from existance!\n\rThey are frozen and will be placed in the Immortal Trophy Room.", ch, NULL, NULL, TO_ROOM );

		char_from_room( ch );

		char_to_room( ch, get_room_index( ROOM_VNUM_TROPHY ) );

		act( "$n has been frozen and added to the Immortal Trophy Room!", ch, NULL, NULL, TO_ROOM );

		do_look( ch, "auto" );

	    }



	    if ( global_trident && ch == global_trident_target )

          {

		ch->timer = 0;

		SET_BIT(ch->act, PLR_FREEZE);

		char_from_room( ch );

		char_to_room( ch, get_room_index( ROOM_VNUM_TROPHY ) );

		switch( global_trident_timer )

		{

			default:

				global_trident_target	= NULL;

				global_trident_room	= NULL;

				global_trident_timer 	= 0;

				global_trident		= FALSE;

				break;



			case 1:

				/* Initialize Launch */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "\n\r{RWARNING{w:{W User Targeting System Initialized...{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 2:

				/* Lock onto Target */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "\n\r{RUSER TARGETING SYSTEM{w: {gLocking onto target...{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 3:

				/* Announce Target Aquired */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "\n\r{RUSER TARGETING SYSTEM{w: {gTarget Aquired!{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 4:

				/* Launch */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "{RWARNING{w: {WTrident Missile Launching!{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 5:

				/* Missile in flight */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "{RFLIGHT CONTROL{w: {WTrident in flight...{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 6:

				/* Missile Closing in on Target */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "{RFLIGHT CONTROL{w: {WTrident targeting...{rLOCK{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 7:

				/* BOOM */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					send_to_char( "{D*{W*{G*{C*{Y*{B*{R*{b*{g*{c*{y*{r*{w* {rKA{D-{yBA{D-{cDA{D-{bBA{D-{gDA{D-{wBOOM *{r*{y*{c*{g*{b*{R*{B*{Y*{C*{G*{W*{D*{w\n\r", d->character );

				   }

				}

				++global_trident_timer;

				break;



			case 8:

				/* Hit Target and Destroy */

				for ( d = descriptor_list; d; d = d->next )

				{

				   if ( d->connected == CON_PLAYING )

				   {

					sprintf( buf, "{RNOTICE{w:{W %s{W has been hit with a Trident Nuclear Missile...{w\n\r",

						PERS(ch, d->character) );

					send_to_char( buf, d->character );

				   }

				}



				raw_kill( ch, TRUE );



				char_from_room( ch );

				char_to_room( ch, get_room_index( ROOM_VNUM_TROPHY ) );



				if ( (corpse = get_obj_here( ch, "corpse" )) != NULL )

				{

					extract_obj( corpse );

					for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )

					{

					    obj_next = obj->next_content;

					    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))

					      extract_obj( obj );

					}

					send_to_char( "Your corpse has been destroyed!\n\r", ch );

				}

				else

				{

					sprintf( buf, "CRITICAL : Bad Corpse in Trident Slay of %s", ch->name );

					log_string( buf );

				}



				if ( !IS_IMMORTAL(ch) )
				{
					send_to_char( "Your ghost rises from the ashes of your corpse...\n\r", ch );
					char_from_room( ch );
					char_to_room( ch, global_trident_room );
					act( "$n rises from the ashes of $s corpse.", ch, NULL, NULL, TO_ROOM );
					do_look( ch, "auto" );
					ch->pcdata->died_in_room = global_trident_room;
					ch->pcdata->timer_ghost /= 2;
					ch->bank /= 2;
					ch->mana = 1;
					ch->hit = 1;
					ch->move = 1;
					REMOVE_BIT(ch->act, PLR_FREEZE);
					save_char_obj( ch );
				}
				else
				{
					char_from_room( ch );
					char_to_room( ch, global_trident_room );
					do_look( ch, "auto" );
					REMOVE_BIT(ch->act, PLR_FREEZE);
					save_char_obj( ch );
				}
				global_trident_target	= NULL;
				global_trident_room	= NULL;
				global_trident_timer 	= 0;
				global_trident		= FALSE;
				break;
		}
          }

	    if (IS_SET(ch->info, INFO_GHOST))
	    {
		if ( ch->pcdata->died_in_room == NULL )
			ch->pcdata->died_in_room = get_room_index( ROOM_VNUM_PRISON );

		if ( ch->pcdata->timer_ghost <= 0 )
		{
			if ( ch->in_room != ch->pcdata->died_in_room )
			{
				send_to_char( "Your time to wander the mortal plane has expired!\n\r", ch );
				send_to_char( "You must now haunt the place of your death until you are reanimated...\n\r", ch );
				act( "$n fades from existance and must now haunt the place of $s death...", ch, NULL, NULL, TO_ROOM );
				char_from_room( ch );
				char_to_room( ch, ch->pcdata->died_in_room );
				act( "$n fades into existance as $e must now haunt this place where $e died...", ch, NULL, NULL, TO_ROOM );
				do_look( ch, "auto" );
			}
		}
		else
			--ch->pcdata->timer_ghost;
	    }
		//END TRIDENT CODE

	    /* OLD AGE - Insert Old Age Code here */


	    /** User Specific Tip Information
		* Tips are broken into the following groups:
		   * Newbies (level 1-20)
		   * Beginners (level 20-50)
		   * Intermediate (level 50-100)
		   * Remorts (remort levels 1-100)
		   * Experienced (level 100-200)
		   * Advanced (level 200+)
		* Tips use the gtip_count global variable to determine
		  when to send a new tip as set by PULSE_TIP in merc.h
		* Players may turn off the Tip channel which uses the INFO_NOTIPS flag
	    **/


	    if (		ch->level == 1
			||	ch->level == 20
			||	ch->level == 50
			||	ch->level == 100
			||	ch->level == 200 )
	    {
		//REMOVE_BIT(ch->info, INFO_NOTIPS);
	    }

	    if( 	gtip_count == PULSE_TIP

			&& !IS_SET(ch->info,INFO_NOTIPS)

			&& ch->fighting == NULL

			&& ch->was_in_room == NULL )

	    {

		char buf[MAX_STRING_LENGTH];



		if( (ch->level <=20) && !IS_REMORT(ch) )

			sprintf(buf, "{RTIP: {W%s{w\n\r", tip_table[number_range(0,3)].newbie );

		else if( (ch->level >20) && (ch->level <=50) && !IS_REMORT(ch) )

			sprintf(buf, "{RTIP: {W%s{w\n\r", tip_table[number_range(0,3)].beginner );

		else if( (ch->level >50) && (ch->level <=100) && !IS_REMORT(ch) )

			sprintf(buf, "{RTIP: {W%s{w\n\r", tip_table[number_range(0,3)].intermediate );

		else if( (ch->level <=100) && IS_REMORT(ch) )

			sprintf(buf, "{RTIP: {W%s{w\n\r", tip_table[number_range(0,3)].remort );

		else if( (ch->level >100) && (ch->level <=150) )

			sprintf(buf, "{RTIP: {W%s{w\n\r", tip_table[number_range(0,3)].experienced );

		else if( (ch->level >150) && (ch->level <=200) )

			sprintf(buf, "{RTIP: {W%s{w\n\r", tip_table[number_range(0,3)].advanced );

		else

		{

			sprintf(buf, "{RTIP: {WYou are level %d, don't you think by now you should know how to play?{w\n\r", ch->level );

			SET_BIT(ch->info, INFO_NOTIPS);

		}



		send_to_char(buf, ch);

	    }



	    update_pos( ch );



	    if ( ch->position == POS_DEAD )

		raw_kill( ch, FALSE );



	    if ( IS_IMMORTAL(ch) && IS_SET(ch->info, INFO_GHOST) )

		REMOVE_BIT(ch->info, INFO_GHOST);



	    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL

	    &&   obj->item_type == ITEM_LIGHT

	    &&   obj->value[2] > 0 )

	    {

		if ( --obj->value[2] == 0 && ch->in_room != NULL )

		{

		    --ch->in_room->light;

		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );

		    act( "$p flickers and goes out.", ch, obj, NULL, TO_CHAR );

		    extract_obj( obj );

		}

	 	else if ( obj->value[2] <= 5 && ch->in_room != NULL)

		    act("$p flickers.",ch,obj,NULL,TO_CHAR);

	    }



	    if  ( ch->timer >= 5

		    && ch->desc != NULL

		    && !IS_SET(ch->act, PLR_FREEZE)

		    && ch->was_in_room == NULL

		    && ch->level < LEVEL_HERO )

	    {

		ch->was_in_room = ch->in_room;

		if ( ch->fighting != NULL )

		   stop_fighting( ch, TRUE );

		act( "$n disappears into the void.",ch, NULL, NULL, TO_ROOM );

		send_to_char( "You disappear into the void.\n\r", ch );

		if (ch->level > 1)

		   save_char_obj( ch );

		char_from_room( ch );

		char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );

		act( "$n arrives here in the void.",ch, NULL, NULL, TO_ROOM );

	    }



	    if ( ch->was_in_room == NULL

	        && ch->fighting == NULL

	        && time_info.hour == 0 )

	    {

		do_new_note_notify( ch );

		do_clanrequest( ch, "xupdlst" );

	    }


	    gain_condition( ch, COND_DRUNK,  -1 );
	    gain_condition( ch, COND_FULL, ch->size > SIZE_MEDIUM ? -4 : -2 );
	    if (race_lookup("vampire") != ch->race)
		    gain_condition( ch, COND_THIRST, -1 );
	    gain_condition( ch, COND_HUNGER, ch->size > SIZE_MEDIUM ? -2 : -1);

	    if ( ch->fighting == NULL && IS_SET(ch->in_room->room_flags,ROOM_SUSTAIN))
	    {
		if (race_lookup("vampire") != ch->race)
			gain_condition( ch, COND_THIRST, 1  );
		gain_condition( ch, COND_HUNGER, ch->size > SIZE_MEDIUM ? 2 : 1 );
		gain_condition( ch, COND_FULL,   ch->size > SIZE_MEDIUM ? 4 : 2 );
	    }

	}

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;
		if (IS_AFFECTED2(ch, AFF_FORCE_TRANCE))
	    {
	    	if (paf->type == gsn_plague || 
	    		paf->type == gsn_poison ||
	    		paf->type == gsn_blindness ||
	    		paf->type == gsn_curse)
	    	{
	    		if (paf->duration > 0)
	    			paf->duration --;
	    		if (paf->duration > 0)
	    			paf->duration --;	
	    	}
	    }		
		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;  /* spell strength fades with time */
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
		    }
		}

		affect_remove( ch, paf );
	    }
	}



	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

		/*Sunlight Kills Vampires 
		 */
		if (race_lookup("vampire") == ch->race && IS_OUTSIDE(ch)
			&& weather_info.sunlight != SUN_DARK ) 
		{
			int dam;
			
			dam = .70*ch->max_hit;
			if(time_info.hour<12)
			{
				send_to_char("As the light of the morning sun hits you, your flesh begins to sear and {Dsmoke{w.\n\r",ch);
				send_to_char("As you burst into {rflames{w you are overcome by the nauseating stench of your own searing flesh.\n\r",ch);
				act("As the morning sun hits $N, $S skin blackens and $E bursts into {rflames{w.",ch,NULL,ch,TO_NOTVICT);			
			}	
			else
			{
				send_to_char("The hot afternoon sun warms your skin and it begins to sear and smoke.\n\r",ch);
				send_to_char("As you burst into {rflames{w you are overcome by the nauseating stench of your own searing flesh.\n\r",ch);	
				act("As the afternoon sun hits $N, $S skin blackens and $E bursts into {rflames{w.",ch,NULL,ch,TO_NOTVICT);			
			}
			damage(ch,ch,dam,TYPE_SUNLIGHT,DAM_SUNLIGHT,TRUE, FALSE); /* Grayson -- Parryable/Vampire 4 May 2003 */	
		}
		
        if (is_affected(ch, gsn_plague) && ch != NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;
            int dam;

		    if (ch->in_room == NULL)
			return;
	
	    if (ch->level >= LEVEL_IMMORTAL)
	    {
			REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
			send_to_char( "The plague sores vanish!.\n\r",ch);
			act("$n's plague sores vanish!",ch,NULL,NULL,TO_ROOM);
			return;
	    }

	    act("$n writhes in agony as plague sores erupt from $s skin.",
		ch,NULL,NULL,TO_ROOM);
	    send_to_char("You writhe in agony from the plague.\n\r",ch);
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

            if (af->level == 1)
            	return;

	    	plague.where	= TO_AFFECTS;
            plague.type 	= gsn_plague;
            plague.level 	= af->level - 1;
            plague.duration 	= number_range(1,2 * plague.level);
            plague.location	= APPLY_STR;
            plague.modifier 	= -5;
            plague.bitvector 	= AFF_PLAGUE;

            for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level - 2,vch,DAM_DISEASE)
			&&  !IS_IMMORTAL(vch)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(4) == 0)
            	{
            	    send_to_char("You feel hot and feverish.\n\r",vch);
            	    act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }

	    dam = UMIN(ch->level,af->level/5+1);
	    ch->mana -= dam;
	    ch->move -= dam;
	    damage_old( ch, ch, dam, gsn_plague,DAM_DISEASE,FALSE);
        }
	else if ( IS_AFFECTED(ch, AFF_POISON) && ch != NULL && !IS_AFFECTED(ch,AFF_SLOW))

	{
	    AFFECT_DATA *poison;

	    poison = affect_find(ch->affected,gsn_poison);

	    if (ch->level >= LEVEL_IMMORTAL)
	    {
		REMOVE_BIT(ch->affected_by,AFF_POISON);
		send_to_char( "The poison is removed from your system!.\n\r",ch);
		return;
	    }

	    if (poison != NULL)
	    {
	        act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	        send_to_char( "You shiver and suffer.\n\r", ch );
	        damage_old(ch,ch,poison->level/10 + 1,gsn_poison,
		    DAM_POISON,FALSE);
	    }
	}

	else if ( ch->position == POS_INCAP && number_range(0,1) == 0)
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE, FALSE); /* Grayson - Parryable 5 May 2003 */
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE, FALSE); /* Grayson - Parryable 5 May 2003 */
	}
    }

	/*
	 * Attribute check and resets
	 */

	/* Check Descriptor Data */
	if ( !is_linkdead )
	{
		if ( ch->desc->host == NULL )
		{

			ch->desc->host = str_dup( "(unknown)" );

			sprintf( buf, "%s had a null HOST value in a valid descriptor", ch->name );

			bug( buf, 0 );

		}



		if ( ch->desc->ipaddy == NULL )

		{

			ch->desc->ipaddy = str_dup( "(unknown)" );

			sprintf( buf, "%s had a null IPADDY value in a valid descriptor", ch->name );

			bug( buf, 0 );

		}

	}





	/*

	 * Check for duplicate IP Addresses

	 */

	if ( time_info.hour == 12 )

	{

		check_duplicate_ipaddy( FALSE );

	}





    /*

     * Autosave and autoquit.

     * Check that these chars still exist.

     */

    for ( ch = char_list; ch != NULL; ch = ch_next )

    {
    //If a player is auto-logged off and his pet is the next ch_next, player-characters in the freed-characterslist might be saved with invalid strings in their playerfiles.
        if (!IS_VALID(ch))
        {
            bug("update_char: Trying to work with an invalidated character.\n",0);
           break;
        }
        ch_next = ch->next;

	if ( !IS_NPC(ch) )
	{
		if (!is_linkdead(ch) && ch->desc->descriptor % 30 == save_number)
			save_char_obj(ch);

		if ( !IS_IMMORTAL(ch) && (ch->timer >= 15) && !(IS_SET(ch->info, INFO_NOIDLE)||IS_SET(ch->info, INFO_IMMMULTI)) )
            	do_quit( ch, "" );

		if ( is_linkdead(ch) && ch->timer >= 5 )
			do_quit( ch, "" );
	}
    }

	/*
	 * Random Global Changes
	 */


	/*
	 * Memory Debugging & Notification
	 */
	if (string_count < nAllocString) /* Strings Increased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gIncrease in strings in char_update {w:: {r%d {w: {r%d {w",
				string_count, nAllocString);
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,0);
		sprintf(buf,
			"MEMCHECK : Increase in strings in char_update :: %d : %d ",
				string_count, nAllocString);
		log_string ( buf );
	}

	if (perm_count < nAllocPerm) /* Perms Increased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gIncrease in perms in char_update {w:: {r%d {w: {r%d {w",
				perm_count, nAllocPerm);
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,0);
		sprintf(buf,
			"MEMCHECK : Increase in perms in char_update :: %d : %d ",
				perm_count, nAllocPerm);
		log_string ( buf );
	}

	if (string_count > nAllocString) /* Strings Decreased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gDecrease in strings in char_update {w:: {r%d {w: {r%d {w",
				string_count, nAllocString);
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,0);
		sprintf(buf,
			"MEMCHECK : Decrease in strings in char_update :: %d : %d ",
				string_count, nAllocString);
		log_string ( buf );
	}

	if (perm_count > nAllocPerm) /* Perms Decreased */
	{
		sprintf(buf,
			"{RMEMCHECK {w: {gDecrease in perms in char_update {w:: {r%d {w: {r%d {w",
				perm_count, nAllocPerm);
		wiznet(buf, NULL, NULL, WIZ_DEBUG_MEMORY,0,0);
		sprintf(buf,
			"MEMCHECK : Decrease in perms in char_update :: %d : %d ",
				perm_count, nAllocPerm);
		log_string ( buf );
	}

    return;
}









/*

 * Update all objs.

 * This function is performance sensitive.

 */

void obj_update( void )

{

    OBJ_DATA *obj;

    OBJ_DATA *obj_next;

    AFFECT_DATA *paf, *paf_next;



    for ( obj = object_list; obj != NULL; obj = obj_next )

    {

	CHAR_DATA *rch;

	char *message;



	obj_next = obj->next;



	/* go through affects and decrement */

        for ( paf = obj->affected; paf != NULL; paf = paf_next )

        {

            paf_next    = paf->next;

            if ( paf->duration > 0 )

            {

                paf->duration--;

                if (number_range(0,4) == 0 && paf->level > 0)

                  paf->level--;  /* spell strength fades with time */

            }

            else if ( paf->duration < 0 )

                ;

            else

            {

                if ( paf_next == NULL

                ||   paf_next->type != paf->type

                ||   paf_next->duration > 0 )

                {

                    if ( paf->type > 0 && skill_table[paf->type].msg_obj )

                    {

			if (obj->carried_by != NULL)

			{

			    rch = obj->carried_by;

			    act(skill_table[paf->type].msg_obj,

				rch,obj,NULL,TO_CHAR);

			}

			if (obj->in_room != NULL

			&& obj->in_room->people != NULL)

			{

			    rch = obj->in_room->people;

			    act(skill_table[paf->type].msg_obj,

				rch,obj,NULL,TO_ALL);

			}

                    }

                }



                affect_remove_obj( obj, paf );

            }

        }





	if ( obj->timer <= 0 || --obj->timer > 0 )

	    continue;



	switch ( obj->item_type )

	{

	default:              message = "$p crumbles into dust.";  break;

	case ITEM_FOUNTAIN:   message = "$p dries up.";         break;

	case ITEM_CORPSE_NPC: message = "$p decays into dust."; break;

	case ITEM_CORPSE_PC:  message = "$p decays into dust."; break;

	case ITEM_FOOD:       message = "$p decomposes.";	break;

	case ITEM_POTION:     message = "$p has evaporated from disuse.";

								break;

	case ITEM_PORTAL:     message = "$p fades out of existence."; break;

	case ITEM_CONTAINER:

	    if (CAN_WEAR(obj,ITEM_WEAR_FLOAT))

		if (obj->contains)

		    message =

		"$p flickers and vanishes, spilling its contents on the floor.";

		else

		    message = "$p flickers and vanishes.";

	    else

		message = "$p crumbles into dust.";

	    break;

	}



	if ( obj->carried_by != NULL )

	{

	    if (IS_NPC(obj->carried_by)

	    &&  obj->carried_by->pIndexData->pShop != NULL)

		obj->carried_by->silver += obj->cost/5;

	    else

	    {

	    	act( message, obj->carried_by, obj, NULL, TO_CHAR );

		if ( obj->wear_loc == WEAR_FLOAT)

		    act(message,obj->carried_by,obj,NULL,TO_ROOM);

	    }

	}

	else if ( obj->in_room != NULL

	&&      ( rch = obj->in_room->people ) != NULL )

	{

	    if (! (obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT

	           && !CAN_WEAR(obj->in_obj,ITEM_TAKE)))

	    {

	    	act( message, rch, obj, NULL, TO_ROOM );

	    	act( message, rch, obj, NULL, TO_CHAR );

	    }

	}



        if ((obj->item_type == ITEM_CORPSE_PC || obj->wear_loc == WEAR_FLOAT)

	&&  obj->contains)

	{   /* save the contents */

     	    OBJ_DATA *t_obj, *next_obj;



	    for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)

	    {

		next_obj = t_obj->next_content;

		obj_from_obj(t_obj);



		if (obj->in_obj) /* in another object */

		    obj_to_obj(t_obj,obj->in_obj);



		else if (obj->carried_by)  /* carried */

		    if (obj->wear_loc == WEAR_FLOAT)

			if (obj->carried_by->in_room == NULL)

			    extract_obj(t_obj);

			else

			    obj_to_room(t_obj,obj->carried_by->in_room);

		    else

		    	obj_to_char(t_obj,obj->carried_by);



		else if (obj->in_room == NULL)  /* destroy it */

		    extract_obj(t_obj);



		else /* to a room */

		    obj_to_room(t_obj,obj->in_room);

	    }

	}



	extract_obj( obj );

    }



    return;

}







/*

 * Aggress.

 *

 * for each mortal PC

 *     for each mob in room

 *         aggress on some random PC

 *

 * This function takes 25% to 35% of ALL Merc cpu time.

 * Unfortunately, checking on each PC move is too tricky,

 *   because we don't the mob to just attack the first PC

 *   who leads the party into the room.

 *

 * -- Furey

 */

void aggr_update( void )

{

    CHAR_DATA *wch;

    CHAR_DATA *wch_next;

    CHAR_DATA *ch;

    CHAR_DATA *ch_next;

    CHAR_DATA *vch;

    CHAR_DATA *vch_next;

    CHAR_DATA *victim;





    for ( wch = char_list; wch != NULL; wch = wch_next )

    {

	wch_next = wch->next;

	if ( IS_NPC(wch)

	||   wch->level >= LEVEL_IMMORTAL

	||   wch->in_room == NULL

	||   wch->in_room->area->empty)

	    continue;



	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )

	{

	    int count;



	    ch_next	= ch->next_in_room;



	    if ( !IS_NPC(ch)

	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)

	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)

	    ||   IS_AFFECTED(ch,AFF_CALM)

	    ||   ch->fighting != NULL

	    ||   IS_AFFECTED(ch, AFF_CHARM)

	    ||   !IS_AWAKE(ch)

	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )

	    ||   !can_see( ch, wch )

	    ||   number_bits(1) == 0)

		continue;



	    /*

	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.

	     * Now make the aggressor fight a RANDOM pc victim in the room,

	     *   giving each 'vch' an equal chance of selection.

	     */

	    count	= 0;

	    victim	= NULL;

	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )

	    {

		vch_next = vch->next_in_room;



		if ( !IS_NPC(vch)

		&&   vch->level < LEVEL_IMMORTAL

		&&   ch->level >= vch->level - 5

		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )

		&&   can_see( ch, vch ) )

		{

		    if ( number_range( 0, count ) == 0 )

			victim = vch;

		    count++;

		}

	    }



	    if ( victim == NULL )

		continue;



	    multi_hit( ch, victim, TYPE_UNDEFINED );

	}

    }



    return;

}





/*

 * Auto Restart Handler

 */

void autodown_update( void )

{

	extern	bool	autodown;

	extern	bool	autodown_reboot;

	extern	int	autodown_timer;

	extern 	bool 	merc_down;



	char 			buf[MAX_STRING_LENGTH];

	DESCRIPTOR_DATA 	*d,*d_next;

	CHAR_DATA 		*vch;

	FILE 			*fp;



	--autodown_timer;



	for ( d = descriptor_list; d; d = d->next )

	{

		if ( d->connected == CON_PLAYING )

		{

			if ( autodown_reboot )

				sprintf( buf, "\n\r\n\r{RNOTICE{W: {cMagrathea{w will automatically {rREBOOT{w in {g%d{w minutes.\n\r\n\r", autodown_timer );

			else

				sprintf( buf, "\n\r\n\r{RNOTICE{W: {cMagrathea{w will automatically {rSHUTDOWN{w in {g%d{w minutes.\n\r\n\r", autodown_timer );



			send_to_char( buf, d->character );

			send_to_char( "{w\n\r",   d->character );



			if ( autodown_timer == 1 )

				send_to_char( "{gPlease Exit {cMagrathea{g now...{w\n\r\n\r", d->character );

		}

	}



	if ( autodown_timer <= 0 )

	{

		if ( autodown_reboot )

			log_string( "AUTORESTART : Initiated : REBOOT" );

		else

			log_string( "AUTORESTART : Initiated : SHUTDOWN" );



		tablefiles_update( FALSE, NULL, TFU_SAVE, FALSE, TRUE, TFU_ALL );



		for ( d = descriptor_list; d; d = d->next )

		{

			if ( d->connected == CON_PLAYING )

			{

				if ( autodown_reboot )

					send_to_char( "\n\r\n\r{RNOTICE{W: {wAutomatic {RREBOOT{w of {cMagrathea{w!\n\r\n\r", d->character );

				else

					send_to_char( "\n\r\n\r{RNOTICE{W: {wAutomatic {RSHUTDOWN{w of {cMagrathea{w!\n\r\n\r", d->character );

			}

		}



		if ( !autodown_reboot )

		{

			fclose( fpReserve );

			if ( ( fp = fopen( SHUTDOWN_FILE, "a" ) ) == NULL )

			{

				sprintf( buf, "AUTOSHUTDOWN unable to open %s", SHUTDOWN_FILE );

				log_string( buf );

			}

			else

			{

				fprintf( fp, "[0] AUTOSHUTDOWN: AUTOSUTDOWN\n" );

				fclose( fp );

			}

			fpReserve = fopen( NULL_FILE, "r" );

		}



		merc_down = TRUE;

		log_string( "GLOBAL VARIABLE : MERC_DOWN set to TRUE" );



		for ( d = descriptor_list; d != NULL; d = d_next )

		{

			d_next = d->next;

			vch = d->original ? d->original : d->character;

			if (vch != NULL)

				save_char_obj(vch);

			close_socket(d);

		}

	}

	return;

}







/*

 * Handle all kinds of updates.

 * Called once per pulse from game loop.

 * Random times to defeat tick-timing clients and players.

 */



void update_handler( void )

{

    static  int     pulse_area;

    static  int     pulse_mobile;

    static  int     pulse_violence;

    static  int     pulse_point;

    static  int	  pulse_music;

    static  int	  pulse_webwho;

    extern  int     wiztime_count;

    extern	bool	  autodown;

    char buf[MAX_STRING_LENGTH];



    if ( --pulse_area     <= 0 )

    {

	pulse_area	= PULSE_AREA;

	/* number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 ); */

	area_update	( );

        quest_update     ( );

    }



    if ( --pulse_music	  <= 0 )

    {

	pulse_music	= PULSE_MUSIC;

	song_update();

    }



    if ( --pulse_webwho   <= 0 )

    {

	pulse_webwho = PULSE_WEBWHO;

	who_html_update();

    }



    if ( --pulse_mobile   <= 0 )

    {

	pulse_mobile	= PULSE_MOBILE;

	mobile_update	( );

    }



    if ( --pulse_violence <= 0 )

    {

	pulse_violence	= PULSE_VIOLENCE;

	violence_update	( );

    }



    if ( --pulse_point    <= 0 )

    {

	wiznet("TICK!",NULL,NULL,WIZ_TICKS,0,0);

	pulse_point     = PULSE_TICK;

	number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 );

	weather_update	( );

	char_update	( );

	obj_update	( );

	if ( wiztime_count++ >= get_econ_value( "wiz_time_count" ) )

	{

		sprintf(buf, "{r%s {w: {c%s{w", TZ_NAME, ((char *) ctime( &current_time )));

		wiznet(buf,NULL,NULL,WIZ_TIME,0,0);

		wiztime_count 	= 0;

	}

	if ( autodown )

		autodown_update( );

    }



    aggr_update( );

    tail_chain( );

    return;

}

