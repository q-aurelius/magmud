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
#include "recycle.h"
#include "tables.h"
#include "lookup.h"


/* command procedures needed */
DECLARE_DO_FUN(do_rstat		);
DECLARE_DO_FUN(do_mstat		);
DECLARE_DO_FUN(do_ostat		);
DECLARE_DO_FUN(do_rset		);
DECLARE_DO_FUN(do_mset		);
DECLARE_DO_FUN(do_oset		);
DECLARE_DO_FUN(do_sset		);
DECLARE_DO_FUN(do_mfind		);
DECLARE_DO_FUN(do_ofind		);
DECLARE_DO_FUN(do_rfind		);
DECLARE_DO_FUN(do_slookup	);
DECLARE_DO_FUN(do_mload		);
DECLARE_DO_FUN(do_oload		);
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_help        );


/*
 * Local functions.
 */
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );

void do_wiznet( CHAR_DATA *ch, char *argument )
{
    int flag;
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
	send_to_char( "SYNTAX: {cwiznet {R<{Won{c|{Woff{c|{Wstat{c|{Wshow{c|{Wclear{c|{Wflag{R>{w\n\r", ch );
	send_to_char( "   {cclear{w will remove all wiznet flags                     \n\r", ch );
	send_to_char( "   {cstat{w and {cshow{w show all the flags and their status  \n\r", ch );
	send_to_char( "   {cflag{w toggles the flag on or off                        \n\r", ch );
	return;
    }

    if (!str_prefix(argument,"on"))
    {
	send_to_char("Welcome to Wiznet!\n\r",ch);
	SET_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    if (!str_prefix(argument,"off"))
    {
	send_to_char("Signing off of Wiznet.\n\r",ch);
	REMOVE_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    if (!str_prefix(argument,"clear"))
    {
	REMOVE_BIT(ch->wiznet,WIZ_TICKS);
	REMOVE_BIT(ch->wiznet,WIZ_RESETS);
	REMOVE_BIT(ch->wiznet,WIZ_LEVELS);
	REMOVE_BIT(ch->wiznet,WIZ_DEATHS);
	REMOVE_BIT(ch->wiznet,WIZ_MOBDEATHS);
	REMOVE_BIT(ch->wiznet,WIZ_SACCING);
	REMOVE_BIT(ch->wiznet,WIZ_LOG);
	REMOVE_BIT(ch->wiznet,WIZ_TIME);
	REMOVE_BIT(ch->wiznet,WIZ_TIME);
	REMOVE_BIT(ch->wiznet,WIZ_LOGOUT);
	REMOVE_BIT(ch->wiznet,WIZ_NEWBIE);
	REMOVE_BIT(ch->wiznet,WIZ_SITES);
	REMOVE_BIT(ch->wiznet,WIZ_LINKS);
	REMOVE_BIT(ch->wiznet,WIZ_SPAM);
	REMOVE_BIT(ch->wiznet,WIZ_FLAGS);
	REMOVE_BIT(ch->wiznet,WIZ_PENALTIES);
	REMOVE_BIT(ch->wiznet,WIZ_LOAD);
	REMOVE_BIT(ch->wiznet,WIZ_RESTORE);
	REMOVE_BIT(ch->wiznet,WIZ_SNOOPS);
	REMOVE_BIT(ch->wiznet,WIZ_SWITCHES);
	REMOVE_BIT(ch->wiznet,WIZ_SECURE);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG);
	REMOVE_BIT(ch->wiznet,WIZ_OLC);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG_EXP);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG_FIGHT);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG_CHAN);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG_MAGIC);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG_OTHER);
	REMOVE_BIT(ch->wiznet,WIZ_DEBUG_MEMORY);
	save_char_obj( ch );
	send_to_char( "Wiznet Flags Cleared.\n\r", ch );
	return;
    }


    if (!str_prefix(argument,"show")||!str_prefix(argument,"status"))
    /* list of all wiznet options */
    {
	buf[0] = '\0';

	if (!IS_SET(ch->wiznet,WIZ_ON))
	{
	    strcat(buf,"Wiznet is currently {ROFF{w\n\r");
	    return;
	}

	send_to_char("Wiznet options and status:    \n\r", ch );
	send_to_char("Level   Status   Message Type \n\r", ch );
	send_to_char("----------------------------- \n\r", ch );

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	{
	    if (wiznet_table[flag].level <= get_trust(ch))
	    {
		sprintf( buf, " {y%s      %s    {c%s{w\n\r",
			lev_name(wiznet_table[flag].level),
			IS_SET(ch->wiznet,wiznet_table[flag].flag) ? "{gON " : "{ROFF",
			wiznet_table[flag].name );
		send_to_char( buf, ch );
	    }
	}

	send_to_char("\n\r", ch);
	send_to_char("Wiznet Messages: {R--{r>{w {g<{Wmessage type{g>{w : {g<{Wmessage{g>{w\n\r", ch );
	send_to_char("{gGreen{w  Message Types are informational and player related.\n\r", ch );
	send_to_char("{rRed{w    Message Types are Immortal related.\n\r", ch );
	send_to_char("{yYellow{w Message Types are for connection information.\n\r", ch );
	send_to_char("{cCyan{w   Message Types are for administrative information.\n\r", ch );
	return;
    }

    flag = wiznet_lookup(argument);

    if (flag == -1 )
    {
	send_to_char("No such option.\n\r",ch);
	return;
    }

    if ( get_trust(ch) < wiznet_table[flag].level )
    {
	if ( IS_SET(ch->wiznet,wiznet_table[flag].flag) )
	{
		REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
		sprintf( buf, "%s had an invalid wiznet flag set [ %s ]", ch->name, wiznet_table[flag].name );
		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
		log_string( buf );
	}
	send_to_char("Invalid option.\n\r",ch);
	return;
    }


    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
    {
	sprintf(buf,"You will no longer see %s on wiznet.\n\r",
	        wiznet_table[flag].name);
	send_to_char(buf,ch);
	REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
    	return;
    }
    else
    {
    	sprintf(buf,"You will now see %s on wiznet.\n\r",
		wiznet_table[flag].name);
	send_to_char(buf,ch);
    	SET_BIT(ch->wiznet,wiznet_table[flag].flag);
	return;
    }

}

void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj,
	    long flag, long flag_skip, int min_level)
{
    char buf[MAX_STRING_LENGTH];
    char wnetflg[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    /* Show Wiznet flag name and set color for type.
       Green  - {g for informational or player related
       Red    - {r for a immortal related
       Yellow - {y for connections
       Cyan   - {c for administrative
    */

    switch(flag)
    {
	default:			sprintf(wnetflg, "{wWiznet{w");		break;
	case WIZ_TICKS:		sprintf(wnetflg, "{gTick{w");			break;
	case WIZ_RESETS:		sprintf(wnetflg, "{gReset{w");		break;
	case WIZ_LEVELS:		sprintf(wnetflg, "{gLevel{w");		break;
	case WIZ_DEATHS:		sprintf(wnetflg, "{gDeath{w");		break;
	case WIZ_MOBDEATHS:	sprintf(wnetflg, "{gMobDeath{w");		break;
	case WIZ_SACCING:		sprintf(wnetflg, "{gSaccing{w");		break;
	case WIZ_LOG:		sprintf(wnetflg, "{gPLog{w");			break;
	case WIZ_TIME:		sprintf(wnetflg, "{gTime{w");			break;
	case WIZ_LOGOUT:		sprintf(wnetflg, "{yLogout{w");		break;
	case WIZ_NEWBIE:		sprintf(wnetflg, "{yNewbie{w");		break;
	case WIZ_SITES:		sprintf(wnetflg, "{ySite{w");			break;
	case WIZ_LINKS:		sprintf(wnetflg, "{yLink{w");			break;
	case WIZ_SPAM:		sprintf(wnetflg, "{ySpam{w");			break;
	case WIZ_FLAGS:		sprintf(wnetflg, "{rFlag{w");			break;
	case WIZ_PENALTIES:	sprintf(wnetflg, "{rPenalty{w");		break;
	case WIZ_LOAD:		sprintf(wnetflg, "{rLoad{w");			break;
	case WIZ_RESTORE:		sprintf(wnetflg, "{rRestore{w");		break;
	case WIZ_SNOOPS:		sprintf(wnetflg, "{rSnoop{w");		break;
	case WIZ_SWITCHES:	sprintf(wnetflg, "{rSwitch{w");		break;
	case WIZ_SECURE:		sprintf(wnetflg, "{rSecure{w");		break;
	case WIZ_DEBUG:		sprintf(wnetflg, "{cDEBUG{w");		break;
	case WIZ_OLC:		sprintf(wnetflg, "{cOLC{w");			break;
	case WIZ_DEBUG_EXP:	sprintf(wnetflg, "{cDEBUG_Exp{w");		break;
	case WIZ_DEBUG_FIGHT:	sprintf(wnetflg, "{cDEBUG_Fight{w");	break;
	case WIZ_DEBUG_CHAN:	sprintf(wnetflg, "{cDEBUG_Channels{w");	break;
	case WIZ_DEBUG_MAGIC:	sprintf(wnetflg, "{cDEBUG_Magic{w");	break;
	case WIZ_DEBUG_OTHER:	sprintf(wnetflg, "{cDEBUG_Other{w");	break;
	case WIZ_DEBUG_MEMORY:	sprintf(wnetflg, "{cDEBUG_Memory{w");	break;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
      if (d->connected == CON_PLAYING
	&&  IS_IMMORTAL(d->character)
	&&  IS_SET(d->character->wiznet,WIZ_ON)
	&&  (!flag || IS_SET(d->character->wiznet,flag))
	&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
	&&  get_trust(d->character) >= min_level
	&&  d->character != ch)
        {
	    if (IS_SET(d->character->wiznet,WIZ_PREFIX))
	    {
		sprintf( buf, "{R--{r>{w %s{w : %s", wnetflg, string );
	    }
	    else
	    {
		sprintf( buf, "{w%s{w : %s", wnetflg, string );
	    }
          act_new(buf,d->character,obj,ch,TO_CHAR,POS_DEAD);
        }
    }

    return;
}

void do_clanrequest( CHAR_DATA *ch, char * argument )
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *reject;
	int clan;
	DESCRIPTOR_DATA *d;
	bool found;

	if (IS_NPC(ch))
		return;

	found = FALSE;
	clan = 0;

	argument = one_argument( argument, arg );

	if (	IS_HERO(ch)
		|| 	IS_SET(ch->act,PLR_CLANLEADER)
		|| 	IS_SET(ch->info,INFO_CLANGM)
		|| 	IS_SET(ch->info,INFO_CLANTRAINER)
		|| 	IS_SET(ch->olcs,OLC_CLANADMIN) )
	{
		if ( arg[0] == '\0' || !str_cmp( arg, "xupdlst" ) )
		{
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && d->character->clanreq != 0 )
				{
					if ( !str_cmp( arg, "xupdlst" ) )
					{
						if ( d->character->clanreq == ch->clan &&
							(	IS_SET(d->character->act,PLR_CLANLEADER)
							|| 	IS_SET(d->character->info,INFO_CLANGM)
							|| 	IS_SET(d->character->info,INFO_CLANTRAINER)
							|| 	IS_SET(d->character->olcs,OLC_CLANADMIN) ) )
							found = TRUE;
						else
							found = FALSE;
					}
					else
						found = TRUE;

					break;
				}
			}

			if ( found )
			{
				send_to_char( "\n\r{cOutstanding Clan Requests from On-Line Characters:{w\n\r", ch );
				send_to_char( "{W--------------------------------------------------{w\n\r", ch );
				for ( d = descriptor_list; d != NULL; d = d->next )
				{
					if ( 	d->connected == CON_PLAYING && d->character->clanreq != 0 )
					{
					  if ( clan_table[d->character->clanreq].visible )
					  {
						sprintf( buf, "{w%-15s   {g%-15s   {w%-25s{w\n\r",
							d->character->name,
							clan_table[d->character->clanreq].name,
							clan_table[d->character->clanreq].who_name );
						send_to_char( buf, ch );
					  }

					  if ( !clan_table[d->character->clanreq].visible )
					  {
						d->character->clanreq = 0;
					  }
					}
				}
				send_to_char( "\n\r{wType {cclanrequest reject {R<{Wname{R>{w to reject an outstanding request.\n\r", ch );
				send_to_char( "Type {cguild {R<{Wname{R> <{Wclan{R>{w to fulfill the request and add them to a clan.\n\r", ch );
			}

			if ( !found && arg[0] == '\0' )
				send_to_char( "No outstanding Clan Requests\n\r", ch );

			return;
		}
		else if ( !str_cmp( arg, "reject" ) )
		{
			argument = one_argument( argument, arg2 );

			if ( ( reject = get_char_world( ch, arg2 ) ) == NULL )
			{
				send_to_char( "They are not here.\n\r", ch );
				return;
			}

			sprintf( buf, "We are sorry, but, %s has rejected your request.\n\r",
				clan_table[reject->clanreq].who_name );
			send_to_char( buf, reject );
			reject->clanreq = 0;
			send_to_char( "Application Rejected.\n\r", ch );

			return;
		}
		else
		{
			send_to_char( "Type {cclanrequest{w with no argument to see the list of outstanding requests.\n\r", ch );
			send_to_char( "Type {cclanrequest reject {R<{Wname{R>{w to reject an outstanding request.\n\r", ch );
			return;
		}

		return;
	}

	if ( !str_cmp( arg, "xupdlst" ) )
		return;

	if ( arg[0] == '\0' )
	{
		if ( ch->clanreq == 0 )
		{
			send_to_char( "Please include the clan name you are interested in.\n\r", ch );
			return;
		}
		else
		{
			sprintf( buf, "Your request to join %s has been removed.\n\r", clan_table[ch->clanreq].who_name );
			send_to_char( buf, ch );
			return;
		}
	}

	if ( is_clan(ch) )
	{
		send_to_char( "You must first leave your current clan before requesting information on a new clan.\n\r", ch );
		return;
	}

	if ((clan = clan_lookup(arg)) == 0)
	{
		send_to_char("No such clan exists.\n\r",ch);
		return;
	}

	if ( !clan_table[clan].visible )
	{
		send_to_char("No such clan exists.\n\r",ch);
		return;
	}

	if ( ch->clanreq != 0 )
	{
		sprintf( buf, "{wYou must first remove your current request to join %s.", clan_table[ch->clanreq].who_name );
		send_to_char( buf, ch );
		send_to_char( "You may do this by typing {cclanrequest{w with no arguments.\n\r", ch );
	}


	if ( IS_SET(ch->comm, COMM_NOCHANNELS) )
	{
		send_to_char( "The gods have revoked your ability to make clan requests.\n\r", ch );
		ch->clanreq = 0;
		return;
	}

	ch->clanreq = clan;

	sprintf( buf, "Your request to join %s has been submitted.\n\r", clan_table[ch->clanreq].who_name );
	send_to_char( buf, ch );

	sprintf( buf, "%s has submitted a request to join clan %s [%s]", ch->name, clan_table[ch->clanreq].name, clan_table[ch->clanreq].who_name );
	wiznet( buf, NULL, NULL, 0, 0, 0 );

	sprintf( buf, "%s has submitted a request to join %s\n\r", ch->name, clan_table[ch->clanreq].who_name );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
		if ( 	d->connected == CON_PLAYING
		&&    d->character->clan == ch->clanreq
		&& (  IS_SET(d->character->act,PLR_CLANLEADER)
			|| IS_SET(d->character->info,INFO_CLANGM)
			|| IS_SET(d->character->info,INFO_CLANTRAINER)
			|| IS_SET(d->character->olcs,OLC_CLANADMIN) ) )
		send_to_char( buf, d->character );
	}
	return;
}

void do_guild( CHAR_DATA *ch, char *argument )
{
    char 	arg1_opt	[MAX_INPUT_LENGTH];
    char 	arg2_char	[MAX_INPUT_LENGTH];
    char 	arg3_clan	[MAX_INPUT_LENGTH];
    char 	arg4_flag	[MAX_INPUT_LENGTH];
    char 	buf		[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int clan;
    DESCRIPTOR_DATA *d;
    bool can_change;
    bool remove;
    bool admin;
    bool setflag;
    bool setclan;

    if (IS_NPC(ch))
	return;

    if( !(   get_trust(ch) >= SUPREME
	    || IS_SET(ch->act,PLR_CLANLEADER)
	    || IS_SET(ch->info,INFO_CLANGM)
	    || IS_SET(ch->info,INFO_CLANTRAINER)
	    || IS_SET(ch->olcs,OLC_CLANADMIN) ) )
    {
	send_to_char( "Only an Admin Imm, Clan Administrator, Clan Leader, Clan Grandmaster \n\r", ch);
	send_to_char( "or Clan Head Trainer may guild players into or out of a clan.        \n\r", ch);
	return;
    }


    argument = one_argument( argument, arg1_opt );
    argument = one_argument( argument, arg2_char );

    if ( arg1_opt[0] == '\0' || arg2_char[0] == '\0' )
    {
	  send_to_char( "\n\r", ch );
        send_to_char( "{rSyntax{w: {cguild clan {g<{Wchar{g> <{Wclan name {g|{W none{g>{w\n\r", ch );
	  send_to_char( "{wGuild places a player into a clan.{w\n\r", ch );
	  send_to_char( "\n\r", ch );
	  send_to_char( "If the person is already in your clan, and you are the Leader\n\r", ch );
	  send_to_char( "\n\r", ch );
	  send_to_char( "{rSyntax{w: {cguild flag {R<{Wchar{R> <{Wclan{R> <{Wgm{c|{Wht{c|{Wenforcer{R>{w\n\r", ch );
	  send_to_char( "Toggles GM, Head Trainer, and Enforcer flags.\n\r", ch );
	  send_to_char( "Exceeding your approved limits on these ranks will result in disbandment.\n\r", ch );
	  send_to_char( "\n\r", ch );
	  send_to_char( "Only an Admin Imm, Clan Administrator, Clan Leader,\n\r", ch );
	  send_to_char( "Clan Grandmaster or Clan Head Trainer may use guild.\n\r", ch );
        return;
    }


    /* Initialize some variables */
    clan 		= 0;
    can_change	= FALSE;
    remove		= FALSE;
    admin		= FALSE;
    setflag		= FALSE;
    setclan		= FALSE;

    if( get_trust(ch) >= SUPREME || IS_SET(ch->olcs,OLC_CLANADMIN) )
    {
	admin		= TRUE;
    }

    /*
     * Okay, are we guilding or changing flags?
     */

    if ( !str_cmp( arg1_opt, "clan" ) )
    {
	setflag	= FALSE;
	setclan	= TRUE;
    }

    if ( !str_cmp( arg1_opt, "flag" ) )
    {
	setflag	= FALSE;
	setclan	= TRUE;
    }

    if ( !setflag && !setclan )
    {
	do_guild(ch, "");
	return;
    }

    if ( setflag && setclan )
    {
	bug( "Guild: setflag and setclan are TRUE", 0 );
	do_guild(ch, "");
	return;
    }

    /*
     * Let's find our victim
     */
    if ( ( victim = get_char_world( ch, arg2_char ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on mobiles!\n\r", ch );
	return;
    }

    if ( IS_HERO(victim) && (get_trust(ch) < get_trust(victim)) )
    {
	send_to_char( "I don't think so...\n\r", ch );
	return;
    }

    /*
     * Flagging First
     */
    if ( setflag )
    {
	argument = one_argument( argument, arg3_clan );
	argument = one_argument( argument, arg4_flag );

	if ( arg3_clan[0] != '\0' || arg4_flag[0] != '\0' )
	{
	   do_guild(ch, "");
	   return;
	}


	/* Test to see if we can do the change */

	can_change		= FALSE;						/* Just to make sure */

	if ((clan = clan_lookup(arg3_clan)) == 0)				/* Get the clan */
	{
		send_to_char("No such clan exists.\n\r",ch);
		return;
	}

	if ( !clan_table[clan].visible )					/* Invisible Clans do not exist */
	{
		if ( ch->clan != clan && !admin )
		{
			send_to_char("No such clan exists.\n\r",ch);
			return;
		}
	}

	if ( ch->clan == clan )							/* Are we in the clan we want to change? */
	{
		can_change	= TRUE;
	}

	if ( admin )								/* Admins can do anything */
	{
		can_change	= TRUE;
	}

	if ( (IS_SET(ch->act,PLR_CLANLEADER) || IS_SET(ch->info,INFO_CLANGM))	/* We know we are in the same clan     */
		&& is_clan( ch )									/* or an admin, so let's see if we     */
		&& is_clan( victim )								/* are in the same clan as our target  */
		&& is_same_clan( ch, victim ) )						/* and if we have privleges to make    */
	{												/* a change.                           */
		can_change	= TRUE;
	}


	/* At this point, what do we know:
       * we know we have something in the arguments,
       * we are either in the same clan we want to modify or an admin
       * and that we are a clanleader or gm and we are the victim are in the same clan
 	 */

	if ( !can_change )
	{
		do_guild(ch, "");
		return;
	}

	/* Okay, time to make a change */

	if ( !str_cmp( arg4_flag, "gm" ) )
	{
		can_change	= FALSE;			/* For each command in the loop, we will test again */

		if ( admin )								/* Admin can do anything */
		{
			can_change	= TRUE;
		}

		if ( IS_SET(ch->act,PLR_CLANLEADER) )				/* Clan Leader can do this */
		{
			can_change	= TRUE;
		}

		if ( !can_change )
		{
			send_to_char( "You do not have sufficient privleges to make this change.\n\r\n\r", ch );
			do_guild(ch, "");
			return;
		}

		/* Now the fun begins! */

		if( IS_SET(victim->info,INFO_CLANGM) )
		{
			REMOVE_BIT(victim->info,INFO_CLANGM);
			send_to_char( "You are no longer a Grand Master.\n\r", victim);
			send_to_char( "They are no longer your Grand Master.\n\r", ch );
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
				&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
				{
					sprintf( buf, "%s is no longer the Grand Master of %s\n\r",
						PERS(victim, d->character), clan_table[clan].who_name );
					send_to_char( buf, d->character );
				}
			}
			return;
		}

		if( !IS_SET(victim->info,INFO_CLANGM) )
		{
			SET_BIT(victim->info,INFO_CLANGM);
			send_to_char( "You are now a Grand Master.\n\r", victim);
			send_to_char( "They are now your Grand Master.\n\r", ch );
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
				&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
				{
					sprintf( buf, "%s is now the Grand Master of %s\n\r",
						PERS(victim, d->character), clan_table[clan].who_name );
					send_to_char( buf, d->character );
				}
			}
			return;
		}

		bug ( "Guild: GM Not changed.", 0 );
		return;
	}

	if ( !str_cmp( arg4_flag, "ht" ) )
	{
		can_change	= FALSE;			/* For each command in the loop, we will test again */

		if ( admin )								/* Admin can do anything */
		{
			can_change	= TRUE;
		}

		if ( IS_SET(ch->act,PLR_CLANLEADER) || IS_SET(victim->info,INFO_CLANGM) )	/* Lader & GM can do this */
		{
			can_change	= TRUE;
		}

		if ( !can_change )
		{
			send_to_char( "You do not have sufficient privleges to make this change.\n\r\n\r", ch );
			do_guild(ch, "");
			return;
		}

		/* Now the fun begins! */

		if( IS_SET(victim->info,INFO_CLANTRAINER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANTRAINER);
			send_to_char( "You are no longer a Head Trainer.\n\r", victim);
			send_to_char( "They are no longer your Head Trainer.\n\r", ch );
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
				&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
				{
					sprintf( buf, "%s is no longer the Head Trainer of %s\n\r",
						PERS(victim, d->character), clan_table[clan].who_name );
					send_to_char( buf, d->character );
				}
			}
			return;
		}

		if( !IS_SET(victim->info,INFO_CLANTRAINER) )
		{
			SET_BIT(victim->info,INFO_CLANTRAINER);
			send_to_char( "You are now a Head Trainer.\n\r", victim);
			send_to_char( "They are now your Head Trainer.\n\r", ch );
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
				&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
				{
					sprintf( buf, "%s is now the Head Trainer of %s\n\r",
						PERS(victim, d->character), clan_table[clan].who_name );
					send_to_char( buf, d->character );
				}
			}
			return;
		}

		bug ( "Guild: Trainer Not changed.", 0 );
		return;
	}

	if ( !str_cmp( arg4_flag, "enforcer" ) )
	{
		can_change	= FALSE;			/* For each command in the loop, we will test again */

		if ( admin )								/* Admin can do anything */
		{
			can_change	= TRUE;
		}

		if ( IS_SET(ch->act,PLR_CLANLEADER) || IS_SET(victim->info,INFO_CLANGM) )	/* Lader & GM can do this */
		{
			can_change	= TRUE;
		}

		if ( !can_change )
		{
			send_to_char( "You do not have sufficient privleges to make this change.\n\r\n\r", ch );
			do_guild(ch, "");
			return;
		}

		/* Now the fun begins! */

		if( IS_SET(victim->info,INFO_CLANENFORCER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANENFORCER);
			send_to_char( "You are no longer an Enforcer.\n\r", victim);
			send_to_char( "They are no longer your Enforcer.\n\r", ch );
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
				&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
				{
					sprintf( buf, "%s is no longer the Enforcer of %s\n\r",
						PERS(victim, d->character), clan_table[clan].who_name );
					send_to_char( buf, d->character );
				}
			}
			return;
		}

		if( !IS_SET(victim->info,INFO_CLANENFORCER) )
		{
			SET_BIT(victim->info,INFO_CLANENFORCER);
			send_to_char( "You are now an Enforcer.\n\r", victim);
			send_to_char( "They are now your Enforcer.\n\r", ch );
			for ( d = descriptor_list; d != NULL; d = d->next )
			{
				if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
				&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
				{
					sprintf( buf, "%s is now the Enforcer of %s\n\r",
						PERS(victim, d->character), clan_table[clan].who_name );
					send_to_char( buf, d->character );
				}
			}
			return;
		}

		bug ( "Guild: Enforcer Not changed.", 0 );
		return;
	}

	do_guild( ch, "" );
	return;
    }

    /*
     * Now we'll handle moving people into and out of clans
     */
    if ( setclan )
    {
	argument = one_argument( argument, arg3_clan );

	if ( arg3_clan[0] == '\0' )
	{
		do_guild(ch, "" );
		return;
	}


	/* Test to see if we can do the change */
	can_change		= FALSE;						/* Just to make sure */


	/* Are we removing someone? */
	if ( !str_cmp( arg3_clan, "none" ) )
	{
		remove	= TRUE;
	}
	else
	{
		remove	= FALSE;
		can_change	= TRUE;							/* Assume we can */

		if ((clan = clan_lookup(arg3_clan)) == 0)				/* Get the clan */
		{
			send_to_char("No such clan exists.\n\r",ch);
			return;
		}

		if ( !clan_table[clan].visible )					/* Invisible Clans do not exist */
		{
			if ( ch->clan != clan && !admin )
			{
				send_to_char("No such clan exists.\n\r",ch);
				return;
			}
		}

		if ( ch->clan != clan )
		{
			can_change		= FALSE;
			if ( !admin )
			{
				send_to_char( "You can only guild into your clan!\n\r", ch );
			}
		}

		if ( is_clan(victim) )
		{
			can_change	= FALSE;
			if ( !admin )
			{
				send_to_char( "You can not guild people in other clans!\n\r", ch );
			}
		}

		if ( clan_table[clan].visible && (victim->clanreq != ch->clan) )
		{
			can_change	= FALSE;
			if ( !admin )
			{
				send_to_char( "They have not requested to join your clan!\n\r", ch );
			}
		}

		if ( !clan_table[clan].visible && (ch->in_room != victim->in_room) )
		{
			can_change	= FALSE;
			if ( !admin )
			{
				send_to_char( "They must be in the same room as you!\n\r", ch );
			}
		}

		if ( victim->level < 10 )
		{
			can_change	= FALSE;
			if ( !admin )
			{
				send_to_char( "You can not guild people under level 10!\n\r", ch );
			}
		}
	}

	if ( remove )								/* We want to remove someone, okay... */
	{
		if ( is_same_clan(ch, victim) )
		{
			if ( IS_SET(ch->act,PLR_CLANLEADER) )
			{
				can_change = TRUE;
			}

			if ( IS_SET(ch->info,INFO_CLANGM) && !IS_SET(victim->act,PLR_CLANLEADER) )
			{
				can_change = TRUE;
			}

			if (	IS_SET(ch->info,INFO_CLANTRAINER)
					&& !IS_SET(victim->info,INFO_CLANGM)
					&& !IS_SET(victim->act,PLR_CLANLEADER) )
			{
				can_change = TRUE;
			}

			can_change	= FALSE;

			if ( !admin )
			{
				send_to_char( "You can not remove them from the clan!\n\r", ch);
				sprintf( buf, "%s attempted to remove you from the clan!\n\r", ch->name);
				send_to_char( buf, victim );
			}
		}
		else
		{
			can_change	= FALSE;

			if ( !admin )
			{
				send_to_char( "You can not remove people from other clans!\n\r", ch );
			}
		}
	}

	if ( admin )								/* Admins can do anything */
	{
		can_change	= TRUE;
	}

	if ( !can_change )
	{
		do_guild(ch, "");
		return;
	}

	/* Okay, time to make a change */

	if ( remove && can_change )
	{
		clan = victim->clan;
		send_to_char("They are now clanless.\n\r",ch);
		if(!IS_HERO(victim))
		{
		    for ( d = descriptor_list; d != NULL; d = d->next )
		    {
			if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
			&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
			{
			  sprintf( buf, "%s is no longer a member of %s\n\r", PERS(victim, d->character), clan_table[clan].who_name );
			  send_to_char( buf, d->character );
			}
		    }
		}

		if( IS_SET(victim->info,INFO_CLANGM) )
		{
			REMOVE_BIT(victim->info,INFO_CLANGM);
			send_to_char( "You are no longer a Grand Master.\n\r", victim);
		}

		if( IS_SET(victim->info,INFO_CLANTRAINER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANTRAINER);
			send_to_char( "You are no longer a Head Trainer.\n\r", victim );
		}

		if( IS_SET(victim->info,INFO_CLANENFORCER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANENFORCER);
			send_to_char( "You are no longer an Enforcer.\n\r", victim );
		}

		if( IS_SET(victim->act,PLR_CLANLEADER) )
		{
			REMOVE_BIT(victim->act,PLR_CLANLEADER);
			send_to_char( "You are no longer a Clan Leader.\n\r", victim );
		}

		sprintf(buf, "%s has been removed from %s (%s) by %s",
		        victim->name, clan_table[clan].name, clan_table[clan].who_name, ch->name );
		wiznet( buf, NULL, NULL, WIZ_FLAGS, 0, get_trust(ch) );
		sprintf( buf, "%s removed %s from %s.", ch->name, victim->name, clan_table[clan].name );
		log_string( buf );
		send_to_char("You are now a member of no clan!\n\r",victim);
		victim->clan = 0;
		victim->clanreq = 0;
		return;
	}

	if (clan_table[clan].independent && can_change)
	{
		sprintf(buf,"They are now a member of %s.\n\r",clan_table[clan].who_name);
		send_to_char(buf,ch);

		if(!IS_HERO(victim))
		{
		    for ( d = descriptor_list; d != NULL; d = d->next )
		    {
			if ( 	d->connected == CON_PLAYING && IS_HERO(d->character) && can_see( d->character, victim ) )
			{
			  sprintf( buf, "%s is now a member of %s \n\r", PERS(victim, d->character), clan_table[clan].who_name );
			  send_to_char( buf, d->character );
			}
		    }
		}

		if( IS_SET(victim->info,INFO_CLANGM) )
		{
			REMOVE_BIT(victim->info,INFO_CLANGM);
			send_to_char( "You are no longer a Grand Master.\n\r", victim);
		}

		if( IS_SET(victim->info,INFO_CLANTRAINER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANTRAINER);
			send_to_char( "You are no longer a Head Trainer.\n\r", victim );
		}

		if( IS_SET(victim->info,INFO_CLANENFORCER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANENFORCER);
			send_to_char( "You are no longer an Enforcer.\n\r", victim );
		}

		if( IS_SET(victim->act,PLR_CLANLEADER) )
		{
			REMOVE_BIT(victim->act,PLR_CLANLEADER);
			send_to_char( "You are no longer a Clan Leader.\n\r", victim );
		}

		sprintf(buf, "%s has been added to %s (%s) by %s",
			 victim->name, clan_table[clan].name, clan_table[clan].who_name, ch->name );
		wiznet( buf, NULL, NULL, WIZ_FLAGS, 0, get_trust(ch) );
		sprintf( buf, "%s added %s to %s", ch->name, victim->name, clan_table[clan].name );
		log_string( buf );
		sprintf(buf,"You are now a member of %s.\n\r",clan_table[clan].who_name);
		send_to_char(buf,victim);

		victim->clan = clan;
		victim->clanreq = 0;
		return;
	}


	if ( can_change )
	{
		sprintf(buf,"They are now a member of %s.\n\r", clan_table[clan].who_name);
		send_to_char(buf,ch);

		if(!IS_HERO(victim))
		{
		    for ( d = descriptor_list; d != NULL; d = d->next )
		    {
			if ( 	d->connected == CON_PLAYING && can_see( d->character, victim )
			&& (is_same_clan(victim,d->character) || IS_HERO(d->character)) )
			{
			  sprintf( buf, "%s is now a member of %s\n\r", PERS(victim, d->character), clan_table[clan].who_name );
			  send_to_char( buf, d->character );
			}
		    }
		}

		if( IS_SET(victim->info,INFO_CLANGM) )
		{
			REMOVE_BIT(victim->info,INFO_CLANGM);
			send_to_char( "You are no longer a Grand Master.\n\r", victim);
		}

		if( IS_SET(victim->info,INFO_CLANTRAINER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANTRAINER);
			send_to_char( "You are no longer a Head Trainer.\n\r", victim );
		}

		if( IS_SET(victim->info,INFO_CLANENFORCER) )
		{
			REMOVE_BIT(victim->info,INFO_CLANENFORCER);
			send_to_char( "You are no longer an Enforcer.\n\r", victim );
		}

		if( IS_SET(victim->act,PLR_CLANLEADER) )
		{
			REMOVE_BIT(victim->act,PLR_CLANLEADER);
			send_to_char( "You are no longer a Clan Leader.\n\r", victim );
		}

		sprintf(buf, "%s has been added to %s (%s) by %s",
		       victim->name, clan_table[clan].name, clan_table[clan].who_name, ch->name );
		wiznet( buf, NULL, NULL, WIZ_FLAGS, 0, get_trust(ch) );
		sprintf( buf, "%s added %s to %s.", ch->name, victim->name, clan_table[clan].name );
		log_string( buf );
		sprintf(buf,"You are now a member of %s.\n\r",clan_table[clan].who_name);
		send_to_char(buf,victim);

		victim->clan = clan;
		victim->clanreq = 0;
		return;
	}

	bug("Guild: setclan did not set", 0);
	do_guild( ch, "" );
	return;
    }

    do_guild( ch, "" );
    return;
}

/* equips a character */
void do_outfit ( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int i,sn,vnum;

    if (ch->level > 5 || IS_NPC(ch))
    {
	send_to_char("Find it yourself!\n\r",ch);
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) == NULL )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_BANNER), 0 );
	obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_LIGHT );
    }

    if ( ( obj = get_eq_char( ch, WEAR_BODY ) ) == NULL )
    {
	obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_VEST), 0 );
	obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_BODY );
    }

    /* do the weapon thing */
    if ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL)
    {
    	sn = 0;
    	vnum = OBJ_VNUM_SCHOOL_SWORD; /* just in case! */



    	for (i = 0; weapon_table[i].name != NULL; i++)

    	{

	    if (ch->pcdata->learned[sn] <

		ch->pcdata->learned[*weapon_table[i].gsn])

	    {

	    	sn = *weapon_table[i].gsn;

	    	vnum = weapon_table[i].vnum;

	    }

    	}



    	obj = create_object(get_obj_index(vnum),0);

     	obj_to_char(obj,ch);

    	equip_char(ch,obj,WEAR_WIELD);

    }



    if (((obj = get_eq_char(ch,WEAR_WIELD)) == NULL

    ||   !IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))

    &&  (obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )

    {

        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_SHIELD), 0 );

	obj->cost = 0;

        obj_to_char( obj, ch );

        equip_char( ch, obj, WEAR_SHIELD );

    }



    send_to_char("You have been equipped by Mota.\n\r",ch);

}





/* RT nochannels command, for those spammers */

void do_nochannels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH], varg[MAX_STRING_LENGTH];
    int value;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, varg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Nochannel whom?", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if ( varg[0] == '\0' )
	value = 0;
    else
	value = atoi( varg );

    if ( value > get_econ_value( "max_punish" ) )
    {
	sprintf( buf, "You may only NOCHANNEL someone for up to %d ticks.\n\r", get_econ_value( "max_punish" ) );
	send_to_char( buf, ch );
	return;
    }

    if ( !IS_ADMIN(ch) && (value == 0) )
    {
	send_to_char( "Only Administrators may NOCHANNEL someone indefinately.\n\r", ch );
	return;
    }

	if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
	{
	REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
	if(!IS_NPC(victim))//this if should stop the mud from crashing when NPCs are nochanned, as it is now
		victim->pcdata->timer_punished[PUNISH_NOCHAN] = 0; // 'cause NPCs don't have pcdata
	send_to_char( "The gods have restored your channel priviliges.\n\r", victim );
	sprintf( buf, "{cNOCHANNELS{w on {m%s{w have been removed.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name) );
	send_to_char( buf, ch );
	sprintf(buf, "%s restores channels to %s", ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name) );
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	log_string( buf );
	}
	else
	{
	SET_BIT(victim->comm, COMM_NOCHANNELS);
	if(!IS_NPC(victim))
		victim->pcdata->timer_punished[PUNISH_NOCHAN] = value; //NPCs don't have pcdata
	sprintf( buf, "The gods have revoked your channel priviliges for %d ticks.\n\r", value );
	send_to_char( buf, victim );
	sprintf( buf, "{m%s{w has {cNOCHANNELS{w set for {g%d{w ticks.\n\r",
		(IS_NPC(victim) ? victim->short_descr : victim->name),
		value );
	send_to_char( buf, ch );
	sprintf(buf,"%s revokes channels from %s for %d ticks",
		ch->name,
		(IS_NPC(victim) ? victim->short_descr : victim->name),
		value);
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	log_string( buf );
	if(!IS_NPC(victim)) // can't do these to NPCs...
	{
	free_string( victim->pcdata->title );
	victim->pcdata->title 	= str_dup( " the {rNo Channeled{w. " );
	free_string( victim->pcdata->roomin );
	victim->pcdata->roomin 	= str_dup( "" );
	free_string( victim->pcdata->roomout );
	victim->pcdata->roomin 	= str_dup( "" );
	free_string( victim->pcdata->bamfin );
	victim->pcdata->roomin 	= str_dup( "" );
	free_string( victim->pcdata->bamfout );
	victim->pcdata->roomin 	= str_dup( "" );
	}
	}
    return;
}





