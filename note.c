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
#include <sys/time.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"

/* globals from db.c for load_notes */
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif
extern FILE *                  fpArea;
extern char                    strArea[MAX_INPUT_LENGTH];

/* local procedures */
void load_thread(char *name, NOTE_DATA **list, int type, time_t free_time);
void parse_note(CHAR_DATA *ch, char *argument, int type);
bool hide_note(CHAR_DATA *ch, NOTE_DATA *pnote);
bool msgspoollock args( ( CHAR_DATA *ch ) );

NOTE_DATA *note_list;
NOTE_DATA *idea_list;
NOTE_DATA *penalty_list;
NOTE_DATA *news_list;
NOTE_DATA *changes_list;
NOTE_DATA *immnote_list;
NOTE_DATA *clannote_list;
NOTE_DATA *rpnote_list;

bool msgspoollock( CHAR_DATA *ch )
{
	char buf[MAX_STRING_LENGTH];
	extern bool spool_lock;
	extern int spool_lock_count;
	int count;

	if ( spool_lock )
	{
		spool_lock_count++;
		for ( count = 0; economy_table[count].name != NULL; count++ )
		{
			if ( !str_prefix( economy_table[count].name ,"spool_lock_report") )
			{
				if ( spool_lock_count >= economy_table[count].value )
				{
					sprintf( buf, "Spool Lockout Threshold Exceeded : User: {c%s{w   Count: {y%d{w   Threshold: {g%ld{w",
						ch->name, spool_lock_count, economy_table[count].value );
					wiznet(buf,NULL,NULL,WIZ_SECURE,0,get_trust(ch));
				}
			}
		}
		sprintf( buf, "MESSAGE SPOOL LOCKOUT : User: %s   Count: %d", ch->name, spool_lock_count );
		log_string( buf );
		send_to_char( "Message Spool Lock is active!\n\r", ch );
		send_to_char( "Operation did not complete, please try again in a minute.\n\r", ch );
		WAIT_STATE(ch, 6 * PULSE_VIOLENCE);
		return TRUE;
	}
	else
	{
		spool_lock_count = 0;
		return FALSE;
	}
	return FALSE;
}


int count_spool(CHAR_DATA *ch, NOTE_DATA *spool)
{
    int count = 0;
    NOTE_DATA *pnote;
    extern bool spool_lock;

    spool_lock 	= TRUE;

    for (pnote = spool; pnote != NULL; pnote = pnote->next)
	if (!hide_note(ch,pnote))
	    count++;

    spool_lock	= FALSE;

    return count;
}

