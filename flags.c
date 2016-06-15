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

#include <time.h>

#else

#include <sys/types.h>

#include <sys/time.h>

#endif

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "merc.h"

#include "tables.h"



int flag_lookup args( ( const char *name, const struct flag_type *flag_table) );



void do_hack( CHAR_DATA *ch, char *argument )

{    OBJ_DATA *term;

     long *flag;

     char arg1[MAX_INPUT_LENGTH];

     char arg2[MAX_INPUT_LENGTH];

     CHAR_DATA *victim;

     const struct flag_type *flag_table;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    for (term = ch->in_room->contents; term != NULL; term = term->next_content)

	if (term->item_type == ITEM_TERMINAL && can_see_obj(ch,term))

	    break;



    for (term = object_list; term != NULL; term = term->next)

    {

	if (term->item_type != ITEM_TERMINAL || term->value[1] < 0)

	    continue;



    	if ( !str_cmp( arg2, "wanted" ) )

    	{

		if ( victim = (get_char_world(ch,arg1)) )

		{

	    	flag = &victim->act;

	    	flag_table = plr_flags;

    		SET_BIT(victim->act, PLR_WANTED);

		send_to_char("Hack complete.\n\r",ch);

		return;

		}

	

    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    	{

	send_to_char( "They aren't here.\n\r", ch );

	return;

    	}



    	if ( IS_NPC(victim) )

    	{

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    	}



    	if ( arg1[0] == '\0' || arg2[0] == '\0' )

    	{

	send_to_char( "Syntax: hack <character> <wanted>.\n\r", ch );

	return;

    	}



	}



    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    	{

	send_to_char( "They aren't here.\n\r", ch );

	return;

    	}



    	if ( IS_NPC(victim) )

    	{

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    	}



    	if ( arg1[0] == '\0' || arg2[0] == '\0' )

    	{

	send_to_char( "Syntax: hack <character> <wanted>.\n\r", ch );

	return;

    	}

    }



		if (term == NULL)

   		{

		send_to_char("You see nothing to hack.\n\r",ch);

		return;

    		}



}



void do_flag(CHAR_DATA *ch, char *argument)