void do_smote(CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *vch;

    char *letter,*name;

    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];

    int matches = 0;



    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )

    {

        send_to_char( "You can't show your emotions.\n\r", ch );

        return;

    }



    if ( argument[0] == '\0' )

    {

        send_to_char( "Emote what?\n\r", ch );

        return;

    }



    if (strstr(argument,ch->name) == NULL)

    {

	send_to_char("You must include your name in an smote.\n\r",ch);

	return;

    }



    send_to_char(argument,ch);

    send_to_char("\n\r",ch);



    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)

    {

        if (vch->desc == NULL || vch == ch)

            continue;



        if ((letter = strstr(argument,vch->name)) == NULL)

        {

	    send_to_char(argument,vch);

	    send_to_char("\n\r",vch);

            continue;

        }



        strcpy(temp,argument);

        temp[strlen(argument) - strlen(letter)] = '\0';

        last[0] = '\0';

        name = vch->name;



        for (; *letter != '\0'; letter++)

        {

            if (*letter == '\'' && matches == strlen(vch->name))

            {

                strcat(temp,"r");

                continue;

            }



            if (*letter == 's' && matches == strlen(vch->name))

            {

                matches = 0;

                continue;

            }



            if (matches == strlen(vch->name))

            {

                matches = 0;

            }



            if (*letter == *name)

            {

                matches++;

                name++;

                if (matches == strlen(vch->name))

                {

                    strcat(temp,"you");

                    last[0] = '\0';

                    name = vch->name;

                    continue;

                }

                strncat(last,letter,1);

                continue;

            }



            matches = 0;

            strcat(temp,last);

            strncat(temp,letter,1);

            last[0] = '\0';

            name = vch->name;

        }



	send_to_char(temp,vch);

	send_to_char("\n\r",vch);

    }



    return;

}



void do_bamfin( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you a poofin.\n\r", ch);

	return;

    }



    if ( !IS_NPC(ch) )

    {

	smash_tilde( argument );



	if (argument[0] == '\0')

	{

	    sprintf(buf,"Your poofin is %s\n\r",ch->pcdata->bamfin);

	    send_to_char(buf,ch);

	    return;

	}



	if ( strstr(argument,ch->name) == NULL)

	{

	    send_to_char("You must include your name.\n\r",ch);

	    return;

	}



	free_string( ch->pcdata->bamfin );

	ch->pcdata->bamfin = str_dup( argument );



        sprintf(buf,"Your poofin is now %s\n\r",ch->pcdata->bamfin);

        send_to_char(buf,ch);

    }

    return;

}







void do_bamfout( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you a poofout.\n\r", ch);

	return;

    }



    if ( !IS_NPC(ch) )

    {

        smash_tilde( argument );



        if (argument[0] == '\0')

        {

            sprintf(buf,"Your poofout is %s\n\r",ch->pcdata->bamfout);

            send_to_char(buf,ch);

            return;

        }



        if ( strstr(argument,ch->name) == NULL)

        {

            send_to_char("You must include your name.\n\r",ch);

            return;

        }



        free_string( ch->pcdata->bamfout );

        ch->pcdata->bamfout = str_dup( argument );



        sprintf(buf,"Your poofout is now %s\n\r",ch->pcdata->bamfout);

        send_to_char(buf,ch);

    }

    return;

}





void do_roomin( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you a roomin.\n\r", ch);

	return;

    }



    if ( !IS_NPC(ch) )

    {

	smash_tilde( argument );



	if (argument[0] == '\0')

	{

	    sprintf(buf,"Your roomin is %s %s\n\r", ch->name, ch->pcdata->roomin);

	    send_to_char(buf,ch);

	    return;

	}



	free_string( ch->pcdata->roomin );

	ch->pcdata->roomin = str_dup( argument );



        sprintf(buf,"Your roomin is now %s %s\n\r", ch->name, ch->pcdata->roomin);

        send_to_char(buf,ch);

    }

    return;

}







void do_roomout( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you a roomout.\n\r", ch);

	return;

    }



    if ( !IS_NPC(ch) )

    {

        smash_tilde( argument );



        if (argument[0] == '\0')

        {

            sprintf(buf,"Your roomout is %s %s\n\r",ch->name,ch->pcdata->roomout);

            send_to_char(buf,ch);

            return;

        }



        free_string( ch->pcdata->roomout );

        ch->pcdata->roomout = str_dup( argument );



        sprintf(buf,"Your roomout is now %s %s\n\r",ch->name,ch->pcdata->roomout);

        send_to_char(buf,ch);

    }

    return;

}


void do_wiziin( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );

        if (argument[0] == '\0')
        {
            sprintf(buf,"Your wiziin is %s\n\r",ch->pcdata->wiziin);
            send_to_char(buf,ch);
            return;
        }

        if ( strstr(argument,ch->name) == NULL)
        {
            send_to_char("You must include your name.\n\r",ch);
            return;
        }

        free_string( ch->pcdata->wiziin );
        ch->pcdata->wiziin = str_dup( argument );

        sprintf(buf,"Your wiziin is now %s\n\r",ch->pcdata->wiziin);
        send_to_char(buf,ch);
    }
    return;
}

void do_wiziout( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );

        if (argument[0] == '\0')
        {
            sprintf(buf,"Your wiziout is %s\n\r",ch->pcdata->wiziout);
            send_to_char(buf,ch);
            return;
        }

        if ( strstr(argument,ch->name) == NULL)
        {
            send_to_char("You must include your name.\n\r",ch);
            return;
        }

        free_string( ch->pcdata->wiziout );
        ch->pcdata->wiziout = str_dup( argument );

        sprintf(buf,"Your wiziout is now %s\n\r",ch->pcdata->wiziout);
        send_to_char(buf,ch);
    }
    return;
}



void do_deny( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];

    CHAR_DATA *victim;



    one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	send_to_char( "Deny whom?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    }



    if ( get_trust( victim ) >= get_trust( ch ) )

    {

	send_to_char( "You failed.\n\r", ch );

	return;

    }



    SET_BIT(victim->act, PLR_DENY);

    send_to_char( "You are denied access!\n\r", victim );

    sprintf(buf,"%s denies access to %s",ch->name, victim->name);

    wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));

    log_string( buf );

    send_to_char( "OK.\n\r", ch );

    save_char_obj(victim);

    stop_fighting(victim,TRUE);

    do_quit( victim, "" );



    return;

}







void do_disconnect( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    DESCRIPTOR_DATA *d;

    CHAR_DATA *victim;



    one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	send_to_char( "Disconnect whom?\n\r", ch );

	return;

    }



    if (is_number(arg))

    {

	int desc;



	desc = atoi(arg);

    	for ( d = descriptor_list; d != NULL; d = d->next )

    	{

            if ( d->descriptor == desc )

            {

            	close_socket( d );

            	send_to_char( "Ok.\n\r", ch );

            	return;

            }

	}

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( victim->desc == NULL )

    {

	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );

	return;

    }



    for ( d = descriptor_list; d != NULL; d = d->next )

    {

	if ( d == victim->desc )

	{

	    close_socket( d );

	    send_to_char( "Ok.\n\r", ch );

	    return;

	}

    }



    bug( "Do_disconnect: desc not found.", 0 );

    send_to_char( "Descriptor not found!\n\r", ch );

    return;

}







void do_pardon( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if ( arg1[0] == '\0' || arg2[0] == '\0' )

    {

	send_to_char( "Syntax: pardon <character> <killer|thief|wanted|bozo>.\n\r", ch );

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



    if ( !str_cmp( arg2, "killer" ) )

    {

	if ( IS_SET(victim->act, PLR_KILLER) )

	{

	    REMOVE_BIT( victim->act, PLR_KILLER );

	    send_to_char( "Killer flag removed.\n\r", ch );

	    send_to_char( "You are no longer a KILLER.\n\r", victim );

	}

	return;

    }



    if ( !str_cmp( arg2, "thief" ) )

    {

	if ( IS_SET(victim->act, PLR_THIEF) )

	{

	    REMOVE_BIT( victim->act, PLR_THIEF );

	    send_to_char( "Thief flag removed.\n\r", ch );

	    send_to_char( "You are no longer a THIEF.\n\r", victim );

	}

	return;

    }



    if ( !str_cmp( arg2, "wanted" ) )

    {

	if ( IS_SET(victim->act, PLR_WANTED) )

	{

	    REMOVE_BIT( victim->act, PLR_WANTED );

	    send_to_char( "Wanted flag removed.\n\r", ch );

	    send_to_char( "You are no longer wanted by the law.\n\r", victim );

	}

	return;

    }



    if ( !str_cmp( arg2, "bozo" ) )

    {

	if (!IS_ADMIN(ch))

	{

		send_to_char( "Only Administrators may pardon {R({GB{YO{GZ{YO{R){ws!\n\r", ch );

		return;

	}



	if ( IS_SET(victim->info, INFO_BOZO) )

	{

	    REMOVE_BIT( victim->info, INFO_BOZO );

	    send_to_char( "{R({GB{YO{GZ{YO{R){w flag removed.\n\r", ch );

	    send_to_char( "You are no longer a {R({GB{YO{GZ{YO{R){w.\n\r", victim );

	}

	return;

    }



    send_to_char( "Syntax: pardon <character> <killer|thief|wanted|bozo>.\n\r", ch );

     return;

}







void do_echo( CHAR_DATA *ch, char *argument )

{

    DESCRIPTOR_DATA *d;



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you the ability to Globally Echo.\n\r", ch);

	return;

    }



    if ( argument[0] == '\0' )

    {

	send_to_char( "Global echo what?\n\r", ch );

	return;

    }



    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING )

	{

	    if (get_trust(d->character) >= get_trust(ch))

		send_to_char( "global> ",d->character);

	    send_to_char( argument, d->character );

	    send_to_char( "{w\n\r",   d->character );

	}

    }



    return;

}







void do_recho( CHAR_DATA *ch, char *argument )

{

    DESCRIPTOR_DATA *d;



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you the ability to Room Echo.\n\r", ch);

	return;

    }



    if ( argument[0] == '\0' )

    {

	send_to_char( "Local echo what?\n\r", ch );



	return;

    }



    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == ch->in_room )

	{

            if (get_trust(d->character) >= get_trust(ch))

                send_to_char( "local> ",d->character);

	    send_to_char( argument, d->character );

	    send_to_char( "\n\r",   d->character );

	}

    }



    return;

}



void do_zecho(CHAR_DATA *ch, char *argument)

{

    DESCRIPTOR_DATA *d;



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you the ability to Zone Echo.\n\r", ch);

	return;

    }



    if (argument[0] == '\0')

    {

	send_to_char("Zone echo what?\n\r",ch);

	return;

    }



    for (d = descriptor_list; d; d = d->next)

    {

	if (d->connected == CON_PLAYING

	&&  d->character->in_room != NULL && ch->in_room != NULL

	&&  d->character->in_room->area == ch->in_room->area)

	{

	    if (get_trust(d->character) >= get_trust(ch))

		send_to_char("zone> ",d->character);

	    send_to_char(argument,d->character);

	    send_to_char("\n\r",d->character);

	}

    }

}



void do_pecho( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have denied you the ability to Echo to a Person.\n\r", ch);

	return;

    }



    argument = one_argument(argument, arg);



    if ( argument[0] == '\0' || arg[0] == '\0' )

    {

	send_to_char("Personal echo what?\n\r", ch);

	return;

    }



    if  ( (victim = get_char_world(ch, arg) ) == NULL )

    {

	send_to_char("Target not found.\n\r",ch);

	return;

    }



    if (get_trust(victim) >= get_trust(ch) && get_trust(ch) != MAX_LEVEL)

        send_to_char( "personal> ",victim);



    send_to_char(argument,victim);

    send_to_char("\n\r",victim);

    send_to_char( "personal> ",ch);

    send_to_char(argument,ch);

    send_to_char("\n\r",ch);

}





ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )

{

    CHAR_DATA *victim;

    OBJ_DATA *obj;



    if ( is_number(arg) )

	return get_room_index( atoi( arg ) );



    if ( ( victim = get_char_world( ch, arg ) ) != NULL )

	return victim->in_room;



    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )

	return obj->in_room;



    return NULL;

}







void do_transfer( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    ROOM_INDEX_DATA *location;

    DESCRIPTOR_DATA *d;

    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];

    bool to_prison;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if ( arg1[0] == '\0' )

    {

	send_to_char( "Transfer whom (and where)?\n\r", ch );

	return;

    }



    if ( !str_cmp( arg1, "all" ) )

    {

	for ( d = descriptor_list; d != NULL; d = d->next )

	{

	    if ( d->connected == CON_PLAYING

	    &&   d->character != ch

	    &&   d->character->in_room != NULL

	    &&   can_see( ch, d->character ) )

	    {

		sprintf( buf, "%s %s", d->character->name, arg2 );

		do_transfer( ch, buf );

	    }

	}

	return;

    }



    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( victim->in_room == NULL )

    {

	send_to_char( "They are in limbo.\n\r", ch );

	return;

    }



    /*

     * Thanks to Grodyn for the optional location parameter.

     */

    if ( arg2[0] == '\0' )

    {

	location = ch->in_room;

    }

    else

    {

	if ( ( location = find_location( ch, arg2 ) ) == NULL )

	{

	    send_to_char( "No such location.\n\r", ch );

	    return;

	}



	if ( !is_room_owner(ch,location) && room_is_private( location )

	&&  get_trust(ch) < MAX_LEVEL)

	{

	    send_to_char( "That room is private right now.\n\r", ch );

	    return;

	}



	to_prison	= TRUE;		/* Can we send them to Prison */



	if (		(location == get_room_index(ROOM_VNUM_JAIL))

		||	(location == get_room_index(ROOM_VNUM_PRISON))

		||	(location == get_room_index(ROOM_VNUM_SC01))

		||	(location == get_room_index(ROOM_VNUM_SC02))

		||	(location == get_room_index(ROOM_VNUM_SC03))

		||	(location == get_room_index(ROOM_VNUM_SC04))

		||	(location == get_room_index(ROOM_VNUM_SC05))

	   )

		to_prison	= FALSE;		/* Well, first are we sending them there */



	if ( IS_NPC(victim) )

		to_prison	= TRUE;		/* We can send NPC's */



	if ( IS_IMMORTAL(victim) )		/* We can send IMM's */

		to_prison	= TRUE;



	if ( IS_ADMIN(ch) )			/* If we are an ADMIN, we can do anything */

		to_prison	= TRUE;



	if ( !to_prison )

	{

		send_to_char( "You may not use Transfer to send someone to a Prison Cell.\n\r", ch );

		sprintf( buf,

			"Prison Rooms are:\n\r     %d\n\r     %d\n\r     %d\n\r     %d\n\r     %d\n\r     %d\n\r     %d{w\n\r",

			ROOM_VNUM_JAIL,

			ROOM_VNUM_PRISON,

			ROOM_VNUM_SC01,

			ROOM_VNUM_SC02,

			ROOM_VNUM_SC03,

			ROOM_VNUM_SC04,

			ROOM_VNUM_SC05 );

		send_to_char( buf, ch );

		send_to_char( "Use JAIL to send someone to prison.\n\r", ch );

		return;

	}

    }



    if ( victim->fighting != NULL )

	stop_fighting( victim, TRUE );

    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == victim->in_room

      &&   can_see(d->character, victim)

	&&   d->character != victim )

	{

	  if ( !IS_NPC(victim) )

	  {

		sprintf(buf, "%s disappears in a mushroom cloud.\n\r", victim->name );

		send_to_char( buf, d->character );

	  }

	  else

	  {

		sprintf(buf, "%s disappears in a mushroom cloud.\n\r", victim->short_descr );

		send_to_char( buf, d->character );

	  }

    	}

    }

    char_from_room( victim );

    char_to_room( victim, location );

    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == victim->in_room

      &&   can_see(d->character, victim)

	&&   d->character != victim )

	{

	  if ( !IS_NPC(victim) )

	  {

		sprintf(buf, "%s arrives from a puff of smoke.\n\r", victim->name );

		send_to_char( buf, d->character );

	  }

	  else

	  {

		sprintf(buf, "%s arrives from a puff of smoke.\n\r", victim->short_descr );

		send_to_char( buf, d->character );

	  }

    	}

    }

    if ( ch != victim )

	act( "$n has transferred you.", ch, NULL, victim, TO_VICT );

    do_look( victim, "auto" );

    victim->position = POS_STANDING;

    send_to_char( "Ok.\n\r", ch );



    return;

}







void do_at( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    ROOM_INDEX_DATA *location;

    ROOM_INDEX_DATA *original;

    OBJ_DATA *on;

    CHAR_DATA *wch;



    argument = one_argument( argument, arg );



    if ( arg[0] == '\0' || argument[0] == '\0' )

    {

	send_to_char( "At where what?\n\r", ch );

	return;

    }



    if ( ( location = find_location( ch, arg ) ) == NULL )

    {

	send_to_char( "No such location.\n\r", ch );

	return;

    }



    if (!is_room_owner(ch,location) && room_is_private( location )

    &&  get_trust(ch) < MAX_LEVEL)

    {

	send_to_char( "That room is private right now.\n\r", ch );

	return;

    }



    original = ch->in_room;

    on = ch->on;

    char_from_room( ch );

    char_to_room( ch, location );

    interpret( ch, argument );



    /*

     * See if 'ch' still exists before continuing!

     * Handles 'at XXXX quit' case.

     */

    for ( wch = char_list; wch != NULL; wch = wch->next )

    {

	if ( wch == ch )

	{

	    char_from_room( ch );

	    char_to_room( ch, original );

	    ch->on = on;

	    break;

	}

    }



    return;

}







void do_goto( CHAR_DATA *ch, char *argument )

{

    ROOM_INDEX_DATA *location;

    CHAR_DATA *rch;

    int count = 0;



    if ( argument[0] == '\0' )

    {

	send_to_char( "Goto where?\n\r", ch );

	return;

    }



    if ( ( location = find_location( ch, argument ) ) == NULL )

    {

	send_to_char( "No such location.\n\r", ch );

	return;

    }



    count = 0;

    for ( rch = location->people; rch != NULL; rch = rch->next_in_room )

        count++;



    if (!is_room_owner(ch,location) && room_is_private(location)

    &&  (count > 1 || get_trust(ch) < MAX_LEVEL))

    {

	send_to_char( "That room is private right now.\n\r", ch );

	return;

    }



    if ( ch->fighting != NULL )

	stop_fighting( ch, TRUE );



    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)

    {

	if (get_trust(rch) >= ch->invis_level)

	{

	    if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')

		act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);

	    else

		act("$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);

	}

    }



    char_from_room( ch );

    char_to_room( ch, location );





    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)

    {

        if (get_trust(rch) >= ch->invis_level)

        {

            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')

                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);

            else

                act("$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);

        }

    }



    ch->position = POS_STANDING;

    do_look( ch, "auto" );

    return;

}



void do_violate( CHAR_DATA *ch, char *argument )

{

    ROOM_INDEX_DATA *location;

    CHAR_DATA *rch;



    if ( argument[0] == '\0' )

    {

        send_to_char( "Goto where?\n\r", ch );

        return;

    }



    if ( ( location = find_location( ch, argument ) ) == NULL )

    {

        send_to_char( "No such location.\n\r", ch );

        return;

    }



    if (!room_is_private( location ))

    {

        send_to_char( "That room isn't private, use goto.\n\r", ch );

        return;

    }



    if ( ch->fighting != NULL )

        stop_fighting( ch, TRUE );



    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)

    {

        if (get_trust(rch) >= ch->invis_level)

        {

            if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')

                act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);

            else

                act("$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);

        }

    }



    char_from_room( ch );

    char_to_room( ch, location );





    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)

    {

        if (get_trust(rch) >= ch->invis_level)

        {

            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')

                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);

            else

                act("$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);

        }

    }



    ch->position = POS_STANDING;

    do_look( ch, "auto" );

    return;

}



/* RT to replace the 3 stat commands */



void do_stat ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  stat <name>\n\r",ch);
	send_to_char("  stat obj <name>\n\r",ch);
	send_to_char("  stat mob <name>\n\r",ch);
 	send_to_char("  stat room <number>\n\r",ch);
	return;
   }

   if (!str_cmp(arg,"room"))
   {
	do_rstat(ch,string);
	return;
   }

   if (!str_cmp(arg,"obj"))
   {
	do_ostat(ch,string);
	return;
   }

   if(!str_cmp(arg,"char")  || !str_cmp(arg,"mob"))
   {
	do_mstat(ch,string);
	return;
   }

   /* do it the old way */

   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_ostat(ch,argument);
     return;
   }

  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_mstat(ch,argument);
    return;
  }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_rstat(ch,argument);
    return;
  }

  send_to_char("Nothing by that name found anywhere.\n\r",ch);
  return;
}





void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location
    &&  room_is_private( location ) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "{wName: '%s'\n\r{wArea:{g '%s'{w\n\r",
	location->name,
	location->area->name );
    send_to_char( buf, ch );

    sprintf( buf,
	"{wVnum: {g%d{w  Sector: {g%d{w  Light: {y%d{w  Healing: {r%d{w  Mana: {b%d{w\n\r",
	location->vnum,
	location->sector_type,
	location->light,
	location->heal_rate,
	location->mana_rate );
    send_to_char( buf, ch );

    sprintf( buf,
	"{wRoom flags:{y %s\n\r{wDescription:\n\r%s{w",
	room_bit_name(location->room_flags),
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "{wExtra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'{w.\n\r", ch );
    }

    send_to_char( "{wCharacters:{g", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	if (can_see(ch,rch))
        {
	    send_to_char( " ", ch );
	    one_argument( rch->name, buf );
	    send_to_char( buf, ch );
	}
    }

    send_to_char( "\n\r{wObjects:{g   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( "{w\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"{wDoor: {y%d{w  To: {g%d{w  Key: {y%d{w  Exit flags: {y%d{w\n\rKeyword: {y'%s'{w  Description: %s{w",

		door,
		(pexit->u1.to_room == NULL ? -1 : pexit->u1.to_room->vnum),
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "{W(none){w\n\r" );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    AFFECT_DATA *paf;

    OBJ_DATA *obj;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Stat what?\n\r", ch );

	return;

    }



    if ( ( obj = get_obj_world( ch, argument ) ) == NULL )

    {

	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

	return;

    }



    if (obj->carried_by != NULL)

    {

	if (!can_see(ch,obj->carried_by))

	{

	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

	return;

    	}

    }



    sprintf( buf, "Name(s):{g %s{w\n\r",

	obj->name );

    send_to_char( buf, ch );



    sprintf( buf, "{wVnum: {g%d{w  Format: {y%s{w  Type: {y%s{w  Resets: {y%d{w\n\r",

	obj->pIndexData->vnum, obj->pIndexData->new_format ? "new" : "old",

	item_name(obj->item_type), obj->pIndexData->reset_num );

    send_to_char( buf, ch );



    sprintf( buf, "{wShort description: %s\n\r{wLong description: %s{w\n\r",

	obj->short_descr, obj->description );

    send_to_char( buf, ch );



    sprintf( buf, "{wWear bits: {r%s\n\r{wExtra bits: {r%s{w\n\r",

	wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags ) );

    send_to_char( buf, ch );



    sprintf( buf, "{wNumber: {y%d{w/{y%d{w  Weight: {y%d{w/{y%d{w/{y%d{w (10th pounds)\n\r",

	1,           get_obj_number( obj ),

	obj->weight, get_obj_weight( obj ),get_true_weight(obj) );

    send_to_char( buf, ch );



    sprintf( buf, "{wLevel: {g%s{w  Cost: {y%d{w  Condition: {r%d{w  Timer: {b%d{w\n\r",

	lev_name(obj->level), obj->cost, obj->condition, obj->timer );

    send_to_char( buf, ch );



    sprintf( buf,

	"{wIn room: {g%d{w  In object: {g%s{w  Carried by: {y%s{w  Wear_loc: {y%d{w\n\r",

	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,

	obj->in_obj     == NULL    ? "{W(none){w" : obj->in_obj->short_descr,

	obj->carried_by == NULL    ? "{W(none){w" :

	    can_see(ch,obj->carried_by) ? obj->carried_by->name

				 	: "someone",

	obj->wear_loc );

    send_to_char( buf, ch );



    sprintf( buf, "{wValues: {g%d %d %d %d %d{w\n\r",

	obj->value[0], obj->value[1], obj->value[2], obj->value[3],

	obj->value[4] );

    send_to_char( buf, ch );



    /* now give out vital statistics as per identify */



    switch ( obj->item_type )

    {

    	case ITEM_SCROLL:

    	case ITEM_POTION:

    	case ITEM_PILL:

	    sprintf( buf, "{wLevel {g%d{w spells of:{g", obj->value[0] );

	    send_to_char( buf, ch );



	    if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )

	    {

	    	send_to_char( " '", ch );

	    	send_to_char( skill_table[obj->value[1]].name, ch );

	    	send_to_char( "'", ch );

	    }



	    if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )

	    {

	    	send_to_char( " '", ch );

	    	send_to_char( skill_table[obj->value[2]].name, ch );

	    	send_to_char( "'", ch );

	    }



	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )

	    {

	    	send_to_char( " '", ch );

	    	send_to_char( skill_table[obj->value[3]].name, ch );

	    	send_to_char( "'", ch );

	    }



	    if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)

	    {

		send_to_char(" '",ch);

		send_to_char(skill_table[obj->value[4]].name,ch);

		send_to_char("'",ch);

	    }



	    send_to_char( ".\n\r", ch );

	break;



    	case ITEM_WAND:

    	case ITEM_STAFF:

	    sprintf( buf, "{wHas {y%d{w({g%d{w) charges of level {g%d{w",

	    	obj->value[1], obj->value[2], obj->value[0] );

	    send_to_char( buf, ch );



	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )

	    {

	    	send_to_char( " '", ch );

	    	send_to_char( skill_table[obj->value[3]].name, ch );

	    	send_to_char( "'", ch );

	    }



	    send_to_char( ".\n\r", ch );

	break;



	case ITEM_DRINK_CON:

	    sprintf(buf,"{wIt holds %s-colored %s.{w\n\r",

		liq_table[obj->value[2]].liq_color,

		liq_table[obj->value[2]].liq_name);

	    send_to_char(buf,ch);

	    break;





    	case ITEM_WEAPON:

 	    send_to_char("{wWeapon type is{r ",ch);

	    switch (obj->value[0])

	    {

	    	case(WEAPON_EXOTIC):

		    send_to_char("exotic\n\r{w",ch);

		    break;

	    	case(WEAPON_SWORD):

		    send_to_char("sword\n\r{w",ch);

		    break;

	    	case(WEAPON_DAGGER):

		    send_to_char("dagger\n\r{w",ch);

		    break;

	    	case(WEAPON_SPEAR):

		    send_to_char("spear/staff\n\r{w",ch);

		    break;

	    	case(WEAPON_MACE):

		    send_to_char("mace/club\n\r{w",ch);

		    break;

	   	case(WEAPON_AXE):

		    send_to_char("axe\n\r{w",ch);

		    break;

	    	case(WEAPON_FLAIL):

		    send_to_char("flail\n\r{w",ch);

		    break;

	    	case(WEAPON_WHIP):

		    send_to_char("whip\n\r{w",ch);

		    break;

	    	case(WEAPON_POLEARM):

		    send_to_char("polearm\n\r{w",ch);

		    break;

			case(WEAPON_PROJ):

		    send_to_char("projectile\n\r{w",ch);

		    break;

		    case(WEAPON_BTL):
		    send_to_char("bat'telh\n\r{w",ch);
		    break;

	    	default:

		    send_to_char("unknown\n\r{w",ch);

		    break;

 	    }

	    if (obj->pIndexData->new_format)

	    	sprintf(buf,"{wDamage is {r%dd%d{w (average {b%d{w)\n\r",

		    obj->value[1],obj->value[2],

		    (1 + obj->value[2]) * obj->value[1] / 2);

	    else

	    	sprintf( buf, "{wDamage is {r%d{w to {r%d{w (average {b%d{w)\n\r",

	    	    obj->value[1], obj->value[2],

	    	    ( obj->value[1] + obj->value[2] ) / 2 );

	    send_to_char( buf, ch );



	    sprintf(buf,"{wDamage noun is {r%s{w.\n\r",

		(obj->value[3] > 0 && obj->value[3] < MAX_DAMAGE_MESSAGE) ?

		    attack_table[obj->value[3]].noun : "undefined");

	    send_to_char(buf,ch);



	    if (obj->value[4])  /* weapon flags */

	    {

	        sprintf(buf,"{wWeapons flags:{r %s{w\n\r",

		    weapon_bit_name(obj->value[4]));

	        send_to_char(buf,ch);

            }

	break;



    	case ITEM_ARMOR:

	    sprintf( buf,

	    "{wArmor class is {y%d{w pierce, {y%d{w bash, {y%d{w slash, and {y%d{w vs. magic{w\n\r",

	        obj->value[0], obj->value[1], obj->value[2], obj->value[3] );

	    send_to_char( buf, ch );

	break;



        case ITEM_CONTAINER:

            sprintf(buf,"{wCapacity: {y%d#  {wMaximum weight: {y%d#  {wflags: {y%s{w\n\r",

                obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));

            send_to_char(buf,ch);

            if (obj->value[4] != 100)

            {

                sprintf(buf,"{wWeight multiplier:{y %d%%{w\n\r",

		    obj->value[4]);

                send_to_char(buf,ch);

            }

        break;

    }





    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )

    {

	EXTRA_DESCR_DATA *ed;



	send_to_char( "{wExtra description keywords:{w '", ch );



	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )

	{

	    send_to_char( ed->keyword, ch );

	    if ( ed->next != NULL )

	    	send_to_char( " ", ch );

	}



	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )

	{

	    send_to_char( ed->keyword, ch );

	    if ( ed->next != NULL )

		send_to_char( " ", ch );

	}



	send_to_char( "'{w\n\r", ch );

    }



    for ( paf = obj->affected; paf != NULL; paf = paf->next )

    {

	sprintf( buf, "{wAffects {b%s{w by {b%d{w, level {b%d{w",

	    affect_loc_name( paf->location ), paf->modifier,paf->level );

	send_to_char(buf,ch);

	if ( paf->duration > -1)

	    sprintf(buf,", {y%d{g hours.{w\n\r",paf->duration);

	else

	    sprintf(buf,".{w\n\r");

	send_to_char( buf, ch );

	if (paf->bitvector)

	{

	    switch(paf->where)

	    {

		case TO_AFFECTS:

		    sprintf(buf,"{bAdds %s affect.{w\n",

			affect_bit_name(paf->bitvector));

		    break;

                case TO_WEAPON:

                    sprintf(buf,"{rAdds %s weapon flags.{w\n",

                        weapon_bit_name(paf->bitvector));

		    break;

		case TO_OBJECT:

		    sprintf(buf,"{gAdds %s object flag.{w\n",

			extra_bit_name(paf->bitvector));

		    break;

		case TO_IMMUNE:

		    sprintf(buf,"{yAdds immunity to %s.{w\n",

			imm_bit_name(paf->bitvector));

		    break;

		case TO_RESIST:

		    sprintf(buf,"{wAdds resistance to %s.\n\r",

			imm_bit_name(paf->bitvector));

		    break;

		case TO_VULN:

		    sprintf(buf,"{wAdds vulnerability to %s.\n\r",

			imm_bit_name(paf->bitvector));

		    break;

		default:

		    sprintf(buf,"{WUnknown bit %d: %d{w\n\r",

			paf->where,paf->bitvector);

		    break;

	    }

	    send_to_char(buf,ch);

	}

    }



    if (!obj->enchanted)

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )

    {

	sprintf( buf, "{wAffects {y%s{w by {y%d{w, level {y%d{w.{w\n\r",

	    affect_loc_name( paf->location ), paf->modifier,paf->level );

	send_to_char( buf, ch );

        if (paf->bitvector)

        {

            switch(paf->where)

            {

                case TO_AFFECTS:

                    sprintf(buf,"{bAdds %s affect.{w\n",

                        affect_bit_name(paf->bitvector));

                    break;

                case TO_OBJECT:

                    sprintf(buf,"{gAdds %s object flag.{w\n",

                        extra_bit_name(paf->bitvector));

                    break;

                case TO_IMMUNE:

                    sprintf(buf,"{yAdds immunity to %s.{w\n",

                        imm_bit_name(paf->bitvector));

                    break;

                case TO_RESIST:

                    sprintf(buf,"{wAdds resistance to %s.\n\r",

                        imm_bit_name(paf->bitvector));

                    break;

                case TO_VULN:

                    sprintf(buf,"{wAdds vulnerability to %s.\n\r",

                        imm_bit_name(paf->bitvector));

                    break;

                default:

                    sprintf(buf,"{WUnknown bit %d: %d{w\n\r",

                        paf->where,paf->bitvector);

                    break;

            }

            send_to_char(buf,ch);

        }

    }



    return;

}







