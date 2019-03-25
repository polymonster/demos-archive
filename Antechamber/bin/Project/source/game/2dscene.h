/****************************************
/	Al (c) 2008 - 2dscene.cpp
/****************************************
/	Draw 2D graphics!
/****************************************/

#ifndef _2DSCENE_H_
#define _2DSCENE_H_

/*************** INCLUDES ***************/

#include <windows.h>
#include <windowsx.h>

#include "dappinput.h"
#include "d3dapp.h"

#include "actor.h"
#include "level.h"


/*************** CONSTANTS **************/



/**************** CLASSES ***************/

typedef enum
{
	GAME_STATE_INIT			=	0,
	GAME_STATE_POLYHADRON,
	GAME_STATE_FRONTEND,
	GAME_STATE_QUIT_TO_MAIN,
	GAME_STATE_LOADMAIN,
	GAME_STATE_LOAD,
	GAME_STATE_LOAD_EDITOR,
	GAME_STATE_RUN,
	GAME_STATE_EDIT,
	GAME_STATE_GAMEMENU,
	GAME_STATE_EXIT,

	GAME_STATE_PAD			=	0xffffffff

}game_state_e;

typedef enum
{
	MENU_STATE_SLIDEON		=	0,
	MENU_STATE_MAIN,
	MENU_STATE_BACKTO_MAIN,
	MENU_STATE_NEW_GAME,
	MENU_STATE_LOAD_GAME,
	MENU_STATE_LOAD_EDITOR,
	MENU_STATE_OPTIONS,
	MENU_STATE_PICK_NEW,

	MENU_STATE_PAD			=	0xffffffff

}menu_state_e;

typedef struct
{
	int bullet,
		missile,
		beam,
		collectable,
		explosion,
		explosion2,
		activate,
		gate_open,
		gate_close,
		gate_motion,
		gate_stop,
		death,
		start,
		jump,
		double_jump,
		land,
		respawn,
		save,
		interact;

}s_sound_id;

typedef struct
{
	bool	inuse;
	vector2 position;
	vector2	mappos;
	int		score;
	int		lives;

	// phase 2
	int		level;
	float	energy;

}profiles_s;

class Scene2D
{

public:

	Scene2D();
	~Scene2D();

	bool			execute				( void );
	void			render				( void );

	void			print_framerate		( void );

	float			m_game_time;
	float			m_execution_time;
	game_state_e	m_state;
	menu_state_e	m_menu_state;

private:

	int				m_player_handle[2];
	int				m_guinode_handle[6];
	int				m_menub_handle;
	int				m_title_handle;
	int				m_game_mode;

	bool			m_slot_toggle[2][3];

	GUINode			*m_slots[12];
	Quad			*m_modes[2];
	GUINode			*m_back_p;

	bool			m_game_menu_active;

};


/*********** GLOBAL VARIABLES ***********/

extern		Scene2D		*g_scene_p;
extern		int			g_sfx_volume;
extern		int			g_music_volume;
extern		int			g_dialog_volume;
extern		s_sound_id	g_sound_id;
extern		Actor		*g_player[2];
extern		int			g_active_profile;
// needs to be more dynamic for game mods
extern		profiles_s		g_profile[2][3];

/********* FUNCTION PROTOTYPES **********/

extern		float		get_game_time( float cap );


#endif