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
 **************************************************************************/

/***************************************************************************
*       ROM 2.4 is copyright 1993-1996 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Rom24/doc/rom.license                     *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"

void do_remort_engine args(( CHAR_DATA *ch, bool IsRemort));
void do_vampire_engine args(( CHAR_DATA *ch, bool IsRemort ));

void do_remor( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REMORT, you must spell it out.\n\r", ch );
    return;
}

void do_remort( CHAR_DATA *ch, char *argument )
{
    extern bool betalock;
    char buf[MAX_INPUT_LENGTH]; 

    if (betalock)
    {
	send_to_char("The game is Betalocked.\n\rYou may not remort in this mode.\n\r", ch);
	return;
    }

    if ( IS_NPC(ch) || (ch->desc == NULL) )
	return;

    if ( ch->level < 100 && !IS_REMORT(ch) )
    {
	send_to_char( "You must be level 100 to remort.\n\r", ch );
	return;
    }

	if (ch->level >= 400 && IS_REMORT(ch) )
	{
		send_to_char("You are eligible for second tier remort races.", ch);	
	}

    if ( ch->pcdata->confirm_remort )
    {
	if (argument[0] != '\0')
	{
		send_to_char( "Remort status removed.\n\r", ch );
		ch->pcdata->confirm_remort = FALSE;
		return;
	}
	else
	{
		do_remort_engine( ch, TRUE );
		return;
	}
    }

    if ( argument[0] != '\0' )
    {
	send_to_char( "Just type remort.  No argument.\n\r", ch );
	return;
    }

    send_to_char("Type remort again to confirm this command.\n\r", ch );
    send_to_char("{rWARNING:{w This command is irreversible.\n\r", ch );
    send_to_char("Typing remort with an argument will undo remort status.\n\r", ch );
    ch->pcdata->confirm_remort = TRUE;
    sprintf(buf, "%s is contemplating remorting.", ch->name );
    wiznet( buf,NULL,NULL,0,0,get_trust(ch));
    log_string( buf );

    return;
}

void do_remort_engine( CHAR_DATA *ch, bool IsRemort )
{
    DESCRIPTOR_DATA *d;

    char strsave[MAX_INPUT_LENGTH], player_name[MAX_INPUT_LENGTH];
    char player_pwd[MAX_STRING_LENGTH];
    int player_incarnations;
    int player_hours;
    int player_bank;
    int player_jedi;
    int player_remort2;

    extern bool betalock;
    char buf[MAX_INPUT_LENGTH]; 

    if (betalock)
    {
	send_to_char("The game is Betalocked.\n\rYou may not remort in this mode.\n\r", ch);
	return;
    }

    /*
     * Get ready to delete the pfile, send a nice informational message.
     */

    d = ch->desc;

    if ( IsRemort )
    {
	    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	    stop_fighting( ch, TRUE );
	    send_to_char( "You have chosen to remort.  You will now be dropped in at the race\n\r", ch );
	    send_to_char( "selection section of character creation, and will be allowed to choose from\n\r", ch );
	    send_to_char( "a wider selection of races and classes.\n\r\n\r", ch );
	    send_to_char( "In the unlikely event that you are disconnected or the MUD\n\r", ch );
	    send_to_char( "crashes while you are creating your character, create a new character\n\r", ch );
	    send_to_char( "as normal and write a note to 'immortal'.\n\r", ch );
	    send_to_char( "\n\r[Hit Enter to Continue]\n\r", ch );
	    sprintf( buf, "%s has remorted.", ch->name );
	    wiznet( buf, NULL, NULL, 0, 0, 0 );
	    log_string( buf );
	    do_mudwatch_out( ch );

	    /*
	     * I quote:
	     * "After extract_char the ch is no longer valid!"
	     */
	    sprintf( player_name, "%s", ch->name );
	    sprintf( player_pwd, "%s", ch->pcdata->pwd );
          player_bank = ++ch->bank;
	    player_incarnations = ++ch->pcdata->incarnations;
		player_jedi = ch->pcdata->jedi; //Jedi Grayson 24 June 2004

	    if ( ch->perm_played < ch->played )
		player_hours = ++ch->played;
	    else
		player_hours = ++ch->perm_played;

	    if (ch->level >= 400 && IS_REMORT(ch) )
		{
			player_remort2 = 1;	
		}
		else player_remort2 = 0;
	    
	    extract_char( ch, TRUE );

	    /*
	     * Delete the pfile, but don't boot the character.
	     * Instead, do a load_char_obj to get a new ch,
	     * saving the password, and the incarnations.  Then,
	     * set the PLR_REMORT bit and drop the player in at
	     * CON_BEGIN_REMORT.
	     */
	    unlink( strsave );
	    load_char_obj( d, player_name );
	    d->character->pcdata->pwd = str_dup( player_pwd );
	    d->character->pcdata->incarnations = player_incarnations;
	    d->character->played = 0;
        d->character->perm_played = player_hours;
        d->character->bank = player_bank;
		d->character->pcdata->jedi = player_jedi; //Jedi Grayson 24 June 2004
	    if( !IS_SET( ch->act, PLR_REMORT ) )
		SET_BIT( ch->act, PLR_REMORT );
	    
	    if (player_remort2 && !IS_SET(ch->act, PLR_REMORT2) )
	    	SET_BIT(ch->act, PLR_REMORT2);
	    
	    d->connected = CON_BEGIN_REMORT;
	    return;
    }
    else
    {
	    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	    stop_fighting( ch, TRUE );
	    send_to_char( "You will now be dropped in at the race selection section of character creation.\n\r", ch );
	    send_to_char( "In the unlikely event that you are disconnected or the MUD\n\r", ch );
	    send_to_char( "crashes while you are creating your character, create a new character\n\r", ch );
	    send_to_char( "as normal and write a note to 'immortal'.\n\r", ch );
	    send_to_char( "\n\r[Hit Enter to Continue]\n\r", ch );
	    sprintf( buf, "%s has been recreated.", ch->name );
	    wiznet( buf, NULL, NULL, 0, 0, 0 );
	    log_string( buf );
	    do_mudwatch_out( ch );

	    /*
	     * I quote:
	     * "After extract_char the ch is no longer valid!"
	     */
	    sprintf( player_name, "%s", ch->name );
	    sprintf( player_pwd, "%s", ch->pcdata->pwd );

	    if ( ch->perm_played < ch->played )
		player_hours = ++ch->played;
	    else
		player_hours = ++ch->perm_played;

	    extract_char( ch, TRUE );

	    /*
	     * Delete the pfile, but don't boot the character.
	     * Instead, do a load_char_obj to get a new ch,
	     * saving the password, and the incarnations.  Then,
	     * drop the player in at CON_BEGIN_REMORT.
 	     */
	    unlink( strsave );
	    load_char_obj( d, player_name );
	    d->character->pcdata->pwd = str_dup( player_pwd );
	    d->character->played = 0;
        d->character->perm_played = player_hours;
	    d->connected = CON_BEGIN_REMORT;
	    return;
    }

    return;
}

