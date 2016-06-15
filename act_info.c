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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

int flag_lookup args( ( const char *name, const struct flag_type *flag_table) );

/* command procedures needed */
DECLARE_DO_FUN( do_exits        );
DECLARE_DO_FUN( do_look         );
DECLARE_DO_FUN( do_help         );
DECLARE_DO_FUN( do_affects      );
DECLARE_DO_FUN( do_play         );
DECLARE_DO_FUN( do_say		  );
DECLARE_DO_FUN( do_yell         );
DECLARE_DO_FUN( do_flag         );

char *  const   where_name      [] =
{    "{G<{gused as light{G>     {w",
    "{G<{gworn on finger{G>    {w",
    "{G<{gworn on finger{G>    {w",
    "{G<{gworn around neck{G>  {w",
    "{G<{gworn around neck{G>  {w",
    "{G<{gworn on torso{G>     {w",
    "{G<{gworn on head{G>      {w",
    "{G<{gworn on legs{G>      {w",
    "{G<{gworn on feet{G>      {w",
    "{G<{gworn on hands{G>     {w",
    "{G<{gworn on arms{G>      {w",
    "{G<{gworn as shield{G>    {w",
    "{G<{gworn about body{G>   {w",
    "{G<{gworn about waist{G>  {w",
    "{G<{gworn around wrist{G> {w",
    "{G<{gworn around wrist{G> {w",
    "{G<{gwielded{G>           {w",
    "{G<{gheld{G>              {w",
    "{G<{gfloating nearby{G>   {w",
    "{G<{gsecondary weapon{G>  {w",
    "{C<{coptical implant{C>   {w",
    "{C<{ccomm socket{C>       {w",
    "{C<{cleg implant{C>       {w",
    "{C<{cchest implant{C>     {w",
    "{C<{creflex implant{C>    {w",
    "{C<{cprocessor socket{C>  {w",
    "{C<{cstrength implant{C>  {w",
};





/* for do_count */

int max_on = 0;







/*

 * Local functions.

 */

char *  lev_imm                 args( ( int level ) );

char *  lev_name                args( ( int level ) );

char *  lev_colname             args( ( int level ) );

int     lev_num                 args( ( CHAR_DATA *ch, char *argument ) );

char *  format_obj_to_char      args( ( OBJ_DATA *obj, CHAR_DATA *ch,

				    bool fShort ) );

void    show_list_to_char       args( ( OBJ_DATA *list, CHAR_DATA *ch,

				    bool fShort, bool fShowNothing ) );

void    show_char_to_char_0     args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );

void    show_char_to_char_1     args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );

void    show_char_to_char       args( ( CHAR_DATA *list, CHAR_DATA *ch ) );

bool    check_blind             args( ( CHAR_DATA *ch ) );





char *lev_imm( int level )

{

    char buf[MAX_STRING_LENGTH];



    if (level <=MAX_LEVEL && level >=LEVEL_HERO)

    {

	switch(level)

	{

	    case LEVEL_HERO:

		return "IMM";

	    case AVATAR:

		return "IMM";

	    case ANGEL:

		return "IMM";

	    case DEMI:

		return "IMM";

	    case IMMORTAL:

		return "IMM";

	    case GOD:

		return "IMM";

	    case DEITY:

		return "IMM";

	    case SUPREME:

		return "ADM";

	    case CREATOR:

		return "ADM";

	    case MAX_LEVEL:

		return "ADM";

	}

    }



    if (level<10)

	sprintf(buf, "  %d",level);

    else if (level<100)

	sprintf(buf, " %d",level);

    else if (level<1000)

	sprintf(buf, "%d",level);

    else

	sprintf(buf, "BOZO");



    return buf;

}





char *lev_colname( int level )

{

    char buf[MAX_STRING_LENGTH];



    if (level <=MAX_LEVEL && level >=LEVEL_HERO)

    {

	switch(level)

	{

	    case LEVEL_HERO:

		return "{mHER";

	    case AVATAR:

		return "{mAVA";

	    case ANGEL:

		return "{mANG";

	    case DEMI:

		return "{mDEM";

	    case IMMORTAL:

		return "{mIMM";

	    case GOD:

		return "{mGOD";

	    case DEITY:

		return "{mDEI";

	    case SUPREME:

		return "{mSUP";

	    case CREATOR:

		return "{mCRE";

	    case MAX_LEVEL:

		return "{mIMP";

	}

    }



    if (level<10)

	sprintf(buf, "  %d",level);

    else if (level<100)

	sprintf(buf, " %d",level);

    else if (level<1000)

	sprintf(buf, "%d",level);

    else

	sprintf(buf, "BOZO");



    return buf;

}





char *lev_name( int level )

{

    char buf[MAX_STRING_LENGTH];



    if (level <=MAX_LEVEL && level >=LEVEL_HERO)

    {

	switch(level)

	{

	    case LEVEL_HERO:

		return "HER";

	    case AVATAR:

		return "AVA";

	    case ANGEL:

		return "ANG";

	    case DEMI:

		return "DEM";

	    case IMMORTAL:

		return "IMM";

	    case GOD:

		return "GOD";

	    case DEITY:

		return "DEI";

	    case SUPREME:

		return "SUP";

	    case CREATOR:

		return "CRE";

	    case MAX_LEVEL:

		return "IMP";

	}

    }



    if (level<10)

	sprintf(buf, "  %d",level);

    else if (level<100)

	sprintf(buf, " %d",level);

    else if (level<1000)

	sprintf(buf, "%d",level);

    else

	sprintf(buf, "BOZO");



    return buf;

}



int lev_num ( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    int level;



    argument = one_argument( argument, arg1 );

    if ( is_number(arg1) )

	level = atoi(arg1);

    else

	level = -1;



    if (!str_cmp(arg1,"her"))

	level = LEVEL_HERO;

    if (!str_cmp(arg1,"ava"))

	level = AVATAR;

    if (!str_cmp(arg1,"ang"))

	level = ANGEL;

    if (!str_cmp(arg1,"dem"))

	level = DEMI;

    if (!str_cmp(arg1,"imm"))

	level = IMMORTAL;

    if (!str_cmp(arg1,"god"))

	level = GOD;

    if (!str_cmp(arg1,"dei"))

	level = DEITY;

    if (!str_cmp(arg1,"sup"))

	level = SUPREME;

    if (!str_cmp(arg1,"cre"))

	level = CREATOR;

    if (!str_cmp(arg1,"imp"))

	level = MAX_LEVEL;



    if (get_trust(ch)<level)

	return -1;



    return level;

}





char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )

{

    static char buf[MAX_STRING_LENGTH];



    buf[0] = '\0';



    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))

    ||  (obj->description == NULL || obj->description[0] == '\0'))

	return buf;



    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "{B({bInvis{B){w "     );

    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)

	 && IS_OBJ_STAT(obj, ITEM_EVIL)   )   strcat( buf, "{R({rRed Aura{R){w "  );

    if (IS_AFFECTED(ch, AFF_DETECT_GOOD)

    &&  IS_OBJ_STAT(obj,ITEM_BLESS))          strcat(buf,"{B({bBlue Aura{B){w " );

    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)

	 && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "{M({mMagical{M){w "   );

    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "{Y({yGlowing{Y){w "   );

    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "{C({cHumming{C){w "   );



    if ( fShort )

    {

	if ( obj->short_descr != NULL )

	    strcat( buf, obj->short_descr );

    }

    else

    {

	if ( obj->description != NULL)

	    strcat( buf, obj->description );

    }



    return buf;

}







/*

 * Show a list to a character.

 * Can coalesce duplicated items.

 */

void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )

{

    char buf[MAX_STRING_LENGTH];

    BUFFER *output;

    char **prgpstrShow;

    int *prgnShow;

    char *pstrShow;

    OBJ_DATA *obj;

    int nShow;

    int iShow;

    int count;

    bool fCombine;



    if ( ch->desc == NULL )

	return;



    /*

     * Alloc space for output lines.

     */

    output = new_buf();



    count = 0;

    for ( obj = list; obj != NULL; obj = obj->next_content )

	count++;

    prgpstrShow = alloc_mem( count * sizeof(char *) );

    prgnShow    = alloc_mem( count * sizeof(int)    );

    nShow       = 0;



    /*

     * Format the list of objects.

     */

    for ( obj = list; obj != NULL; obj = obj->next_content )

    {

	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ))

	{

	    pstrShow = format_obj_to_char( obj, ch, fShort );



	    fCombine = FALSE;



	    if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )

	    {

		/*

		 * Look for duplicates, case sensitive.

		 * Matches tend to be near end so run loop backwords.

		 */

		for ( iShow = nShow - 1; iShow >= 0; iShow-- )

		{

		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )

		    {

			prgnShow[iShow]++;

			fCombine = TRUE;

			break;

		    }

		}

	    }



	    /*

	     * Couldn't combine, or didn't want to.

	     */

	    if ( !fCombine )

	    {

		prgpstrShow [nShow] = str_dup( pstrShow );

		prgnShow    [nShow] = 1;

		nShow++;

	    }

	}

    }



    /*

     * Output the formatted list.

     */

    for ( iShow = 0; iShow < nShow; iShow++ )

    {

	if (prgpstrShow[iShow][0] == '\0')

	{

	    free_string(prgpstrShow[iShow]);

	    continue;

	}



	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )

	{

	    if ( prgnShow[iShow] != 1 )

	    {

		sprintf( buf, "(%2d) ", prgnShow[iShow] );

		add_buf(output,buf);

	    }

	    else

	    {

		add_buf(output,"     ");

	    }

	}

	add_buf(output,prgpstrShow[iShow]);

	add_buf(output,"\n\r");

	free_string( prgpstrShow[iShow] );

    }



    if ( fShowNothing && nShow == 0 )

    {

	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )

	    send_to_char( "     ", ch );

	send_to_char( "Nothing.\n\r", ch );

    }

    page_to_char(buf_string(output),ch);



    /*

     * Clean up.

     */

    free_buf(output);

    free_mem( prgpstrShow, count * sizeof(char *) );

    free_mem( prgnShow,    count * sizeof(int)    );



    return;

}







