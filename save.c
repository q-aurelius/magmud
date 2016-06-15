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


int rename(const char *oldfname, const char *newfname);

char *print_flags(int flag)
{
    int count, pos = 0;
    static char buf[52];


    for (count = 0; count < 32;  count++)
    {
        if (IS_SET(flag,1<<count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

    buf[pos] = '\0';

    return buf;
}


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];



/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest ) );
void	fwrite_pet	args( ( CHAR_DATA *pet, FILE *fp) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void    fread_pet	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );



/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */

void save_char_obj( CHAR_DATA *ch )
{
    char strsave[MAX_INPUT_LENGTH];
    extern bool betalock;
    FILE *fp;

    if( betalock )
	return;

    if( IS_NPC(ch) )
	return;
    //
    // Don't save if the character is invalidated.
    // This might happen during the auto-logoff of players.
    // (or other places not yet found out)
    //
    if ( !IS_VALID(ch))
    {
     bug("save_char_obj: Trying to save an invalidated character.\n", 0);
     return;
    }

    if ( ch->desc != NULL && ch->desc->original != NULL )

	ch = ch->desc->original;



#if defined(unix)

    /* create god log */

    if (IS_IMMORTAL(ch) || ch->level >= LEVEL_IMMORTAL)

    {

	fclose(fpReserve);

	sprintf(strsave, "%s%s",GOD_DIR, capitalize(ch->name));

	if ((fp = fopen(strsave,"w")) == NULL)

	{

	    bug("Save_char_obj: fopen",0);

	    perror(strsave);

 	}



	fprintf(fp,"Lev %2d Trust %2d  %s%s\n",

	    ch->level, get_trust(ch), ch->name, ch->pcdata->title);

	fclose( fp );

	fpReserve = fopen( NULL_FILE, "r" );

    }

#endif



    fclose( fpReserve );

    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );

    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )

    {

	bug( "Save_char_obj: fopen", 0 );

	perror( strsave );

    }

    else

    {

	fwrite_char( ch, fp );

	if ( ch->carrying != NULL )

	    fwrite_obj( ch, ch->carrying, fp, 0 );

	/* save the pets */

	if (ch->pet != NULL && ch->pet->in_room == ch->in_room)

	    fwrite_pet(ch->pet,fp);

	fprintf( fp, "#END\n" );

    }

    fclose( fp );

    rename(TEMP_FILE,strsave);

    fpReserve = fopen( NULL_FILE, "r" );

    return;

}







/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn, gn, pos;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"					);

    fprintf( fp, "Name %s~\n",	ch->name							);

    fprintf( fp, "Anymud %d\n",	ch->pcdata->anymud							);
    fprintf( fp, "Rommud %d\n",	ch->pcdata->rommud							);
	fprintf( fp, "Magmud %d\n",	ch->pcdata->magmud							);
	fprintf( fp, "Jedi %d\n",	ch->pcdata->jedi							);
			//Jedi Grayson 24 June 2004
    fprintf( fp, "Id   %ld\n", 	ch->id							);
    fprintf( fp, "LogO %ld\n",	current_time						);
    fprintf( fp, "Vers %d\n",   ch->version								);
    if (ch->short_descr[0] != '\0')
      fprintf( fp, "ShD  %s~\n",	ch->short_descr						);
    if( ch->long_descr[0] != '\0')
	fprintf( fp, "LnD  %s~\n",	ch->long_descr						);
    if (ch->description[0] != '\0')
    	fprintf( fp, "Desc %s~\n",	ch->description						);
    if (ch->prompt != NULL || !str_cmp(ch->prompt,"<%hhp %mm %vmv> "))
        fprintf( fp, "Prom %s~\n",	ch->prompt  						);
    fprintf( fp, "Race %s~\n", pc_race_table[ch->race].name 				);
    if (ch->clan)
    	fprintf( fp, "Clan %s~\n",clan_table[ch->clan].name					);
    if (ch->clanreq)
	fprintf( fp, "ClanReq %s~\n", clan_table[ch->clanreq].name				);
    fprintf( fp, "Sex  %d\n",		ch->sex							);
    fprintf( fp, "Cla  %d\n",		ch->class							);
    fprintf( fp, "Levl %d\n",		ch->level							);
    if (ch->trust != 0)
	fprintf( fp, "Tru  %d\n",	ch->trust							);
    fprintf( fp, "Scro %d\n", 	ch->lines							);
    fprintf( fp, "Room %d\n",
        (  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
        && ch->was_in_room != NULL )
            ? ch->was_in_room->vnum
            : ch->in_room == NULL ? 3001 : ch->in_room->vnum 				);
