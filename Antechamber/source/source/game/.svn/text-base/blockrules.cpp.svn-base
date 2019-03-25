/****************************************
/	Al (c) 2008 - blockrules.cpp
/****************************************
/	rules for level editor blocks
/****************************************/


/*************** INCLUDES ***************/

#include "blockrules.h"


/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/

block_ids_s g_block_ids[] =
{

	{ BLOCK_SINGLE, 0 },

	{ BLOCK_PLAT_M, 0 },
	{ BLOCK_PLAT_L, 0 },
	{ BLOCK_PLAT_R, 0 },

	{ BLOCK_TOP_S, 0 },
	{ BLOCK_TOP_M, 0 },
	{ BLOCK_TOP_L, 0 },
	{ BLOCK_TOP_R, 0 },

	{ BLOCK_BOT_S, 0 },
	{ BLOCK_BOT_M, 0 },
	{ BLOCK_BOT_L, 0 },
	{ BLOCK_BOT_R, 0 },

	{ BLOCK_WALL_S, 0 },
	{ BLOCK_WALL_L, 0 },
	{ BLOCK_WALL_R, 0 },

	{ BLOCK_BLEND_TOP_WALL_S, 0 },
	{ BLOCK_BLEND_TOP_WALL_L, 0 },
	{ BLOCK_BLEND_TOP_WALL_R, 0 },

	{ BLOCK_BLEND_BOT_WALL_S, 0 },
	{ BLOCK_BLEND_BOT_WALL_L, 0 },
	{ BLOCK_BLEND_BOT_WALL_R, 0 },

	{ BLOCK_BLEND_TOP_L, 0 },
	{ BLOCK_BLEND_TOP_R, 0 },

	{ BLOCK_BLEND_BOT_L, 0 },
	{ BLOCK_BLEND_BOT_R, 0 },

	{ BLOCK_BLEND_WALL_UP_L, 0 },
	{ BLOCK_BLEND_WALL_UP_R, 0 },

	{ BLOCK_BLEND_WALL_DOWN_L, 0 },
	{ BLOCK_BLEND_WALL_DOWN_R, 0 },

	{ BLOCK_BLEND_TOP_S_L, 0 },
	{ BLOCK_BLEND_TOP_S_R, 0 },

	{ BLOCK_BLEND_BOT_S_L, 0 },
	{ BLOCK_BLEND_BOT_S_R, 0 },

	{ BLOCK_BLEND_PLAT_M_DOWN, 0 },
	{ BLOCK_BLEND_PLAT_M_UP, 0 },

	{ BLOCK_BLEND_WALL_S_L, 0 },
	{ BLOCK_BLEND_WALL_S_R, 0 },

	{ BLOCK_BLEND_FULL, 0 },
	{ BLOCK_BLEND_TL_BR, 0 },
	{ BLOCK_BLEND_TR_BL, 0 },
	{ BLOCK_BLEND_NO_TL, 0 },
	{ BLOCK_BLEND_NO_TR, 0 },
	{ BLOCK_BLEND_NO_BL, 0 },
	{ BLOCK_BLEND_NO_BR, 0 },

	{ BLOCK_FILLER, 0 }

};