void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH],message[MAX_STRING_LENGTH];

    buf[0] = '\0';

    if ( IS_SET(victim->comm,COMM_AFK     )   ) strcat( buf, "{Y[{GAFK{Y]{w "              );
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "{B({bInvis{B){w "            );
    if ( victim->invis_level >= 1             ) strcat( buf, "{R({rWizi:"                  );
    if ( victim->invis_level >= 1             ) strcat( buf, lev_name(victim->invis_level) );
    if ( victim->invis_level >= 1             ) strcat( buf, "{R){w "                      );
    if ( IS_SET(victim->info, INFO_GHOST)     ) strcat( buf, "{w({WGHOST{w) "              );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "{D(Hide){w "                 );
    if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "{c({cCharmed{c){w "          );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "{C({cTranslucent{C){w "      );
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "{M({mPink Aura{M){w "        );
    if ( IS_EVIL(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "{R({rRed Aura{R){w "         );
    if ( IS_GOOD(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_GOOD)     ) strcat( buf, "{Y({yGolden Aura{Y){w "      );
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "{W({wWhite Aura{W){w "       );
    if ( !IS_NPC(victim)
    &&   ch->class == class_lookup("jedi")
    &&   victim->class == class_lookup("jedi")) strcat( buf, "{c({WThe Force{c){w "       );
    if ( !IS_NPC(victim)
    &&	 ch->class == class_lookup("jedi")
    &&   IS_AFFECTED2(victim, AFF_FORCE_ABSORB))strcat( buf, "{C({cBlue Aura{C){w "       );   
   // if ( ch->class == class_lookup("jedi")
   // &&   victim->class != class_lookup("jedi")
   // &&   victim->pcdata->jedi == 1            ) strcat( buf, "{w({WThe Force{w){w "       );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER ) )
						strcat( buf, "{R({rKILLER{R){w "     );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF  ) )
						strcat( buf, "{B({bTHIEF{B){w "      );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED  ) )
						strcat( buf, "{G({WWANTED{G){w "     );
    if ( victim->position == victim->start_pos && victim->long_descr[0] != '\0' )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	return;
    }



    strcat( buf, PERS( victim, ch ) );

    if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF)

    &&   victim->position == POS_STANDING && ch->on == NULL )

	strcat( buf, victim->pcdata->title );



    strcat( buf, "{w" );



    switch ( victim->position )

    {

    case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;

    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;

    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;

    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;

    case POS_SLEEPING:

	if (victim->on != NULL)

	{

	    if (IS_SET(victim->on->value[2],SLEEP_AT))

	    {

		sprintf(message," is sleeping at %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else if (IS_SET(victim->on->value[2],SLEEP_ON))

	    {

		sprintf(message," is sleeping on %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else

	    {

		sprintf(message, " is sleeping in %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	}

	else

	    strcat(buf," is sleeping here.");

	break;

    case POS_RESTING:

	if (victim->on != NULL)

	{

	    if (IS_SET(victim->on->value[2],REST_AT))

	    {

		sprintf(message," is resting at %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else if (IS_SET(victim->on->value[2],REST_ON))

	    {

		sprintf(message," is resting on %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else

	    {

		sprintf(message, " is resting in %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	}

	else

	    strcat( buf, " is resting here." );

	break;

    case POS_SITTING:

	if (victim->on != NULL)

	{

	    if (IS_SET(victim->on->value[2],SIT_AT))

	    {

		sprintf(message," is sitting at %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else if (IS_SET(victim->on->value[2],SIT_ON))

	    {

		sprintf(message," is sitting on %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else

	    {

		sprintf(message, " is sitting in %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	}

	else

	    strcat(buf, " is sitting here.");

	break;

    case POS_STANDING:

	if (victim->on != NULL)

	{

	    if (IS_SET(victim->on->value[2],STAND_AT))

	    {

		sprintf(message," is standing at %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	    else if (IS_SET(victim->on->value[2],STAND_ON))

	    {

		sprintf(message," is standing on %s.",

		   victim->on->short_descr);

		strcat(buf,message);

	    }

	    else

	    {

		sprintf(message," is standing in %s.",

		    victim->on->short_descr);

		strcat(buf,message);

	    }

	}

	else

	    strcat( buf, " is here." );

	break;

    case POS_FIGHTING:

	strcat( buf, " is here, fighting " );

	if ( victim->fighting == NULL )

	    strcat( buf, "thin air??" );

	else if ( victim->fighting == ch )

	    strcat( buf, "YOU!" );

	else if ( victim->in_room == victim->fighting->in_room )

	{

	    strcat( buf, PERS( victim->fighting, ch ) );

	    strcat( buf, "." );

	}

	else

	    strcat( buf, "someone who left??" );

	break;

    }



    strcat( buf, "\n\r" );

    buf[0] = UPPER(buf[0]);

    send_to_char( buf, ch );

    return;

}







void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )

{

    char buf[MAX_STRING_LENGTH];

    OBJ_DATA *obj;

    int iWear;

    int percent;

    bool found;



    if ( can_see( victim, ch ) )

    {

	if (ch == victim)

	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);

	else

	{

	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );

	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );

	}

    }



    if ( victim->description[0] != '\0' )

    {

	send_to_char( victim->description, ch );

    }

    else

    {

	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );

    }



    if ( victim->max_hit > 0 )

	percent = ( 100 * victim->hit ) / victim->max_hit;

    else

	percent = -1;



    strcpy( buf, PERS(victim, ch) );



    if (percent >= 100)

	strcat( buf, " is in excellent condition.\n\r");

    else if (percent >= 90)

	strcat( buf, " has a few scratches.\n\r");

    else if (percent >= 75)

	strcat( buf," has some small wounds and bruises.\n\r");

    else if (percent >=  50)

	strcat( buf, " has quite a few wounds.\n\r");

    else if (percent >= 30)

	strcat( buf, " has some big nasty wounds and scratches.\n\r");

    else if (percent >= 15)

	strcat ( buf, " looks pretty hurt.\n\r");

    else if (percent >= 0 )

	strcat (buf, " is in awful condition.\n\r");

    else

	strcat(buf, " is bleeding to death.\n\r");



    buf[0] = UPPER(buf[0]);

    send_to_char( buf, ch );



    found = FALSE;

    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )

    {

	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL

	&&   can_see_obj( ch, obj ) )

	{

	    if ( !found )

	    {

		send_to_char( "\n\r", ch );

		act( "$N is using:", ch, NULL, victim, TO_CHAR );

		found = TRUE;

	    }

	    send_to_char( where_name[iWear], ch );

	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );

	    send_to_char( "\n\r", ch );

	}

    }



    if ( victim != ch

    &&   !IS_NPC(ch)

    &&   number_percent( ) < get_skill(ch,gsn_peek))

    {

	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );

	check_improve(ch,gsn_peek,TRUE,4);

	show_list_to_char( victim->carrying, ch, TRUE, TRUE );

    }



    return;

}







void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )

{

    CHAR_DATA *rch;



    for ( rch = list; rch != NULL; rch = rch->next_in_room )

    {

	if ( rch == ch )

	    continue;



	if ( get_trust(ch) < rch->invis_level)

	    continue;



	if ( can_see( ch, rch ) )

	{

	    show_char_to_char_0( rch, ch );

	}

	else if ( room_is_dark( ch->in_room )

	&&        IS_AFFECTED(rch, AFF_INFRARED ) )

	{

	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );

	}

    }



    return;

}







bool check_blind( CHAR_DATA *ch )

{



    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))

	return TRUE;



    if ( IS_AFFECTED(ch, AFF_BLIND) )

    {

	send_to_char( "You can't see a thing!\n\r", ch );

	return FALSE;

    }



    return TRUE;

}



/* changes your scroll */

void do_scroll(CHAR_DATA *ch, char *argument)

{

    char arg[MAX_INPUT_LENGTH];

    char buf[100];

    int lines;



    one_argument(argument,arg);



    if (arg[0] == '\0')

    {

	if (ch->lines == 0)

	    send_to_char("You do not page long messages.\n\r",ch);

	else

	{

	    sprintf(buf,"You currently display %d lines per page.\n\r",

		    ch->lines + 2);

	    send_to_char(buf,ch);

	}

	return;

    }



    if (!is_number(arg))

    {

	send_to_char("You must provide a number.\n\r",ch);

	return;

    }



    lines = atoi(arg);



    if (lines == 0)

    {

	send_to_char("Paging disabled.\n\r",ch);

	ch->lines = 0;

	return;

    }



    if (lines < 10 || lines > 100)

    {

	send_to_char("You must provide a reasonable number.\n\r",ch);

	return;

    }



    sprintf(buf,"Scroll set to %d lines.\n\r",lines);

    send_to_char(buf,ch);

    ch->lines = lines - 2;

}





/* RT does socials */

void do_socials(CHAR_DATA *ch, char *argument)

{

    char buf[MAX_STRING_LENGTH];

    int iSocial;

    int col;



    col = 0;



    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)

    {

	sprintf(buf,"%-12s",social_table[iSocial].name);

	send_to_char(buf,ch);

	if (++col % 6 == 0)

	    send_to_char("\n\r",ch);

    }



    if ( col % 6 != 0)

	send_to_char("\n\r",ch);

    return;

}





/* Finger command, by Wench. */


void do_finger(CHAR_DATA *ch, char *argument)
{
    FILE *fp;
    char strsave[MAX_INPUT_LENGTH];
    bool aim_found = FALSE;
    bool msn_found = FALSE;
    bool yahoo_found = FALSE;
    bool icq_found = FALSE;
    bool email_found = FALSE;
    bool web_found = FALSE;
    bool real_found = FALSE;
    bool laston_found = FALSE;
    bool host_found = FALSE;
    bool level_found = FALSE;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *real = "";
    char *email = "";
    char *web = "";
    char *aim = "";
    char *msn = "";
    char *yahoo = "";
    char *icq = "";
    char *host = "";
    long  chlevel = 0;
    long laston;
    bool finished = FALSE;
    int d, h, m, s = 0;


    if(argument[0] == '\0' || argument[0] == '/' || argument[0] == '\\')
    {
    send_to_char("Finger who?\n\r", ch);
    send_to_char("{rSyntax:{w\n\r To look someone up: 'finger <name>'\n\r", ch);
    send_to_char("To set your real name: 'finger real <real name>'\n\r", ch);
    send_to_char("To set your email address: 'finger email <email address>'\n\r", ch);
    send_to_char("To set your web address: 'finger web <web address>'\n\r", ch);
    send_to_char("To set your AIM ID: 'finger aim <aim id>'\n\r", ch);
    send_to_char("To set your MSN Messenger ID: 'finger msn <msn id>'\n\r", ch);
    send_to_char("To set your Yahoo Messenger ID: 'finger yahoo <yahoo id>'\n\r", ch);
    send_to_char("To set your ICQ ID: 'finger icq <icq id>'\n\r", ch);
    return;
    }

    smash_tilde(argument);
    argument = one_argument(argument, arg);
    strcpy(arg2, argument);
    if(!str_cmp(arg, "email"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your email address is currently listed as %s.\n\r",
         ch->pcdata->email);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->email = strdup(arg2);
    sprintf(buf, "Your email address will now be displayed as %s.\n\r",
          ch->pcdata->email);
    send_to_char(buf, ch);
    return;
    }

    if(!str_cmp(arg, "web"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your web address is currently listed as %s.\n\r",
         ch->pcdata->web);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->web = strdup(arg2);
    sprintf(buf, "Your web address will now be displayed as %s.\n\r",
          ch->pcdata->web);
    send_to_char(buf, ch);
    return;
    }
    if(!str_cmp(arg, "aim"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your AIM ID is currently listed as %s.\n\r",
         ch->pcdata->aim);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->aim = strdup(arg2);
    sprintf(buf, "Your AIM ID will now be displayed as %s.\n\r",
          ch->pcdata->aim);
    send_to_char(buf, ch);
    return;
    }

   if(!str_cmp(arg, "msn"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your MSN ID is currently listed as %s.\n\r",
         ch->pcdata->msn);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->msn = strdup(arg2);
    sprintf(buf, "Your MSN ID will now be displayed as %s.\n\r",
          ch->pcdata->msn);
    send_to_char(buf, ch);
    return;
    }
    if(!str_cmp(arg, "yahoo"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your Yahoo Messenger ID is currently listed as %s.\n\r",
         ch->pcdata->yahoo);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->yahoo = strdup(arg2);
    sprintf(buf, "Your Yahoo Messenger ID will now be displayed as %s.\n\r",
          ch->pcdata->yahoo);
    send_to_char(buf, ch);
    return;
    }
    if(!str_cmp(arg, "icq"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your ICQ ID is currently listed as %s.\n\r",
         ch->pcdata->icq);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->icq = strdup(arg2);
    sprintf(buf, "Your ICQ ID will now be displayed as %s.\n\r",
          ch->pcdata->icq);
    send_to_char(buf, ch);
    return;
    }
    if(!str_cmp(arg, "real"))
    {
    if(arg2[0] == '\0')
    {
      sprintf(buf, "Your real name is currently listed as %s.\n\r",
       ch->pcdata->real_name);
      send_to_char(buf, ch);
      return;
    }
    ch->pcdata->real_name = strdup(arg2);
    sprintf(buf, "Your real name will now appear as %s.\n\r",
       ch->pcdata->real_name);
    send_to_char(buf, ch);
    return;
    }

    if(((victim = get_char_world(ch, arg)) != NULL) && (!IS_NPC(victim)))
    {
      sprintf(buf,"Name: %16s\n\r", victim->name);
      send_to_char(buf, ch);

	if (victim->pcdata->real_name != NULL)
	{
		sprintf(buf, "Real Name: %10s\n\r", victim->pcdata->real_name);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("Real Name:         Not Specified.\n\r", ch);
	}

	if (victim->pcdata->email != NULL)
	{
		sprintf(buf, "E-mail address: %5s\n\r", victim->pcdata->email);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("E-mail address:    Not Specified.\n\r", ch);
	}
	if (victim->pcdata->web != NULL)
	{
		sprintf(buf, "Web: %5s\n\r", victim->pcdata->web);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("Web address:    Not Specified.\n\r", ch);
	}
	if (victim->pcdata->aim != NULL)
	{
		sprintf(buf, "AIM ID: %5s\n\r", victim->pcdata->aim);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("AIM ID:    Not Specified.\n\r", ch);
	}

	if (victim->pcdata->msn != NULL)
	{
		sprintf(buf, "MSN Messenger ID: %5s\n\r", victim->pcdata->msn);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("MSN Messenger ID:    Not Specified.\n\r", ch);
	}

	if (victim->pcdata->yahoo != NULL)
	{
		sprintf(buf, "Yahoo Messenger ID: %5s\n\r", victim->pcdata->yahoo);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("Yahoo Messenger ID:    Not Specified.\n\r", ch);
	}

	if (victim->pcdata->icq != NULL)
	{
		sprintf(buf, "ICQ ID: %5s\n\r", victim->pcdata->icq);
    		send_to_char(buf, ch);
	}
	else
	{
		send_to_char("ICQ ID:    Not Specified.\n\r", ch);
	}
      if ( (victim->level < LEVEL_IMMORTAL) ||
	     ((victim->level >= LEVEL_IMMORTAL) && (get_trust(ch) >= victim->level)) )
	{
	   if(victim->desc != NULL)
    		sprintf(buf, "%s is currently playing.\n\r", victim->name);
    	   else
    		sprintf(buf, "%s is currently linkdead.\n\r", victim->name);
    		send_to_char(buf, ch);
	}
    	return;
    }


    sprintf(strsave, "%s%s", PLAYER_DIR, capitalize(arg));
    if((fp = fopen(strsave, "r")) == NULL)
    {
       sprintf(buf, "No such player: %s.\n\r", capitalize(arg));
       send_to_char(buf, ch);
       return;
    }

    for(;;)
    {
    	char *word;
    	fread_to_eol(fp);
    	word = feof(fp) ? "End" : fread_word(fp);
    	switch(UPPER(word[0]))
    	{
        case 'A':
        	if(!str_cmp(word, "Aim"))
        	{
          		aim = fread_string(fp);
          		aim_found = TRUE;
        	}
        	break;
        case 'E':
        	if(!str_cmp(word, "Email"))
        	{
          		email = fread_string(fp);
          		email_found = TRUE;
        	}
        	if(!str_cmp(word, "End"))
        	{
          		finished = TRUE;
        	}
        	break;
        case 'H':
        	if(!str_cmp(word, "Host"))
        	{
       		host = fread_string(fp);
       		host_found = TRUE;
        	}
        	break;
        case 'I':
        	if(!str_cmp(word, "Icq"))
        	{
          		icq = fread_string(fp);
          		icq_found = TRUE;
        	}
        	break;
        case 'M':
        	if(!str_cmp(word, "Msn"))
        	{
          		msn = fread_string(fp);
          		msn_found = TRUE;
        	}
        	break;
        case 'R':
        	if(!str_cmp(word, "Real"))
        	{
          		real = fread_string(fp);
          		real_found = TRUE;
        	}
        	break;
        case 'L':
        	if(!str_cmp(word, "LogO"))
        	{
          		laston = fread_number(fp);
          		laston_found = TRUE;
        	}
	  	if(!str_cmp(word, "Levl"))
	  	{
	    		chlevel = fread_number(fp);
	    		level_found = TRUE;
	  	}
        	break;
        case 'W':
        	if(!str_cmp(word, "Web"))
        	{
          		web = fread_string(fp);
          		web_found = TRUE;
        	}
        	break;

        case 'Y':
        	if(!str_cmp(word, "Yahoo"))
        	{
          		yahoo = fread_string(fp);
          		yahoo_found = TRUE;
        	}
        	break;
        default:
        	break;
    	}
      if((finished)||(aim_found&&icq_found&&msn_found&&real_found&&laston_found&&email_found&&level_found&&web_found&&yahoo_found))
      break;
    }

    s = current_time - laston;
    d = s/86400;
    s-=d*86400;
    h = s/3600;
    s -= h*3600;
    m = s/60;
    s -= m*60;

    sprintf(buf, "Name: %16s\n\rReal Name: %16s\n\r", capitalize(arg), real_found ? real: "Not specified.");
    send_to_char(buf, ch);
    sprintf(buf, "E-mail address: %20s\n\r", email_found ? email : "Not specified.");
    send_to_char(buf, ch);

    sprintf(buf, "Web address: %20s\n\r", web_found ? web : "Not specified.");
    send_to_char(buf, ch);

    sprintf(buf, "AIM ID: %20s\n\r", aim_found ? aim : "Not specified.");
    send_to_char(buf, ch);

    sprintf(buf, "MSN Messenger ID: %20s\n\r", msn_found ? msn : "Not specified.");
    send_to_char(buf, ch);

    sprintf(buf, "Yahoo Messenger ID: %20s\n\r", yahoo_found ? yahoo : "Not specified.");
    send_to_char(buf, ch);

    sprintf(buf, "ICQ ID: %20s\n\r", icq_found ? icq : "Not specified.");
    send_to_char(buf, ch);
    if ( (chlevel < LEVEL_IMMORTAL) ||
         ((chlevel >= LEVEL_IMMORTAL) && (get_trust(ch) >= chlevel)) ||
         (get_trust(ch) >= CREATOR) )
    {
    	sprintf(buf, "%s last logged on at %s\n\r", capitalize(arg), laston_found ? ctime(&laston) : "Not found.");
    	send_to_char(buf, ch);
    	sprintf(buf, "That was %d days, %d hours, %d minutes and %d seconds ago.\n\r", d, h, m, s);
    	send_to_char(buf, ch);
    	if(IS_IMMORTAL(ch))
    	{
      	if(host_found)
		{
      		sprintf(buf, "%s last logged on from %s\n\r", capitalize(arg), host);
    			send_to_char(buf, ch);
		}
    		else
		{
      		sprintf(buf, "No logon site found for %s.\n\r", capitalize(arg));
    			send_to_char(buf, ch);
      	}
    	}
    }

    fclose(fp);
    return;

}

/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
   do_help(ch,"wizlist");
}

void do_version(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	send_to_char( "\n\r", ch );
	send_to_char( "Current Version:\n\r", ch );
	send_to_char( "\n\r", ch );

	sprintf( buf, "{g     Magrathea %s{g build %s{w\n\r", MAG_VERSION, MAG_BUILD );
	send_to_char( buf, ch );
	send_to_char( "\n\r", ch );
	send_to_char( "\n\r", ch );

	send_to_char( "{wMagrathea is copyrighted (c) 1997-2004, all rights reserved.\n\r", ch );
	send_to_char( "\n\r", ch );
	send_to_char( "\n\r", ch );

	send_to_char( "{wGame Owned and Operated by:\n\r", ch );
	send_to_char( "     {cJason LeBlanc{w   ({yUrk{w)      \n\r", ch );
	send_to_char( "     {cScott Baker{w     ({yOmne{w)     \n\r", ch );
	send_to_char( "\n\r", ch );

	send_to_char( "{wGame Design and Concepts:\n\r", ch );
	send_to_char( "     {cJason LeBlanc{w   ({yUrk{w)      \n\r", ch );
	send_to_char( "     {cScott Baker{w     ({yOmne{w)     \n\r", ch );
	send_to_char( "     {cTodd C. Harris{w  ({DGrayson{w)  \n\r", ch );
	send_to_char( "\n\r", ch );

	send_to_char( "{wSource Code copyrighted by and licensed from:\n\r", ch );
	send_to_char( "     {cScott Baker{w     ({yOmne{w)     \n\r" , ch );
	send_to_char( "     {cJason LeBlanc{w   ({yUrk{w)      \n\r", ch );
	send_to_char( "\n\r", ch );

	send_to_char( "{wAdditonal Code contributed by and licensed from:\n\r", ch );
	send_to_char( "     {cTodd C. Harris{w  ({DGrayson{w)  \n\r", ch );
	send_to_char( "     {cChris Lucas{w     ({yStorm{w)    \n\r", ch );
	send_to_char( "     {cSteven Ferguson{w ({yFerg{w)     \n\r", ch );
	send_to_char( "     {cAndy Brown{w      ({yIstennu{w)  \n\r", ch );
	send_to_char( "\n\r", ch );
	send_to_char( "\n\r", ch );
	send_to_char( "\n\r", ch );
	send_to_char( "{wType {chelp version{w for the revision history.\n\r", ch );

	send_to_char( "\n\r", ch );

	return;

}



void do_trustlist(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"trustlist");
}



void do_clanlist(CHAR_DATA *ch, char *argument)
{
	int iClan;
	char buf[MAX_STRING_LENGTH];

	send_to_char( "The Clans of Magrathea\n\r", ch );
	send_to_char( "Clan                     Flag\n\r", ch );

	for (iClan = 0 ; clan_table[iClan].name != NULL; ++iClan)
	{
		if ( clan_table[iClan].active && clan_table[iClan].visible )
		{
			sprintf( buf, "%-20s     %s\n\r", clan_table[iClan].name, clan_table[iClan].who_name );
			send_to_char( buf, ch );
		}
	}
}

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("   action     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);

    send_to_char("autoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autowizi       ",ch);
    if (IS_SET(ch->act,PLR_AUTOWIZI))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("compact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("prompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    if (!IS_SET(ch->act,PLR_CANLOOT))
	send_to_char("Your corpse is safe from thieves.\n\r",ch);
    else
	send_to_char("Your corpse may be looted.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You cannot be summoned.\n\r",ch);
    else
	send_to_char("You can be summoned.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.\n\r",ch);
    else
	send_to_char("You accept followers.\n\r",ch);
}

void do_autoassist(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOASSIST))

    {

      send_to_char("Autoassist removed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOASSIST);

    }

    else

    {

      send_to_char("You will now assist when needed.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOASSIST);

    }

}



void do_autoexit(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOEXIT))

    {

      send_to_char("Exits will no longer be displayed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOEXIT);

    }

    else

    {

      send_to_char("Exits will now be displayed.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOEXIT);

    }

}



void do_autogold(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOGOLD))

    {

      send_to_char("Autogold removed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOGOLD);

    }

    else

    {

      send_to_char("Automatic gold looting set.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOGOLD);

    }

}



void do_autowizi(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOWIZI))

    {

      send_to_char("Autowizi removed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOWIZI);

    }

    else

    {

      send_to_char("Automatic displaying of Immortal Wizi/Incog is on.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOWIZI);

    }

}



void do_autoloot(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOLOOT))

    {

      send_to_char("Autolooting removed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOLOOT);

    }

    else

    {

      send_to_char("Automatic corpse looting set.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOLOOT);

    }

}



void do_autosac(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOSAC))

    {

      send_to_char("Autosacrificing removed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOSAC);

    }

    else

    {

      send_to_char("Automatic corpse sacrificing set.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOSAC);

    }

}



void do_autosplit(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_AUTOSPLIT))

    {

      send_to_char("Autosplitting removed.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);

    }

    else

    {

      send_to_char("Automatic gold splitting set.\n\r",ch);

      SET_BIT(ch->act,PLR_AUTOSPLIT);

    }

}



void do_brief(CHAR_DATA *ch, char *argument)

{

    if (IS_SET(ch->comm,COMM_BRIEF))

    {

      send_to_char("Full descriptions activated.\n\r",ch);

      REMOVE_BIT(ch->comm,COMM_BRIEF);

    }

    else

    {

      send_to_char("Short descriptions activated.\n\r",ch);

      SET_BIT(ch->comm,COMM_BRIEF);

    }

}



void do_compact(CHAR_DATA *ch, char *argument)

{

    if (IS_SET(ch->comm,COMM_COMPACT))

    {

      send_to_char("Compact mode removed.\n\r",ch);

      REMOVE_BIT(ch->comm,COMM_COMPACT);

    }

    else

    {

      send_to_char("Compact mode set.\n\r",ch);

      SET_BIT(ch->comm,COMM_COMPACT);

    }

}



void do_show(CHAR_DATA *ch, char *argument)

{

    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))

    {

      send_to_char("Affects will no longer be shown in score.\n\r",ch);

      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);

    }

    else

    {

      send_to_char("Affects will now be shown in score.\n\r",ch);

      SET_BIT(ch->comm,COMM_SHOW_AFFECTS);

    }

}



void do_prompt(CHAR_DATA *ch, char *argument)

{

   char buf[MAX_STRING_LENGTH];



   if ( argument[0] == '\0' )

   {

	if (IS_SET(ch->comm,COMM_PROMPT))

	{

	    send_to_char("You will no longer see prompts.\n\r",ch);

	    REMOVE_BIT(ch->comm,COMM_PROMPT);

	}

	else

	{

	    send_to_char("You will now see prompts.\n\r",ch);

	    SET_BIT(ch->comm,COMM_PROMPT);

	}

       return;

   }



   if( !strcmp( argument, "all" ) )

      strcpy( buf, "{c<%hhp %mm %vmv>{x " );

   else

   {

      if ( strlen(argument) > 50 )

	 argument[50] = '\0';

      strcpy( buf, argument );

      smash_tilde( buf );

      if (str_suffix("%c",buf))

	strcat(buf," ");



   }



   free_string( ch->prompt );

   ch->prompt = str_dup( buf );

   sprintf(buf,"Prompt set to %s\n\r",ch->prompt );

   send_to_char(buf,ch);

   return;

}



void do_combine(CHAR_DATA *ch, char *argument)

{

    if (IS_SET(ch->comm,COMM_COMBINE))

    {

      send_to_char("Long inventory selected.\n\r",ch);

      REMOVE_BIT(ch->comm,COMM_COMBINE);

    }

    else

    {

      send_to_char("Combined inventory selected.\n\r",ch);

      SET_BIT(ch->comm,COMM_COMBINE);

    }

}



void do_noloot(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_CANLOOT))

    {

      send_to_char("Your corpse is now safe from thieves.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_CANLOOT);

    }

    else

    {

      send_to_char("Your corpse may now be looted.\n\r",ch);

      SET_BIT(ch->act,PLR_CANLOOT);

    }

}



void do_nofollow(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

      return;



    if (IS_SET(ch->act,PLR_NOFOLLOW))

    {

      send_to_char("You now accept followers.\n\r",ch);

      REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    }

    else

    {

      send_to_char("You no longer accept followers.\n\r",ch);

      SET_BIT(ch->act,PLR_NOFOLLOW);

      die_follower( ch );

    }

}



void do_nosummon(CHAR_DATA *ch, char *argument)

{

    if (IS_NPC(ch))

    {

      if (IS_SET(ch->imm_flags,IMM_SUMMON))

      {

	send_to_char("You are no longer immune to summon.\n\r",ch);

	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);

      }

      else

      {

	send_to_char("You are now immune to summoning.\n\r",ch);

	SET_BIT(ch->imm_flags,IMM_SUMMON);

      }

    }

    else

    {

      if (IS_SET(ch->act,PLR_NOSUMMON))

      {

	send_to_char("You are no longer immune to summon.\n\r",ch);

	REMOVE_BIT(ch->act,PLR_NOSUMMON);

      }

      else

      {

	send_to_char("You are now immune to summoning.\n\r",ch);

	SET_BIT(ch->act,PLR_NOSUMMON);

      }

    }

}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *pdesc;
    int door;
    int number,count;

    if ( ch->desc == NULL )
	return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
	    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
  	    &&   room_is_dark( ch->in_room ) 
		&&	 !IS_AFFECTED(ch,AFF_DARK_VISION))
    {

		send_to_char( "It is pitch black ... \n\r", ch );
		show_char_to_char( ch->in_room->people, ch );
		return;
    }


    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	if (IS_HERO(ch) && (IS_NPC(ch) || IS_SET(ch->act,PLR_HOLYLIGHT)))
	{
	    sprintf(buf,"{R[{rRoom {y%d{R]",ch->in_room->vnum);
	    send_to_char(buf,ch);
	}

	sprintf( buf, " {c%s",ch->in_room->name );
	send_to_char( buf, ch );

	if (IS_SET( ch->in_room->room_flags, ROOM_BANK ))
		send_to_char(" {G[{gBank{G]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_ENGRAVING ))
		send_to_char(" {G[{gEngraving{G]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_ARENA ))
		send_to_char(" {r[{yArena{r]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_SUSTAIN ))
		send_to_char(" {c[{wSustain{c]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_SAFE ))
		send_to_char(" {c[{gSafe{c]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_NOPK ))
		send_to_char(" {c[{bPK Safe{c]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_HELPER ))
		send_to_char(" {y[{YHelper{y]", ch);

	if (IS_SET( ch->in_room->room_flags, ROOM_CODER ))
		send_to_char(" {B[{DCODER{B]", ch);

	send_to_char( "{w\n\r", ch );

	if ( arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) ) )
	{
	    send_to_char( "  ",ch);
	    send_to_char( ch->in_room->description, ch );
	}

	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
	{
	    send_to_char("\n\r",ch);
	    do_exits( ch, "auto" );
	}

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    sprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than half-" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about half-"     : "more than half-",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p holds:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{  /* player can see object */
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
		if (++count == number)
		{
		    send_to_char( pdesc, ch );
		    return;
		}
		else continue;

	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
		if (++count == number)
		{
		    send_to_char( pdesc, ch );
		    return;
		}
		else continue;

	    if ( is_name( arg3, obj->name ) )
		if (++count == number)
		{
		    send_to_char( obj->description, ch );
		    send_to_char( "\n\r",ch);
		    return;
		  }
	  }
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
		if (++count == number)
		{
		    send_to_char( pdesc, ch );

		    return;

		}



	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );

	    if ( pdesc != NULL )

		if (++count == number)

		{

		    send_to_char( pdesc, ch );

		    return;

		}



	    if ( is_name( arg3, obj->name ) )

		if (++count == number)

		{

		    send_to_char( obj->description, ch );

		    send_to_char("\n\r",ch);

		    return;

		}

	}

    }



    pdesc = get_extra_descr(arg3,ch->in_room->extra_descr);

    if (pdesc != NULL)

    {

	if (++count == number)

	{

	    send_to_char(pdesc,ch);

	    return;

	}

    }



    if (count > 0 && count != number)

    {

	if (count == 1)

	    sprintf(buf,"You only see one %s here.\n\r",arg3);

	else

	    sprintf(buf,"You only see %d of those here.\n\r",count);



	send_to_char(buf,ch);

	return;

    }



	 if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;

    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;

    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;

    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;

    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;

    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;

    else

    {

	send_to_char( "You do not see that here.\n\r", ch );

	return;

    }



    /* 'look direction' */

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )

    {

	send_to_char( "Nothing special there.\n\r", ch );

	return;

    }



    if ( pexit->description != NULL && pexit->description[0] != '\0' )

	send_to_char( pexit->description, ch );

    else

	send_to_char( "Nothing special there.\n\r", ch );



    if ( pexit->keyword    != NULL

    &&   pexit->keyword[0] != '\0'

    &&   pexit->keyword[0] != ' ' )

    {

	if ( IS_SET(pexit->exit_info, EX_CLOSED) )

	{

	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );

	}

	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )

	{

	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );

	}

    }



    return;

}



