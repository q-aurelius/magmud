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
 **************************************************************************/

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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

/* command procedures needed */
DECLARE_DO_FUN(do_quit  );

char *  awiz_lvl		args( ( CHAR_DATA *ch ) );
char *  strip_color	args( ( CHAR_DATA *ch, char *argument ) );



char *awiz_lvl( CHAR_DATA *ch )
{
    char wbuf[MAX_STRING_LENGTH];
    char ibuf[MAX_STRING_LENGTH];
    char rbuf[MAX_STRING_LENGTH];

    if (!IS_IMMORTAL(ch))
    {
	sprintf( rbuf, "{w" );
	return rbuf;
    }

    if (ch->invis_level > 1)
    {
	if (ch->invis_level <=MAX_LEVEL && ch->invis_level >=LEVEL_HERO)
    	{
	  switch(ch->invis_level)
	  {
	    default:		sprintf(wbuf, "{rBOZO");	break;
	    case LEVEL_HERO:	sprintf(wbuf, "{rHER");		break;
	    case AVATAR:		sprintf(wbuf, "{rAVA");		break;
	    case ANGEL:		sprintf(wbuf, "{rANG");		break;
	    case DEMI:		sprintf(wbuf, "{rDEM");		break;
	    case IMMORTAL:	sprintf(wbuf, "{rIMM");		break;
	    case GOD:		sprintf(wbuf, "{rGOD");		break;
	    case DEITY:		sprintf(wbuf, "{rDEI");		break;
	    case SUPREME:		sprintf(wbuf, "{rSUP");		break;
	    case CREATOR:		sprintf(wbuf, "{rCRE");		break;
	    case MAX_LEVEL:	sprintf(wbuf, "{rIMP");		break;
	  }
      }
      else
	  sprintf(wbuf, "{r%d",ch->invis_level);
    }
    else
	sprintf(wbuf, "{w");

    if (ch->incog_level > 1)
    {
	if (ch->incog_level <=MAX_LEVEL && ch->incog_level >=LEVEL_HERO)
    	{
	  switch(ch->incog_level)
	  {
	    default:		sprintf(ibuf, "{bBOZO");	break;
	    case LEVEL_HERO:	sprintf(ibuf, "{bHER");		break;
	    case AVATAR:		sprintf(ibuf, "{bAVA");		break;
	    case ANGEL:		sprintf(ibuf, "{bANG");		break;
	    case DEMI:		sprintf(ibuf, "{bDEM");		break;
	    case IMMORTAL:	sprintf(ibuf, "{bIMM");		break;
	    case GOD:		sprintf(ibuf, "{bGOD");		break;
	    case DEITY:		sprintf(ibuf, "{bDEI");		break;
	    case SUPREME:		sprintf(ibuf, "{bSUP");		break;
	    case CREATOR:		sprintf(ibuf, "{bCRE");		break;
	    case MAX_LEVEL:	sprintf(ibuf, "{bIMP");		break;
	  }
      }
      else
	  sprintf(ibuf, "{b%d",ch->incog_level);
    }
    else
	sprintf(ibuf, "{w");

    if (ch->invis_level > 1 && ch->incog_level > 1)
	sprintf( rbuf, "{W{{%s{w/%s{W}{w ", wbuf, ibuf );
    else if (ch->invis_level < 1 && ch->incog_level < 1)
	sprintf( rbuf, "{w" );
    else
	sprintf( rbuf, "{W{{%s%s{W}{w ", wbuf, ibuf );

    return rbuf;
}

char *strip_color( CHAR_DATA *ch, char *argument )
{
	int	ichar;
	int	arglen;

	if ( !IS_IMMORTAL(ch) )
	{
		arglen = strlen(argument);

		for ( ichar = 0; ichar <= arglen; ichar++ )
		{
			if ( argument[ichar] == '{' )
			{
				argument[ichar] = '~';
			}
		}
	}

	return argument;
}


/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, char *argument)
{
    send_to_char("You must type the full command to delete yourself.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];

   if (IS_NPC(ch))
	return;

   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("Delete status removed.\n\r",ch);
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
	    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	    sprintf( buf, "%s turns themself into line noise.", ch->name );
	    wiznet(buf,NULL,NULL,0,0,0);
	    sprintf( buf, "%s has deleted.", ch->name );
	    log_string( buf );
	    stop_fighting(ch,TRUE);
	    do_quit(ch,"");
	    unlink(strsave);
	    return;
	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("Just type delete. No argument.\n\r",ch);
	return;
    }

    send_to_char("Type delete again to confirm this command.\n\r",ch);
    send_to_char("WARNING: this command is irreversible.\n\r",ch);
    send_to_char("Typing delete with an argument will undo delete status.\n\r",
	ch);
    ch->pcdata->confirm_delete = TRUE;
    sprintf( buf, "%s is contemplating deletion.", ch->name );
    wiznet(buf,NULL,NULL,0,0,get_trust(ch));
    log_string( buf );
}


void do_nobounty( CHAR_DATA *ch, char *argument )
{
    {
      if (IS_SET(ch->info,INFO_NOBOUNTY))
      {
	send_to_char("Bounty messages are now ON.\n\r",ch);
	REMOVE_BIT(ch->info,INFO_NOBOUNTY);
      }
      else
      {
	send_to_char("Bounty messages are now OFF.\n\r",ch);
	SET_BIT(ch->info,INFO_NOBOUNTY);
      }
    }
}


void do_bounty( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' )
        {
      	send_to_char( "{wPlace a bounty on who's head?\n\r", ch );
		send_to_char( "{rSyntax: {wBounty <victim> <amount in gold>\n\r", ch );
      	send_to_char( "{gType 'nobounty' to turn bounty messages off.{w\n\r", ch );
      	return;
        }

        if ( ( victim = get_char_world( ch, arg1 ) ) == NULL)
	{
  	   send_to_char( "They are currently not logged in!", ch );
	   return;
      }

      if (IS_NPC(victim))
      {
	send_to_char( "You cannot put a bounty on NPCs!", ch );
	return;
      }

	if ( is_number( arg2 ) )
        {
	int amount;
	amount   = atoi(arg2);
        if (ch->gold < amount)
        {
		send_to_char( "You don't have that much gold!", ch );
		return;
        }
        if (amount < 0)
        {
		send_to_char( "I don't think so!", ch );
		return;
        }
        if (victim->pcdata->bounty > 25000)
        {
		send_to_char( "Their head is already priced high enough!", ch );
		return;
        }
        if (amount > 25000)
        {
		send_to_char( "You can't place a bounty that large!", ch );
		return;
        }
	ch->gold -= amount;
	victim->pcdata->bounty +=amount;

	sprintf( buf, "{wYou have added %d gold to the bounty on %s.\n\r",
	amount,victim->name );
	send_to_char(buf,ch);

	sprintf(buf, "The total bounty is now %d gold on %s.\n\r",
	victim->pcdata->bounty,victim->name );
	send_to_char( buf, ch);

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     d->character != ch
	     && !IS_SET(d->character->info,INFO_NOBOUNTY)
	     && !IS_SET(d->character->comm,COMM_QUIET) )
	{
		if ( IS_IMMORTAL(d->character) )
		{
			sprintf(buf, "{RBOUNTY{w: {W%s{W has placed a %d bounty on %s{W.{w\n\r",
			PERS(ch, d->character), amount, PERS(victim, d->character));
			send_to_char( buf, d->character );
		}
		sprintf(buf, "{RBOUNTY{w: {WThere is now a %d gold bounty on %s{W.{w\n\r",
		victim->pcdata->bounty,PERS(victim, d->character));
	    	send_to_char( buf, d->character );
	}
    }

	return;
	}
}