//Grayson Vampire 28 June 2004

void do_reject_vampire (CHAR_DATA *ch, char *argument )
{
	send_to_char( "Vampire status removed.\n\r", ch );
	ch->pcdata->confirm_vampire = FALSE;
	return;	
}

void do_accept_vampire( CHAR_DATA *ch, char *argument )
{
    extern bool betalock;
    char buf[MAX_INPUT_LENGTH]; 

    if (betalock)
    {
		send_to_char("The game is Betalocked.\n\rYou may not become a vampire in this mode.\n\r", ch);
		return;
    }

    if ( IS_NPC(ch) || (ch->desc == NULL) )
	return;

    if ( ch->level < 100 && !IS_REMORT(ch) )
    {
		send_to_char( "You must be level 100 to become a vampire.\n\r", ch );
		return;
    }

    if ( ch->pcdata->confirm_vampire )
    {
		if (argument[0] != '\0')
		{
			send_to_char( "Vampire status removed.\n\r", ch );
			ch->pcdata->confirm_vampire = FALSE;
			return;
		}
		else
		{
			do_vampire_engine( ch, TRUE );
			return;
		}
    }
/*
    if ( argument[0] != '\0' )
    {
	send_to_char( "Just type accept.  No argument.\n\r", ch );
	return;
    }
*/
/*
    send_to_char("Type accept again to confirm this command.\n\r", ch );
    send_to_char("{rWARNING:{w This command is irreversible.\n\r", ch );
    send_to_char("Typing accept with an argument will undo vampire status.\n\r", ch );
    ch->pcdata->confirm_remort = TRUE;
*/
    sprintf(buf, "%s is contemplating becoming a vampire.", ch->name );
    wiznet( buf,NULL,NULL,0,0,get_trust(ch));
    log_string( buf );

    return;
}