rules_s	g_ruleset[] =
{
//	  T  L  R  B  TL TR BL BR
	{ 0, 0, 0, 0, 0, 0, 0, 0, BLOCK_SINGLE },

	// ========= ONES =========

	{ 1, 0, 0, 0, 0, 0, 0, 0, BLOCK_BOT_S },
	{ 0, 1, 0, 0, 0, 0, 0, 0, BLOCK_PLAT_R },
	{ 0, 0, 1, 0, 0, 0, 0, 0, BLOCK_PLAT_L },
	{ 0, 0, 0, 1, 0, 0, 0, 0, BLOCK_TOP_S },
	{ 0, 0, 0, 0, 1, 0, 0, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 1, 0, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 0, 1, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 0, 0, 1, BLOCK_SINGLE },

	// ========= TWOS =========

	{ 1, 1, 0, 0, 0, 0, 0, 0, BLOCK_BLEND_TOP_S_R },		// add BLOCK_R_PLAT_FLOAT_BLEND_R
	{ 1, 0, 1, 0, 0, 0, 0, 0, BLOCK_BLEND_TOP_S_L },
	{ 1, 0, 0, 1, 0, 0, 0, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 0, 1, 0, 0, 0, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 0, 1, 0, 0, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 0, 0, 1, 0, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 0, 0, 0, 1, BLOCK_BOT_S },
	{ 0, 1, 1, 0, 0, 0, 0, 0, BLOCK_PLAT_M },
	{ 0, 1, 0, 1, 0, 0, 0, 0, BLOCK_TOP_R },		// add BLOCK_R_PLAT_TOP_BLEND_END_R
	{ 0, 1, 0, 0, 1, 0, 0, 0, BLOCK_PLAT_R },
	{ 0, 1, 0, 0, 0, 1, 0, 0, BLOCK_PLAT_R },
	{ 0, 1, 0, 0, 0, 0, 1, 0, BLOCK_PLAT_R },
	{ 0, 1, 0, 0, 0, 0, 0, 1, BLOCK_PLAT_R },
	{ 0, 0, 1, 1, 0, 0, 0, 0, BLOCK_TOP_L },		// add BLOCK_R_PLAT_TOP_BLEND_END_L
	{ 0, 0, 1, 0, 1, 0, 0, 0, BLOCK_PLAT_L },
	{ 0, 0, 1, 0, 0, 1, 0, 0, BLOCK_PLAT_L },
	{ 0, 0, 1, 0, 0, 0, 1, 0, BLOCK_PLAT_L },
	{ 0, 0, 1, 0, 0, 0, 0, 1, BLOCK_PLAT_L },
	{ 0, 0, 0, 1, 1, 0, 0, 0, BLOCK_TOP_S },
	{ 0, 0, 0, 1, 0, 1, 0, 0, BLOCK_TOP_S },
	{ 0, 0, 0, 1, 0, 0, 1, 0, BLOCK_TOP_S },
	{ 0, 0, 0, 1, 0, 0, 0, 1, BLOCK_TOP_S },
	{ 0, 0, 0, 0, 1, 1, 0, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 1, 0, 1, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 1, 0, 0, 1, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 1, 1, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 1, 0, 1, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 0, 1, 1, BLOCK_SINGLE },

	// ======== THREES ========

//	  T  L  R  B  TL TR BL BR
	{ 1, 1, 1, 0, 0, 0, 0, 0, BLOCK_BLEND_PLAT_M_UP },
	{ 0, 1, 1, 1, 0, 0, 0, 0, BLOCK_BLEND_PLAT_M_DOWN },
	{ 0, 0, 1, 1, 1, 0, 0, 0, BLOCK_BLEND_TOP_S_L },
	{ 0, 0, 0, 1, 1, 1, 0, 0, BLOCK_TOP_S },
	{ 0, 0, 0, 0, 1, 1, 1, 0, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 1, 1, 1, BLOCK_SINGLE },
	{ 1, 1, 0, 1, 0, 0, 0, 0, BLOCK_BLEND_WALL_S_R },
	{ 1, 1, 0, 0, 1, 0, 0, 0, BLOCK_BOT_R },
	{ 1, 1, 0, 0, 0, 1, 0, 0, BLOCK_BLEND_TOP_S_R },
	{ 1, 1, 0, 0, 0, 0, 1, 0, BLOCK_BLEND_TOP_S_R },
	{ 1, 1, 0, 0, 0, 0, 0, 1, BLOCK_BLEND_TOP_S_R },
	{ 1, 0, 1, 1, 0, 0, 0, 0, BLOCK_BLEND_WALL_S_L },
	{ 1, 0, 1, 0, 1, 0, 0, 0, BLOCK_BLEND_TOP_S_L },
	{ 1, 0, 1, 0, 0, 1, 0, 0, BLOCK_BOT_L },
	{ 1, 0, 1, 0, 0, 0, 1, 0, BLOCK_BLEND_BOT_S_L },
	{ 1, 0, 1, 0, 0, 0, 0, 1, BLOCK_BLEND_BOT_S_L },
	{ 1, 0, 0, 1, 1, 0, 0, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 0, 1, 0, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 0, 0, 1, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 0, 0, 0, 1, BLOCK_WALL_S },
	{ 1, 0, 0, 0, 1, 1, 0, 0, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 1, 0, 1, 0, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 1, 0, 0, 1, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 0, 1, 1, 0, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 0, 1, 0, 1, BLOCK_BOT_S },
	{ 1, 0, 0, 0, 0, 0, 1, 1, BLOCK_BOT_S },
	{ 0, 1, 0, 0, 0, 0, 1, 1, BLOCK_PLAT_R },
	{ 0, 0, 1, 0, 0, 0, 1, 1, BLOCK_PLAT_L },
	{ 0, 0, 0, 1, 0, 0, 1, 1, BLOCK_TOP_S },
	{ 0, 0, 0, 0, 1, 0, 1, 1, BLOCK_SINGLE },
	{ 0, 0, 0, 0, 0, 1, 1, 1, BLOCK_SINGLE },
	{ 0, 1, 0, 0, 0, 1, 0, 1, BLOCK_PLAT_R },
	{ 0, 0, 1, 0, 0, 1, 0, 1, BLOCK_PLAT_L },
	{ 0, 0, 0, 1, 0, 1, 0, 1, BLOCK_TOP_S },
	{ 0, 0, 0, 0, 1, 1, 0, 1, BLOCK_SINGLE },
	{ 0, 1, 0, 0, 1, 0, 0, 1, BLOCK_PLAT_R },
	{ 0, 0, 1, 0, 1, 0, 0, 1, BLOCK_PLAT_L },
	{ 0, 0, 0, 1, 1, 0, 0, 1, BLOCK_TOP_S },
	{ 0, 1, 0, 1, 0, 0, 0, 1, BLOCK_TOP_R },		// add BLOCK_R_PLAT_TOP_BLEND_END_R
	{ 0, 0, 1, 1, 0, 0, 0, 1, BLOCK_TOP_L },
	{ 0, 1, 1, 0, 1, 0, 0, 0, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 0, 1, 0, 0, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 0, 0, 1, 0, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 0, 0, 0, 1, BLOCK_PLAT_M },
	{ 0, 0, 1, 1, 0, 1, 0, 0, BLOCK_TOP_L },			// add BLOCK_R_FLOAT_TOP_END_L
	{ 0, 0, 1, 1, 0, 0, 1, 0, BLOCK_TOP_L },			// add BLOCK_R_FLOAT_TOP_END_L
	{ 0, 0, 0, 1, 1, 0, 1, 0, BLOCK_TOP_S },
	{ 0, 0, 0, 1, 0, 0, 1, 1, BLOCK_TOP_S },
	{ 0, 1, 0, 1, 1, 0, 0, 0, BLOCK_TOP_R },		// add BLOCK_R_PLAT_TOP_BLEND_END_R
	{ 0, 1, 0, 1, 0, 1, 0, 0, BLOCK_TOP_R },		// add BLOCK_R_PLAT_TOP_BLEND_END_R
	{ 0, 1, 0, 1, 0, 0, 1, 0, BLOCK_TOP_R },
	{ 0, 1, 0, 0, 1, 1, 0, 0, BLOCK_PLAT_R },
	{ 0, 0, 1, 0, 1, 1, 0, 0, BLOCK_PLAT_L },
	{ 0, 1, 0, 0, 1, 0, 1, 0, BLOCK_PLAT_R },
	{ 0, 1, 0, 0, 1, 0, 0, 1, BLOCK_PLAT_R },
	{ 0, 1, 0, 0, 0, 1, 1, 0, BLOCK_PLAT_R },
	{ 0, 1, 0, 0, 0, 1, 0, 1, BLOCK_PLAT_R },

	// ======== FOURS ========

//	  T  L  R  B  TL TR BL BR
	{ 0, 1, 0, 1, 1, 0, 1, 0, BLOCK_TOP_R },
	{ 0, 1, 0, 1, 0, 0, 1, 1, BLOCK_TOP_R },
	{ 1, 0, 1, 0, 0, 1, 0, 1, BLOCK_BOT_L },
	{ 1, 0, 1, 0, 1, 1, 0, 0, BLOCK_BOT_L },
	{ 1, 1, 0, 0, 1, 1, 0, 0, BLOCK_BOT_R },
	{ 1, 1, 0, 0, 1, 0, 1, 0, BLOCK_BOT_R },
	{ 0, 0, 1, 1, 0, 0, 1, 1, BLOCK_TOP_L },
	{ 0, 0, 1, 1, 0, 1, 0, 1, BLOCK_TOP_L },
	{ 0, 1, 1, 0, 0, 0, 1, 1, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 1, 1, 0, 0, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 1, 0, 1, 0, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 0, 1, 0, 1, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 1, 0, 0, 1, BLOCK_PLAT_M },
	{ 0, 1, 1, 0, 0, 1, 1, 0, BLOCK_PLAT_M },
	{ 1, 0, 0, 1, 0, 0, 1, 1, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 1, 1, 0, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 1, 0, 1, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 0, 1, 0, 1, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 0, 1, 1, 0, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 1, 0, 0, 1, BLOCK_WALL_S },

	// and lots more....

	// ======== FIVES ========

//	  T  L  R  B  TL TR BL BR
	{ 1, 0, 1, 1, 0, 1, 0, 1, BLOCK_WALL_L },
	{ 1, 1, 0, 1, 1, 0, 1, 0, BLOCK_WALL_R },
	{ 0, 1, 1, 1, 0, 0, 1, 1, BLOCK_TOP_M },
	{ 1, 1, 1, 0, 1, 1, 0, 0, BLOCK_BOT_M },
	{ 0, 0, 0, 1, 1, 1, 1, 1, BLOCK_TOP_S },
	{ 0, 1, 0, 1, 1, 0, 1, 1, BLOCK_TOP_R },
	{ 1, 0, 0, 0, 1, 1, 1, 1, BLOCK_BOT_S },
	{ 1, 1, 0, 0, 0, 1, 1, 1, BLOCK_BOT_R },		// add BLOCK_R_PLAT_FLOAT_BLEND_R
	{ 1, 0, 1, 0, 1, 1, 0, 1, BLOCK_BOT_L },
	{ 0, 0, 1, 1, 0, 1, 1, 1, BLOCK_TOP_L },
	{ 1, 1, 0, 0, 1, 1, 1, 0, BLOCK_BOT_R },
	{ 1, 0, 1, 1, 0, 1, 1, 0, BLOCK_WALL_L },		// add something better
	{ 1, 1, 0, 1, 0, 1, 1, 0, BLOCK_WALL_R },		// add something better
	{ 1, 0, 1, 1, 1, 1, 0, 0, BLOCK_WALL_L },		// add something better
	{ 1, 1, 0, 1, 1, 1, 0, 0, BLOCK_WALL_R },		// add something better
	{ 1, 0, 1, 0, 1, 1, 1, 0, BLOCK_BOT_L },
	{ 1, 0, 0, 1, 1, 1, 0, 1, BLOCK_WALL_S },
	{ 1, 0, 0, 1, 1, 1, 1, 0, BLOCK_WALL_S },
	{ 0, 1, 1, 1, 1, 0, 0, 1, BLOCK_TOP_M },	// add something better
	{ 0, 0, 1, 1, 1, 0, 1, 1, BLOCK_TOP_L },
	{ 1, 1, 0, 0, 1, 0, 1, 1, BLOCK_BOT_R },
	{ 0, 1, 1, 0, 1, 0, 1, 1, BLOCK_PLAT_M },
	{ 0, 1, 0, 1, 0, 1, 1, 1, BLOCK_TOP_R },

	// TODO
	{ 1, 1, 1, 0, 0, 0, 1, 1, BLOCK_FILLER },
	{ 1, 1, 1, 1, 0, 0, 0, 1, BLOCK_FILLER },
	{ 1, 1, 1, 1, 1, 0, 0, 0, BLOCK_FILLER },
	{ 0, 0, 1, 0, 1, 1, 1, 1, BLOCK_FILLER },
	{ 0, 0, 1, 1, 1, 1, 0, 1, BLOCK_FILLER },
	{ 0, 0, 1, 1, 1, 1, 1, 0, BLOCK_FILLER },
	{ 0, 1, 0, 0, 1, 1, 1, 1, BLOCK_FILLER },
	{ 0, 1, 0, 1, 1, 1, 0, 1, BLOCK_FILLER },
	{ 0, 1, 0, 1, 1, 1, 1, 0, BLOCK_FILLER },
	{ 0, 1, 1, 0, 0, 1, 1, 1, BLOCK_FILLER },
	{ 0, 1, 1, 0, 1, 1, 0, 1, BLOCK_FILLER },
	{ 0, 1, 1, 0, 1, 1, 1, 0, BLOCK_FILLER },
	{ 0, 1, 1, 1, 0, 1, 0, 1, BLOCK_FILLER },
	{ 0, 1, 1, 1, 0, 1, 1, 0, BLOCK_FILLER },
	{ 0, 1, 1, 1, 1, 0, 1, 0, BLOCK_FILLER },
	{ 0, 1, 1, 1, 1, 1, 0, 0, BLOCK_FILLER },
	{ 1, 1, 1, 1, 0, 1, 0, 0, BLOCK_FILLER },
	{ 1, 1, 1, 1, 1, 0, 0, 0, BLOCK_FILLER },
	{ 1, 0, 1, 1, 1, 0, 1, 0, BLOCK_FILLER },
	{ 1, 1, 1, 0, 1, 0, 1, 0, BLOCK_FILLER },
	{ 1, 1, 1, 1, 0, 0, 1, 0, BLOCK_FILLER },
	{ 1, 1, 1, 1, 1, 0, 1, 0, BLOCK_FILLER },
	{ 1, 1, 1, 0, 0, 1, 1, 0, BLOCK_FILLER },
	{ 1, 0, 0, 1, 0, 1, 1, 1, BLOCK_FILLER },
	{ 1, 0, 0, 1, 1, 0, 1, 1, BLOCK_FILLER },
	{ 1, 1, 0, 0, 1, 1, 0, 1, BLOCK_FILLER },
	{ 1, 1, 1, 0, 0, 1, 0, 1, BLOCK_FILLER },
	{ 1, 1, 1, 1, 0, 0, 1, 0, BLOCK_FILLER },
	{ 1, 0, 1, 0, 0, 1, 1, 1, BLOCK_FILLER },
	{ 1, 0, 1, 0, 1, 0, 1, 1, BLOCK_FILLER },
	{ 1, 0, 1, 1, 0, 0, 1, 1, BLOCK_FILLER },
	{ 1, 0, 1, 1, 0, 1, 1, 0, BLOCK_FILLER },
	{ 1, 0, 1, 1, 1, 0, 0, 1, BLOCK_FILLER },
	{ 1, 0, 1, 1, 1, 0, 1, 0, BLOCK_FILLER },

	// ======== SIXES ========

//	  T  L  R  B  TL TR BL BR
	{ 0, 0, 1, 1, 1, 1, 1, 1, BLOCK_TOP_L },
	{ 0, 1, 0, 1, 1, 1, 1, 1, BLOCK_TOP_R },
	{ 0, 1, 1, 0, 1, 1, 1, 1, BLOCK_PLAT_M },
	{ 0, 1, 1, 1, 0, 1, 1, 1, BLOCK_TOP_M },
	{ 0, 1, 1, 1, 1, 0, 1, 1, BLOCK_TOP_M },
	{ 0, 1, 1, 1, 1, 1, 0, 1, BLOCK_TOP_M },			// add BLOCK_R_PLAT_TOP_BLEND_L
	{ 0, 1, 1, 1, 1, 1, 1, 0, BLOCK_TOP_M },			// add BLOCK_R_PLAT_TOP_BLEND_R
	{ 1, 0, 0, 1, 1, 1, 1, 1, BLOCK_WALL_S },
	{ 1, 0, 1, 0, 1, 1, 1, 1, BLOCK_BOT_L },
	{ 1, 0, 1, 1, 0, 1, 1, 1, BLOCK_WALL_L },
	{ 1, 0, 1, 1, 1, 0, 1, 1, BLOCK_WALL_L },
	{ 1, 0, 1, 1, 1, 1, 0, 1, BLOCK_WALL_L },
	{ 1, 0, 1, 1, 1, 1, 1, 0, BLOCK_WALL_L },
	{ 1, 1, 0, 0, 1, 1, 1, 1, BLOCK_BOT_R },
	{ 1, 1, 0, 1, 0, 1, 1, 1, BLOCK_WALL_R },
	{ 1, 1, 0, 1, 1, 0, 1, 1, BLOCK_WALL_R },
	{ 1, 1, 0, 1, 1, 1, 0, 1, BLOCK_WALL_R },
	{ 1, 1, 0, 1, 1, 1, 1, 0, BLOCK_WALL_R },
	{ 1, 1, 1, 0, 0, 1, 1, 1, BLOCK_BOT_M },
	{ 1, 1, 1, 0, 1, 0, 1, 1, BLOCK_BOT_M },
	{ 1, 1, 1, 0, 1, 1, 0, 1, BLOCK_BOT_M },
	{ 1, 1, 1, 0, 1, 1, 1, 0, BLOCK_BOT_M },
	{ 1, 1, 1, 1, 0, 1, 0, 1, BLOCK_FILLER },			// add a "pf - top wl & bot wl blend"
	{ 1, 1, 1, 1, 1, 0, 1, 0, BLOCK_FILLER },			// add a "TR wr & BR wr - pf blend"
	{ 1, 1, 1, 1, 1, 1, 0, 0, BLOCK_FILLER },			// add a "TL bot & TR bot - ws | w1 blend"
	{ 1, 1, 1, 1, 0, 0, 1, 1, BLOCK_BLEND_TOP_WALL_S },
	{ 1, 1, 1, 1, 0, 1, 1, 0, BLOCK_FILLER },			// add a SERIOUSLY CRAZY TR-BL FULL BLEND
	{ 1, 1, 1, 1, 1, 0, 0, 1, BLOCK_FILLER },			// add a SERIOUSLY CRAZY TL-BR FULL BLEND

	// ======= SEVENS ========

//	  T  L  R  B  TL TR BL BR
	{ 0, 1, 1, 1, 1, 1, 1, 1, BLOCK_TOP_M },
	{ 1, 0, 1, 1, 1, 1, 1, 1, BLOCK_WALL_L },
	{ 1, 1, 0, 1, 1, 1, 1, 1, BLOCK_WALL_R },
	{ 1, 1, 1, 0, 1, 1, 1, 1, BLOCK_BOT_M },
	{ 1, 1, 1, 1, 0, 1, 1, 1, BLOCK_BLEND_TOP_WALL_L },
	{ 1, 1, 1, 1, 1, 0, 1, 1, BLOCK_BLEND_TOP_WALL_R },
	{ 1, 1, 1, 1, 1, 1, 0, 1, BLOCK_BLEND_BOT_WALL_L },
	{ 1, 1, 1, 1, 1, 1, 1, 0, BLOCK_BLEND_BOT_WALL_R },


	{ 1, 1, 1, 1, 1, 1, 1, 1, BLOCK_FILLER },
};

/********* FUNCTION DEFINITIONS *********/

