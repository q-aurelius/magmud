/**************************************************************************r
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

#ifndef GRAYSON_HANDLER_H
#define GRAYSON_HANDLER_H

#include "merc.h"

DECLARE_DO_FUN(do_return	);

void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
bool	is_ploaded 		args( ( CHAR_DATA *ch ) );
bool	is_linkdead 	args( ( CHAR_DATA *ch ) );

bool is_friend(CHAR_DATA *ch,CHAR_DATA *victim);
int count_users(OBJ_DATA *obj);

int material_lookup args( (const char *name) );
int race_lookup args( (const char *name) );
int liq_lookup args( (const char *name) );
int weapon_lookup args( (const char *name) );
int weapon_type args( (const char *name) );
int item_lookup args( (const char *name) );
char *item_name args( (int item_type) );
char *weapon_name args( ( int weapon_type) );
int attack_lookup  args( (const char *name) );
long wiznet_lookup args( (const char *name) );
int class_lookup args( (const char *name) );
int check_immune args( (CHAR_DATA *ch, int dam_type) );
bool is_clan args( (CHAR_DATA *ch) );
bool is_same_clan args( (CHAR_DATA *ch, CHAR_DATA *victim) );
bool is_old_mob args( (CHAR_DATA *ch) );
int get_skill args( (CHAR_DATA *ch, int sn) );
int get_weapon_sn args( (CHAR_DATA *ch) );
int get_weapon_skill args( (CHAR_DATA *ch, int sn) );
void reset_char args( (CHAR_DATA *ch) );
int get_trust args( ( CHAR_DATA *ch ) );
int get_age args( ( CHAR_DATA *ch ) );
int get_curr_stat args( ( CHAR_DATA *ch, int stat ) );
int get_max_train args( ( CHAR_DATA *ch, int stat ) );
int can_carry_n args( ( CHAR_DATA *ch ) );
int can_carry_w args( ( CHAR_DATA *ch ) );
bool is_name args( ( char *str, char *namelist ) );
bool is_exact_name args( (char *str, char *namelist ) );
void affect_enchant args( (OBJ_DATA *obj) );
void affect_modify args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
AFFECT_DATA  *affect_find args( (AFFECT_DATA *paf, int sn) );
void affect_check args( (CHAR_DATA *ch,int where,int vector) );
void affect_to_char args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void affect_to_obj args( (OBJ_DATA *obj, AFFECT_DATA *paf) );
void affect_remove args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void affect_remove_obj args( ( OBJ_DATA *obj, AFFECT_DATA *paf) );
void affect_strip args( ( CHAR_DATA *ch, int sn ) );
bool is_affected args( ( CHAR_DATA *ch, int sn ) );
int lvl_affected args( ( CHAR_DATA *ch, int sn ) );
void affect_join args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void char_from_room args( ( CHAR_DATA *ch ) );
void char_to_room args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void obj_to_char args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void obj_from_char args( ( OBJ_DATA *obj ) );
int apply_ac args( ( OBJ_DATA *obj, int iWear, int type ) );
OBJ_DATA *get_eq_char args( ( CHAR_DATA *ch, int iWear ) );
void equip_char args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void unequip_char args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int count_obj_list args( ( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list ) );
void obj_from_room args( ( OBJ_DATA *obj ) );
void obj_to_room args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void obj_to_obj args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void obj_from_obj args( ( OBJ_DATA *obj ) );
void extract_obj args( ( OBJ_DATA *obj ) );
void extract_char args( ( CHAR_DATA *ch, bool fPull ) );
CHAR_DATA *get_char_room args( ( CHAR_DATA *ch, char *argument ) );
CHAR_DATA *get_char_world args( ( CHAR_DATA *ch, char *argument ) );
OBJ_DATA *get_obj_type args( ( OBJ_INDEX_DATA *pObjIndex ) );
OBJ_DATA *get_obj_list args( ( CHAR_DATA *ch, char *argument, OBJ_DATA *list ) );
OBJ_DATA *get_obj_carry args( ( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer ) );
OBJ_DATA *get_obj_wear args( ( CHAR_DATA *ch, char *argument ) );
OBJ_DATA *get_obj_here args( ( CHAR_DATA *ch, char *argument ) );
OBJ_DATA *get_obj_world args( ( CHAR_DATA *ch, char *argument ) );
void deduct_cost args( (CHAR_DATA *ch, int cost) );
void deduct_long_cost args( (CHAR_DATA *ch, long cost) );
OBJ_DATA *create_money args( ( int gold, int silver ) );
int get_obj_number args( ( OBJ_DATA *obj ) );
int get_obj_weight args( ( OBJ_DATA *obj ) );
int get_true_weight args( (OBJ_DATA *obj) );
bool room_is_dark args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool is_room_owner args( (CHAR_DATA *ch, ROOM_INDEX_DATA *room) );
bool room_is_private args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool can_see_room args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
bool is_coder args( ( CHAR_DATA *ch) );
bool can_see args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool can_see_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool can_drop_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *affect_loc_name args( ( int location ) );
char *affect_bit_name args( ( int vector ) );
char *affect2_bit_name args( ( int vector ) );
char *extra_bit_name args( ( int extra_flags ) );
char *act_bit_name args( ( int act_flags ) );
char *comm_bit_name args( (int comm_flags) );
char *info_bit_name args( (int info_flags) );
char *olcs_bit_name args( (int olcs_flags) );
char *imm_bit_name args( (int imm_flags) );
char *wear_bit_name args( (int wear_flags) );
char *form_bit_name args( (int form_flags) );
char *part_bit_name args( (int part_flags) );
char *cyber_bit_name args( (int cyber_flags) );
char *weapon_bit_name args( (int weapon_flags) );
char *cont_bit_name args( ( int cont_flags) );
char *off_bit_name args( (int off_flags) );
bool can_build args( ( CHAR_DATA *ch, AREA_DATA *area ) );
bool can_redit args( ( CHAR_DATA *ch, AREA_DATA *area ) );
bool can_medit args( ( CHAR_DATA *ch, AREA_DATA *area ) );
bool can_oedit args( ( CHAR_DATA *ch, AREA_DATA *area ) );
bool can_reset args( ( CHAR_DATA *ch, AREA_DATA *area ) );
bool can_aedit args( ( CHAR_DATA *ch, AREA_DATA *area ) );
char *PERS args( ( CHAR_DATA *ch, CHAR_DATA *looker ) );
bool is_aff_age args( ( CHAR_DATA *ch ) );
bool is_aff_cur_age args( ( CHAR_DATA *ch ) );
char *icf_her_bit_name args( (int icf_her_flags) );
char *icf_ang_bit_name args( (int icf_ang_flags) );
char *icf_stat_bit_name( args( int icf_stat_flags) );
char *icf_sup_bit_name args( (int icf_sup_flags)  );
int get_econ_value args( ( char *argument ) );
int get_combat_value args( ( char *argument ) );
char *room_bit_name args( (int room_flags) );
bool is_ploaded  args( ( CHAR_DATA *ch ) );
bool is_linkdead  args( ( CHAR_DATA *ch ) );
bool    can_use_skill args( ( CHAR_DATA *ch, int sn ) );
bool    is_race_skill args( ( CHAR_DATA *ch, int sn ) );
bool censor_text args( ( CHAR_DATA *ch, char *argument ) );
bool is_in_prison args( ( CHAR_DATA *ch ) );
bool is_punished args( ( CHAR_DATA *ch ) );
char *punished_bit_name args( ( CHAR_DATA *ch ) );

#endif