void do_vampire_engine( CHAR_DATA *ch, bool IsRemort )
{
    DESCRIPTOR_DATA *d;

    char strsave[MAX_INPUT_LENGTH], player_name[MAX_INPUT_LENGTH];
    char player_pwd[MAX_STRING_LENGTH];
    int player_incarnations;
    int player_hours;
    int player_bank;
    int player_jedi;
    int player_remort2;
    int confirm_vampire;

    extern bool betalock;
    char buf[MAX_INPUT_LENGTH]; 

    if (betalock)
    {
	send_to_char("The game is Betalocked.\n\rYou may not become a vampire in this mode.\n\r", ch);
	return;
    }

    /*
     * Get ready to delete the pfile, send a nice informational message.
     */

    d = ch->desc;

    if ( IsRemort )
    {
	    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	    stop_fighting( ch, TRUE );
	    send_to_char( "You have chosen to become a vampire.  You will now be dropped in at the sex\n\r", ch );
	    send_to_char( "selection section of character creation.", ch );
	    send_to_char( "In the unlikely event that you are disconnected or the MUD\n\r", ch );
	    send_to_char( "crashes while you are creating your character, create a new character\n\r", ch );
	    send_to_char( "as normal and write a note to 'immortal'.\n\r", ch );
	    send_to_char( "\n\r[Hit Enter to Continue]\n\r", ch );
	    sprintf( buf, "%s has become a vampire.", ch->name );
	    wiznet( buf, NULL, NULL, 0, 0, 0 );
	    log_string( buf );
	    do_mudwatch_out( ch );

	    /*
	     * I quote:
	     * "After extract_char the ch is no longer valid!"
	     */
	    sprintf( player_name, "%s", ch->name );
	    sprintf( player_pwd, "%s", ch->pcdata->pwd );
          player_bank = ++ch->bank;
	    player_incarnations = ++ch->pcdata->incarnations;
		player_jedi = ch->pcdata->jedi; //Jedi Grayson 24 June 2004

	    if ( ch->perm_played < ch->played )
		player_hours = ++ch->played;
	    else
		player_hours = ++ch->perm_played;

	    extract_char( ch, TRUE );

	    /*
	     * Delete the pfile, but don't boot the character.
	     * Instead, do a load_char_obj to get a new ch,
	     * saving the password, and the incarnations.  Then,
	     * set the PLR_REMORT bit and drop the player in at
	     * CON_BEGIN_REMORT.
	     */
	    unlink( strsave );
	    load_char_obj( d, player_name );
	    d->character->pcdata->pwd = str_dup( player_pwd );
	    d->character->pcdata->incarnations = player_incarnations;
	    d->character->played = 0;
        d->character->perm_played = player_hours;
        d->character->bank = player_bank;
		d->character->pcdata->jedi = player_jedi; //Jedi Grayson 24 June 2004
/*
	    if( !IS_SET( ch->act, PLR_REMORT ) )
		SET_BIT( ch->act, PLR_REMORT );
	    
	    if (player_remort2 && !IS_SET(ch->act, PLR_REMORT2) )
	    	SET_BIT(ch->act, PLR_REMORT2);
*/	    
	    d->connected = CON_BEGIN_VAMPIRE;
	    return;
    }
    else
    {
	    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	    stop_fighting( ch, TRUE );
	    send_to_char( "You have chosen to become a vampire.  You will now be dropped in at the sex\n\r", ch );
	    send_to_char( "selection section of character creation.", ch );
	    send_to_char( "In the unlikely event that you are disconnected or the MUD\n\r", ch );
	    send_to_char( "crashes while you are creating your character, create a new character\n\r", ch );
	    send_to_char( "as normal and write a note to 'immortal'.\n\r", ch );
	    send_to_char( "\n\r[Hit Enter to Continue]\n\r", ch );
	    sprintf( buf, "%s has become a vampire.", ch->name );
	    wiznet( buf, NULL, NULL, 0, 0, 0 );
	    log_string( buf );
	    do_mudwatch_out( ch );

	    /*
	     * I quote:
	     * "After extract_char the ch is no longer valid!"
	     */
	    sprintf( player_name, "%s", ch->name );
	    sprintf( player_pwd, "%s", ch->pcdata->pwd );

	    if ( ch->perm_played < ch->played )
		player_hours = ++ch->played;
	    else
		player_hours = ++ch->perm_played;

	    extract_char( ch, TRUE );

	    /*
	     * Delete the pfile, but don't boot the character.
	     * Instead, do a load_char_obj to get a new ch,
	     * saving the password, and the incarnations.  Then,
	     * drop the player in at CON_BEGIN_REMORT.
 	     */
	    unlink( strsave );
	    load_char_obj( d, player_name );
	    d->character->pcdata->pwd = str_dup( player_pwd );
	    d->character->played = 0;
        d->character->perm_played = player_hours;
	    d->connected = CON_BEGIN_VAMPIRE;
	    return;
    }

    return;
}

void do_turn_vampire( CHAR_DATA *ch, char *argument )
{
    extern bool betalock;
    char buf[MAX_INPUT_LENGTH]; 
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    
    argument = one_argument( argument, arg );
	
		
	if ( ( victim = get_char_room( ch, arg)  ) == NULL )
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}
	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
	{
		send_to_char("The gods will not allow that here.\n\r",ch);
		return;	
	}
	
	if (race_lookup("vampire") != ch->race)
	{
		send_to_char("What? Do you think you're some kind of vampire or something?\n\r",ch);
		return;	
	}
	if (race_lookup("vampire") == victim->race)
	{
		act("$N already is a vampire.",ch,NULL,victim,TO_CHAR);
		return;	
	}

	act("You sink your teeth into $N's neck.",ch,NULL,victim,TO_CHAR);
	act("You can feel $N's heart pumping as $S blood flows into your mouth.",ch,NULL,victim,TO_CHAR);
	act("$n sinks $s teeth into $N's neck.",ch,NULL,victim,TO_NOTVICT);
	act("$n sinks $s teeth into your neck.",ch,NULL,victim,TO_VICT);
	send_to_char("You begin to grow cold as your body is drained of blood.\n\r", victim);
	act("$n offers you $s vein.",ch, NULL,victim, TO_VICT);
	send_to_char("Do you wish to drink?\n\r", victim );
    send_to_char("Type 'accept' with no arguement to accept, or type 'reject' to fight\n\r",victim);
	victim->pcdata->confirm_vampire = TRUE;
	return;
}