/* URK_KILLCOUNTER */
    fprintf( fp, "PkPdMkMdSkSd %d %d %d %d %d %d\n",
	ch->pkill, ch->pdeath, ch->mkill, ch->mdeath, ch->slaykill, ch->slaydeath );

    fprintf( fp, "HMV  %d %d %d %d %d %d\n",
	ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move 		);
    if (ch->gold > 0)
      fprintf( fp, "Gold %ld\n",	ch->gold							);
    else
      fprintf( fp, "Gold %d\n", 0									);
    if (ch->silver > 0)
	fprintf( fp, "Silv %ld\n",ch->silver							);
    else
	fprintf( fp, "Silv %d\n",0									);
    if (ch->bank > 0)
      fprintf( fp, "Bank %ld\n",	ch->bank							);
    else
      fprintf( fp, "Bank %d\n", 0									);
    fprintf( fp, "Exp  %d\n",		ch->exp							);
    if (ch->act != 0)
	fprintf( fp, "Act  %s\n",	print_flags(ch->act)					);
    if (ch->affected_by != 0)
	fprintf( fp, "AfBy %s\n",	print_flags(ch->affected_by)				);
	if (ch->affected2_by != 0)
	fprintf( fp, "Af2By %s\n",	print_flags(ch->affected2_by)				);
    fprintf( fp, "Comm %s\n",       print_flags(ch->comm)					);
    fprintf( fp, "cyber %s\n",       print_flags(ch->cyber));
    fprintf( fp, "Info %s\n",       print_flags(ch->info)					);
    if (ch->olcs)
	fprintf( fp, "OLC %s\n",	print_flags(ch->olcs)					);
    if (ch->icf_her)
	fprintf( fp, "ICFHER %s\n",	print_flags(ch->icf_her)				);
    if (ch->icf_ang)
	fprintf( fp, "ICFANG %s\n",	print_flags(ch->icf_ang)				);
    if (ch->icf_stat)
	fprintf( fp, "ICFSTAT %s\n",	print_flags(ch->icf_stat)				);
    if (ch->icf_sup)
	fprintf( fp, "ICFSUP %s\n",	print_flags(ch->icf_sup)				);
    if (ch->wiznet)
    	fprintf( fp, "Wizn %s\n",	print_flags(ch->wiznet)					);
    if (ch->invis_level)
	fprintf( fp, "Invi %d\n", 	ch->invis_level						);
    if (ch->incog_level)
	fprintf(fp,"Inco %d\n",		ch->incog_level						);
    fprintf( fp, "Pos  %ld\n",
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position 				);
    if (ch->practice != 0)
    	fprintf( fp, "Prac %d\n",	ch->practice						);
    if (ch->train != 0)
	fprintf( fp, "Trai %d\n",	ch->train							);
    if (ch->saving_throw != 0)
	fprintf( fp, "Save  %d\n",	ch->saving_throw						);
    fprintf( fp, "Alig  %d\n",	ch->alignment						);
    if (ch->hitroll != 0)
	fprintf( fp, "Hit   %d\n",	ch->hitroll							);
    if (ch->damroll != 0)
	fprintf( fp, "Dam   %d\n",	ch->damroll							);
	fprintf( fp, "Def	%d\n",  ch->defroll							);
    fprintf( fp, "ACs %d %d %d %d\n",
	ch->armor[0],ch->armor[1],ch->armor[2],ch->armor[3]					);
    if (ch->wimpy !=0 )
	fprintf( fp, "Wimp  %d\n",	ch->wimpy							);
    fprintf( fp, "Attr %d %d %d %d %d %d %d %d\n",
	ch->perm_stat[STAT_STR],
	ch->perm_stat[STAT_INT],
	ch->perm_stat[STAT_WIS],
	ch->perm_stat[STAT_DEX],
	ch->perm_stat[STAT_CON],
	ch->perm_stat[STAT_AGI],
	ch->perm_stat[STAT_MCH],
	ch->perm_stat[STAT_TCH] 									);

    fprintf (fp, "AMod %d %d %d %d %d %d %d %d\n",
	ch->mod_stat[STAT_STR],
	ch->mod_stat[STAT_INT],
	ch->mod_stat[STAT_WIS],
	ch->mod_stat[STAT_DEX],
	ch->mod_stat[STAT_CON],
	ch->mod_stat[STAT_AGI],
	ch->mod_stat[STAT_MCH],
	ch->mod_stat[STAT_TCH] 										);

    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum %d\n",	ch->pIndexData->vnum					);
    }
    else
    {
	fprintf( fp, "Sec  %d\n",    ch->pcdata->security					);
	if ( IS_SET( ch->info, INFO_GHOST ) )
	{
		fprintf( fp, "DRoom %d\n", 	ch->pcdata->died_in_room->vnum 		);
		fprintf( fp, "GTimer %d\n", 	ch->pcdata->timer_ghost 			);
	}
	fprintf( fp, "PTimer %d %d %d %d %d %d %d %d %d\n",
		ch->pcdata->timer_punished[PUNISH_NOCHAN],
		ch->pcdata->timer_punished[PUNISH_NOEMOTE],
		ch->pcdata->timer_punished[PUNISH_NOTELL],
		ch->pcdata->timer_punished[PUNISH_NOSHOUT],
		ch->pcdata->timer_punished[PUNISH_BOZO],
		ch->pcdata->timer_punished[PUNISH_WANTED],
		ch->pcdata->timer_punished[PUNISH_FREEZE],
		ch->pcdata->timer_punished[PUNISH_PRISON],
		ch->pcdata->timer_punished[PUNISH_EAT] );
	fprintf( fp, "Plyd %d\n", ch->played + (int) (current_time - ch->logon)		);
	fprintf( fp, "PermPlyd %d\n",
		(ch->perm_played < ch->played) ?
		(ch->played + (int) (current_time - ch->logon)) :
		(ch->perm_played + (int) (current_time - ch->logon))				);
	fprintf( fp, "Not  %ld %ld %ld %ld %ld %ld %ld %ld\n",
		ch->pcdata->last_note,ch->pcdata->last_idea,ch->pcdata->last_penalty,
		ch->pcdata->last_news,ch->pcdata->last_changes,ch->pcdata->last_immnote,
		ch->pcdata->last_clannote,ch->pcdata->last_rpnote				);
	fprintf( fp, "Pass %s~\n",	ch->pcdata->pwd						);

	if (ch->pcdata->bamfin[0] != '\0')
	    fprintf( fp, "Bin  %s~\n",	ch->pcdata->bamfin				);
	if (ch->pcdata->bamfout[0] != '\0')
		fprintf( fp, "Bout %s~\n",	ch->pcdata->bamfout				);
	if (ch->pcdata->roomin[0] != '\0')
		fprintf( fp, "Rmin %s~\n",	ch->pcdata->roomin				);
	if (ch->pcdata->roomout[0] != '\0')
		fprintf( fp, "Rmout %s~\n",	ch->pcdata->roomout				);
	if (ch->pcdata->wiziin[0] != '\0')
		fprintf( fp,  "Win %s~\n",	ch->pcdata->wiziin				);
	if (ch->pcdata->wiziout[0] != '\0')
		fprintf( fp,  "Wout %s~\n",	ch->pcdata->wiziout				);
	fprintf( fp, "Titl %s~\n",		ch->pcdata->title					);
	fprintf( fp, "Bounty %d\n",		ch->pcdata->bounty  				);
    	fprintf( fp, "Pnts %d\n",   		ch->pcdata->points      			);
	fprintf( fp, "TSex %d\n",		ch->pcdata->true_sex				);
	fprintf( fp, "LLev %d\n",		ch->pcdata->last_level				);
	fprintf( fp, "HMVP %d %d %d\n",
		ch->pcdata->perm_hit,
	  	ch->pcdata->perm_mana,
	   	ch->pcdata->perm_move									);
	fprintf( fp, "Cnd  %d %d %d %d\n",
		ch->pcdata->condition[0],
		ch->pcdata->condition[1],
		ch->pcdata->condition[2],
		ch->pcdata->condition[3] 								);
	if (ch->pcdata->questpoints != 0)
		fprintf( fp, "QuestPnts %d\n",	ch->pcdata->questpoints 		);
	if (ch->pcdata->nextquest != 0)
		fprintf( fp, "QuestNext %d\n",	ch->pcdata->nextquest			);
	else if (ch->pcdata->countdown != 0)
		fprintf( fp, "QuestNext %d\n",  10              				);
	fprintf( fp, "Incr %d\n",		ch->pcdata->incarnations 			);

    if (ch->pcdata->real_name)
	fprintf( fp, "Real %s~\n",		ch->pcdata->real_name				);
    if (ch->pcdata->host)
	fprintf( fp, "Host %s~\n",		ch->pcdata->host					);
    if (ch->pcdata->email)
	fprintf( fp, "Email %s~\n",		ch->pcdata->email					);
    if (ch->pcdata->web)
	fprintf( fp, "Web %s~\n",		ch->pcdata->web 					);

    if (ch->pcdata->aim)
	fprintf( fp, "Aim %s~\n",		ch->pcdata->aim 					);

    if (ch->pcdata->msn)
	fprintf( fp, "Msn %s~\n",		ch->pcdata->msn 					);

    if (ch->pcdata->yahoo)
	fprintf( fp, "Yahoo %s~\n",		ch->pcdata->yahoo 					);

    if (ch->pcdata->icq)
	fprintf( fp, "Icq %s~\n",		ch->pcdata->icq 					);

	/* write alias */

        for (pos = 0; pos < MAX_ALIAS; pos++)

	{

	    if (ch->pcdata->alias[pos] == NULL

	    ||  ch->pcdata->alias_sub[pos] == NULL)

		break;



	    fprintf(fp,"Alias %s %s~\n",ch->pcdata->alias[pos],

		    ch->pcdata->alias_sub[pos]);

	}



	for ( sn = 0; sn < MAX_SKILL; sn++ )

	{

	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )

	    {

		fprintf( fp, "Sk %d '%s'\n",

		    ch->pcdata->learned[sn], skill_table[sn].name );

	    }

	}



	for ( gn = 0; gn < MAX_GROUP; gn++ )

        {

            if ( group_table[gn].name != NULL && ch->pcdata->group_known[gn])

            {

                fprintf( fp, "Gr '%s'\n",group_table[gn].name);

            }

        }

    }



    for ( paf = ch->affected; paf != NULL; paf = paf->next )

    {

	if (paf->type < 0 || paf->type>= MAX_SKILL)

	    continue;



	fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
	    skill_table[paf->type].name,
	    paf->where,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }

    fprintf( fp, "End\n\n" );
    return;
}