/* RT added back for the hell of it */

void do_read (CHAR_DATA *ch, char *argument )

{

    do_look(ch,argument);

}



void do_examine( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Examine what?\n\r", ch );

	return;

    }



    do_look( ch, arg );



    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )

    {

	switch ( obj->item_type )

	{

	default:

	    break;



	case ITEM_JUKEBOX:

	    do_play(ch,"list");

	    break;



	case ITEM_MONEY:

	    if (obj->value[0] == 0)

	    {

		if (obj->value[1] == 0)

		    sprintf(buf,"Odd...there's no coins in the pile.\n\r");

		else if (obj->value[1] == 1)

		    sprintf(buf,"Wow. One gold coin.\n\r");

		else

		    sprintf(buf,"There are %d gold coins in the pile.\n\r",

			obj->value[1]);

	    }

	    else if (obj->value[1] == 0)

	    {

		if (obj->value[0] == 1)

		    sprintf(buf,"Wow. One silver coin.\n\r");

		else

		    sprintf(buf,"There are %d silver coins in the pile.\n\r",

			obj->value[0]);

	    }

	    else

		sprintf(buf,

		    "There are %d gold and %d silver coins in the pile.\n\r",

		    obj->value[1],obj->value[0]);

	    send_to_char(buf,ch);

	    break;



	case ITEM_DRINK_CON:

	case ITEM_CONTAINER:

	case ITEM_CORPSE_NPC:

	case ITEM_CORPSE_PC:

	    sprintf(buf,"in %s",argument);

	    do_look( ch, buf );

	}

    }



    return;

}