void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;
    extern bool global_trident;
    extern CHAR_DATA *global_trident_target;
    extern int global_trident_timer;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    sprintf( buf, "{wName:{g %s\n\r",
	victim->name);
    send_to_char( buf, ch );

    sprintf( buf,
	"{wVnum: {y%d{w  Format: {y%s{w  Race: {g%s{w  Group: {y%d{w  Sex: {y%s{w  Room: {y%d{w\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	IS_NPC(victim) ? victim->pIndexData->new_format ? "new" : "old" : "pc",
	race_table[victim->race].name,
	IS_NPC(victim) ? victim->group : 0, sex_table[victim->sex].name,
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    if (IS_NPC(victim))
    {
	sprintf(buf,"{gCount:{w %d  {gKilled:{w %d\n\r",
	    victim->pIndexData->count,victim->pIndexData->killed);
	send_to_char(buf,ch);
    }

    sprintf( buf,
	"{wLv: {c%s{w  Class: {g%s{w  Align: {y%d{w  Gold: {y%ld{w  Silver: {y%ld{w  Bank: {y%ld{w  Exp: {g%d{w\n\r",
	lev_name(victim->level),
	IS_NPC(victim) ? "mobile" : class_table[victim->class].name,
	victim->alignment,
	victim->gold, victim->silver, victim->bank, victim->exp );
    send_to_char( buf, ch );

    if (get_trust(ch) >= get_trust(victim))
    {
	sprintf( buf, "{wTrust: {c%s\n\r", lev_name(get_trust(victim)));
	send_to_char( buf, ch );
    }
	if(!IS_NPC(victim))
	{
		if (victim->pcdata->jedi == 0)
		{
			sprintf(buf, "{wJedi:{c FALSE{w\n\r");
		}
		else
		{
			if (victim->pcdata->jedi == 1)
				sprintf(buf, "{wJedi:{c TRUE{w\n\r");
			else
				sprintf(buf, "{wJedi:{c Error{w\n\r");
		}
		send_to_char( buf, ch );
	}
    if ( !IS_NPC(victim) && victim->pcdata->incarnations > 1 )
    {
    	sprintf( buf, "Incarnations: {c%d{w\n\r", victim->pcdata->incarnations );
    	send_to_char( buf, ch );
    }

    sprintf( buf,
   	"{wStr: {y%d{g({y%d{g){w  Int: {y%d{g({y%d{g){w  Wis: {y%d{g({y%d{g){w  Dex: {y%d{g({y%d{g){w  Con: {y%d{g({y%d{g){w\n\r",
	victim->perm_stat[STAT_STR],
	get_curr_stat(victim,STAT_STR),
	victim->perm_stat[STAT_INT],
	get_curr_stat(victim,STAT_INT),
	victim->perm_stat[STAT_WIS],
	get_curr_stat(victim,STAT_WIS),
	victim->perm_stat[STAT_DEX],
	get_curr_stat(victim,STAT_DEX),
	victim->perm_stat[STAT_CON],
	get_curr_stat(victim,STAT_CON) );
    send_to_char( buf, ch );
    
    sprintf( buf,
   	"{wAgi: {y%d{g({y%d{g){w  Mch: {y%d{g({y%d{g){w  Tch: {y%d{g({y%d{g){w\n\r",
	victim->perm_stat[STAT_AGI],
	get_curr_stat(victim,STAT_AGI),
	victim->perm_stat[STAT_MCH],
	get_curr_stat(victim,STAT_MCH),
	victim->perm_stat[STAT_TCH],
	get_curr_stat(victim,STAT_TCH) );
    send_to_char( buf, ch );

    sprintf( buf, "{wHp: {y%d{g/{y%d{w  Mana: {y%d{g/{y%d{w  Move: {y%d{g/{y%d{w  Practices: {y%d{w  Trains: {y%d{w\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
	IS_NPC(ch) ? 0 : victim->practice,
	IS_NPC(ch) ? 0 : victim->train );
    send_to_char( buf, ch );

    sprintf(buf,"{wArmor: {bpierce:{r %d  {bbash:{r %d  {bslash:{r %d  {bmagic:{r %d{w\n\r",
	    GET_AC(victim,AC_PIERCE), GET_AC(victim,AC_BASH),
	    GET_AC(victim,AC_SLASH),  GET_AC(victim,AC_EXOTIC));
    send_to_char(buf,ch);

    sprintf( buf,
	"{wHit: {g%d{w  Dam: {g%d{w  Def: {g%d{w  Saves: {g%d{w  Size: {g%s{w  Position: {g%s{w  Wimpy: {g%d{w\n\r",
	GET_HITROLL(victim), GET_DAMROLL(victim), GET_DEFROLL(victim), victim->saving_throw,
	size_table[victim->size].name, position_table[victim->position].name,
	victim->wimpy );
    send_to_char( buf, ch );

    if (IS_NPC(victim) && victim->pIndexData->new_format)
    {
	sprintf(buf, "{wDamage: {r%dd%d{w  Message:{r  %s{w\n\r",
	    victim->damage[DICE_NUMBER],victim->damage[DICE_TYPE],
	    attack_table[victim->dam_type].noun);
	send_to_char(buf,ch);
    }

    sprintf( buf, "{wFighting:{r %s\n\r",
	victim->fighting ? victim->fighting->name : "{W(none){w" );
    send_to_char( buf, ch );
    sprintf( buf, "{wWas Fighting:{r %s\n\r",
	victim->was_fighting ? victim->was_fighting->name : "{W(none){w" );
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "{wThirst: {g%d  {wHunger:{g %d  {wFull:{g %d  {wDrunk:{g %d\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_HUNGER],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK] );
	send_to_char( buf, ch );
    }

    sprintf( buf, "{wCarry number:{y %d  {wCarry weight:{y %ld\n\r",
	victim->carry_number, get_carry_weight(victim) / 10 );
    send_to_char( buf, ch );


    if (!IS_NPC(victim))
    {
    	sprintf( buf,
	    "{wAge:{y %d  {wIncarnation Played:{g %d  {wPermament Played:{g %d  {wLast Level:{y %d  {wTimer:{g %d\n\r",
	    get_age(victim),
	    (int) (victim->played + current_time - victim->logon) / 3600,
	    (int) (victim->perm_played + current_time - victim->logon) / 3600,
	    victim->pcdata->last_level,
	    victim->timer );
    	send_to_char( buf, ch );
    }

    sprintf(buf, "{wAct:{g %s\n\r",act_bit_name(victim->act));
    send_to_char(buf,ch);

    if (victim->cyber)
    {
        sprintf(buf,"{wCyber:{g %s{w\n\r",cyber_bit_name(victim->cyber));
        send_to_char(buf,ch);
    }

    if (victim->comm)
    {
    	sprintf(buf,"{wComm:{g %s{w\n\r",comm_bit_name(victim->comm));
    	send_to_char(buf,ch);
    }

    if (victim->info)
    {
	sprintf(buf, "{wInfo:{g %s{w\n\r",info_bit_name(victim->info));
	send_to_char(buf, ch);
    }

    if (!IS_NPC(victim))
    {
	if ( is_punished(victim) )
	{
		sprintf(buf, "{RPunished{w: {r%s{w\n\r", punished_bit_name(victim));
		send_to_char( buf, ch );

		if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
		{
			sprintf( buf, "   {rNoChan{w:          {c%d\n\r", victim->pcdata->timer_punished[PUNISH_NOCHAN] );
			send_to_char( buf, ch );
		}

		if ( IS_SET(victim->comm, COMM_NOEMOTE) )
		{
			sprintf( buf, "   {rNoEmote{w:         {c%d\n\r", victim->pcdata->timer_punished[PUNISH_NOEMOTE] );
			send_to_char( buf, ch );
		}

		if ( IS_SET(victim->comm, COMM_NOTELL) )
		{
			sprintf( buf, "   {rNoTell{w:          {c%d\n\r", victim->pcdata->timer_punished[PUNISH_NOTELL] );
			send_to_char( buf, ch );
		}

		if ( IS_SET(victim->comm, COMM_NOSHOUT) )
		{
			sprintf( buf, "   {rNoShout{w:         {c%d\n\r", victim->pcdata->timer_punished[PUNISH_NOSHOUT] );
			send_to_char( buf, ch );
		}

		if ( IS_SET(victim->info, INFO_BOZO) )
		{
			sprintf( buf, "   {rBozo{w:            {c%d\n\r", victim->pcdata->timer_punished[PUNISH_BOZO] );
			send_to_char( buf, ch );
		}

		if ( IS_SET(victim->act,  PLR_WANTED) )
		{
			sprintf( buf, "   {rWanted{w:          {c%d\n\r", victim->pcdata->timer_punished[PUNISH_WANTED] );
			send_to_char( buf, ch );
		}

		if ( IS_SET(victim->act,  PLR_FREEZE) )
		{
			sprintf( buf, "   {rFreeze{w:          {c%d\n\r", victim->pcdata->timer_punished[PUNISH_FREEZE] );
			send_to_char( buf, ch );
		}

		if ( is_in_prison(victim) )
		{
			sprintf( buf, "   {rPrison{w:          {c%d\n\r", victim->pcdata->timer_punished[PUNISH_PRISON] );
			send_to_char( buf, ch );
		}

		if ( (victim->in_room == get_room_index(ROOM_VNUM_IMMINT)) )
		{
			sprintf( buf, "   {rEat{w:             {c%d\n\r", victim->pcdata->timer_punished[PUNISH_EAT] );
			send_to_char( buf, ch );
		}

		if ( global_trident && (global_trident_target == victim) )
		{
			sprintf( buf, "   {rTrident Timer{w:   {c%d\n\r", global_trident_timer );
			send_to_char( buf, ch );
		}
	}
	else
		send_to_char("{wPunished: {gNo Active Punishments{w\n\r", ch);
    }

    if (IS_NPC(victim) && victim->off_flags)
    {
    	sprintf(buf, "{wOffense:{r %s{w\n\r",off_bit_name(victim->off_flags));
	send_to_char(buf,ch);
    }

    if (victim->imm_flags)
    {
	sprintf(buf, "{wImmune:{b %s\n\r",imm_bit_name(victim->imm_flags));
	send_to_char(buf,ch);
    }

    if (victim->res_flags)
    {
	sprintf(buf, "{wResist:{b %s\n\r", imm_bit_name(victim->res_flags));
	send_to_char(buf,ch);
    }

    if (victim->vuln_flags)
    {
	sprintf(buf, "{wVulnerable:{b %s\n\r", imm_bit_name(victim->vuln_flags));
	send_to_char(buf,ch);
    }

    sprintf(buf, "{wForm:{y %s\n\r{wParts:{y %s\n\r",
	form_bit_name(victim->form), part_bit_name(victim->parts));
    send_to_char(buf,ch);

    if (victim->affected_by)
    {
	sprintf(buf, "{wAffected by {g%s{w\n\r",
	    affect_bit_name(victim->affected_by));
	send_to_char(buf,ch);
    }

    if (victim->affected2_by)
    {
	sprintf(buf, "{wAlso Affected by {g%s{w\n\r",
	    affect2_bit_name(victim->affected2_by));
	send_to_char(buf,ch);
    }

    sprintf( buf, "{wMaster:{W %s  {wLeader:{W %s  {wPet:{W %s\n\r",
	victim->master      ? victim->master->name   : "{W(none){w",
	victim->leader      ? victim->leader->name   : "{W(none){w",
	victim->pet 	    ? victim->pet->name	   : "{W(none){w");
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) && victim->olcs )
    {
    	sprintf(buf,"{gOLC Privs{w:{W %s{w\n\r",olcs_bit_name(victim->olcs));
    	send_to_char(buf,ch);
    }

    if ( !IS_NPC(victim) && victim->pcdata->security > 0 )
    {
	sprintf( buf, "{gOLC Security{w:{W %d{w\n\r", victim->pcdata->security ); /* OLC */
	send_to_char( buf, ch );					   /* OLC */
    }

	/* Storm's OLC_ON Flag - Model Code */
	if ( !IS_NPC(victim) && IS_BUILDER(victim) )
	{
		sprintf( buf, "{gOLC Editor{w: %s{w\n\r",
			victim->pcdata->olc_on ? "{rON{w" : "{WOff{w" );
		send_to_char ( buf, ch );
	}

    if ( IS_SET(victim->info, INFO_GHOST ) )
    {
	sprintf( buf, "{WGHOST{w:   {WDeath Room{w: %d   {WTime{w: %d\n\r",
		victim->pcdata->died_in_room->vnum, victim->pcdata->timer_ghost );
	send_to_char( buf, ch );
    }

	/* Immortal Command Flags -- Temporarially Removed by Omne DEBUG
	if ( !IS_NPC(victim) )
	{
		sprintf(buf, "{CHero ICF{w:     {W %s{w\n\r", icf_her_bit_name(victim->icf_her));
		send_to_char(buf,ch);
		sprintf(buf, "{YImmortal ICF{w: {W %s{w\n\r", icf_ang_bit_name(victim->icf_ang));
		send_to_char(buf,ch);
		sprintf(buf, "{BStat ICF{w:     {W %s{w\n\r", icf_stat_bit_name(victim->icf_stat));
		send_to_char(buf,ch);
		sprintf(buf, "{RAdmin ICF{w:    {W %s{w\n\r", icf_sup_bit_name(victim->icf_sup));
		send_to_char(buf,ch);
	}
	*/

    sprintf( buf, "{wShort description:{w %s\n\r{wLong  description:{w %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "{W(none){w\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
    {
	sprintf(buf,"{wMobile has special procedure {g%s.{w\n\r",
		spec_name(victim->spec_fun));
	send_to_char(buf,ch);
    }

    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
    if (paf->where == TO_AFFECTS)
    {
	sprintf( buf,
	    "{wSpell:{r '%s' {wmodifies{b %s {wby{b %d {wfor{g %d {whours with bits{r %s, {wlevel{y %d{w.\n\r",
	    skill_table[(int) paf->type].name,
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration,
	    affect_bit_name( paf->bitvector ),
	    paf->level
	    );
	send_to_char( buf, ch );
    }
    if(paf->where == TO_AFFECTS2)
    {
    sprintf( buf,
	    "{wSpell:{r '%s' {wmodifies{b %s {wby{b %d {wfor{g %d {whours with bits{r %s, {wlevel{y %d{w.\n\r",
	    skill_table[(int) paf->type].name,
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration,
	    affect2_bit_name( paf->bitvector ),
	    paf->level
	    );
	send_to_char( buf, ch );
    }
    }

    return;

}



/* ofind and mfind replaced with vnum, vnum skill also added */



void do_vnum(CHAR_DATA *ch, char *argument)

{

    char arg[MAX_INPUT_LENGTH];

    char *string;



    string = one_argument(argument,arg);



    if (arg[0] == '\0')

    {

	send_to_char("Syntax:\n\r",ch);

	send_to_char("  vnum obj <name>\n\r",ch);

	send_to_char("  vnum mob <name>\n\r",ch);

	send_to_char("  vnum skill <skill or spell>\n\r",ch);

	send_to_char("  vnum room <name>\n\r",ch);		/* Added by Istennu */

	return;

    }



    if (!str_cmp(arg,"obj"))

    {

	do_ofind(ch,string);

 	return;

    }



    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))

    {

	do_mfind(ch,string);

	return;

    }



    if (!str_cmp(arg,"room"))

    {

	do_rfind(ch,string);

	return;

    }



    if (!str_cmp(arg,"skill") || !str_cmp(arg,"spell"))

    {

	do_slookup(ch,string);

	return;

    }

    /* do all */

    do_mfind(ch,argument);

    do_ofind(ch,argument);

    do_rfind(ch,argument);

}





/* Added by Istennu */

void do_rfind( CHAR_DATA *ch, char *argument )

{

  char buf[MAX_STRING_LENGTH];

  bool found = FALSE;

  int i;



  ROOM_INDEX_DATA *pRoomIndex;



  /* Ugh.  We have to brute through the entire hash/list structure */

  for(i=0; i<MAX_KEY_HASH; i++)

  {

    pRoomIndex = room_index_hash[i];

    while(pRoomIndex != NULL) {

      if(is_name(argument, pRoomIndex->name))

	{

        found = TRUE;

        sprintf(buf, "[%5d] %s\n\r",

          pRoomIndex->vnum, pRoomIndex->name);

        send_to_char( buf, ch );

      }

      pRoomIndex = pRoomIndex->next;

    }

  }

  if (!found )

    send_to_char( "No rooms by that name.\n\r", ch );



  return;

}





void do_mfind( CHAR_DATA *ch, char *argument )

{

    extern int top_mob_index;

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    MOB_INDEX_DATA *pMobIndex;

    int vnum;

    int nMatch;

    bool fAll;

    bool found;



    one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	send_to_char( "Find whom?\n\r", ch );

	return;

    }



    fAll	= FALSE; /* !str_cmp( arg, "all" ); */

    found	= FALSE;

    nMatch	= 0;



    /*

     * Yeah, so iterating over all vnum's takes 10,000 loops.

     * Get_mob_index is fast, and I don't feel like threading another link.

     * Do you?

     * -- Furey

     */

    for ( vnum = 0; nMatch < top_mob_index; vnum++ )

    {

	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )

	{

	    nMatch++;

	    if ( fAll || is_name( argument, pMobIndex->player_name ) )

	    {

		found = TRUE;

		sprintf( buf, "[%5d] %s\n\r",

		    pMobIndex->vnum, pMobIndex->short_descr );

		send_to_char( buf, ch );

	    }

	}

    }



    if ( !found )

	send_to_char( "No mobiles by that name.\n\r", ch );



    return;

}







void do_ofind( CHAR_DATA *ch, char *argument )

{

    extern int top_obj_index;

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    OBJ_INDEX_DATA *pObjIndex;

    int vnum;

    int nMatch;

    bool fAll;

    bool found;



    one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	send_to_char( "Find what?\n\r", ch );

	return;

    }



    fAll	= FALSE; /* !str_cmp( arg, "all" ); */

    found	= FALSE;

    nMatch	= 0;



    /*

     * Yeah, so iterating over all vnum's takes 10,000 loops.

     * Get_obj_index is fast, and I don't feel like threading another link.

     * Do you?

     * -- Furey

     */

    for ( vnum = 0; nMatch < top_obj_index; vnum++ )

    {

	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )

	{

	    nMatch++;

	    if ( fAll || is_name( argument, pObjIndex->name ) )

	    {

		found = TRUE;

		sprintf( buf, "[%5d] %s\n\r",

		    pObjIndex->vnum, pObjIndex->short_descr );

		send_to_char( buf, ch );

	    }

	}

    }



    if ( !found )

	send_to_char( "No objects by that name.\n\r", ch );



    return;

}





void do_owhere(CHAR_DATA *ch, char *argument )

{

    char buf[MAX_INPUT_LENGTH];

    BUFFER *buffer;

    OBJ_DATA *obj;

    OBJ_DATA *in_obj;

    bool found;

    int number = 0, max_found;



    found = FALSE;

    number = 0;

    max_found = 200;



    buffer = new_buf();



    if (argument[0] == '\0')

    {

	send_to_char("Find what?\n\r",ch);

	return;

    }



    for ( obj = object_list; obj != NULL; obj = obj->next )

    {

        if ( !can_see_obj( ch, obj ) || !is_name( argument, obj->name )

        ||   ch->level < obj->level )

            continue;



        found = TRUE;

        number++;



        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )

            ;



        if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)

	&&   in_obj->carried_by->in_room != NULL)

            sprintf( buf, "%3d) %s is carried by %s [Room %d]\n\r",

                number, obj->short_descr,PERS(in_obj->carried_by, ch),

		in_obj->carried_by->in_room->vnum );

        else if (in_obj->in_room != NULL && can_see_room(ch,in_obj->in_room))

            sprintf( buf, "%3d) %s is in %s [Room %d]\n\r",

                number, obj->short_descr,in_obj->in_room->name,

	   	in_obj->in_room->vnum);

	else

            continue;



        buf[0] = UPPER(buf[0]);

        add_buf(buffer,buf);



        if (number >= max_found)

            break;

    }



    if ( !found )

        send_to_char( "Nothing like that in heaven or earth.\n\r", ch );

    else

        page_to_char(buf_string(buffer),ch);



    free_buf(buffer);

}





void do_mwhere( CHAR_DATA *ch, char *argument )



{

    char buf[MAX_STRING_LENGTH];

    BUFFER *buffer;

    CHAR_DATA *victim;

    bool found;

    int count = 0;



    if ( argument[0] == '\0' )

    {

	DESCRIPTOR_DATA *d;



	/* show characters logged */



	buffer = new_buf();

	for (d = descriptor_list; d != NULL; d = d->next)

	{

	    if (d->character != NULL && d->connected == CON_PLAYING

	    &&  d->character->in_room != NULL && can_see(ch,d->character)

	    &&  can_see_room(ch,d->character->in_room))

	    {

		victim = d->character;

		count++;

		if (d->original != NULL)

		    sprintf(buf,"%3d) %s (in the body of %s) is in %s [%d]\n\r",

			count, d->original->name,victim->short_descr,

			victim->in_room->name,victim->in_room->vnum);

		else

		    sprintf(buf,"%3d) %s is in %s [%d]\n\r",

			count, victim->name,victim->in_room->name,

			victim->in_room->vnum);

		add_buf(buffer,buf);

	    }

	}



        page_to_char(buf_string(buffer),ch);

	free_buf(buffer);

	return;

    }





    found = FALSE;

    buffer = new_buf();

    for ( victim = char_list; victim != NULL; victim = victim->next )

    {

	if ( victim->in_room != NULL

	&&   is_name( argument, victim->name )

	&&   (get_trust(ch) >= get_trust(victim)))

	{

	    found = TRUE;

	    count++;

	    sprintf( buf, "%3d) [%5d] %-28s [%5d] %s\n\r", count,

		IS_NPC(victim) ? victim->pIndexData->vnum : 0,

		IS_NPC(victim) ? victim->short_descr : victim->name,

		victim->in_room->vnum,

		victim->in_room->name );

	    add_buf(buffer,buf);

	}

    }



    if ( !found )

	act( "You didn't find any $T.", ch, NULL, argument, TO_CHAR );

    else

    	page_to_char(buf_string(buffer),ch);



    free_buf(buffer);



    return;

}







void do_reboo( CHAR_DATA *ch, char *argument )

{

    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );

    return;

}







void do_reboot( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool merc_down;

    DESCRIPTOR_DATA *d,*d_next;

    CHAR_DATA *vch;



    tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_ALL );



    sprintf( buf, "Reboot by %s.", ch->name );

    do_echo( ch, buf );



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



    return;

}







void do_shutdow( CHAR_DATA *ch, char *argument )

{

    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );

    return;

}







void do_shutdown( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool merc_down;

    DESCRIPTOR_DATA *d,*d_next;

    CHAR_DATA *vch;



    tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_ALL );



    sprintf( buf, "Shutdown by %s.", ch->name );

    append_file( ch, SHUTDOWN_FILE, buf );

    strcat( buf, "\n\r" );

    do_echo( ch, buf );

    merc_down = TRUE;

    log_string( "GLOBAL VARIABLE : MERC_DOWN set to TRUE" );



    for ( d = descriptor_list; d != NULL; d = d_next)

    {

	d_next = d->next;

	vch = d->original ? d->original : d->character;

	if (vch != NULL)

	    save_char_obj(vch);

	close_socket(d);

    }

    return;

}



void do_protect( CHAR_DATA *ch, char *argument)

{

    CHAR_DATA *victim;



    if (argument[0] == '\0')

    {

	send_to_char("Protect whom from snooping?\n\r",ch);

	return;

    }



    if ((victim = get_char_world(ch,argument)) == NULL)

    {

	send_to_char("You can't find them.\n\r",ch);

	return;

    }



    if (IS_SET(victim->comm,COMM_SNOOP_PROOF))

    {

	act_new("$N is no longer snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);

	send_to_char("Your snoop-proofing was just removed.\n\r",victim);

	REMOVE_BIT(victim->comm,COMM_SNOOP_PROOF);

    }

    else

    {

	act_new("$N is now snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);

	send_to_char("You are now immune to snooping.\n\r",victim);

	SET_BIT(victim->comm,COMM_SNOOP_PROOF);

    }

}







void do_snoop( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];

    DESCRIPTOR_DATA *d;

    CHAR_DATA *victim;

    bool snoopon;



    snoopon = FALSE;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Snoop whom?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( victim->desc == NULL )

    {

	send_to_char( "No descriptor to snoop.\n\r", ch );

	return;

    }



    if ( victim == ch )

    {

	send_to_char( "Cancelling all snoops.\n\r", ch );

	sprintf( buf, "%s stops all snoops", ch->name );

	log_string( buf );

	sprintf( buf, "%s stops being such a snoop.", ch->name );

	wiznet( buf,NULL,NULL,WIZ_SNOOPS,0,get_trust(ch));

	for ( d = descriptor_list; d != NULL; d = d->next )

	{

		if ( d->snoop_by1 == ch->desc )

				d->snoop_by1 = NULL;

		if ( d->snoop_by2 == ch->desc )

				d->snoop_by2 = NULL;

		if ( d->snoop_by3 == ch->desc )

				d->snoop_by3 = NULL;

		if ( d->snoop_by4 == ch->desc )

				d->snoop_by4 = NULL;

		if ( d->snoop_by5 == ch->desc )

				d->snoop_by5 = NULL;

	}

	return;

    }



    if ( 		victim->desc->snoop_by1 != NULL

		&&	victim->desc->snoop_by2 != NULL

		&&	victim->desc->snoop_by3 != NULL

		&&	victim->desc->snoop_by4 != NULL

		&&	victim->desc->snoop_by5 != NULL )

    {

	send_to_char( "Busy already.\n\r", ch );

	return;

    }



    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room

    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))

    {

        send_to_char("That character is in a private room.\n\r",ch);

        return;

    }



    if ( get_trust( victim ) >= get_trust( ch )

    ||   IS_SET(victim->comm,COMM_SNOOP_PROOF))

    {

	send_to_char( "They are protected from snooping.\n\r", ch );

	return;

    }



    if ( ch->desc != NULL )

    {

	for ( d = ch->desc->snoop_by1; d != NULL; d = d->snoop_by1 )

	{

		if ( d->character == victim || d->original == victim )

		{

			send_to_char( "No snoop loops.\n\r", ch );

			return;

		}

	}

	for ( d = ch->desc->snoop_by2; d != NULL; d = d->snoop_by2 )

	{

		if ( d->character == victim || d->original == victim )

		{

			send_to_char( "No snoop loops.\n\r", ch );

			return;

		}

	}

	for ( d = ch->desc->snoop_by3; d != NULL; d = d->snoop_by3 )

	{

		if ( d->character == victim || d->original == victim )

		{

			send_to_char( "No snoop loops.\n\r", ch );

			return;

		}

	}

	for ( d = ch->desc->snoop_by4; d != NULL; d = d->snoop_by4 )

	{

		if ( d->character == victim || d->original == victim )

		{

			send_to_char( "No snoop loops.\n\r", ch );

			return;

		}

	}

	for ( d = ch->desc->snoop_by5; d != NULL; d = d->snoop_by5 )

	{

		if ( d->character == victim || d->original == victim )

		{

			send_to_char( "No snoop loops.\n\r", ch );

			return;

		}

	}

    }



    if ( victim->desc->snoop_by1 == NULL )

    {

	victim->desc->snoop_by1 = ch->desc;

	snoopon = TRUE;

    }

    else if ( victim->desc->snoop_by2 == NULL )

    {

	victim->desc->snoop_by2 = ch->desc;

	snoopon = TRUE;

    }

    else if ( victim->desc->snoop_by3 == NULL )

    {

	victim->desc->snoop_by3 = ch->desc;

	snoopon = TRUE;

    }

    else if ( victim->desc->snoop_by4 == NULL )

    {

	victim->desc->snoop_by4 = ch->desc;

	snoopon = TRUE;

    }

    else if ( victim->desc->snoop_by5 == NULL )

    {

	victim->desc->snoop_by5 = ch->desc;

	snoopon = TRUE;

    }

    else

    {

	snoopon = FALSE;

    }



    if ( snoopon )

    {

	sprintf( buf, "%s starts snooping on %s", ch->name, victim->name );

	wiznet( buf,NULL,NULL,WIZ_SNOOPS,0,get_trust(ch));

	log_string( buf );

	send_to_char( "Ok.\n\r", ch );

	return;

    }

    else

    {

	sprintf( buf, "%s unable to snoop %s - Snoop List Exceeded!", ch->name, victim->name );

	wiznet( buf,NULL,NULL,WIZ_SNOOPS,0,get_trust(ch));

	log_string( buf );

	send_to_char( "Snoop Failed - Snoop List Exceeded!\n\r", ch );

	return;

    }



    return;

}



void do_snooplist( CHAR_DATA *ch, char *argument )

{

	char buf[MAX_STRING_LENGTH];

	DESCRIPTOR_DATA *d;

	CHAR_DATA *snooper;

	CHAR_DATA *snooped;

	bool found;



	found = FALSE;

	send_to_char( "\n\rSnoop List:\n\r", ch );

	for ( d = descriptor_list; d != NULL; d = d->next)

	{

		if ( d->snoop_by1 != NULL )

		{

			snooper = d->snoop_by1->character;

			snooped = d->character;



			if ( (get_trust( ch ) >= get_trust( snooper )) && (get_trust( ch ) >= get_trust( snooped )) )

			{

				found = TRUE;

				sprintf( buf, "{c%-15s   {r<-- Snooped by --   {g%-15s{w\n\r",

					snooped->name, snooper->name );

				send_to_char( buf, ch );

			}

		}

		if ( d->snoop_by2 != NULL )

		{

			snooper = d->snoop_by2->character;

			snooped = d->character;



			if ( (get_trust( ch ) >= get_trust( snooper )) && (get_trust( ch ) >= get_trust( snooped )) )

			{

				found = TRUE;

				sprintf( buf, "{c%-15s   {r<-- Snooped by --   {g%-15s{w\n\r",

					snooped->name, snooper->name );

				send_to_char( buf, ch );

			}

		}

		if ( d->snoop_by3 != NULL )

		{

			snooper = d->snoop_by3->character;

			snooped = d->character;



			if ( (get_trust( ch ) >= get_trust( snooper )) && (get_trust( ch ) >= get_trust( snooped )) )

			{

				found = TRUE;

				sprintf( buf, "{c%-15s   {r<-- Snooped by --   {g%-15s{w\n\r",

					snooped->name, snooper->name );

				send_to_char( buf, ch );

			}

		}

		if ( d->snoop_by4 != NULL )

		{

			snooper = d->snoop_by4->character;

			snooped = d->character;



			if ( (get_trust( ch ) >= get_trust( snooper )) && (get_trust( ch ) >= get_trust( snooped )) )

			{

				found = TRUE;

				sprintf( buf, "{c%-15s   {r<-- Snooped by --   {g%-15s{w\n\r",

					snooped->name, snooper->name );

				send_to_char( buf, ch );

			}

		}

		if ( d->snoop_by5 != NULL )

		{

			snooper = d->snoop_by5->character;

			snooped = d->character;



			if ( (get_trust( ch ) >= get_trust( snooper )) && (get_trust( ch ) >= get_trust( snooped )) )

			{

				found = TRUE;

				sprintf( buf, "{c%-15s   {r<-- Snooped by --   {g%-15s{w\n\r",

					snooped->name, snooper->name );

				send_to_char( buf, ch );

			}

		}

	}



	if ( !found )

		send_to_char( "\n\r\n\rNone Found.\n\r", ch );

	return;

}





void do_switch( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];

    CHAR_DATA *victim;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Switch into whom?\n\r", ch );

	return;

    }



    if ( ch->desc == NULL )

	return;



    if ( ch->desc->original != NULL )

    {

	send_to_char( "You are already switched.\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



    if ( victim == ch )

    {

	send_to_char( "Ok.\n\r", ch );

	return;

    }



    if (!IS_NPC(victim))

    {

	send_to_char("You can only switch into mobiles.\n\r",ch);

	return;

    }



    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room

    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))

    {

	send_to_char("That character is in a private room.\n\r",ch);

	return;

    }



    if ( victim->desc != NULL )

    {

	send_to_char( "Character in use.\n\r", ch );

	return;

    }



    sprintf( buf, "%s switches into %s", ch->name, victim->short_descr );

    wiznet( buf,NULL,NULL,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));

    log_string( buf );



    ch->desc->character = victim;

    ch->desc->original  = ch;

    victim->desc        = ch->desc;

    ch->desc            = NULL;

    /* change communications to match */

    if (ch->prompt != NULL)

        victim->prompt = str_dup(ch->prompt);

    victim->comm = ch->comm;

    victim->lines = ch->lines;

    send_to_char( "Ok.\n\r", victim );

    return;

}







void do_return( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if ( ch->desc == NULL )

	return;



    if ( ch->desc->original == NULL )

    {

	send_to_char( "You are not switched.\n\r", ch );

	return;

    }



    send_to_char(

"You return to your original body. Type replay to see any missed tells.\n\r",

	ch );

    if (ch->prompt != NULL)

    {

	free_string(ch->prompt);

	ch->prompt = NULL;

    }



    sprintf(buf,"%s returns from %s", ch->desc->original->name, ch->short_descr);

    wiznet(buf,NULL,NULL,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));

    log_string( buf );

    ch->desc->character       = ch->desc->original;

    ch->desc->original        = NULL;

    ch->desc->character->desc = ch->desc;

    ch->desc                  = NULL;

    return;

}





/* trust levels for load and clone */

bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj)
{
    if (IS_TRUSTED(ch,GOD)
	|| (IS_TRUSTED(ch,IMMORTAL) && obj->level <= 20 && obj->cost <= 1000)
	|| (IS_TRUSTED(ch,DEMI)	    && obj->level <= 10 && obj->cost <= 500)
	|| (IS_TRUSTED(ch,ANGEL)    && obj->level <=  5 && obj->cost <= 250)
	|| (IS_TRUSTED(ch,AVATAR)   && obj->level ==  0 && obj->cost <= 100))
	return TRUE;
    else
	return FALSE;
}



/* for clone, to insure that cloning goes many levels deep */
void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)
{
    OBJ_DATA *c_obj, *t_obj;


    for (c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content)
    {
	if (obj_check(ch,c_obj))
	{
	    t_obj = create_object(c_obj->pIndexData,0);
	    clone_object(c_obj,t_obj);
	    obj_to_obj(t_obj,clone);
	    recursive_clone(ch,c_obj,t_obj);
	}
    }
}

/* command that is similar to load */
//Former Function For Until Flaws Filtered From Ferg Fixes and Ferg Fixes Functional
/* Former Function Saved, just in case...
void do_clone(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char varg[MAX_INPUT_LENGTH];
    CHAR_DATA *mob;
    OBJ_DATA  *obj;
    int count, cnt;
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument,arg);
    argument = one_argument(argument,varg);
    count = atoi( varg );

    if ((arg[0] == '\0') || (varg[0] == '\0'))
    {
	send_to_char("{rSyntax{w: {cClone {r<{wobject{c|{wmobile{r> <{wcount{r> <{wtarget{r>{w\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	mob = NULL;
	obj = get_obj_here(ch,argument);
	if (obj == NULL)
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
    }
    else if (!str_prefix(arg,"mobile"))
    {
	obj = NULL;
	mob = get_char_room(ch,argument);
	if (mob == NULL)
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
    }
    else
    {
	do_clone(ch, "" );
	return;
    }

    if ( (count < 1) || (count > 50) )
    {
	send_to_char( "Count must be between 1 and 50.\n\r", ch );
	return;
    }

    // clone an object
    if (obj != NULL)
    {
	OBJ_DATA *clone;

	if (!obj_check(ch,obj))
	{
	    send_to_char(
		"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	for ( cnt = 1; cnt <= count; ++cnt )
	{
		clone = create_object(obj->pIndexData,0);
		clone_object(obj,clone);
		if (obj->carried_by != NULL)
		    obj_to_char(clone,ch);
		else
		    obj_to_room(clone,ch->in_room);
	 	recursive_clone(ch,obj,clone);
	}

	if ( count > 1 )
	{
		sprintf(buf, "You have cloned {g%d{w copies of {c%s{w\n\r", count, clone->short_descr );
		send_to_char( buf, ch );
		sprintf(buf, "%s cloned %d copies of %s",
			ch->name, count, clone->short_descr );
		log_string(buf);
		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	}

	act("$n has created $p.",ch,clone,NULL,TO_ROOM);
	act("You clone $p.",ch,clone,NULL,TO_CHAR);
	wiznet("$N clones $p.",ch,clone,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
	return;
    }
    else if (mob != NULL)
    {
	CHAR_DATA *clone;
	OBJ_DATA *new_obj;
	char buf[MAX_STRING_LENGTH];

	if ( !IS_ADMIN(ch) )
	{
	    send_to_char( "Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	if (!IS_NPC(mob))
	{
	    send_to_char("You can only clone mobiles.\n\r",ch);
	    return;
	}

	for( cnt = 1; cnt <= count; ++cnt )
	{
	  clone = create_mobile(mob->pIndexData);
	  clone_mobile(mob,clone);

	  for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	  {
	    if (obj_check(ch,obj))
	    {
		new_obj = create_object(obj->pIndexData,0);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,clone);
		new_obj->wear_loc = obj->wear_loc;
	    }
	  }
	  char_to_room(clone,ch->in_room);
	}

	if ( count > 1 )
	{
		sprintf(buf, "You have cloned {g%d{w copies of {c%s{w\n\r", count, clone->short_descr );
		send_to_char( buf, ch );
		sprintf(buf, "%s cloned %d copies of %s",
			ch->name, count, clone->short_descr );
		log_string(buf);
		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	}

      act("$n has created $N.",ch,NULL,clone,TO_ROOM);
      act("You clone $N.",ch,NULL,clone,TO_CHAR);
	sprintf(buf,"$N clones %s.",clone->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
      return;
    }
}
*/

/* command that is similar to load */
//Syntax fixed by Ferg  -- Ferg, Fix your Fixes -- Grayson, Fixes Fixed -- Ferg
//New syntax: clone <object|mobile> <target> <count>
//Allowances for not using obj/mob or count, thus clone <target> would work, finally
void do_clone(CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    CHAR_DATA *mob;
    OBJ_DATA  *obj;
    int count, cnt;
    char buf[MAX_STRING_LENGTH];

	argument = one_argument(argument,arg1); //obj/mob OR target
	argument = one_argument(argument,arg2); // target OR count
	argument = one_argument(argument,arg3); //count or null
	if (is_number(arg3))
		count = atoi(arg3);
	else if (is_number(arg2))
		count = atoi(arg2);
	else
		count = 1;

    if ((arg1[0] == '\0') ||
    	((!str_prefix(arg1,"object")||!str_prefix(arg1,"mobile"))&& arg2[0] == '\0'))
    // This says if arg1 is empty OR ( (if arg1 is object or mob) AND (arg2 is empty) THEN
    {
		send_to_char("{rSyntax{w: {cclone {r<{wobject{c|{wmobile{r> <{wtarget{r> <{wcount{r>{w\n\r",ch);
		return;
    }

    if(!str_prefix(arg1,"object"))
    {
		mob = NULL;
		obj = get_obj_here(ch,arg2);
		if (obj == NULL)
		{
			send_to_char("You do not see that here.\n\r",ch);
			return;
		}
    }
    else if(!str_prefix(arg1,"mobile"))
    {
		obj = NULL;
		mob = get_char_room(ch,arg2);
		if (mob == NULL)
		{
			send_to_char("You do not see that here.\n\r",ch);
			return;
		}
    }
    else // find both
	{
		mob = get_char_room(ch,arg1);
		obj = get_obj_here(ch,arg1);
		if (mob == NULL && obj == NULL)
		{
		    send_to_char("You don't see that here.\n\r",ch);
		    return;
		}
	}

    if ( (count < 1) || (count > 50) )
    {
		send_to_char( "Count must be between 1 and 50.\n\r", ch );
		return;
    }

	// clone an object
	if (obj != NULL)
	{
		OBJ_DATA *clone;

		if (!obj_check(ch,obj))
		{
			send_to_char(
			"Your powers are not great enough for such a task.\n\r",ch);
			return;
		}

		for ( cnt = 1; cnt <= count; ++cnt )
		{
			clone = create_object(obj->pIndexData,0);
			clone_object(obj,clone);
			if (obj->carried_by != NULL)
				obj_to_char(clone,ch);
			else
				obj_to_room(clone,ch->in_room);
			recursive_clone(ch,obj,clone);
		}

		if ( count > 1 )
		{
			sprintf(buf, "You have cloned {g%d{w copies of {c%s{w\n\r", count, clone->short_descr );
			send_to_char( buf, ch );
			sprintf(buf, "%s cloned %d copies of %s",
				ch->name, count, clone->short_descr );
			log_string(buf);
			wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
		}

		act("$n has created $p.",ch,clone,NULL,TO_ROOM);
		act("You clone $p.",ch,clone,NULL,TO_CHAR);
		wiznet("$N clones $p.",ch,clone,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
		return;
	}
	else if (mob != NULL)
	{
	CHAR_DATA *clone;
	OBJ_DATA *new_obj;
	char buf[MAX_STRING_LENGTH];

	if ( !IS_ADMIN(ch) )
	{
		send_to_char( "Your powers are not great enough for such a task.\n\r",ch);
		return;
	}

	if (!IS_NPC(mob))
	{
		send_to_char("You can only clone mobiles.\n\r",ch);
		return;
	}

	for( cnt = 1; cnt <= count; ++cnt )
	{
	  clone = create_mobile(mob->pIndexData);
	  clone_mobile(mob,clone);

	  for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	  {
		if (obj_check(ch,obj))
		{
			new_obj = create_object(obj->pIndexData,0);
			clone_object(obj,new_obj);
			recursive_clone(ch,obj,new_obj);
			obj_to_char(new_obj,clone);
			new_obj->wear_loc = obj->wear_loc;
		}
	  }
	  char_to_room(clone,ch->in_room);
	}

	if ( count > 1 )
	{
		sprintf(buf, "You have cloned {g%d{w copies of {c%s{w\n\r", count, clone->short_descr );
		send_to_char( buf, ch );
		sprintf(buf, "%s cloned %d copies of %s",
			ch->name, count, clone->short_descr );
		log_string(buf);
		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	}

	  act("$n has created $N.",ch,NULL,clone,TO_ROOM);
	  act("You clone $N.",ch,NULL,clone,TO_CHAR);
	sprintf(buf,"$N clones %s.",clone->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
	  return;
	}
}


/* RT to replace the two load commands */



void do_load(CHAR_DATA *ch, char *argument )

{

   char arg[MAX_INPUT_LENGTH];



    argument = one_argument(argument,arg);



    if (arg[0] == '\0')

    {

	send_to_char("Syntax:\n\r",ch);

	send_to_char("  load mob <vnum>\n\r",ch);

	send_to_char("  load obj <vnum> <level>\n\r",ch);

	return;

    }



    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))

    {

	do_mload(ch,argument);

	return;

    }



    if (!str_cmp(arg,"obj"))

    {

	do_oload(ch,argument);

	return;

    }

    /* echo syntax */

    do_load(ch,"");

}





