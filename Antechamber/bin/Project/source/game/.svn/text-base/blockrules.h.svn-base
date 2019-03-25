/****************************************
/	Al (c) 2008 - blockrules.h
/****************************************
/	rules for level editor blocks
/****************************************/

#ifndef _BLOCKRULES_H_
#define _BLOCKRULES_H_

/*************** INCLUDES ***************/

#include <windows.h>


/*************** CONSTANTS **************/

typedef enum
{
	BLOCK_EMPTY		= 0,

	BLOCK_SINGLE,

	BLOCK_PLAT_M,
	BLOCK_PLAT_L,
	BLOCK_PLAT_R,

	BLOCK_TOP_S,
	BLOCK_TOP_M,
	BLOCK_TOP_L,
	BLOCK_TOP_R,

	BLOCK_BOT_S,
	BLOCK_BOT_M,
	BLOCK_BOT_L,
	BLOCK_BOT_R,

	BLOCK_WALL_S,
	BLOCK_WALL_L,
	BLOCK_WALL_R,

	BLOCK_BLEND_TOP_WALL_S,
	BLOCK_BLEND_TOP_WALL_L,
	BLOCK_BLEND_TOP_WALL_R,

	BLOCK_BLEND_BOT_WALL_S,
	BLOCK_BLEND_BOT_WALL_L,
	BLOCK_BLEND_BOT_WALL_R,

	BLOCK_BLEND_TOP_L,
	BLOCK_BLEND_TOP_R,

	BLOCK_BLEND_BOT_L,
	BLOCK_BLEND_BOT_R,

	BLOCK_BLEND_WALL_UP_L,
	BLOCK_BLEND_WALL_UP_R,

	BLOCK_BLEND_WALL_DOWN_L,
	BLOCK_BLEND_WALL_DOWN_R,

	BLOCK_BLEND_TOP_S_L,
	BLOCK_BLEND_TOP_S_R,

	BLOCK_BLEND_BOT_S_L,
	BLOCK_BLEND_BOT_S_R,

	BLOCK_BLEND_PLAT_M_DOWN,
	BLOCK_BLEND_PLAT_M_UP,

	BLOCK_BLEND_WALL_S_L,
	BLOCK_BLEND_WALL_S_R,

	BLOCK_BLEND_FULL,
	BLOCK_BLEND_TL_BR,
	BLOCK_BLEND_TR_BL,
	BLOCK_BLEND_NO_TL,
	BLOCK_BLEND_NO_TR,
	BLOCK_BLEND_NO_BL,
	BLOCK_BLEND_NO_BR,

	BLOCK_FILLER,


	PAD_LONG		= 0xffffffff

}types_e;


/*************** STRUCTURES *************/

typedef struct
{
	bool		neighbours[8];
	types_e		blocktype;

}rules_s;

typedef struct
{
	types_e		type;
	short		uv_id;

}block_ids_s;

/*********** GLOBAL VARIABLES ***********/

extern rules_s			g_ruleset[];
extern block_ids_s		g_block_ids[];


/********* FUNCTION PROTOTYPES **********/


#endif