{

    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH],arg3[MAX_INPUT_LENGTH];

    char word[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    long *flag, old = 0, new = 0, marked = 0, pos;

    char type;

    const struct flag_type *flag_table;

    char buf[MAX_STRING_LENGTH];



    argument = one_argument(argument,arg1);

    argument = one_argument(argument,arg2);

    argument = one_argument(argument,arg3);



    type = argument[0];



    if (type == '=' || type == '-' || type == '+')

        argument = one_argument(argument,word);



    if (arg1[0] == '\0')

    {

	send_to_char("Syntax:\n\r",ch);

	send_to_char("  flag mob  <name> <field> <flags>\n\r",ch);

	send_to_char("  flag char <name> <field> <flags>\n\r",ch);

	send_to_char("  mob  flags: act, off, pos, form, part          \n\r",ch);

	send_to_char("              imm, res, vuln, aff                \n\r",ch);

	send_to_char("  char flags: plr, comm, info, aff, pos, vuln    \n\r",ch);

	send_to_char("              her, ang, stat, sup, imm, res      \n\r",ch); /* ###### URK_CYBER ###### */
	
	send_to_char("              olc, cyber                         \n\r",ch); 

	send_to_char("  +: add flag, -: remove flag, = set equal to\n\r",ch);

	send_to_char("  otherwise flag toggles the flags listed.\n\r",ch);

	return;

    }



    if (arg2[0] == '\0')

    {

	send_to_char("What do you wish to set flags on?\n\r",ch);

	return;

    }



    if (arg3[0] == '\0')

    {

	send_to_char("You need to specify a flag to set.\n\r",ch);

	return;

    }



    if (argument[0] == '\0')

    {

	send_to_char("Which flags do you wish to change?\n\r",ch);

	return;

    }



    if (!str_prefix(arg1,"mob") || !str_prefix(arg1,"char"))

    {

	victim = get_char_world(ch,arg2);

	if (victim == NULL)

	{

	    send_to_char("You can't find them.\n\r",ch);

	    return;

	}



        /* select a flag to set */

	if (!str_prefix(arg3,"act"))

	{

	    if (!IS_NPC(victim))

	    {

		send_to_char("Use plr for PCs.\n\r",ch);

		return;

	    }



	    flag = &victim->act;

	    flag_table = act_flags;

	}



	else if (!str_prefix(arg3,"plr"))

	{

	    if (IS_NPC(victim))

	    {

		send_to_char("Use act for NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->act;

	    flag_table = plr_flags;

	}



	else if (!str_prefix(arg3,"olc"))

	{

	    if (IS_NPC(victim))

	    {

		send_to_char("Use act for NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->olcs;

	    flag_table = olcs_flags;

	}



 	else if (!str_prefix(arg3,"aff"))

	{

	    flag = &victim->affected_by;

	    flag_table = affect_flags;

	}



  	else if (!str_prefix(arg3,"immunity"))

	{

	    flag = &victim->imm_flags;

	    flag_table = imm_flags;

	}



	else if (!str_prefix(arg3,"resist"))

	{

	    flag = &victim->res_flags;

	    flag_table = imm_flags;

	}



	else if (!str_prefix(arg3,"vuln"))

	{

	    flag = &victim->vuln_flags;

	    flag_table = imm_flags;

	}



	else if (!str_prefix(arg3,"form"))

	{

	    if (!IS_NPC(victim))

	    {

	 	send_to_char("Form can't be set on PCs.\n\r",ch);

		return;

	    }



	    flag = &victim->form;

	    flag_table = form_flags;

	}



	else if (!str_prefix(arg3,"parts"))

	{

	    if (!IS_NPC(victim))

	    {

		send_to_char("Parts can't be set on PCs.\n\r",ch);

		return;

	    }



	    flag = &victim->parts;

	    flag_table = part_flags;

	}



	else if (!str_prefix(arg3,"comm"))

	{

	    if (IS_NPC(victim))

	    {

		send_to_char("Comm can't be set on NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->comm;

	    flag_table = comm_flags;

	}



	/* ###### URK_CYBER ###### */



        else if (!str_prefix(arg3,"cyber"))

        {



            flag = &victim->cyber;

            flag_table = cyber_flags;

        }

	

	else if (!str_prefix(arg3,"info"))

	{

	    if (IS_NPC(victim))

	    {

		send_to_char("Info can't be set on NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->info;

	    flag_table = info_flags;

	}



	else if (!str_prefix(arg3,"her"))

	{

	    if ( (ch->level < MAX_LEVEL) || (get_trust(ch) < MAX_LEVEL) )

	    {

		send_to_char("You must be an Implementor to set Immortal Command Flags!\n\r", ch);

		sprintf( buf, "%s attempted to change %s Immortal Command Flags", ch->name, victim->name );

		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

		log_string( buf );

		return;

	    }



	    if (IS_NPC(victim))

	    {

		send_to_char("Immortal Command Flags can not be set on NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->icf_her;

	    flag_table = icf_her_flags;

	}



	else if (!str_prefix(arg3,"ang"))

	{

	    if ( (ch->level < MAX_LEVEL) || (get_trust(ch) < MAX_LEVEL) )

	    {

		send_to_char("You must be an Implementor to set Immortal Command Flags!\n\r", ch);

		sprintf( buf, "%s attempted to change %s Immortal Command Flags", ch->name, victim->name );

		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

		log_string( buf );

		return;

	    }



	    if (IS_NPC(victim))

	    {

		send_to_char("Immortal Command Flags can not be set on NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->icf_ang;

	    flag_table = icf_ang_flags;

	}



	else if (!str_prefix(arg3,"stat"))

	{

	    if ( (ch->level < MAX_LEVEL) || (get_trust(ch) < MAX_LEVEL) )

	    {

		send_to_char("You must be an Implementor to set Immortal Command Flags!\n\r", ch);

		sprintf( buf, "%s attempted to change %s Immortal Command Flags", ch->name, victim->name );

		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

		log_string( buf );

		return;

	    }



	    if (IS_NPC(victim))

	    {

		send_to_char("Immortal Command Flags can not be set on NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->icf_stat;

	    flag_table = icf_stat_flags;

	}



	else if (!str_prefix(arg3,"sup"))

	{

	    if ( (ch->level < MAX_LEVEL) || (get_trust(ch) < MAX_LEVEL) )

	    {

		send_to_char("You must be an Implementor to set Immortal Command Flags!\n\r", ch);

		sprintf( buf, "%s attempted to change %s Immortal Command Flags", ch->name, victim->name );

		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

		log_string( buf );

		return;

	    }



	    if (IS_NPC(victim))

	    {

		send_to_char("Immortal Command Flags can not be set on NPCs.\n\r",ch);

		return;

	    }



	    flag = &victim->icf_sup;

	    flag_table = icf_sup_flags;

	}



	else if (!str_prefix(arg3,"pos"))

	{

	    flag = &victim->position;

	    flag_table = position_flags;

	}



	else 

	{

	    send_to_char("That's not an acceptable flag.\n\r",ch);

	    return;

	}



	old = *flag;

	victim->zone = NULL;



	if (type != '=')

	    new = old;



        /* mark the words */

        for (; ;)

        {

	    argument = one_argument(argument,word);



	    if (word[0] == '\0')

		break;



	    pos = flag_lookup(word,flag_table);

	    if (pos == 0)

	    {

		send_to_char("That flag doesn't exist!\n\r",ch);

		return;

	    }

	    else

		SET_BIT(marked,pos);

	}



	for (pos = 0; flag_table[pos].name != NULL; pos++)

	{

	    if (!flag_table[pos].settable && IS_SET(old,flag_table[pos].bit))

	    {

		SET_BIT(new,flag_table[pos].bit);

		continue;

	    }



	    if (IS_SET(marked,flag_table[pos].bit))

	    {

		switch(type)

		{

		    case '=':

		    case '+':

			SET_BIT(new,flag_table[pos].bit);

			break;

		    case '-':

			REMOVE_BIT(new,flag_table[pos].bit);

			break;

		    default:

			if (IS_SET(new,flag_table[pos].bit))

			    REMOVE_BIT(new,flag_table[pos].bit);

			else

			    SET_BIT(new,flag_table[pos].bit);

		}

	    }

	}

	*flag = new;

	return;

    }

}







    