void do_mload( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    MOB_INDEX_DATA *pMobIndex;

    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];



    one_argument( argument, arg );



    if ( arg[0] == '\0' || !is_number(arg) )

    {

	send_to_char( "Syntax: load mob <vnum>.\n\r", ch );

	return;

    }



    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )

    {

	send_to_char( "No mob has that vnum.\n\r", ch );

	return;

    }



    victim = create_mobile( pMobIndex );

    char_to_room( victim, ch->in_room );

    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );

    sprintf(buf,"$N loads %s.",victim->short_descr);

    wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));

    send_to_char( "Ok.\n\r", ch );

    return;

}







void do_oload( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH] ,arg2[MAX_INPUT_LENGTH];

    OBJ_INDEX_DATA *pObjIndex;

    OBJ_DATA *obj;

    int level;



    argument = one_argument( argument, arg1 );

    one_argument( argument, arg2 );



    if ( arg1[0] == '\0' || !is_number(arg1))

    {

	send_to_char( "Syntax: load obj <vnum> <level>.\n\r", ch );

	return;

    }



    level = get_trust(ch); /* default */



    if ( arg2[0] != '\0')  /* load with a level */

    {

	if (!is_number(arg2))

        {

	  send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );

	  return;

	}

        level = atoi(arg2);

        if (level < 0 || level > get_trust(ch))

	{

	  send_to_char( "Level must be be between 0 and your level.\n\r",ch);

  	  return;

	}

    }



    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )

    {

	send_to_char( "No object has that vnum.\n\r", ch );

	return;

    }



    obj = create_object( pObjIndex, level );

    if ( CAN_WEAR(obj, ITEM_TAKE) )

	obj_to_char( obj, ch );

    else

	obj_to_room( obj, ch->in_room );

    act( "$n has created $p!", ch, obj, NULL, TO_ROOM );

    wiznet("$N loads $p.",ch,obj,WIZ_LOAD,WIZ_SECURE,get_trust(ch));

    send_to_char( "Ok.\n\r", ch );

    return;

}







void do_purge( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    char buf[100];

    CHAR_DATA *victim;

    OBJ_DATA *obj;

    DESCRIPTOR_DATA *d;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	/* 'purge' */

	CHAR_DATA *vnext;

	OBJ_DATA  *obj_next;



	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )

	{

	    vnext = victim->next_in_room;

	    if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE)

	    &&   victim != ch /* safety precaution */ )

		extract_char( victim, TRUE );

	}



	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )

	{

	    obj_next = obj->next_content;

	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))

	      extract_obj( obj );

	}



	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);

	send_to_char( "Ok.\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



    if ( !IS_NPC(victim) )

    {



	if (ch == victim)

	{

	  send_to_char("Ho ho ho.\n\r",ch);

	  return;

	}



	if (get_trust(ch) <= get_trust(victim) && (get_trust(ch) < CREATOR))

	{

	  send_to_char("That was not a good idea...\n\r",ch);

	  sprintf(buf,"%s tried to purge you!\n\r",ch->name);

	  send_to_char(buf,victim);

	  return;

	}



	act("$n disintegrates $N.",ch,0,victim,TO_NOTVICT);

      sprintf(buf,"You disintegrate %s!\n\r",victim->name);

	send_to_char(buf,ch);

	send_to_char("You have been disintegrated!",victim);



    	if (victim->level > 1)

	    save_char_obj( victim );

    	d = victim->desc;

    	extract_char( victim, TRUE );

    	if ( d != NULL )

          close_socket( d );



	return;

    }



    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );

    extract_char( victim, TRUE );

    return;

}







void do_advance( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    int level;

    int iLevel;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if ( arg1[0] == '\0' || arg2[0] == '\0' )

    {

	send_to_char( "Syntax: advance <char> <level>.\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    {

	send_to_char( "That player is not here.\n\r", ch);

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    }



    if (!is_number(arg2))

    {

	level = lev_num(ch,arg2);

    }

    else

    {

	level = atoi( arg2 );

    }



    if ( level < 1 || level > MAX_LEVEL )

    {

	send_to_char("Level must be 1 to 500 or an Imortal Level.\n\r", ch );

	return;

    }



    if ( level > get_trust( ch ) )

    {

	send_to_char( "Limited to your trust level.\n\r", ch );

	return;

    }



    if ( get_trust( ch ) <= get_trust( victim ) )

    {

	send_to_char( "Only on people with a lower trust than you.\n\r", ch );

	return;

    }



    /*

     * Lower level:

     *   Reset to level 1.

     *   Then raise again.

     *   -- Swiftest

     */

    if ( level <= victim->level )

    {

        int temp_prac;



	send_to_char( "Lowering a player's level!\n\r", ch );

	send_to_char( "{b**** {rOOOOHHHHHHHHHH  NNNNOOOO {b****{w\n\r", victim );

	temp_prac = victim->practice;

	victim->level    = 1;

	victim->exp      = exp_per_level(victim,victim->pcdata->points);

	victim->max_hit  = 10;

	victim->max_mana = 100;

	victim->max_move = 100;

	victim->practice = 0;

	victim->hit      = victim->max_hit;

	victim->mana     = victim->max_mana;

	victim->move     = victim->max_move;

	advance_level( victim, TRUE );

	victim->practice = temp_prac;

    }

    else

    {

	send_to_char( "Raising a player's level!\n\r", ch );

	send_to_char( "{b**** {rOOOOHHHHHHHHHH  YYYYEEEESSS {b****{w\n\r", victim );

    }



    for ( iLevel = victim->level ; iLevel < level; iLevel++ )

    {

	victim->level += 1;

	advance_level( victim,TRUE);

    }



    save_char_obj(victim);



    if (level >= LEVEL_HERO)

    {

	switch (level)

	{

		case LEVEL_HERO:

			victim->max_hit 	= 	25000;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	1000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case AVATAR:

			victim->max_hit 	= 	25000;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	2000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case ANGEL:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	3000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case DEMI:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	4000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case IMMORTAL:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	5000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case GOD:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	6000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case DEITY:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	25000;

			victim->max_move 	= 	7000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case SUPREME:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	30000;

			victim->max_move 	= 	10000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case CREATOR:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	30000;

			victim->max_move 	= 	10000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

		case MAX_LEVEL:

			victim->max_hit 	= 	1;

			victim->max_mana 	= 	30000;

			victim->max_move 	= 	10000;

			victim->hit 		= 	victim->max_hit;

			victim->mana 		= 	victim->max_mana;

			victim->move 		= 	victim->max_move;

		return;

	}

	return;

    }



    victim->pcdata->perm_hit		= victim->max_hit;

    victim->pcdata->perm_mana		= victim->max_mana;

    victim->pcdata->perm_move		= victim->max_move;



    sprintf(buf,"You are now level %d.\n\r",victim->level);

    send_to_char(buf,victim);

    victim->exp   = exp_per_level(victim,victim->pcdata->points)

		  * UMAX( 1, victim->level );

    victim->trust = 0;

    save_char_obj(victim);

    return;

}





void do_trust( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    int level;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if ( arg1[0] == '\0' || arg2[0] == '\0'  )

    {

	send_to_char( "Syntax: trust <char> <level>.\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    {

	send_to_char( "That player is not here.\n\r", ch);

	return;

    }



    if (!is_number(arg2))

    {

	level = lev_num(ch,arg2);

    }

    else

    {

	level = atoi( arg2 );

    }



    if ( level < 0 || level > MAX_LEVEL )

    {

	send_to_char("Level must be 0 (reset) or 1 to 20000, or HER-IMP.\n\r", ch );

	return;

    }



    if ( level > get_trust( ch ) )

    {

	send_to_char( "Limited to your trust.\n\r", ch );

	return;

    }



    victim->trust = level;

    return;

}







void do_restore( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];

    CHAR_DATA *victim;

    CHAR_DATA *vch;

    DESCRIPTOR_DATA *d;





    one_argument( argument, arg );

    if (arg[0] == '\0' || !str_cmp(arg,"room"))

    {

    /* cure room */



        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)

        {

            affect_strip(vch,gsn_plague);

            affect_strip(vch,gsn_poison);

            affect_strip(vch,gsn_blindness);

            affect_strip(vch,gsn_sleep);

            affect_strip(vch,gsn_curse);

		affect_strip(vch,skill_lookup("fire breath"));

		affect_strip(vch,skill_lookup("calm"));

		affect_strip(vch,skill_lookup("change sex"));

		affect_strip(vch,skill_lookup("charm person"));

		affect_strip(vch,skill_lookup("chill touch"));

		affect_strip(vch,skill_lookup("faerie fire"));

		affect_strip(vch,skill_lookup("slow"));

		affect_strip(vch,skill_lookup("weaken"));



            vch->hit 	= vch->max_hit;

            vch->mana	= vch->max_mana;

            vch->move	= vch->max_move;

            update_pos( vch);

		act("$n has restored you.",ch,NULL,vch,TO_VICT);

        }



        sprintf(buf,"$N restored room %d.",ch->in_room->vnum);

        wiznet(buf,ch,NULL,WIZ_RESTORE,0,get_trust(ch));



        send_to_char("Room restored.\n\r",ch);

        return;



    }



    if ( get_trust(ch) >=  SUPREME && !str_cmp(arg,"all"))

    {

    /* cure all */



        for (d = descriptor_list; d != NULL; d = d->next)

        {

	    victim = d->character;



	    if (victim == NULL || IS_NPC(victim))

		continue;



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

            update_pos( victim);

	    if (victim->in_room != NULL)

                act("$n has restored you.",ch,NULL,victim,TO_VICT);

        }

	send_to_char("All active players restored.\n\r",ch);

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



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



    victim->hit  = victim->max_hit;

    victim->mana = victim->max_mana;

    victim->move = victim->max_move;



    update_pos( victim );

    act( "$n has restored you.", ch, NULL, victim, TO_VICT );

    sprintf(buf,"$N restored %s",

	IS_NPC(victim) ? victim->short_descr : victim->name);

    wiznet(buf,ch,NULL,WIZ_RESTORE,0,get_trust(ch));

    send_to_char( "Ok.\n\r", ch );

    return;

}





void do_freeze( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH], varg[MAX_STRING_LENGTH];

    CHAR_DATA *victim;

    int value;



    argument = one_argument( argument, arg );

    argument = one_argument( argument, varg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Freeze whom?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    }



    if ( get_trust( victim ) >= get_trust( ch ) )

    {

	send_to_char( "You failed.\n\r", ch );

	return;

    }



    if ( varg[0] == '\0' )

	value = 0;

    else

	value = atoi( varg );



    if ( value > get_econ_value( "max_punish" ) )

    {

	sprintf( buf, "You may only NOCHANNEL someone for up to %d ticks.\n\r", get_econ_value( "max_punish" ) );

	send_to_char( buf, ch );

	return;

    }



    if ( !IS_ADMIN(ch) && (value == 0) )

    {

	send_to_char( "Only Administrators may NOCHANNEL someone indefinately.\n\r", ch );

	return;

    }



    if ( IS_SET(victim->act, PLR_FREEZE) )

    {

	REMOVE_BIT(victim->act, PLR_FREEZE);

	victim->pcdata->timer_punished[PUNISH_FREEZE] = 0;

	send_to_char( "You can play again.\n\r", victim );

	sprintf( buf, "{cFREEZE{w removed on {m%s{w.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name) );

	send_to_char( buf, ch );

	sprintf(buf,"$N thaws %s.",(IS_NPC(victim) ? victim->short_descr : victim->name));

	wiznet(buf,ch,NULL,WIZ_PENALTIES,0,0);

	sprintf( buf, "%s removes the freeze from %s", ch->name, victim->name );

	log_string( buf );

    }

    else

    {

	SET_BIT(victim->act, PLR_FREEZE);

	victim->pcdata->timer_punished[PUNISH_FREEZE] = value;

	sprintf( buf, "You can not do ANYthing for %d ticks!\n\r", value );

	send_to_char( buf, victim );

	sprintf( buf, "{cFREEZE{w set on {m%s{w for {g%d{w ticks.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name), value );

	send_to_char( buf, ch );

	sprintf(buf,"$N puts %s in the deep freeze for %d ticks",(IS_NPC(victim) ? victim->short_descr : victim->name), value);

	wiznet(buf,ch,NULL,WIZ_PENALTIES,0,0);

	sprintf( buf, "%s freezes %s for %d ticks", ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name), value );

	log_string( buf );

    }



    save_char_obj( victim );



    return;

}







void do_log( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Log whom?\n\r", ch );

	return;

    }



    if ( !str_cmp( arg, "all" ) )

    {

	if ( fLogAll )

	{

	    fLogAll = FALSE;

	    send_to_char( "Log ALL off.\n\r", ch );

	}

	else

	{

	    fLogAll = TRUE;

	    send_to_char( "Log ALL on.\n\r", ch );

	}

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    }



    /*

     * No level check, gods can log anyone.

     */

    if ( IS_SET(victim->act, PLR_LOG) )

    {

	REMOVE_BIT(victim->act, PLR_LOG);

	send_to_char( "LOG removed.\n\r", ch );

    }

    else

    {

	SET_BIT(victim->act, PLR_LOG);

	send_to_char( "LOG set.\n\r", ch );

    }



    return;

}







void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH], varg[MAX_STRING_LENGTH];
    int value;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, varg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }


    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( varg[0] == '\0' )
	value = 0;
    else
	value = atoi( varg );

    if ( value > get_econ_value( "max_punish" ) )
    {
	sprintf( buf, "You may only NOEMOTE someone for up to %d ticks.\n\r", get_econ_value( "max_punish" ) );
	send_to_char( buf, ch );
	return;
    }

    if ( !IS_ADMIN(ch) && (value == 0) )
    {
	send_to_char( "Only Administrators may NOEMOTE someone indefinately.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOEMOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NOEMOTE);
	if(!IS_NPC(victim))
		victim->pcdata->timer_punished[PUNISH_NOEMOTE] = 0; //npcs have no pcdata!
	send_to_char( "You can emote again.\n\r", victim );
	sprintf( buf, "{cNOEMOTE{w on {m%s{w has been removed.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name) );
	send_to_char( buf, ch );
	sprintf(buf,"%s restores emotes to %s.",ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name));
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	log_string( buf );
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOEMOTE);
	if(!IS_NPC(victim))
		victim->pcdata->timer_punished[PUNISH_NOEMOTE] = value; //npcs have no pcdata!
	send_to_char( "You can not emote!\n\r", victim );
	sprintf( buf, "{cNOEMOTE{w set on {m%s{w for {g%d{w ticks.\n\r",
		(IS_NPC(victim) ? victim->short_descr : victim->name),
		value );
	send_to_char( buf, ch );
	sprintf(buf,"%s revokes emotes of %s for %d ticks",
		ch->name,
		(IS_NPC(victim) ? victim->short_descr : victim->name),
		value);
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	log_string( buf );
    }

    return;
}



void do_noshout( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH], varg[MAX_STRING_LENGTH];

    CHAR_DATA *victim;

    int value;



    argument = one_argument( argument, arg );

    argument = one_argument( argument, varg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Noshout whom?\n\r",ch);

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    }



    if ( get_trust( victim ) >= get_trust( ch ) )

    {

	send_to_char( "You failed.\n\r", ch );

	return;

    }



    if ( varg[0] == '\0' )

	value = 0;

    else

	value = atoi( varg );



    if ( value > get_econ_value( "max_punish" ) )

    {

	sprintf( buf, "You may only NOSHOUT someone for up to %d ticks.\n\r", get_econ_value( "max_punish" ) );

	send_to_char( buf, ch );

	return;

    }



    if ( !IS_ADMIN(ch) && (value == 0) )

    {

	send_to_char( "Only Administrators may NOEMOTE someone indefinately.\n\r", ch );

	return;

    }



    if ( IS_SET(victim->comm, COMM_NOSHOUT) )

    {

	REMOVE_BIT(victim->comm, COMM_NOSHOUT);

	victim->pcdata->timer_punished[PUNISH_NOSHOUT] = 0;

	send_to_char( "You can shout again.\n\r", victim );

	sprintf( buf, "{cNOSHOUT{w on {m%s{w has been removed.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name) );

	send_to_char( buf, ch );

	sprintf(buf,"%s restores shouts to %s.",ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name));

	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));

	log_string( buf );

    }

    else

    {

	SET_BIT(victim->comm, COMM_NOSHOUT);

	victim->pcdata->timer_punished[PUNISH_NOSHOUT] = value;

	sprintf( buf, "You can no longer shout for %d ticks.\n\r", value );

	send_to_char( buf, victim );

	sprintf( buf, "{m%s{w has {cNOSHOUT{w set for {g%d{w ticks.\n\r",

		(IS_NPC(victim) ? victim->short_descr : victim->name), value );

	send_to_char( buf, ch );

	sprintf(buf,"%s revokes shouting for %s for %d ticks",

		ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name), value);

	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));

	log_string( buf );

    }



    return;

}







void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH], varg[MAX_STRING_LENGTH];
    int value;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, varg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( varg[0] == '\0' )
	value = 0;
    else
	value = atoi( varg );

    if ( value > get_econ_value( "max_punish" ) )
    {
	sprintf( buf, "You may only NOTELL someone for up to %d ticks.\n\r", get_econ_value( "max_punish" ) );
	send_to_char( buf, ch );
	return;
    }

    if ( !IS_ADMIN(ch) && (value == 0) )
    {
	send_to_char( "Only Administrators may NOTELL someone indefinately.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOTELL) )
    {
	REMOVE_BIT(victim->comm, COMM_NOTELL);
	if(!IS_NPC(victim))
		victim->pcdata->timer_punished[PUNISH_NOTELL] = 0; //npcs have no pcdata
	send_to_char( "You can tell again.\n\r", victim );
	sprintf( buf, "{cNOTELL{w on {m%s{w has been removed.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name) );
	send_to_char( buf, ch );
	sprintf(buf,"%s restores tells to %s.",ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name));
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	log_string( buf );
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOTELL);
	if(!IS_NPC(victim))
		victim->pcdata->timer_punished[PUNISH_NOTELL] = value; //npcs have no pcdata
	sprintf( buf, "You can not tell for %d ticks!\n\r", value );
	send_to_char( buf, victim );
	sprintf( buf, "{cNOTELL{w set on {m%s{w for {g%d{w ticks.\n\r", (IS_NPC(victim) ? victim->short_descr : victim->name), value );
	send_to_char( buf, ch );
	sprintf(buf,"%s revokes tells from %s for %d ticks",
		ch->name, (IS_NPC(victim) ? victim->short_descr : victim->name), value);
	wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));
	log_string( buf );
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{

    CHAR_DATA *rch;



    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )

    {

	if ( rch->fighting != NULL )

	    stop_fighting( rch, TRUE );

	if (IS_NPC(rch) && IS_SET(rch->act,ACT_AGGRESSIVE))

	    REMOVE_BIT(rch->act,ACT_AGGRESSIVE);

    }



    send_to_char( "Ok.\n\r", ch );

    return;

}



void do_wizlock( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool wizlock;

    wizlock = !wizlock;



    if ( wizlock )

    {

	sprintf( buf, "%s has wizlocked the game.", ch->name );

	wiznet(buf,NULL,NULL,0,0,0);

	log_string( buf );

	log_string( "GLOBAL VARIABLE : wizlock set to TRUE" );

	send_to_char( "Game wizlocked.\n\r", ch );

    }

    else

    {

	sprintf( buf, "%s has un-wizlocked the game.", ch->name );

	wiznet(buf,NULL,NULL,0,0,0);

	log_string( buf );

	log_string( "GLOBAL VARIABLE : wizlock set to FALSE" );

	send_to_char( "Game un-wizlocked.\n\r", ch );

    }



    return;

}



void do_totalwar( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool totalwar;

    totalwar = !totalwar;



    if ( totalwar )

    {

	sprintf( buf, "%s has declared total war.", ch->name );

	wiznet(buf,NULL,NULL,0,0,0);

	log_string( buf );

	log_string( "GLOBAL VARIABLE : totalwar set to TRUE" );

	send_to_char( "Total War Declared.\n\r", ch );

	do_echo(ch, "{rTOTAL WAR HAS NOW BEEN DECLARED!{w");

    }

    else

    {

	sprintf( buf, "%s has ended total war.", ch->name );

	wiznet(buf,NULL,NULL,0,0,0);

	log_string( buf );

	log_string( "GLOBAL VARIABLE : totalwar set to FALSE" );

	send_to_char( "Total War ended.\n\r", ch );

	do_echo(ch, "{cTOTAL WAR IS NOW OVER!{w");

    }



    return;

}



void do_betalock( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool betalock;

    betalock = TRUE;



    send_to_char( "The game is now in {cBETALOCK{w mode.\n\r", ch );

    send_to_char( "To release the lock, you must restart the game.\n\r", ch );

    sprintf( buf, "%s has activated Betalock.", ch->name );

    wiznet(buf,NULL,NULL,0,0,0);

    log_string( buf );

    log_string( "GLOBAL VARIABLE : betalock set to TRUE" );



    return;

}



void do_unlock_db( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool db_lock;

    extern int db_lock_count;



    db_lock 		= FALSE;

    db_lock_count 	= 0;



    send_to_char( "The database has been unlocked.\n\r", ch );

    sprintf( buf, "%s has manually unlocked the database.", ch->name );

    wiznet(buf,NULL,NULL,0,0,0);

    log_string( buf );

    log_string( "GLOBAL VARIABLE : db_lock set to FALSE" );



    return;

}



void do_unlock_spool( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool spool_lock;

    extern int spool_lock_count;



    spool_lock 		= FALSE;

    spool_lock_count	= 0;



    send_to_char( "The message spools have been unlocked.\n\r", ch );

    sprintf( buf, "%s has manually unlocked the message spools.", ch->name );

    wiznet(buf,NULL,NULL,0,0,0);

    log_string( buf );

    log_string( "GLOBAL VARIABLE : spool_lock set to FALSE" );



    return;

}



/* RT anti-newbie code */



void do_newlock( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool newlock;

    newlock = !newlock;



    if ( newlock )

    {

	sprintf( buf, "%s has locked out new characters.", ch->name );

	wiznet(buf,NULL,NULL,0,0,0);

	log_string( buf );

	log_string( "GLOBAL VARIABLE : newlock set to TRUE" );

      send_to_char( "New characters have been locked out.\n\r", ch );

    }

    else

    {

	sprintf( buf, "%s has allowed new characters back in the game.", ch->name );

	wiznet(buf,NULL,NULL,0,0,0);

	log_string( buf );

	log_string( "GLOBAL VARIABLE : newlock set to FALSE" );

      send_to_char( "Newlock removed.\n\r", ch );

    }



    return;

}



void do_lockstat( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    extern bool betalock;

    extern bool newlock;

    extern bool wizlock;

    extern bool totalwar;

    extern bool olclock;

    extern bool db_lock;

    extern bool spool_lock;

    extern int db_lock_count;

    extern int spool_lock_count;

    extern int webwho_port;

    extern bool autodown;

    extern bool autodown_reboot;

    extern int autodown_timer;



    send_to_char( "\n\rStatus of game locks:\n\rLock                  Status\n\r----------------------------\n\r", ch );



    if ( betalock )

	send_to_char( "Betalock               {cON{w  \n\r", ch );

    else

	send_to_char( "Betalock               OFF     \n\r", ch );



    if ( newlock )

	send_to_char( "Newlock                {yON{w  \n\r", ch );

    else

	send_to_char( "Newlock                OFF     \n\r", ch );



    if ( wizlock )

	send_to_char( "Wizlock                {gON{w  \n\r", ch );

    else

	send_to_char( "Wizlock                OFF     \n\r", ch );



    if ( totalwar )

	send_to_char( "Total War              {rON{w  \n\r", ch );

    else

	send_to_char( "Total War              OFF     \n\r", ch );



    if ( olclock )

	send_to_char( "OLC Lock               {bON{w  \n\r", ch );

    else

	send_to_char( "OLC Lock               OFF     \n\r", ch );



    sprintf( buf, "WebWho Port            {g%d{w\n\r", webwho_port );

    send_to_char( buf, ch );



    if ( get_econ_value( "fileload_clan" ) > 1 )

	send_to_char( "Table Load: Clan       ON      \n\r", ch );

    else

	send_to_char( "Table Load: Clan       {rOFF{w \n\r", ch );



    if ( get_econ_value( "fileload_econ" ) > 1 )

	send_to_char( "Table Load: Economy    ON      \n\r", ch );

    else

	send_to_char( "Table Load: Combat    {rOFF{w \n\r", ch );

    if ( get_econ_value( "fileload_combat" ) > 1 )
        send_to_char( "Table Load: Combat    ON      \n\r", ch );
    else
        send_to_char( "Table Load: Combat    {rOFF{w \n\r", ch );



    if ( get_econ_value( "fileload_curse" ) > 1 )

	send_to_char( "Table Load: Censored   ON      \n\r", ch );

    else

	send_to_char( "Table Load: Censored   {rOFF{w \n\r", ch );



    if ( autodown )

	send_to_char( "Auto Shutdown          {rON{w  \n\r", ch );

    else

	send_to_char( "Auto Shutdown          OFF     \n\r", ch );



    if ( autodown_reboot )

	send_to_char( "Shutdown and Reboot    {gON{w  \n\r", ch );

    else

	send_to_char( "Shutdown and Reboot    OFF     \n\r", ch );



    if ( autodown && (autodown_timer > 0) )

    {

	sprintf( buf, "Autorestart Timer:     {c%d{w\n\r", autodown_timer );

	send_to_char( buf, ch );

    }



    if ( db_lock )

	send_to_char( "Database Lock          {rON{w      Lock Count: ", ch );

    else

	send_to_char( "Database Lock          OFF     Lock Count: ", ch );



    sprintf( buf, "%d \n\r", db_lock_count );

    send_to_char( buf, ch );



    if ( spool_lock )

	send_to_char( "Message Spool Lock     {rON{w      Lock Count: ", ch );

    else

	send_to_char( "Message Spool Lock     OFF     Lock Count: ", ch );



    sprintf( buf, "%d \n\r", spool_lock_count );

    send_to_char( buf, ch );



    return;

}









void do_slookup( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    int sn;

    int line_count;

    int start_sn;



    one_argument( argument, arg );

    if ( arg[0] == '\0' )

    {

	send_to_char( "Lookup which skill or spell?\n\r", ch );

	return;

    }



    line_count = 0;

    start_sn = 0;



    if ( !str_cmp( arg, "all" ) )

    {

	if ( ch->desc->iswait )

	    start_sn = ch->desc->swcount;



	for ( sn = start_sn; sn < MAX_SKILL; sn++ )

	{

	    if ( skill_table[sn].name == NULL )

		break;

	    sprintf( buf, "Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",

		sn, skill_table[sn].slot, skill_table[sn].name );

	    send_to_char( buf, ch );



	    if ( line_count++ >= (ch->lines - 2) )

	    {

		scroll_wait( ch, "vnum", "skill all", sn++, TRUE );

		return;

	    }

	}



	scroll_wait( ch, "", "", 0, FALSE );

    }

    else

    {

	if ( ( sn = skill_lookup( arg ) ) < 0 )

	{

	    send_to_char( "No such skill or spell.\n\r", ch );

	    return;

	}



	sprintf( buf, "Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",

	    sn, skill_table[sn].slot, skill_table[sn].name );

	send_to_char( buf, ch );

    }



    return;

}



/* RT set replaces sset, mset, oset, and rset */



void do_set( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set mob    <name> <field> <value>          \n\r", ch);
	send_to_char("  set obj    <name> <field> <value>          \n\r", ch);
	send_to_char("  set room   <room> <field> <value>          \n\r", ch);
    send_to_char("  set skill  <name> <spell or skill> <value> \n\r", ch);
    send_to_char("  set combat <name> <value>                  \n\r", ch);
	send_to_char("  set global <name> <value>                  \n\r", ch);
	return;
    }

    if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	do_mset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"skill") || !str_prefix(arg,"spell"))
    {
	do_sset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	do_oset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"room"))
    {
	do_rset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"global"))
    {
	char global[MAX_INPUT_LENGTH];
	char varg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	long value;
	int  count;
	bool found;

	if( ch->level < CREATOR || get_trust(ch) < CREATOR )
	{
		send_to_char("You must be kidding, right?\n\r", ch );
		return;
	}

	argument = one_argument(argument,global);
	argument = one_argument(argument,varg);
	value = atoi( varg );

	if ( global[0] == '\0' || varg[0] == '\0' )
	{
		send_to_char( "{rSyntax:{w\n\r", ch );
		send_to_char( "   set global {g<{Wvariable{g> {g<{WNew Value{g>{w\n\r", ch );
		return;
	}

	if ( value < 0 )
	{
		send_to_char( "The new value must be 0 or greater.\n\r", ch );
		return;
	}

	found = FALSE;

	for ( count = 0; economy_table[count].name != NULL; ++count)
	{
		if ( !str_prefix( economy_table[count].name , global) )
		{
			found = TRUE;
			break;
		}
	}

	if ( !found )
	{
		send_to_char("Global Variable not found.\n\r", ch);
		return;
	}


	sprintf( buf, "%s changed global variable %s from %ld to %ld",
		ch->name,
		economy_table[count].name,
		economy_table[count].value,
		value );
	wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );
	sprintf( buf, "GLOBAL VARIABLE : %s changed global variable %s [ %s ] from %ld to %ld",
		ch->name,
		economy_table[count].name,
		economy_table[count].description,
		economy_table[count].value,
		value );
	log_string( buf );
	sprintf( buf, "{wGlobal Variable {g%s{w {C[ {c%s {C]{w changed from {y%ld{w to {y%ld{w\n\r",
		economy_table[count].name,
		economy_table[count].description,
		economy_table[count].value,
		value );
	send_to_char( buf, ch );

	economy_table[count].value = value;

	return;
    }
    if (!str_prefix(arg,"combat"))
    {
	char combat[MAX_INPUT_LENGTH];
	char varg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	long value;
	int  count;
	bool found;

	if( ch->level < CREATOR || get_trust(ch) < CREATOR )
	{
		send_to_char("You must be kidding, right?\n\r", ch );
		return;
	}

	argument = one_argument(argument,combat);
	argument = one_argument(argument,varg);
	value = atoi( varg );

	if ( combat[0] == '\0' || varg[0] == '\0' )
	{
		send_to_char( "{rSyntax:{w\n\r", ch );
		send_to_char( "   set combat {g<{Wvariable{g> {g<{WNew Value{g>{w\n\r", ch );
		return;
	}

	if ( value < 0 )
	{
		send_to_char( "The new value must be 0 or greater.\n\r", ch );
		return;
	}

	found = FALSE;

	for ( count = 0; combatmod_table[count].name != NULL; ++count)
	{
		if ( !str_prefix( combatmod_table[count].name , combat) )
		{
			found = TRUE;
			break;
		}
	}

	if ( !found )
	{
		send_to_char("Combat Variable not found.\n\r", ch);
		return;
	}

	sprintf( buf, "%s changed combat variable %s from %ld to %ld",
		ch->name,
		combatmod_table[count].name,
		combatmod_table[count].value,
		value );

	wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

	sprintf( buf, "COMBAT VARIABLE : %s changed combat variable %s [ %s ] from %ld to %ld",
		ch->name,
		combatmod_table[count].name,
		combatmod_table[count].description,
		combatmod_table[count].value,
		value );

	log_string( buf );

	sprintf( buf, "{wCombat Variable {g%s{w {C[ {c%s {C]{w changed from {y%ld{w to {y%ld{w\n\r",
		combatmod_table[count].name,
		combatmod_table[count].description,
		combatmod_table[count].value,
		value );

	send_to_char( buf, ch );

	combatmod_table[count].value = value;

	return;
    }


    /* echo syntax */

    do_set(ch,"");

}





void do_sset( CHAR_DATA *ch, char *argument )

{

    char arg1 [MAX_INPUT_LENGTH];

    char arg2 [MAX_INPUT_LENGTH];

    char arg3 [MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    int value;

    int sn;

    bool fAll;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    argument = one_argument( argument, arg3 );



    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )

    {

	send_to_char( "Syntax:\n\r",ch);

	send_to_char( "  set skill <name> <spell or skill> <value>\n\r", ch);

	send_to_char( "  set skill <name> all <value>\n\r",ch);

	send_to_char("   (use the name of the skill, not the number)\n\r",ch);

	return;

    }



    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    {

	send_to_char( "They are not here.\n\r", ch );

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

    }



    fAll = !str_cmp( arg2, "all" );

    sn   = 0;

    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )

    {

	send_to_char( "No such skill or spell.\n\r", ch );

	return;

    }



    /*

     * Snarf the value.

     */

    if ( !is_number( arg3 ) )

    {

	send_to_char( "Value must be numeric.\n\r", ch );

	return;

    }



    value = atoi( arg3 );

    if ( value < 0 || value > 100 )

    {

	send_to_char( "Value range is 0 to 100.\n\r", ch );

	return;

    }



    if ( fAll )

    {

	for ( sn = 0; sn < MAX_SKILL; sn++ )

	{

	    if ( skill_table[sn].name != NULL )

		victim->pcdata->learned[sn]	= value;

	}

    }

    else

    {

	victim->pcdata->learned[sn] = value;

    }



    return;

}