/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    /* lists all channels and their status */
    send_to_char("   channel     status\n\r",ch);
    send_to_char("------------------------\n\r",ch);

    send_to_char("{mgossip            ",ch);
    if (!IS_SET(ch->comm,COMM_NOGOSSIP))
      send_to_char("ON{w\n\r",ch);
    else
      send_to_char("OFF{w\n\r",ch);

    send_to_char("{Yauction           ",ch);
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
      send_to_char("ON{w\n\r",ch);
    else
      send_to_char("OFF{w\n\r",ch);

    send_to_char("{ymusic             ",ch);
    if (!IS_SET(ch->comm,COMM_NOMUSIC))
      send_to_char("ON{w\n\r",ch);
    else
      send_to_char("OFF{w\n\r",ch);

    send_to_char("{GQ/A               ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUESTION))
      send_to_char("ON{w\n\r",ch);
    else
      send_to_char("OFF{w\n\r",ch);

    send_to_char("{MQuote             ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUOTE))
	send_to_char("ON{w\n\r",ch);
    else
	send_to_char("OFF{w\n\r",ch);

    send_to_char("{rgrats             ",ch);
    if (!IS_SET(ch->comm,COMM_NOGRATS))
      send_to_char("ON{w\n\r",ch);
    else
      send_to_char("OFF{w\n\r",ch);

    send_to_char("shouts            ",ch);
    if (!IS_SET(ch->comm,COMM_SHOUTSOFF))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("bounty            ",ch);
    if (!IS_SET(ch->info,INFO_NOBOUNTY))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("{gtips              ",ch);
    if (!IS_SET(ch->info,INFO_NOTIPS))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("{ginfo messages     ",ch);
    if (!IS_SET(ch->info,INFO_NOINFO))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("{gtells             ",ch);
    if (!IS_SET(ch->comm,COMM_DEAF))
	send_to_char("ON{w\n\r",ch);
    else
	send_to_char("OFF{w\n\r",ch);

    if (( is_clan(ch) || ch->level >=20 ) || IS_HERO(ch))
    {
	send_to_char("{rflame             ",ch);
	if (!IS_SET(ch->comm,COMM_NOFLAME))
		send_to_char("{r-({WON{r)-{w\n\r",ch);
	else
		send_to_char("{r-({WOFF{r)-{w\n\r",ch);
    }

    if (ch->level >=5)
    {
	send_to_char("{cOOC               ",ch);
	if (!IS_SET(ch->comm,COMM_NOOOC))
		send_to_char("{C[{cON{C]{w\n\r",ch);
	else
		send_to_char("{C[{cOFF{C]{w\n\r",ch);
    }


    send_to_char("quiet mode        ",ch);
    if (IS_SET(ch->comm,COMM_QUIET))
      send_to_char("ON",ch);
    else
      send_to_char("OFF",ch);

    if (IS_SET(ch->info, INFO_QUIETALL))
	send_to_char(" {rALL{w\n\r",ch);
    else
	send_to_char("{w\n\r",ch);


  if (IS_HERO(ch))
    {
      send_to_char("{bimm channel       ",ch);
      if(!IS_SET(ch->comm,COMM_NOWIZ))
	send_to_char("{b[{gON{b]>{w\n\r",ch);
      else
	send_to_char("{b[{gOFF{b]>{w\n\r",ch);
    }

    if (get_trust(ch)>=SUPREME)
    {
       send_to_char("{rsup channel       ",ch);
       if(!IS_SET(ch->comm,COMM_NOSUP))
	send_to_char("{r(({yON{r)>>{w\n\r",ch);
       else
	send_to_char("{r(({yOFF{r)>>{w\n\r",ch);
    }

    if (ch->pcdata->security > 0)
    {
       send_to_char("{Gbuilder channel   ",ch);
       if(!IS_SET(ch->comm,COMM_NOBUILDER))
	send_to_char("{G<{WON{G>{w\n\r",ch);
       else
	send_to_char("{G<{WOFF{G>{w\n\r",ch);
    }

   if (ch->level <= 10 || IS_HERO(ch) || IS_SET(ch->act, PLR_HELPER) )
    {
	send_to_char("{wnewbie            ",ch);
	if (!IS_SET(ch->info,INFO_NONEWBIE))
		send_to_char("{G[{gON{G]{w\n\r",ch);
	else
		send_to_char("{G[{gOFF{G]{w\n\r",ch);
    }


   if (IS_IMMORTAL(ch))
    {
	send_to_char("{bangel channel     ",ch);
	if (!IS_SET(ch->info,INFO_NOANGEL))
	send_to_char("{b({wON{b)>{w\n\r",ch);
      else
	send_to_char("{b({wOFF{b)>{w\n\r",ch);
    }


   if (IS_HERO(ch) || IS_SET(ch->act, PLR_HELPER) )
    {
	send_to_char("{whelper            ",ch);
	if (!IS_SET(ch->info,INFO_NOHELPER))
		send_to_char("{G[{gON{G]{w\n\r",ch);
	else
		send_to_char("{G[{gOFF{G]{w\n\r",ch);
    }


    if (IS_SET(ch->comm,COMM_AFK))
	send_to_char("You are {Y[{GAFK{Y]{w.\n\r",ch);

    if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
	send_to_char("You are immune to snooping.\n\r",ch);

    if (ch->lines != PAGELEN)
    {
	if (ch->lines)
	{
	    sprintf(buf,"You display %d lines of scroll.\n\r",ch->lines+2);
	    send_to_char(buf,ch);
	}
	else
	    send_to_char("Scroll buffering is off.\n\r",ch);
    }

    if (ch->prompt != NULL)
    {
	sprintf(buf,"Your current prompt is: %s\n\r",ch->prompt);
	send_to_char(buf,ch);
    }

    if (IS_SET(ch->comm,COMM_NOSHOUT))
      send_to_char("You cannot shout.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOTELL))
      send_to_char("You cannot use tell.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("You cannot use channels.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("You cannot show emotions.\n\r",ch);

}

/* RT deaf blocks out all shouts */

void do_deaf( CHAR_DATA *ch, char *argument)
{

   if (IS_SET(ch->comm,COMM_DEAF))
   {
     send_to_char("You can now hear tells again.\n\r",ch);
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else
   {
     send_to_char("From now on, you won't hear tells.\n\r",ch);
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */
/* Modified by Storm to allow the all keyword */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		if ( IS_SET(ch->comm,COMM_QUIET) || IS_SET(ch->info,INFO_QUIETALL) )
    		{
      		send_to_char("Quiet mode removed.\n\r",ch);
     			REMOVE_BIT(ch->comm,COMM_QUIET);
			REMOVE_BIT(ch->info,INFO_QUIETALL);
    		}
   		else
   		{
     			send_to_char("From now on, you will only hear says, emotes and special channels.\n\r",ch);
     			SET_BIT(ch->comm,COMM_QUIET);
			REMOVE_BIT(ch->info,INFO_QUIETALL);
   		}
	}
	else if( !strcmp( arg, "all" ) )
	{
		if ( IS_SET(ch->comm,COMM_QUIET) && IS_SET(ch->info,INFO_QUIETALL) )
    		{
      		send_to_char("Quiet mode removed.\n\r",ch);
     			REMOVE_BIT(ch->comm,COMM_QUIET);
			REMOVE_BIT(ch->info,INFO_QUIETALL);
    		}
   		else
   		{
      		send_to_char("From now on, you will only hear says and emotes.\n\r",ch);
     			SET_BIT(ch->comm,COMM_QUIET);
			SET_BIT(ch->info,INFO_QUIETALL);
   		}
	}
	else
	{
		send_to_char( "Quiet turns off and on channels.\n\r", ch );
		send_to_char( "{rSyntax: {wQuiet <all>\n\r", ch );
	}

	return;
}
// Ferg - TIP and INFO channels for immortal play
void do_info ( CHAR_DATA *ch, char * argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if ( IS_NPC(ch) )
		return;

    if (argument[0] == '\0' )
    {
		if (IS_SET(ch->info,INFO_NOINFO))
		{
		   send_to_char("You will now see informational messages again.\n\r",ch);
		   REMOVE_BIT(ch->info,INFO_NOINFO);
		}
		else
		{
		  send_to_char("From now on, you will not see information messages.\n\r",ch);
		  SET_BIT(ch->info,INFO_NOINFO);
		}
		return;
	}

	if ( IS_IMMORTAL(ch) )
	{
		REMOVE_BIT(ch->info,INFO_NOINFO);

		for ( d = descriptor_list; d; d = d->next )
		{
			CHAR_DATA *victim;
			victim = d->original ? d->original : d->character;
			if ( d->connected == CON_PLAYING && !IS_SET(d->character->info, INFO_NOINFO) )
			{
				sprintf(buf, "{RINFO: {W%s{w\n\r", strip_color( ch, argument));
				send_to_char(buf, victim);
			}
		}
		wiznet( "$N sent info.", ch, NULL, WIZ_SECURE, 0, get_trust(ch));
	}
	return;
}

// Ferg - TIP and INFO channels for immortal play
void do_tips ( CHAR_DATA *ch, char * argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if ( IS_NPC(ch) )
		return;

	if (argument[0] == '\0' )
	{
		if (IS_SET(ch->info,INFO_NOTIPS))
   		{
   		   send_to_char("Tips will now be displayed.\n\r",ch);
   		   REMOVE_BIT(ch->info,INFO_NOTIPS);
   		}
   		else
   		{
   		  send_to_char("Tips will no longer be displayed\n\r",ch);
   		  SET_BIT(ch->info,INFO_NOTIPS);
   		}
   		return;
   	}

   	if ( IS_IMMORTAL(ch) )
   	{
		for ( d = descriptor_list; d; d = d->next )
		{
			CHAR_DATA *victim;
			victim = d->original ? d->original : d->character;
			if ( d->connected == CON_PLAYING )
			{
				sprintf(buf, "{RTIP: {W%s{w\n\r", strip_color( ch, argument));
				send_to_char(buf, victim);
			}
		}
		wiznet( "$N sent a tip.", ch, NULL, WIZ_SECURE, 0, get_trust(ch) );
   	}
   	return;
}


void do_pray ( CHAR_DATA *ch, char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *target;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
	return;

    target = NULL;

    if ( IS_HERO(ch) )
    {
	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		send_to_char( "Answer the prayers of whom with what?\n\r", ch );
		return;
	}

	if ( ( target = get_char_world( ch, arg ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if ( IS_HERO(target) || IS_NPC(target) )
	{
		send_to_char( "You can only answer the prayers of Mortals!\n\r", ch );
		return;
	}

	sprintf(awiz ,awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
		sprintf( buf, "%s{wYou answer %s{w's prayer: {W'%s{W'{w\n\r", awiz, target->name, strip_color( ch, argument) );
		send_to_char( buf, ch );
	}
	else
	{
		sprintf( buf, "{wYou answer %s{w's prayer: {W'%s{W'{w\n\r", target->name, strip_color( ch, argument) );
		send_to_char( buf, ch );
	}

	sprintf( buf, "{WThe Gods whisper to you: {w%s{w\n\r", strip_color( ch, argument) );
	send_to_char( buf, target );

    	for ( d = descriptor_list; d != NULL; d = d->next )
    	{
		if ( d->connected == CON_PLAYING
		     && d->character != ch
		     && !IS_NPC(d->character)
		     && IS_HERO(d->character)
		     && !IS_SET(d->character->comm,COMM_QUIET) )
		{
	  		if(IS_SET(d->character->act,PLR_AUTOWIZI) && can_see(d->character, ch))
	  		{
				sprintf( buf, "%s{w%s answers %s{w's prayer: {W'%s{W'{w\n\r",
						awiz, ch->name, target->name, strip_color( ch, argument) );
				send_to_char( buf, d->character );
	  		}
	  		else
	  		{
				sprintf( buf, "{w%s answers the prayer of %s{w: {W'%s{W'{w\n\r",
					PERS(ch, d->character), target->name, strip_color( ch, argument) );
				send_to_char( buf, d->character );
			}
		}
    	}
    }
    else
    {
	if (argument[0] == '\0' )
	{
		send_to_char("What is your prayer?\n\r", ch);
		return;
	}

	if ( censor_text(ch,argument) )
		return;

	sprintf( buf, "{wYou kneel down and pray {W'%s{W'{w.\n\r", strip_color( ch, argument) );
	send_to_char( buf, ch );

    	WAIT_STATE( ch, 12 * PULSE_VIOLENCE );

    	for ( d = descriptor_list; d != NULL; d = d->next )
    	{
		if ( d->connected == CON_PLAYING
		     && d->character != ch
		     && !IS_NPC(d->character)
		     && IS_HERO(d->character)
		     && !IS_SET(d->character->comm,COMM_QUIET) )
		{
			sprintf( buf, "{w%s{w prays {W'%s{W'{w.\n\r", ch->name, strip_color( ch, argument) );
			send_to_char( buf, d->character );
		}
    	}
    }

    return;
}



/* afk command */

void do_afk ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_AFK))
    {
      send_to_char("{Y[{GAFK{Y]{w mode removed. Type 'replay' to see tells.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_AFK);
    }
   else
   {
     send_to_char("You are now in {Y[{GAFK{Y]{w mode.\n\r",ch);
     SET_BIT(ch->comm,COMM_AFK);
   }
}

void do_replay (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
	send_to_char("You can't replay.\n\r",ch);
	return;
    }

    if (buf_string(ch->pcdata->buffer)[0] == '\0')
    {
	send_to_char("You have no tells to replay.\n\r",ch);
	return;
    }

    page_to_char(buf_string(ch->pcdata->buffer),ch);
    clear_buf(ch->pcdata->buffer);
}

/* RT auction rewritten in ROM style */
void do_auction( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOAUCTION))
      {
	send_to_char("Auction channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOAUCTION);
      }
      else
      {
	send_to_char("Auction channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOAUCTION);
      }
    }
    else  /* auction message sent, turn auction on if it is off */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
	}

	REMOVE_BIT(ch->comm,COMM_NOAUCTION);

      sprintf( awiz, awiz_lvl(ch));

    	if (IS_SET(ch->act, PLR_AUTOWIZI))
    	{
		sprintf( buf, "%s{YYou auction '%s{Y'{w\n\r", awiz, strip_color( ch, argument) );
		send_to_char( buf, ch );
    	}
    	else
    	{
		sprintf( buf, "{YYou auction '%s{Y'{w\n\r", strip_color( ch, argument) );
		send_to_char( buf, ch );
    	}

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOAUCTION) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
	    if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
	    {
		sprintf( buf, "%s{Y%s{Y auctions '%s{Y'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		send_to_char( buf, victim );
	    }
	    else
	    {
		sprintf( buf, "{Y%s{Y auctions '%s{Y'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		send_to_char( buf, victim );
	    }
	  }
      }
    }
    return;
}