/* write a pet */
void fwrite_pet( CHAR_DATA *pet, FILE *fp)
{
    AFFECT_DATA *paf;

    fprintf(fp,"#PET\n");

    fprintf(fp,"Vnum %d\n",pet->pIndexData->vnum);

    fprintf(fp,"Name %s~\n", pet->name);
    fprintf(fp,"LogO %ld\n", current_time);
    if (pet->short_descr != pet->pIndexData->short_descr)
    	fprintf(fp,"ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
    	fprintf(fp,"LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
    	fprintf(fp,"Desc %s~\n", pet->description);
    if (pet->race != pet->pIndexData->race)
    	fprintf(fp,"Race %s~\n", race_table[pet->race].name);
    if (pet->clan)
        fprintf( fp, "Clan %s~\n",clan_table[pet->clan].name);
    fprintf(fp,"Sex  %d\n", pet->sex);
    if (pet->level != pet->pIndexData->level)
    	fprintf(fp,"Levl %d\n", pet->level);
    fprintf(fp, "HMV  %d %d %d %d %d %d\n",
    	pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->move, pet->max_move);
    if (pet->gold > 0)
    	fprintf(fp,"Gold %ld\n",pet->gold);
    if (pet->silver > 0)
    	fprintf(fp,"Silv %ld\n",pet->silver);
    if (pet->exp > 0)
    	fprintf(fp, "Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act)
    	fprintf(fp, "Act  %s\n", print_flags(pet->act));
    if (pet->affected_by != pet->pIndexData->affected_by)
    	fprintf(fp, "AfBy %s\n", print_flags(pet->affected_by));
    if (pet->comm != 0)
    	fprintf(fp, "Comm %s\n", print_flags(pet->comm));
    fprintf(fp,"Pos  %ld\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
    	fprintf(fp, "Save %d\n", pet->saving_throw);
    if (pet->alignment != pet->pIndexData->alignment)
    	fprintf(fp, "Alig %d\n", pet->alignment);
    if (pet->hitroll != pet->pIndexData->hitroll)
    	fprintf(fp, "Hit  %d\n", pet->hitroll);
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
    	fprintf(fp, "Dam  %d\n", pet->damroll);
    if (pet->defroll != pet->pIndexData->damage[DICE_BONUS])
    	fprintf(fp, "Def  %d\n", pet->defroll);

    fprintf(fp, "ACs  %d %d %d %d\n",

    	pet->armor[0],pet->armor[1],pet->armor[2],pet->armor[3]);

    fprintf(fp, "Attr %d %d %d %d %d %d %d %d\n",
    	pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
    	pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
    	pet->perm_stat[STAT_CON], pet->perm_stat[STAT_AGI],
    	pet->perm_stat[STAT_MCH], pet->perm_stat[STAT_TCH]);

    fprintf(fp, "AMod %d %d %d %d %d %d %d %d\n",
	   	pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
	   	pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
    	pet->mod_stat[STAT_CON],pet->perm_stat[STAT_AGI],
    	pet->perm_stat[STAT_MCH],pet->perm_stat[STAT_TCH]);



    for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
    	if (paf->type < 0 || paf->type >= MAX_SKILL)
    	    continue;

    	fprintf(fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
    	    skill_table[paf->type].name,
    	    paf->where, paf->level, paf->duration, paf->modifier,paf->location,
    	    paf->bitvector);
    }

    fprintf(fp,"End\n");
    return;
}

/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */

    if ( obj->next_content != NULL )

	fwrite_obj( ch, obj->next_content, fp, iNest );



    /*

     * Castrate storage characters.

     */

    if ( (ch->level < obj->level - 2 && obj->item_type != ITEM_CONTAINER)

    ||   obj->item_type == ITEM_KEY

    ||   (obj->item_type == ITEM_MAP && !obj->value[0]))

	return;



    fprintf( fp, "#O\n" );

    fprintf( fp, "Vnum %d\n",   obj->pIndexData->vnum        );

    if (!obj->pIndexData->new_format)

	fprintf( fp, "Oldstyle\n");

    if (obj->enchanted)

	fprintf( fp,"Enchanted\n");

    fprintf( fp, "Nest %d\n",	iNest	  	     );



    /* these data are only used if they do not match the defaults */



    if ( obj->name != obj->pIndexData->name)

    	fprintf( fp, "Name %s~\n",	obj->name		     );

    if ( obj->short_descr != obj->pIndexData->short_descr)

        fprintf( fp, "ShD  %s~\n",	obj->short_descr	     );

    if ( obj->description != obj->pIndexData->description)

        fprintf( fp, "Desc %s~\n",	obj->description	     );

    if ( obj->extra_flags != obj->pIndexData->extra_flags)

        fprintf( fp, "ExtF %d\n",	obj->extra_flags	     );

    if ( obj->wear_flags != obj->pIndexData->wear_flags)

        fprintf( fp, "WeaF %d\n",	obj->wear_flags		     );

    if ( obj->item_type != obj->pIndexData->item_type)

        fprintf( fp, "Ityp %d\n",	obj->item_type		     );

    if ( obj->weight != obj->pIndexData->weight)

        fprintf( fp, "Wt   %d\n",	obj->weight		     );

    if ( obj->condition != obj->pIndexData->condition)

	fprintf( fp, "Cond %d\n",	obj->condition		     );



    /* variable data */



    fprintf( fp, "Wear %d\n",   obj->wear_loc                );

    if (obj->level != obj->pIndexData->level)

        fprintf( fp, "Lev  %d\n",	obj->level		     );

    if (obj->timer != 0)

        fprintf( fp, "Time %d\n",	obj->timer	     );

    fprintf( fp, "Cost %d\n",	obj->cost		     );

    if (obj->value[0] != obj->pIndexData->value[0]

    ||  obj->value[1] != obj->pIndexData->value[1]

    ||  obj->value[2] != obj->pIndexData->value[2]

    ||  obj->value[3] != obj->pIndexData->value[3]

    ||  obj->value[4] != obj->pIndexData->value[4])

    	fprintf( fp, "Val  %d %d %d %d %d\n",

	    obj->value[0], obj->value[1], obj->value[2], obj->value[3],

	    obj->value[4]	     );



    switch ( obj->item_type )

    {

    case ITEM_POTION:

    case ITEM_SCROLL:

    case ITEM_PILL:

	if ( obj->value[1] > 0 )

	{

	    fprintf( fp, "Spell 1 '%s'\n",

		skill_table[obj->value[1]].name );

	}



	if ( obj->value[2] > 0 )

	{

	    fprintf( fp, "Spell 2 '%s'\n",

		skill_table[obj->value[2]].name );

	}



	if ( obj->value[3] > 0 )

	{

	    fprintf( fp, "Spell 3 '%s'\n",

		skill_table[obj->value[3]].name );

	}



	break;



    case ITEM_STAFF:

    case ITEM_WAND:

	if ( obj->value[3] > 0 )

	{

	    fprintf( fp, "Spell 3 '%s'\n",

		skill_table[obj->value[3]].name );

	}



	break;

    }



    for ( paf = obj->affected; paf != NULL; paf = paf->next )

    {

	if (paf->type < 0 || paf->type >= MAX_SKILL)

	    continue;

        fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",

            skill_table[paf->type].name,

            paf->where,

            paf->level,

            paf->duration,

            paf->modifier,

            paf->location,

            paf->bitvector

            );

    }



    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )

    {

	fprintf( fp, "ExDe %s~ %s~\n",

	    ed->keyword, ed->description );

    }



    fprintf( fp, "End\n\n" );



    if ( obj->contains != NULL )

	fwrite_obj( ch, obj->contains, fp, iNest + 1 );



    return;

}







/*

 * Load a char and inventory into a new ch structure.

 */

bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int stat;

    ch = new_char();
    ch->pcdata = new_pcdata();

    d->character					= ch;
    ch->desc					= d;
    ch->name					= str_dup( name );
    ch->id						= get_pc_id();
    ch->version					= 9;
    ch->pcdata->real_name			= str_dup( "" );
    sprintf( buf, "%s [%s]", d->host, d->ipaddy);
    ch->pcdata->host				= str_dup( buf );
    ch->pcdata->email				= str_dup( "" );
    ch->pcdata->web				= str_dup( "" );
    ch->pcdata->aim				= str_dup( "" );
    ch->pcdata->msn				= str_dup( "" );
    ch->pcdata->yahoo			= str_dup( "" );
    ch->pcdata->icq				= str_dup( "" );
    ch->race					= race_lookup("human");
    ch->act						= PLR_NOSUMMON;
    ch->comm					= COMM_COMBINE | COMM_PROMPT;
    ch->prompt 					= str_dup( "{c<%hhp %mm %vmv>{w " );
    ch->pcdata->confirm_delete		= FALSE;
    ch->pcdata->ploaded				= FALSE;
    ch->pcdata->linkdead			= FALSE;
    ch->pcdata->pwd				= str_dup( "" );
    ch->pcdata->bamfin				= str_dup( "" );
    ch->pcdata->bamfout				= str_dup( "" );
    ch->pcdata->roomin				= str_dup( "" );
    ch->pcdata->roomout				= str_dup( "" );
    ch->pcdata->wiziin				= str_dup( "" );
    ch->pcdata->wiziout				= str_dup( "" );
    ch->pcdata->title				= str_dup( "" );
    ch->pcdata->rs_oname			= str_dup( "" );
    ch->pcdata->rs_nname			= str_dup( "" );
    ch->pcdata->rs_sdesc			= str_dup( "" );
    ch->pcdata->rs_ldesc			= str_dup( "" );
    ch->pcdata->bounty				= 0;
    ch->pcdata->timer_ghost			= 0;
    ch->pcdata->jedi				= 0; //Jedi Grayson 24 June 2004
    ch->pcdata->died_in_room			= get_room_index( ROOM_VNUM_PRISON );
    for (stat =0; stat < MAX_STATS; stat++)
	ch->perm_stat[stat]			= 13;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->condition[COND_HUNGER]	= 48;
    ch->pcdata->security			= 0;	/* OLC */
    ch->pcdata->olc_on				= FALSE; /* Storm */
    ch->pcdata->incarnations			= 1;

    ch->pkill                           = 0;
    ch->pdeath                          = 0;
    ch->mkill                           = 0;
    ch->mdeath                          = 0;
    ch->slaykill                        = 0;
    ch->slaydeath                        = 0;
    ch->pcdata->timer_punished[PUNISH_NOCHAN]	= 0;
    ch->pcdata->timer_punished[PUNISH_NOEMOTE]	= 0;
    ch->pcdata->timer_punished[PUNISH_NOTELL]	= 0;
    ch->pcdata->timer_punished[PUNISH_NOSHOUT]	= 0;
    ch->pcdata->timer_punished[PUNISH_BOZO]	= 0;
    ch->pcdata->timer_punished[PUNISH_WANTED]	= 0;
    ch->pcdata->timer_punished[PUNISH_FREEZE]	= 0;
    ch->pcdata->timer_punished[PUNISH_PRISON]	= 0;
    ch->pcdata->timer_punished[PUNISH_EAT]	= 0;

    found = FALSE;
    fclose( fpReserve );

    #if defined(unix)
    /* decompress if .gz file exists */
    sprintf( strsave, "%s%s%s", PLAYER_DIR, capitalize(name),".gz");
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	fclose(fp);
	sprintf(buf,"gzip -dfq %s",strsave);
	system(buf);
    }
    #endif

    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "O"      ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "PET"    ) ) fread_pet  ( ch, fp );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );

    /* Initialize Descriptor Variables */
    sprintf( ch->desc->curcmd,	"-> %s", ch->name );
    sprintf( ch->desc->cmdline,	"-> %s", ch->name );
    sprintf( ch->desc->swcmd,		"-> %s", ch->name );
    ch->desc->iswait		= FALSE;
    ch->desc->swcount		= 0;

    /* initialize race */
    if (found)
    {
	int i;

	if (ch->race == 0)
	    ch->race = race_lookup("human");

	ch->size = pc_race_table[ch->race].size;
	if (ch->size < SIZE_HUGE)
		ch->dam_type = 17; /*punch */
	else
		ch->dam_type = 5;


	for (i = 0; i < 5; i++)
	{
	    if (pc_race_table[ch->race].skills[i] == NULL)
		break;
	    group_add(ch,pc_race_table[ch->race].skills[i],FALSE);
	}
	ch->affected_by 	= ch->affected_by | race_table[ch->race].aff;
	ch->affected2_by 	= ch->affected2_by | race_table[ch->race].aff2;
	ch->imm_flags	= ch->imm_flags   | race_table[ch->race].imm;
	ch->res_flags	= ch->res_flags   | race_table[ch->race].res;
	ch->vuln_flags	= ch->vuln_flags  | race_table[ch->race].vuln;
	ch->form		= race_table[ch->race].form;
	ch->parts		= race_table[ch->race].parts;
    }


    /* RT initialize skills */

    if (found && ch->version < 2)  /* need to add the new skills */
    {
	group_add(ch,"rom basics",FALSE);
	group_add(ch,class_table[ch->class].base_group,FALSE);
	group_add(ch,class_table[ch->class].default_group,TRUE);
	ch->pcdata->learned[gsn_recall] = 50;
    }

    /* fix levels */
    if (found && ch->version < 3 && (ch->level > 35 || ch->trust > 35))
    {
	switch (ch->level)
	{
	    case(40) : ch->level = 60;	break;  	/* imp -> imp */
	    case(39) : ch->level = 58; 	break;	/* god -> supreme */
	    case(38) : ch->level = 56;  	break;	/* deity -> god */
	    case(37) : ch->level = 53;  	break;	/* angel -> demigod */
	}

        switch (ch->trust)
        {
            case(40) : ch->trust = 60;  break;	/* imp -> imp */
            case(39) : ch->trust = 58;  break;	/* god -> supreme */
            case(38) : ch->trust = 56;  break;	/* deity -> god */
            case(37) : ch->trust = 53;  break;	/* angel -> demigod */
            case(36) : ch->trust = 51;  break;	/* hero -> hero */
        }
    }

    /* ream gold */
    if (found && ch->version < 4)
    {
	ch->gold   /= 100;
    }
    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)

#undef KEY

#endif



#define KEY( literal, field, value ) if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE; break; }



/* provided to free strings */

#if defined(KEYS)

#undef KEYS

#endif



#define KEYS( literal, field, value ) if ( !str_cmp( word, literal ) ) { free_string(field); field  = value; fMatch = TRUE; break; }