void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set char <name> <field> <value>                              \n\r", ch );
	send_to_char( "  Field being one of:                                         \n\r", ch );
	send_to_char( "    str int wis dex con sex class level hours timer           \n\r", ch );
	send_to_char( "    race group gold silver bounty bank hp mana move prac      \n\r", ch );
	send_to_char( "    align train thirst hunger drunk full incarnations         \n\r", ch );
	send_to_char( "    death_room timer_ghost security nextquest                 \n\r", ch );
	send_to_char( "    p_notell p_noshout p_bozo p_wanted p_freeze p_eat         \n\r", ch );
	send_to_char( "    p_prison p_nochan p_noemote jedi agi tch mch              \n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }

    /* clear zones for mobs */
    victim->zone = NULL;

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */

    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_STR) )
	{
	    sprintf(buf,
		"Strength range is 3 to %d\n\r.",
		get_max_train(victim,STAT_STR));
	    send_to_char(buf,ch);
	    return;
	}

	victim->perm_stat[STAT_STR] = value;
	return;
    }

    if ( !str_cmp( arg2, "p_nochan" ) )	/* PUNISHMENT */
    {
      if ( IS_NPC( victim ) )
      {
      	send_to_char( "Not on NPC's.\n\r", ch );
      	return;
      }

	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )
	{
		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );
		send_to_char( buf, ch );
		return;
	}

	if ( (value == 0) && !IS_ADMIN(ch) )
	{
		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->comm, COMM_NOCHANNELS) )
	{
		send_to_char( "They do not have NOCHANNELS set!\n\r", ch );
		return;
	}

	if ( !is_punished(victim) )
	{
		send_to_char( "They have not been punished!\n\r", ch );
		return;
	}

	if ( get_trust(ch) <= get_trust(victim) )
	{
		send_to_char( "That would be a bad idea.\n\r", ch );
		return;
	}

	victim->pcdata->timer_punished[PUNISH_NOCHAN] = value;
	return;
    }

    if ( !str_cmp( arg2, "p_noemote" ) )	/* PUNISHMENT */
    {
      if ( IS_NPC( victim ) )
      {
      	send_to_char( "Not on NPC's.\n\r", ch );
      	return;
      }

	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )
	{
		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );
		send_to_char( buf, ch );
		return;
	}

	if ( (value == 0) && !IS_ADMIN(ch) )
	{
		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->comm, COMM_NOEMOTE) )
	{
		send_to_char( "They do not have NOEMOTE set!\n\r", ch );
		return;
	}

	if ( !is_punished(victim) )
	{
		send_to_char( "They have not been punished!\n\r", ch );
		return;
	}

	if ( get_trust(ch) <= get_trust(victim) )
	{
		send_to_char( "That would be a bad idea.\n\r", ch );
		return;
	}

	victim->pcdata->timer_punished[PUNISH_NOEMOTE] = value;
	return;
    }

    if ( !str_cmp( arg2, "p_notell" ) )	/* PUNISHMENT */
    {
      if ( IS_NPC( victim ) )
      {
      	send_to_char( "Not on NPC's.\n\r", ch );
      	return;
      }

	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )
	{
		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );
		send_to_char( buf, ch );
		return;
	}

	if ( (value == 0) && !IS_ADMIN(ch) )
	{
		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->comm, COMM_NOTELL) )
	{
		send_to_char( "They do not have NOTELL set!\n\r", ch );
		return;
	}

	if ( !is_punished(victim) )
	{
		send_to_char( "They have not been punished!\n\r", ch );
		return;
	}

	if ( get_trust(ch) <= get_trust(victim) )
	{
		send_to_char( "That would be a bad idea.\n\r", ch );
		return;
	}

	victim->pcdata->timer_punished[PUNISH_NOTELL] = value;
	return;
    }

    if ( !str_cmp( arg2, "p_noshout" ) )	/* PUNISHMENT */
    {
      if ( IS_NPC( victim ) )
      {
      	send_to_char( "Not on NPC's.\n\r", ch );
      	return;
      }

	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )
	{
		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );
		send_to_char( buf, ch );
		return;
	}

	if ( (value == 0) && !IS_ADMIN(ch) )
	{
		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->comm, COMM_NOSHOUT) )
	{
		send_to_char( "They do not have NOSHOUT set!\n\r", ch );
		return;
	}

	if ( !is_punished(victim) )
	{
		send_to_char( "They have not been punished!\n\r", ch );
		return;
	}

	if ( get_trust(ch) <= get_trust(victim) )
	{
		send_to_char( "That would be a bad idea.\n\r", ch );
		return;
	}

	victim->pcdata->timer_punished[PUNISH_NOSHOUT] = value;
	return;
    }

    if ( !str_cmp( arg2, "p_bozo" ) )	/* PUNISHMENT */
    {
      if ( IS_NPC( victim ) )
      {
      	send_to_char( "Not on NPC's.\n\r", ch );
      	return;
      }

	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )
	{
		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );
		send_to_char( buf, ch );
		return;
	}

	if ( (value == 0) && !IS_ADMIN(ch) )
	{
		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->info, INFO_BOZO) )
	{
		send_to_char( "They do not have BOZO set!\n\r", ch );
		return;
	}

	if ( !is_punished(victim) )
	{
		send_to_char( "They have not been punished!\n\r", ch );
		return;
	}

	if ( get_trust(ch) <= get_trust(victim) )
	{
		send_to_char( "That would be a bad idea.\n\r", ch );
		return;
	}

	victim->pcdata->timer_punished[PUNISH_BOZO] = value;
	return;
    }

    if ( !str_cmp( arg2, "p_wanted" ) )	/* PUNISHMENT */
    {
      if ( IS_NPC( victim ) )
      {
      	send_to_char( "Not on NPC's.\n\r", ch );
      	return;
      }

	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )
	{
		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );
		send_to_char( buf, ch );
		return;
	}

	if ( (value == 0) && !IS_ADMIN(ch) )
	{
		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->act, PLR_WANTED) )
	{
		send_to_char( "They do not have WANTED set!\n\r", ch );
		return;
	}

	if ( !is_punished(victim) )
	{
		send_to_char( "They have not been punished!\n\r", ch );
		return;
	}

	if ( get_trust(ch) <= get_trust(victim) )
	{
		send_to_char( "That would be a bad idea.\n\r", ch );
		return;
	}

	victim->pcdata->timer_punished[PUNISH_WANTED] = value;

	return;

    }



    if ( !str_cmp( arg2, "p_freeze" ) )	/* PUNISHMENT */

    {

      if ( IS_NPC( victim ) )

      {

      	send_to_char( "Not on NPC's.\n\r", ch );

      	return;

      }



	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )

	{

		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );

		send_to_char( buf, ch );

		return;

	}



	if ( (value == 0) && !IS_ADMIN(ch) )

	{

		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );

		return;

	}



	if ( !IS_SET(victim->act, PLR_FREEZE) )

	{

		send_to_char( "They do not have FREEZE set!\n\r", ch );

		return;

	}



	if ( !is_punished(victim) )

	{

		send_to_char( "They have not been punished!\n\r", ch );

		return;

	}



	if ( get_trust(ch) <= get_trust(victim) )

	{

		send_to_char( "That would be a bad idea.\n\r", ch );

		return;

	}



	victim->pcdata->timer_punished[PUNISH_FREEZE] = value;

	return;

    }



    if ( !str_cmp( arg2, "p_prison" ) )	/* PUNISHMENT */

    {

      if ( IS_NPC( victim ) )

      {

      	send_to_char( "Not on NPC's.\n\r", ch );

      	return;

      }



	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )

	{

		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );

		send_to_char( buf, ch );

		return;

	}



	if ( (value == 0) && !IS_ADMIN(ch) )

	{

		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );

		return;

	}



	if (	!is_in_prison(victim) )

	{

		send_to_char( "They are not in a prison cell.\n\r", ch );

		return;

	}



	if ( !is_punished(victim) )

	{

		send_to_char( "They have not been punished!\n\r", ch );

		return;

	}



	if ( get_trust(ch) <= get_trust(victim) )

	{

		send_to_char( "That would be a bad idea.\n\r", ch );

		return;

	}



	if ( IS_IMMORTAL(victim) )

	{

		send_to_char( "You can not imprison Immortals!\n\r", ch );

		victim->pcdata->timer_punished[PUNISH_PRISON] = 0;

		return;

	}



	victim->pcdata->timer_punished[PUNISH_PRISON] = value;

	return;

    }



    if ( !str_cmp( arg2, "p_eat" ) )	/* PUNISHMENT */

    {

      if ( IS_NPC( victim ) )

      {

      	send_to_char( "Not on NPC's.\n\r", ch );

      	return;

      }



	if ( (value > get_econ_value( "max_punish" )) || (value < 0) )

	{

		sprintf( buf, "Valid punishments are 0 to %d.\n\r", get_econ_value( "max_punish" ) );

		send_to_char( buf, ch );

		return;

	}



	if ( (value == 0) && !IS_ADMIN(ch) )

	{

		send_to_char( "Only Administrators may set an indefinite punishment.\n\r", ch );

		return;

	}



	if (	!(victim->in_room == get_room_index(ROOM_VNUM_IMMINT)) )

	{

		send_to_char( "They are not in the Immortal Stomach.\n\r", ch );

		return;

	}



	if ( !is_punished(victim) )

	{

		send_to_char( "They have not been punished!\n\r", ch );

		return;

	}



	if ( get_trust(ch) <= get_trust(victim) )

	{

		send_to_char( "That would be a bad idea.\n\r", ch );

		return;

	}



	if ( IS_IMMORTAL(victim) )

	{

		send_to_char( "You can not imprison Immortals!\n\r", ch );

		victim->pcdata->timer_punished[PUNISH_EAT] = 0;

		return;

	}



	victim->pcdata->timer_punished[PUNISH_EAT] = value;

	return;

    }



    if ( !str_cmp( arg2, "security" ) )	/* OLC */

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	if ( value > ch->pcdata->security || value < 0 )

	{

	    if ( ch->pcdata->security != 0 )

	    {

		sprintf( buf, "Valid security is 0-%d.\n\r",

		    ch->pcdata->security );

		send_to_char( buf, ch );

	    }

	    else

	    {

		send_to_char( "Valid security is 0 only.\n\r", ch );

	    }

	    return;

	}

	victim->pcdata->security = value;

	return;

    }



    if ( !str_cmp( arg2, "death_room" ) )

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	victim->pcdata->died_in_room = get_room_index( value );

	return;

    }



    if ( !str_cmp( arg2, "timer" ) )

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	if ( value < 0 )

	{

		send_to_char( "Must be a positive number.\n\r", ch );

		return;

	}



	victim->timer = value;

	return;

    }



    if ( !str_cmp( arg2, "timer_ghost" ) )

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	if ( value < 0 )

	{

		send_to_char( "Must be a positive number.\n\r", ch );

		return;

	}



	victim->pcdata->timer_ghost = value;

	return;

    }



    if ( !str_cmp( arg2, "incarnations" ) )

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	if ( value < 0 )

	{

		send_to_char( "Must be a positive number.\n\r", ch );

		return;

	}



	victim->pcdata->incarnations = value;

	return;

    }



    if ( !str_cmp( arg2, "bounty" ) )

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	if ( value < 0 )

	{

		send_to_char( "Must be a positive number.\n\r", ch );

	    return;

	}

	victim->pcdata->bounty = value;

	return;

    }



    if ( !str_cmp( arg2, "hours" ) )

    {

        if ( IS_NPC( victim ) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



	if ( value < 0 )

	{

		send_to_char( "Must be a positive number.\n\r", ch );

	    return;

	}

	victim->played = value * 3600;

	victim->perm_played = value * 3600;

	return;

    }



    if ( !str_cmp( arg2, "bank" ) )

    {

        if ( IS_NPC( victim ) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }



	if ( value < 0 )

	{

		send_to_char( "Must be a positive number.\n\r", ch );

	    return;

	}

	victim->bank = value;

	return;

    }





    if ( !str_cmp( arg2, "int" ) )

    {

        if ( value < 3 || value > get_max_train(victim,STAT_INT) )

        {

            sprintf(buf,

		"Intelligence range is 3 to %d.\n\r",

		get_max_train(victim,STAT_INT));

            send_to_char(buf,ch);

            return;

        }



        victim->perm_stat[STAT_INT] = value;

        return;

    }



    if ( !str_cmp( arg2, "wis" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_WIS) )
	{
	    sprintf(buf,
		"Wisdom range is 3 to %d.\n\r",get_max_train(victim,STAT_WIS));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_WIS] = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_DEX) )
	{
	    sprintf(buf,
		"Dexterity ranges is 3 to %d.\n\r",
		get_max_train(victim,STAT_DEX));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_DEX] = value;
	return;
    }

    if ( !str_cmp( arg2, "con" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_CON) )
	{
	    sprintf(buf,
		"Constitution range is 3 to %d.\n\r",
		get_max_train(victim,STAT_CON));
	    send_to_char( buf, ch );
	    return;
	}
	victim->perm_stat[STAT_CON] = value;
	return;
    }

    if ( !str_cmp( arg2, "agi" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_AGI) )
	{
	    sprintf(buf,
		"Agility range is 3 to %d.\n\r",
		get_max_train(victim,STAT_AGI));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_AGI] = value;
	return;
    }
    
    if ( !str_cmp( arg2, "mch" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_MCH) )
	{
	    sprintf(buf,
		"Mechanical Aptitude range is 3 to %d.\n\r",
		get_max_train(victim,STAT_MCH));
	    send_to_char( buf, ch );
	    return;
	}
	victim->perm_stat[STAT_MCH] = value;
	return;
    }
	
    if ( !str_cmp( arg2, "tch" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_TCH) )
	{
	    sprintf(buf,
		"Technical Aptitude range is 3 to %d.\n\r",
		get_max_train(victim,STAT_TCH));
	    send_to_char( buf, ch );
	    return;
	}
	victim->perm_stat[STAT_TCH] = value;
	return;
    }

    if ( !str_prefix( arg2, "sex" ) )
   {
	if ( value < 0 || value > 2 )
	{
		    send_to_char( "Sex range is 0 to 2.\n\r", ch );
		    return;
		}
		victim->sex = value;
	
		if (!IS_NPC(victim))
	
		    victim->pcdata->true_sex = value;
		return;
    }

    if ( !str_prefix( arg2, "jedi" ) )
    {
		if ( value < 0 || value > 1 )
		{
		    send_to_char( "Jedi range is 0 or 1.\n\r", ch );
		    send_to_char("0 = not Jedi, 1 = Jedi", ch);
		    return;
		}
		//victim->pcdata->jedi = value;
	
			
		if (!IS_NPC(victim))
			    victim->pcdata->jedi = value;
		return;
		
    }


    if ( !str_prefix( arg2, "class" ) )

    {

	int class;



	if (IS_NPC(victim))

	{

	    send_to_char("Mobiles have no class.\n\r",ch);

	    return;

	}



	class = class_lookup(arg3);

	if ( class == -1 )

	{

	    char buf[MAX_STRING_LENGTH];



        	strcpy( buf, "Possible classes are: " );

        	for ( class = 0; class_table[class].name != NULL; class++ )

        	{

			strcat( buf, class_table[class].name );

			strcat( buf, " " );

        	}

            strcat( buf, ".\n\r" );



	    send_to_char(buf,ch);

	    return;

	}



	victim->class = class;

	return;

    }



    if ( !str_prefix( arg2, "level" ) )

    {

	if ( !IS_NPC(victim) )

	{

	    send_to_char( "Not on PC's.\n\r", ch );

	    return;

	}



	if ( value < 0 || value > MAX_LEVEL )

	{

	    send_to_char("Level must be in the range of 0 to 20000.\n\r", ch );

	    return;

	}

	victim->level = value;

	return;

    }



    if ( !str_prefix( arg2, "gold" ) )

    {

	victim->gold = value;

	return;

    }



    if ( !str_prefix(arg2, "silver" ) )

    {

	victim->silver = value;

	return;

    }



    if ( !str_prefix(arg2, "bank" ) )

    {

	victim->bank = value;

	return;

    }



    if ( !str_prefix( arg2, "hp" ) )

    {

	if ( value < -10 || value > 30000 )

	{

	    send_to_char( "Hp range is -10 to 30,000 hit points.\n\r", ch );

	    return;

	}

	victim->max_hit = value;

        if (!IS_NPC(victim))

            victim->pcdata->perm_hit = value;

	return;

    }



    if ( !str_prefix( arg2, "mana" ) )

    {

	if ( value < 0 || value > 30000 )

	{

	    send_to_char( "Mana range is 0 to 30,000 mana points.\n\r", ch );

	    return;

	}

	victim->max_mana = value;

        if (!IS_NPC(victim))

            victim->pcdata->perm_mana = value;

	return;

    }



    if ( !str_prefix( arg2, "move" ) )

    {

	if ( value < 0 || value > 30000 )

	{

	    send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch );

	    return;

	}

	victim->max_move = value;

        if (!IS_NPC(victim))

            victim->pcdata->perm_move = value;

	return;

    }



    if ( !str_prefix( arg2, "practice" ) )

    {

	if ( value < 0 || value > 250 )

	{

	    send_to_char( "Practice range is 0 to 250 sessions.\n\r", ch );

	    return;

	}

	victim->practice = value;

	return;

    }



    if ( !str_prefix( arg2, "train" ))

    {

	if (value < 0 || value > 50 )

	{

	    send_to_char("Training session range is 0 to 50 sessions.\n\r",ch);

	    return;

	}

	victim->train = value;

	return;

    }



    if ( !str_prefix( arg2, "align" ) )

    {

	if ( value < -1000 || value > 1000 )

	{

	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );

	    return;

	}

	victim->alignment = value;

	return;

    }



    if ( !str_prefix( arg2, "thirst" ) )

    {

	if ( IS_NPC(victim) )

	{

	    send_to_char( "Not on NPC's.\n\r", ch );

	    return;

	}



	if ( value < -1 || value > 100 )

	{

	    send_to_char( "Thirst range is -1 to 100.\n\r", ch );

	    return;

	}



	victim->pcdata->condition[COND_THIRST] = value;

	return;

    }



    if ( !str_prefix( arg2, "drunk" ) )

    {

	if ( IS_NPC(victim) )

	{

	    send_to_char( "Not on NPC's.\n\r", ch );

	    return;

	}



	if ( value < -1 || value > 100 )

	{

	    send_to_char( "Drunk range is -1 to 100.\n\r", ch );

	    return;

	}



	victim->pcdata->condition[COND_DRUNK] = value;

	return;

    }



    if ( !str_prefix( arg2, "full" ) )

    {

	if ( IS_NPC(victim) )

	{

	    send_to_char( "Not on NPC's.\n\r", ch );

	    return;

	}



	if ( value < -1 || value > 100 )

	{

	    send_to_char( "Full range is -1 to 100.\n\r", ch );

	    return;

	}



	victim->pcdata->condition[COND_FULL] = value;

	return;

    }



    if ( !str_prefix( arg2, "hunger" ) )

    {

        if ( IS_NPC(victim) )

        {

            send_to_char( "Not on NPC's.\n\r", ch );

            return;

        }



        if ( value < -1 || value > 100 )

        {

            send_to_char( "Full range is -1 to 100.\n\r", ch );

            return;

        }



        victim->pcdata->condition[COND_HUNGER] = value;

        return;

    }

    if ( !str_prefix( arg2, "nextquest" ) )

    {

	if ( value < 0 || value > 100 )

	{

	    send_to_char( "Mana range is 0 to 100 minutes.\n\r", ch );

	    return;

	}

	victim->pcdata->nextquest = value;

	return;

    }

    if (!str_prefix( arg2, "race" ) )

    {

	int race;



	race = race_lookup(arg3);



	if ( race == 0)

	{

		char buf[MAX_STRING_LENGTH];



		send_to_char("That is not a valid race.\n\r",ch);



        	strcpy( buf, "Possible races are: " );

        	for ( race = 0; race_table[race].name != NULL; race++ )

        	{

			strcat( buf, race_table[race].name );

			strcat( buf, " " );

        	}

            strcat( buf, ".\n\r" );



		send_to_char(buf,ch);

		return;

	}



	if (!IS_NPC(victim) && !race_table[race].pc_race)

	{

		char buf[MAX_STRING_LENGTH];



		send_to_char("That is not a valid player race.\n\r",ch);



        	strcpy( buf, "Possible races are: " );

        	for ( race = 0; race_table[race].name != NULL; race++ )

        	{

			if ( race_table[race].pc_race )

			{

				strcat( buf, race_table[race].name );

				strcat( buf, " " );

			}

        	}

            strcat( buf, ".\n\r" );



		send_to_char(buf,ch);

		return;

	}



	victim->race = race;

	return;

    }



    if (!str_prefix(arg2,"group"))

    {

	if (!IS_NPC(victim))

	{

	    send_to_char("Only on NPCs.\n\r",ch);

	    return;

	}

	victim->group = value;

	return;

    }





    /*

     * Generate usage message.

     */

    do_mset( ch, "" );

    return;

}



void do_string( CHAR_DATA *ch, char *argument )

{

    char type [MAX_INPUT_LENGTH];

    char arg1 [MAX_INPUT_LENGTH];

    char arg2 [MAX_INPUT_LENGTH];

    char arg3 [MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    OBJ_DATA *obj;



    smash_tilde( argument );

    argument = one_argument( argument, type );

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    strcpy( arg3, argument );



    if ( type[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )

    {

	send_to_char("Syntax:\n\r",ch);

	send_to_char("  string char <name> <field> <string>\n\r",ch);

	send_to_char("    fields: name short long desc title roomin roomout spec\n\r",ch);

	send_to_char("  string obj  <name> <field> <string>\n\r",ch);

	send_to_char("    fields: name short long extended\n\r",ch);

	return;

    }



    if (!str_prefix(type,"character") || !str_prefix(type,"mobile"))

    {

    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    	{

	    send_to_char( "They are not here.\n\r", ch );

	    return;

    	}



	/* clear zone for mobs */

	victim->zone = NULL;



	/* string something */



     	if ( !str_prefix( arg2, "name" ) )

    	{

	    if ( !IS_NPC(victim) )

	    {

	    	send_to_char( "Not on PC's.\n\r", ch );

	    	return;

	    }

	    free_string( victim->name );

	    victim->name = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "description" ) )

    	{

    	    free_string(victim->description);

    	    victim->description = str_dup(arg3);

    	    return;

    	}



    	if ( !str_prefix( arg2, "short" ) )

    	{

	    free_string( victim->short_descr );

	    victim->short_descr = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "long" ) )

    	{

	    free_string( victim->long_descr );

	    strcat(arg3,"\n\r");

	    victim->long_descr = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "title" ) )

    	{

	    if ( IS_NPC(victim) )

	    {

	    	send_to_char( "Not on NPC's.\n\r", ch );

	    	return;

	    }



	    set_title( victim, arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "roomin" ) )

    	{

	    if ( IS_NPC(victim) )

	    {

	    	send_to_char( "Not on NPC's.\n\r", ch );

	    	return;

	    }



	    free_string( victim->pcdata->roomin );

	    victim->pcdata->roomin = str_dup( arg3 );

	    return;

    	}

    	if ( !str_prefix( arg2, "roomout" ) )

    	{

	    if ( IS_NPC(victim) )

	    {

	    	send_to_char( "Not on NPC's.\n\r", ch );

	    	return;

	    }



	    free_string( victim->pcdata->roomout );

	    victim->pcdata->roomout = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "spec" ) )

    	{

	    if ( !IS_NPC(victim) )

	    {

	    	send_to_char( "Not on PC's.\n\r", ch );

	    	return;

	    }



	    if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )

	    {

	    	send_to_char( "No such spec fun.\n\r", ch );

	    	return;

	    }



	    return;

    	}

    }



    if (!str_prefix(type,"object"))

    {

    	/* string an obj */



   	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )

    	{

	    send_to_char( "Nothing like that in heaven or earth.\n\r", ch );

	    return;

    	}



        if ( !str_prefix( arg2, "name" ) )

    	{

	    free_string( obj->name );

	    obj->name = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "short" ) )

    	{

	    free_string( obj->short_descr );

	    obj->short_descr = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "long" ) )

    	{

	    free_string( obj->description );

	    obj->description = str_dup( arg3 );

	    return;

    	}



    	if ( !str_prefix( arg2, "ed" ) || !str_prefix( arg2, "extended"))

    	{

	    EXTRA_DESCR_DATA *ed;



	    argument = one_argument( argument, arg3 );

	    if ( argument == NULL )

	    {

	    	send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",

		    ch );

	    	return;

	    }



 	    strcat(argument,"\n\r");



	    ed = new_extra_descr();



	    ed->keyword		= str_dup( arg3     );

	    ed->description	= str_dup( argument );

	    ed->next		= obj->extra_descr;

	    obj->extra_descr	= ed;

	    return;

    	}

    }





    /* echo bad use message */

    do_string(ch,"");

}







void do_oset( CHAR_DATA *ch, char *argument )

{

    char arg1 [MAX_INPUT_LENGTH];

    char arg2 [MAX_INPUT_LENGTH];

    char arg3 [MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int value;



    smash_tilde( argument );

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    strcpy( arg3, argument );



    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )

    {

	send_to_char("Syntax:\n\r",ch);

	send_to_char("  set obj <object> <field> <value>\n\r",ch);

	send_to_char("  Field being one of:\n\r",				ch );

	send_to_char("    value0 value1 value2 value3 value4 (v1-v4)\n\r",	ch );

	send_to_char("    extra wear level weight cost timer\n\r",		ch );

	return;

    }



    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )

    {

	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );

	return;

    }



    /*

     * Snarf the value (which need not be numeric).

     */

    value = atoi( arg3 );



    /*

     * Set something.

     */

    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )

    {

	obj->value[0] = UMIN(50,value);

	return;

    }



    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )

    {

	obj->value[1] = value;

	return;

    }



    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )

    {

	obj->value[2] = value;

	return;

    }



    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )

    {

	obj->value[3] = value;

	return;

    }



    if ( !str_cmp( arg2, "value4" ) || !str_cmp( arg2, "v4" ) )

    {

	obj->value[4] = value;

	return;

    }



    if ( !str_prefix( arg2, "extra" ) )

    {

	obj->extra_flags = value;

	return;

    }



    if ( !str_prefix( arg2, "wear" ) )

    {

	obj->wear_flags = value;

	return;

    }



    if ( !str_prefix( arg2, "level" ) )

    {

	obj->level = value;

	return;

    }



    if ( !str_prefix( arg2, "weight" ) )

    {

	obj->weight = value;

	return;

    }



    if ( !str_prefix( arg2, "cost" ) )

    {

	obj->cost = value;

	return;

    }



    if ( !str_prefix( arg2, "timer" ) )

    {

	obj->timer = value;

	return;

    }



    /*

     * Generate usage message.

     */

    do_oset( ch, "" );

    return;

}







void do_rset( CHAR_DATA *ch, char *argument )

{

    char arg1 [MAX_INPUT_LENGTH];

    char arg2 [MAX_INPUT_LENGTH];

    char arg3 [MAX_INPUT_LENGTH];

    ROOM_INDEX_DATA *location;

    int value;



    smash_tilde( argument );

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    strcpy( arg3, argument );



    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )

    {

	send_to_char( "Syntax:\n\r",ch);

	send_to_char( "  set room <location> <field> <value>\n\r",ch);

	send_to_char( "  Field being one of:\n\r",			ch );

	send_to_char( "    flags sector\n\r",				ch );

	return;

    }



    if ( ( location = find_location( ch, arg1 ) ) == NULL )

    {

	send_to_char( "No such location.\n\r", ch );

	return;

    }



    if (!is_room_owner(ch,location) && ch->in_room != location

    &&  room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR))

    {

        send_to_char("That room is private right now.\n\r",ch);

        return;

    }



    /*

     * Snarf the value.

     */

    if ( !is_number( arg3 ) )

    {

	send_to_char( "Value must be numeric.\n\r", ch );

	return;

    }

    value = atoi( arg3 );



    /*

     * Set something.

     */

    if ( !str_prefix( arg2, "flags" ) )

    {

	location->room_flags	= value;

	return;

    }



    if ( !str_prefix( arg2, "sector" ) )

    {

	location->sector_type	= value;

	return;

    }



    /*

     * Generate usage message.

     */

    do_rset( ch, "" );

    return;

}



void do_sockets( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA       *vch;

    DESCRIPTOR_DATA *d;

    char            buf  [ MAX_STRING_LENGTH ];

    char            buf2 [ MAX_STRING_LENGTH ];

    int             count;

    char *          st;

    char            s[100];

    char            idle[10];





    count       = 0;

    buf[0]      = '\0';

    buf2[0]     = '\0';



    strcat( buf2, "\n\r[ ID Connected State  Login@  Idle ] Player Name  Host\n\r" );

    strcat( buf2,

"{b--------------------------------------------------------------------------------------------\n\r");

    for ( d = descriptor_list; d; d = d->next )

    {
    ch->pcdata->host = d->host;

        if ( d->character != NULL && can_see( ch, d->character ) )

        {

           /* NB: You may need to edit the CON_ values */

           switch( d->connected )

           {

              case CON_PLAYING:              st = "{g    PLAYING    ";    break;

              case CON_GET_NAME:             st = "{c   Get Name    ";    break;

              case CON_GET_OLD_PASSWORD:     st = "{cGet Old Passwd ";    break;

              case CON_CONFIRM_NEW_NAME:     st = "{c Confirm Name  ";    break;

              case CON_GET_NEW_PASSWORD:     st = "{cGet New Passwd ";    break;

              case CON_CONFIRM_NEW_PASSWORD: st = "{cConfirm Passwd ";    break;

              case CON_GET_NEW_RACE:         st = "{c Get New Race  ";    break;

              case CON_GET_NEW_SEX:          st = "{c  Get New Sex  ";    break;

              case CON_GET_NEW_CLASS:        st = "{c Get New Class ";    break;

              case CON_GET_ALIGNMENT:   	   st = "{c Get New Align ";	  break;

              case CON_DEFAULT_CHOICE:	   st = "{c Choosing Cust ";	  break;

              case CON_GEN_GROUPS:		   st = "{c Customization ";	  break;

              case CON_PICK_WEAPON:		   st = "{cPicking Weapon ";	  break;

		  case CON_READ_IMOTD:		   st = "{c Reading IMOTD "; 	  break;

		  case CON_BREAK_CONNECT:	   st = "{r   LINKDEAD    ";	  break;

              case CON_READ_MOTD:            st = "{c  Reading MOTD ";    break;

		  case CON_LOGIN_WIZI:		   st = "{c  Wizi Login   ";	  break;

		  case CON_BEGIN_REMORT:	   st = "{r Begin Remort  ";	  break;

		  case CON_SCROLL_WAIT:		   st = "{g  Scroll Wait  ";	  break;

              default:                       st = "{m   !UNKNOWN!   ";    break;

           }

           count++;



           /* Format "login" value... */

           vch = d->original ? d->original : d->character;

           strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );



           if ( vch->timer > 0 )

              sprintf( idle, "%4d", vch->timer );

           else

              sprintf( idle, "    " );



           sprintf( buf, "{W[{w%3d  %16s {D%7s {y%s{W ]{c %-12s{w %-40.40s\n\r",

              d->descriptor,

              st,

              s,

              idle,

              ( d->original ) ? d->original->name

                              : ( d->character )  ? d->character->name : "{R({rNone!{R){w",

              d->host);



           strcat( buf2, buf );



        }

    }



    sprintf( buf, "\n\r%d user%s\n\r", count, count == 1 ? "" : "s" );

    strcat( buf2, buf );

    send_to_char( buf2, ch );

    return;

}





/*

 * Thanks to Grodyn for pointing out bugs in this function.

 */

void do_force( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];



    argument = one_argument( argument, arg );



    if ( arg[0] == '\0' || argument[0] == '\0' )

    {

	send_to_char( "{wForce <name> <command>\n\r", ch );

	send_to_char( "IMM may force MOB's by name\n\r", ch );

	send_to_char( "GOD may force anyone (of lower trust) by name\n\r", ch );

	send_to_char( "DEI may force {gplayers{w (all mortals)\n\r", ch );

	send_to_char( "SUP may force {ggods{w or {gall{w\n\r", ch );

	return;

    }



    one_argument(argument,arg2);



    if (!str_cmp(arg2,"delete"))

    {

	send_to_char("That will NOT be done.\n\r",ch);

	return;

    }



    sprintf( buf, "$n forces you to '%s'.", argument );



    if ( !str_cmp( arg, "all" ) )

    {

	CHAR_DATA *vch;

	CHAR_DATA *vch_next;



	if (get_trust(ch) < SUPREME)

	{

	    send_to_char("Not at your level!\n\r",ch);

	    return;

	}



	for ( vch = char_list; vch != NULL; vch = vch_next )

	{

	    vch_next = vch->next;



	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )

	    {

		act( buf, ch, NULL, vch, TO_VICT );

		interpret( vch, argument );

	    }

	}

    }

    else if (!str_cmp(arg,"players"))

    {

        CHAR_DATA *vch;

        CHAR_DATA *vch_next;



        if (get_trust(ch) < DEITY)

        {

            send_to_char("Not at your level!\n\r",ch);

            return;

        }



        for ( vch = char_list; vch != NULL; vch = vch_next )

        {

            vch_next = vch->next;



            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )

	    &&	 vch->level < LEVEL_HERO)

            {

                act( buf, ch, NULL, vch, TO_VICT );

                interpret( vch, argument );

            }

        }

    }

    else if (!str_cmp(arg,"gods"))

    {

        CHAR_DATA *vch;

        CHAR_DATA *vch_next;



        if (get_trust(ch) < SUPREME)

        {

            send_to_char("Not at your level!\n\r",ch);

            return;

        }



        for ( vch = char_list; vch != NULL; vch = vch_next )

        {

            vch_next = vch->next;



            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )

            &&   vch->level >= LEVEL_HERO)

            {

                act( buf, ch, NULL, vch, TO_VICT );

                interpret( vch, argument );

            }

        }

    }

    else

    {

	CHAR_DATA *victim;



	if ( ( victim = get_char_world( ch, arg ) ) == NULL )

	{

	    send_to_char( "They are not here.\n\r", ch );

	    return;

	}



	if ( victim == ch )

	{

	    send_to_char( "Aye aye, right away!\n\r", ch );

	    return;

	}



    	if (!is_room_owner(ch,victim->in_room)

	&&  ch->in_room != victim->in_room

        &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))

    	{

            send_to_char("That character is in a private room.\n\r",ch);

            return;

        }



	if ( get_trust( victim ) >= get_trust( ch ) )

	{

	    send_to_char( "Do it yourself!\n\r", ch );

	    return;

	}



	if ( !IS_NPC(victim) && get_trust(ch) < GOD)

	{

	    send_to_char("Not at your level!\n\r",ch);

	    return;

	}



	act( buf, ch, NULL, victim, TO_VICT );

	interpret( victim, argument );

    }



    send_to_char( "Ok.\n\r", ch );

    return;

}







/*

 * New routines by Dionysos.

 */

void do_invis( CHAR_DATA *ch, char *argument )

{

    int level;

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_STRING_LENGTH];

    char awiz[MAX_STRING_LENGTH];

    int owizlvl;

    DESCRIPTOR_DATA *d;



    /* RT code for taking a level argument */

    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

    /* take the default path */



      if ( ch->invis_level )

      {

	  ch->invis_level = 0;

	  sprintf(awiz, awiz_lvl(ch));

	  for ( d = descriptor_list; d; d = d->next )

	  {

	    if ( d->connected == CON_PLAYING

	    &&   d->character->in_room == ch->in_room

	    &&   can_see(d->character, ch)

	    &&   d->character != ch )

	    {

		if (IS_SET(d->character->act, PLR_AUTOWIZI))
		{
			if (ch->pcdata != NULL && ch->pcdata->wiziin[0] != '\0')
				sprintf(buf, "%s %s\n\r", ch->pcdata->wiziin, awiz);
			else
				sprintf( buf, "{w%s{w slowly fades into existence. %s{w\n\r", ch->name, awiz );
		}

		else
		{
			if (ch->pcdata != NULL && ch->pcdata->wiziin[0] != '\0')
				sprintf(buf, "%s\n\r", ch->pcdata->wiziin);
			else
				sprintf( buf, "{w%s{w slowly fades into existence.\n\r", ch->name );
	    }

		send_to_char( buf, d->character );

	    }

	  }

	  if (IS_SET(ch->act, PLR_AUTOWIZI))

		sprintf( buf, "{wYou slowly fade back into existence. %s{w\n\r", awiz );

	  else

		sprintf( buf, "{wYou slowly fade back into existence.\n\r" );

	  send_to_char( buf, ch );

      }

      else

      {

	  owizlvl = ch->invis_level;

        ch->invis_level = get_trust(ch);

	  sprintf(awiz, awiz_lvl(ch));

        ch->invis_level = owizlvl;

	  for ( d = descriptor_list; d; d = d->next )

	  {

		if ( d->connected == CON_PLAYING

		&&   d->character->in_room == ch->in_room

		&&   can_see(d->character, ch)

		&&   d->character != ch )

		{

		   if (IS_SET(d->character->act, PLR_AUTOWIZI))
		   {
			   if (ch->pcdata !=NULL && ch->pcdata->wiziout[0] != '\0')
			   {
			   		sprintf(buf, "%s %s\n\r", ch->pcdata->wiziout, awiz);
				}
			   else
			   		sprintf(buf, "{w%s{w slowly vanishes into thin air. %s{w\n\r", ch->name, awiz );
		   }

		   else
		   {
			    if(ch->pcdata !=NULL && ch->pcdata->wiziout[0] !='\0')
			   		sprintf(buf, "%s\n\r", ch->pcdata->wiziout);
				else
					sprintf(buf, "{w%s{w slowly vanishes into thin air.\n\r", ch->name );
			}

		   send_to_char( buf, d->character );

    		}

	  }

	  if (IS_SET(ch->act, PLR_AUTOWIZI))

	  {

		sprintf(buf, "{wYou slowly vanish into thin air. %s{w\n\r", awiz );

		send_to_char( buf, ch );

	  }

	  else

		send_to_char( "{wYou slowly vanish into thin air.\n\r", ch );

        ch->invis_level = get_trust(ch);

      }

    }

    else

    /* do the level thing */

    {

    	if (!is_number(arg))

    	{

		level = lev_num(ch,arg);

    	}

    	else

    	{

		level = atoi( arg );

    	}



      if (level > get_trust(ch))

      {

	  send_to_char("Invis level must be less than or equal to your level.\n\r",ch);

        return;

      }

      else

      {

	  if (level == 0)

	  {

		ch->invis_level = 0;

		sprintf(awiz, awiz_lvl(ch));

		for ( d = descriptor_list; d; d = d->next )

		{

		   if ( d->connected == CON_PLAYING

		   &&   d->character->in_room == ch->in_room

		   &&   can_see(d->character, ch)

		   &&   d->character != ch )

		   {

				if (IS_SET(d->character->act, PLR_AUTOWIZI))
				{
					if (ch->pcdata != NULL && ch->pcdata->wiziin[0] != '\0')
						sprintf(buf, "%s %s\n\r", ch->pcdata->wiziin, awiz);
					else
						sprintf( buf, "{w%s{w slowly fades into existence. %s{w\n\r", ch->name, awiz );
				}

				else
				{
					if (ch->pcdata != NULL && ch->pcdata->wiziin[0] != '\0')
						sprintf(buf, "%s\n\r", ch->pcdata->wiziin);
					else
						sprintf( buf, "{w%s{w slowly fades into existence.\n\r", ch->name );
				}

			send_to_char( buf, d->character );

		   }

		}

		if (IS_SET(ch->act, PLR_AUTOWIZI))

		   sprintf( buf, "{wYou slowly fade back into existence. %s{w\n\r", awiz );

		else

		   sprintf( buf, "{wYou slowly fade back into existence.\n\r" );

		send_to_char( buf, ch );

	  }

	  else

	  {

		owizlvl = ch->invis_level;

            ch->invis_level = level;

		sprintf(awiz, awiz_lvl(ch));

            ch->invis_level = owizlvl;

		for ( d = descriptor_list; d; d = d->next )

		{

		   if ( d->connected == CON_PLAYING

		   &&   d->character->in_room == ch->in_room

		   &&   d->character != ch

		   &&   can_see(d->character, ch) )

		   {

			   if (IS_SET(d->character->act, PLR_AUTOWIZI))
			   {
				   if (ch->pcdata !=NULL && ch->pcdata->wiziout[0] != '\0')
				   {
				   		sprintf(buf, "%s %s\n\r", ch->pcdata->wiziout, awiz);
					}
				   else
						sprintf(buf, "{w%s{w slowly vanishes into thin air. %s{w\n\r", ch->name, awiz );
			   }

			   else
			   {
					if(ch->pcdata !=NULL && ch->pcdata->wiziout[0] !='\0')
						sprintf(buf, "%s\n\r", ch->pcdata->wiziout);
					else
						sprintf(buf, "{w%s{w slowly vanishes into thin air.\n\r", ch->name );
				}

			send_to_char( buf, d->character );

    		   }

		}

		if (IS_SET(ch->act, PLR_AUTOWIZI))

		{

		   sprintf(buf, "{wYou slowly vanish into thin air. %s{w\n\r", awiz );

		   send_to_char( buf, ch );

		}

		else

		   send_to_char( "{wYou slowly vanish into thin air.\n\r", ch );

            ch->invis_level = level;

	  }

      }

    }



    return;

}





void do_incognito( CHAR_DATA *ch, char *argument )

{

    int level;

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_STRING_LENGTH];

    char awiz[MAX_STRING_LENGTH];

    char csex[5];

    int owizlvl;

    DESCRIPTOR_DATA *d;



    if (IS_SET(ch->sex,SEX_MALE))

	sprintf(csex ,"his");

    else if (IS_SET(ch->sex,SEX_FEMALE))

	sprintf(csex, "her");

    else

	sprintf(csex, "its");



    /* RT code for taking a level argument */

    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

    /* take the default path */



      if ( ch->incog_level)

      {

	  ch->incog_level = 0;

	  sprintf(awiz, awiz_lvl(ch));

	  for ( d = descriptor_list; d; d = d->next )

	  {

	    if ( d->connected == CON_PLAYING

	    &&   d->character->in_room == ch->in_room

	    &&   can_see(d->character, ch)

	    &&   d->character != ch )

	    {

		if (IS_SET(d->character->act, PLR_AUTOWIZI))

		   sprintf( buf, "{w%s{w is no longer cloaked. %s{w\n\r", ch->name, awiz );

		else

		   sprintf( buf, "{w%s{w is no longer cloaked.\n\r", ch->name );

		send_to_char( buf, d->character );

	    }

	  }

	  if (IS_SET(ch->act, PLR_AUTOWIZI))

		sprintf( buf, "{wYou are no longer cloaked. %s{w\n\r", awiz );

	  else

		sprintf( buf, "{wYou are no longer cloaked.\n\r" );

	  send_to_char( buf, ch );

      }

      else

      {

	  owizlvl = ch->incog_level;

        ch->incog_level = get_trust(ch);

	  sprintf(awiz, awiz_lvl(ch));

        ch->incog_level = owizlvl;

	  for ( d = descriptor_list; d; d = d->next )

	  {

		if ( d->connected == CON_PLAYING

		&&   d->character->in_room == ch->in_room

		&&   d->character != ch

		&&   can_see(d->character, ch) )

		{

		   if (IS_SET(d->character->act, PLR_AUTOWIZI))

			sprintf(buf, "{w%s{w cloaks %s presence. %s{w\n\r", ch->name, csex,	awiz );

		   else

			sprintf(buf, "{w%s{w cloaks %s presence.\n\r", ch->name, csex );

		   send_to_char( buf, d->character );

    		}

	  }

	  if (IS_SET(ch->act, PLR_AUTOWIZI))

	  {

		sprintf(buf, "{wYou cloak your presence. %s{w\n\r", awiz );

		send_to_char( buf, ch );

	  }

	  else

		send_to_char( "{wYou cloak your presence.\n\r", ch );

        ch->incog_level = get_trust(ch);

      }

    }

    else

    /* do the level thing */

    {

    	if (!is_number(arg))

    	{

		level = lev_num(ch,arg);

    	}

    	else

    	{

		level = atoi( arg );

    	}



      if (level > get_trust(ch))

      {

	send_to_char("Incog level must be less than or equal to your level.\n\r",ch);

        return;

      }

      else

      {

	    if (level == 0)

	    {

		ch->incog_level = 0;

		sprintf(awiz, awiz_lvl(ch));

		for ( d = descriptor_list; d; d = d->next )

		{

		   if ( d->connected == CON_PLAYING

		   &&   d->character->in_room == ch->in_room

		   &&   can_see(d->character, ch)

		   &&   d->character != ch )

		   {

			if (IS_SET(d->character->act, PLR_AUTOWIZI))

			   sprintf( buf, "{w%s{w is no longer cloaked. %s{w\n\r", ch->name, awiz );

			else

			   sprintf( buf, "{w%s{w is no longer cloaked.\n\r", ch->name );

			send_to_char( buf, d->character );

		   }

		}

		if (IS_SET(ch->act, PLR_AUTOWIZI))

		   sprintf( buf, "{wYou are no longer cloaked. %s{w\n\r", awiz );

		else

		   sprintf( buf, "{wYou are no longer cloaked.\n\r" );

		send_to_char( buf, ch );

          }

	    else

	    {

		owizlvl = ch->incog_level;

            ch->incog_level = level;

		sprintf(awiz, awiz_lvl(ch));

            ch->incog_level = owizlvl;

		for ( d = descriptor_list; d; d = d->next )

		{

		   if ( d->connected == CON_PLAYING

		   &&   d->character->in_room == ch->in_room

		   &&   d->character != ch

		   &&   can_see(d->character, ch) )

		   {

			if (IS_SET(d->character->act, PLR_AUTOWIZI))

				sprintf(buf, "{w%s{w cloaks %s presence. %s{w\n\r", ch->name, csex, awiz );

			else

				sprintf(buf, "{w%s{w cloaks %s presence.\n\r", ch->name, csex );

			send_to_char( buf, d->character );

    		   }

		}

		if (IS_SET(ch->act, PLR_AUTOWIZI))

		{

		   sprintf(buf, "{wYou cloak your presence. %s{w\n\r", awiz );

		   send_to_char( buf, ch );

		}

		else

		   send_to_char( "{wYou cloak your presence.\n\r", ch );

            ch->incog_level = level;

	    }

      }

    }



    return;

}







