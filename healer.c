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

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"

/* Local Functions */
long calculate_cost	args( ( CHAR_DATA *ch, CHAR_DATA *mob, long base ) );


long calculate_cost( CHAR_DATA *ch, CHAR_DATA *mob, long base )
{
	long surcharge;
	long cost;
	int hlrmult;


	if ( IS_NPC(ch) )
	{
		return 0;
	}

	if ( ch->level < 10 )
	{
		return base;
	}

	if ( IS_HERO(ch) )
	{
		return base;
	}


	surcharge	= 0;
	cost		= 0;
	hlrmult	= 0;


	if ( mob->level <= 100 )
	{
		hlrmult	= 1;
	}
	else if ( mob->level > 100 && mob->level <= 200 )
	{
		hlrmult	= 2;
	}
	else if ( mob->level > 200 && mob->level <= 300 )
	{
		hlrmult	= 3;
	}
	else if ( mob->level > 300 && mob->level <= 400 )
	{
		hlrmult	= 4;
	}
	else if ( mob->level > 400 && mob->level <= 500 )
	{
		hlrmult	= 5;
	}
	else if ( mob->level > 500 && mob->level <= 600 )
	{
		hlrmult	= 6;
	}
	else if ( mob->level > 600 && mob->level <= 700 )
	{
		hlrmult	= 7;
	}
	else if ( mob->level > 700 && mob->level <= 800 )
	{
		hlrmult	= 8;
	}
	else if ( mob->level > 800 && mob->level <= 900 )
	{
		hlrmult	= 9;
	}
	else
	{
		hlrmult	= 10;
	}



	cost 		= base;

	surcharge 	= ch->level;

	surcharge	= surcharge * get_econ_value("healer_percentage");

	surcharge	= surcharge / 100;

	surcharge	= surcharge * cost;

	cost		= cost + surcharge;

	cost		= cost * hlrmult;



	return cost;

}





void do_heal(CHAR_DATA *ch, char *argument)