void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int count = 0;
    int lastlogoff = current_time;
    int percent;

    sprintf(buf,"Loading %s.",ch->name);
    log_string(buf);

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "Act",		ch->act,		fread_flag( fp ) );
	    KEY( "AffectedBy",	ch->affected_by,	fread_flag( fp ) );
	    KEY( "Affected2By",	ch->affected2_by,	fread_flag( fp ) );
	    KEY( "AfBy",	ch->affected_by,	fread_flag( fp ) );
	    KEY( "Af2By",	ch->affected2_by,	fread_flag( fp ) );
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Alig",	ch->alignment,		fread_number( fp ) );

	    if (!str_cmp( word, "Alia"))
	    {
		if (count >= MAX_ALIAS)
		{
		    fread_to_eol(fp);
		    fMatch = TRUE;
		    break;
		}

		ch->pcdata->alias[count] 	= str_dup(fread_word(fp));
		ch->pcdata->alias_sub[count]	= str_dup(fread_word(fp));
		count++;
		fMatch = TRUE;
		break;
	    }

            if (!str_cmp( word, "Alias"))
            {
                if (count >= MAX_ALIAS)
                {
                    fread_to_eol(fp);
                    fMatch = TRUE;
                    break;
                }

                ch->pcdata->alias[count]        = str_dup(fread_word(fp));
                ch->pcdata->alias_sub[count]    = fread_string(fp);
                count++;
                fMatch = TRUE;
                break;
            }

	    if (!str_cmp( word, "AC") || !str_cmp(word,"Armor"))
	    {
		fread_to_eol(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!str_cmp(word,"ACs"))
	    {
		int i;

		for (i = 0; i < 4; i++)
		    ch->armor[i] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }



	    if (!str_cmp(word, "AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug("Fread_char: unknown skill.",0);
		else
		    paf->type = sn;

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= ch->affected;
		ch->affected	= paf;
		fMatch = TRUE;
		break;
	    }

            if (!str_cmp(word, "Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
                else
                    paf->type = sn;

                paf->where  = fread_number(fp);
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
                paf->bitvector  = fread_number( fp );
                paf->next       = ch->affected;
                ch->affected    = paf;
                fMatch = TRUE;
                break;
            }
        KEY( "Aim",		ch->pcdata->aim,	fread_string( fp ) );
        KEY( "Anymud",	ch->pcdata->anymud,	fread_number( fp ) );
	    //sprintf(buf,"Loading %s AttrMod.",ch->name);
		    //log_string(buf);
	    if ( !str_cmp( word, "AttrMod"  ) || !str_cmp(word,"AMod"))
	    {
			int stat;
			int i, race;
			race = ch->race;
			sprintf(buf,"Loading %s AttrMod.",ch->name);
		    log_string(buf);
			if (ch->version >= 8)
			{
				/*
				for (stat = 0; stat < MAX_STATS; stat ++)
				   ch->mod_stat[stat] = fread_number(fp);
				*/
				ch->mod_stat[STAT_STR] = fread_number(fp);
				ch->mod_stat[STAT_INT] = fread_number(fp);
				ch->mod_stat[STAT_WIS] = fread_number(fp);
				ch->mod_stat[STAT_DEX] = fread_number(fp);
				ch->mod_stat[STAT_CON] = fread_number(fp);
				ch->mod_stat[STAT_AGI] = fread_number(fp);
				ch->mod_stat[STAT_MCH] = fread_number(fp);
				ch->mod_stat[STAT_TCH] = fread_number(fp);
				fMatch = TRUE;
			}
			else
			{
				/*
				for (stat = 0; stat < 5; stat ++)
				   ch->mod_stat[stat] = fread_number(fp);
				for (i = 5; i < MAX_STATS; i++)
				    ch->mod_stat[i] = pc_race_table[race].stats[i];
				*/
				ch->mod_stat[STAT_STR] = fread_number(fp);
				ch->mod_stat[STAT_INT] = fread_number(fp);
				ch->mod_stat[STAT_WIS] = fread_number(fp);
				ch->mod_stat[STAT_DEX] = fread_number(fp);
				ch->mod_stat[STAT_CON] = fread_number(fp);
				ch->mod_stat[STAT_AGI] = 0;
				ch->mod_stat[STAT_MCH] = 0;
				ch->mod_stat[STAT_TCH] = 0;
				fMatch = TRUE;
				ch->version = 8;
			}
			sprintf(buf,"Loaded %s AttrMod.",ch->name);
		    log_string(buf);
		break;
	    }
			
	    if ( !str_cmp( word, "AttrPerm" ) || !str_cmp(word,"Attr"))
	    {
			int stat, race, i;
			race = ch->race;
			sprintf(buf,"Loading %s AttrPerm.",ch->name);
		    log_string(buf);
			
			if (ch->version >= 8)
			{
				for (stat = 0; stat < MAX_STATS; stat++)	//Reads in stats Grayson's notes
				    ch->perm_stat[stat] = fread_number(fp);
				fMatch = TRUE;
			}
			else
			{
				for (stat = 0; stat < 5; stat++)	//Reads in stats Grayson's notes
			    	ch->perm_stat[stat] = fread_number(fp);
			    for (i = 5; i < MAX_STATS; i++)
				    ch->perm_stat[i] = pc_race_table[race].stats[i];
			    fMatch = TRUE;
			    //ch->version = 8;
			}
			sprintf(buf,"Loaded %s AttrPerm.",ch->name);
		    log_string(buf);
			break;
	    }
	    break;
		//sprintf(buf,"Loaded %s AttrPerm",ch->name);
		    //log_string(buf);
		

	case 'B':
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( "Bank",		ch->bank,			fread_number( fp ) );
	    KEY( "Bin",		ch->pcdata->bamfin,	fread_string( fp ) );
          KEY( "Bounty",  ch->pcdata->bounty,	fread_number( fp ) );
	    KEY( "Bout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    break;

	case 'C':
	    KEY( "Class",	ch->class,		fread_number( fp ) );
	    KEY( "Cla",		ch->class,		fread_number( fp ) );
	    //KEY( "Clan",	ch->clan,	clan_lookup(fread_string(fp)));
	    // When reading the Clan/Race of a player, memory is lost.
   		if ( !str_cmp( word, "Clan" ) )
        {
          char *tmp = fread_string(fp);
          ch->clan = clan_lookup(tmp);
          free_string(tmp);
          fMatch = TRUE;
          break;
        }
	    KEY( "ClanReq", ch->clanreq, clan_lookup(fread_string(fp)));

	    if ( !str_cmp( word, "Condition" ) || !str_cmp(word,"Cond"))
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
            if (!str_cmp(word,"Cnd"))
            {
                ch->pcdata->condition[0] = fread_number( fp );
                ch->pcdata->condition[1] = fread_number( fp );
                ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
	    KEY("Comm",		ch->comm,		fread_flag( fp ) );
         KEY("Cyber",	ch->cyber,		fread_flag( fp ) );


	    break;

	case 'D':
	    KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	    KEY( "Dam",		ch->damroll,		fread_number( fp ) );
	    KEY( "Def",		ch->defroll,		fread_number( fp ) );
	    //KEY( "Defroll",		ch->defroll,		fread_number( fp ) );
	    KEY( "Description",	ch->description,		fread_string( fp ) );
	    KEY( "Desc",		ch->description,		fread_string( fp ) );
	    if ( !str_cmp( word, "DRoom" ) )
	    {
		ch->pcdata->died_in_room = get_room_index( fread_number( fp ) );
		if ( ch->pcdata->died_in_room == NULL )
		    ch->pcdata->died_in_room = get_room_index( ROOM_VNUM_PRISON );
	    }
	    break;

	case 'E':
	    KEY( "Email",		ch->pcdata->email,	fread_string( fp ) );
	    if ( !str_cmp( word, "End" ) )
	    {
    		/* adjust hp mana move up  -- here for speed's sake */
    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

		percent = UMIN(percent,100);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
        	    ch->hit	+= (ch->max_hit - ch->hit) * percent / 100;
        	    ch->mana    += (ch->max_mana - ch->mana) * percent / 100;
        	    ch->move    += (ch->max_move - ch->move)* percent / 100;
    		}
		return;
	    }

	    KEY( "Exp",		ch->exp,		fread_number( fp ) );
	    break;

	case 'G':
	    KEY( "Gold",		ch->gold,				fread_number( fp ) );
	    KEY( "GTimer",	ch->pcdata->timer_ghost,	fread_number( fp ) );
            if ( !str_cmp( word, "Group" )  || !str_cmp(word,"Gr"))
            {
                int gn;
                char *temp;

                temp = fread_word( fp ) ;
                gn = group_lookup(temp);
                /* gn    = group_lookup( fread_word( fp ) ); */
                if ( gn < 0 )
                {
                    fprintf(stderr,"%s",temp);
                    bug( "Fread_char: unknown group. ", 0 );
                }
                else
		    gn_add(ch,gn);
                fMatch = TRUE;
            }
	    break;

	case 'H':
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );
	    KEY( "Hit",		ch->hitroll,		fread_number( fp ) );
	    KEY( "Host",		ch->pcdata->host,		fread_string( fp ) );

	    if ( !str_cmp( word, "HpManaMove" ) || !str_cmp(word,"HMV"))
	    {
		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->max_mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !str_cmp( word, "HpManaMovePerm" ) || !str_cmp(word,"HMVP"))
            {
                ch->pcdata->perm_hit	= fread_number( fp );
                ch->pcdata->perm_mana   = fread_number( fp );
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    break;

	case 'I':
	    KEY( "ICFHER",	ch->icf_her,			fread_flag( fp ) );
	    KEY( "ICFANG",	ch->icf_ang,			fread_flag( fp ) );
	    KEY( "ICFSTAT",	ch->icf_stat,			fread_flag( fp ) );
	    KEY( "ICFSUP",	ch->icf_sup,			fread_flag( fp ) );
	    KEY( "Icq",		ch->pcdata->icq,     	fread_string( fp ) );
	    KEY( "Id",		ch->id,				fread_number( fp ) );
	    KEY( "Info",		ch->info,				fread_flag( fp ) );
	    KEY( "InvisLevel",	ch->invis_level,			fread_number( fp ) );
	    KEY( "Inco",		ch->incog_level,			fread_number( fp ) );
	    KEY( "Invi",		ch->invis_level,			fread_number( fp ) );
		KEY( "Incr",		ch->pcdata->incarnations, 	fread_number(fp) );
	    break;

	case 'J':
		KEY( "Jedi",		ch->pcdata->jedi, 	fread_number(fp) );
		break;

	case 'L':
	    KEY( "LastLevel",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "LLev",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Lev",		ch->level,		fread_number( fp ) );
	    KEY( "Levl",	ch->level,		fread_number( fp ) );
	    KEY( "LogO",	lastlogoff,		fread_number( fp ) );
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );
	    KEY( "LnD",		ch->long_descr,		fread_string( fp ) );
	    break;
    case 'M':
        KEY( "Magmud",     ch->pcdata->magmud,     fread_number( fp ) );
        KEY( "Msn",		   ch->pcdata->msn,	        fread_string( fp ) );
        break;
	case 'N':
	    KEYS( "Name",	ch->name,		fread_string( fp ) );
	    KEY( "Note",	ch->pcdata->last_note,	fread_number( fp ) );
	    if (!str_cmp(word,"Not"))
	    {
		ch->pcdata->last_note			= fread_number(fp);
		ch->pcdata->last_idea			= fread_number(fp);
		ch->pcdata->last_penalty		= fread_number(fp);
		ch->pcdata->last_news			= fread_number(fp);
		ch->pcdata->last_changes		= fread_number(fp);
		if ( ch->version >= 6 )
		{
			ch->pcdata->last_immnote		= fread_number(fp);
			ch->pcdata->last_clannote		= fread_number(fp);
			ch->pcdata->last_rpnote			= fread_number(fp);
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'O':
	    KEY("OLC",		ch->olcs,		fread_flag( fp ) );
		break;

	case 'P':
	    KEY( "Password",	ch->pcdata->pwd,		fread_string( fp ) );
	    KEY( "Pass",		ch->pcdata->pwd,		fread_string( fp ) );
	    KEY( "Played",	ch->played,			fread_number( fp ) );
	    KEY( "Plyd",		ch->played,			fread_number( fp ) );
	    KEY( "PermPlyd",	ch->perm_played,		fread_number( fp ) );
	    KEY( "Points",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( "Pnts",		ch->pcdata->points,	fread_number( fp ) );
	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    KEY( "Pos",		ch->position,		fread_number( fp ) );
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    KEY( "Prac",		ch->practice,		fread_number( fp ) );

	    if ( !str_cmp( word, "PkPdMkMdSkSd" ) )
		{
			ch->pkill	= fread_number( fp );
			ch->pdeath	= fread_number( fp );
			ch->mkill	= fread_number( fp );
			ch->mdeath	= fread_number( fp );
			ch->slaykill	= fread_number( fp );
			ch->slaydeath	= fread_number( fp );
			fMatch = TRUE;
			break;
		}

	    KEYS( "Prompt",     ch->prompt,             fread_string( fp ) );
 	    KEYS( "Prom",		ch->prompt,			fread_string( fp ) );
	    if (!str_cmp(word,"PTimer"))
	    {
		ch->pcdata->timer_punished[PUNISH_NOCHAN]		= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_NOEMOTE]	= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_NOTELL]		= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_NOSHOUT]	= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_BOZO]		= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_WANTED]		= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_FREEZE]		= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_PRISON]		= fread_number(fp);
		ch->pcdata->timer_punished[PUNISH_EAT]		= fread_number(fp);

		fMatch = TRUE;
		break;
	    }
	    break;


        case 'Q':
            KEY( "QuestPnts",   ch->pcdata->questpoints,        fread_number( fp ) );
            KEY( "QuestNext",   ch->pcdata->nextquest,          fread_number( fp ) );
            break;

	case 'R':
	   // KEY( "Race",        ch->race, race_lookup(fread_string( fp )) );
	   // When reading the Clan/Race of a player, memory is lost.
	   if ( !str_cmp( word, "Race" ) )
	   {
         char *tmp = fread_string(fp);
         ch->race = race_lookup(tmp);
         free_string(tmp);
         fMatch = TRUE;
         break;
       }
	    KEY( "Real",		ch->pcdata->real_name,	fread_string( fp ) );
	    KEY( "Rmin",		ch->pcdata->roomin,	fread_string( fp ) );
	    KEY( "Rmout",		ch->pcdata->roomout,	fread_string( fp ) );
	    KEY( "Rommud",		ch->pcdata->rommud,	    fread_number( fp ) );

	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
	    }

	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Save",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Scro",	ch->lines,		fread_number( fp ) );
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		ch->short_descr,	fread_string( fp ) );
	    KEY( "Sec",         ch->pcdata->security,	fread_number( fp ) );	/* OLC */
            KEY( "Silv",        ch->silver,             fread_number( fp ) );


	    if ( !str_cmp( word, "Skill" ) || !str_cmp(word,"Sk"))
	    {
		int sn;
		int value;
		char *temp;

		value = fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		/* sn    = skill_lookup( fread_word( fp ) ); */
		if ( sn < 0 )
		{
		    fprintf(stderr,"%s",temp);
		    bug( "Fread_char: unknown skill. ", 0 );
		}
		else
		    ch->pcdata->learned[sn] = value;
		fMatch = TRUE;
	    }

	    break;

	case 'T':
        KEY( "TrueSex",     ch->pcdata->true_sex,  	fread_number( fp ) );
	    KEY( "TSex",	ch->pcdata->true_sex,   fread_number( fp ) );
	    KEY( "Trai",	ch->train,		fread_number( fp ) );
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );
	    KEY( "Tru",		ch->trust,		fread_number( fp ) );

	    if ( !str_cmp( word, "Title" )  || !str_cmp( word, "Titl"))
	    {
		ch->pcdata->title = fread_string( fp );
    		if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ','
		&&  ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    free_string( ch->pcdata->title );
		    ch->pcdata->title = str_dup( buf );
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
	    KEY( "Version",     ch->version,		fread_number ( fp ) );
	    KEY( "Vers",	ch->version,		fread_number ( fp ) );
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "Web",		ch->pcdata->web,	fread_string( fp ) );
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wimp",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Win",	ch->pcdata->wiziin,	fread_string( fp ) ); /* Ferg-wizicust */
	    KEY( "Wout",	ch->pcdata->wiziout,	fread_string( fp ) ); /* Ferg-wizicust */
	    KEY( "Wizn",	ch->wiznet,		fread_flag( fp ) );
	    break;
	case 'Y':
	    KEY( "Yahoo",		ch->pcdata->yahoo,	fread_string( fp ) );
	    break;
	}
	if ( !fMatch )
	{
	    bug( "Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}

/* load a pet from the forgotten reaches */
void fread_pet( CHAR_DATA *ch, FILE *fp )
{

    char *word;

    CHAR_DATA *pet;

    bool fMatch;

    int lastlogoff = current_time;

    int percent;



    /* first entry had BETTER be the vnum or we barf */

    word = feof(fp) ? "END" : fread_word(fp);

    if (!str_cmp(word,"Vnum"))

    {

    	int vnum;



    	vnum = fread_number(fp);

    	if (get_mob_index(vnum) == NULL)

	{

    	    bug("Fread_pet: bad vnum %d.",vnum);

	    pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));

	}

    	else

    	    pet = create_mobile(get_mob_index(vnum));

    }

    else

    {

        bug("Fread_pet: no vnum in file.",0);

        pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));

    }



    for ( ; ; )

    {

    	word 	= feof(fp) ? "END" : fread_word(fp);

    	fMatch = FALSE;



    	switch (UPPER(word[0]))

    	{

    	case '*':

    	    fMatch = TRUE;

    	    fread_to_eol(fp);

    	    break;



    	case 'A':

    	    KEY( "Act",		pet->act,		fread_flag(fp));

    	    KEY( "AfBy",	pet->affected_by,	fread_flag(fp));

    	    KEY( "Alig",	pet->alignment,		fread_number(fp));



    	    if (!str_cmp(word,"ACs"))

    	    {

    	    	int i;



    	    	for (i = 0; i < 4; i++)

    	    	    pet->armor[i] = fread_number(fp);

    	    	fMatch = TRUE;

    	    	break;

    	    }



    	    if (!str_cmp(word,"AffD"))

    	    {

    	    	AFFECT_DATA *paf;

    	    	int sn;



    	    	paf = new_affect();



    	    	sn = skill_lookup(fread_word(fp));

    	     	if (sn < 0)

    	     	    bug("Fread_char: unknown skill.",0);

    	     	else

    	     	   paf->type = sn;



    	     	paf->level	= fread_number(fp);

    	     	paf->duration	= fread_number(fp);

    	     	paf->modifier	= fread_number(fp);

    	     	paf->location	= fread_number(fp);

    	     	paf->bitvector	= fread_number(fp);

    	     	paf->next	= pet->affected;

    	     	pet->affected	= paf;

    	     	fMatch		= TRUE;

    	     	break;

    	    }



            if (!str_cmp(word,"Affc"))

            {

                AFFECT_DATA *paf;

                int sn;



                paf = new_affect();



                sn = skill_lookup(fread_word(fp));

                if (sn < 0)

                    bug("Fread_char: unknown skill.",0);

                else

                   paf->type = sn;



		paf->where	= fread_number(fp);

                paf->level      = fread_number(fp);

                paf->duration   = fread_number(fp);

                paf->modifier   = fread_number(fp);

                paf->location   = fread_number(fp);

                paf->bitvector  = fread_number(fp);

                paf->next       = pet->affected;

                pet->affected   = paf;

                fMatch          = TRUE;

                break;

            }



    	    if (!str_cmp(word,"AMod"))

    	    {

    	     	int stat;



    	     	for (stat = 0; stat < MAX_STATS; stat++)

    	     	    pet->mod_stat[stat] = fread_number(fp);

    	     	fMatch = TRUE;

    	     	break;

    	    }



    	    if (!str_cmp(word,"Attr"))

    	    {

    	         int stat;



    	         for (stat = 0; stat < MAX_STATS; stat++)

    	             pet->perm_stat[stat] = fread_number(fp);

    	         fMatch = TRUE;

    	         break;

    	    }

    	    break;



    	 case 'C':

             KEY( "Clan",       pet->clan,       clan_lookup(fread_string(fp)));

    	     KEY( "Comm",	pet->comm,		fread_flag(fp));

    	     break;



    	 case 'D':
    	     KEY( "Dam",	pet->damroll,		fread_number(fp));
    	     KEY( "Desc",	pet->description,	fread_string(fp));
    		 KEY( "Def",	pet->defroll,		fread_number(fp));	
    	     break;



    	 case 'E':

    	     if (!str_cmp(word,"End"))

	     {

		pet->leader = ch;

		pet->master = ch;

		ch->pet = pet;

    		/* adjust hp mana move up  -- here for speed's sake */

    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);



    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)

    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))

    		{

		    percent = UMIN(percent,100);

    		    pet->hit	+= (pet->max_hit - pet->hit) * percent / 100;

        	    pet->mana   += (pet->max_mana - pet->mana) * percent / 100;

        	    pet->move   += (pet->max_move - pet->move)* percent / 100;

    		}

    	     	return;

	     }

    	     KEY( "Exp",	pet->exp,		fread_number(fp));

    	     break;



    	 case 'G':

    	     KEY( "Gold",	pet->gold,		fread_number(fp));

    	     break;



    	 case 'H':

    	     KEY( "Hit",	pet->hitroll,		fread_number(fp));



    	     if (!str_cmp(word,"HMV"))

    	     {

    	     	pet->hit	= fread_number(fp);

    	     	pet->max_hit	= fread_number(fp);

    	     	pet->mana	= fread_number(fp);

    	     	pet->max_mana	= fread_number(fp);

    	     	pet->move	= fread_number(fp);

    	     	pet->max_move	= fread_number(fp);

    	     	fMatch = TRUE;

    	     	break;

    	     }

    	     break;



     	case 'L':

    	     KEY( "Levl",	pet->level,		fread_number(fp));

    	     KEY( "LnD",	pet->long_descr,	fread_string(fp));

	     KEY( "LogO",	lastlogoff,		fread_number(fp));

    	     break;



    	case 'N':

    	     KEY( "Name",	pet->name,		fread_string(fp));

    	     break;



    	case 'P':

    	     KEY( "Pos",	pet->position,		fread_number(fp));

    	     break;



	case 'R':

    	    KEY( "Race",	pet->race, race_lookup(fread_string(fp)));

    	    break;



    	case 'S' :

    	    KEY( "Save",	pet->saving_throw,	fread_number(fp));

    	    KEY( "Sex",		pet->sex,		fread_number(fp));

    	    KEY( "ShD",		pet->short_descr,	fread_string(fp));

            KEY( "Silv",        pet->silver,            fread_number( fp ) );

    	    break;



    	if ( !fMatch )

    	{

    	    bug("Fread_pet: no match.",0);

    	    fread_to_eol(fp);

    	}



    	}

    }

}