/*

 * Thanks to Zrin for auto-exit part.

 */

void do_exits( CHAR_DATA *ch, char *argument )

{

    extern char * const dir_name[];

    char buf[MAX_STRING_LENGTH];

    EXIT_DATA *pexit;

    bool found;

    bool fAuto;

    int door;



    fAuto  = !str_cmp( argument, "auto" );



    if ( !check_blind( ch ) )

	return;



    if (fAuto)

	sprintf(buf,"{Y[{GExits:");

    else if (IS_IMMORTAL(ch))

	sprintf(buf,"Obvious exits from room %d:\n\r",ch->in_room->vnum);

    else

	sprintf(buf,"Obvious exits:\n\r");



    found = FALSE;

    for ( door = 0; door <= 5; door++ )

    {

	if ( ( pexit = ch->in_room->exit[door] ) != NULL

	&&   pexit->u1.to_room != NULL

	&&   can_see_room(ch,pexit->u1.to_room)

	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )

	{

	    found = TRUE;

	    if ( fAuto )

	    {

		strcat( buf, " " );

		strcat( buf, dir_name[door] );

	    }

	    else

	    {

		sprintf( buf + strlen(buf), "%-5s - %s",

		    capitalize( dir_name[door] ),

		    room_is_dark( pexit->u1.to_room )

			?  "Too dark to tell"

			: pexit->u1.to_room->name

		    );

		if (IS_IMMORTAL(ch))

		    sprintf(buf + strlen(buf),

			" (room %d){w\n\r",pexit->u1.to_room->vnum);

		else

		    sprintf(buf + strlen(buf), "{w\n\r");

	    }

	}

    }



    if ( !found )

	strcat( buf, fAuto ? " none" : "None.{w\n\r" );



    if ( fAuto )

	strcat( buf, "{Y]{w\n\r" );



    send_to_char( buf, ch );

    return;

}



void do_worth( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_NPC(ch))

    {

	sprintf(buf,"You have %ld gold and %ld silver.\n\r",

	    ch->gold,ch->silver);

	send_to_char(buf,ch);

	return;

    }



    sprintf(buf,

    "You have %ld gold, %ld silver, and %d experience (%d exp to level).\n\r",

	ch->gold, ch->silver,ch->exp,

	(ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp);



    send_to_char(buf,ch);



    return;

}