void do_unread(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    int count;
    bool found = FALSE;

    if (IS_NPC(ch))
	return; 

    if ((count = count_spool(ch,news_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gThere %s {y%d{g new news article%s waiting.{w\n\r",
	    count > 1 ? "are" : "is",count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,changes_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gThere %s {y%d{g change%s waiting to be read.{w\n\r",
	    count > 1 ? "are" : "is", count, count > 1 ? "s" : "");
        send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,note_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gYou have {y%d{g new note%s waiting.{w\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,idea_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gYou have {y%d{g unread idea%s to peruse.{w\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,rpnote_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gYou have {y%d{g new RP note%s waiting.{w\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if (IS_TRUSTED(ch,LEVEL_HERO) && (count = count_spool(ch,immnote_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gYou have {y%d{g unread Immortal note%s to peruse.{w\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((is_clan(ch) || IS_ADMIN(ch) || IS_SET(ch->olcs,OLC_CLANADMIN)) && (count = count_spool(ch,clannote_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{gYou have {y%d{g unread clan note%s to peruse.{w\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if (IS_TRUSTED(ch,LEVEL_HERO) && (count = count_spool(ch,penalty_list)) > 0)
    {
	found = TRUE;
	sprintf(buf,"{y%d{r %s been added.{w\n\r",
	    count, count > 1 ? "penalties have" : "penalty has");
	send_to_char(buf,ch);
    }

    if (!found)
	send_to_char("You have no unread messages.\n\r",ch);

    return;
}

void do_new_note_notify(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    int count;
    
    if (IS_NPC(ch))
	return; 

    if (IS_SET(ch->info,INFO_NOINFO))
	return;

    if ((count = count_spool(ch,news_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WThere %s {y%d{W new News article%s waiting. {w{*\n\r",
	    count > 1 ? "are" : "is",count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,changes_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WThere %s {y%d{W Change%s waiting to be read. {w{*\n\r",
	    count > 1 ? "are" : "is", count, count > 1 ? "s" : "");
        send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,note_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WYou have {y%d{W new Note%s waiting. {w{*\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,idea_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WYou have {y%d{W unread Idea%s to peruse. {w{*\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((count = count_spool(ch,rpnote_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WYou have {y%d{W unread RPNote%s to peruse. {w{*\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if ((is_clan(ch) || IS_ADMIN(ch) || IS_SET(ch->olcs,OLC_CLANADMIN)) && (count = count_spool(ch,clannote_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WYou have {y%d{W unread Clan Note%s to read. {w{*\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if (IS_HERO(ch) && (count = count_spool(ch,immnote_list)) > 0)
    {
	sprintf(buf,"{RINFO: {WYou have {y%d{W unread Imm Note%s to review. {w{*\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }

    if (IS_HERO(ch) && (count = count_spool(ch,penalty_list)) > 0)
    {
	sprintf(buf,"{RINFO {y%d{r %s been added. {w{*\n\r",
	    count, count > 1 ? "penalties have" : "penalty has");
	send_to_char(buf,ch);
    }

    return;
}


void do_note(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_NOTE);
    return;
}

void do_idea(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_IDEA);
    return;
}

void do_news(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_NEWS);
    return;
}

void do_changes(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_CHANGES);
    return;
}

void do_rpnote(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_RPNOTE);
    return;
}

void do_penalty(CHAR_DATA *ch,char *argument)
{
  if ( !IS_HERO(ch) )
  {
	send_to_char( "You must be an Immortal to use penalties.\n\r", ch );
	return;
  }
  parse_note(ch,argument,NOTE_PENALTY);
  return;
}

void do_immnote(CHAR_DATA *ch,char *argument)
{
  if ( !IS_HERO(ch) )
  {
	send_to_char( "You must be an Immortal to use Imm Notes.\n\r", ch );
	return;
  }
  parse_note(ch,argument,NOTE_IMMNOTE);
  return;
}

void do_clannote(CHAR_DATA *ch,char *argument)
{
  if ( !(is_clan(ch) || IS_ADMIN(ch) || IS_SET(ch->olcs,OLC_CLANADMIN)) )
  {
	send_to_char( "You must be in a clan, a Clan Administrator or a Mud Administrator to use Clan Notes.\n\r", ch );
	return;
  }

  parse_note(ch,argument,NOTE_CLANNOTE);
  return;
}

void save_notes(int type)
{
    FILE *fp;
    char *name;
    NOTE_DATA *pnote;

    switch (type)
    {
	default:
	    return;
	case NOTE_NOTE:
	    name = NOTE_FILE;
	    pnote = note_list;
	    break;
	case NOTE_IDEA:
	    name = IDEA_FILE;
	    pnote = idea_list;
	    break;
	case NOTE_PENALTY:
	    name = PENALTY_FILE;
	    pnote = penalty_list;
	    break;
	case NOTE_NEWS:
	    name = NEWS_FILE;
	    pnote = news_list;
	    break;
	case NOTE_CHANGES:
	    name = CHANGES_FILE;
	    pnote = changes_list;
	    break;
	case NOTE_RPNOTE:
	    name = RPNOTE_FILE;
	    pnote = rpnote_list;
	    break;
	case NOTE_IMMNOTE:
	    name = IMMNOTE_FILE;
	    pnote = immnote_list;
	    break;
	case NOTE_CLANNOTE:
	    name = CLANNOTE_FILE;
	    pnote = clannote_list;
	    break;
    }

    fclose( fpReserve );
    if ( ( fp = fopen( name, "w" ) ) == NULL )
    {
	perror( name );
    }
    else
    {
	for ( ; pnote != NULL; pnote = pnote->next )
	{
	    fprintf( fp, "Sender  %s~\n", pnote->sender);
	    fprintf( fp, "Date    %s~\n", pnote->date);
	    fprintf( fp, "Stamp   %ld\n", pnote->date_stamp);
	    fprintf( fp, "To      %s~\n", pnote->to_list);
	    fprintf( fp, "Subject %s~\n", pnote->subject);
	    fprintf( fp, "Text\n%s~\n",   pnote->text);
	}
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
   	return;
    }
}

void load_notes(void)
{
 /*
  * We want all Notes and Ideas loaded
  *
  * load_thread(NOTE_FILE,&note_list, NOTE_NOTE, 14*24*60*60);
  * load_thread(IDEA_FILE,&idea_list, NOTE_IDEA, 28*24*60*60);
  */
    log_string( "!-- Load Notes: Note File ------------------------------!" );
    load_thread( NOTE_FILE,     &note_list,      NOTE_NOTE,     0 );
    log_string( "!-- Load Notes: Idea File ------------------------------!" );
    load_thread( IDEA_FILE,     &idea_list,      NOTE_IDEA,     0 );
    log_string( "!-- Load Notes: Penalty File ---------------------------!" );
    load_thread( PENALTY_FILE,  &penalty_list,   NOTE_PENALTY,  0 );
    log_string( "!-- Load Notes: News File ------------------------------!" );
    load_thread( NEWS_FILE,     &news_list,      NOTE_NEWS,     0 );
    log_string( "!-- Load Notes: Changes File ---------------------------!" );
    load_thread( CHANGES_FILE,  &changes_list,   NOTE_CHANGES,  0 );
    log_string( "!-- Load Notes: RP Note File ---------------------------!" );
    load_thread( RPNOTE_FILE,   &rpnote_list,    NOTE_RPNOTE,   0 );
    log_string( "!-- Load Notes: Clan Note File -------------------------!" );
    load_thread( CLANNOTE_FILE, &clannote_list,  NOTE_CLANNOTE, 0 );
    log_string( "!-- Load Notes: Imm Note File --------------------------!" );
    load_thread( IMMNOTE_FILE,  &immnote_list,   NOTE_IMMNOTE,  0 );
}

void load_thread(char *name, NOTE_DATA **list, int type, time_t free_time)
{
    FILE *fp;
    NOTE_DATA *pnotelast;
 
    if ( ( fp = fopen( name, "r" ) ) == NULL )
	return;
	 
    pnotelast = NULL;
    for ( ; ; )
    {
	NOTE_DATA *pnote;
	char letter;
	 
	do
	{
	    letter = getc( fp );
            if ( feof(fp) )
            {
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        pnote           = alloc_perm( sizeof(*pnote) );
 
        if ( str_cmp( fread_word( fp ), "sender" ) )
            break;
        pnote->sender   = fread_string( fp );
 
        if ( str_cmp( fread_word( fp ), "date" ) )
            break;
        pnote->date     = fread_string( fp );
 
        if ( str_cmp( fread_word( fp ), "stamp" ) )
            break;
        pnote->date_stamp = fread_number(fp);
 
        if ( str_cmp( fread_word( fp ), "to" ) )
            break;
        pnote->to_list  = fread_string( fp );
 
        if ( str_cmp( fread_word( fp ), "subject" ) )
            break;
        pnote->subject  = fread_string( fp );
 
        if ( str_cmp( fread_word( fp ), "text" ) )
            break;
        pnote->text     = fread_string( fp );
 
        if (free_time && pnote->date_stamp < current_time - free_time)
        {
	    free_note(pnote);
          continue;
        }

	pnote->type = type;
 
        if (*list == NULL)
            *list           = pnote;
        else
            pnotelast->next     = pnote;
 
        pnotelast       = pnote;
    }
 
    strcpy( strArea, NOTE_FILE );
    fpArea = fp;
    bug( "Load_notes: bad key word.", 0 );
    exit( 1 );
    return;
}

void append_note(NOTE_DATA *pnote)
{
    FILE *fp;
    char *name;
    NOTE_DATA **list;
    NOTE_DATA *last;

    switch(pnote->type)
    {
	default:
	    return;
	case NOTE_NOTE:
	    name = NOTE_FILE;
	    list = &note_list;
	    break;
	case NOTE_IDEA:
	    name = IDEA_FILE;
	    list = &idea_list;
	    break;
	case NOTE_PENALTY:
	    name = PENALTY_FILE;
	    list = &penalty_list;
	    break;
	case NOTE_NEWS:
	     name = NEWS_FILE;
	     list = &news_list;
	     break;
	case NOTE_CHANGES:
	     name = CHANGES_FILE;
	     list = &changes_list;
	     break;
	case NOTE_RPNOTE:
	     name = RPNOTE_FILE;
	     list = &rpnote_list;
	     break;
	case NOTE_IMMNOTE:
	     name = IMMNOTE_FILE;
	     list = &immnote_list;
	     break;
	case NOTE_CLANNOTE:
	     name = CLANNOTE_FILE;
	     list = &clannote_list;
	     break;
    }

    if (*list == NULL)
	*list = pnote;
    else
    {
	for ( last = *list; last->next != NULL; last = last->next);
	last->next = pnote;
    }

    fclose(fpReserve);
    if ( ( fp = fopen(name, "a" ) ) == NULL )
    {
        perror(name);
    }
    else
    {
        fprintf( fp, "Sender  %s~\n", pnote->sender);
        fprintf( fp, "Date    %s~\n", pnote->date);
        fprintf( fp, "Stamp   %ld\n", pnote->date_stamp);
        fprintf( fp, "To      %s~\n", pnote->to_list);
        fprintf( fp, "Subject %s~\n", pnote->subject);
        fprintf( fp, "Text\n%s~\n", pnote->text);
        fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
}

bool is_note_to( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    if ( !str_cmp( ch->name, pnote->sender ) )
	return TRUE;

    if ( is_name( "all", pnote->to_list ) )
	return TRUE;

    if ( get_trust(ch) >= SUPREME && is_name( "admin", pnote->to_list ) )
	return TRUE;

    if ( IS_HERO(ch) && is_name( "immortal", pnote->to_list ) )
	return TRUE;

    if ( IS_HERO(ch) && is_name( "imm", pnote->to_list ) )
	return TRUE;

    if ( ( IS_SET(ch->act, PLR_CLANLEADER) || get_trust(ch) >= SUPREME ) && ( is_name( "clanleaders", pnote->to_list ) || is_name( "clanleader", pnote->to_list ) ) )
	return TRUE;

    if ( ( IS_SET(ch->olcs, OLC_CLANADMIN) || get_trust(ch) >= SUPREME ) && ( is_name( "clanadministrators", pnote->to_list ) || is_name( "clanadmin", pnote->to_list ) ) )
	return TRUE;
    
    if ( ( IS_HERO(ch) || IS_SET(ch->act, PLR_HELPER) ) && is_name( "helper", pnote->to_list ) )
	return TRUE;

    if ( IS_BUILDER(ch) && is_name( "builder", pnote->to_list ) )
	return TRUE;

    if ( is_coder(ch) && (is_name( "coders", pnote->to_list ) || is_name( "coder", pnote->to_list )) )	/* Storm */
	return TRUE;															/* Storm */

    if (ch->clan && is_name(clan_table[ch->clan].name,pnote->to_list))
	return TRUE;

    if ( is_name( ch->name, pnote->to_list ) )
	return TRUE;

    if ( ch->level == MAX_LEVEL && get_trust(ch) == MAX_LEVEL )
	return TRUE;

    return FALSE;
}



void note_attach( CHAR_DATA *ch, int type )
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    pnote = new_note();

    pnote->next		= NULL;
    pnote->sender		= str_dup( ch->name );
    pnote->date		= str_dup( "" );
    pnote->to_list	= str_dup( "" );
    pnote->subject	= str_dup( "" );
    pnote->text		= str_dup( "" );
    pnote->type		= type;
    ch->pnote		= pnote;
    return;
}



void note_remove( CHAR_DATA *ch, NOTE_DATA *pnote, bool delete)
{
    char to_new[MAX_INPUT_LENGTH];
    char to_one[MAX_INPUT_LENGTH];
    NOTE_DATA *prev;
    NOTE_DATA **list;
    char *to_list;

    /* Just a simple recipient removal */
    if (!delete)
    {
	/* make a new to_list */
        to_new[0]	= '\0';
        to_list	= pnote->to_list;
        while ( *to_list != '\0' )
        {
    	    to_list	= one_argument( to_list, to_one );
    	    if ( to_one[0] != '\0' && str_cmp( ch->name, to_one ) )
	    {
	        strcat( to_new, " " );
	        strcat( to_new, to_one );
	    }
        }

       if ( str_cmp( ch->name, pnote->sender ) && to_new[0] != '\0' )
       {
	   free_string( pnote->to_list );
	   pnote->to_list = str_dup( to_new + 1 );
	   return;
       }
    }

    /* nuke the whole note */

    switch(pnote->type)
    {
	default:
	    return;
	case NOTE_NOTE:
	    list = &note_list;
	    break;
	case NOTE_IDEA:
	    list = &idea_list;
	    break;
	case NOTE_PENALTY:
	    list = &penalty_list;
	    break;
	case NOTE_NEWS:
	    list = &news_list;
	    break;
	case NOTE_CHANGES:
	    list = &changes_list;
	    break;
	case NOTE_RPNOTE:
	    list = &rpnote_list;
	    break;
	case NOTE_CLANNOTE:
	    list = &clannote_list;
	    break;
	case NOTE_IMMNOTE:
	    list = &immnote_list;
	    break;
    }

    /*
     * Remove note from linked list.
     */
    if ( pnote == *list )
    {
	*list = pnote->next;
    }
    else
    {
	for ( prev = *list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == pnote )
		break;
	}

	if ( prev == NULL )
	{
	    bug( "Note_remove: pnote not found.", 0 );
	    return;
	}

	prev->next = pnote->next;
    }

    save_notes(pnote->type);
    free_note(pnote);
    return;
}

bool hide_note (CHAR_DATA *ch, NOTE_DATA *pnote)
{
    time_t last_read;

    if (IS_NPC(ch))
	return TRUE;

    switch (pnote->type)
    {
	default:
	    return TRUE;
	case NOTE_NOTE:
	    last_read = ch->pcdata->last_note;
	    break;
	case NOTE_IDEA:
	    last_read = ch->pcdata->last_idea;
	    break;
	case NOTE_PENALTY:
	    last_read = ch->pcdata->last_penalty;
	    break;
	case NOTE_NEWS:
	    last_read = ch->pcdata->last_news;
	    break;
	case NOTE_CHANGES:
	    last_read = ch->pcdata->last_changes;
	    break;
	case NOTE_RPNOTE:
	    last_read = ch->pcdata->last_rpnote;
	    break;
	case NOTE_CLANNOTE:
	    last_read = ch->pcdata->last_clannote;
	    break;
	case NOTE_IMMNOTE:
	    last_read = ch->pcdata->last_immnote;
	    break;
    }
    
    if (pnote->date_stamp <= last_read)
	return TRUE;

    if (!str_cmp(ch->name,pnote->sender))
	return TRUE;

    if (!is_note_to(ch,pnote))
	return TRUE;

    return FALSE;
}

void update_read(CHAR_DATA *ch, NOTE_DATA *pnote)
{
    time_t stamp;

    if (IS_NPC(ch))
	return;

    stamp = pnote->date_stamp;

    switch (pnote->type)
    {
        default:
            return;
        case NOTE_NOTE:
	    ch->pcdata->last_note = UMAX(ch->pcdata->last_note,stamp);
            break;
        case NOTE_IDEA:
	    ch->pcdata->last_idea = UMAX(ch->pcdata->last_idea,stamp);
            break;
        case NOTE_PENALTY:
	    ch->pcdata->last_penalty = UMAX(ch->pcdata->last_penalty,stamp);
            break;
        case NOTE_NEWS:
	    ch->pcdata->last_news = UMAX(ch->pcdata->last_news,stamp);
            break;
        case NOTE_CHANGES:
	    ch->pcdata->last_changes = UMAX(ch->pcdata->last_changes,stamp);
            break;
        case NOTE_RPNOTE:
	    ch->pcdata->last_rpnote = UMAX(ch->pcdata->last_rpnote,stamp);
            break;
        case NOTE_CLANNOTE:
	    ch->pcdata->last_clannote = UMAX(ch->pcdata->last_clannote,stamp);
            break;
        case NOTE_IMMNOTE:
	    ch->pcdata->last_immnote = UMAX(ch->pcdata->last_immnote,stamp);
            break;
    }
}

void parse_note( CHAR_DATA *ch, char *argument, int type )
{
    BUFFER *buffer;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    NOTE_DATA *pnote;
    NOTE_DATA **list;
    char *list_name;
    int vnum;
    int anum;
    DESCRIPTOR_DATA *d;
    extern bool betalock;
    extern bool spool_lock;

    if ( IS_NPC(ch) )
	return;

    switch(type)
    {
	default:
	    return;
        case NOTE_NOTE:
            list = &note_list;
	    list_name = "notes";
            break;
        case NOTE_IDEA:
            list = &idea_list;
	    list_name = "ideas";
            break;
        case NOTE_PENALTY:
            list = &penalty_list;
	    list_name = "penalties";
            break;
        case NOTE_NEWS:
            list = &news_list;
	    list_name = "news";
            break;
        case NOTE_CHANGES:
            list = &changes_list;
	    list_name = "changes";
            break;
        case NOTE_CLANNOTE:
            list = &clannote_list;
	    list_name = "clannote";
            break;
        case NOTE_RPNOTE:
            list = &rpnote_list;
	    list_name = "rpnote";
            break;
        case NOTE_IMMNOTE:
            list = &immnote_list;
	    list_name = "immnote";
            break;
    	}

    argument = one_argument( argument, arg );
    smash_tilde( argument );

    if ( arg[0] == '\0' || !str_prefix( arg, "read" ) )
    {
        bool fAll;
 
        if ( !str_cmp( argument, "all" ) )
        {
            fAll = TRUE;
            anum = 0;
        }
 
        else if ( argument[0] == '\0' || !str_prefix(argument, "next"))
        /* read next unread note */
        {
		if ( msgspoollock( ch ) )
			return;

            vnum = 0;
		spool_lock	= TRUE;
            for ( pnote = *list; pnote != NULL; pnote = pnote->next)
            {
                if (!hide_note(ch,pnote))
                {
			  sprintf( buf, "{c[{C%4d{c] {W%s\n\r{yFrom:    {W%s{w\n\r{cTo:      {W%s\n\r{gSubject: {W%s\n\r{w",
                        vnum,
				pnote->date,
                        pnote->sender,
				pnote->to_list,
                        pnote->subject);
			  spool_lock	= FALSE;
                    send_to_char( buf, ch );
                    page_to_char( pnote->text, ch );
                    update_read(ch,pnote);
                    return;
                }
                else if (is_note_to(ch,pnote))
                    vnum++;

		    spool_lock	= TRUE;
            }
		spool_lock	= FALSE;
		sprintf(buf,"You have no unread %s.\n\r",list_name);
		send_to_char(buf,ch);
            return;
        }
 
        else if ( is_number( argument ) )
        {
            fAll = FALSE;
            anum = atoi( argument );
        }
        else
        {
            send_to_char( "Read which number?\n\r", ch );
            return;
        }
 
	  if ( msgspoollock( ch ) )
			return;
        vnum = 0;
	  spool_lock	= TRUE;
        for ( pnote = *list; pnote != NULL; pnote = pnote->next )
        {
            if ( is_note_to( ch, pnote ) && ( vnum++ == anum || fAll ) )
            {
			  sprintf( buf, "{c[{C%4d{c] {W%s\n\r{yFrom:    {W%s{w\n\r{cTo:      {W%s\n\r{gSubject: {W%s\n\r{w",
                        vnum -1,
				pnote->date,
                        pnote->sender,
				pnote->to_list,
                        pnote->subject);
		    spool_lock	= FALSE;
                send_to_char( buf, ch );
                page_to_char( pnote->text, ch );
		    update_read(ch,pnote);
                return;
            }

		spool_lock	= TRUE;
        }
 	  spool_lock	= FALSE;
	  sprintf(buf,"There aren't that many %s.\n\r",list_name);
	  send_to_char(buf,ch);
        return;
    }

    if ( !str_prefix( arg, "list" ) )
    {
	int line_count;
	bool showline;

	if ( msgspoollock( ch ) )
		return;

	line_count = 0;

	if ( !ch->desc->iswait )
	{
		if (IS_HERO(ch))
		{
			send_to_char("{w\n\r         Sender        Recipients                      Subject\n\r", ch);
			send_to_char("         ------------  ------------------------------  -------\n\r", ch);
		}
		else
		{
			send_to_char("{w\n\r         Sender        Subject\n\r", ch);
			send_to_char("         ------------  -------\n\r", ch);
		}
		line_count = 2;
	}

	vnum = 0;
	spool_lock	= TRUE;
	for ( pnote = *list; pnote != NULL; pnote = pnote->next )
	{
		if ( is_note_to( ch, pnote ) )
		{
			showline = TRUE;
			if ( ch->desc->iswait )
			{
				if ( vnum < ch->desc->swcount )
					showline = FALSE;
				else
					showline = TRUE;
			}

			/* Immortals */
			if ( IS_HERO(ch) && showline ) 
			{
				if (!hide_note(ch,pnote))
				{
					sprintf( buf, "{D[{g%4dN{D]  {y%-12s  {c%-30s  {w%s{w\n\r",
					   vnum, pnote->sender, pnote->to_list, pnote->subject );
				}
				else
				{
					sprintf( buf, "{D[{w%4d {D]  {y%-12s  {c%-30s  {w%s{w\n\r",
					   vnum, pnote->sender, pnote->to_list, pnote->subject );
				}
				line_count++;
				send_to_char( buf, ch );
			}

			/* Mortals */
			if ( !IS_HERO(ch) && showline )
			{
				if (!hide_note(ch,pnote))
				{
					sprintf( buf, "{D[{g%4dN{D]  {y%-12s  {w%s{w\n\r",
					   vnum, pnote->sender, pnote->subject );
				}
				else
				{
					sprintf( buf, "{D[{w%4d {D]  {c%-12s  {w%s{w\n\r",
					   vnum, pnote->sender, pnote->subject );
				}
				line_count++;
				send_to_char( buf, ch );
			}

			vnum++;
			if ( line_count == (ch->lines - 2) )
			{
				switch(type)
				{
					default:
					   return;
					case NOTE_NOTE:
					   scroll_wait( ch, "note", "list", vnum, TRUE );
					   break;
					case NOTE_IDEA:
					   scroll_wait( ch, "idea", "list", vnum, TRUE );
					   break;
					case NOTE_PENALTY:
					   scroll_wait( ch, "penalty", "list", vnum, TRUE );
					   break;
					case NOTE_NEWS:
					   scroll_wait( ch, "news", "list", vnum, TRUE );
					   break;
					case NOTE_CHANGES:
					   scroll_wait( ch, "change", "list", vnum, TRUE );
					   break;
					case NOTE_CLANNOTE:
					   scroll_wait( ch, "clannote", "list", vnum, TRUE );
					   break;
					case NOTE_RPNOTE:
					   scroll_wait( ch, "rpnote", "list", vnum, TRUE );
					   break;
					case NOTE_IMMNOTE:
					   scroll_wait( ch, "immnote", "list", vnum, TRUE );
					   break;
				}
				spool_lock	= FALSE;
				return;
			}
		}
	}
	spool_lock	= FALSE;
	if (!vnum)
	{
	    switch(type)
	    {
		case NOTE_NOTE:	
		    send_to_char("There are no Notes for you.\n\r",ch);
		    break;
		case NOTE_IDEA:
		    send_to_char("There are no Ideas for you.\n\r",ch);
		    break;
		case NOTE_PENALTY:
		    send_to_char("There are no Penalties for you.\n\r",ch);
		    break;
		case NOTE_NEWS:
		    send_to_char("There is no News for you.\n\r",ch);
		    break;
		case NOTE_CHANGES:
		    send_to_char("There are no Changes for you.\n\r",ch);
		    break;
		case NOTE_IMMNOTE:
		    send_to_char("There are no IMM Notes for you.\n\r",ch);
		    break;
		case NOTE_RPNOTE:
		    send_to_char("There are no RP Notes for you.\n\r",ch);
		    break;
		case NOTE_CLANNOTE:
		    send_to_char("There are no Clan Notes for you.\n\r",ch);
		    break;
	    }
	}
	scroll_wait( ch, "", "", 0, FALSE );
	return;
    }

    if ( !str_prefix( arg, "remove" ) )
    {

	  if (betalock)
	  {
	    send_to_char("The game is betalocked.\n\rYou may not remove notes in this mode.\n\r", ch);
	    return;
	  }

        if ( !is_number( argument ) )
        {
            send_to_char( "Note remove which number?\n\r", ch );
            return;
        }
 
	  if ( msgspoollock( ch ) )
		return;

        anum = atoi( argument );
        vnum = 0;
	  spool_lock	= TRUE;
        for ( pnote = *list; pnote != NULL; pnote = pnote->next )
        {
            if ( is_note_to( ch, pnote ) && vnum++ == anum )
            {
                note_remove( ch, pnote, FALSE );
                send_to_char( "Ok.\n\r", ch );
		    spool_lock	= FALSE;
                return;
            }
		spool_lock	= TRUE;
        }
 	  spool_lock	= FALSE;
	  sprintf(buf,"There aren't that many %s.",list_name);
	  send_to_char(buf,ch);
	  return;
    }
 
    if ( !str_prefix( arg, "delete" ) )
    {
	  if ( (get_trust(ch) < MAX_LEVEL) || (ch->level < MAX_LEVEL) )
	  {
		send_to_char( "You can't do that.\n\r", ch );
		return;
	  }

	  if (betalock)
	  {
	    send_to_char("The game is betalocked.\n\rYou may not delete notes in this mode.\n\r", ch);
	    return;
	  }

        if ( !is_number( argument ) )
        {
            send_to_char( "Note delete which number?\n\r", ch );
            return;
        }
 
	  if ( msgspoollock( ch ) )
		return;

        anum = atoi( argument );
        vnum = 0;
	  spool_lock	= TRUE;
        for ( pnote = *list; pnote != NULL; pnote = pnote->next )
        {
            if ( vnum++ == anum )
            {
                note_remove( ch, pnote,TRUE );
                send_to_char( "Ok.\n\r", ch );
		    spool_lock	= FALSE;
                return;
            }
		spool_lock	= TRUE;
        }
	  spool_lock	= FALSE;
	  sprintf(buf,"There aren't that many %s.",list_name);
	  send_to_char(buf,ch);
        return;
    }

    if (!str_prefix(arg,"catchup"))
    {
	switch(type)
	{
	    case NOTE_NOTE:	
		ch->pcdata->last_note = current_time;
		break;
	    case NOTE_IDEA:
		ch->pcdata->last_idea = current_time;
		break;
	    case NOTE_PENALTY:
		ch->pcdata->last_penalty = current_time;
		break;
	    case NOTE_NEWS:
		ch->pcdata->last_news = current_time;
		break;
	    case NOTE_CHANGES:
		ch->pcdata->last_changes = current_time;
		break;
	    case NOTE_RPNOTE:
		ch->pcdata->last_rpnote = current_time;
		break;
	    case NOTE_IMMNOTE:
		ch->pcdata->last_immnote = current_time;
		break;
	    case NOTE_CLANNOTE:
		ch->pcdata->last_clannote = current_time;
		break;
	}
	return;
    }

    /* below this point only certain people can edit notes */
    if ((type == NOTE_NEWS && !IS_TRUSTED(ch,LEVEL_HERO))
    ||  (type == NOTE_CHANGES && !IS_TRUSTED(ch,SUPREME))
    ||  (type == NOTE_IMMNOTE && !IS_TRUSTED(ch,LEVEL_HERO))
    ||  (type == NOTE_PENALTY && !IS_TRUSTED(ch,LEVEL_HERO)))
    {
	sprintf(buf,"You aren't high enough level to write %s.\n\r",list_name);
	send_to_char(buf,ch);
	return;
    }

    if ((type == NOTE_CLANNOTE) && !(is_clan(ch) || IS_ADMIN(ch) || IS_SET(ch->olcs,OLC_CLANADMIN)))
    {
	send_to_char("You must be in a clan, a clan administrator, or a mud administrator to write a clan note.\n\r", ch);
	return;
    }

    if ( !str_cmp( arg, "format" ) )
    {
	note_attach( ch,type );
	if (ch->pnote->type != type)
	{
	    send_to_char(
		"You already have a different note in progress.\n\r",ch);
	    return;
	}

	if (strlen(ch->pnote->text) >= 4096)
	{
	    send_to_char( "Note too long.\n\r", ch );
	    return;
	}

	ch->pnote->text = format_string(ch->pnote->text);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "replace" ) )
    {
	note_attach( ch,type );
	if (ch->pnote->type != type)
	{
	    send_to_char(
		"You already have a different note in progress.\n\r",ch);
	    return;
	}

	if (strlen(ch->pnote->text) >= 4096)
	{
	    send_to_char( "Note too long.\n\r", ch );
	    return;
	}

	smash_tilde( argument );
	argument = one_argument( argument, arg2 );
	smash_tilde( argument );

	ch->pnote->text = string_replace(ch->pnote->text, arg2, argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "+" ) )
    {
	note_attach( ch,type );
	if (ch->pnote->type != type)
	{
	    send_to_char(
		"You already have a different note in progress.\n\r",ch);
	    return;
	}

	if (strlen(ch->pnote->text)+strlen(argument) >= 4096)
	{
	    send_to_char( "Note too long.\n\r", ch );
	    return;
	}

 	buffer = new_buf();

	add_buf(buffer,ch->pnote->text);
	add_buf(buffer,argument);
	add_buf(buffer,"\n\r");
	free_string( ch->pnote->text );
	ch->pnote->text = str_dup( buf_string(buffer) );
	free_buf(buffer);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if (!str_cmp(arg,"-"))
    {
 	int len;
	bool found = FALSE;

	note_attach(ch,type);
        if (ch->pnote->type != type)
        {
            send_to_char(
                "You already have a different note in progress.\n\r",ch);
            return;
        }

	if (ch->pnote->text == NULL || ch->pnote->text[0] == '\0')
	{
	    send_to_char("No lines left to remove.\n\r",ch);
	    return;
	}

	strcpy(buf,ch->pnote->text);

	for (len = strlen(buf); len > 0; len--)
 	{
	    if (buf[len] == '\r')
	    {
		if (!found)  /* back it up */
		{
		    if (len > 0)
			len--;
		    found = TRUE;
		}
		else /* found the second one */
		{
		    buf[len + 1] = '\0';
		    free_string(ch->pnote->text);
		    ch->pnote->text = str_dup(buf);
		    return;
		}
	    }
	}
	buf[0] = '\0';
	free_string(ch->pnote->text);
	ch->pnote->text = str_dup(buf);
	return;
    }

    if ( !str_prefix( arg, "subject" ) )
    {
	note_attach( ch,type );
        if (ch->pnote->type != type)
        {
            send_to_char(
                "You already have a different note in progress.\n\r",ch);
            return;
        }

	free_string( ch->pnote->subject );
	ch->pnote->subject = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "to" ) )
    {
	note_attach( ch,type );
        if (ch->pnote->type != type)
        {
            send_to_char(
                "You already have a different note in progress.\n\r",ch);
            return;
        }
	free_string( ch->pnote->to_list );
	ch->pnote->to_list = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "clear" ) )
    {
	if ( ch->pnote != NULL )
	{
	    free_note(ch->pnote);
	    ch->pnote = NULL;
	}

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "show" ) )
    {
	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

	if (ch->pnote->type != type)
	{
	    send_to_char("You aren't working on that kind of note.\n\r",ch);
	    return;
	}

	sprintf( buf, "{w%s{w: %s{w\n\rTo: %s{w\n\r",
	    ch->pnote->sender,
	    ch->pnote->subject,
	    ch->pnote->to_list
	    );
	send_to_char( buf, ch );
	send_to_char( ch->pnote->text, ch );
	return;
    }

    if ( !str_prefix( arg, "post" ) || !str_prefix(arg, "send") || !str_prefix(arg, "staff") )
    {
	char *strtime;

	if (betalock)
	{
	  send_to_char("The game is betalocked.\n\rYou may not post notes in this mode.\n\r", ch);
	  return;
	}

	if ( IS_SET(ch->comm,COMM_NOCHANNELS) )
	{
	  send_to_char("The gods have revoked your capacity to post messages.\n\r",ch);
	  return;
	}

	if (!str_prefix(arg, "staff") && ch->level < SUPREME )
	{
		send_to_char( "You can't do that.\n\r", ch );
		return;
	}

	if ( msgspoollock( ch ) )
		return;

	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

        if (ch->pnote->type != type)
        {
            send_to_char("You aren't working on that kind of note.\n\r",ch);
            return;
        }

	if (!str_cmp(ch->pnote->to_list,""))
	{
	    send_to_char(
		"You need to provide a recipient (name, all, immortal, clan, clanleaders, clanadministrators, builder, helper, or admin).\n\r",
		ch);
	    return;
	}

	if (!str_cmp(ch->pnote->subject,""))
	{
	    send_to_char("You need to provide a subject.\n\r",ch);
	    return;
	}

	if (!str_cmp(arg, "staff"))
	{
		ch->pnote->sender		= "Immortal";

	 	buffer = new_buf();

		add_buf(buffer,ch->pnote->text);
		add_buf(buffer,"\n\r\n\r{cThe Immortal Staff{w\n\r");
		free_string( ch->pnote->text );
		ch->pnote->text = str_dup( buf_string(buffer) );
		free_buf(buffer);
	}

	ch->pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	ch->pnote->date			= str_dup( strtime );
	ch->pnote->date_stamp		= current_time;

	spool_lock	= TRUE;
	append_note(ch->pnote);
	spool_lock	= FALSE;

	/* Notify recipients of a new note */
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING 
		   && is_note_to( d->character, ch->pnote ) 
		   && d->character->fighting == NULL 
		   && !IS_SET(d->character->info,INFO_NOINFO) 
		   && can_see(d->character, ch) )
	    {
		if ( ch->pnote->type == NOTE_NOTE )
		{
		    sprintf( buf, "{RINFO: {WNew Note from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( ch->pnote->type == NOTE_IDEA )
		{
		    sprintf( buf, "{RINFO: {WNew Idea from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( (ch->pnote->type == NOTE_PENALTY) && IS_HERO(d->character))
		{
		    sprintf( buf, "{RINFO: {WNew Penalty from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( ch->pnote->type == NOTE_NEWS )
		{
		    sprintf( buf, "{RINFO: {WNew News from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( ch->pnote->type == NOTE_CHANGES )
		{
		    sprintf( buf, "{RINFO: {WNew Changes from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( (ch->pnote->type == NOTE_IMMNOTE) && IS_HERO(d->character))
		{
		    sprintf( buf, "{RINFO: {WNew Imm Note from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( ch->pnote->type == NOTE_RPNOTE )
		{
		    sprintf( buf, "{RINFO: {WNew RP Note from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

		if ( (ch->pnote->type == NOTE_CLANNOTE) && (is_clan(ch) || IS_ADMIN(ch) || IS_SET(ch->olcs,OLC_CLANADMIN)))
		{
		    sprintf( buf, "{RINFO: {WNew Clan Note from {r%s{W regarding {b%s {*{w\n\r", 
		    ch->pnote->sender,
		    ch->pnote->subject );
		    send_to_char( buf, d->character );
		}

	    }
	}
	ch->pnote = NULL;
	return;
    }

    send_to_char( "You can't do that.\n\r", ch );
    return;
}