extern	OBJ_DATA	*obj_free;



void fread_obj( CHAR_DATA *ch, FILE *fp )

{

    OBJ_DATA *obj;

    char *word;

    int iNest;

    bool fMatch;

    bool fNest;

    bool fVnum;

    bool first;

    bool new_format;  /* to prevent errors */

    bool make_new;    /* update object */



    fVnum = FALSE;

    obj = NULL;

    first = TRUE;  /* used to counter fp offset */

    new_format = FALSE;

    make_new = FALSE;



    word   = feof( fp ) ? "End" : fread_word( fp );

    if (!str_cmp(word,"Vnum" ))

    {

        int vnum;

	first = FALSE;  /* fp will be in right place */



        vnum = fread_number( fp );

        if (  get_obj_index( vnum )  == NULL )

	{

            bug( "Fread_obj: bad vnum %d.", vnum );

		return;

	}

        else

	{

	    obj = create_object(get_obj_index(vnum),-1);

	    new_format = TRUE;

	}



    }



    if (obj == NULL)  /* either not found or old style */

    {

    	obj = new_obj();

    	obj->name		= str_dup( "" );

    	obj->short_descr	= str_dup( "" );

    	obj->description	= str_dup( "" );

    }



    fNest		= FALSE;

    fVnum		= TRUE;

    iNest		= 0;



    for ( ; ; )

    {

	if (first)

	    first = FALSE;

	else

	    word   = feof( fp ) ? "End" : fread_word( fp );

	fMatch = FALSE;



	switch ( UPPER(word[0]) )

	{

	case '*':

	    fMatch = TRUE;

	    fread_to_eol( fp );

	    break;



	case 'A':

	    if (!str_cmp(word,"AffD"))

	    {

		AFFECT_DATA *paf;

		int sn;



		paf = new_affect();



		sn = skill_lookup(fread_word(fp));

		if (sn < 0)

		    bug("Fread_obj: unknown skill.",0);

		else

		    paf->type = sn;



		paf->level	= fread_number( fp );

		paf->duration	= fread_number( fp );

		paf->modifier	= fread_number( fp );

		paf->location	= fread_number( fp );

		paf->bitvector	= fread_number( fp );

		paf->next	= obj->affected;

		obj->affected	= paf;

		fMatch		= TRUE;

		break;

	    }

            if (!str_cmp(word,"Affc"))

            {

                AFFECT_DATA *paf;

                int sn;



                paf = new_affect();



                sn = skill_lookup(fread_word(fp));

                if (sn < 0)

                    bug("Fread_obj: unknown skill.",0);

                else

                    paf->type = sn;



		paf->where	= fread_number( fp );

                paf->level      = fread_number( fp );

                paf->duration   = fread_number( fp );

                paf->modifier   = fread_number( fp );

                paf->location   = fread_number( fp );

                paf->bitvector  = fread_number( fp );

                paf->next       = obj->affected;

                obj->affected   = paf;

                fMatch          = TRUE;

                break;

            }

	    break;



	case 'C':

	    KEY( "Cond",	obj->condition,		fread_number( fp ) );

	    KEY( "Cost",	obj->cost,		fread_number( fp ) );

	    break;



	case 'D':

	    KEY( "Description",	obj->description,	fread_string( fp ) );

	    KEY( "Desc",	obj->description,	fread_string( fp ) );

	    break;



	case 'E':



	    if ( !str_cmp( word, "Enchanted"))

	    {

		obj->enchanted = TRUE;

	 	fMatch 	= TRUE;

		break;

	    }



	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );

	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );



	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))

	    {

		EXTRA_DESCR_DATA *ed;



		ed = new_extra_descr();



		ed->keyword		= fread_string( fp );

		ed->description		= fread_string( fp );

		ed->next		= obj->extra_descr;

		obj->extra_descr	= ed;

		fMatch = TRUE;

	    }



	    if ( !str_cmp( word, "End" ) )

	    {

		if ( !fNest || ( fVnum && obj->pIndexData == NULL ) )

		{

		    bug( "Fread_obj: incomplete object.", 0 );

		    free_string( obj->name        );

		    free_string( obj->description );

		    free_string( obj->short_descr );

		    obj->next = obj_free;

		    obj_free  = obj;

		    return;

		}

		else

	        {

		    if ( !fVnum )

		    {

		        free_string( obj->name        );

		        free_string( obj->description );

			free_string( obj->short_descr );

			obj->next = obj_free;

			obj_free  = obj;



			obj = create_object( get_obj_index( OBJ_VNUM_DUMMY ), 0 );

		    }



		    if (!new_format)

		    {

		    	obj->next	= object_list;

		    	object_list	= obj;

		    	obj->pIndexData->count++;

		    }



		    if (!obj->pIndexData->new_format

		    && obj->item_type == ITEM_ARMOR

		    &&  obj->value[1] == 0)

		    {

			obj->value[1] = obj->value[0];

			obj->value[2] = obj->value[0];

		    }

		    if (make_new)

		    {

			int wear;



			wear = obj->wear_loc;

			extract_obj(obj);



			obj = create_object(obj->pIndexData,0);

			obj->wear_loc = wear;

		    }

		    if ( iNest == 0 || rgObjNest[iNest] == NULL )

			obj_to_char( obj, ch );

		    else

			obj_to_obj( obj, rgObjNest[iNest-1] );

		    return;

		}

	    }

	    break;



	case 'I':

	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );

	    KEY( "Ityp",	obj->item_type,		fread_number( fp ) );

	    break;



	case 'L':

	    KEY( "Level",	obj->level,		fread_number( fp ) );

	    KEY( "Lev",		obj->level,		fread_number( fp ) );

	    break;



	case 'N':

	    KEY( "Name",	obj->name,		fread_string( fp ) );



	    if ( !str_cmp( word, "Nest" ) )

	    {

		iNest = fread_number( fp );

		if ( iNest < 0 || iNest >= MAX_NEST )

		{

		    bug( "Fread_obj: bad nest %d.", iNest );

		}

		else

		{

		    rgObjNest[iNest] = obj;

		    fNest = TRUE;

		}

		fMatch = TRUE;

	    }

	    break;



   	case 'O':

	    if ( !str_cmp( word,"Oldstyle" ) )

	    {

		if (obj->pIndexData != NULL && obj->pIndexData->new_format)

		    make_new = TRUE;

		fMatch = TRUE;

	    }

	    break;





	case 'S':

	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );

	    KEY( "ShD",		obj->short_descr,	fread_string( fp ) );



	    if ( !str_cmp( word, "Spell" ) )

	    {

		int iValue;

		int sn;



		iValue = fread_number( fp );

		sn     = skill_lookup( fread_word( fp ) );

		if ( iValue < 0 || iValue > 3 )

		{

		    bug( "Fread_obj: bad iValue %d.", iValue );

		}

		else if ( sn < 0 )

		{

		    bug( "Fread_obj: unknown skill.", 0 );

		}

		else

		{

		    obj->value[iValue] = sn;

		}

		fMatch = TRUE;

		break;

	    }



	    break;



	case 'T':

	    KEY( "Timer",	obj->timer,		fread_number( fp ) );

	    KEY( "Time",	obj->timer,		fread_number( fp ) );

	    break;



	case 'V':

	    if ( !str_cmp( word, "Values" ) || !str_cmp(word,"Vals"))

	    {

		obj->value[0]	= fread_number( fp );

		obj->value[1]	= fread_number( fp );

		obj->value[2]	= fread_number( fp );

		obj->value[3]	= fread_number( fp );

		if (obj->item_type == ITEM_WEAPON && obj->value[0] == 0)

		   obj->value[0] = obj->pIndexData->value[0];

		fMatch		= TRUE;

		break;

	    }



	    if ( !str_cmp( word, "Val" ) )

	    {

		obj->value[0] 	= fread_number( fp );

	 	obj->value[1]	= fread_number( fp );

	 	obj->value[2] 	= fread_number( fp );

		obj->value[3]	= fread_number( fp );

		obj->value[4]	= fread_number( fp );

		fMatch = TRUE;

		break;

	    }



	    if ( !str_cmp( word, "Vnum" ) )

	    {

		int vnum;



		vnum = fread_number( fp );

		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )

		    bug( "Fread_obj: bad vnum %d.", vnum );

		else

		    fVnum = TRUE;

		fMatch = TRUE;

		break;

	    }

	    break;



	case 'W':

	    KEY( "WearFlags",	obj->wear_flags,	fread_number( fp ) );

	    KEY( "WeaF",	obj->wear_flags,	fread_number( fp ) );

	    KEY( "WearLoc",	obj->wear_loc,		fread_number( fp ) );

	    KEY( "Wear",	obj->wear_loc,		fread_number( fp ) );

	    KEY( "Weight",	obj->weight,		fread_number( fp ) );

	    KEY( "Wt",		obj->weight,		fread_number( fp ) );

	    break;



	}



	if ( !fMatch )

	{

	    bug( "Fread_obj: no match.", 0 );

	    fread_to_eol( fp );

	}

    }

}