void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    if IS_NPC(ch)
	return;

    sprintf( buf,
	"{wYou are %s%s{w, level {r%s{w, {c%d{w years old ({c%d/%d{w hours).\n\r",
	ch->name,
	ch->pcdata->title,
	lev_name(ch->level), get_age(ch),
	( ch->played + (int) (current_time - ch->logon) ) / 3600,
	( ch->perm_played + (int) (current_time - ch->logon) ) / 3600);
    send_to_char( buf, ch );

    if ( IS_SET(ch->info, INFO_GHOST) )
    {
	send_to_char( "{wYou are currently a {WGHOST{w.\n\r", ch );
	if ( ch->pcdata->timer_ghost > 0 )
	{
		sprintf( buf, "You have %d ticks remaining until you will return to the place of your death.\n\r",
			ch->pcdata->timer_ghost );
		send_to_char( buf, ch );
	}
	else
		send_to_char( "You are now bound to the place of your death!\n\r", ch );
    }

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "{wYou are trusted at level {r%s{w.\n\r",
	    lev_name(get_trust( ch )) );
	send_to_char( buf, ch );
    }

    sprintf(buf, "Race: {g%s{w  Sex: {g%s{w  Class: {g%s{w\n\r",
	race_table[ch->race].name,
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
	class_table[ch->class].name);
    send_to_char(buf,ch);

	if (ch->pcdata->incarnations > 1)
	{
    	sprintf( buf, "Incarnations: {c%d{w\n\r", ch->pcdata->incarnations );
    	send_to_char( buf, ch );
	}
	if (ch->pcdata->anymud)
	{
    	sprintf( buf, "MudExp: Mud:{c%s {wRom:{c%s {wMag:{c%s{w\n\r",
            ch->pcdata->anymud == 0 ? "no" : "yes",
            ch->pcdata->rommud == 0 ? "no" : "yes",
            ch->pcdata->magmud == 0 ? "no" : "yes");
   	   send_to_char( buf, ch );
    }

	/* URK_KILLCOUNTER */

	sprintf(buf, "Mob Kills:    %4d.  Mob Deaths:    %4d.\n\r",
	ch->mkill, ch->mdeath);
	send_to_char(buf, ch);
	sprintf(buf, "Player Kills: %4d.  Player Deaths: %4d.\n\r",
	ch->pkill, ch->pdeath);
	send_to_char(buf, ch);
    sprintf(buf, "Slay Kills:   %4d.  Slay Deaths:   %4d.\n\r",
	ch->slaykill, ch->slaydeath);
    send_to_char(buf, ch);

    sprintf( buf,
	"{wYou have {c%d{w/{c%d{w hit, {c%d{w/{c%d{w mana, {c%d{w/{c%d{w movement.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
    send_to_char( buf, ch );

    sprintf( buf,
	"You have {c%d{w practices and {c%d{w training sessions.\n\r",
	ch->practice, ch->train);
    send_to_char( buf, ch );

    sprintf( buf,
	"You are carrying {c%d{w/{c%d{w items with weight {c%ld{w/{c%d{w pounds.\n\r",
	ch->carry_number, can_carry_n(ch),
	get_carry_weight(ch) / 10, can_carry_w(ch) /10 );
    send_to_char( buf, ch );

    sprintf( buf,
	"Str: {c%d{w({y%d{w)  Int: {c%d{w({y%d{w)  Wis: {c%d{w({y%d{w)  Dex: {c%d{w({y%d{w)  Con: {c%d{w({y%d{w)\n\r",
	ch->perm_stat[STAT_STR],
	get_curr_stat(ch,STAT_STR),
	ch->perm_stat[STAT_INT],
	get_curr_stat(ch,STAT_INT),
	ch->perm_stat[STAT_WIS],
	get_curr_stat(ch,STAT_WIS),
	ch->perm_stat[STAT_DEX],
	get_curr_stat(ch,STAT_DEX),
	ch->perm_stat[STAT_CON],
	get_curr_stat(ch,STAT_CON) );
    send_to_char( buf, ch );

	sprintf( buf,
	"Agi: {c%d{w({y%d{w)  Mch: {c%d{w({y%d{w)  Tch: {c%d{w({y%d{w)\n\r",
	ch->perm_stat[STAT_AGI],
	get_curr_stat(ch,STAT_AGI),
	ch->perm_stat[STAT_MCH],
	get_curr_stat(ch,STAT_MCH),
	ch->perm_stat[STAT_TCH],
	get_curr_stat(ch,STAT_TCH) );
    send_to_char( buf, ch );
   
    
    sprintf( buf,
	"You have scored {c%d{w exp, and have {y%ld{w gold and {g%ld{w silver coins.\n\r",
	ch->exp,  ch->gold, ch->silver );
    send_to_char( buf, ch );


    sprintf( buf, "You have {y%ld{w gold in the bank.\n\r", ch->bank );
    send_to_char( buf, ch );

    /* RT shows exp to level */
    if ( ch->level < LEVEL_HERO )
    {
      sprintf (buf,
	"You need {c%d{w exp to level.\n\r",
	((ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp));
      send_to_char( buf, ch );
     }

	if (ch->pcdata->bounty > 0)
	{
    	sprintf( buf, "There is currently a {r%d{w gold bounty on your head.\n\r", ch->pcdata->bounty );
    	send_to_char( buf, ch );
	}

    sprintf( buf, "Wimpy set to {c%d{w hit points.\n\r", ch->wimpy );
    send_to_char( buf, ch );

    if ( ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( ch->pcdata->condition[COND_THIRST] <= 3 )
	send_to_char( "You are thirsty.\n\r", ch );
    if ( ch->pcdata->condition[COND_THIRST] == 0 )
	send_to_char( "{rYou are faint with fatigue and thirst.{w\n\r", ch );
    if ( ch->pcdata->condition[COND_HUNGER]   <= 3 )
	send_to_char( "You are hungry.\n\r",  ch );
    if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	send_to_char( "{rYou are faint with fatigue and hunger.{w\n\r",  ch );

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "{rYou are DEAD!!{w\n\r", ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",  ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",     ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",           ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",          ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",           ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",           ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",          ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",          ch );
	break;
    }

    /* print AC values */
    if (ch->level >= 25)
    {
	sprintf( buf,"Armor: pierce: {c%d{w  bash: {c%d{w  slash: {c%d{w  magic: {c%d{w\n\r",
		 GET_AC(ch,AC_PIERCE),
		 GET_AC(ch,AC_BASH),
		 GET_AC(ch,AC_SLASH),
		 GET_AC(ch,AC_EXOTIC));
	send_to_char(buf,ch);
    }

    for (i = 0; i < 4; i++)
    {
	char * temp;

	switch(i)
	{
	    case(AC_PIERCE):    temp = "piercing";      break;
	    case(AC_BASH):      temp = "bashing";       break;
	    case(AC_SLASH):     temp = "slashing";      break;
	    case(AC_EXOTIC):    temp = "magic";         break;
	    default:            temp = "error";         break;
	}

	send_to_char("You are ", ch);

	if      (GET_AC(ch,i) >=  101 )
	    sprintf(buf,"hopelessly vulnerable to %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 75)
	    sprintf(buf,"defenseless against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 50)
	    sprintf(buf,"barely protected from %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 25)
	    sprintf(buf,"slightly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 0)
	    sprintf(buf,"somewhat armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -25)
	    sprintf(buf,"armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -50)
	    sprintf(buf,"well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -100)
	    sprintf(buf,"very well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -200)
	    sprintf(buf,"heavily armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -500)
	    sprintf(buf,"superbly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -1000)
	    sprintf(buf,"almost invulnerable to %s.\n\r",temp);
	else
	    sprintf(buf,"divinely armored against %s.\n\r",temp);

	send_to_char(buf,ch);
    }



    /* RT wizinvis and holy light */

    if ( IS_HERO(ch))

    {

      send_to_char("Holy Light: ",ch);

      if (IS_SET(ch->act,PLR_HOLYLIGHT))

	send_to_char("{yon{w",ch);

      else

	send_to_char("off",ch);



      if (ch->invis_level)

      {

	sprintf( buf, "  {rInvisible: %s{w",lev_name(ch->invis_level));

	send_to_char(buf,ch);

      }



      if (ch->incog_level)
      {
	sprintf(buf,"  {bIncognito: %s{w",lev_name(ch->incog_level));
	send_to_char(buf,ch);
      }
      send_to_char("\n\r",ch);
    }

	if (ch->olcs)
	{
    		sprintf( buf, "{cOLC{W:[{w %s {W]{w\n\r", olcs_bit_name(ch->olcs) );
    		send_to_char( buf, ch );
	}



	if (ch->pcdata->olc_on)								/*Storm*/
	{											/*Storm*/
		send_to_char( "You are in {c<{mOLC{c>{w mode.\n\r", ch );	/*Storm*/
	}											/*Storm*/

    if ( ch->level >= 15 )
    {
	sprintf( buf, "Hitroll: {c%d{w\nDamroll: {c%d\n{wDefroll: {c%d{w\n\r",
	    GET_HITROLL(ch), GET_DAMROLL(ch), GET_DEFROLL(ch) );
	send_to_char( buf, ch );
    }


    if ( ch->level >= 10 )
    {
	sprintf( buf, "Alignment: {c%d{w.  ", ch->alignment );
	send_to_char( buf, ch );
    }

    send_to_char( "You are ", ch );
    if ( ch->alignment >  900 ) send_to_char( "angelic.\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_char( "saintly.\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_char( "good.\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "kind.\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "neutral.\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_char( "mean.\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "evil.\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "demonic.\n\r", ch );
    else                             send_to_char( "satanic.\n\r", ch );

    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
	do_affects(ch,"");
}

void do_affects(CHAR_DATA *ch, char *argument )
{

    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];

    if ( ch->affected != NULL )
    {
	send_to_char( "You are affected by the following spells:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20)
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
		sprintf( buf, "Spell: {r%-15s{w", skill_table[paf->type].name );

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
		    ": modifies {g%s{w by {c%d{w ",
		    affect_loc_name( paf->location ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 )
		    sprintf( buf, "{rpermanently{w" );
		else
		    sprintf( buf, "for {c%d{w hours", paf->duration );
		send_to_char( buf, ch );
	    }

	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else
	send_to_char("You are not affected by any spells.\n\r",ch);

    return;
}



char *  const   day_name        [] =
{
    "the Moon", "Honor", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};


char *  const   month_name      [] =
{
    "Knives", "the Blue Lady", "the Frost Giant", "the Old Forces of Fingolfin",
    "the Grand Struggle", "the Spring", "Pizza", "Futility", "the Dragon",
    "Grayson's Wrath", "the Sun", "the Heat", "the Battle", "the Dark Shades",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil Urk", "the Shadows",
    "the Benevolent Omne", "the Zot", "the Long Dark Night"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

	 if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, the %d%s of the Month of %s.\n\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month]);
    send_to_char(buf,ch);
    sprintf(buf,"Magrathea started up at %s\n\rThe system time is %s\n\r",
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}




void do_weather( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    static char * const sky_look[10] =

    {

	"cloudless",

	"cloudy",

	"dark and gloomy",

	"forboding",

	"rainy",

	"snowy",

	"bright and sunny",

	"fresh as the spring",

	"dark like the hounds of hell",

	"lit by flashes of lightning"

    };



    if ( !IS_OUTSIDE(ch) )

    {

	send_to_char( "You can't see the weather indoors.\n\r", ch );

	return;

    }



    sprintf( buf, "The sky is %s and %s.\n\r",

	sky_look[weather_info.sky],

	weather_info.change >= 0

	? "a warm southerly breeze blows"

	: "a cold northern gust blows"

	);

    send_to_char( buf, ch );

    return;

}



void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    BUFFER *output;
    bool found = FALSE;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];
    int level;

   output = new_buf();

    if ( argument[0] == '\0' )
	argument = "summary";

	if ( argument[1] == '\0')
	{
		send_to_char("Help arguments must be at least two characters in length.",ch);
		return;
	}

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;

	if (level > get_trust( ch ) )
	    continue;

	if ( is_name( argall, pHelp->keyword ) )
	{
	    /* add seperator if found */
	    if (found)
		add_buf(output,
    "\n\r============================================================\n\r\n\r");
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		add_buf(output,pHelp->keyword);
		add_buf(output,"\n\r");
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		add_buf(output,pHelp->text+1);
	    else
		add_buf(output,pHelp->text);
	    found = TRUE;
	    /* small hack :) */
	    if (ch->desc != NULL && ch->desc->connected != CON_PLAYING
	    &&                      ch->desc->connected != CON_GEN_GROUPS)
		break;
	}
    }

    if (!found)
	send_to_char( "No help on that word.\n\r", ch );
    else
	page_to_char(buf_string(output),ch);
    free_buf(output);
}




/* whois command */

void do_whois (CHAR_DATA *ch, char *argument)

{

    char arg[MAX_INPUT_LENGTH];

    BUFFER *output;

    char buf[MAX_STRING_LENGTH];

    bool found = FALSE;

    CHAR_DATA *wch;



    one_argument(argument,arg);



    if (arg[0] == '\0')

    {

	send_to_char("You must provide a name.\n\r",ch);

	return;

    }



    output = new_buf();



    if ( ( wch = get_char_world( ch, arg ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }



    if (IS_NPC(wch))

    {

	send_to_char( "WhoIs may only be used on players.\n\r", ch );

	return;

    }

    else

    {

	found = TRUE;



	send_to_char("{r_____________________________________________________________________________{w\n\r\n\r\n\r\n\r\n\r",ch);



	sprintf( buf,

	   "{w%s%s{w, level {r%s{w, {c%d{w years old ({c%d/%d{w hours).\n\r",

	   wch->name,

	   IS_NPC(wch) ? "" : wch->pcdata->title,

	   lev_name(wch->level), get_age(wch),

	   ( wch->played + (int) (current_time - wch->logon) ) / 3600,

	   ( wch->perm_played + (int) (current_time - ch->logon) ) / 3600);

	send_to_char( buf, ch );



	sprintf(buf, "Race: {g%s{w  Sex: {g%s{w  Class: {g%s{w\n\r",

	   race_table[wch->race].name,

	   wch->sex == 0 ? "sexless" : wch->sex == 1 ? "male" : "female",

	   IS_NPC(wch) ? "mobile" : class_table[wch->class].name);

	send_to_char(buf,ch);



	if (wch->pcdata->incarnations > 1)

	{

	   sprintf( buf, "Incarnations: {c%d{w\n\r", wch->pcdata->incarnations );

    	   send_to_char( buf, ch );

	}

	if (wch->pcdata->anymud)
	{
	   sprintf( buf, "MudExp: Mud:{c%s {wRom:{c%s {wMag:{c%s{w\n\r",
            wch->pcdata->anymud == 0 ? "no" : "yes",
            wch->pcdata->rommud == 0 ? "no" : "yes",
            wch->pcdata->magmud == 0 ? "no" : "yes");
   	   send_to_char( buf, ch );
    }

	sprintf( buf,

	   "{gEXP: %d{w\n\r",

	   wch->exp);

	send_to_char( buf, ch );



	if (wch->pcdata->bounty > 0)

	{

	   sprintf( buf, "{RBOUNTY{W: {w%d gold\n\r", wch->pcdata->bounty );

	   send_to_char( buf, ch );

	}



	if (wch->olcs)

	{

	   sprintf( buf, "{cOLC{W:[{w %s {W]{w\n\r", olcs_bit_name(wch->olcs) );

	   send_to_char( buf, ch );

	}



	if (wch->pcdata->olc_on)							/*Storm*/

	{											/*Storm*/

	    send_to_char("{c<{mOLC{c>{w mode is active.{w\n\r",ch);		/*Storm*/

	}											/*Storm*/



	/* URK_KILLCOUNTER */

        sprintf(buf, "Mob Kills:    %4d.  Mob Deaths:    %4d.\n\r",

        wch->mkill, wch->mdeath);

        send_to_char(buf, ch);

        sprintf(buf, "Player Kills: %4d.  Player Deaths: %4d.\n\r",

        wch->pkill, wch->pdeath);

        send_to_char(buf, ch);

        sprintf(buf, "Slay Kills:   %4d.  Slay Deaths:   %4d.\n\r",
        wch->slaykill, wch->slaydeath);
        send_to_char(buf, ch);




	if (IS_SET(wch->info, INFO_GHOST))

	   send_to_char( "\n\r{r*** GHOST ***{w\n\r", ch);



	if (!found)

	{

	   send_to_char("No one of that name is playing.\n\r",ch);

	   return;

	}



	send_to_char("\n\r\n\r\n\r{r_____________________________________________________________________________{w\n\r",ch);



}}





/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */

void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    BUFFER *output;
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iClan;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool rgfClan[MAX_CLAN];
    bool fClassRestrict = FALSE;
    bool fClanRestrict = FALSE;
    bool fClan = FALSE;
    bool fRaceRestrict = FALSE;
    bool fImmortalOnly = FALSE;
    bool see_clan;
    char RILvl[MAX_INPUT_LENGTH];

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
	rgfRace[iRace] = FALSE;
    for (iClan = 0; iClan < MAX_CLAN; iClan++)
	rgfClan[iClan] = FALSE;

    see_clan	= TRUE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if ( is_number( arg ) )
	{
	    switch ( ++nNumber )
	    {
	    case 1: iLevelLower = atoi( arg ); break;
	    case 2: iLevelUpper = atoi( arg ); break;
	    default:
		send_to_char( "Only two level numbers allowed.\n\r", ch );
		return;
	    }
	}
	else
	{

	    /*
	     * Look for classes to turn on.
	     */
	    if (!str_prefix(arg,"immortals"))
	    {
		fImmortalOnly = TRUE;
	    }
	    else if (!str_prefix(arg,"imm"))
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
		iClass = class_lookup(arg);
		if (iClass == -1)
		{
		    iRace = race_lookup(arg);

		    if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
			if (!str_prefix(arg,"clan"))
			    fClan = TRUE;
			else
			{
			    iClan = clan_lookup(arg);
			    if (iClan)
			    {
				if ( clan_table[iClan].visible )
				{
					see_clan		= TRUE;
					fClanRestrict 	= TRUE;
					rgfClan[iClan] 	= TRUE;
				}

				if ( !clan_table[iClan].visible )
				{
					see_clan	= FALSE;

					if ( get_trust(ch) >= SUPREME || IS_SET(ch->olcs,OLC_CLANADMIN) || ch->clan == iClan )
					{
						see_clan		= TRUE;
						fClanRestrict 	= TRUE;
						rgfClan[iClan] 	= TRUE;
					}
				}

				if ( !see_clan )
				{
					send_to_char("That's not a valid race, class, or clan.\n\r",ch);
					return;
				}
			    }
			    else
			    {
				send_to_char(
				"That's not a valid race, class, or clan.\n\r",
				   ch);
				return;
			    }
			}
		    }
		    else
		    {
			fRaceRestrict = TRUE;
			rgfRace[iRace] = TRUE;
		    }
		}
		else
		{
		    fClassRestrict = TRUE;
		    rgfClass[iClass] = TRUE;
		}
	    }
	}
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    output = new_buf();
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *class;

	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;

	if (!can_see(ch,wch))
	    continue;

	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level < LEVEL_HERO )
	|| ( fClassRestrict && !rgfClass[wch->class] )
	|| ( fRaceRestrict && !rgfRace[wch->race])
	|| ( fClan && !is_clan(wch))
	|| ( fClan && is_clan(wch) && !clan_table[wch->clan].visible && get_trust(ch) < SUPREME )
	|| ( fClanRestrict && !rgfClan[wch->clan]))
	    continue;

	nMatch++;

	    /*Calculate the Incog level*/
	   if (wch->incog_level > 1)
	   {
		if (wch->incog_level > get_trust(ch) && wch->incog_level > wch->level)
		{
			sprintf(RILvl, "{B|{bI:%s{B|{w", lev_name(wch->level));
		}
		else
		{
			sprintf(RILvl, "{B|{bI:%s{B|{w", lev_name(wch->incog_level));
		}
	   }

	/*
	 * Visible Clan Information
	 */
	see_clan	= FALSE;
	if ( clan_table[wch->clan].visible )
	{
		see_clan	= TRUE;
	}
	if ( get_trust(ch) >= SUPREME )
	{
		see_clan	= TRUE;
	}
	if ( IS_SET(ch->olcs,OLC_CLANADMIN) )
	{
		see_clan	= TRUE;
	}
	if ( is_same_clan( ch, wch ) )
	{
		see_clan	= TRUE;
	}


	/*
	 * Figure out what to print for class.
	 */
	class = class_table[wch->class].who_name;
	switch ( wch->level )
	{
	default: break;
	/* Again we comment this out to format like we want
	    {
		case IMPLEMENTOR : class = "IMP";     break;
		case CREATOR : class = "CRE";     break;
		case SUPREME : class = "SUP";     break;
		case DEITY : class = "DEI";     break;
		case GOD : class = "GOD";     break;
		case IMMORTAL : class = "IMM";     break;
		case DEMI : class = "DEM";     break;
		case ANGEL : class = "ANG";     break;
		case AVATAR : class = "AVA";     break;
	    case HERO : class = "HER";  break;
	    }
	*/
	}

	/*
	 * Format it up.
	 */

    if (ch-> level >= 10 && !IS_HERO(ch) && !IS_SET(ch->act, PLR_HELPER) )
	{
	sprintf( buf, "{W[{c%3s{C %13s{c %s{W]{w %s%s%s%s%s%s%s%s%s%s%s%s%s%s{w %s{w %s%s%s%s%s{w\n\r",
	    (lev_imm(wch->level)),
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name
				    : "     ",
	    class,
	    wch->incog_level >= 1 ? RILvl : "",
	    wch->invis_level >= 1 ? "{R|{rW:" : "",
	    wch->invis_level >= 1 ? (lev_name(wch->invis_level)) : "",
	    wch->invis_level >= 1 ? "{R|{w " : "",
	    IS_SET(wch->comm, COMM_AFK) ? "{Y[{GAFK{Y]{w " : "",
	    IS_SET(wch->info, INFO_GHOST) ? "{w|{WGHOST{w|" : "",
	    IS_SET(wch->act, PLR_WANTED) ? "{G({WWANTED{G) {w" : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{R({rKILLER{R) {w" : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{B({bTHIEF{B) {w"  : "",
	    IS_SET(wch->info, INFO_BOZO) ? "{R({GB{YO{GZ{YO{R) {w" : "",
	    IS_SET(wch->olcs, OLC_BUILDER)  ? "{C({cBUILDER{C) {w"  : "",
	    wch->pcdata->olc_on ? "{c<{mOLC{c> {w" : "",			/* Storm */
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title,
	    (see_clan ? clan_table[wch->clan].who_name : ""),
	    ((see_clan && IS_SET(wch->act, PLR_CLANLEADER)) ? "{W[{yLeader{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANGM)) ? "{W[{gGrand Master{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANTRAINER)) ? "{W[{bHead Trainer{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANENFORCER)) ? "{W[{rEnforcer{W]{w" : ""),
	    ((!clan_table[wch->clan].visible && see_clan) ? "{W[{mI{W]{w" : "") );
	add_buf(output,buf);
	}

	else if ((ch-> level < 10 || IS_SET(ch->act, PLR_HELPER)) && !IS_HERO(ch))
      {
	    sprintf( buf, "{W[{c%3s{C %13s{c %s{W]{w %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s{w %s{w %s%s%s%s%s{w\n\r",
	    (lev_imm(wch->level)),
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name
				    : "     ",
	    class,
	    wch->incog_level >= 1 ? RILvl : "",
	    wch->invis_level >= 1 ? "{R|{rW:" : "",
	    wch->invis_level >= 1 ? (lev_name(wch->invis_level)) : "",
	    wch->invis_level >= 1 ? "{R|{w " : "",
	    IS_SET(wch->comm, COMM_AFK) ? "{Y[{GAFK{Y]{w " : "",
	    IS_SET(wch->info, INFO_GHOST) ? "{w|{WGHOST{w|" : "",
	    IS_SET(wch->act, PLR_WANTED) ? "{G({WWANTED{G) {w" : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{R({rKILLER{R){w " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{B({bTHIEF{B){w "  : "",
	    IS_SET(wch->info, INFO_BOZO) ? "{R({GB{YO{GZ{YO{R) {w" : "",
	    IS_SET(wch->act, PLR_HELPER)  ? "{y[{YHELPER{y]{w "  : "",
	    IS_SET(wch->olcs, OLC_BUILDER)  ? "{C({cBUILDER{C){w "  : "",
	    wch->pcdata->olc_on ? "{c<{mOLC{c> {w" : "",			/*Storm*/
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title,
	    (see_clan ? clan_table[wch->clan].who_name : ""),
	    ((see_clan && IS_SET(wch->act, PLR_CLANLEADER)) ? "{W[{yLeader{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANGM)) ? "{W[{gGrand Master{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANTRAINER)) ? "{W[{bHead Trainer{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANENFORCER)) ? "{W[{rEnforcer{W]{w" : ""),
	    ((!clan_table[wch->clan].visible && see_clan) ? "{W[{mI{W]{w" : "") );
	add_buf(output,buf);
	}

	else if ( IS_HERO(ch) )
	{

	    sprintf( buf, "{W[{w%3s{w %13s{w %s{W]{w %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s{w %s{w %s%s%s%s%s%s%s%s{w\n\r",
	    (lev_colname(wch->level)),
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name
				    : "     ",
	    class,
	    wch->timer >=5 ? "{r|{yI{r|{w" : "",
	    wch->incog_level >= 1 ? RILvl : "",
	    wch->invis_level >= 1 ? "{R|{rW:" : "",
	    wch->invis_level >= 1 ? (lev_name(wch->invis_level)) : "",
	    wch->invis_level >= 1 ? "{R|{w" : "",
	    IS_SET(wch->comm, COMM_AFK) ? "{Y|{GA{Y|{w" : "",
	    IS_SET(wch->info, INFO_GHOST) ? "{w|{WG{w|" : "",
	    IS_SET(wch->act, PLR_WANTED) ? "{G|{WW{G|{w" : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{R|{rK{R|{w" : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{B|{bT{B|{w"  : "",
	    IS_SET(wch->act, PLR_HELPER)  ? "{y|{YH{y|{w"  : "",
	    IS_SET(wch->olcs, OLC_BUILDER)  ? "{C|{cB"  : "",
	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_ROOM))  ? "R"  : "",
	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_MOB))  ? "M"  : "",
	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_OBJ))  ? "O"  : "",
	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_RESET))  ? "X"  : "",
	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_ADMIN))  ? "A"  : "",
	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_INSPECTOR)) ? "I" : "",  /* Ferg */
 	    ( IS_SET(ch->olcs, OLC_ADMIN) && IS_SET(wch->olcs, OLC_CODER))  ? "C"  : "",		/* Storm */
	    IS_SET(wch->olcs, OLC_BUILDER)  ? "{C|{w"  : "",
	    wch->pcdata->olc_on ? "{c|{mO{c| {w" : "",			/*Storm*/
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title,
	    (see_clan ? clan_table[wch->clan].who_name : ""),
	    /* Clan position flags modified by Ferg to be shorter, like the rest of the flags for imm-who :P */
	    ((see_clan && IS_SET(wch->act, PLR_CLANLEADER)) ? "{W[{yL{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANGM)) ? "{W[{gGM{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANTRAINER)) ? "{W[{bHT{W]{w" : ""),
	    ((see_clan && IS_SET(wch->info, INFO_CLANENFORCER)) ? "{W[{rE{W]{w" : ""),
	    ((!clan_table[wch->clan].visible && see_clan) ? "{W[{mI{W]{w" : ""),
	    ( (IS_ADMIN(ch) && IS_SET(wch->info, INFO_IMMMULTI)) ? " {R[{rMULTI{R]{w" : ""),
	    ( IS_SET(wch->info, INFO_BOZO) ? " {R({GB{YO{GZ{YO{R){w" : ""),
	    ( is_punished(wch) ? " {R[{rP{R]{w" : "") );
	    add_buf(output,buf);
	}


    }

    sprintf( buf2, "\n\r" );   /* URK_COUNT - 4/12/03 - Replaced 'Players found' with do_count function */
    add_buf(output,buf2);
    page_to_char( buf_string(output), ch );
    free_buf(output);
    do_count(ch, argument);    /* URK_COUNT */
    return;
}

void do_count ( CHAR_DATA *ch, char *argument )

{

    int count;

    DESCRIPTOR_DATA *d;

    char buf[MAX_STRING_LENGTH];



    count = 0;



    for ( d = descriptor_list; d != NULL; d = d->next )

	if ( d->connected == CON_PLAYING && can_see( ch, d->character ) )

	    count++;



    max_on = UMAX(count,max_on);



    if (max_on == count)

	sprintf(buf,"There are %d characters on, the most so far today.\n\r",

	    count);

    else

	sprintf(buf,"There are %d characters on, the most on today was %d.\n\r",

	    count,max_on);



    send_to_char(buf,ch);

}



void do_inventory( CHAR_DATA *ch, char *argument )

{

    send_to_char( "You are carrying:\n\r", ch );

    show_list_to_char( ch->carrying, ch, TRUE, TRUE );

    return;

}



void do_equipment( CHAR_DATA *ch, char *argument )

{

    OBJ_DATA *obj;

    int iWear;

    bool found;



    send_to_char( "You are using:\n\r", ch );

    found = FALSE;

    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )

    {

        if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )

            continue;



        send_to_char( where_name[iWear], ch );

        if ( can_see_obj( ch, obj ) )

        {

            send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );

            send_to_char( "\n\r", ch );

        }

        else

        {

            send_to_char( "something.\n\r", ch );

        }

        found = TRUE;

    }



    if ( !found )

        send_to_char( "Nothing.\n\r", ch );



    return;

}



void do_compare( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    OBJ_DATA *obj1;

    OBJ_DATA *obj2;

    int value1;

    int value2;

    char *msg;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )

    {

	send_to_char( "Compare what to what?\n\r", ch );

	return;

    }



    if ( ( obj1 = get_obj_carry( ch, arg1, ch ) ) == NULL )

    {

	send_to_char( "You do not have that item.\n\r", ch );

	return;

    }



    if (arg2[0] == '\0')

    {

	for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content)

	{

	    if (obj2->wear_loc != WEAR_NONE

	    &&  can_see_obj(ch,obj2)

	    &&  obj1->item_type == obj2->item_type

	    &&  (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )

		break;

	}



	if (obj2 == NULL)

	{

	    send_to_char("You aren't wearing anything comparable.\n\r",ch);

	    return;

	}

    }



    else if ( (obj2 = get_obj_carry(ch,arg2,ch) ) == NULL )

    {

	send_to_char("You do not have that item.\n\r",ch);

	return;

    }



    msg         = NULL;

    value1      = 0;

    value2      = 0;



    if ( obj1 == obj2 )

    {

	msg = "You compare $p to itself.  It looks about the same.";

    }

    else if ( obj1->item_type != obj2->item_type )

    {

	msg = "You can't compare $p and $P.";

    }

    else

    {

	switch ( obj1->item_type )

	{

	default:

	    msg = "You can't compare $p and $P.";

	    break;



	case ITEM_ARMOR:

	    value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];

	    value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];

	    break;



	case ITEM_WEAPON:

	    if (obj1->pIndexData->new_format)

		value1 = (1 + obj1->value[2]) * obj1->value[1];

	    else

		value1 = obj1->value[1] + obj1->value[2];



	    if (obj2->pIndexData->new_format)

		value2 = (1 + obj2->value[2]) * obj2->value[1];

	    else

		value2 = obj2->value[1] + obj2->value[2];

	    break;

	}

    }



    if ( msg == NULL )

    {

	     if ( value1 == value2 ) msg = "$p and $P look about the same.";

	else if ( value1  > value2 ) msg = "$p looks better than $P.";

	else                         msg = "$p looks worse than $P.";

    }



    act( msg, ch, obj1, obj2, TO_CHAR );

    return;

}







void do_credits( CHAR_DATA *ch, char *argument )

{

    do_help( ch, "diku" );

    return;

}





/* Modified by Storm so that victims who can't be seen by ch show up as "someone" */



void do_where( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    DESCRIPTOR_DATA *d;

    bool found;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	send_to_char( "Players near you:\n\r", ch );

	found = FALSE;

	for ( d = descriptor_list; d; d = d->next )

	{

	    if ( d->connected == CON_PLAYING

	    && ( victim = d->character ) != NULL

	    &&   !IS_NPC(victim)

	    &&   victim->in_room != NULL

	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)

	    &&   (is_room_owner(ch,victim->in_room)

	    ||    !room_is_private(victim->in_room))

	    &&   victim->in_room->area == ch->in_room->area

	    &&   can_see( ch, victim ) )

	    {

		found = TRUE;

		if ( (victim->in_room == ch->in_room) && can_see(ch, victim) )

		{

			sprintf( buf, "%-28s %s\n\r", victim->name, "Here with you");

		}

		else

		{

			if ( can_see(ch, victim) )

			{

				sprintf( buf, "%-28s %s\n\r", PERS(victim, ch) , "Somewhere nearby");

			}

		}



		send_to_char( buf, ch );

	    }

	}

	if ( !found )

	    send_to_char( "None\n\r", ch );

    }

    else

    {

	found = FALSE;

	for ( victim = char_list; victim != NULL; victim = victim->next )

	{

	    if ( victim->in_room != NULL

	    &&   victim->in_room->area == ch->in_room->area

	    &&   !IS_AFFECTED(victim, AFF_HIDE)

	    &&   !IS_AFFECTED(victim, AFF_SNEAK)

	    &&   can_see( ch, victim )

	    &&   is_name( arg, victim->name ) )

	    {

		found = TRUE;

		sprintf( buf, "%-28s %s\n\r",

		    PERS(victim, ch), victim->in_room->name );

		send_to_char( buf, ch );

		break;

	    }

	}

	if ( !found )

	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );

    }



    return;

}


void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	send_to_char("Don't even think about it.\n\r",ch);
	return;
    }

    diff = victim->level - ch->level;
	
	//msg = "tempstring";
	
	if (ch->level < 40)
	{
	 if ( diff <= -10 ) msg = "You can kill $N naked and weaponless.";
    else if ( diff <=  -5 ) msg = "$N is no match for you.";
    else if ( diff <=  -2 ) msg = "$N looks like an easy kill.";
    else if ( diff <=   1 ) msg = "The perfect match!";
    else if ( diff <=   4 ) msg = "$N says 'Do you feel lucky, punk?'.";
    else if ( diff <=   9 ) msg = "$N laughs at you mercilessly.";
    else                    msg = "Why not just hang yourself? It'd be much less messy.";
	}
	else //if (ch->level < 100)
	{
	 if ( diff <= -20 ) msg = "You can kill $N naked and weaponless.";
    else if ( diff <=  -10 ) msg = "$N is no match for you.";
    else if ( diff <=  -5 ) msg = "$N looks like an easy kill.";
    else if ( diff <=   10 ) msg = "The perfect match!";
    else if ( diff <=   20 ) msg = "$N says 'Do you feel lucky, punk?'.";
    else if ( diff <=   30 ) msg = "$N laughs at you mercilessly.";
    else                    msg = "Why not just hang yourself? It'd be much less messy.";	
	}
	
    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}







void set_title( CHAR_DATA *ch, char *title )

{

    char buf[MAX_STRING_LENGTH];



    if ( IS_NPC(ch) )

    {

	bug( "Set_title: NPC.", 0 );

	return;

    }



    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' && title[0] != ':')

    {

	buf[0] = ' ';

	strcpy( buf+1, title );

    }

    else

    {

	strcpy( buf, title );

    }



    free_string( ch->pcdata->title );

    ch->pcdata->title = str_dup( buf );

    return;

}







void do_title( CHAR_DATA *ch, char *argument )

{

    if ( IS_NPC(ch) )

	return;



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have revoked your title changing privleges.\n\r", ch);

	return;

    }



    if ( argument[0] == '\0' )

    {

	send_to_char( "Change your title to what?\n\r", ch );

	return;

    }



    if ( strlen(argument) > 45 )

	argument[45] = '\0';



    smash_tilde( argument );

    set_title( ch, argument );

    send_to_char( "Ok.\n\r", ch );

}







void do_description( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_SET(ch->comm,COMM_NOCHANNELS))

    {

	send_to_char("The Gods have revoked your description changing privleges.\n\r", ch);

	return;

    }



    if ( argument[0] != '\0' )

    {

	buf[0] = '\0';

	smash_tilde( argument );



	if (argument[0] == '-')

	{

	    int len;

	    bool found = FALSE;



	    if (ch->description == NULL || ch->description[0] == '\0')

	    {

		send_to_char("No lines left to remove.\n\r",ch);

		return;

	    }



	    strcpy(buf,ch->description);



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

			free_string(ch->description);

			ch->description = str_dup(buf);

			send_to_char( "Your description is:\n\r", ch );

			send_to_char( ch->description ? ch->description :

			    "(None).\n\r", ch );

			return;

		    }

		}

	    }

	    buf[0] = '\0';

	    free_string(ch->description);

	    ch->description = str_dup(buf);

	    send_to_char("Description cleared.\n\r",ch);

	    return;

	}

	if ( argument[0] == '+' )

	{

	    if ( ch->description != NULL )

		strcat( buf, ch->description );

	    argument++;

	    while ( isspace(*argument) )

		argument++;

	}



	if ( strlen(buf) >= 1024)

	{

	    send_to_char( "Description too long.\n\r", ch );

	    return;

	}



	strcat( buf, argument );

	strcat( buf, "\n\r" );

	free_string( ch->description );

	ch->description = str_dup( buf );

    }



    send_to_char( "Your description is:\n\r", ch );

    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );

    return;

}







void do_report( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_INPUT_LENGTH];



    sprintf( buf,

	"{gYou say 'I have %d/%d hp %d/%d mana %d/%d mv %d xp.'{w\n\r",

	ch->hit,  ch->max_hit,

	ch->mana, ch->max_mana,

	ch->move, ch->max_move,

	ch->exp   );



    send_to_char( buf, ch );



    sprintf( buf, "{g$n says 'I have %d/%d hp %d/%d mana %d/%d mv %d xp.{w'",

	ch->hit,  ch->max_hit,

	ch->mana, ch->max_mana,

	ch->move, ch->max_move,

	ch->exp   );



    act( buf, ch, NULL, NULL, TO_ROOM );



    return;

}







void do_practice( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    int sn;



    if ( IS_NPC(ch) )

	return;



    if ( argument[0] == '\0' )

    {

	int col;



	col    = 0;

	for ( sn = 0; sn < MAX_SKILL; sn++ )

	{

	    if ( skill_table[sn].name == NULL )

		break;



	    if ( !can_use_skill(ch,sn) )

		continue;



	    sprintf( buf, "%s%-18s {c%3d%%{w  ",

		( is_race_skill(ch,sn) ? "{g" : "{w" ),

		skill_table[sn].name,

		ch->pcdata->learned[sn] );

	    send_to_char( buf, ch );

	    if ( ++col % 3 == 0 )

		send_to_char( "\n\r", ch );

	}



	if ( col % 3 != 0 )

	    send_to_char( "\n\r", ch );



	sprintf( buf, "You have %d practice sessions left.\n\r",

	    ch->practice );

	send_to_char( buf, ch );

	send_to_char( "Racial skills are listed in {ggreen{w.\n\r", ch );

    }

    else

    {

	CHAR_DATA *mob;

	int adept;



	if ( !IS_AWAKE(ch) )

	{

	    send_to_char( "In your dreams, or what?\n\r", ch );

	    return;

	}



	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )

	{

	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )

		break;

	}



	if ( mob == NULL )

	{

	    send_to_char( "You can't do that here.\n\r", ch );

	    return;

	}



	if ( ch->practice <= 0 )

	{

	    send_to_char( "You have no practice sessions left.\n\r", ch );

	    return;

	}





	if (	((sn = find_spell(ch,argument))<0)

		|| IS_NPC(ch)

		|| !can_use_skill(ch,sn) )

	{

		send_to_char( "You can't practice that.\n\r", ch );

	    	return;

	}



	adept = IS_NPC(ch) ? 100 : class_table[ch->class].skill_adept;



	if ( ch->pcdata->learned[sn] >= adept )

	{

	    sprintf( buf, "You are already learned at %s.\n\r",

		skill_table[sn].name );

	    send_to_char( buf, ch );

	}

	else

	{

	    ch->practice--;



	    if (is_race_skill(ch,sn))

	    {

		ch->pcdata->learned[sn] +=

			int_app[get_curr_stat(ch,STAT_INT)].learn /

			(get_econ_value("race_skill_cost"));

	    }

	    else

	    {

		ch->pcdata->learned[sn] +=

			int_app[get_curr_stat(ch,STAT_INT)].learn /

			(skill_table[sn].rating[ch->class] + 1);

	    }

	    if ( ch->pcdata->learned[sn] < adept )

	    {

		act( "You practice $T.",

		    ch, NULL, skill_table[sn].name, TO_CHAR );

		act( "$n practices $T.",

		    ch, NULL, skill_table[sn].name, TO_ROOM );

	    }

	    else

	    {

		ch->pcdata->learned[sn] = adept;

		act( "You are now learned at $T.",

		    ch, NULL, skill_table[sn].name, TO_CHAR );

		act( "$n is now learned at $T.",

		    ch, NULL, skill_table[sn].name, TO_ROOM );

	    }

	}

    }

    return;

}