void do_holylight( CHAR_DATA *ch, char *argument )

{

    if ( IS_NPC(ch) )

	return;



    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )

    {

	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);

	send_to_char( "Holy light mode off.\n\r", ch );

    }

    else

    {

	SET_BIT(ch->act, PLR_HOLYLIGHT);

	send_to_char( "Holy light mode on.\n\r", ch );

    }



    return;

}



/* prefix command: it will put the string typed on each line typed */



void do_prefi (CHAR_DATA *ch, char *argument)

{

    send_to_char("You cannot abbreviate the prefix command.\r\n",ch);

    return;

}



void do_prefix (CHAR_DATA *ch, char *argument)

{

    char buf[MAX_INPUT_LENGTH];



    if (argument[0] == '\0')

    {

	if (ch->prefix[0] == '\0')

	{

	    send_to_char("You have no prefix to clear.\r\n",ch);

	    return;

	}



	send_to_char("Prefix removed.\r\n",ch);

	free_string(ch->prefix);

	ch->prefix = str_dup("");

	return;

    }



    if (ch->prefix[0] != '\0')

    {

	sprintf(buf,"Prefix changed to %s.\r\n",argument);

	free_string(ch->prefix);

    }

    else

    {

	sprintf(buf,"Prefix set to %s.\r\n",argument);

    }



    ch->prefix = str_dup(argument);

}



void do_restring( CHAR_DATA *ch, char *argument )

{

    char type [MAX_INPUT_LENGTH];

    char arg [MAX_INPUT_LENGTH];

    CHAR_DATA *engraver;

    OBJ_DATA *obj;

    OBJ_DATA *prize;

    bool have_prize;

    char buf[MAX_INPUT_LENGTH];





    if (IS_NPC(ch))

	return;



    have_prize = FALSE;



    smash_tilde( argument );

    argument = one_argument( argument, type );

    strcpy( arg, argument );



    if ( type[0] == '\0' )

    {

	send_to_char("{rSyntax:{w\n\r",ch);

	send_to_char("{grestring object <old object>   {w-- Object to be restrung.                {w\n\r", ch);

	send_to_char("{W                               {WThe name you currently use to refer      {w\n\r", ch);

	send_to_char("{W                               {Wto the object when you wield or hold it. {w\n\r", ch);

	send_to_char("{grestring name <new object>     {w-- New name of the object.               {w\n\r", ch);

	send_to_char("{W                               {WThe new name of the object that you will {w\n\r", ch);

	send_to_char("{W                               {Wuse to wield or hold it in the future.   {w\n\r", ch);

	send_to_char("{grestring short <description>   {w-- New Short Description of object.      {w\n\r", ch);

	send_to_char("{W                               {WHow the object appears when in your      {w\n\r", ch);

	send_to_char("{W                               {Winventory or equipment list.             {w\n\r", ch);

	send_to_char("{grestring long <description>    {w-- New Long Description of object.       {w\n\r", ch);

	send_to_char("{W                               {WHow the object appears when lying on the {w\n\r", ch);

	send_to_char("{W                               {Wground or when you type {glook <name>{W  {w\n\r", ch);

	send_to_char("{grestring show                  {w-- Show current restring settings.       {w\n\r", ch);

	send_to_char("{grestring clear                 {w-- Clears all restring settings.         {w\n\r", ch);

	send_to_char("{grestring do                    {w-- Restring the object per settings.     {w\n\r", ch);

	send_to_char("\n\r", ch);

	send_to_char("{WYou must be in the restring shop, with the object and restring prize      {w\n\r", ch);

	send_to_char("{Win your inventory and the Engraver present.  When you actual do the       {w\n\r", ch);

	send_to_char("{Wcommand, the engraver will take your restring prize and engrave the       {w\n\r", ch);

	send_to_char("{Wobject as you have requested.                                             {w\n\r", ch);

	send_to_char("\n\r", ch);

	return;

    }



    if (!IS_SET(ch->in_room->room_flags, ROOM_ENGRAVING) )

    {

	sprintf( buf, "But you are not in a Restring Shoppe.\n\r" );

	send_to_char( buf, ch );

	return;

    }



    engraver = NULL;

    for ( engraver = ch->in_room->people; engraver; engraver = engraver->next_in_room )

    {

	if ( IS_NPC( engraver ) && IS_SET(engraver->pIndexData->act, ACT_ENGRAVER) )

	    break;

    }



    if ( !engraver )

    {

	 sprintf( buf, "The engraver is currently not available.\n\r" );

	 send_to_char( buf, ch );

	 return;

    }



    if ( !str_prefix( type, "object" ) )

    {

	ch->pcdata->rs_oname = str_dup( "" );

	ch->pcdata->rs_oname = str_dup( arg );



	sprintf( buf, "%s{w writes down the objects name on a note pad.\n\r", engraver->short_descr );

	send_to_char( buf, ch );



	return;

    }

    else if ( !str_prefix( type, "name" ) )

    {

	ch->pcdata->rs_nname = str_dup( "" );

	ch->pcdata->rs_nname = str_dup( arg );



	sprintf( buf, "%s{w writes down the objects new name on a note pad.\n\r", engraver->short_descr );

	send_to_char( buf, ch );



	return;

    }

    else if ( !str_prefix( type, "short" ) )

    {

	ch->pcdata->rs_sdesc = str_dup( "" );

	ch->pcdata->rs_sdesc = str_dup( arg );



	sprintf( buf, "%s{w writes down what you see when you hold the object on a note pad.\n\r", engraver->short_descr );

	send_to_char( buf, ch );



	return;

    }

    else if ( !str_prefix( type, "long" ) )

    {

	ch->pcdata->rs_ldesc = str_dup( "" );

	ch->pcdata->rs_ldesc = str_dup( arg );



	sprintf( buf, "%s{w writes down the what you see when you look at the object on a note pad.\n\r", engraver->short_descr );

	send_to_char( buf, ch );



	return;

    }

    else if ( !str_prefix( type, "show" ) )

    {

	sprintf( buf, "%s{w reads from his notepad...\n\r", engraver->short_descr );

	send_to_char( buf, ch );

	sprintf( buf, "Object's Current Name ...............: %s\n\r", ch->pcdata->rs_oname );

	send_to_char( buf, ch );

	sprintf( buf, "Object's New Name ...................: %s\n\r", ch->pcdata->rs_nname );

	send_to_char( buf, ch );

	sprintf( buf, "Description of Object when held .....: %s\n\r", ch->pcdata->rs_sdesc );

	send_to_char( buf, ch );

	sprintf( buf, "Description of object when viewed ...: %s\n\r", ch->pcdata->rs_ldesc );

	send_to_char( buf, ch );



	return;

    }

    else if ( !str_prefix( type, "clear" ) )

    {

	ch->pcdata->rs_oname		= str_dup( "" );

	ch->pcdata->rs_nname		= str_dup( "" );

	ch->pcdata->rs_sdesc		= str_dup( "" );

	ch->pcdata->rs_ldesc		= str_dup( "" );



	sprintf( buf, "%s{w erases a note pad and waits for you to begin again.\n\r", engraver->short_descr );

	send_to_char( buf, ch );



	return;

    }

    else if ( !str_prefix( type, "do" ) )

    {

	if (  ch->pcdata->rs_oname == '\0'

	   || ch->pcdata->rs_nname == '\0'

	   || ch->pcdata->rs_sdesc == '\0'

	   || ch->pcdata->rs_ldesc == '\0' )

	{

		sprintf( buf, "%s{g tells you 'I do not have all the information I need to finish the job.'\n\r", engraver->short_descr );

		send_to_char( buf, ch );

		do_restring( ch, "show" );

		return;

	}



	if ( ( obj = get_obj_carry( ch, ch->pcdata->rs_oname, ch ) ) == NULL )

	{

		sprintf( buf, "{g%s{g tells you 'You do not have that object in your inventory.'\n\r", engraver->short_descr );

		send_to_char( buf, ch);

		return;

	}



	for ( prize = ch->carrying; prize != NULL; prize = prize->next_content )

	{

	  if ( prize->wear_loc == WEAR_NONE

	  &&   (can_see_obj( ch, prize ) )

	  &&   (prize->pIndexData->vnum == OBJ_VNUM_RESTRING_PRIZE) )

	  {

		obj_from_char( prize );

		have_prize = TRUE;

	  }

	}



	if (!have_prize)

	{

		sprintf(buf, "{g%s{g tells you 'You do not have any more restring prizes in your inventory.'\n\r", engraver->short_descr );

		send_to_char( buf, ch );

		return;

	}

	else

	{

		sprintf( buf, "{w%s{w thanks you as you give him a restring prize for payment.\n\r", engraver->short_descr );

		send_to_char( buf, ch);

	}



	sprintf( buf, "%s{w takes %s{w and begins to work on it as you wait patiently.\n\r", engraver->short_descr, obj->short_descr );

	send_to_char( buf, ch );



	free_string( obj->name );

	obj->name = str_dup( ch->pcdata->rs_nname );



	free_string( obj->short_descr );

	obj->short_descr = str_dup( ch->pcdata->rs_sdesc );



	free_string( obj->description );

	obj->description = str_dup( ch->pcdata->rs_ldesc );



	WAIT_STATE( ch, 24 );



	sprintf( buf, "%s{w hands you back your %s{w and smiles at you, his work complete.\n\r", engraver->short_descr, obj->short_descr );

	send_to_char( buf, ch );

	sprintf( buf, "{g%s{g tells you 'Thank you %s{g, please come again.'\n\r", engraver->short_descr, ch->name );

	send_to_char( buf, ch );



	ch->pcdata->rs_oname		= str_dup( "" );

	ch->pcdata->rs_nname		= str_dup( "" );

	ch->pcdata->rs_sdesc		= str_dup( "" );

	ch->pcdata->rs_ldesc		= str_dup( "" );



	return;

    }

    else

	do_restring( ch, "" );



    return;

}



void do_addlag(CHAR_DATA *ch, char *argument)

{



	CHAR_DATA *victim;

	char arg1[MAX_STRING_LENGTH];

	int x;

      char buf[MAX_INPUT_LENGTH];



	argument = one_argument(argument, arg1);



	if (arg1[0] == '\0')

	{

		send_to_char("SYNTAX: addlag <victim> <ammount (1-10000)>\n\r", ch);

		return;

	}



	if ((victim = get_char_world(ch, arg1)) == NULL)

	{

		send_to_char("They're not here.\n\r", ch);

		return;

	}



	if ((x = atoi(argument)) <= 0)

	{

		send_to_char("SYNTAX: addlag <victim> <amount (1-10000)>\n\r", ch);

		return;

	}



	if (x > 10000)

	{

		send_to_char("SYNTAX: addlag <victim> <amount (1-10000)>\n\r", ch);

		return;

	}



	if (get_trust(ch) < get_trust(victim))

	{

		send_to_char("That was a bad idea.\n\r", ch);

		return;

	}



	if (IS_HERO(victim))

		send_to_char("You have been {rLAGGED!{w\n\r", victim);

	WAIT_STATE(victim, x);

	send_to_char("Spewing {rLAG{w now...\n\r", ch);

	sprintf(buf,"%s spewed lag onto %s for %d pulses.", ch->name, victim->name, x);

    	wiznet(buf,NULL,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));

	log_string( buf );

	return;

}





void do_bonus(CHAR_DATA *ch, char *argument)

{



	CHAR_DATA *victim;

	char arg1[MAX_STRING_LENGTH];

	int x;

      char buf[MAX_INPUT_LENGTH];



	argument = one_argument(argument, arg1);



	if (arg1[0] == '\0')

	{

		send_to_char("SYNTAX: bonus <character> <ammount (1-1000)>\n\r", ch);

		return;

	}



	if ((victim = get_char_world(ch, arg1)) == NULL)

	{

		send_to_char("They're not here.\n\r", ch);

		return;

	}



	if (IS_NPC(victim))

	{

		send_to_char("Bonuses may only be applied to Players.\n\r", ch);

		return;

	}



	if ((x = atoi(argument)) < 1)

	{

		send_to_char("SYNTAX: bonus <character> <ammount (1-1000)>\n\r", ch);

		return;

	}



	if (x > 1000)

	{

		send_to_char("SYNTAX: bonus <character> <ammount (1-1000)>\n\r", ch);

		return;

	}



	if (IS_HERO(victim))

	{

		send_to_char("You can not add Experience Points to Immortals.\n\r", ch);

		return;

	}



	gain_exp( victim, x );



	sprintf(buf,"%s gave %s %d experience points as a bonus.", ch->name, victim->name, x);

    	wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

	log_string( buf );

	return;

}





void do_tabledump( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];

    int line_count;

    int cnt;

    bool sl_all;

    bool sl_clan;

    bool sl_econ;

    bool sl_combat;

    bool sl_curse;