/* RT chat replaced with ROM gossip */
void do_gossip( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;


    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOGOSSIP))
      {
	send_to_char("Gossip channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOGOSSIP);
      }
      else
      {
	send_to_char("Gossip channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOGOSSIP);
      }
    }
    else  /* gossip message sent, turn gossip on if it isn't already */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
 	}

      REMOVE_BIT(ch->comm,COMM_NOGOSSIP);

 	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	  sprintf( buf, "%s{mYou gossip '%s{m'{w\n\r", awiz, strip_color( ch, argument) );
	  send_to_char( buf, ch );
	}
	else
	{
	  sprintf( buf, "{mYou gossip '%s{m'{w\n\r", strip_color( ch, argument) );
	  send_to_char( buf, ch );
	}

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOGOSSIP) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
   	    if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
	    {
		sprintf( buf, "%s{m%s{m gossips '%s{m'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		send_to_char( buf, victim );
	    }
	    else
	    {
		sprintf( buf, "{m%s{m gossips '%s{m'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		send_to_char( buf, victim );
	    }
	  }
      }
    }
    return;
}

void do_flamechan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (!( is_clan(ch) || ch->level >= 20  || IS_HERO(ch) ))
    {
	send_to_char( "You must join a clan or be at or above level 20 to flame someone.\n\r", ch );
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOFLAME))
      {
	send_to_char("Flame channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOFLAME);
      }
      else
      {
	send_to_char("Flame channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOFLAME);
      }
    }
    else
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;

	}

      REMOVE_BIT(ch->comm,COMM_NOFLAME);

	sprintf( awiz, awiz_lvl(ch));

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       (victim->level >= 20 || is_clan(victim) || IS_HERO(victim)) &&
	       !IS_SET(victim->comm,COMM_NOFLAME) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{r-(%s{W%s{W FLAMES{r)- '{W%s{r'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{r-({W%s{W FLAMES{r)- '{W%s{r'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

void do_oocchan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (ch->level < 5)
    {
	send_to_char( "You must be level 5 or above to use the OOC channel.\n\r", ch);
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOOOC))
      {
	send_to_char("Out Of Character channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOOOC);
      }
      else
      {
	send_to_char("Out Of Character channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOOOC);
      }
    }
    else
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
 	}

      REMOVE_BIT(ch->comm,COMM_NOOOC);

	sprintf( awiz, awiz_lvl(ch));

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim->level >= 5 &&
	       !IS_SET(victim->comm,COMM_NOOOC) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{C[{cOOC{C]{c:%s{C%s {c'%s{c'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{C[{cOOC{C]{c:{C%s {c'%s{c'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
  	  }
      }
    }
    return;
}

void do_newbietalk( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char awiz[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   if (ch->level > 10 &&
	 !IS_HERO(ch) &&
	 !IS_SET(ch->act, PLR_HELPER) )
   {
	send_to_char( "You must be level 10 or below to use the newbie channel.\n\r", ch);
	return;
   }
   else  /* newbie message sent, turn newbie on if it isn't already */
   {
	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
 	}

	if ( argument[0] == '\0' && (ch->level > 10) )
    	{
      	if (IS_SET(ch->info,INFO_NONEWBIE))
      	{
			send_to_char("Newbie channel is now ON\n\r",ch);
			REMOVE_BIT(ch->info,INFO_NONEWBIE);
      	}
      	else
      	{
			send_to_char("Newbie channel is now OFF\n\r",ch);
			SET_BIT(ch->info,INFO_NONEWBIE);
      	}
      	return;
	}

	REMOVE_BIT(ch->info,INFO_NONEWBIE);

	sprintf( awiz, awiz_lvl(ch));

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	   CHAR_DATA *victim;

	   victim = d->original ? d->original : d->character;

	   if ( d->connected == CON_PLAYING
	   		&& ch->level <= 10
	   		&& (victim->level <= 10 || IS_SET(victim->act, PLR_HELPER)
	   		|| IS_HERO(victim)) )
	   {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{G[{gNEWBIE{G] %s{w%s{w '%s{w'{w\n\r", awiz, PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{G[{gNEWBIE{G] {w%s{w '%s{w'{w\n\r", PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	   }
	   else if ( d->connected == CON_PLAYING &&
	            (ch->level >= LEVEL_HERO ) &&
	            (victim->level <= 10 || IS_SET(victim->act, PLR_HELPER) || IS_HERO(victim)) &&
	            !IS_SET(victim->comm,COMM_QUIET) )
	   {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{G[{gNEWBIE{G] {C<{cIMM{C> %s{w%s{w '%s{w'{w\n\r", awiz, PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{G[{gNEWBIE{G] {C<{cIMM{C> {w%s{w '%s{w'{w\n\r", PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	   }
	   else if ( d->connected == CON_PLAYING &&
	            (IS_SET(ch->act, PLR_HELPER)) && !(ch->level >= LEVEL_HERO ) &&
	            (victim->level <= 10 || IS_SET(victim->act, PLR_HELPER) || IS_HERO(victim)) )
	   {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{G[{gNEWBIE{G] {y[{YHELPER{y] %s{w%s{w '%s{w'{w\n\r", awiz, PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{G[{gNEWBIE{G] {y[{YHELPER{y] {w%s{w '%s{w'{w\n\r", PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	   }
	}
    }
    return;
}


void do_helpertalk( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char awiz[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   if (!IS_HERO(ch) &&
	 !IS_SET(ch->act, PLR_HELPER) )
   {
	send_to_char( "You must be a helper to use the newbie channel.\n\r", ch);
	return;
   }
   else  /* helper message sent, turn helper on if it isn't already */
   {
	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
 	}

	if (IS_SET(ch->info, INFO_QUIETALL))
	{
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
	}

	if ( argument[0] == '\0' )
    	{
      	if (IS_SET(ch->info,INFO_NOHELPER))
      	{
			send_to_char("Helper channel is now ON\n\r",ch);
			REMOVE_BIT(ch->info,INFO_NOHELPER);
      	}
      	else
      	{
			send_to_char("Helper channel is now OFF\n\r",ch);
			SET_BIT(ch->info,INFO_NOHELPER);
      	}
      	return;
	}

	REMOVE_BIT(ch->info,INFO_NOHELPER);

	sprintf( awiz, awiz_lvl(ch));

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	   CHAR_DATA *victim;

	   victim = d->original ? d->original : d->character;

	   if ( d->connected == CON_PLAYING &&
	             (ch->level >= LEVEL_HERO) &&
	            (IS_SET(victim->act, PLR_HELPER) || IS_HERO(victim)) && !IS_SET(victim->info,INFO_QUIETALL) )
	   {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{y[{YHELPER{y] {C<{cIMM{C> %s{w%s{w '%s{w'{w\n\r", awiz, PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{y[{YHELPER{y] {C<{cIMM{C> {w%s{w '%s{w'{w\n\r", PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	   }
	   else if ( d->connected == CON_PLAYING &&
	            (IS_SET(ch->act, PLR_HELPER)) && (ch->level < LEVEL_HERO) &&
	            (IS_SET(victim->act, PLR_HELPER) || IS_HERO(victim)) && !IS_SET(victim->info,INFO_QUIETALL))
	   {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{y[{YHELPER{y] %s{w%s{w '%s{w'{w\n\r", awiz, PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{y[{YHELPER{y] {w%s{w '%s{w'{w\n\r", PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	   }
	}
    }
    return;
}

/* Coded by Storm on 4/27/01*/
void do_codertalk( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char awiz[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   if (!is_coder(ch))
   {
	send_to_char("The coder channel is restricted to approved coders only.\n\r",ch);
	return;
   }

   if (IS_SET(ch->comm,COMM_NOCHANNELS))
   {
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
   }

   if (IS_SET(ch->info, INFO_QUIETALL))
   {
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
   }

   if (argument[0] == '\0' )
   {
      if (IS_SET(ch->info,INFO_NOCODE))
      {
	send_to_char("Coder channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->info,INFO_NOCODE);
      }
      else
      {
	send_to_char("Coder channel is now OFF.\n\r",ch);
	SET_BIT(ch->info,INFO_NOCODE);
      }
   }
   else  /* coder message sent, turn coder on if it isn't already */
   {

      REMOVE_BIT(ch->info,INFO_NOCODE);

	sprintf( awiz, awiz_lvl(ch));

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
  	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       is_coder(victim) &&
	       !IS_SET(victim->info,INFO_NOCODE) && !IS_SET(victim->info,INFO_QUIETALL) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "{B[{DCODER{B] %s{D%s{B: '{D%s{B'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{B[{DCODER{B] {D%s{B: '{D%s{B'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

void do_grats( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOGRATS))
      {
	send_to_char("Grats channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOGRATS);
      }
      else
      {
	send_to_char("Grats channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOGRATS);
      }
    }
    else  /* grats message sent, turn grats on if it isn't already */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;

	}

      REMOVE_BIT(ch->comm,COMM_NOGRATS);

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{rYou grats '%s{r'{w\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{rYou grats '%s{r'{w\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
  	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOGRATS) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "%s{r%s{r grats '%s{r'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{r%s{r grats '%s{r'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

void do_quote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOQUOTE))
      {
	send_to_char("Quote channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOQUOTE);
      }
      else
      {
	send_to_char("Quote channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOQUOTE);
      }
    }
    else  /* quote message sent, turn quote on if it isn't already */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
 	}

      REMOVE_BIT(ch->comm,COMM_NOQUOTE);

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{MYou quote '%s{M'{w\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{MYou quote '%s{M'{w\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}


      for ( d = descriptor_list; d != NULL; d = d->next )
      {
 	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOQUOTE) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "%s{M%s{M quotes '%s{M'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{M%s{M quotes '%s{M'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

/* RT question channel */
void do_question( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOQUESTION))
      {
	send_to_char("Q/A channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOQUESTION);
      }
      else
      {
	send_to_char("Q/A channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOQUESTION);
      }
    }
    else  /* question sent, turn Q/A on if it isn't already */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
	}

	REMOVE_BIT(ch->comm,COMM_NOQUESTION);

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{GYou question '%s{G'{w\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{GYou question '%s{G'{w\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOQUESTION) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "%s{G%s{G questions '%s{G'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{G%s{G questions '%s{G'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

/* RT answer channel - uses same line as questions */
void do_answer( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOQUESTION))
      {
	send_to_char("Q/A channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOQUESTION);
      }
      else
      {
	send_to_char("Q/A channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOQUESTION);
      }
    }
    else  /* answer sent, turn Q/A on if it isn't already */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
	}

	REMOVE_BIT(ch->comm,COMM_NOQUESTION);

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{GYou answer '%s{G'{w\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{GYou answer '%s{G'{w\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOQUESTION) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "%s{G%s{G answers '%s{G'{w\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{G%s{G answers '%s{G'{w\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

/* RT music channel */
void do_music( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOMUSIC))
      {
	send_to_char("Music channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOMUSIC);
      }
      else
      {
	send_to_char("Music channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOMUSIC);
      }
    }
    else  /* music sent, turn music on if it isn't already */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if ( censor_text(ch,argument) )
		return;

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
	}

	REMOVE_BIT(ch->comm,COMM_NOMUSIC);

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{yYou MUSIC: '%s{y'{w\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{yYou MUSIC: '%s{y'{w\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	  CHAR_DATA *victim;

	  victim = d->original ? d->original : d->character;

	  if ( d->connected == CON_PLAYING &&
	       victim != ch &&
	       !IS_SET(victim->comm,COMM_NOMUSIC) &&
	       !IS_SET(victim->comm,COMM_QUIET) )
	  {
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "%s{y%s{y MUSIC: '%s{y'{w\n\r", awiz, PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{y%s{y MUSIC: '%s{y'{w\n\r", PERS(ch, victim), strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	  }
      }
    }
    return;
}

/* clan channels */
void do_clantalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    char rank[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int clan;
    DESCRIPTOR_DATA *d;
    bool admin;


    if IS_NPC(ch)
	return;

    admin	= FALSE;
    clan	= 0;

    if ( get_trust(ch) >= SUPREME || IS_SET(ch->olcs,OLC_CLANADMIN) )
    {
	admin = TRUE;
    }

    if ( !is_clan(ch) )
    {
	if ( !admin )
	{
		send_to_char( "You must join a clan to use the clan channel.\n\r", ch);
		return;
	}
    }

    if ( is_clan(ch) )
    {
	clan = ch->clan;
	if ( clan_table[clan].independent && !admin )
	{
		send_to_char( "You are in an independent clan, you can not use the clan channel.\n\r", ch);
		return;
	}
    }

    if (IS_SET(ch->info, INFO_QUIETALL))
    {
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
    }

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOCLAN))
      {
	send_to_char("Clan channel is now ON\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOCLAN);
      }
      else
      {
	send_to_char("Clan channel is now OFF\n\r",ch);
	SET_BIT(ch->comm,COMM_NOCLAN);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOCLAN);

    if ( admin )
    {
	argument = one_argument( argument, arg );
	if ( argument[0] == '\0' || arg[0] == '\0' )
	{
		send_to_char( "SYNTAX: {cclantalk {R<{Wclan name{R> <{Wmessage{R>{w\n\r", ch );
		return;
	}
	clan = clan_lookup( arg );
    }

    if ( clan == 0 )
    {
	send_to_char( "That clan does not exist!\n\r", ch);
	return;
    }

    if ( !clan_table[clan].active )
    {
	send_to_char( "That clan is not active!\n\r", ch);
	return;
    }

    if ( clan_table[clan].independent )
    {
	send_to_char( "Independent clans can not use the clan channel.\n\r", ch );
	return;
    }

    sprintf( awiz, awiz_lvl(ch));


    if ( (clan != ch->clan) || !is_clan(ch) )
	{
		sprintf( rank, "{c" );
	}
    else if (  IS_SET(ch->act,PLR_CLANLEADER)
	   || IS_SET(ch->info,INFO_CLANGM)
	   || IS_SET(ch->info,INFO_CLANTRAINER)
	   || IS_SET(ch->info,INFO_CLANENFORCER))
    {   /*Checks to attmempt to squash showing rank flags on channels other than your own, mostly for admins*/
		if ( IS_SET(ch->info,INFO_CLANENFORCER))
		{
			sprintf( rank, "{W[{rEnforcer{W]{w" );
		}

		if ( IS_SET(ch->info,INFO_CLANTRAINER)  )
		{
			sprintf( rank, "{W[{gTrainer{W]{w" );
		}

		if ( IS_SET(ch->info,INFO_CLANGM)  )
		{
			sprintf( rank, "{W[{yGrand Master{W]{w" );
		}

		if ( IS_SET(ch->act,PLR_CLANLEADER)  )
		{
			sprintf( rank, "{W[{yLeader{W]{w" );
		}
    }
    else
    {
		sprintf( rank, "{c" );
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
		if (	d->connected == CON_PLAYING )
		{
		  if ( 	!IS_NPC( d->character )
				&& !IS_SET(d->character->comm,COMM_NOCLAN)
				&& !IS_SET(d->character->comm,COMM_QUIET)
				&& !IS_SET(d->character->info,INFO_QUIETALL)
				&& ( (d->character->clan == clan)
					|| (get_trust(d->character) >= SUPREME)
					|| IS_SET(d->character->olcs, OLC_CLANADMIN)) )
		  {
			sprintf( buf, "%s%s %s%s%s{c '%s{c'{w\n\r",
				(!clan_table[clan].visible ? "{W[{mI{W]{w " : "" ),
				clan_table[clan].who_name,
				(IS_SET(d->character->act,PLR_AUTOWIZI) ? awiz : "" ),
				( can_see(d->character, ch) ? rank : "" ),
				PERS(ch, d->character),
				strip_color( ch, argument) );
			send_to_char( buf, d->character );
		  }
		}
    }
    return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (IS_SET(ch->info, INFO_QUIETALL))
    {
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
    }

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOWIZ))
      {
		send_to_char("Immortal channel is now ON\n\r",ch);
		REMOVE_BIT(ch->comm,COMM_NOWIZ);
      }
      else
      {
		send_to_char("Immortal channel is now OFF\n\r",ch);
		SET_BIT(ch->comm,COMM_NOWIZ);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOWIZ);

    sprintf( awiz, awiz_lvl(ch));

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     IS_HERO(d->character) &&
	     !IS_SET(d->character->comm,COMM_NOWIZ) &&
	     !IS_SET(d->character->info,INFO_QUIETALL) )
	{
		if(IS_SET(d->character->act,PLR_AUTOWIZI) && can_see(d->character, ch))
		{
		  sprintf( buf, "{b[{gIMM{b][%s{g%s{b]>:{g %s{w\n\r", awiz, PERS(ch, d->character), strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
		else
		{
		  sprintf( buf, "{b[{gIMM{b][{g%s{b]>:{g %s{w\n\r", PERS(ch, d->character), strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
	}
    }
    return;
}


void do_suptalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (IS_SET(ch->info, INFO_QUIETALL))
    {
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
    }

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOSUP))
      {
	send_to_char("Admin channel is now ON\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOSUP);
      }
      else
      {
	send_to_char("Admin channel is now OFF\n\r",ch);
	SET_BIT(ch->comm,COMM_NOSUP);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOSUP);

    sprintf( awiz, awiz_lvl(ch));

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     get_trust(d->character)>=SUPREME &&
	     !IS_SET(d->character->comm,COMM_NOSUP) &&
	     !IS_SET(d->character->info,INFO_QUIETALL) )
	{
		if(IS_SET(d->character->act,PLR_AUTOWIZI) && can_see(d->character, ch))
		{
		  sprintf( buf, "{r((%s{y%s{r)>>{w::{r %s{w\n\r", awiz, PERS(ch, d->character), strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
		else
		{
		  sprintf( buf, "{r(({y%s{r)>>{w::{r %s{w\n\r", PERS(ch, d->character), strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
	}
    }
    return;
}



void do_buildtalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if ( !IS_BUILDER(ch) )
    {
	send_to_char("{cYou are not a builder!{w\n\rYou do not have access to this channel.\n\r", ch );
	return;
    }

    if (IS_SET(ch->info, INFO_QUIETALL))
    {
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
    }

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
    {
	send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOBUILDER))
      {
	send_to_char("Builder channel is now ON\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOBUILDER);
      }
      else
      {
	send_to_char("Builder channel is now OFF\n\r",ch);
	SET_BIT(ch->comm,COMM_NOBUILDER);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOBUILDER);

    sprintf( awiz, awiz_lvl(ch));

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     IS_BUILDER(d->character) &&
	     !IS_SET(d->character->comm,COMM_NOBUILDER) &&
	     !IS_SET(d->character->info,INFO_QUIETALL) )
	{
		if(IS_SET(d->character->act,PLR_AUTOWIZI) && can_see(d->character, ch))
		{
		  sprintf( buf, "{G<%s{W%s{G> '{W%s{G'{w\n\r",
			awiz,
			PERS(ch, d->character),
			strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
		else
		{
		  sprintf( buf, "{G<{W%s{G> '{W%s{G'{w\n\r",
			PERS(ch, d->character),
			strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
	}
    }
    return;
}


void do_angtalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (IS_SET(ch->info, INFO_QUIETALL))
    {
	  send_to_char("You are in Quiet All Mode.\n\r",ch);
	  return;
    }

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->info,INFO_NOANGEL))
      {
		send_to_char("Angel channel is now ON\n\r",ch);
		REMOVE_BIT(ch->info,INFO_NOANGEL);
      }
      else
      {
		send_to_char("Angel channel is now OFF\n\r",ch);
		SET_BIT(ch->info,INFO_NOANGEL);
      }
      return;
    }

    REMOVE_BIT(ch->info,INFO_NOANGEL);

    sprintf( awiz, awiz_lvl(ch));

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     IS_IMMORTAL(d->character) &&
	     !IS_SET(d->character->info,INFO_NOANGEL) && !IS_SET(d->character->info,INFO_QUIETALL) )
	{
		if(IS_SET(d->character->act,PLR_AUTOWIZI) && can_see(d->character, ch))
		{
		  sprintf( buf, "{b({wANG{b)(%s{w%s{b)>{w: %s{w\n\r", awiz, PERS(ch, d->character), strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
		else
		{
		  sprintf( buf, "{b({wANG{b)({w%s{b)>{w: %s{w\n\r", PERS(ch, d->character), strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
	}
    }
    return;
}



void do_say( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }

    act( "{g$n{g says '$T{g'{w", ch, NULL, argument, TO_ROOM );
    act( "{gYou say '$T{g'{w", ch, NULL, argument, TO_CHAR );
    return;
}



void do_shout( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char awiz[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if (argument[0] == '\0' )
    {
	if (IS_SET(ch->comm,COMM_SHOUTSOFF))
	{
	    send_to_char("You can hear shouts again.\n\r",ch);
	    REMOVE_BIT(ch->comm,COMM_SHOUTSOFF);
	}
	else
	{
	    send_to_char("You will no longer hear shouts.\n\r",ch);
	    SET_BIT(ch->comm,COMM_SHOUTSOFF);
	}
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
	send_to_char( "You can't shout.\n\r", ch );
	return;
    }

    if ( censor_text(ch,argument) )
	return;

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
    {
	send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	return;
    }

    REMOVE_BIT(ch->comm,COMM_SHOUTSOFF);

    WAIT_STATE( ch, 50 );

	sprintf(awiz ,awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{wYou shout '%s{w'\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{wYou shout '%s{w'\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING &&
	     victim != ch &&
	     !IS_SET(victim->comm, COMM_SHOUTSOFF) &&
	     !IS_SET(victim->comm, COMM_QUIET) )
	{
		if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
		{
		  sprintf( buf, "%s{w%s{w shouts '%s{w'\n\r",
			awiz,
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
		else
		{
		  sprintf( buf, "{w%s{w shouts '%s{w'\n\r",
			PERS(ch, victim),
			strip_color( ch, argument) );
		  send_to_char( buf, victim );
		}
	}
    }

    return;
}



void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    char awiz[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    if ( IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) && !IS_IMMORTAL(ch) )
    {
	send_to_char( "You must turn off quiet mode first.\n\r", ch);
	return;
    }

    if (IS_SET(ch->comm,COMM_DEAF) && !IS_IMMORTAL(ch) )
    {
	send_to_char("You must turn off deaf mode first.\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("$N seems to have misplaced $S link...try again later.",
	    ch,NULL,victim,TO_CHAR);
	sprintf(buf,"{g%s{g tells you '%s{g'\{wn\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    && !IS_IMMORTAL(ch))
    {
	act( "$E is not receiving tells.", ch, 0, victim, TO_CHAR );
	return;
    }

    if (IS_SET(victim->comm,COMM_AFK))
    {
	if (IS_NPC(victim))
	{
	    act("$E is {Y[{GAFK{Y]{w, and not receiving tells.",ch,NULL,victim,TO_CHAR);
	    return;
	}

	act("$E is {Y[{GAFK{Y]{w, but your tell will go through when $E returns.",
	    ch,NULL,victim,TO_CHAR);
	sprintf(buf,"{g%s{g tells you '%s{g'{w\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
    }

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{gYou tell %s{g '%s{g'{w\n\r", awiz, PERS(victim, ch), argument );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{gYou tell %s{g '%s{g'{w\n\r", PERS(victim, ch), argument );
	   send_to_char( buf, ch );
	}

	if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
	{
	  sprintf( buf, "%s{g%s{g tells you '%s{g'{w\n\r",
		awiz,
		PERS(ch, victim),
		argument );
	  send_to_char( buf, victim );
	}
	else
	{
	  sprintf( buf, "{g%s{g tells you '%s{g'{w\n\r",
		PERS(ch, victim),
		argument );
	  send_to_char( buf, victim );
	}

    victim->reply       = ch;

    return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char awiz[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->comm, COMM_NOTELL) && !IS_IMMORTAL(ch) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    victim = ch->reply;

    if ( victim  == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf(buf, "{gYou are replying to %s{g.{w\n\r", PERS(victim,ch));
	send_to_char(buf, ch);
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("$N seems to have misplaced $S link...try again later.",
	    ch,NULL,victim,TO_CHAR);
	sprintf(buf,"{g%s{g replies '%s{g'{w\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    &&  !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
    {
	act_new( "$E is not receiving tells.", ch, 0, victim, TO_CHAR,POS_DEAD);
	return;
    }

    if (IS_SET(victim->comm,COMM_AFK))
    {
	if (IS_NPC(victim))
	{
	    act_new("$E is {Y[{GAFK{Y]{w, and not receiving tells.",
		ch,NULL,victim,TO_CHAR,POS_DEAD);
	    return;
	}

	act_new("$E is {Y[{GAFK{Y]{w, but your tell will go through when $E returns.",
	    ch,NULL,victim,TO_CHAR,POS_DEAD);
	sprintf(buf,"{g%s{g replies '%s{g'{w\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
    }

	sprintf( awiz, awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{gYou reply to %s{g '%s{g'{w\n\r", awiz, PERS(victim, ch), argument );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{gYou reply to %s{g '%s{g'{w\n\r", PERS(victim, ch), argument );
	   send_to_char( buf, ch );
	}

	if(IS_SET(victim->act,PLR_AUTOWIZI) && can_see(victim, ch))
	{
	  sprintf( buf, "%s{g%s{g replies '%s{g'{w\n\r",
		awiz,
		PERS(ch, victim),
		argument );
	  send_to_char( buf, victim );
	}
	else
	{
	  sprintf( buf, "{g%s{g replies '%s{g'{w\n\r",
		PERS(ch, victim),
		argument );
	  send_to_char( buf, victim );
	}

    victim->reply       = ch;

    return;
}



void do_yell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char awiz[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
	send_to_char( "You can't yell.\n\r", ch );
	return;
    }

    if ( censor_text(ch,argument) )
	return;

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
    {
	send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Yell what?\n\r", ch );
	return;
    }

	sprintf(awiz ,awiz_lvl(ch));

	if (IS_SET(ch->act, PLR_AUTOWIZI))
	{
	   sprintf( buf, "%s{gYou yell '%s{g'{w\n\r", awiz, strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}
	   else
	{
	   sprintf( buf, "{gYou yell '%s{g'{w\n\r", strip_color( ch, argument) );
	   send_to_char( buf, ch );
	}

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
	&&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
		if(IS_SET(d->character->act,PLR_AUTOWIZI) && can_see(d->character, ch))
		{
		  sprintf( buf, "%s{g%s{g yells '%s{g'{w\n\r",
			awiz,
			PERS(ch, d->character),
			strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
		else
		{
		  sprintf( buf, "{g%s{g yells '%s{g'{w\n\r",
			PERS(ch, d->character),
			strip_color( ch, argument) );
		  send_to_char( buf, d->character );
		}
	}
    }

    return;
}


void do_emote( CHAR_DATA *ch, char *argument )
{
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

    act( "$n $T", ch, NULL, argument, TO_ROOM );
    act( "$n $T", ch, NULL, argument, TO_CHAR );
    return;
}


void do_pmote( CHAR_DATA *ch, char *argument )
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

    act( "$n $t", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
	    continue;

	if ((letter = strstr(argument,vch->name)) == NULL)
	{
	    act("$N $t",vch,argument,ch,TO_CHAR);
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

	act("$N $t",vch,temp,ch,TO_CHAR);
    }

    return;
}


/*
 * All the posing stuff.
 */
struct  pose_table_type
{
    char *      message[2*9]; // 8 = num classes
};

const   struct  pose_table_type pose_table      []      =
{
    {
	{
	    "You sizzle with energy.",
	    "$n sizzles with energy.",
	    "You feel very holy.",
	    "$n looks very holy.",
	    "You perform a small card trick.",
	    "$n performs a small card trick.",
	    "You show your bulging muscles.",
	    "$n shows $s bulging muscles.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "You turn into a butterfly, then return to your normal shape.",
	    "$n turns into a butterfly, then returns to $s normal shape.",
	    "You nonchalantly turn wine into water.",
	    "$n nonchalantly turns wine into water.",
	    "You wiggle your ears alternately.",
	    "$n wiggles $s ears alternately.",
	    "You crack nuts between your fingers.",
	    "$n cracks nuts between $s fingers.",
	    "You play Styx and do the robot.",
	    "Styx begins to play from $n's ears as $e dances The Robot.",
	    "A pack of wolves comes to lay at your feet.",
	    "A pack of wolves comes and lays at $n's feet.",
	    "Abraham Van Helsing nods at you admiringly.",
	    "Abraham Van Helsing nods at $n with admiration.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You ignite your lightsaber and twirl it above your head.",
	    "$n ignites $s lightsaber and twils it above $s head with no hands."
	}
    },

    {
	{
	    "Blue sparks fly from your fingers.",
	    "Blue sparks fly from $n's fingers.",
	    "A halo appears over your head.",
	    "A halo appears over $n's head.",
	    "You nimbly tie yourself into a knot.",
	    "$n nimbly ties $mself into a knot.",
	    "You grizzle your teeth and look mean.",
	    "$n grizzles $s teeth and looks mean.",
	    "You transform into a boom box and play '80s songs.",
	    "$n transforms into a boombox and plays songs from the '80s.",
	    "You sprinkle some dust and a garden grows before you.",
	    "$n sprinkles some dust and a garden suddenly springs forth.",
	    "Count Dracula shows up, but you kick his ass.",
	    "Count Dracula shows up, only to get his ass kicked by $n.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "Little red lights dance in your eyes.",
	    "Little red lights dance in $n's eyes.",
	    "You recite words of wisdom.",
	    "$n recites words of wisdom.",
	    "You juggle with daggers, apples, and eyeballs.",
	    "$n juggles with daggers, apples, and eyeballs.",
	    "You hit your head, and your eyes roll.",
	    "$n hits $s head, and $s eyes roll.",
	    "Your eyes glow bright {Rred{w.",
	    "$n's eyes glow bright {Rred{w.",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "A slimy green monster appears before you and bows.",
	    "A slimy green monster appears before $n and bows.",
	    "Deep in prayer, you levitate.",
	    "Deep in prayer, $n levitates.",
	    "You steal the underwear off every person in the room.",
	    "Your underwear is gone!  $n stole it!",
	    "Crunch, crunch -- you munch a bottle.",
	    "Crunch, crunch -- $n munches a bottle.",
	    "You do the knife trick from 'Aliens'.",
	    "$n spreads one hand on the ground and rapidly stabs a knife into the spaces between $s fingers.",
	    "You raise a hand and{r roses{w spring from the ground.",
	    "$n raise a hand and {rroses{w spring forth from the ground.",
	    "You pull out some garlic and cook a nice tomato sauce.",
	    "$n pulls out some garlic cloves and makes a nice tomato sauce.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use the Jedi mind trick.",
	    "{gYou say 'These aren't the droids we're looking for.'{w"
	}
    },

    {
	{
	    "You turn everybody into a little pink elephant.",
	    "You are turned into a little pink elephant by $n.",
	    "An angel consults you.",
	    "An angel consults $n.",
	    "The dice roll ... and you win again.",
	    "The dice roll ... and $n wins again.",
	    "... 98, 99, 100 ... you do pushups.",
	    "... 98, 99, 100 ... $n does pushups.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "A small ball of light dances on your fingertips.",
	    "A small ball of light dances on $n's fingertips.",
	    "Your body glows with an unearthly light.",
	    "$n's body glows with an unearthly light.",
	    "You count the money in everyone's pockets.",
	    "Check your money, $n is counting it.",
	    "Governor Schwarzenegger admires your physique.",
	    "Governor Schwarzenegger admires $n's physique.",
	    "You scare away Sarah Connor.",
	    "$n frightens away Sarah Connor.",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "Smoke and fumes leak from your nostrils.",
	    "Smoke and fumes leak from $n's nostrils.",
	    "A spot light hits you.",
	    "A spot light hits $n.",
	    "You balance a pocket knife on your tongue.",
	    "$n balances a pocket knife on your tongue.",
	    "Watch your feet, you are juggling granite boulders.",
	    "Watch your feet, $n is juggling granite boulders.",
	    "You assure everyone that you'll return.",
	    "{g$n says 'Ah'll be bah-ck'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "The light flickers as you rap in magical languages.",
	    "The light flickers as $n raps in magical languages.",
	    "Everyone levitates as you pray.",
	    "You levitate as $n prays.",
	    "You produce a coin from everyone's ear.",
	    "$n produces a coin from your ear.",
	    "Oomph!  You squeeze water out of a granite boulder.",
	    "Oomph!  $n squeezes water out of a granite boulder.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "Your head disappears.",
	    "$n's head disappears.",
	    "A cool breeze refreshes you.",
	    "A cool breeze refreshes $n.",
	    "You step behind your shadow.",
	    "$n steps behind $s shadow.",
	    "You pick your teeth with a spear.",
	    "$n picks $s teeth with a spear.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You stand on your head while lifting a number of rocks into the air.",
	    "$n stands on $s head and telekinetically lifts several rocks into the air."
	}
    },

    {
	{
	    "A fire elemental singes your hair.",
	    "A fire elemental singes $n's hair.",
	    "The sun pierces through the clouds to illuminate you.",
	    "The sun pierces through the clouds to illuminate $n.",
	    "Your eyes dance with greed.",
	    "$n's eyes dance with greed.",
	    "Everyone is swept off their foot by your hug.",
	    "You are swept off your feet by $n's hug.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "The sky changes color to match your eyes.",
	    "The sky changes color to match $n's eyes.",
	    "The ocean parts before you.",
	    "The ocean parts before $n.",
	    "You deftly steal everyone's weapon.",
	    "$n deftly steals your weapon.",
	    "Your karate chop splits a tree.",
	    "$n's karate chop splits a tree.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You decline 'interficio'.",
	    "{g$n says 'interficio, interficis, interficit, interficimus ...'{w",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "The stones dance to your command.",
	    "The stones dance to $n's command.",
	    "A thunder cloud kneels to you.",
	    "A thunder cloud kneels to $n.",
	    "The Grey Mouser buys you a beer.",
	    "The Grey Mouser buys $n a beer.",
	    "A strap of your armor breaks over your mighty thews.",
	    "A strap of $n's armor breaks over $s mighty thews.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You talk to the animals.",
	    "$n has a philosophical debate with a chipmunk.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "The heavens and grass change colour as you smile.",
	    "The heavens and grass change colour as $n smiles.",
	    "The Burning Man speaks to you.",
	    "The Burning Man speaks to $n.",
	    "Everyone's pocket explodes with your fireworks.",
	    "Your pocket explodes with $n's fireworks.",
	    "A boulder cracks at your frown.",
	    "A boulder cracks at $n's frown.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You toss a couple holy water baloons.",
	    "$n hits you in the face with a holy water baloon. Hope you're not a vampire...",
	    "Liu Kang bows before you.",
   	    "Liu Kang bows before $n.",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "Everyone's clothes are transparent, and you are laughing.",
	    "Your clothes are transparent, and $n is laughing.",
	    "An eye in a pyramid winks at you.",
	    "An eye in a pyramid winks at $n.",
	    "Everyone discovers your dagger a centimeter from their eye.",
	    "You discover $n's dagger a centimeter from your eye.",
	    "Mercenaries arrive to do your bidding.",
	    "Mercenaries arrive to do $n's bidding.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You show off with your lightsaber.",
	    "$n's lightsaber floats into $s hand.  It ignites suddenly and spins on $s open palm."
	}
    },

    {
	{
	    "A black hole swallows you.",
	    "A black hole swallows $n.",
	    "Valentine Michael Smith offers you a glass of water.",
	    "Valentine Michael Smith offers $n a glass of water.",
	    "Where did you go?",
	    "Where did $n go?",
	    "Four matched Percherons bring in your chariot.",
	    "Four matched Percherons bring in $n's chariot.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
   	    "You pull out a wooden square and a large pawn and play Zen Chess.",
   	    "$n pulls out a wooden square and one giant pawn.  Wanna play Zen Chess?",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "The world shimmers in time with your whistling.",
	    "The world shimmers in time with $n's whistling.",
	    "The great god Urk gives you a staff.",
	    "The great god Urk gives $n a staff.",
	    "Click.",
	    "Click.",
	    "Atlas asks you to relieve him.",
	    "Atlas asks $n to relieve him.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You summon an Italian teenager to wax your cars and paint your fence.",
   	    "$n summons an Italian teenager to wax $s cars and paint $s fence.",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    },

    {
	{
	    "The universe bows to your phenominal cosmic powers.",
	    "The universe bows to $n's phenominal cosmic powers",
	    "You heal the world of all sin and decay.",
	    "$n heal's the world of all sin and decay.",
	    "From the shadows, you slit a few throats.",
	    "From the shadows, $n slits a few throats.",
	    "You are the immovable object.",
	    "$n is the immovable object.",
	    "You begin to recite the digits of Pi.",
	    "{g$n says 'Pi equals 3.1415926535897932384626433832795. . .'{w",
	    "You gesture and the clouds fade away.",
	    "$n gestures and the clouds fade away.",
	    "You begin to juggle wooden stakes.",
	    "$n pulls out a half-dozen wooden stakes and begins to juggle.",
	    "You relentessly clean everything in sight.",
   	    "$n suddenly and relentlessly cleans and organizes everything in sight.",
	    "You use{W The Force{w to lift everyone into the air.",
	    "$n lifts you into the air just by raising $s hand."
	}
    }

};



void do_pose( CHAR_DATA *ch, char *argument )
{
    int level;
    int pose;

    if ( IS_NPC(ch) )
	return;

    level = UMIN( ch->level, sizeof(pose_table) / sizeof(pose_table[0]) - 1 );
    pose  = number_range(0, level);

    act( pose_table[pose].message[2*ch->class+0], ch, NULL, NULL, TO_CHAR );
    act( pose_table[pose].message[2*ch->class+1], ch, NULL, NULL, TO_ROOM );

    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];

	sprintf( buf, "{RBUG{w: {C[{c%d{C] {W%s{w : %s",
		ch->in_room ? ch->in_room->vnum : 0, ch->name, strip_color( ch, argument) );
	wiznet( buf,NULL,NULL,WIZ_LOG,0,get_trust(ch));
	append_file( ch, BUG_FILE, strip_color( ch, argument) );
	send_to_char( "Bug logged.\n\r", ch );
	return;
}

void do_typo( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];

	sprintf( buf, "{RTYPO{w: {C[{c%d{C] {W%s{w : %s",
		ch->in_room ? ch->in_room->vnum : 0, ch->name, strip_color( ch, argument) );
	wiznet( buf,NULL,NULL,WIZ_LOG,0,get_trust(ch));
	append_file( ch, TYPO_FILE, strip_color( ch, argument) );
	send_to_char( "Typo logged.\n\r", ch );
	return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here.  Just save and quit.\n\r", ch );
    return;
}


void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}



void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d,*d_next;
    int id;
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( is_ploaded(ch) )
    {
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
	send_to_char( "You're not DEAD yet.\n\r", ch );
	return;
    }

    send_to_char( "Alas, all good things must come to an end.\nTake care!\n\r",ch);
    for ( d = descriptor_list; d; d = d->next )
    {
		if ( d->connected == CON_PLAYING
		&&   d->character->in_room == ch->in_room
		&&   d->character != ch
		&&   can_see(d->character, ch) )
		{
			send_to_char( ch->name, d->character );
			send_to_char( " has left the game.\n\r", d->character );
		}
    }

    if ( !is_linkdead(ch) )
    {
	sprintf( buf, "%s@%s [%s] is disconnecting at %s.", ch->name, ch->desc->host, ch->desc->ipaddy, ((char *) ctime( &current_time )) );
	log_string( buf );
	wiznet(buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));
    }

    sprintf( buf, "%s has quit.", ch->name );
    log_string( buf );
    sprintf( buf, "%s rejoins the real world.", ch->name );
    wiznet(buf,NULL,NULL,WIZ_LOGOUT,0,get_trust(ch));
    do_mudwatch_out( ch );

    /*
     * After extract_char the ch is no longer valid!
     */
    save_char_obj( ch );
    id = ch->id;
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
	close_socket( d );

    /* toast evil cheating bastards */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char(tch,TRUE);
	    close_socket(d);
	}
    }

    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
    extern bool betalock;

    if ( IS_NPC(ch) )
	return;

    if ( betalock )
    {
	send_to_char( "Game is in {cBETA TEST MODE{w!\n\rChanges were not saved!\n\r", ch );
	return;
    }

    save_char_obj( ch );
    send_to_char("Saving... Remember that there is automatic saving.\n\r", ch);
    WAIT_STATE(ch,4 * PULSE_VIOLENCE);
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
	act("$N doesn't seem to want any followers.\n\r",
	     ch,NULL,victim, TO_CHAR);
	return;
    }

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
	act( "$n stops following you.",     ch, NULL, ch->master, TO_VICT    );
	act( "You stop following $N.",      ch, NULL, ch->master, TO_CHAR    );
    }
    if (ch->master->pet == ch)
	ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
	stop_follower(pet);
	if (pet->in_room != NULL)
	    act("$N slowly fades away.",ch,NULL,pet,TO_NOTVICT);
	extract_char(pet,TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
    {
	if (ch->master->pet == ch)
	    ch->master->pet = NULL;
	stop_follower( ch );
    }

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if (!str_cmp(arg2,"delete"))
    {
	send_to_char("That will NOT be done.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
	||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    sprintf( buf, "$n orders you to '%s'.", strip_color( ch, argument) );
	    act( buf, ch, NULL, och, TO_VICT );
	    if ( !IS_NPC(och) )
	    {
		sprintf( buf, "%s orders %s to '%s'", ch->name, och->name, strip_color( ch, argument) );
		log_string( buf );
		wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	    }
	    interpret( och, strip_color( ch, argument) );
	}
    }

    if ( found )
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	send_to_char( "Ok.\n\r", ch );
    }
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}



void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char log_buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		sprintf( buf,
		"{w[{g%3s %s{w] %-16s {r%4d{w/{r%4d{w hp  {y%4d{w/{y%4d{w mana  {c%4d{w/{c%4d{w mv  {b%5d{w xp\n\r",
		    lev_name(gch->level),
		    IS_NPC(gch) ? "Mob" : class_table[gch->class].who_name,
		    capitalize( PERS(gch, ch) ),
		    gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp    );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act_new("$N isn't following you.",ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	return;
    }

    if (IS_AFFECTED(victim,AFF_CHARM))
    {
	send_to_char("You can't remove charmed mobs from your group.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
	act_new("You like your master too much to leave $m!",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act_new("$n removes $N from $s group.",
	    ch,NULL,victim,TO_NOTVICT,POS_RESTING);
	act_new("$n removes you from $s group.",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
	act_new("You remove $N from your group.",
	    ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	return;
    }

    victim->leader = ch;
    act_new("$N joins $n's group.",ch,NULL,victim,TO_NOTVICT,POS_RESTING);
    act_new("You join $n's group.",ch,NULL,victim,TO_VICT,POS_SLEEPING);
    act_new("$N joins your group.",ch,NULL,victim,TO_CHAR,POS_SLEEPING);
    if (( ch->level >= LEVEL_HERO ) && ( victim->level <= MAX_MORT_LEVEL ))
    {
	sprintf( log_buf, "%s has grouped with a mortal, %s.", ch->name, victim->name );
	wiznet(log_buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	log_string( log_buf );
    }
    if (( victim->level >= LEVEL_HERO ) && ( ch->level <= MAX_MORT_LEVEL ))
    {
	sprintf( log_buf, "%s has grouped with a mortal, %s.", victim->name, ch->name );
	wiznet(log_buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
	log_string( log_buf );
    }
    return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_gold = 0, amount_silver = 0;
    int share_gold, share_silver;
    int extra_gold, extra_silver;

    argument = one_argument( argument, arg1 );
	       one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }

    amount_silver = atoi( arg1 );

    if (arg2[0] != '\0')
	amount_gold = atoi(arg2);

    if ( amount_gold < 0 || amount_silver < 0)
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount_gold == 0 && amount_silver == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    if ( ch->gold <  amount_gold || ch->silver < amount_silver)
    {
	send_to_char( "You don't have that much to split.\n\r", ch );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
    }

    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    share_gold   = amount_gold / members;
    extra_gold   = amount_gold % members;

    if ( share_gold == 0 && share_silver == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

    ch->silver  -= amount_silver;
    ch->silver  += share_silver + extra_silver;
    ch->gold    -= amount_gold;
    ch->gold    += share_gold + extra_gold;

    if (share_silver > 0)
    {
	sprintf(buf,
	    "You split %d silver coins. Your share is %d silver.\n\r",
	    amount_silver,share_silver + extra_silver);
	send_to_char(buf,ch);
    }

    if (share_gold > 0)
    {
	sprintf(buf,
	    "You split %d gold coins. Your share is %d gold.\n\r",
	     amount_gold,share_gold + extra_gold);
	send_to_char(buf,ch);
    }

    if (share_gold == 0)
    {
	sprintf(buf,"$n splits %d silver coins. Your share is %d silver.",
		amount_silver,share_silver);
    }
    else if (share_silver == 0)
    {
	sprintf(buf,"$n splits %d gold coins. Your share is %d gold.",
		amount_gold,share_gold);
    }
    else
    {
	sprintf(buf,
"$n splits %d silver and %d gold coins, giving you %d silver and %d gold.\n\r",
	 amount_silver,amount_gold,share_silver,share_gold);
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share_gold;
	    gch->silver += share_silver;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	{
	    act_new("{M$n tells the group '{c$t{M'{w",
		ch,argument,gch,TO_VICT,POS_SLEEPING);
	}
    }

    sprintf( buf, "{MYou tell your group '{c%s{M'.{w\n\r", argument );
    send_to_char( buf, ch );

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;

    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

/*
 * Colour setting and unsetting, way cool, Lope Oct '94
 */
void do_colour( CHAR_DATA *ch, char *argument )
{
    char        arg[ MAX_STRING_LENGTH ];

    argument = one_argument( argument, arg );

    if( !*arg )
    {
	if( !IS_SET( ch->act, PLR_COLOUR ) )
	{
	    SET_BIT( ch->act, PLR_COLOUR );
	    send_to_char( "{bC{ro{yl{co{mu{gr{x is now {rON{x, Way Cool!\n\r", ch );
	}
	else
	{
	    send_to_char_bw( "Colour is now OFF, <sigh>\n\r", ch );
	    REMOVE_BIT( ch->act, PLR_COLOUR );
	}
	return;
    }
    else
    {
	send_to_char_bw( "Colour Configuration is unavailable in this\n\r", ch );
	send_to_char_bw( "version of colour, sorry\n\r", ch );
    }

    return;
}


void do_clear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int count;

    if IS_NPC(ch)
	return;

    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{rSYNTAX:{w Clear <argument>                                                  \n\r", ch );
	send_to_char( "Arguments are {cscreen{w, {ctitle{w, {cprompt{w, {croomin{w, and {croomout{w. \n\r", ch );

	if ( IS_IMMORTAL(ch) )
		send_to_char( "Immortals may also clear their {cpoofin{w, {cpoofout{w, {cwiziin{w, and {cwiziout{w.\n\r", ch );

	send_to_char( "Clearing them resets them to their default, or clears them completely.        \n\r", ch );
      return;
    }

    if ( !str_prefix( arg, "screen" ) )
    {
	for (count=0 ; count <= (ch->lines+24) ; ++count)
	{
		send_to_char("\n\r", ch);
	}
	return;
    }
    else if ( !str_prefix( arg, "title" ) )
    {
	ch->pcdata->title 	= str_dup( "" );
	send_to_char( "Title cleared.\n\r", ch );
 	return;
    }
    else if ( !str_prefix( arg, "prompt" ) )
    {
	ch->prompt 			= str_dup( "{c<%hhp %mm %vmv>{x " );
	send_to_char( "Prompt cleared and reset.\n\r", ch );
 	return;
    }
    else if ( !str_prefix( arg, "roomin" ) )
    {
	free_string( ch->pcdata->roomin );
	ch->pcdata->roomin 	= str_dup( "" );
	send_to_char( "Roomin cleared.\n\r", ch );
 	return;
    }
    else if ( !str_prefix( arg, "roomout" ) )
    {
	free_string( ch->pcdata->roomout );
	ch->pcdata->roomin 	= str_dup( "" );
	send_to_char( "Roomout cleared.\n\r", ch );
 	return;
    }
    else if ( IS_IMMORTAL(ch) && !str_prefix( arg, "poofin" ) )
    {
	free_string( ch->pcdata->bamfin );
	ch->pcdata->roomin 	= str_dup( "" );
	send_to_char( "Poofin cleared.\n\r", ch );
 	return;
    }
    else if ( IS_IMMORTAL(ch) && !str_prefix( arg, "poofout" ) )
    {
	free_string( ch->pcdata->bamfout );
	ch->pcdata->roomin 	= str_dup( "" );
	send_to_char( "Poofout cleared.\n\r", ch );
 	return;
    }
    /* Ferg-wizicust */
	else if ( IS_IMMORTAL(ch) && !str_prefix( arg, "wiziin" ) )
	{
	free_string( ch->pcdata->wiziin );
	ch->pcdata->roomin 	= str_dup( "" );
	send_to_char( "Wiziin cleared.\n\r", ch );
	return;
    }
	else if ( IS_IMMORTAL(ch) && !str_prefix( arg, "wiziout" ) )
	{
	free_string( ch->pcdata->wiziout );
	ch->pcdata->roomin 	= str_dup( "" );
	send_to_char( "Wiziout cleared.\n\r", ch );
	return;
    }
    /* end Ferg-wizicust*/
    else
    {
	do_clear( ch, "" );
	return;
    }

    return;
}

void do_soundon( CHAR_DATA *ch, char *argument )
{
	if( !IS_SET( ch->info, INFO_SOUNDON ) )
	{
		SET_BIT( ch->info, INFO_SOUNDON );
		send_to_char( "Sound is now on!\n\r", ch );
		send_sound( "welcome.wav", ch );
	}
	else
	{
		send_to_char_bw( "Sound is now off.\n\r", ch );
		REMOVE_BIT( ch->info, INFO_SOUNDON );
	}
	return;
}

void    send_sound( char *argument, CHAR_DATA *ch )
{
	char	buf[MAX_STRING_LENGTH];

	if( IS_SET(ch->info, INFO_SOUNDON) )
	{
		sprintf( buf, "!!SOUND(%s%s)", SOUND_DIR, argument );
		send_to_char( buf, ch );
	}
	return;
}