/*

 * 'Wimpy' originally by Dionysos.

 */

void do_wimpy( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    int wimpy;



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

	wimpy = ch->max_hit / 5;

    else

	wimpy = atoi( arg );



    if ( wimpy < 0 )

    {

	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );

	return;

    }



    if ( wimpy > ch->max_hit/2 )

    {

	send_to_char( "Such cowardice ill becomes you.\n\r", ch );

	return;

    }



    ch->wimpy   = wimpy;

    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );

    send_to_char( buf, ch );

    return;

}







void do_password( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char *pArg;

    char *pwdnew;

    char *p;

    char cEnd;



    if ( IS_NPC(ch) )

	return;



    /*

     * Can't use one_argument here because it smashes case.

     * So we just steal all its code.  Bleagh.

     */

    pArg = arg1;

    while ( isspace(*argument) )

	argument++;



    cEnd = ' ';

    if ( *argument == '\'' || *argument == '"' )

	cEnd = *argument++;



    while ( *argument != '\0' )

    {

	if ( *argument == cEnd )

	{

	    argument++;

	    break;

	}

	*pArg++ = *argument++;

    }

    *pArg = '\0';



    pArg = arg2;

    while ( isspace(*argument) )

	argument++;



    cEnd = ' ';

    if ( *argument == '\'' || *argument == '"' )

	cEnd = *argument++;



    while ( *argument != '\0' )

    {

	if ( *argument == cEnd )

	{

	    argument++;

	    break;

	}

	*pArg++ = *argument++;

    }

    *pArg = '\0';



    if ( arg1[0] == '\0' || arg2[0] == '\0' )

    {

	send_to_char( "Syntax: password <old> <new>.\n\r", ch );

	return;

    }



    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )

    {

	WAIT_STATE( ch, 40 );

	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );

	return;

    }



    if ( strlen(arg2) < 5 )

    {

	send_to_char(

	    "New password must be at least five characters long.\n\r", ch );

	return;

    }



    /*

     * No tilde allowed because of player file format.

     */

    pwdnew = crypt( arg2, ch->name );

    for ( p = pwdnew; *p != '\0'; p++ )

    {

	if ( *p == '~' )

	{

	    send_to_char(

		"New password not acceptable, try again.\n\r", ch );

	    return;

	}

    }



    free_string( ch->pcdata->pwd );

    ch->pcdata->pwd = str_dup( pwdnew );

    save_char_obj( ch );

    send_to_char( "Ok.\n\r", ch );

    return;

}