/*    int iCnt; */



    argument = one_argument(argument,arg);



    if (arg[0] == '\0')

    {

	send_to_char("{rSyntax:{w\n\r",ch);

	if (IS_ADMIN(ch))

	{

		send_to_char("{rSyntax{w: {ctabledump save {R<{Wall{g|{Wclan{g|{Wecon{g|{Wcombat{g|{Wcurse{R>{w  \n\r",ch);

		send_to_char("{wSaves the global tables to a file.{w                        \n\r",ch);

		send_to_char("{rSyntax{w: {ctabledump load {R<{Wall{g|{Wclan{g|{Wecon{g|{Wcombat{g|{Wcurse{R>{w  \n\r",ch);

		send_to_char("{wLoads the global tables from files.{w                       \n\r",ch);

		send_to_char("\n\r", ch);

		send_to_char("{g   Tables Loaded/Saved {w\n\r", ch);

		send_to_char("{c   Clan Table          {w\n\r", ch);

		send_to_char("{c   Economy Table       {w\n\r", ch);

		send_to_char("{c   Combat Table        {w\n\r", ch);

		send_to_char("\n\r", ch);

	}



	send_to_char("{gtabledump <tablename>{w\n\r",ch);

	send_to_char("   Dumps the contents of a table.\n\r\n\r", ch);



	send_to_char("   {rSystem{w",ch);

	if (IS_IMMORTAL(ch))

		send_to_char(", {gCharacter{w",ch);

	if (IS_BUILDER(ch))

		send_to_char(", {WOLC/Character{w, {bOLC{w",ch);

	send_to_char("{w\n\r\n\r",ch);



	send_to_char("{c   Table Name           Description     {w\n\r",ch);

	send_to_char("{c   ----------           ----------------{w\n\r",ch);



	send_to_char("{r   tips                 Tip Table       {w\n\r",ch);

	send_to_char("{r   curse                Censored Words  {w\n\r",ch);



	if (IS_IMMORTAL(ch))

	{

		send_to_char("{r   clan                 Clan Table      {w\n\r",ch);

		send_to_char("{r   economy              Economy Table   {w\n\r",ch);

		send_to_char("{r   combat               Combat Table    {w\n\r",ch);

		send_to_char("{r   insult               Insult Table    {w\n\r",ch);

		send_to_char("{g   olc                  OLC Flags       {w\n\r",ch);

/*		send_to_char("{g   position             Position Flags  {w\n\r",ch);   */

		send_to_char("{g   her                  Hero Flags      {w\n\r",ch);

		send_to_char("{g   ang                  Angel Flags     {w\n\r",ch);

		send_to_char("{g   sup                  Supremacy Flags {w\n\r",ch);

		send_to_char("{g   stat                 Stat Flags      {w\n\r",ch);

		send_to_char("{g   comm                 Comm Flags      {w\n\r",ch);

		send_to_char("{g   info                 Info Flags      {w\n\r",ch);

		send_to_char("{W   act                  Act Flags       {w\n\r",ch);

		send_to_char("{W   plr                  Plr Flags       {w\n\r",ch);

		send_to_char("{g   race                 Race Table      {w\n\r",ch);

/*		send_to_char("{g   pcrace               PC Race Table   {w\n\r",ch); */

/*		send_to_char("{g   class                Class Table     {w\n\r",ch); */

                send_to_char("{g   cyber                Cyber Table     {w\n\r",ch);

	}



/*

 * Not included, for now

 *

	if (IS_BUILDER(ch))

	{

		send_to_char("{W   act                  Act Flags       {w\n\r",ch);

		send_to_char("{W   plr                  Plr Flags       {w\n\r",ch);

		send_to_char("{W   affect               Affect Flags    {w\n\r",ch);

		send_to_char("{W   offense              Offense Flags   {w\n\r",ch);

		send_to_char("{W   imm                  Immunities      {w\n\r",ch);

		send_to_char("{W   resist               Resistances     {w\n\r",ch);

		send_to_char("{W   vuln                 Vulnerabilities {w\n\r",ch);

		send_to_char("{W   form                 Form Flags      {w\n\r",ch);

		send_to_char("{W   part                 Body Parts      {w\n\r",ch);

		send_to_char("{W   type                 Type Flags      {w\n\r",ch);

		send_to_char("{W   area                 Area Flags      {w\n\r",ch);

		send_to_char("{b   room                 Room Flags      {w\n\r",ch);

		send_to_char("{b   extra                Extra Flags     {w\n\r",ch);

		send_to_char("{b   wear                 Wear Flags      {w\n\r",ch);

		send_to_char("{b   wtype                Weapon Type     {w\n\r",ch);

		send_to_char("{b   wclass               Weapon Class    {w\n\r",ch);

		send_to_char("{b   wflag                Weapon Flag     {w\n\r",ch);

		send_to_char("{b   furniture            Furniture Flags {w\n\r",ch);

	}

*/



	return;

    }



    if (!str_prefix(arg,"save"))

    {

	if ( !IS_ADMIN(ch) )

	{

		do_tabledump( ch, "" );

		return;

	}



	argument = one_argument(argument,arg2);



	sl_all	= FALSE;

	sl_clan	= FALSE;

	sl_econ	= FALSE;

    sl_combat = FALSE;

	sl_curse	= FALSE;



	if ( arg2[0] == '\0' )

	{

		do_tabledump( ch, "" );

		return;

	}



	if ( !str_prefix( arg2, "all" ) )

	{

		sl_all	= TRUE;

	}



	if ( !str_prefix( arg2, "clan" ) )

	{

		sl_clan	= TRUE;

	}



	if ( !str_prefix( arg2, "econ" ) )

	{

		sl_econ	= TRUE;

	}

    if ( !str_prefix( arg2, "combat" ) )
    {
        sl_combat = TRUE;
    }

	if ( !str_prefix( arg2, "curse" ) )

	{

		sl_curse	= TRUE;

	}



	if ( !(sl_all || sl_clan || sl_econ || sl_combat || sl_curse) )

	{

		do_tabledump( ch, "" );

		return;

	}



	/* All Tables */

	if ( sl_all )

		tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_ALL );



	/* Clan Table */

	if ( sl_clan )

		tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_CLAN );



	/* Economy Table */

	if ( sl_econ )

		tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_ECON );

    /* Combat Table */
    if ( sl_combat )
        tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_COMBAT );

	/* Censor Table */

	if ( sl_curse )

		tablefiles_update( TRUE, ch, TFU_SAVE, TRUE, TRUE, TFU_CURSE );



	send_to_char( "Ok.\n\r", ch );

	return;

    }



    if (!str_prefix(arg,"load"))

    {

	if ( !IS_ADMIN(ch) )

	{

		do_tabledump( ch, "" );

		return;

	}



	argument = one_argument(argument,arg2);



	sl_all	= FALSE;

	sl_clan	= FALSE;

	sl_econ	= FALSE;

	sl_combat = FALSE;

	sl_curse	= FALSE;



	if ( arg2[0] == '\0' )

	{

		do_tabledump( ch, "" );

		return;

	}



	if ( !str_prefix( arg2, "all" ) )

	{

		sl_all	= TRUE;

	}



	if ( !str_prefix( arg2, "clan" ) )

	{

		sl_clan	= TRUE;

	}



	if ( !str_prefix( arg2, "econ" ) )

	{

		sl_econ	= TRUE;

	}

    if ( !str_prefix( arg2, "combat" ) )
    {
        sl_combat = TRUE;
    }

	if ( !str_prefix( arg2, "curse" ) )

	{

		sl_curse	= TRUE;

	}



	if ( !(sl_all || sl_clan || sl_econ || sl_combat || sl_curse) )

	{

		do_tabledump( ch, "" );

		return;

	}



	/* All Tables */

	if ( sl_all )

		tablefiles_update( TRUE, ch, TFU_LOAD, TRUE, TRUE, TFU_ALL );



	/* Clan Table */

	if ( sl_clan )

		tablefiles_update( TRUE, ch, TFU_LOAD, TRUE, TRUE, TFU_CLAN );



	/* Economy Table */

	if ( sl_econ )

		tablefiles_update( TRUE, ch, TFU_LOAD, TRUE, TRUE, TFU_ECON );

    /* Combat Table */
    if ( sl_combat )
        tablefiles_update( TRUE, ch, TFU_LOAD, TRUE, TRUE, TFU_COMBAT );

	/* Censor Table */

	if ( sl_curse )

		tablefiles_update( TRUE, ch, TFU_LOAD, TRUE, TRUE, TFU_CURSE );



	send_to_char( "Ok.\n\r", ch );

	return;

    }



    if (!str_prefix(arg,"tips"))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; tip_table[cnt].newbie != NULL; ++cnt)

		{

			sprintf( buf, "{gNewbie:         {w%s {w\n\r{cBeginner:       {w%s {w\n\r{yIntermediate:   {w%s {w\n\r{bRemort:         {w%s {w\n\r{rExperienced:    {w%s {w\n\r{mAdvanced:       {w%s {w\n\r",

				tip_table[cnt].newbie,

				tip_table[cnt].beginner,

				tip_table[cnt].intermediate,

				tip_table[cnt].remort,

				tip_table[cnt].experienced,

				tip_table[cnt].advanced );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "tips", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char("Tips:\n\r\n\r", ch );

		line_count = 2;

		for ( cnt = 0; tip_table[cnt].newbie != NULL; ++cnt)

		{

			sprintf( buf, "{gNewbie:         {w%s {w\n\r{cBeginner:       {w%s {w\n\r{yIntermediate:   {w%s {w\n\r{bRemort:         {w%s {w\n\r{rExperienced:    {w%s {w\n\r{mAdvanced:       {w%s {w\n\r\n\r",

				tip_table[cnt].newbie,

				tip_table[cnt].beginner,

				tip_table[cnt].intermediate,

				tip_table[cnt].remort,

				tip_table[cnt].experienced,

				tip_table[cnt].advanced );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "tips", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}



	send_to_char( "{w\n\r", ch );

	sprintf( buf, "{wCurrent elements in{c tip_table {w(without NULL):     {g%d{w\n\r", cnt++ );

	send_to_char( buf, ch );

	sprintf( buf, "{wMaximum elements in{c tip_table {w(including NULL):   {c%d{w\n\r", MAX_TIPS );

	send_to_char( buf, ch );

	return;

    }



    if (!str_prefix(arg,"curse"))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; censor_table[cnt].word != NULL; ++cnt)

		{

			sprintf( buf, "{g     %s{w\n\r", censor_table[cnt].word );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "curse", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char("Censored Words:\n\r\n\r", ch );

		line_count = 2;

		for ( cnt = 0; censor_table[cnt].word != NULL; ++cnt)

		{

			sprintf( buf, "{g     %s{w\n\r", censor_table[cnt].word );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "curse", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}



	send_to_char( "{w\n\r", ch );

	sprintf( buf, "{wCurrent elements in{c censor_table {w(without NULL):     {g%d{w\n\r", cnt++ );

	send_to_char( buf, ch );

	sprintf( buf, "{wMaximum elements in{c censor_table {w(including NULL):   {c%d{w\n\r", MAX_CENSOR );

	send_to_char( buf, ch );

	return;

    }



    if (!str_prefix(arg,"clan") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; clan_table[cnt].name != NULL; ++cnt)

		{

			if ( clan_table[cnt].active )

			{

				sprintf( buf, "%-15s  {c%10d       %s      %s    %s     %-20s{w\n\r",

					clan_table[cnt].name,

					clan_table[cnt].hall,

					(clan_table[cnt].independent ? "{rTrue " : "{gFalse"),

					(clan_table[cnt].pksafe ? "{rTrue " : "{gFalse"),

					(clan_table[cnt].visible ? "{rTrue " : "{gFalse"),

					clan_table[cnt].who_name );

				send_to_char( buf, ch );



				if ( line_count++ == (ch->lines - 2) )

				{

					scroll_wait( ch, "tabledump", "clan", cnt++, TRUE );

					return;

				}

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Clan Name         Clan Hall    Independent   PKSafe   Visible   Clan Flag            \n\r", ch);

		send_to_char( "------------------------------------------------------------------------------------ \n\r", ch);



		line_count = 2;

		for ( cnt = 0; clan_table[cnt].name != NULL; ++cnt)

		{

			if ( clan_table[cnt].active )

			{

				sprintf( buf, "%-15s  {c%10d       %s      %s    %s     %-20s{w\n\r",

					clan_table[cnt].name,

					clan_table[cnt].hall,

					(clan_table[cnt].independent ? "{rTrue " : "{gFalse"),

					(clan_table[cnt].pksafe ? "{rTrue " : "{gFalse"),

					(clan_table[cnt].visible ? "{rTrue " : "{gFalse"),

					clan_table[cnt].who_name );

				send_to_char( buf, ch );



				if ( line_count++ == (ch->lines - 2) )

				{

					scroll_wait( ch, "tabledump", "clan", cnt++, TRUE );

					return;

				}

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}



	send_to_char( "{w\n\r", ch );

	sprintf( buf, "{wCurrent elements in{c clan_table {w(without NULL):     {g%d{w\n\r", cnt++ );

	send_to_char( buf, ch );

	sprintf( buf, "{wMaximum elements in{c clan_table {w(including NULL):   {c%d{w\n\r", MAX_CLAN );

	send_to_char( buf, ch );

	return;

    }

    if (!str_prefix(arg,"combat") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; combatmod_table[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "{g%-25s   {w%-40s   {c%ld {w\n\r",

				combatmod_table[cnt].name,

				combatmod_table[cnt].description,

				combatmod_table[cnt].value );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "combat", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Global Combat Variable      Description                                Value     \n\r", ch);

		send_to_char( "-------------------------------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = 0; combatmod_table[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "{g%-25s   {w%-40s   {c%ld {w\n\r",

				combatmod_table[cnt].name,

				combatmod_table[cnt].description,

				combatmod_table[cnt].value );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "combat", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}



	send_to_char( "{w\n\r", ch );

	sprintf( buf, "{wCurrent elements in{c combatmod_table {w(without NULL):     {g%d{w\n\r", cnt++ );

	send_to_char( buf, ch );

	sprintf( buf, "{wMaximum elements in{c combatmod_table {w(including NULL):   {c%d{w\n\r", MAX_COMBAT );

	send_to_char( buf, ch );

	return;

    }

    if (!str_prefix(arg,"economy") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; economy_table[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "{g%-25s   {w%-40s   {c%ld {w\n\r",

				economy_table[cnt].name,

				economy_table[cnt].description,

				economy_table[cnt].value );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "economy", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Global Variable             Description                                Value     \n\r", ch);

		send_to_char( "-------------------------------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = 0; economy_table[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "{g%-25s   {w%-40s   {c%ld {w\n\r",

				economy_table[cnt].name,

				economy_table[cnt].description,

				economy_table[cnt].value );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "economy", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}



	send_to_char( "{w\n\r", ch );

	sprintf( buf, "{wCurrent elements in{c economy_table {w(without NULL):     {g%d{w\n\r", cnt++ );

	send_to_char( buf, ch );

	sprintf( buf, "{wMaximum elements in{c economy_table {w(including NULL):   {c%d{w\n\r", MAX_ECON );

	send_to_char( buf, ch );

	return;

    }



    if (!str_prefix(arg,"insult") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; insult_table[cnt].insult != NULL; ++cnt)

		{

			sprintf( buf, "{g%d     {w%s{w\n\r", cnt, insult_table[cnt].insult );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "insult", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char("Mob Insults:\n\r\n\r", ch );

		line_count = 2;

		for ( cnt = 0; insult_table[cnt].insult != NULL; ++cnt)

		{

			sprintf( buf, "{g%-3d {w%s{w\n\r", cnt, insult_table[cnt].insult );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "insult", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}



	send_to_char( "{w\n\r", ch );

	sprintf( buf, "{wCurrent elements in{c insult_table {w(without NULL):     {g%d{w\n\r", cnt++ );

	send_to_char( buf, ch );

	sprintf( buf, "{wMaximum elements in{c insult_table {w(including NULL):   {c%d{w\n\r", MAX_INSULTS );

	send_to_char( buf, ch );

	return;

    }



    if (!str_prefix(arg,"olc") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; olcs_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				olcs_flags[cnt].name,

				olcs_flags[cnt].bit,

				(olcs_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "olc", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; olcs_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				olcs_flags[cnt].name,

				olcs_flags[cnt].bit,

				(olcs_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "olc", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"her") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; icf_her_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_her_flags[cnt].name,

				icf_her_flags[cnt].bit,

				(icf_her_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "her", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; icf_her_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_her_flags[cnt].name,

				icf_her_flags[cnt].bit,

				(icf_her_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "her", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"ang") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; icf_ang_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_ang_flags[cnt].name,

				icf_ang_flags[cnt].bit,

				(icf_ang_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "ang", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; icf_ang_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_ang_flags[cnt].name,

				icf_ang_flags[cnt].bit,

				(icf_ang_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "ang", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"stat") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; icf_stat_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_stat_flags[cnt].name,

				icf_stat_flags[cnt].bit,

				(icf_stat_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "stat", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; icf_stat_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_stat_flags[cnt].name,

				icf_stat_flags[cnt].bit,

				(icf_stat_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "stat", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"sup") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; icf_sup_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_sup_flags[cnt].name,

				icf_sup_flags[cnt].bit,

				(icf_sup_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "sup", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; icf_sup_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				icf_sup_flags[cnt].name,

				icf_sup_flags[cnt].bit,

				(icf_sup_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "sup", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



        if (!str_prefix(arg,"wear") && IS_IMMORTAL(ch))

		    {

			            if ( ch->desc->iswait )

					            {

							                    line_count = 0;

									                    for ( cnt = ch->desc->swcount; wear_flags[cnt].name != NULL; ++cnt)

												                    {

															                            sprintf( buf, "%-20s   %-20d   %s {w\n\r",

																				                                    wear_flags[cnt].name,

																								                                    wear_flags[cnt].bit,

																												                                    (wear_flags[cnt].settable ? "{gTrue" : "{rFalse") );

																		                            send_to_char( buf, ch );



																					                            if ( line_count++ == (ch->lines - 2) )

																									                            {

																													                                    scroll_wait( ch, "tabledump", "wear", cnt++, TRUE );

																																	                                    return;

																																					                            }

																								                    }

											                    send_to_char( "\n\r", ch);

													                    scroll_wait( ch, "", "", 0, FALSE );

															            }

				            else

						            {

								                    send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

										                    send_to_char( "--------------------------------------------------------- \n\r", ch);



												                    line_count = 2;

														                    for ( cnt = ch->desc->swcount; wear_flags[cnt].name != NULL; ++cnt)

																	                    {

																				                            sprintf( buf, "%-20s   %-20d   %s {w\n\r",

																									                                    wear_flags[cnt].name,

																													                                    wear_flags[cnt].bit,

																																	                                    (wear_flags[cnt].settable ? "{gTrue" : "{rFalse") );

																							                            send_to_char( buf, ch );



																										                            if ( line_count++ == (ch->lines - 2) )

																														                            {

																																		                                    scroll_wait( ch, "tabledump", "wear", cnt++, TRUE );

																																						                                    return;

																																										                            }

																													                    }

																                    send_to_char( "\n\r", ch);

																		                    scroll_wait( ch, "", "", 0, FALSE );

																				            }

					            return;

						        }



    if (!str_prefix(arg,"comm") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; comm_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				comm_flags[cnt].name,

				comm_flags[cnt].bit,

				(comm_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "comm", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; comm_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				comm_flags[cnt].name,

				comm_flags[cnt].bit,

				(comm_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "comm", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"info") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; info_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				info_flags[cnt].name,

				info_flags[cnt].bit,

				(info_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "info", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; info_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				info_flags[cnt].name,

				info_flags[cnt].bit,

				(info_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "info", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"act") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; act_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				act_flags[cnt].name,

				act_flags[cnt].bit,

				(act_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "act", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; act_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				act_flags[cnt].name,

				act_flags[cnt].bit,

				(act_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "act", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



        if (!str_prefix(arg,"cyber") && IS_IMMORTAL(ch))

		    {

			            if ( ch->desc->iswait )

					            {

							                    line_count = 0;

									                    for ( cnt = ch->desc->swcount; cyber_flags[cnt].name != NULL; ++cnt)

												                    {

															                            sprintf( buf, "%-20s   %-20d   %s {w\n\r",

																				                                    cyber_flags[cnt].name,

																								                                    cyber_flags[cnt].bit,

																												                                    (cyber_flags[cnt].settable ? "{gTrue" : "{rFalse") );

																		                            send_to_char( buf, ch );



																					                            if ( line_count++ == (ch->lines - 2) )

																									                            {

																													                                    scroll_wait( ch, "tabledump", "cyber", cnt++, TRUE );

																																	                                    return;

																																					                            }

																								                    }

											                    send_to_char( "\n\r", ch);

													                    scroll_wait( ch, "", "", 0, FALSE );

															            }

				            else

						            {

								                    send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

										                    send_to_char( "--------------------------------------------------------- \n\r", ch);



												                    line_count = 2;

														                    for ( cnt = ch->desc->swcount; cyber_flags[cnt].name != NULL; ++cnt)

																	                    {

																				                            sprintf( buf, "%-20s   %-20d   %s {w\n\r",

																									                                    cyber_flags[cnt].name,

																													                                    cyber_flags[cnt].bit,

																																	                                    (cyber_flags[cnt].settable ? "{gTrue" : "{rFalse") );

																							                            send_to_char( buf, ch );



																										                            if ( line_count++ == (ch->lines - 2) )

																														                            {

																																		                                    scroll_wait( ch, "tabledump", "cyber", cnt++, TRUE );

																																						                                    return;

																																										                            }

																													                    }

																                    send_to_char( "\n\r", ch);

																		                    scroll_wait( ch, "", "", 0, FALSE );

																				            }

					            return;

						        }



    if (!str_prefix(arg,"plr") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; plr_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				plr_flags[cnt].name,

				plr_flags[cnt].bit,

				(plr_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "plr", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Name                   Value                  Can be Set? \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; plr_flags[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "%-20s   %-20d   %s {w\n\r",

				plr_flags[cnt].name,

				plr_flags[cnt].bit,

				(plr_flags[cnt].settable ? "{gTrue" : "{rFalse") );

			send_to_char( buf, ch );



			if ( line_count++ == (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "plr", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



    if (!str_prefix(arg,"race") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; race_table[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "{gName:{w                %s\n\r", race_table[cnt].name );

			send_to_char( buf, ch );

			sprintf( buf, "   {yPC Race:{w          %s\n\r", (race_table[cnt].pc_race ? "TRUE" : "FALSE") );

			send_to_char( buf, ch );

			sprintf( buf, "   {yAct:{w              %s\n\r", act_bit_name( race_table[cnt].act ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yAffects:{w          %s\n\r", affect_bit_name ( race_table[cnt].aff ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yOffence:{w          %s\n\r", off_bit_name( race_table[cnt].off ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yImmunities:{w       %s\n\r", imm_bit_name( race_table[cnt].imm ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yResistances:{w      %s\n\r", imm_bit_name( race_table[cnt].res ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yVulnerabilities:{w  %s\n\r", imm_bit_name( race_table[cnt].vuln ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yForm:{w             %s\n\r", form_bit_name( race_table[cnt].form ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yParts:{w            %s\n\r", part_bit_name( race_table[cnt].parts ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yRemort Race:{w      %s\n\r\n\r", (race_table[cnt].remort_race ? "TRUE" : "FALSE") );

			send_to_char( buf, ch );



			if ( (line_count += 12) >= (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "race", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "Race Table \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; race_table[cnt].name != NULL; ++cnt)

		{

			sprintf( buf, "{gName:{w                %s\n\r", race_table[cnt].name );

			send_to_char( buf, ch );

			sprintf( buf, "   {yPC Race:{w          %s\n\r", (race_table[cnt].pc_race ? "TRUE" : "FALSE") );

			send_to_char( buf, ch );

			sprintf( buf, "   {yAct:{w              %s\n\r", act_bit_name( race_table[cnt].act ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yAffects:{w          %s\n\r", affect_bit_name ( race_table[cnt].aff ) );

			send_to_char( buf, ch );
			/*
			sprintf( buf, "   {yOther Affects:{w          %s\n\r", affect2_bit_name ( race_table[cnt].aff ) );

			send_to_char( buf, ch ); */

			sprintf( buf, "   {yOffence:{w          %s\n\r", off_bit_name( race_table[cnt].off ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yImmunities:{w       %s\n\r", imm_bit_name( race_table[cnt].imm ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yResistances:{w      %s\n\r", imm_bit_name( race_table[cnt].res ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yVulnerabilities:{w  %s\n\r", imm_bit_name( race_table[cnt].vuln ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yForm:{w             %s\n\r", form_bit_name( race_table[cnt].form ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yParts:{w            %s\n\r", part_bit_name( race_table[cnt].parts ) );

			send_to_char( buf, ch );

			sprintf( buf, "   {yRemort Race:{w      %s\n\r\n\r", (race_table[cnt].remort_race ? "TRUE" : "FALSE") );

			send_to_char( buf, ch );



			if ( (line_count += 12) >= (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "race", cnt++, TRUE );

				return;

			}

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }



/*

 * DEBUG

 *

    if (!str_prefix(arg,"pcrace") && IS_IMMORTAL(ch))

    {

	if ( ch->desc->iswait )

	{

		line_count = 0;

		for ( cnt = ch->desc->swcount; cnt <= MAX_PC_RACE ; ++cnt)

		{

		  if ( pc_race_table[cnt].name != NULL )

		  {

			sprintf( buf, "{gName:{w                 %s\n\r", pc_race_table[cnt].name );

			send_to_char( buf, ch );

			sprintf( buf, "   {yWho Name:{w          %s\n\r", pc_race_table[cnt].who_name );

			send_to_char( buf, ch );

			sprintf( buf, "   {yPoints:{w            %d\n\r", pc_race_table[cnt].points );

			send_to_char( buf, ch );

			send_to_char( "   {yClass Multiplyer:{w\n\r", ch );

			for ( iCnt = 0; iCnt <= MAX_CLASS; iCnt++ )

			{

				sprintf( buf, "      {c%s:{w %d\n\r",

					class_table[iCnt].who_name,

					pc_race_table[cnt].class_mult[iCnt] );

				send_to_char( buf, ch );

			}

			send_to_char( "   {ySkills:{w            \n\r", ch );

			for ( iCnt = 0; iCnt <= 10; iCnt++ )

			{

				send_to_char( pc_race_table[cnt].skills[iCnt], ch );

				send_to_char( " ", ch );

			}

			send_to_char( "   {yStats:{w\n\r", ch );

			sprintf( buf, "      {cSTR:{w %d\n\r", pc_race_table[cnt].stats[STAT_STR] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cINT:{w %d\n\r", pc_race_table[cnt].stats[STAT_INT] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cWIS:{w %d\n\r", pc_race_table[cnt].stats[STAT_WIS] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cDEX:{w %d\n\r", pc_race_table[cnt].stats[STAT_DEX] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cCON:{w %d\n\r", pc_race_table[cnt].stats[STAT_CON] );

			send_to_char( buf, ch );

			send_to_char( "   {yMax Stats:{w\n\r", ch );

			sprintf( buf, "      {cSTR:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_STR] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cINT:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_INT] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cWIS:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_WIS] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cDEX:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_DEX] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cCON:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_CON] );

			send_to_char( buf, ch );

			sprintf( buf, "   {ySize:{w              %s\n\r", size_table[pc_race_table[cnt].size].name );

			send_to_char( buf, ch );



			if ( (line_count += 16) >= (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "pcrace", cnt++, TRUE );

				return;

			}

		  }

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	else

	{

		send_to_char( "PC Race Table \n\r", ch);

		send_to_char( "--------------------------------------------------------- \n\r", ch);



		line_count = 2;

		for ( cnt = ch->desc->swcount; cnt <= MAX_PC_RACE ; ++cnt)

		{

		  if ( pc_race_table[cnt].name != NULL )

		  {

			sprintf( buf, "{gName:{w                 %s\n\r", pc_race_table[cnt].name );

			send_to_char( buf, ch );

			sprintf( buf, "   {yWho Name:{w          %s\n\r", pc_race_table[cnt].who_name );

			send_to_char( buf, ch );

			sprintf( buf, "   {yPoints:{w            %d\n\r", pc_race_table[cnt].points );

			send_to_char( buf, ch );

			send_to_char( "   {yClass Multiplyer:{w\n\r", ch );

			for ( iCnt = 0; iCnt <= MAX_CLASS; iCnt++ )

			{

				sprintf( buf, "      {c%s:{w %d\n\r",

					class_table[iCnt].who_name,

					pc_race_table[cnt].class_mult[iCnt] );

				send_to_char( buf, ch );

			}

			send_to_char( "   {ySkills:{w            \n\r", ch );

			for ( iCnt = 0; iCnt <= 10; iCnt++ )

			{

				send_to_char( pc_race_table[cnt].skills[iCnt], ch );

				send_to_char( " ", ch );

			}

			send_to_char( "   {yStats:{w\n\r", ch );

			sprintf( buf, "      {cSTR:{w %d\n\r", pc_race_table[cnt].stats[STAT_STR] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cINT:{w %d\n\r", pc_race_table[cnt].stats[STAT_INT] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cWIS:{w %d\n\r", pc_race_table[cnt].stats[STAT_WIS] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cDEX:{w %d\n\r", pc_race_table[cnt].stats[STAT_DEX] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cCON:{w %d\n\r", pc_race_table[cnt].stats[STAT_CON] );

			send_to_char( buf, ch );

			send_to_char( "   {yMax Stats:{w\n\r", ch );

			sprintf( buf, "      {cSTR:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_STR] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cINT:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_INT] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cWIS:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_WIS] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cDEX:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_DEX] );

			send_to_char( buf, ch );

			sprintf( buf, "      {cCON:{w %d\n\r", pc_race_table[cnt].max_stats[STAT_CON] );

			send_to_char( buf, ch );

			sprintf( buf, "   {ySize:{w              %s\n\r", size_table[pc_race_table[cnt].size].name );

			send_to_char( buf, ch );



			if ( (line_count += 16) >= (ch->lines - 2) )

			{

				scroll_wait( ch, "tabledump", "pcrace", cnt++, TRUE );

				return;

			}

		  }

		}

		send_to_char( "\n\r", ch);

		scroll_wait( ch, "", "", 0, FALSE );

	}

	return;

    }

*

*/



    /* echo syntax */

    do_tabledump(ch,"");



    return;

}



void do_reanimate( CHAR_DATA *ch, char *argument )

{

	OBJ_DATA *corpse;

	CHAR_DATA *healer;

	CHAR_DATA *victim;

	CHAR_DATA *looker;

	char arg[MAX_INPUT_LENGTH];

	char buf[MAX_STRING_LENGTH];

	bool can_reanimate;

	bool found_pc_corpse;

	bool found_npc_corpse;

	int count;

	long cost;

	long cost_extra;

	long cost_min;

	long bank_multiplier;



	if ( IS_NPC(ch) )

		return;



	corpse = NULL;

	healer = NULL;

	victim = NULL;

	looker = NULL;

	can_reanimate = FALSE;

	found_pc_corpse = FALSE;

	found_npc_corpse = FALSE;

	count = 0;

	cost = 10000;

	cost_extra = 1000;

	cost_min = 1000;

	bank_multiplier = 10;



	argument = one_argument(argument,arg);



	if ( arg[0] == '\0' )

	{

		victim = ch;

	}

	else

	{

		if ( IS_HERO(ch) )

		{

			if ( ( victim = get_char_world( ch, arg ) ) == NULL )

			{

				send_to_char( "They are not here!\n\r", ch );

				return;

			}

		}

		else

		{

			if ( ( victim = get_char_room( ch, arg ) ) == NULL )

			{

				send_to_char( "They are not here!\n\r", ch );

				return;

			}

		}

	}



	/*
	 * Time to test if we can do this:
	 * + First off, the victim has to be dead and a player
	 * + If we are an Immortal, we can reanimate anyone we wish
	 * + If we are a Cleric, we need Mana based on the Global Economy Table,
	 *   we better be alive, and it has to be someone else
	 * + If not, we need cash and a healer.  If we're a Cleric at the Healer
	 *   doing ourselves, we pay cash if we don't have the mana.
	 */

	if ( IS_NPC( victim ) )
	{
		send_to_char( "You may only reanimate other players or yourself!\n\r", ch );
		return;
	}

	if ( victim->desc->connected != CON_PLAYING )
	{
		send_to_char( "They are currently link dead!\n\r", ch );
		return;
	}

	if ( !IS_SET(victim->info, INFO_GHOST) )
	{
		if ( victim == ch )
		{
			send_to_char( "You are not dead!\n\r", ch );
			return;
		}
		else
		{
			sprintf( buf, "%s is not dead!\n\r", victim->name );
			send_to_char( buf, ch );
			return;
		}
	}



	if ( !IS_IMMORTAL(ch) )

	{

		if ( (ch == victim) && (ch->position != POS_STANDING) )

		{

			send_to_char( "You must be standing to reanimate yourself.\n\r", ch );

			return;

		}



		if ( ch->position != POS_STANDING )

		{

			sprintf( buf, "You must be standing to reanimate %s{w.\n\r", PERS( victim, ch ) );

			send_to_char( buf, ch );

			return;

		}



		if ( victim->position != POS_STANDING )

		{

			sprintf( buf, "%s{w must be standing before you can reanimate %s.\n\r",

				PERS( victim, ch ),

				( victim->sex == SEX_MALE ? "him" : ( victim->sex == SEX_FEMALE ? "her" : "it" ) ) );

			send_to_char( buf, ch );

			sprintf( buf, "You must be standing before %s{w can reanimate you.\n\r", PERS( ch, victim ) );

			send_to_char( buf, victim );

			return;

		}

	}





	if ( IS_HERO(ch) )

	{

		can_reanimate = TRUE;

	}

	else if ( victim != ch )

	{

		if ( IS_SET( ch->info, INFO_GHOST ) )

		{

			send_to_char( "You are dead!  You must reanimate yourself first!\n\r", ch );

			return;

		}



		if ( ch->class != class_lookup( "cleric" ) )

		{

			send_to_char( "Only Clerics may reanimate another person.\n\r", ch );

			return;

		}



		if ( (corpse = get_obj_here( ch, "corpse" )) != NULL

			&& can_see_obj(ch,corpse) )

		{

			if ( corpse->item_type == ITEM_CORPSE_PC )

			{

				if ( can_loot( victim, corpse ) )

				{

					found_pc_corpse 	= TRUE;

					found_npc_corpse 	= FALSE;

				}

				else

				{

					found_pc_corpse 	= FALSE;

					found_npc_corpse 	= FALSE;

				}

			}

			else if ( corpse->item_type == ITEM_CORPSE_NPC )

			{

				found_pc_corpse = FALSE;

				found_npc_corpse = TRUE;

			}

			else

			{

				log_string( "BUG: Bad Corpse Object found in reanimate!" );

				found_pc_corpse = FALSE;

				found_npc_corpse = FALSE;

			}

		}

		else

		{

			found_pc_corpse = FALSE;

			found_npc_corpse = FALSE;

		}



		WAIT_STATE( victim, 6 * PULSE_VIOLENCE );

		WAIT_STATE( ch,     6 * PULSE_VIOLENCE );



/*

 * Commented out and replaced with new get_econ_value lookup

 *

		for ( count = 0; economy_table[count].name != NULL; ++count)

		{

			if ( found_pc_corpse && !str_prefix( economy_table[count].name ,"ghost_pccorpse_mana") )

					cost = economy_table[count].value;



			if ( found_npc_corpse && !str_prefix( economy_table[count].name ,"ghost_npccorpse_mana") )

					cost = economy_table[count].value;



			if ( !str_prefix( economy_table[count].name ,"ghost_min_mana") )

				cost_min = economy_table[count].value;



			if ( 	   !found_pc_corpse

				&& !found_npc_corpse

				&& !str_prefix( economy_table[count].name ,"ghost_nocorpse_mana") )

					cost = economy_table[count].value;

		}

*

* So here is our new routine

*/



		if ( found_pc_corpse )

			cost = get_econ_value( "ghost_pccorpse_mana" );



		if ( found_npc_corpse )

			cost = get_econ_value( "ghost_npccorpse_mana" );



		cost_min = get_econ_value( "ghost_min_mana" );



		if ( 	   !found_pc_corpse

			&& !found_npc_corpse )

			cost = get_econ_value( "ghost_nocorpse_mana" );





		if ( ch->level < victim->level )

			cost_extra = (victim->level - ch->level);

		else

			cost_extra = 0;



		cost_extra = number_range( (cost_extra*10), (cost_extra*20) );

		cost = cost + cost_extra;

		cost = number_range( (cost*7/9), (cost*14/9) );



		if ( cost < cost_min )

			cost = cost_min;



		if ( ch->mana >= cost )

		{

			can_reanimate = TRUE;

			ch->mana -= cost;

			sprintf( buf, "You feel your energies drained by {c%ld{w mana as the reanimation begins...\n\r", cost);

			send_to_char( buf, ch );

			sprintf( buf, "Magical energies begin to swirl around %s as reanimation begins...\n\r",

				PERS( ch, victim ) );

			send_to_char( buf, victim );

		}

		else

		{

			can_reanimate = FALSE;

			sprintf( buf, "Your magical energies are depleated, you need %ld mana to reanimate %s\n\r",

					cost,

					PERS( victim, ch ) );

			send_to_char( buf, ch );

			sprintf( buf, "%s does not have enough energy to reanimate you!\n\r", PERS( ch, victim ) );

			send_to_char( buf, victim );

		}



		WAIT_STATE( victim, 6 * PULSE_VIOLENCE );

		WAIT_STATE( ch,     6 * PULSE_VIOLENCE );

	}

	else

	{

		healer = NULL;

		for ( healer = ch->in_room->people; healer; healer = healer->next_in_room )

		{

			if ( IS_NPC( healer ) && IS_SET(healer->pIndexData->act, ACT_IS_HEALER) )

				break;

		}



		if ( !healer )

		{

			sprintf( buf, "You need a healer to reanimate you!\n\r" );

			send_to_char( buf, ch );

			return;

		}



		if ( (corpse = get_obj_here( ch, "corpse" )) != NULL

			&& can_see_obj(ch,corpse) )

		{

			if ( corpse->item_type == ITEM_CORPSE_PC )

			{

				if ( can_loot( victim, corpse ) )

				{

					found_pc_corpse = TRUE;

					found_npc_corpse = FALSE;

				}

				else

				{

					found_pc_corpse = FALSE;

					found_npc_corpse = FALSE;

				}

			}

			else if ( corpse->item_type == ITEM_CORPSE_NPC )

			{

				found_pc_corpse = FALSE;

				found_npc_corpse = TRUE;

			}

			else

			{

				log_string( "BUG: Bad Corpse Object found in reanimate!" );

				found_pc_corpse = FALSE;

				found_npc_corpse = FALSE;

			}

		}

		else

		{

			found_pc_corpse = FALSE;

			found_npc_corpse = FALSE;

		}



		WAIT_STATE( victim, 6 * PULSE_VIOLENCE );



/*

 * Same as above for Mana, now for Cash

 *

		for ( count = 0; economy_table[count].name != NULL; ++count)

		{

			if ( found_pc_corpse && !str_prefix( economy_table[count].name ,"ghost_pccorpse_cash") )

					cost = economy_table[count].value;



			if ( found_npc_corpse && !str_prefix( economy_table[count].name ,"ghost_npccorpse_cash") )

					cost = economy_table[count].value;



			if ( 	   !found_pc_corpse

				&& !found_npc_corpse

				&& !str_prefix( economy_table[count].name ,"ghost_nocorpse_cash") )

					cost = economy_table[count].value;



			if ( !str_prefix( economy_table[count].name ,"ghost_healerrate_cash") )

				bank_multiplier = economy_table[count].value;



			if ( !str_prefix( economy_table[count].name ,"ghost_min_cash") )

				cost_min = economy_table[count].value;

		}

*/



		if ( found_pc_corpse )

			cost = get_econ_value( "ghost_pccorpse_cash" );



		if ( found_npc_corpse )

			cost = get_econ_value( "ghost_npccorpse_cash" );



		if ( 	   !found_pc_corpse

			&& !found_npc_corpse )

			cost = get_econ_value( "ghost_nocorpse_cash" );



		cost_min = get_econ_value( "ghost_min_cash" );

		bank_multiplier = get_econ_value( "ghost_healerrate_cash" );







		cost_extra = cost * bank_multiplier;

		cost = cost + cost_extra;

		cost = number_range( (cost*7/9), (cost*14/9) );



		if ( cost < cost_min )

			cost = cost_min;



		if ( ch->bank >= cost )

		{

			can_reanimate = TRUE;

			ch->bank -= cost;

			sprintf( buf,  "%s{w takes his fee of {y%ld{w gold from your bank account...\n\r",

					healer->short_descr, cost );

			send_to_char( buf, ch );

		}

		else

		{

			can_reanimate = FALSE;

			sprintf( buf, "Your bank account is depleated, you need %ld gold to be reanimated.\n\r", cost );

			send_to_char( buf, ch );

		}



		WAIT_STATE( victim, 6 * PULSE_VIOLENCE );

	}



	if ( can_reanimate )

	{

		if ( IS_HERO(ch) )

		{

			if ( !IS_IMMORTAL(victim) )

			{

				send_to_char( "You are surrounded by a holy light and fade from existance.\n\r", victim );



				for ( looker = victim->in_room->people; looker; looker = looker->next_in_room )

				{

					if ( IS_AWAKE( looker ) && looker != victim )

					{

						sprintf( buf, "{w%s{w is surrounded by a holy light and then fades from existance.{w\n\r",

							PERS( victim, looker ) );

						send_to_char( buf, looker );

					}

				}



				if ( ch == victim )

				{

					sprintf( buf, "You leave %s to ", victim->in_room->name );

					send_to_char( buf, victim );

					char_from_room( victim );

					char_to_room( victim, get_room_index( ROOM_VNUM_HEROES ) );

					sprintf( buf, "return to %s and be reanimated.\n\r", victim->in_room->name );

					send_to_char( buf, victim );

				}

				else

				{

					if ( is_clan(victim) )

					{

						sprintf( buf, "The power of %s draws you from %s to ",

							PERS( ch, victim ), victim->in_room->name );

						send_to_char( buf, victim );

						char_from_room( victim );

						char_to_room( victim, get_room_index( clan_table[victim->clan].hall ) );

						sprintf( buf, "return to %s and be reanimated.\n\r", victim->in_room->name );

						send_to_char( buf, victim );

					}

					else

					{

						sprintf( buf, "The power of %s draws you from %s to ",

							PERS( ch, victim ), victim->in_room->name );

						send_to_char( buf, victim );

						char_from_room( victim );

						char_to_room( victim, get_room_index( ROOM_VNUM_ALTAR ) );

						sprintf( buf, "return to %s and be reanimated.\n\r", victim->in_room->name );

						send_to_char( buf, victim );

					}

				}



				for ( looker = victim->in_room->people; looker; looker = looker->next_in_room )

				{

					if ( IS_AWAKE( looker ) && looker != victim )

					{

						sprintf( buf, "{w%s{w fades into view, surrounded by a holy light.{w\n\r",

							PERS( victim, looker ) );

						send_to_char( buf, looker );

					}

				}

			}



			if ( ch == victim )

			{

				send_to_char( "You forge yourself a new body.\n\r", ch );

			}

			else

			{

				sprintf( buf, "{y%s{y begins forging you a new body.{w\n\r", PERS(ch, victim) );

				send_to_char( buf, victim );

				sprintf( buf, "{yYou begin forging %s{y a new body.{w\n\r", PERS(victim, ch) );

				send_to_char( buf, ch );

			}

		}

		else

		{

			if ( found_pc_corpse || found_npc_corpse )

			{

				if ( corpse == NULL )

				{

					sprintf( buf, "ERROR: Bad Corpse while trying to reanimate %s", ch->name );

					log_string( buf );

				}

				else

				{

					if ( ch == victim )

					{

						sprintf( buf, "{yThe powerful forces wielded by %s{y begin to remake %s{y into your new body.{w\n\r",

							healer->short_descr, corpse->short_descr );

						send_to_char( buf, ch );

						WAIT_STATE( ch,     12 * PULSE_VIOLENCE );

					}

					else

					{

						sprintf( buf, "{yThe powerful forces wielded by %s{y begin to remake %s{y into your new body.{w\n\r",

							PERS( ch, victim ), corpse->short_descr );

						send_to_char( buf, victim );

						sprintf( buf, "{yThe powerful forces wielded by you begin to remake %s{y into %s{y's new body.{w\n\r",

							corpse->short_descr, PERS( victim, ch ) );

						send_to_char( buf, ch );

						WAIT_STATE( victim, 6 * PULSE_VIOLENCE );

						WAIT_STATE( ch,     6 * PULSE_VIOLENCE );

					}



					for ( looker = victim->in_room->people; looker; looker = looker->next_in_room )

					{

						if ( IS_AWAKE( looker ) )

						{

							sprintf( buf, "%s glows with a holy light and then vanishes!{w\n\r",

								corpse->short_descr );

							send_to_char( buf, looker );

						}

					}

					extract_obj( corpse );

				}

			}

			else

			{

				if ( ch == victim )

				{

					sprintf( buf, "{y%s{y begins forging you a new body.{w\n\r", healer->short_descr );

					send_to_char( buf, ch );

					WAIT_STATE( ch,     6 * PULSE_VIOLENCE );

				}

				else

				{

					sprintf( buf, "{y%s{y begins forging you a new body.{w\n\r", PERS(ch, victim) );

					send_to_char( buf, victim );

					sprintf( buf, "{yYou begin forging %s{y a new body.{w\n\r", PERS(victim, ch) );

					send_to_char( buf, ch );

					WAIT_STATE( victim, 6 * PULSE_VIOLENCE );

					WAIT_STATE( ch,     6 * PULSE_VIOLENCE );

				}

			}

		}



		WAIT_STATE( victim, 6 * PULSE_VIOLENCE );

		victim->pcdata->timer_ghost = 0;

		victim->pcdata->died_in_room = get_room_index( ROOM_VNUM_PRISON );

		REMOVE_BIT( victim->info, INFO_GHOST );

		save_char_obj( victim );



		for ( looker = victim->in_room->people; looker; looker = looker->next_in_room )

		{

			if ( IS_AWAKE( looker ) && looker != victim )

			{

				sprintf( buf, "{w%s{w glows with a holy light and is reanimated from the dead!{w\n\r",

					PERS( victim, looker ) );

				send_to_char( buf, looker );

			}

		}



		if ( victim != ch )

		{

			send_to_char( "{wReanimation was a success!\n\r", ch );

			sprintf( buf, "{w%s{w has reanimated you!\n\r", PERS( ch, victim ) );

			send_to_char( buf, victim );

		}

		send_to_char( "{wYou are now fully reanimated and back to normal!\n\r", victim );

	}

	else

	{

		if ( victim != ch )

		{

			send_to_char( "{wReanimation failed.\n\r", ch );

			sprintf( buf, "{w%s{w failed to reanimate you.\n\r", PERS( ch, victim ) );

			send_to_char( buf, victim );

		}

		send_to_char( "{wReanimation failed, you are still a Ghost.\n\r", victim );

	}



	if ( victim != ch )

	{

		if ( can_reanimate )

			sprintf( buf, "%s has reanimated %s", ch->name, victim->name );

		else

			sprintf( buf, "%s failed to reanimate %s", ch->name, victim->name );

	}

	else

	{

		if ( can_reanimate )

			sprintf( buf, "%s has reanimated %s",

				ch->name,

				( ch->sex == SEX_MALE ? "himself" : ( ch->sex == SEX_FEMALE ? "herself" : "itself" ) ) );

		else

			sprintf( buf, "%s failed to reanimate %s",

				ch->name,

				( ch->sex == SEX_MALE ? "himself" : ( ch->sex == SEX_FEMALE ? "herself" : "itself" ) ) );

	}

	wiznet( buf, NULL, NULL, WIZ_DEATHS, 0, get_trust(ch) );



	if ( IS_HERO(ch) && !IS_HERO(victim) )

	{

		sprintf( buf, "%s has reanimated a mortal, %s", ch->name, victim->name );

		wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

	}



	return;

}



void do_clanadmin( CHAR_DATA *ch, char *argument )

{

	DESCRIPTOR_DATA *d;

	char buf[MAX_STRING_LENGTH];

	char name[MAX_STRING_LENGTH];

	char arg[MAX_INPUT_LENGTH];

	char value[MAX_INPUT_LENGTH];

	char oldstr[MAX_STRING_LENGTH];

	int iClan;

	int count;

	int line_count;

	int oldhall;

	int newhall;

	bool found;



	if ( !( IS_SET(ch->olcs, OLC_CLANADMIN)

		  || ( ch->level == MAX_LEVEL && get_trust(ch) == MAX_LEVEL ) ) )

	{

		send_to_char( "You must be a Clan Administrator to use this command.\n\r", ch );

		return;

	}



	argument = one_argument( argument, name  );

	argument = one_argument( argument, arg   );



	if ( name[0] == '\0' )

	{

		send_to_char( "{rSyntax:{w\n\r", ch );

		send_to_char( "   {gclanadmin{w {R<{Wclan name{c|{Wall{c|{Wlist{R> <{Wvariable{R> <{Wvalue{R>{w \n\r", ch );

		send_to_char( "   {cclanadmin list{W                 -- Lists all clans in the table            \n\r", ch );

		send_to_char( "   {cclanadmin <clan> <var> <value>   -- Changes the value for a clan entry      \n\r", ch );

		send_to_char( "   {cclanadmin <all> <var> <value>    -- Allows changing active, visible,        \n\r", ch );

		send_to_char( "   {c                                    or the PK Safe Status on all clans    {w\n\r", ch );

		return;

	}





	/*

	 * List First

	 */

	if ( !str_cmp( name, "list" ) )

	{

		if ( ch->desc->iswait )

		{

			line_count = 0;

			for ( iClan = ch->desc->swcount; clan_table[iClan].name != NULL; ++iClan)

			{

				sprintf( buf, "%s%-15s{c%10d         %s       %s    %s    {c%-20s{w\n\r",

					(clan_table[iClan].active ? "{g" : "{r"),

					clan_table[iClan].name,

					clan_table[iClan].hall,

					(clan_table[iClan].independent ? "{gTrue " : "{rFalse"),

					(clan_table[iClan].pksafe ? "{gTrue " : "{rFalse"),

					(clan_table[iClan].visible ? "{gTrue " : "{rFalse"),

					clan_table[iClan].who_name );

				send_to_char( buf, ch );



				if ( line_count++ == (ch->lines - 2) )

				{

					scroll_wait( ch, "clanadmin", "list", iClan++, TRUE );

					return;

				}

			}

			send_to_char( "\n\r", ch);

			scroll_wait( ch, "", "", 0, FALSE );

		}

		else

		{

			send_to_char( "Clan Name         Clan Hall    Independent   PKSafe   Visible   Clan Flag            \n\r", ch);

			send_to_char( "------------------------------------------------------------------------------------ \n\r", ch);



			line_count = 2;

			for ( iClan = ch->desc->swcount; clan_table[iClan].name != NULL; ++iClan)

			{

				sprintf( buf, "%s%-15s{c%10d         %s       %s    %s    {c%-20s{w\n\r",

					(clan_table[iClan].active ? "{g" : "{r"),

					clan_table[iClan].name,

					clan_table[iClan].hall,

					(clan_table[iClan].independent ? "{gTrue " : "{rFalse"),

					(clan_table[iClan].pksafe ? "{gTrue " : "{rFalse"),

					(clan_table[iClan].visible ? "{gTrue " : "{rFalse"),

					clan_table[iClan].who_name );

				send_to_char( buf, ch );



				if ( line_count++ == (ch->lines - 2) )

				{

					scroll_wait( ch, "clanadmin", "list", iClan++, TRUE );

					return;

				}

			}

			send_to_char( "\n\r", ch);

			scroll_wait( ch, "", "", 0, FALSE );

		}

		send_to_char( "{rRed{w   Clan Names are {rINACTIVE{w\n\r", ch );

		send_to_char( "{gGreen{w Clan Names are {gACTIVE{w\n\r", ch );

		return;

	}





	/*

	 * Change All Clans

	 */

	if ( !str_cmp( name, "all" ) )

	{

		if ( arg[0] == '\0' )

		{

			send_to_char( "{cclanadmin all {R<{Wactive{c|{Wpksafe{R> <{Wvalue{R>{w\n\r", ch );

			return;

		}



		argument = one_argument( argument, value );



		if ( !str_cmp( arg, "active" ) )

		{

			if ( value[0] == '\0' )

			{

				send_to_char( "{cclanadmin all active {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

				return;

			}

			if ( !str_cmp( value, "true" ) )

			{

				for( iClan = 0; clan_table[iClan].name != NULL; iClan++ )

				{

					clan_table[iClan].active = TRUE;

				}

				send_to_char( "All clans now marked active.\n\r", ch );

				sprintf( buf, "{cCLANADMIN{w : %s marked all clans as active", ch->name );

				wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

				sprintf( buf, "CLANADMIN : %s marked all clans as active", ch->name );

				log_string( buf );

				return;

			}

			if ( !str_cmp( value, "false" ) )

			{

				for( iClan = 0; clan_table[iClan].name != NULL; iClan++ )

				{

					clan_table[iClan].active = FALSE;

				}



				for ( d = descriptor_list; d; d = d->next )

				{

					if ( !IS_NPC( d->character ) )

					{

						d->character->clan = 0;

						d->character->clanreq = 0;

						if ( d->connected == CON_PLAYING )

							send_to_char( "{RNOTICE: {WAll clans have now been deactivated!{w\n\r", d->character );

					}

				}

				sprintf( buf, "{cCLANADMIN{w : %s marked all clans as inactive", ch->name );

				wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

				sprintf( buf, "CLANADMIN : %s marked all clans as inactive", ch->name );

				log_string( buf );

				send_to_char( "All clans now marked inactive.\n\r", ch );

				return;

			}

			send_to_char( "{cclanadmin all active {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

			return;

		}



		if ( !str_cmp( arg, "pksafe" ) )

		{

			if ( value[0] == '\0' )

			{

				send_to_char( "{cclanadmin all pksafe {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

				return;

			}

			if ( !str_cmp( value, "true" ) )

			{

				for( iClan = 0; clan_table[iClan].name != NULL; iClan++ )

				{

					clan_table[iClan].pksafe = TRUE;

				}

				sprintf( buf, "{cCLANADMIN{w : %s marked all clans as pksafe", ch->name );

				wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

				sprintf( buf, "CLANADMIN : %s marked all clans as pksafe", ch->name );

				log_string( buf );

				send_to_char( "All clans now marked pksafe.\n\r", ch );

				return;

			}

			if ( !str_cmp( value, "false" ) )

			{

				for( iClan = 0; clan_table[iClan].name != NULL; iClan++ )

				{

					clan_table[iClan].pksafe = FALSE;

				}

				sprintf( buf, "{cCLANADMIN{w : %s marked all clans as pkillable", ch->name );

				wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

				sprintf( buf, "CLANADMIN : %s marked all clans as pkillable", ch->name );

				log_string( buf );

				send_to_char( "All clans are now PKillable.\n\r", ch );

				return;

			}

			send_to_char( "{cclanadmin all pksafe {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

			return;

		}



		send_to_char( "{cclanadmin all {R<{Wactive{c|{Wpksafe{R> <{Wvalue{R>{w\n\r", ch );

		return;

	}





	/*

	 * Change a clan

	 */

	found = FALSE;

	iClan = 0;

	for( count = 0 ; clan_table[count].name != NULL; count++ )

	{

		if ( !str_cmp( name, clan_table[count].name ) )

		{

			found = TRUE;

			iClan = count;

		}

	}



	if ( !found )

	{

		send_to_char( "Invalid clan name!\n\r", ch );

		return;

	}



	if ( arg[0] == '\0' )

	{

		send_to_char( "{cclanadmin {C<{cclan{C> {R<{Wname{c|{Wwho_name{c|{Whall{c|{Windependent{c|{Wactive{c|{Wpksafe{R>{c|{Wvisible{R>{w\n\r", ch );

		return;

	}





	/*

	 * The next two manipulate strings, so we gotta watch the pointers

	 */

	if ( !str_cmp( arg, "name" ) )

	{

		sprintf( oldstr, "%s", clan_table[iClan].name );	/* get old value */



		strcpy( value, argument );

		clan_table[iClan].name = str_dup( value );



		sprintf( buf, "Clan name changed from %s to %s.\n\r", oldstr, clan_table[iClan].name );

		send_to_char( buf, ch );



		sprintf( buf, "{cCLANADMIN{w : %s changed clan %s to %s", ch->name, oldstr, clan_table[iClan].name );

		wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );



		sprintf( buf, "CLANADMIN : %s changed clan %s to %s", ch->name, oldstr, clan_table[iClan].name );

		log_string( buf );



		return;

	}



	if ( !str_cmp( arg, "who_name" ) )

	{

		sprintf( oldstr, "%s", clan_table[iClan].who_name );	/* get old value */



		strcpy( value, argument );

		clan_table[iClan].who_name = str_dup( value );



		sprintf( buf, "Clan name changed from %s to %s.\n\r", oldstr, clan_table[iClan].who_name );

		send_to_char( buf, ch );



		sprintf( buf, "{cCLANADMIN{w : %s changed clan %s to %s", ch->name, oldstr, clan_table[iClan].who_name );

		wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );



		sprintf( buf, "CLANADMIN : %s changed clan %s to %s", ch->name, oldstr, clan_table[iClan].who_name );

		log_string( buf );



		return;

	}





	/*

	 * Now we can safely parse the value

	 */

	argument = one_argument( argument, value );



	if ( !str_cmp( arg, "hall" ) )

	{

		newhall = atoi( value );

		if ( newhall < 1 )

		{

			send_to_char( "{cclanadmin {C<{cclan{C>{c hall {R<{Wvnum{R>{w\n\r", ch );

			return;

		}



		oldhall = clan_table[iClan].hall;

		clan_table[iClan].hall = newhall;

		sprintf( buf, "Clan hall of %s changed from %d to %d.\n\r",

			clan_table[iClan].name, oldhall, clan_table[iClan].hall );

		send_to_char( buf, ch );

		sprintf( buf, "{cCLANADMIN{w : %s changed the hall of %s from %d to %d",

			ch->name, clan_table[iClan].name, oldhall, clan_table[iClan].hall );

		wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

		sprintf( buf, "CLANADMIN : %s changed the hall of %s from %d to %d",

			ch->name, clan_table[iClan].name, oldhall, clan_table[iClan].hall );

		log_string( buf );





		return;

	}



	if ( !str_cmp( arg, "independent" ) )

	{

		if ( value[0] == '\0' )

		{

			send_to_char( "{cclanadmin {C<{cclan{C>{c independent {R<{Wtrue{c|{Wfalst{R>{w\n\r", ch );

			return;

		}

		if ( !str_cmp( value, "true" ) )

		{

			clan_table[iClan].independent = TRUE;

			sprintf( buf, "%s changed to an independent clan\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s changed %s to an independent clan", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s changed %s to an independent clan", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		if ( !str_cmp( value, "false" ) )

		{

			clan_table[iClan].independent = FALSE;

			sprintf( buf, "%s has had their independent status removed\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s removed the independent status of %s", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s removed the independent status of %s", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		send_to_char( "{cclanadmin {C<{cclan{C>{c independent {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

		return;

	}



	if ( !str_cmp( arg, "active" ) )

	{

		if ( value[0] == '\0' )

		{

			send_to_char( "{cclanadmin {C<{cclan{C>{c active {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

			return;

		}

		if ( !str_cmp( value, "true" ) )

		{

			clan_table[iClan].active = TRUE;

			sprintf( buf, "%s is now active\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s marked %s active", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s marked %s active", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		if ( !str_cmp( value, "false" ) )

		{

			clan_table[iClan].active = FALSE;

			for ( d = descriptor_list; d; d = d->next )

			{

				if ( !IS_NPC( d->character ) )

				{

					if ( d->character->clan == iClan )

					{

						d->character->clan = 0;

						if ( d->connected == CON_PLAYING )

						{

							sprintf( buf, "{RNOTICE: {W%s{W has been deactivated!{w\n\r",

								clan_table[iClan].who_name );

							send_to_char( buf, d->character );

						}

					}



					if ( d->character->clanreq == iClan )

					{

						d->character->clanreq = 0;

						if ( d->connected == CON_PLAYING )

						{

							sprintf( buf, "{RNOTICE: {W%s{W has been deactivated!{w\n\r",

								clan_table[iClan].who_name );

							send_to_char( buf, d->character );

						}

					}

				}

			}

			sprintf( buf, "%s is now inactive\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s marked %s inactive", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s marked %s inactive", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		send_to_char( "{cclanadmin {C<{cclan{C>{c active {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

		return;

	}



	if ( !str_cmp( arg, "pksafe" ) )

	{

		if ( value[0] == '\0' )

		{

			send_to_char( "{cclanadmin {C<{cclan{C>{c pksafe {R<{Wtrue{c|{Wfalst{R>{w\n\r", ch );

			return;

		}

		if ( !str_cmp( value, "true" ) )

		{

			clan_table[iClan].pksafe = TRUE;

			sprintf( buf, "%s has been made pksafe\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s made %s pksafe", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s made %s pksafe", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		if ( !str_cmp( value, "false" ) )

		{

			clan_table[iClan].pksafe = FALSE;

			sprintf( buf, "%s has been made pkillable\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s made %s pkillable", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s made %s pkillable", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		send_to_char( "{cclanadmin {C<{cclan{C>{c pksafe {R<{Wtrue{c|{Wfalst{R>{w\n\r", ch );

		return;

	}



	if ( !str_cmp( arg, "visible" ) )

	{

		if ( value[0] == '\0' )

		{

			send_to_char( "{cclanadmin {C<{cclan{C>{c visible {R<{Wtrue{c|{Wfalst{R>{w\n\r", ch );

			return;

		}

		if ( !str_cmp( value, "true" ) )

		{

			clan_table[iClan].visible = TRUE;

			sprintf( buf, "%s changed to a visible clan\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s changed %s to a visible clan", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s changed %s to a visible clan", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		if ( !str_cmp( value, "false" ) )

		{

			clan_table[iClan].visible = FALSE;

			sprintf( buf, "%s changed to an invisible clan\n\r", clan_table[iClan].name );

			send_to_char( buf, ch );

			sprintf( buf, "{cCLANADMIN{w : %s changed %s to an invisible clan", ch->name, clan_table[iClan].name );

			wiznet( buf, NULL, NULL, WIZ_SECURE, 0, get_trust(ch) );

			sprintf( buf, "CLANADMIN : %s changed %s to an invisible clan", ch->name, clan_table[iClan].name );

			log_string( buf );

			return;

		}

		send_to_char( "{cclanadmin {C<{cclan{C>{c visible {R<{Wtrue{c|{Wfalse{R>{w\n\r", ch );

		return;

	}



	/* Display the command syntax */

	do_clanadmin( ch, "" );



	return;

}



void do_ipaddress( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA       *vch;

    DESCRIPTOR_DATA *d;

    char            buf  [ MAX_STRING_LENGTH ];

    char            buf2 [ MAX_STRING_LENGTH ];

    int             count;

    char *          st;

    char            s[100];

    char            idle[10];





    count       = 0;

    buf[0]      = '\0';

    buf2[0]     = '\0';



    strcat( buf2, "\n\r[ ID Connected State  Login@  Idle ] Player Name  IP Address\n\r" );

    strcat( buf2,

"{b--------------------------------------------------------------------------------------------\n\r");

    for ( d = descriptor_list; d; d = d->next )

    {

        if ( d->character != NULL && can_see( ch, d->character ) )

        {

           /* NB: You may need to edit the CON_ values */

           switch( d->connected )

           {

              case CON_PLAYING:              st = "{g    PLAYING    ";    break;

              case CON_GET_NAME:             st = "{c   Get Name    ";    break;

              case CON_GET_OLD_PASSWORD:     st = "{cGet Old Passwd ";    break;

              case CON_CONFIRM_NEW_NAME:     st = "{c Confirm Name  ";    break;

              case CON_GET_NEW_PASSWORD:     st = "{cGet New Passwd ";    break;

              case CON_CONFIRM_NEW_PASSWORD: st = "{cConfirm Passwd ";    break;

              case CON_GET_NEW_RACE:         st = "{c Get New Race  ";    break;

              case CON_GET_NEW_SEX:          st = "{c  Get New Sex  ";    break;

              case CON_GET_NEW_CLASS:        st = "{c Get New Class ";    break;

              case CON_GET_ALIGNMENT:   	   st = "{c Get New Align ";	  break;

              case CON_DEFAULT_CHOICE:	   st = "{c Choosing Cust ";	  break;

              case CON_GEN_GROUPS:		   st = "{c Customization ";	  break;

              case CON_PICK_WEAPON:		   st = "{cPicking Weapon ";	  break;

		  case CON_READ_IMOTD:		   st = "{c Reading IMOTD "; 	  break;

		  case CON_BREAK_CONNECT:	   st = "{r   LINKDEAD    ";	  break;

              case CON_READ_MOTD:            st = "{c  Reading MOTD ";    break;

		  case CON_LOGIN_WIZI:		   st = "{c  Wizi Login   ";	  break;

		  case CON_BEGIN_REMORT:	   st = "{r Begin Remort  ";	  break;

		  case CON_SCROLL_WAIT:		   st = "{g  Scroll Wait  ";	  break;

              default:                       st = "{m   !UNKNOWN!   ";    break;

           }

           count++;



           /* Format "login" value... */

           vch = d->original ? d->original : d->character;

           strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );



           if ( vch->timer > 0 )

              sprintf( idle, "%4d", vch->timer );

           else

              sprintf( idle, "    " );



           sprintf( buf, "{W[{w%3d  %16s {D%7s {y%s{W ]{c %-12s{w %-40.40s\n\r",

              d->descriptor,

              st,

              s,

              idle,

              ( d->original ) ? d->original->name

                              : ( d->character )  ? d->character->name : "{R({rNone!{R){w",

              d->ipaddy);



           strcat( buf2, buf );



        }

    }



    sprintf( buf, "\n\r%d user%s\n\r", count, count == 1 ? "" : "s" );

    strcat( buf2, buf );

    send_to_char( buf2, ch );

    return;

}



void do_auto_shutdown()

{

   FILE *fp;

   extern bool merc_down;

   extern bool betalock;

   DESCRIPTOR_DATA *d,*d_next;

   CHAR_DATA *vch;

   extern char last_command_who [MAX_STRING_LENGTH];

   extern char last_command_loctime [MAX_STRING_LENGTH];

   extern char last_command_what [MAX_STRING_LENGTH];

   char   buf[ MAX_STRING_LENGTH ];

   bool   aswrld;





   /* Down the mud */

   merc_down 	= TRUE;





   /* Initialize the Asave World Flag */

   aswrld		= FALSE;





   /* Send Wiznet and write to the logfile */

   wiznet("{rAUTOSHUTDOWN{w :: {bBegin Autoshutdown Procedure{w",NULL,NULL,WIZ_SECURE,0,0);

   log_string( "AUTOSHUTDOWN :: Begin Autoshutdown Procedure" );

   log_string( "GLOBAL VARIABLE : MERC_DOWN set to TRUE" );





   /* This is to write the last command to the file. */

   fclose(fpReserve);

   if((fp = fopen(LAST_COMMAND,"a")) == NULL)

   {

	log_string( "Error in do_auto_save opening last_command.txt" );

   }

   else

   {

	fprintf(fp,"\nMagrathea Last Command Recovery\n");

	if ( betalock )

	{

		fprintf(fp,"Magrathea was BETALOCKED\n");

	}

	fprintf(fp,"Who:              %s\n",last_command_who);

	fprintf(fp,"What:             %s\n",last_command_what);

	fprintf(fp,"Where and When:   %s\n",last_command_loctime);



   }

   fclose( fp );

   fpReserve = fopen( NULL_FILE, "r" );





   /* Tell Everyone we are going down and try to save the area files */

   for ( d = descriptor_list; d; d = d->next )

   {

	if ( d->connected == CON_PLAYING )

	{

	   vch = d->character;

	   send_to_char("\n\r\n\r{rCRITICAL ERROR: {cSystem Crash in progress        {w\n\r",     vch);

	   send_to_char("\n\r{gMagrathea{w is attempting a safe shutdown and restart {w\n\r",     vch);

	   send_to_char("\n\r{wAll systems should be operational in {y2{w minutes    {w\n\r\n\r", vch);



	   if ( IS_OLCADMIN(vch) && !aswrld && !betalock )

	   {

		sprintf( buf, "{rAUTOSHUTDOWN{w :: {bAutomatic ASAVE World{w :: {y%s{w", vch->name );

		wiznet(buf,NULL,NULL,WIZ_SECURE,0,0);

		sprintf( buf, "AUTOSHUTDOWN :: Automatic ASAVE World :: %s", vch->name );

		log_string( buf );

		do_asave( vch, "world" );

		aswrld = TRUE;

	   }

	}

   }





   /* Now we reboot the mud */

   for ( d = descriptor_list; d != NULL; d = d_next )

   {

	d_next = d->next;

	vch = d->original ? d->original : d->character;

	if (vch != NULL && !betalock)

	{

	    save_char_obj(vch);

	}

    	close_socket(d);

   }





   /* Write our final messages */

   sprintf( buf, "{rAUTOSHUTDOWN{w :: {bMagrathea Last Command Recovery {w:: {c%s {w:: {y%s {w:: {g%s{w",

	last_command_who, last_command_what, last_command_loctime );

   wiznet(buf,NULL,NULL,WIZ_SECURE,0,0);



   sprintf( buf, "AUTOSHUTDOWN :: Magrathea Last Command Recovery :: %s :: %s :: %s",

	last_command_who, last_command_what, last_command_loctime );

   log_string(buf);



   wiznet("{rAUTOSHUTDOWN{w :: {bEnd Autoshutdown Procedure{w",NULL,NULL,WIZ_SECURE,0,0);

   log_string( "AUTOSHUTDOWN :: End Autoshutdown Procedure" );



   return;

}





void do_pload( CHAR_DATA *ch, char *argument )

{

  DESCRIPTOR_DATA d;

  bool isChar = FALSE;

  char name[MAX_INPUT_LENGTH];

  char buf[MAX_STRING_LENGTH];

  CHAR_DATA *victim;





  if ( ch->level != MAX_LEVEL && get_trust(ch) != MAX_LEVEL )

  {

    send_to_char("Only Implementors may use pload!\n\r", ch);

    return;

  }



  if (argument[0] == '\0')

  {

    send_to_char("Who do you wish to pull from the pattern?\n\r", ch);

    return;

  }



  argument[0] = UPPER(argument[0]);

  argument = one_argument(argument, name);



  /* Dont want to load a second copy of a player who's allready online! */

  if ( (victim = get_char_world( ch, name )) != NULL  )

  {

    sprintf(buf, "{c%s{w is currently playing.\n\r", victim->name);

    send_to_char( buf, ch );

    return;

  }





  isChar = load_char_obj(&d, name); /* char pfile exists? */



  if (!isChar)

  {

    sprintf(buf, "{wUnable to find {c%s{w in the pattern.\n\r", name);

    send_to_char(buf, ch);

    return;

  }



  /* Set up the ploaded character */

  d.character->desc     	= NULL;

  d.character->next     	= char_list;

  char_list             	= d.character;

  d.connected           	= CON_BREAK_CONNECT;

  victim				= d.character;

  victim->pcdata->ploaded 	= TRUE;

  victim->pcdata->linkdead	= TRUE;

  reset_char(victim);



  /* bring player to imm */

  if ( victim->in_room != NULL )

  {

    char_to_room( victim, ch->in_room );

  }



  /* Tell the world */

  sprintf(buf, "You pull {c%s{w from the pattern.\n\r", victim->name);

  send_to_char(buf, ch);



  act( "$n has pulled $N from the pattern!",

        ch, NULL, victim, TO_ROOM );



  sprintf(buf, "%s has loaded %s from the pattern.", ch->name, victim->name );

  log_string( buf );

  sprintf(buf, "{y%s{w has loaded {c%s{w from the pattern.", ch->name, victim->name );

  wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));



  if (victim->pet != NULL)

  {

     char_to_room(victim->pet,victim->in_room);

     act("$n has entered the game.",victim->pet,NULL,NULL,TO_ROOM);

  }



  return;

}





void do_punload( CHAR_DATA *ch, char *argument )

{

  CHAR_DATA *victim;

  char who[MAX_INPUT_LENGTH];

  char buf[MAX_STRING_LENGTH];





  if ( ch->level != MAX_LEVEL && get_trust(ch) != MAX_LEVEL )

  {

    send_to_char("Only Implementors may use pUnload!\n\r", ch);

    return;

  }



  argument = one_argument(argument, who);



  if ( ( victim = get_char_world( ch, who ) ) == NULL )

  {

    send_to_char( "They aren't here.\n\r", ch );

    return;

  }



  /* Person is legitametly logged on... was not ploaded. */

  if (!is_ploaded(victim))

  {

    sprintf(buf, "%s is currently playing.\n\r", victim->name);

    send_to_char(buf, ch);

    return;

  }



  if (victim->was_in_room != NULL) /* return player and pet to orig room */

  {

    char_to_room(victim, victim->was_in_room);



    if (victim->pet != NULL)

    {

      char_to_room(victim->pet, victim->was_in_room);

    }

  }



  sprintf(buf, "You release {c%s{w back to the pattern.\n\r", victim->name);

  send_to_char(buf, ch);



  act("$n has released $N back to the Pattern.",

       ch, NULL, victim, TO_ROOM);



  sprintf(buf, "%s has released %s back into the pattern.", ch->name, victim->name );

  log_string( buf );

  sprintf(buf, "{y%s{w has released {c%s{w back into the pattern.", ch->name, victim->name );

  wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));



  save_char_obj(victim);

  extract_char(victim, TRUE );



  return;

}





void do_chkdupip( CHAR_DATA *ch, char *argument  )

{

	DESCRIPTOR_DATA *d, *d_search;

	char buf[MAX_STRING_LENGTH];

	char arg[MAX_STRING_LENGTH];

	int count;

	CHAR_DATA *dch, *dsch;

	extern bool dupipcheck;



	if (argument[0] != '\0')

	{



		argument = one_argument(argument, arg);



		if ( !str_cmp( arg, "off" ) || !str_cmp( arg, "disable" ) )

		{

			if ( ch->level < MAX_LEVEL && get_trust(ch) < MAX_LEVEL )

			{

				send_to_char( "Only an implementor may disable automatic duplicate IP checking.\n\r", ch);

				return;

			}



			dupipcheck		= FALSE;

			send_to_char( "Automatic duplicate IP Checking has been disabled.\n\r", ch );

			sprintf( buf, "{rDUPLICATE IP CHECK{w :: %s has disabled duplicate IP checking", ch->name );

			wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

			sprintf( buf, "DUPLICATE IP CHECK :: %s has disabled duplicate IP checking", ch->name );

			log_string( buf );

			return;

		}



		if ( !str_cmp( arg, "on" ) || !str_cmp( arg, "enable" ) )

		{

			if ( ch->level < MAX_LEVEL && get_trust(ch) < MAX_LEVEL )

			{

				send_to_char( "Only an implementor may enable automatic duplicate IP checking.\n\r", ch);

				return;

			}



			dupipcheck		= TRUE;

			send_to_char( "Automatic duplicate IP Checking has been enabled.\n\r", ch );

			sprintf( buf, "{rDUPLICATE IP CHECK{w :: %s has enabled duplicate IP checking", ch->name );

			wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

			sprintf( buf, "DUPLICATE IP CHECK :: %s has enabled duplicate IP checking", ch->name );

			log_string( buf );

			return;

		}



		send_to_char( "\n\r", ch );

		send_to_char( "{rSyntax{w: {cchkdupip {g<{Won{g|{Woff{g|{Wenable{g|{Wdisable{g>{w\n\r", ch );

		send_to_char( "{wToggles automatic duplicate IP address checking{w\n\r", ch );

		send_to_char( "No argument runs a duplicate IP check.\n\r", ch );

		return;

	}



	count 	= 0;

	send_to_char( "\n\r{wDuplicate IP Addresses\n\r\n\r", ch );



	for ( d = descriptor_list; d; d = d->next )

	{

	  if ( d->character != NULL )

	  {

		dch = d->original ? d->original : d->character;

		for ( d_search = descriptor_list; d_search; d_search = d_search->next )

		{

		  if ( d_search->character != NULL )

		  {

			dsch = d_search->original ? d_search->original : d_search->character;

			if ( !str_cmp(d->ipaddy, d_search->ipaddy) && ( dch != dsch ) )

			{

			  if ( can_see(ch, d->character) && can_see(ch, d_search->character) )

			  {

				count++;

				sprintf( buf, "{g%-15s   {w%-15s   {c<-->   {g%-15s   {w%-15s {w\n\r",

					dch->name, d->ipaddy, dsch->name, d_search->ipaddy );

				send_to_char(buf,ch);

			  }

			}

		  }

		}

	  }

	}

	sprintf(buf, "{w\n\rTotal Duplicate IP Addresses found:   {g%d{w\n\r", count);

	send_to_char(buf, ch);



	return;

}



void check_duplicate_ipaddy( bool logimm )

{

	DESCRIPTOR_DATA *d, *d_search;

	char buf[MAX_STRING_LENGTH];

	CHAR_DATA *dch, *dsch;

	int count;

	extern bool dupipcheck;



	if ( !dupipcheck )

	{

		log_string( "Duplicate IP Checking has been disabled" );

		return;

	}



	count 	= 0;

	sprintf(buf, "Starting duplicate IP check with Imm logging %s", (logimm ? "ON" : "OFF") );

	log_string( buf );



	for ( d = descriptor_list; d; d = d->next )

	{

	  if ( d->character != NULL )

	  {

		dch = d->original ? d->original : d->character;

		for ( d_search = descriptor_list; d_search; d_search = d_search->next )

		{

		  if ( d_search->character != NULL )

		  {

			dsch = d_search->original ? d_search->original : d_search->character;

			if ( !str_cmp(d->ipaddy, d_search->ipaddy) && ( dch != dsch ) )

			{



				if ( 	(IS_IMMORTAL(dch) || IS_IMMORTAL(dsch))

					|| (IS_SET(dch->info, INFO_IMMMULTI) || IS_SET(dsch->info, INFO_IMMMULTI)) )

				{

					if ( logimm )

					{

						count++;

						sprintf( buf, "Duplicate IP Address :: %s :: %s : %s",

							d->ipaddy, dch->name, dsch->name );

						log_string( buf );

					}

				}

				else

				{

					count++;

					sprintf( buf, "Duplicate IP Address :: %s :: %s : %s",

						d->ipaddy, dch->name, dsch->name );

					log_string( buf );



					sprintf( buf, "{rDuplicate IP Address {w:: {c%s {w:: {g%s {w: {g%s{w",

						d->ipaddy, dch->name, dsch->name );

					wiznet(buf,NULL,NULL,WIZ_SITES,0,SUPREME);

				}

			}

		  }

		}

	  }

	}

	sprintf(buf, "Duplicate IP Check finished.  Number of duplicates: %d", count );

	log_string ( buf );

	return;

}



void do_nuke( CHAR_DATA *ch, char *argument )
{
   char strsave[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   extern bool betalock;

   if ( ch->level != MAX_LEVEL && get_trust(ch) != MAX_LEVEL )
   {
     send_to_char("Only Implementors may use nuke!\n\r", ch);
     return;
   }

   if ( betalock )
   {
	send_to_char("You may not nuke pfiles while in Beta Test Mode.\n\r", ch);
	return;
   }

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Syntax:nuke  <character name> \n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not logged on.\n\r", ch );
        return;
    }

    if (IS_NPC(victim))
    {
      send_to_char( "This command only works on players.\n\r", ch);
      return;
    }

    if (IS_IMMORTAL(victim))
    {
      send_to_char( "This command may not be used on Immortals.\n\r", ch);
      return;
    }

    sprintf( buf, "\n\r{yThe Immortals{w have {RN{rU{RK{rE{RD{c %s{w from {gMagrathea{w...\n\r\n\r", victim->name);
    do_echo( ch, buf );

    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
    stop_fighting(victim,TRUE);
    sprintf( buf,"%s has been nuked by %s",victim->name,ch->name);
    log_string( buf );
    wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));

    if ( is_ploaded(victim) )
    {
	if ( is_linkdead(victim) )
	{
		extract_char(victim, TRUE );
	}
	else
	{
		sprintf( buf, "Unable to {rNUKE{c %s{w, they are PLOADED but not LINKDEAD.\n\r", victim->name );
		send_to_char( buf, ch );
		sprintf( buf, "{rNuke Error {w:: {c%s {w:: PLOADED but not LINKDEAD{w", victim->name );
		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
		sprintf( buf,  "Nuke Error :: %s :: PLOADED but not LINKDEAD", victim->name );
		bug ( buf, 0 );
		return;
	}
    }
    else
    {
	do_quit( victim, "" );
    }
    unlink(strsave);
    return;
}


void do_crash( CHAR_DATA *ch, char *argument )

{

	CHAR_DATA *vch;

	char buf[MAX_STRING_LENGTH];

	extern bool betalock;



	if ( !betalock )

	{

		send_to_char( "Not on the main port!\n\r", ch );

		return;

	}



	/* Command to Crash the mud

	 *

       * We know mobs DO NOT have DESCRIPTORS

       * So....

       * We reference something in DESC: mob->desc->ipaddy

	 * ...and we should go BOOM

       */



	send_to_char( "Fasten your seatbelt...\n\r", ch );



	if ( ( vch = get_char_world( ch, "fido" ) ) == NULL )

	{

		sprintf( buf, "Load a Fido (%d).\n\r" , MOB_VNUM_FIDO );

		send_to_char( buf, ch );

		return;

	}



	sprintf( buf, "IPAddress for FIDO: %s", vch->desc->ipaddy );

	send_to_char( buf, ch );



	return;

}



void do_autorestart( CHAR_DATA *ch, char *argument )

{

	extern	bool	autodown;

	extern	bool	autodown_reboot;

	extern 	int	autodown_timer;





	char	buf[MAX_STRING_LENGTH];

	char	arg[MAX_INPUT_LENGTH];

	char	varg[MAX_INPUT_LENGTH];

	int	value;





	if ( IS_NPC(ch) )

		return;



	if ( is_linkdead(ch) || is_ploaded(ch) )

		return;



	if ( !IS_ADMIN(ch) )

	{

		send_to_char( "Only Administrators may initiate an Auto shutdown or reboot!\n\r", ch );

		return;

	}



	argument = one_argument(argument,arg);

	argument = one_argument(argument,varg);

	value = atoi( varg );



	if ( arg[0] == '\0' || varg[0] == '\0' )

	{

		send_to_char( "{RSyntax{w: {cautorestart {g<{wshutdown{g|{wreboot{g> <{wtime (1-30){g>{w\n\r", ch );

		return;

	}



	if ( (value < 1) || (value > 30) )

	{

		send_to_char( "The timer must be between 1 and 30 minutes.\n\r", ch );

		send_to_char( "To restart or shutdown immediately, use {creboot{w or {cshutdown{w commands.\n\r", ch );

		return;

	}



	if ( !str_cmp( arg, "shutdown" ) )

	{

		autodown			= TRUE;

		autodown_reboot		= FALSE;

		autodown_timer		= value;



		sprintf( buf, "%s has initiated an Auto Shutdown for %d minutes.", ch->name, value );

		wiznet(buf,NULL,NULL,0,0,0);

		log_string( buf );



		log_string( "GLOBAL VARIABLE : AUTODOWN set to TRUE" );

		log_string( "GLOBAL VARIABLE : AUTODOWN_REBOOT set to FALSE" );

		sprintf( buf, "GLOBAL VARIABLE : AUTODOWN_TIMER set to %d", value );

		log_string( buf );



		sprintf( buf, "\n\r\n\r{RNOTICE{W: {cMagrathea{w will automatically {rSHUTDOWN{w in {g%d{w minutes.\n\r\n\r", value );

		do_echo( ch, buf );



		send_to_char( "Ok.", ch );



		return;

	}



	if ( !str_cmp( arg, "reboot" ) )

	{

		autodown			= TRUE;

		autodown_reboot		= TRUE;

		autodown_timer		= value;



		sprintf( buf, "%s has initiated an Auto Reboot for %d minutes.", ch->name, value );

		wiznet(buf,NULL,NULL,0,0,0);

		log_string( buf );



		log_string( "GLOBAL VARIABLE : AUTODOWN set to TRUE" );

		log_string( "GLOBAL VARIABLE : AUTODOWN_REBOOT set to TRUE" );

		sprintf( buf, "GLOBAL VARIABLE : AUTODOWN_TIMER set to %d", value );

		log_string( buf );



		sprintf( buf, "\n\r\n\r{RNOTICE{W: {cMagrathea{w will automatically {rREBOOT{w in {g%d{w minutes.\n\r\n\r", value );

		do_echo( ch, buf );



		send_to_char( "Ok.", ch );



		return;

	}



	do_autorestart( ch, "" );

	return;

}









void do_slist ( CHAR_DATA *ch, char *argument )

{

	CHAR_DATA *victim;

	int sn;

	char buf[MAX_STRING_LENGTH];

	int line_count;

	int start_sn;





	if ( IS_NPC(ch) )

		return;



	if ( is_linkdead(ch) || is_ploaded(ch) )

		return;



	if ( argument[0] == '\0')

	{

		send_to_char("{RSyntax{w: {cslist {g<{wtarget{g>{w\n\r",ch);

		return;

	}



	if ( (victim = get_char_world(ch,argument)) == NULL )

	{

		send_to_char( "They are not here.\n\r", ch );

	}



	if ( IS_NPC(victim) )

	{

		send_to_char( "Mobiles do not have skills to list.\n\r", ch );

		return;

	}



	if ( get_trust(ch) < get_trust(victim) )

	{

		send_to_char( "I don't think so.\n\r", ch );

		return;

	}



	if ( ch->desc->iswait )

	{

		start_sn 	= ch->desc->swcount;

		line_count	= 0;

	}

	else

	{

		sprintf( buf, "{wSkill List for: %s%s{w\n\r",

			victim->name,

			IS_SET(victim->info, INFO_IMMMULTI) ? "{R[{rMULTI{R]{w" : "" );

		send_to_char( buf, ch );



		line_count 	= 4;

		start_sn	= 0;



		send_to_char( "\n\r{wSkill               Learned   Value   Racial\n\r", ch );

		send_to_char( "--------------------------------------------{w\n\r", ch );

	}



	for ( sn = start_sn; sn < MAX_SKILL; sn++ )

	{

		if (	(skill_table[sn].name != NULL) && can_use_skill(victim,sn) )

		{

			sprintf( buf, "{c%-20s   {g%-4d    {g%-4d     %s{w\n\r",

				skill_table[sn].name,

				victim->pcdata->learned[sn],

				get_skill(victim,sn),

				(is_race_skill(victim,sn) ? "{rYes" : "{wNo") );

			send_to_char( buf, ch );



			if ( line_count++ >= (ch->lines - 2) )

			{

				scroll_wait( ch, "slist", victim->name, sn++, TRUE );

				return;

			}

		}

	}



	scroll_wait( ch, "", "", 0, FALSE );

	return;

}



void do_arealinks(CHAR_DATA *ch, char *argument)

{

    FILE *fp;

    BUFFER *buffer;

    AREA_DATA *parea;

    EXIT_DATA *pexit;

    ROOM_INDEX_DATA *to_room;

    ROOM_INDEX_DATA *from_room;

    char buf[MAX_STRING_LENGTH];

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    int vnum = 0;

    int iHash, door;

    bool found = FALSE;



    /* To provide a convenient way to translate door numbers to words */

    static char * const dir_name[] =

{"north","east","south","west","up","down"};



    argument = one_argument(argument, arg1);

    argument = one_argument(argument, arg2);



    /* First, the 'all' option */

    if (!str_cmp(arg1,"all"))

    {

	/*

	 * If a filename was provided, try to open it for writing

	 * If that fails, just spit output to the screen.

	 */

	if (arg2[0] != '\0')

	{

	    fclose(fpReserve);

	    if( (fp = fopen(arg2, "w")) == NULL)

	    {

		send_to_char("Error opening file, printing to screen.\n\r",ch);

		fclose(fp);

		fpReserve = fopen(NULL_FILE, "r");

		fp = NULL;

	    }

	}

	else

	    fp = NULL;



	/* Open a buffer if it's to be output to the screen */

	if (!fp)

	    buffer = new_buf();



	/* Loop through all the areas */

	for (parea = area_first; parea != NULL; parea = parea->next)

	{

	    /* First things, add area name  and vnums to the buffer */

	    sprintf(buf, "*** %s (%d to %d) ***\n\r",

			 parea->name, parea->min_vnum, parea->max_vnum);

	    fp ? fprintf(fp, buf) : add_buf(buffer, buf);



	    /* Now let's start looping through all the rooms. */

	    found = FALSE;

	    for(iHash = 0; iHash < MAX_KEY_HASH; iHash++)

	    {

		for( from_room = room_index_hash[iHash];

		     from_room != NULL;

		     from_room = from_room->next )

		{

		    /*

		     * If the room isn't in the current area,

		     * then skip it, not interested.

		     */

		    if ( from_room->vnum < parea->min_vnum

		    ||   from_room->vnum > parea->max_vnum )

			continue;



		    /* Aha, room is in the area, lets check all directions */

		    for (door = 0; door < 5; door++)

		    {

			/* Does an exit exist in this direction? */

			if( (pexit = from_room->exit[door]) != NULL )

			{

			    to_room = pexit->u1.to_room;



			    /*

			     * If the exit links to a different area

			     * then add it to the buffer/file

			     */

			    if( to_room != NULL

			    &&  (to_room->vnum < parea->min_vnum

			    ||   to_room->vnum > parea->max_vnum) )

			    {

				found = TRUE;

				sprintf(buf, "    (%d) links %s to %s (%d)\n\r",

				    from_room->vnum, dir_name[door],

				    to_room->area->name, to_room->vnum);



				/* Add to either buffer or file */

				if(fp == NULL)

				    add_buf(buffer, buf);

				else

				    fprintf(fp, buf);

			    }

			}

		    }

		}

	    }



	    /* Informative message for areas with no external links */

	    if (!found)

		add_buf(buffer, "    No links to other areas found.\n\r");

	}



	/* Send the buffer to the player */

	if (!fp)

	{

	    page_to_char(buf_string(buffer), ch);

	    free_buf(buffer);

	}

	/* Or just clean up file stuff */

	else

	{

	    fclose(fp);

	    fpReserve = fopen(NULL_FILE, "r");

	}



	return;

    }



    /* No argument, let's grab the char's current area */

    if(arg1[0] == '\0')

    {

	parea = ch->in_room ? ch->in_room->area : NULL;



	/* In case something wierd is going on, bail */

	if (parea == NULL)

	{

	    send_to_char("You aren't in an area right now, funky.\n\r",ch);

	    return;

	}

    }

    /* Room vnum provided, so lets go find the area it belongs to */

    else if(is_number(arg1))

    {

	vnum = atoi(arg1);



	/* Hah! No funny vnums! I saw you trying to break it... */

	if (vnum <= 0 || vnum > MAX_VNUM)

	{

	    sprintf( buf, "The vnum must be between 1 and %d.\n\r",MAX_VNUM);

	    send_to_char(buf,ch);

	    return;

	}



	/* Search the areas for the appropriate vnum range */

	for (parea = area_first; parea != NULL; parea = parea->next)

	{

	    if(vnum >= parea->min_vnum && vnum <= parea->max_vnum)

		break;

	}



	/* Whoops, vnum not contained in any area */

	if (parea == NULL)

	{

	    send_to_char("There is no area containing that vnum.\n\r",ch);

	    return;

	}

    }

    /* Non-number argument, must be trying for an area name */

    else

    {

	/* Loop the areas, compare the name to argument */

	for(parea = area_first; parea != NULL; parea = parea->next)

	{

	    if(!str_prefix(arg1, parea->name))

		break;

	}



	/* Sorry chum, you picked a goofy name */

	if (parea == NULL)

	{

	    send_to_char("There is no such area.\n\r",ch);

	    return;

	}

    }



    /* Just like in all, trying to fix up the file if provided */

    if (arg2[0] != '\0')

    {

	fclose(fpReserve);

	if( (fp = fopen(arg2, "w")) == NULL)

	{

	    send_to_char("Error opening file, printing to screen.\n\r",ch);

	    fclose(fp);

	    fpReserve = fopen(NULL_FILE, "r");

	    fp = NULL;

	}

    }

    else

	fp = NULL;



    /* And we loop the rooms */

    for(iHash = 0; iHash < MAX_KEY_HASH; iHash++)

    {

	for( from_room = room_index_hash[iHash];

	     from_room != NULL;

	     from_room = from_room->next )

	{

	    /* Gotta make sure the room belongs to the desired area */

	    if ( from_room->vnum < parea->min_vnum

	    ||   from_room->vnum > parea->max_vnum )

		continue;



	    /* Room's good, let's check all the directions for exits */

	    for (door = 0; door < 5; door++)

	    {

		if( (pexit = from_room->exit[door]) != NULL )

		{

		    to_room = pexit->u1.to_room;



		    /* Found an exit, does it lead to a different area? */

		    if( to_room != NULL

		    &&  (to_room->vnum < parea->min_vnum

		    ||   to_room->vnum > parea->max_vnum) )

		    {

			found = TRUE;

			sprintf(buf, "%s (%d) links %s to %s (%d)\n\r",

				    parea->name, from_room->vnum, dir_name[door],

				    to_room->area->name, to_room->vnum);



			/* File or buffer output? */

			if(fp == NULL)

			    send_to_char(buf, ch);

			else

			    fprintf(fp, buf);

		    }

		}

	    }

	}

    }



    /* Informative message telling you it's not externally linked */

    if(!found)

    {

	send_to_char("No links to other areas found.\n\r",ch);

	/* Let's just delete the file if no links found */

	if (fp)

	    unlink(arg2);

	return;

    }



    /* Close up and clean up file stuff */

    if(fp)

    {

	fclose(fp);

	fpReserve = fopen(NULL_FILE, "r");

    }



}



void do_jail( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char varg[MAX_INPUT_LENGTH];

    ROOM_INDEX_DATA *location;

    DESCRIPTOR_DATA *d;

    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];

    int value;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    argument = one_argument( argument, varg );



    if ( arg1[0] == '\0' )

    {

	send_to_char( "Send whom to which Prison?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if ( victim->in_room == NULL )

    {

	send_to_char( "They are in limbo.\n\r", ch );

	return;

    }



    if ( IS_NPC(victim) )

    {

	send_to_char( "You may not JAIL mobiles.\n\r", ch );

	return;

    }



    if ( IS_IMMORTAL(victim) )

    {

	send_to_char( "You can not JAIL an Immortal!\n\r", ch );

	return;

    }



    if ( arg2[0] == '\0' )

    {

	sprintf( buf, "No Prison was specified, %s will be sent to %d.\n\r", victim->name, ROOM_VNUM_PRISON );

	send_to_char( buf, ch );

	location == get_room_index(ROOM_VNUM_PRISON);

    }

    else

    {

	if ( ( location = find_location( ch, arg2 ) ) == NULL )

	{

	    send_to_char( "No such location.\n\r", ch );

	    return;

	}



	if (	!(	(location == get_room_index(ROOM_VNUM_JAIL))

		||	(location == get_room_index(ROOM_VNUM_PRISON))

		||	(location == get_room_index(ROOM_VNUM_SC01))

		||	(location == get_room_index(ROOM_VNUM_SC02))

		||	(location == get_room_index(ROOM_VNUM_SC03))

		||	(location == get_room_index(ROOM_VNUM_SC04))

		||	(location == get_room_index(ROOM_VNUM_SC05))

	   	))

	{

		send_to_char( "That is not a Prison Cell.\n\r", ch );

		sprintf( buf,

			"Valid Rooms are:\n\r     %-5d     %-5d     %-5d\n\r     %-5d     %-5d     %-5d\n\r     %-5d{w\n\r",

			ROOM_VNUM_JAIL,

			ROOM_VNUM_PRISON,

			ROOM_VNUM_SC01,

			ROOM_VNUM_SC02,

			ROOM_VNUM_SC03,

			ROOM_VNUM_SC04,

			ROOM_VNUM_SC05 );

		send_to_char( buf, ch );

		return;

	}

    }



    if ( varg[0] == '\0' )

	value = 0;

    else

	value = atoi( varg );



    if ( value > get_econ_value( "max_punish" ) )

    {

	sprintf( buf, "You may only JAIL someone for up to %d ticks.\n\r", get_econ_value( "max_punish" ) );

	send_to_char( buf, ch );

	return;

    }



    if ( !IS_ADMIN(ch) && (value == 0) )

    {

	send_to_char( "Only Administrators may JAIL someone indefinately.\n\r", ch );

	return;

    }



    if ( victim->fighting != NULL )

	stop_fighting( victim, TRUE );

    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == victim->in_room

      &&   can_see(d->character, victim)

	&&   d->character != victim )

	{

	  sprintf(buf, "%s is bound in chains and vanishes in a flash of light.\n\r", victim->name );

	  send_to_char( buf, d->character );

    	}

    }

    char_from_room( victim );

    char_to_room( victim, location );

    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == victim->in_room

      &&   can_see(d->character, victim)

	&&   d->character != victim )

	{

	  sprintf(buf, "%s arrives from a flash of light bound in chains.\n\r", victim->name );

	  send_to_char( buf, d->character );

    	}

    }

    if ( ch != victim )

	act( "$n has bound you in chains and sent you to prison!", ch, NULL, victim, TO_VICT );

    do_look( victim, "auto" );

    victim->position = POS_STANDING;

    victim->pcdata->timer_punished[PUNISH_PRISON] = value;

    sprintf(buf,"%s sends %s to prison (%s [%d]) for %d ticks",

	ch->name,

	victim->name,

	location->name,

	location->vnum,

	value);

    wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));

    log_string( buf );

    sprintf( buf, "Prisoner {m%s{w transfered to {c%s{w [{g%d{w] for {r%d{w ticks.\n\r",

	victim->name, location->name, location->vnum, value );

    send_to_char( buf, ch );



    return;

}



void do_release( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    ROOM_INDEX_DATA *location;

    DESCRIPTOR_DATA *d;

    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];



    argument = one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Release whom?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if (	!is_in_prison(victim))

    {

		send_to_char( "They are not in a prison cell.\n\r", ch );

		return;

    }





    if ( victim->fighting != NULL )

	stop_fighting( victim, TRUE );

    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == victim->in_room

      &&   can_see(d->character, victim)

	&&   d->character != victim )

	{

	  sprintf(buf, "%s is released from chains and vanishes in a flash of light.\n\r", victim->name );

	  send_to_char( buf, d->character );

    	}

    }

    char_from_room( victim );

    char_to_room( victim, get_room_index( clan_table[victim->clan].hall ) );

    for ( d = descriptor_list; d; d = d->next )

    {

	if ( d->connected == CON_PLAYING

	&&   d->character->in_room == victim->in_room

      &&   can_see(d->character, victim)

	&&   d->character != victim )

	{

	  sprintf(buf, "%s arrives from a flash of light released from their chains.\n\r", victim->name );

	  send_to_char( buf, d->character );

    	}

    }



    if ( ch != victim )

	act( "$n has released you from chains and set you free!", ch, NULL, victim, TO_VICT );

    do_look( victim, "auto" );

    victim->position = POS_STANDING;

    victim->pcdata->timer_punished[PUNISH_PRISON] = 0;

    sprintf(buf,"%s releases %s from prison", ch->name, victim->name );

    wiznet(buf,NULL,NULL,WIZ_PENALTIES,0,get_trust(ch));

    log_string( buf );

    sprintf( buf, "Prisoner {m%s{w released from prison.\n\r", victim->name );

    send_to_char( buf, ch );



    return;

}