{

    CHAR_DATA *mob;

    char arg[MAX_INPUT_LENGTH];

    long cost;

    int sn;

    SPELL_FUN *spell;

    char *words;

    char buf[MAX_STRING_LENGTH];

    bool hmana, hrestore;



    hmana		= FALSE;

    hrestore	= FALSE;

    cost		= 0;



    /* check for healer */

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )

    {

        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER) )

            break;

    }

 

    if ( mob == NULL )

    {

        send_to_char( "You can't do that here.\n\r", ch );

        return;

    }



    one_argument(argument,arg);



    if (arg[0] == '\0')

    {

	/* display cost list */

	act("$N says 'For you, I offer the following spells and rates:'",ch,NULL,mob,TO_CHAR);



	/* light */

	cost 		= get_econ_value("healer_light");

	sprintf(buf, "  light: cure light wounds       gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* serious */

	cost 		= get_econ_value("healer_serious");

	sprintf(buf, "  serious: cure serious wounds   gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* critical */

	cost 		= get_econ_value("healer_critical");

	sprintf(buf, "  critic: cure critical wounds   gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* renew */

	cost 		= get_econ_value("healer_renew");

	sprintf(buf, "  renew: a full renewal          gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* blind */

	cost 		= get_econ_value("healer_blind");

	sprintf(buf, "  blind: cure blindness          gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* disease */

	cost 		= get_econ_value("healer_disease");

	sprintf(buf, "  disease: cure disease          gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* poison */

	cost 		= get_econ_value("healer_poison");

	sprintf(buf, "  poison:  cure poison           gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* uncurse */

	cost 		= get_econ_value("healer_uncurse");

	sprintf(buf, "  uncurse: remove curse          gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* movement */

	cost 		= get_econ_value("healer_refresh");

	sprintf(buf, "  refresh: restore movement      gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* mana */

	cost 		= get_econ_value("healer_mana");

	sprintf(buf, "  mana: restore mana             gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	/* restore */

	cost 		= get_econ_value("healer_restore");

	sprintf(buf, "  restore: full restore          gold: %ld\n\r", calculate_cost(ch, mob, cost));

	send_to_char( buf, ch );



	send_to_char(" Type heal <type> to be healed.\n\r",ch);

	return;

    }



    if (!str_prefix(arg,"light"))

    {

      spell 	= spell_cure_light;

	sn    	= skill_lookup("cure light");

	words 	= "judicandus dies";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_light"));

    }



    else if (!str_prefix(arg,"restore"))

    {

	spell		= NULL;

	sn		= -1;

	words		= "restore";

	hrestore	= TRUE;

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_restore"));

    }



    else if (!str_prefix(arg,"serious"))

    {

	spell 	= spell_cure_serious;

	sn    	= skill_lookup("cure serious");

	words 	= "judicandus gzfuajg";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_serious"));

    }



    else if (!str_prefix(arg,"critical"))

    {

	spell 	= spell_cure_critical;

	sn    	= skill_lookup("cure critical");

	words 	= "judicandus qfuhuqar";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_critical"));

    }



    else if (!str_prefix(arg,"renew"))

    {

	spell 	= spell_heal;

	sn 		= skill_lookup("renew");

	words 	= "pzar";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_renew"));

    }



    else if (!str_prefix(arg,"blindness"))

    {

	spell 	= spell_cure_blindness;

	sn    	= skill_lookup("cure blindness");

      words 	= "judicandus noselacri";		

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_blind"));

    }



    else if (!str_prefix(arg,"disease"))

    {

	spell 	= spell_cure_disease;

	sn    	= skill_lookup("cure disease");

	words 	= "judicandus eugzagz";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_disease"));

    }



    else if (!str_prefix(arg,"poison"))

    {

	spell 	= spell_cure_poison;

	sn    	= skill_lookup("cure poison");

	words 	= "judicandus sausabru";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_poison"));

    }

	

    else if (!str_prefix(arg,"uncurse") || !str_prefix(arg,"curse"))

    {

	spell 	= spell_remove_curse; 

	sn    	= skill_lookup("remove curse");

	words 	= "candussido judifgz";

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_curse"));

    }



    else if (!str_prefix(arg,"mana") || !str_prefix(arg,"energize"))

    {

      spell 	= NULL;

      sn 		= -1;

      words 	= "energizer";

	hmana 	= TRUE;

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_mana"));

    }



	

    else if (!str_prefix(arg,"refresh") || !str_prefix(arg,"moves"))

    {

	spell 	= spell_refresh;

	sn    	= skill_lookup("refresh");

	words 	= "candusima"; 

	cost 		= calculate_cost(ch, mob, get_econ_value("healer_refresh"));

    }



    else 

    {

	act("$N says 'Type 'heal' for a list of spells.'",

	    ch,NULL,mob,TO_CHAR);

	return;

    }



    cost = cost * 100;



    if (cost > (ch->gold * 100 + ch->silver))

    {

	act("$N says 'You do not have enough gold for my services.'",

	    ch,NULL,mob,TO_CHAR);

	return;

    }



    WAIT_STATE(ch,PULSE_VIOLENCE);



    deduct_long_cost(ch,cost);

    mob->gold += cost;

    act("$n utters the words '$T'.",mob,NULL,words,TO_ROOM);

  

    if (hmana)

    {

	ch->mana += dice(2,8) + mob->level / 3;

	ch->mana = UMIN(ch->mana,ch->max_mana);

	send_to_char("A warm glow passes through you.\n\r",ch);

	return;

    }



    if (hrestore)

    {

	do_restore( mob, ch->name );

	return;

    }



     if (hmana || hrestore)

	return;

    

     spell(sn,mob->level,mob,ch,TARGET_CHAR);



     return;

}



/* ###### URK_CYBER ###### */



void do_cyber(CHAR_DATA *ch, char *argument)
{
	    CHAR_DATA *mob;
	        char arg[MAX_INPUT_LENGTH];
		    char buf[MAX_STRING_LENGTH];
		        int cost;

			    /* check for surgeon */
			    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
		        {
			        if ( IS_NPC(mob) && IS_SET(mob->cyber, CYBER_SURGEON) )
		            break;
			    }

		        /* if there are no surgeon, display the characters enhancements*/ 
		        if ( mob == NULL )
			    {
			        sprintf(buf, "Cybernetic Enhancements:\n\r %s\n\r", 
					cyber_bit_name(ch->cyber));
				    send_to_char(buf,ch);
				    return;
			    }

			    one_argument(argument,arg);

				    	/* if there are a surgeon, give a list*/
		        if (arg[0] == '\0')
				    {
				            /* display price list */
				    	act("{g$N says 'I have these parts in stock:'{w",ch,NULL,mob,TO_CHAR);
				    	   cost = get_econ_value("cyber_cost_comm");				    	
                           sprintf(buf, "{g  comm socket      : Internal Comm Unit      %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch ); 
				    	   cost = get_econ_value("cyber_cost_optic");				    	
                           sprintf(buf, "{g  optical implant  : Enhanced Optics         %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch ); 
				    	   cost = get_econ_value("cyber_cost_leg");				    	
                           sprintf(buf, "{g  leg implant      : Enhanced Mobility       %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch );
				    	   cost = get_econ_value("cyber_cost_chest");				    	
                           sprintf(buf, "{g  chest implant    : Embeded Armor           %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch ); 
				    	   cost = get_econ_value("cyber_cost_reflex");				    	
                           sprintf(buf, "{g  reflex implant   : Enhanced Reflexes       %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch ); 
				    	   cost = get_econ_value("cyber_cost_proc");				    	
                           sprintf(buf, "{g  processor socket : Internal Computer       %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch ); 
				    	   cost = get_econ_value("cyber_cost_str");				    	
                           sprintf(buf, "{g  strength implant : Enhanced Strength       %ld creds{w\n\r", calculate_cost(ch, mob, cost));
                           send_to_char( buf, ch );                                                                                                           
						send_to_char("{g Type cyber <type> to buy one, or help cyber to get more info.{w\n\r",ch);
						return;
				    }

				    /* Lets see what the character wants to have */
				    if (!str_prefix(arg,"comm"))
			        {
						cost  = get_econ_value("cyber_cost_comm");
					    if (cost > (ch->gold))
				        {
							act("{g$N says 'You do not have enough creds for my services.'{w",
						    ch,NULL,mob,TO_CHAR);
							return;
					    }
				    	if (IS_SET(ch->cyber,CYBER_COMM))
						{
							send_to_char( "You already have that enhancement.\n\r", ch );
							return;
						}
						ch->gold -= cost;
					    SET_BIT (ch->cyber, CYBER_COMM );
				        /* add benefit here */
			        }

			        else if (!str_prefix(arg,"optical"))
				    {
				        cost  = get_econ_value("cyber_cost_optic");
					    if (cost > (ch->gold))
				        {
							act("{g$N says 'You do not have enough creds for my services.'{w",
						    ch,NULL,mob,TO_CHAR);
    						return;
					    }
				    	if (IS_SET(ch->cyber,CYBER_EYES))
						{
							send_to_char( "You already have that enhancement.\n\r", ch );
							return;
						}
						ch->gold -= cost;
					    SET_BIT (ch->cyber, CYBER_EYES );	
				       /* add benefit here */
			        }

				    else if (!str_prefix(arg,"legs"))
			        {
						cost = get_econ_value("cyber_cost_leg");
					    if (cost > (ch->gold))
				        {
							act("{g$N says 'You do not have enough creds for my services.'{w",
						    ch,NULL,mob,TO_CHAR);
							return;
					    }
				    	if (IS_SET(ch->cyber,CYBER_LEGS))
						{
							send_to_char( "You already have that enhancement.\n\r", ch );
							return;
						}
						ch->gold -= cost;
					    SET_BIT (ch->cyber, CYBER_LEGS );
				       /* add benefit here */
			    	}

			        else if (!str_prefix(arg,"chest"))
				    {
				    	cost  = get_econ_value("cyber_cost_chest");
					    if (cost > (ch->gold))
				        {
							act("{g$N says 'You do not have enough creds for my services.'{w",
						    ch,NULL,mob,TO_CHAR);
							return;
					    }
				    	if (IS_SET(ch->cyber,CYBER_CHEST))
						{
							send_to_char( "You already have that enhancement.\n\r", ch );
							return;
						}
						ch->gold -= cost;
					    SET_BIT (ch->cyber, CYBER_CHEST );
				       /* add benefit here */
			        }
				    else if (!str_prefix(arg,"reflex"))
			        {
					    cost = get_econ_value("cyber_cost_reflex");
				        if (cost > (ch->gold))
					    {
					    	act("{g$N says 'You do not have enough creds for my services.'{w",
						    ch,NULL,mob,TO_CHAR);
							return;
					    }
						if (IS_SET(ch->cyber,CYBER_REFLEXES))
						{
							send_to_char( "You already have that enhancement.\n\r", ch );
							return;
						}
						ch->gold -= cost;
					    SET_BIT (ch->cyber, CYBER_REFLEXES );
				       /* add benefit here */
			        }

        		        else if (!str_prefix(arg,"processor"))
					    {
    				        cost = get_econ_value("cyber_cost_proc");
						    if (cost > (ch->gold))
					        {
								act("{g$N says 'You do not have enough creds for my services.'{w",
							    ch,NULL,mob,TO_CHAR);
								return;
						    }
					    	if (IS_SET(ch->cyber,CYBER_MIND))
							{
								send_to_char( "You already have that enhancement.\n\r", ch );
								return;
							}
							ch->gold -= cost;
						    SET_BIT (ch->cyber, CYBER_MIND );
					       /* add benefit here */
				        }
							
					    else if (!str_prefix(arg,"strength"))
				        {
							cost = get_econ_value("cyber_cost_str");
						    if (cost > (ch->gold))
					        {
								act("{g$N says 'You do not have enough creds for my services.'{w",
							    ch,NULL,mob,TO_CHAR);
								return;
						    }
					    	if (IS_SET(ch->cyber,CYBER_STRENGTH))
							{
								send_to_char( "You already have that enhancement.\n\r", ch );
								return;
							}
							ch->gold -= cost;
						    SET_BIT (ch->cyber, CYBER_STRENGTH );
					       /* add benefit here */
				        }
				        else 
        			    {
					    	act("{g$N says 'Type 'cyber' for a list of cybernetics.'{w",
						    ch,NULL,mob,TO_CHAR);
							return;
					    }
			    WAIT_STATE(ch,PULSE_VIOLENCE);
		        //deduct_cost(ch,cost);
			    act("{g$n says 'There we go, better than the original'{w.",mob,NULL,NULL,TO_ROOM); 

}