void do_balance ( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    if (IS_NPC(ch))

	return;



    sprintf( buf, "{wYou have {y%ld{w coins in the bank.\n\r", ch->bank );

    send_to_char( buf, ch );

    sprintf( buf, "{wYou have {y%ld{w gold and {y%ld{w silver with you.\n\r", ch->gold, ch->silver );

    send_to_char( buf, ch );

    return;

}



void do_deposit ( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *banker;

    char arg[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];

    int amnt;





    if (IS_NPC(ch))

	return;



    if ( IS_AFFECTED(ch,AFF_CHARM) )

    {

	send_to_char( "Not while charmed.\n\r", ch );

	return;

    }



    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK) )

    {

	sprintf( buf, "But you are not in a bank.\n\r" );

	send_to_char( buf, ch );

	return;

    }



    banker = NULL;

    for ( banker = ch->in_room->people; banker; banker = banker->next_in_room )

    {

	if ( IS_NPC( banker ) && IS_SET(banker->pIndexData->act, ACT_BANKER) )

	    break;

    }



    if ( !banker )

    {

	sprintf( buf, "The banker is currently not available.\n\r" );

	send_to_char( buf, ch );

	return;

    }



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	sprintf( buf, "How much gold do you wish to deposit?\n\r" );

	send_to_char( buf, ch );

	return;

    }



    amnt = atoi( arg );



    if ( amnt >= (ch->gold + 1) )

    {

	sprintf( buf, "{g%s{g says to you '%s{g, you do not have {y%d{g gold coins.'{w\n\r",

		banker->short_descr,

		ch->name,

		amnt );

	send_to_char( buf, ch );

	return;

    }



    if (amnt < 0 )

    {

	ch->bank = 0;

	ch->gold = 0;

	ch->silver = 0;

	sprintf( buf, "%s is trying to cheat the bank!", ch->name );

	if (IS_SET(banker->comm,COMM_NOSHOUT))

      {

		REMOVE_BIT(banker->comm,COMM_NOSHOUT);

		do_yell( banker, buf );

		SET_BIT(banker->comm,COMM_NOSHOUT);

      }

      else

      {

		do_yell( banker, buf );

      }

	sprintf( buf, "Thank you %s for your donation!", ch->name );

	do_say( banker, buf );

	do_balance( ch, "" );



	sprintf( buf, "%s tried to steal from the bank.", ch->name );

	wiznet(buf,NULL,NULL,WIZ_FLAGS,0,get_trust(ch));

	log_string( log_buf );



	if ( !IS_SET(ch->act, PLR_THIEF) )

	{

	    SET_BIT(ch->act, PLR_THIEF);

	    send_to_char( "*** You are now a THIEF!! ***\n\r", ch );

	}



	if ( !IS_SET(ch->info, INFO_BOZO) )

	{

		SET_BIT(ch->info, INFO_BOZO);

		send_to_char( "You are now a {R({GB{YO{GZ{YO{R){w!\n\r", ch );

	}



	save_char_obj(ch);

	return;

    }



    ch->bank += amnt;

    ch->gold -= amnt;

    sprintf( buf, "{g%s{g says to you '%s{g, your account now contains {y%ld{g coins\n\r after depositing {y%d{g coins.'{w\n\r", banker->short_descr, ch->name, ch->bank, amnt );

    send_to_char( buf, ch );

    return;

}



