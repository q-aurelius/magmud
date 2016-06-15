/*
 *  This Code created by Matthew Peck, aka Valatar
 *              sauron@digital-genesis.com
 *
 *  Created so that I wouldn't have to make a CGI script that would update
 *  the page everytime you check.  All you have to do to use this is email 
 *  me and let me know that you are.  Include your mud address and webpage 
 *  if you don't mind.  You have to know a bit of HTML to 
 *  make the changes necessary for this to work for your mud, but other 
 *  than that, its all self contained.  This code is compatible with Lope's
 *  Colour codes, if you use some other system, you'll need to change 
 *  get_who_data to fit.  If there are any problems, email me and let me know.
 *
 *  To get this to work, save this as a .c file.  Then add a line in update.c
 *  that declares the function, and another that calls it.  I added it to 
 *  the pulse_music part, so it updates about every 5 seconds.  You can add
 *  it elsewhere if you like.  The declaration looks like this:
 *                  void    who_html_update args( ( void ) );
 *  Then the function call looks like this:
 *                  who_html_update();
 *  Update the makefile to include this code, then compile and you'll be all set.
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "merc.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
 
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif


void who_html_update (void)
{
  FILE *fp; 
  DESCRIPTOR_DATA *d;
  extern bool merc_down;
  extern int webwho_port;
  char webwho_file[MAX_STRING_LENGTH];
  char buf[MAX_INPUT_LENGTH];
  char buf2[MAX_INPUT_LENGTH];


  buf[0] = '\0';
  buf2[0] = '\0';
  
  if ( webwho_port == 2000 ) /*TEMP CHANGE 1-7-2008 TCH */
	sprintf( webwho_file, "/home/jml/magmud/magrathea/webwho/webwho.html" );
  else
	sprintf( webwho_file, "/home/jml/magmud/magrathea/webwho/webwho_%d.html", webwho_port );

  fclose(fpReserve);
  if ( (fp = fopen(webwho_file, "w") ) == NULL)
  {
     bug( "webwho.html: fopen", 0 );
     perror( "webwho.html" );
  }
  else
  {
	fprintf(fp, "<html>\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<title>\n");
	fprintf(fp, "Players currently on Magrathea port %d\n", webwho_port );
	fprintf(fp, "</title>\n");
	fprintf(fp, "<META HTTP-EQUIV=""refresh"" CONTENT=""60"">\n");
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body bgcolor=""silver"">\n");
	fprintf(fp, "<font size=""-2""><a href=""../menu_main.html""><i><b>Magrathea Main Menu</b></i></a></font><br><br>\n");
	fprintf(fp, "<h1><font color=""black"">\n");

	if ( webwho_port == 2000 )
		fprintf(fp, "Who's on Magrathea?\n");
	else
		fprintf(fp, "Who's on Magrathea (BETA)?\n");

	fprintf(fp, "</font></h1>\n");

	fprintf(fp, "<br>\n");
	fprintf(fp, "<table cellspacing=""2"" cellpadding=""2"" border=""0"">\n");
	fprintf(fp, "<tr>\n");
	fprintf(fp, "<td><font color=""blue"">[</td>\n");
	fprintf(fp, "<td><font color=""black""><b>Lvl</b></td>\n");
	fprintf(fp, "<td>&nbsp;</td>\n");
	fprintf(fp, "<td><font color=""black""><b>Race</b></td>\n");
	fprintf(fp, "<td>&nbsp;</td>\n");
	fprintf(fp, "<td><font color=""black""><b>Class</b></td>\n");
	fprintf(fp, "<td><font color=""blue"">]</td>\n");
	fprintf(fp, "<td>&nbsp;</td>\n");
	fprintf(fp, "<td><font color=""black""><b>Player Name</b></td>\n");
	fprintf(fp, "<td>&nbsp;</td>\n");
	fprintf(fp, "<td><font color=""black""><b>Clan</b></td>\n");
	fprintf(fp, "<td>&nbsp;</td>\n");
	fprintf(fp, "<td><font color=""black""><b>Player Flags</b></td>\n");
	fprintf(fp, "</tr>\n");
	fprintf(fp, "<tr><td colspan=""13""><font color=""blue"">_______________________________________________________________________</font></td></tr>\n");
  
	if ( merc_down )
	{
		fprintf(fp, "</table><br>\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<h2><font color=""red"" face=""courier"">\n");
		fprintf(fp, "Magrathea is currently off-line.<br>Please try back in a few minutes.\n");  
		fprintf(fp, "</font></h2>\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<hr>\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<font color=""black"" face=""Times New Roman"">\n");
		sprintf(buf, "This file last updated at %s Eastern Time.\n", ((char *) ctime( &current_time )));
		fprintf(fp, buf);
		fprintf(fp, "<br>\n");
		fprintf(fp, "This page is refreshed automatically every 60 seconds.\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<br>\n");
		fprintf(fp, "<hr>\n");
		fprintf(fp, "</body>\n");
		fprintf(fp, "</html>\n");
		fclose( fp ); 
		fpReserve = fopen( NULL_FILE, "r" );
		return;
	}

	for ( d = descriptor_list; d != NULL ; d = d->next )
	{
	  CHAR_DATA *wch;
	  char const *class;
	  bool show_char;
    
	  if ( d->connected == CON_PLAYING)
	  {
		wch   = ( d->original != NULL ) ? d->original : d->character;
		show_char	= TRUE;

		if ( is_linkdead(wch) )
			show_char	= FALSE;

		if ( is_ploaded(wch) )
			show_char	= FALSE;

		if ( IS_HERO(wch) && wch->invis_level != 0 )
			show_char	= FALSE;

		if ( IS_HERO(wch) && wch->incog_level != 0 )
			show_char	= FALSE;

		if ( IS_AFFECTED(wch, AFF_HIDE) )
			show_char	= FALSE;

		if ( IS_AFFECTED(wch, AFF_SNEAK) )
			show_char	= FALSE;

		if ( IS_AFFECTED(wch, AFF_INVISIBLE) )
			show_char	= FALSE;
			
		if (show_char)
		{
			class = class_table[wch->class].who_name;

			if ( wch->level >= LEVEL_HERO )
			{
				fprintf(fp, "<tr>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">[</font></td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					lev_imm(wch->level) );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					(wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name : "") );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					class );
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">]</font></td>\n");
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><a href=""mailto:%s""><font face=""courier"" color=""black"" size=""-2"">%s</font></a></td>\n", 
					wch->pcdata->email, 
					wch->name );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					(clan_table[wch->clan].visible ? clan_table[wch->clan].name : "") );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2""><b>[IMMORTAL]</b></font></td>\n");
				fprintf(fp, "</tr>\n");
			}
			else
			{
				fprintf(fp, "<tr>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">[</font></td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					lev_imm(wch->level) );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					(wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name : "") );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					class );
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">]</font></td>\n");
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><a href=""mailto:%s""><font face=""courier"" color=""black"" size=""-2"">%s</font></a></td>\n", 
					wch->pcdata->email, 
					wch->name );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">%s</font></td>\n", 
					(clan_table[wch->clan].visible ? clan_table[wch->clan].name : "") );
				fprintf(fp, "<td>&nbsp;</td>\n");
				fprintf(fp, "<td><font face=""courier"" color=""black"" size=""-2"">");
				sprintf(buf, "%s%s%s%s%s%s%s",
					IS_SET(wch->comm, COMM_AFK) ? "[AFK]&nbsp;" : "",
					IS_SET(wch->info, INFO_GHOST) ? "[GHOST]&nbsp;" : "",
					IS_SET(wch->act, PLR_WANTED) ? "[WANTED]&nbsp;" : "",
					IS_SET(wch->act, PLR_KILLER) ? "[KILLER]&nbsp;" : "",
					IS_SET(wch->act, PLR_THIEF)  ? "[THIEF]&nbsp;"  : "",
					IS_SET(wch->olcs, OLC_BUILDER)  ? "[BUILDER]&nbsp;"  : "",
					wch->pcdata->olc_on ? "[OLC]" : ""	);
				fprintf(fp, buf);
				fprintf(fp, "</font></td>\n");
				fprintf(fp, "</tr>\n");
			}

		} /* end if */
	  } /* end if */
	}     /* end for */

	fprintf(fp, "</table><br>\n");
	fprintf(fp, "</font>\n");
	fprintf(fp, "<p>\n"); 
	fprintf(fp, "<br>\n");
	fprintf(fp, "<br>\n");
	fprintf(fp, "<hr>\n");
	fprintf(fp, "<br>\n");
	fprintf(fp, "<br>\n");
	fprintf(fp, "<font color=""black"" face=""Times New Roman"">\n");
	sprintf(buf, "This file last updated at %s Eastern Time.\n", ((char *) ctime( &current_time )));
	fprintf(fp, buf);
	fprintf(fp, "<br>\n");
	fprintf(fp, "This page is refreshed automatically every 60 seconds.\n");

	fprintf(fp, "<br>\n");
	fprintf(fp, "<br>\n");
	fprintf(fp, "<hr>\n");
	fprintf(fp, "</p>\n");
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>\n");
	fclose( fp ); 
	fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */ 
  
  return;
}/* end function */