void do_withdraw ( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *banker;

    char arg[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];

    int amnt;





    if (IS_NPC(ch))

	return;



    if ( IS_AFFECTED(ch,AFF_CHARM) )

    {

	send_to_char( "Not while charmed.\n\r", ch );

	return;

    }



    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK) )

    {

	sprintf( buf, "But you are not in a bank.\n\r" );

	send_to_char( buf, ch );

	return;

    }



    banker = NULL;

    for ( banker = ch->in_room->people; banker; banker = banker->next_in_room )

    {

	if ( IS_NPC( banker ) && IS_SET(banker->pIndexData->act, ACT_BANKER) )

	    break;

    }



    if ( !banker )

    {

	 sprintf( buf, "The banker is currently not available.\n\r" );

	 send_to_char( buf, ch );

	 return;

    }



    one_argument( argument, arg );



    if ( arg[0] == '\0' )

    {

	 sprintf( buf, "How much gold do you wish to withdraw?\n\r" );

	 send_to_char( buf, ch );

	 return;

    }



    amnt = atoi( arg );



    if ( amnt >= (ch->bank + 1) )

    {

	sprintf( buf, "{g%s{g says to you '%s{g, you do not have {y%d{g gold coins in the bank.'{w\n\r", banker->short_descr, ch->name, amnt );

	send_to_char( buf, ch );

	return;

    }



    if (amnt < 0 )

    {

	ch->bank = 0;

	ch->gold = 0;

	ch->silver = 0;

	sprintf( buf, "%s is trying to cheat the bank!", ch->name );

	if (IS_SET(banker->comm,COMM_NOSHOUT))

      {

	REMOVE_BIT(banker->comm,COMM_NOSHOUT);

	do_yell( banker, buf );

	SET_BIT(banker->comm,COMM_NOSHOUT);

      }

      else

      {

	do_yell( banker, buf );

      }

	sprintf( buf, "Thank you %s for your donation!", ch->name );

	do_say( banker, buf );

	do_balance( ch, "" );



	sprintf( buf, "%s tried to steal from the bank.", ch->name );

	wiznet(buf,NULL,NULL,WIZ_FLAGS,0,get_trust(ch));

	log_string( log_buf );

	if ( !IS_SET(ch->act, PLR_THIEF) )

	{

	    SET_BIT(ch->act, PLR_THIEF);

	    send_to_char( "*** You are now a THIEF!! ***\n\r", ch );

	    save_char_obj( ch );

	}



	return;

    }



    ch->gold += amnt;

    ch->bank -= amnt;

    sprintf( buf, "{g%s{g says to you '%s{g, your account now contains {y%ld{g coins\n\r after withdrawing {y%d{g coins.'{w\n\r", banker->short_descr, ch->name, ch->bank, amnt );

    send_to_char( buf, ch );

    return;

}



void do_wiretransfer ( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *banker;

    CHAR_DATA *victim;

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];

    long amnt;

    long fee;

    int count;





    if (IS_NPC(ch))

	return;



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if ( IS_AFFECTED(ch,AFF_CHARM) )

    {

	send_to_char( "Not while charmed.\n\r", ch );

	return;

    }



    if ( arg1[0] == '\0' && arg2[0] == '\0' )

    {

	send_to_char( "{cSyntax{w: wiretransfer {R<{Wtarget{R> <{Wamount{R>{w\n\r", ch );

	send_to_char( "Transfers {R<{Wamount{R>{w from your account to the account of {R<{Wtarget{R>{w.\n\r", ch );

	return;

    }



    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK) )

    {

	sprintf( buf, "But you are not in a bank.\n\r" );

	send_to_char( buf, ch );

	return;

    }



    banker = NULL;

    for ( banker = ch->in_room->people; banker; banker = banker->next_in_room )

    {

	if ( IS_NPC( banker ) && IS_SET(banker->pIndexData->act, ACT_BANKER) )

	    break;

    }



    if ( !banker )

    {

	 sprintf( buf, "The banker is currently not available.\n\r" );

	 send_to_char( buf, ch );

	 return;

    }



    if ( (victim = get_char_room( ch, arg1 )) == NULL )

    {

	send_to_char( "They are not here at the bank with you.\n\r", ch );

	return;

    }



    if ( arg2[0] == '\0' )

    {

	 sprintf( buf, "How much gold do you wish to transfer?\n\r" );

	 send_to_char( buf, ch );

	 return;

    }



    amnt = atoi( arg2 );



    if ( amnt >= (ch->bank + 1) )

    {

	sprintf( buf, "{g%s{g says to you '%s{g, you do not have {y%ld{g gold coins in the bank.'{w\n\r",

		banker->short_descr, ch->name, amnt );

	send_to_char( buf, ch );

	return;

    }



    if (amnt < 0 )

    {

	ch->bank = 0;

	ch->gold = 0;

	ch->silver = 0;

	sprintf( buf, "%s is trying to cheat the bank!", ch->name );

	if (IS_SET(banker->comm,COMM_NOSHOUT))

      {

		REMOVE_BIT(banker->comm,COMM_NOSHOUT);

		do_yell( banker, buf );

		SET_BIT(banker->comm,COMM_NOSHOUT);

      }

      else

      {

		do_yell( banker, buf );

      }

	sprintf( buf, "Thank you %s for your donation!", ch->name );

	do_say( banker, buf );

	do_balance( ch, "" );



	sprintf( buf, "%s tried to steal from the bank.", ch->name );

	wiznet(buf,NULL,NULL,WIZ_FLAGS,0,get_trust(ch));

	log_string( log_buf );

	if ( !IS_SET(ch->act, PLR_THIEF) )

	{

	    SET_BIT(ch->act, PLR_THIEF);

	    send_to_char( "*** You are now a THIEF!! ***\n\r", ch );

	    save_char_obj( ch );

	}



	return;

    }



    fee = 0;



/*

 * Old Way

 *

    for ( count = 0; economy_table[count].name != NULL; ++count)

    {

	if ( !str_prefix( economy_table[count].name , "bank_wire_fee_percent") )

		fee = (amnt * economy_table[count].value)/100;

    }

*/



    /* DEBUG */

    fee = (amnt * get_econ_value("bank_wire_fee_percent"))/100;

    sprintf(buf, "{cEcon Lookup {w:: {gWire Transfer {w:: {cFee {y%d {w: {camnt {y%d {w: {clookup {y%d {w",

	fee, amnt, get_econ_value("bank_wire_fee_percent") );

    wiznet(buf,NULL,NULL,WIZ_DEBUG,0,get_trust(ch));



    ch->bank -= amnt;

    victim->bank += amnt;



    sprintf( buf, "{g%s{g says to you '%s{g, your account now contains {y%ld{g coins\n\r after transfering {y%ld{g coins to %s{g.'{w\n\r",

		banker->short_descr, ch->name, ch->bank, amnt, victim->name );

    send_to_char( buf, ch );

    sprintf( buf, "{g%s{g says to you '%s{g, your account now contains {y%ld{g coins\n\r after %s{g transfered {y%ld{g coins to your account.'{w\n\r",

		banker->short_descr, victim->name, victim->bank, ch->name, amnt);

    send_to_char( buf, victim );



    ch->bank -= fee;

    sprintf( buf, "{g%s{g says to you '%s{g, your account now contains {y%ld{g coins\n\r after the bank fee of {y%ld{g.'{w\n\r",

		banker->short_descr, ch->name, ch->bank, fee );

    send_to_char( buf, ch );



    return;

}

