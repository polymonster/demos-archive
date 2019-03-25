/****************************************
/	Al (c) 2008 - 2dscene.cpp
/****************************************
/	Draw 2D graphics!
/****************************************/


/*************** INCLUDES ***************/

#include "2dscene.h"
#include "paths.h"
#include "dxio.h"

#include "dappinput.h"

#include "font.h"
#include "guinode.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "rand.h"

#include "soundManager.h"

/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/

Scene2D		*g_scene_p			= NULL;
Font		*g_mini_font_p		= NULL;

float		g_gui_posx			= 60.0f;
float		g_gui_posy			= 80.0f;

int			g_backdrop_handle	= -1;
int			g_cursorbob_handle	= -1;

int			g_active_profile	= -1;

profiles_s	g_profile[2][3];

int g_i = 0;
float f = g_fullscreen ? -2.0f : 0.0f;
float g_time_passed[3] = { f, f, f };

int			g_sfx_volume = 30;
int			g_music_volume = 55;
int			g_dialog_volume = 230;

s_sound_id	g_sound_id;

Actor		*g_player[2];

/*********** GLOBAL FUNCTIONS ***********/

float get_game_time( float cap )
{
	if( g_scene_p )
		return( g_scene_p->m_game_time < cap ? g_scene_p->m_game_time : cap );
	return NULL;
}

/********* FUNCTION DEFINITIONS *********/


Scene2D::Scene2D() :
	m_game_time( 0.0f ),
	m_execution_time( 0.0f ),
	m_game_menu_active( false ),
	m_game_mode( 0 ),
	m_state( GAME_STATE_INIT ),
	m_menu_state( MENU_STATE_SLIDEON )
{
	memset( m_slot_toggle, false, sizeof( bool ) * 6 );

	for( int i = 0; i < 6; i++ ) 
		m_guinode_handle[i] = -1;

	g_mini_font_p = new Font( "font_mini1", 8 );

	g_sound_mgr_p = new CSound();

	//items
	g_sound_id.collectable = g_sound_mgr_p->load("sound/sfx/COLLECT.wav",false);
	g_sound_id.activate = g_sound_mgr_p->load("sound/sfx/ACTIVATE.wav",false);

	//turrents / projectiles
	g_sound_id.explosion = g_sound_mgr_p->load("sound/sfx/EXPLODE2.wav",false);
	g_sound_id.explosion2 = g_sound_mgr_p->load("sound/sfx/EXPLODE.wav",false);
	g_sound_id.beam = g_sound_mgr_p->load("sound/sfx/BEAM.wav",false);
	g_sound_id.bullet = g_sound_mgr_p->load("sound/sfx/BULLET.wav",false);
	g_sound_id.missile = g_sound_mgr_p->load("sound/sfx/MISSILE.wav",false);

	//gates
	g_sound_id.gate_open = g_sound_mgr_p->load("sound/sfx/OPEN.wav",false);
	g_sound_id.gate_close = g_sound_mgr_p->load("sound/sfx/CLOSE.wav",false);
	g_sound_id.gate_motion = g_sound_mgr_p->load("sound/sfx/INMOTION.wav",false);
	g_sound_id.gate_stop = g_sound_mgr_p->load("sound/sfx/STOP.wav",false);

	//player
	g_sound_id.death = g_sound_mgr_p->load("sound/sfx/DEATH.wav",false);
	g_sound_id.jump = g_sound_mgr_p->load("sound/sfx/JUMP1.wav",false);
	g_sound_id.double_jump = g_sound_mgr_p->load("sound/sfx/JUMP2.wav",false);
	g_sound_id.land = g_sound_mgr_p->load("sound/sfx/JUMPLAND.wav",false);
	g_sound_id.respawn = g_sound_mgr_p->load("sound/sfx/RESPAWN.wav",false);
	g_sound_id.save = g_sound_mgr_p->load("sound/sfx/SAVE.wav",false);
	g_sound_id.interact = g_sound_mgr_p->load("sound/sfx/INTERACT.wav",false);

	//menus
	g_sound_id.start = g_sound_mgr_p->load("sound/sfx/START.wav",false);

	g_sound_mgr_p->play(g_sound_id.start,25);


}


Scene2D::~Scene2D()
{
	delete( g_object_list_p );
	dbgmsg( "====== Program Shutdown! ======\n" );
}



bool Scene2D::execute( void )
{
	// game stuff
static float origin = !g_editor_mode ? ( ( (float)g_window_h - 256.0f ) * 0.5f ) : 0;

	int mx, my;
	get_mouse_pos( mx, my );
static int title_alpha = 0;

	switch( m_state )
	{

		case GAME_STATE_INIT:
		{

			// polyhadron presents
			GUINode *node_p;
			node_p = new GUINode( "", "hadron1", ( g_window_w * 0.75f ), g_window_h * 0.5f, 128.0f, 128.0f, 0, 0, 0, 0x00ffffff );
			node_p->set_spin_state( SPIN_STATE_NONE );
			m_guinode_handle[0] = node_p->get_handle();

			node_p = new GUINode( "", "polymonster1", ( g_window_w * 0.25f ), g_window_h * 0.5f, 128.0f, 128.0f, 0, 0, 0, 0x00ffffff );
			node_p->set_spin_state( SPIN_STATE_NONE );
			m_guinode_handle[1] = node_p->get_handle();

			Quad *backdrop_p = new Quad( "", "", 0.0f, 0.0f, (float)g_window_w, (float)g_window_h, 0xff000000 );
			g_backdrop_handle = backdrop_p->get_handle();

			m_state = GAME_STATE_POLYHADRON;

			if( g_fullscreen )
				ShowCursor( false );
		}
		break;

		case GAME_STATE_POLYHADRON:
		{

static bool fade = 0;

			if( mouse_button_click( 0 ) )
				fade = true;

			if( !fade )
			{
				g_time_passed[0] += m_game_time;
				if( g_time_passed[0] > 5.0f )
					fade = true;

				if( g_time_passed[1] < 1.0f )
					g_time_passed[1] += m_game_time;
				if( g_time_passed[1] >= 1.0f )
					g_time_passed[1] = 1.0f;
				
				title_alpha = (float)( g_time_passed[1] ) * 255;
			}
			else
			{
				g_time_passed[1] -= m_game_time;

				if( g_time_passed[1] < 0.0f )
					g_time_passed[1] = 0.0f;
				title_alpha = (float)( g_time_passed[1] ) * 255;
				if( g_time_passed[1] == 0.0f )
				{
					for( int i = 0; i < 3; i++ )
						g_object_list_p->remove( m_guinode_handle[i] );
					g_object_list_p->remove( g_backdrop_handle );

					Quad *cursorbob_p = new Quad( PATH_IMG, "curwidget1", 0.0f, 0.0f, 16.0f, 16.0f, 0xffffffff );
					g_cursorbob_handle = cursorbob_p->get_handle();

					m_state = GAME_STATE_LOADMAIN;
					if( g_fullscreen )
						ShowCursor( true );
					break;
				}
			}

			for( int i = 0; i < 2; i++ )
			{
				GUINode *node_p = (GUINode *)g_object_list_p->get_obj( m_guinode_handle[i] );
				node_p->set_alpha( title_alpha );
			}
		}
		break;

		case GAME_STATE_LOADMAIN:
		{
			g_gui_posx	= 60.0f;
			g_gui_posy	= 80.0f;

			GUINode *node_p;
			node_p = new GUINode( "Play Game", "guibutton1", 800.0f, origin, 32.0f, 32.0f, (int *)&m_menu_state, MENU_STATE_NEW_GAME );
			m_guinode_handle[0] = node_p->get_handle();
			node_p = new GUINode( "Options", "guibutton3", -100.0f, origin, 32.0f, 32.0f, (int *)&m_menu_state, MENU_STATE_OPTIONS );
			m_guinode_handle[2] = node_p->get_handle();
			node_p = new GUINode( "Editor", "guibutton5", 800.0f, origin, 32.0f, 32.0f, (int *)&m_menu_state, MENU_STATE_LOAD_EDITOR );
			m_guinode_handle[3] = node_p->get_handle();
			node_p = new GUINode( "Quit", "guibutton4", -100.0f, origin, 32.0f, 32.0f, (int *)&m_state, GAME_STATE_EXIT );
			m_guinode_handle[4] = node_p->get_handle();
			Quad *title_p = new Quad( PATH_GUI, "title", 64.0f, 0.0f, 512.0f, 256.0f );
			title_p->set_priority( RENDER_NEAR_FRONT );
			m_title_handle = title_p->get_handle();

			Quad *backdrop_p = new Quad( "", "", 0.0f, 0.0f, (float)g_window_w, (float)g_window_h, 0xff000000 );
			g_backdrop_handle = backdrop_p->get_handle();

			g_time_passed[1] = 0.0f;
			m_state = GAME_STATE_FRONTEND;
			m_menu_state = MENU_STATE_SLIDEON;
		}
		break;

		case GAME_STATE_FRONTEND:
		{
			switch( m_menu_state )
			{
				case MENU_STATE_SLIDEON:
				{
					g_time_passed[0] += m_game_time;

					if( g_time_passed[0] > 0.3f )
					{
						g_time_passed[0] = 0.0f;
						GUINode *node_p = (GUINode *)g_object_list_p->get_obj( m_guinode_handle[g_i] );
						if( node_p )
							node_p->set_new_pos( (g_window_w*0.5f)-6.0f, origin+g_gui_posy, 4.0f, true );
						g_gui_posy += 45.0f;
						g_i++;
					}
					if( g_i == 5 )
					{
						g_i = 0;
						g_time_passed[0] = 0.0f;
						m_menu_state = MENU_STATE_MAIN;
					}
				}
				break;

				case MENU_STATE_MAIN:
				{

				}
				break;

				case MENU_STATE_BACKTO_MAIN:
				{
					int handle;
					handle = m_back_p->get_handle();
					g_object_list_p->remove( handle );

					for( int i = 0; i < 2; i++ )
					{
						handle = m_modes[i]->get_handle();
						g_object_list_p->remove( handle );
					}
					for( int i = 0; i < 12; i++ )
					{
						handle = m_slots[i]->get_handle();
						g_object_list_p->remove( handle );
					}
					m_state = GAME_STATE_LOADMAIN;
				}
				break;

				case MENU_STATE_OPTIONS:
				{
					// load options menu (will be a class obj)
					m_menu_state = MENU_STATE_MAIN;
				}
				break;

				case MENU_STATE_LOAD_GAME:
				{
					// load load game menu (class)
					m_menu_state = MENU_STATE_MAIN;
				}
				break;

				case MENU_STATE_NEW_GAME:
				{
					//remove objects from main menu
					g_object_list_p->remove( g_backdrop_handle );
					g_object_list_p->remove( m_title_handle );
					for( int i = 0; i < 5; i++ )
						g_object_list_p->remove( m_guinode_handle[i] );

					// find save files
					for( int type = 0; type < 2; type++ )
					{
						for( int slot = 0; slot < 3; slot++ )
						{
							// find the files for this game

							std::ifstream fin;

							char file[32];
							sprintf( file, "save/save%d%d.txt", type, slot );
							fin.open(file);

							// opened successfully?
							if( fin.is_open() )
							{
								char buffer[32];

								// fill in profile data until eof
								while(!fin.eof())
								{
									fin >> buffer;

									if(!strcmp(buffer,"position"))
									{
										char spacer;
										fin >> spacer;
										fin >> g_profile[type][slot].position.x;
										fin >> spacer;
										fin >> g_profile[type][slot].position.y;
									}

									if(!strcmp(buffer,"map"))
									{
										vector2 map_pos;

										char spacer;
										fin >> spacer;
										fin >> g_profile[type][slot].mappos.x;
										fin >> spacer;
										fin >> g_profile[type][slot].mappos.y;
									}

									if(!strcmp(buffer,"score"))
									{
										char spacer;
										fin >> spacer;
										fin >> g_profile[type][slot].score;
									}

									if(!strcmp(buffer,"lives"))
									{
										char spacer;
										fin >> spacer;
										fin >> g_profile[type][slot].lives;
									}
								}

								fin.close();
								g_profile[type][slot].inuse = true;

							}
							else
								g_profile[type][slot].inuse = false;

						}
					}

					// load/new game menu objects
					m_modes[0] = new Quad( PATH_GUI, "single", 160-32, 100-32, 64, 64 );
					m_modes[0]->set_priority( RENDER_NEAR_FRONT );
					m_modes[1] = new Quad( PATH_GUI, "coop", 320+160-32, 100-32, 64, 64 );
					m_modes[1]->set_priority( RENDER_NEAR_FRONT );

					// toggle buttons
					m_slots[0] = new GUINode( "Slot 1", 160, 170, 64, &m_slot_toggle[0][0] );
					m_slots[1] = new GUINode( "Slot 2", 160, 210, 64, &m_slot_toggle[0][1] );
					m_slots[2] = new GUINode( "Slot 3", 160, 250, 64, &m_slot_toggle[0][2] );

					m_slots[3] = new GUINode( "Slot 1", 320+160, 170, 64, &m_slot_toggle[1][0] );
					m_slots[4] = new GUINode( "Slot 2", 320+160, 210, 64, &m_slot_toggle[1][1] );
					m_slots[5] = new GUINode( "Slot 3", 320+160, 250, 64, &m_slot_toggle[1][2] );

					// enter game with any of these profiles
					m_slots[6] = new GUINode( g_profile[0][0].inuse ? "Resume" : "New Game", 160+80, 170, &g_active_profile, 0, 64 );
					m_slots[7] = new GUINode( g_profile[0][1].inuse ? "Resume" : "New Game", 160+80, 210, &g_active_profile, 1, 64 );
					m_slots[8] = new GUINode( g_profile[0][2].inuse ? "Resume" : "New Game", 160+80, 250, &g_active_profile, 2, 64 );

					// coop profiles
					m_slots[9] = new GUINode( g_profile[1][0].inuse ? "Resume" : "New Game", 320+160+80, 170, &g_active_profile, 3, 64 );
					m_slots[10] = new GUINode( g_profile[1][1].inuse ? "Resume" : "New Game", 320+160+80, 210, &g_active_profile, 4, 64 );
					m_slots[11] = new GUINode( g_profile[1][2].inuse ? "Resume" : "New Game", 320+160+80, 250, &g_active_profile, 5, 64 );

					// hide slots
					for( int i = 6; i < 12; i++ )
						m_slots[i]->activate( false );

					m_back_p = new GUINode( "Back", 54, 470, (int *)&m_menu_state, MENU_STATE_BACKTO_MAIN );

					m_menu_state = MENU_STATE_PICK_NEW;
				}
				break;

				case MENU_STATE_PICK_NEW:
				{
					for( int i = 6; i < 12; i++ )
					{
						// activate toggled slots
						m_slots[i]->activate( m_slot_toggle[(i-6)/3][i-(i<9?6:9)] );
					}
					if( g_active_profile != -1 )
					{
						m_game_mode = g_active_profile <= 2 ? 0 : 1;
						int handle;
						handle = m_back_p->get_handle();
						g_object_list_p->remove( handle );

						for( int i = 0; i < 2; i++ )
						{
							handle = m_modes[i]->get_handle();
							g_object_list_p->remove( handle );
						}
						for( int i = 0; i < 12; i++ )
						{
							handle = m_slots[i]->get_handle();
							g_object_list_p->remove( handle );
						}

						memset( m_slot_toggle, false, sizeof( bool ) * 6 );

						m_state = GAME_STATE_LOAD;
						m_menu_state = MENU_STATE_MAIN;
					}
				}
				break;

				case MENU_STATE_LOAD_EDITOR:
				{
					g_object_list_p->remove( m_title_handle );
					m_state = GAME_STATE_LOAD_EDITOR;
				}
				break;
			}

		}
		break;

		case GAME_STATE_LOAD_EDITOR:
		{
			g_editor_mode = true;

			g_object_list_p->remove( g_cursorbob_handle );

			for( int i = 0; i < 5; i++ )
				g_object_list_p->remove( m_guinode_handle[i] );

			GUINode *menu_p = new GUINode( "Quit", "guibuttonmnu1", 16.0f, 500.0f, 32.0f, 32.0f, (int *)&m_state, GAME_STATE_QUIT_TO_MAIN, 0xffffffff );
			menu_p->set_spin_state( SPIN_STATE_INTERACTIVE );
			menu_p->set_new_pos( 16.0f, 462.0f, 2.0f );
			m_menub_handle = menu_p->get_handle();

			g_object_list_p->remove( g_backdrop_handle );

			vector2 room;
			room.x = 127;
			room.y = 127;
			g_level_p = new Level( "level1", "map1", room, 0, 16, 256, true );

			Quad *quad_p = new Quad( PATH_IMG, "", 0.0f, 0.0f, g_window_w, g_window_h, 0xff000000 );
			g_backdrop_handle = quad_p->get_handle();

			m_state = GAME_STATE_EDIT;
		}
		break;

		case GAME_STATE_LOAD:
		{
			g_clear_col = 0xff000000;

			g_object_list_p->remove( g_backdrop_handle );

			GUINode *menu_p = new GUINode( "Quit", "guibuttonmnu1", 16.0f, 500.0f, 32.0f, 32.0f, (int *)&m_state, GAME_STATE_QUIT_TO_MAIN, 0xffffffff );
			menu_p->set_spin_state( SPIN_STATE_INTERACTIVE );
			menu_p->set_new_pos( 16.0f, 462.0f, 2.0f );
			m_menub_handle = menu_p->get_handle();

			int p = g_active_profile;
			int type = p<3 ? 0 : 1;
			int slot = p<3 ? p : p-3;
			vector2 room;
			if( g_profile[type][slot].inuse )
				room = g_profile[type][slot].mappos;
			else
			{
				room.x = 127;
				room.y = 127;
			}

			g_player[0] = new Actor( "ab14", 250.0f, 40.0f + origin, true , 1);
			m_player_handle[0] = g_player[0]->get_handle();

			if( g_active_profile > 2 )
			{
				g_player[1] = new Actor( "da21", 250.0f, 40.0f + origin, true, 2);
				m_player_handle[1] = g_player[1]->get_handle();
			}
			else
				g_player[1] = NULL;

			g_level_p = new Level( "level1", "map1", room, origin, 16, 256 );

			m_state = GAME_STATE_RUN;
		}
		break;

		case GAME_STATE_QUIT_TO_MAIN:
		{
			// back to main!

			// clean map objects
			g_level_p->cleanup_map();
			g_level_p->cleanup_level();

			// remove players
			g_object_list_p->remove( m_player_handle[0] );
			if( g_player[1] )
				g_object_list_p->remove( m_player_handle[1] );

			int h = g_level_p->get_handle();
			g_object_list_p->remove( h );

			g_object_list_p->remove( m_menub_handle );
			g_object_list_p->remove( g_backdrop_handle );

			g_active_profile = -1;

			m_state = GAME_STATE_LOADMAIN;
			m_menu_state = MENU_STATE_SLIDEON;

		}
		break;

		case GAME_STATE_RUN:
		{

#ifdef _DEBUG
			if( key_hit( DIK_F ) )
				g_mini_font_p->gen_font_info( "images\\fonts\\font_mini1.tga" );
			if( key_hit( DIK_K ) )
				g_object_list_p->remove( m_player_handle[0] );
			if( key_hit( DIK_L ) )
				Actor *player_p = new Actor( "ro", 200.0f, 40.0f + origin, 0xffffffff, true );
#endif
		}
		break;

		case GAME_STATE_EDIT:
		{

		}
		break;

		case GAME_STATE_EXIT:
		{
			return( false );
		}
		break;

	}

	if( g_cursorbob_handle != -1 )
	{
		Quad *cursorbob_p = (Quad *)g_object_list_p->get_obj( g_cursorbob_handle );

		if( cursorbob_p )
		{
			g_time_passed[2] -= m_game_time * 4.5f;
			if( g_time_passed[2] < -1.0f )
				g_time_passed[2] = 0.0f;
			cursorbob_p->rotate( g_time_passed[2] * 360.0f );
			cursorbob_p->translate( mx, my );
		}
	}

	// execute game stuff
	g_object_list_p->execute_list();

	return( true );
}


void Scene2D::render( void )
{

	// game render stuff
	g_object_list_p->render_list();

	if( m_state == GAME_STATE_POLYHADRON )
	{
		unsigned char alpha = g_time_passed[1] * 255;
		g_mini_font_p->print_2d_text( "Polymonster", ( g_window_w * 0.2f ), g_window_h * 0.3f, 1.0f, 0x00ffffff | alpha << 24 );
		g_mini_font_p->print_2d_text( "Hadron", ( g_window_w * 0.72f ), g_window_h * 0.3f, 1.0f, 0x00ffffff | alpha << 24 );
		g_mini_font_p->print_2d_text( "&", ( g_window_w * 0.5f ), g_window_h * 0.5f, 1.0f, 0x00ffffff | alpha << 24 );
		g_mini_font_p->print_2d_text( "Present", ( g_window_w * 0.48f ), g_window_h * 0.8f, 1.0f, 0x00ffffff | alpha << 24 );
	}

	// mostly debug stuff below...

	switch( m_state )
	{
		case GAME_STATE_RUN:
			break;

	}

	switch( m_menu_state )
	{
		case MENU_STATE_PICK_NEW:
		{
			g_mini_font_p->print_2d_text( "Single Player", 160-30, 55 );
			g_mini_font_p->print_2d_text( "Two Player Coop", 320+160-34, 55 );
		}
		break;
	}

	print_framerate();

#ifdef _DEBUG
	char msg[64];
	sprintf( msg, "[grey]Num Objs:[] %d", g_object_list_p->get_used_links() );
	g_mini_font_p->print_2d_text( msg, 8, 48, 1.0f, 0xffe0e000 );
#endif

	//char tmsg[512];
	//sprintf( tmsg, "TEST STRING! [blue]This is blue text[] and [red]this text is hardware[] [green]rendered and []the next step[white] is to make it wrap using the same[] [yellow]strip[]!" );
	//g_mini_font_p->print_2d_text( tmsg, 8, 100, 1.0f, 0xffa0a0a0 );

}



void Scene2D::print_framerate( void )
{
static float time_counter = 0.0f;
static float fps_update = 0.0f;
static int counter = 0;
static float fps = 0.0f;

	unsigned char r, g, b;
	float fr, fb;

	fps_update += m_game_time;
	time_counter += m_game_time;

	// num frames / time passed
	if( time_counter && fps_update > 0.2f )
	{
		fps = ( (float)counter / time_counter );
		fps_update = 0.0f;
	}

	fr = (float)( ( 60.0f - fps ) / 60.0f ) * 255;150;
	fb = (float)( fps / 60.0f ) * 255;

	r = fr > 255.0f ? 255 : fr < 0.0f ? 0 : (unsigned char)fr;
	g = 200-(r*0.3f);
	b = fb > 255.0f ? 255 : fb < 0.0f ? 0 : (unsigned char)fb;

	unsigned int col = ( r << 16 ) | ( g << 8 ) | b | ( 255 << 24 );


	char fps_string[32];
	if( time_counter >= 1.0f && counter <= 1 )
	{
		fps = time_counter;
		sprintf( fps_string, "SPF: %.1f", fps );
		g_mini_font_p->print_2d_text( fps_string, 8, 8, 1.0f, 0xfff0003f );

		time_counter = 0.0f;
		counter = 0;
	}
	else
	{
		if( time_counter >= 1.0f )
		{
			time_counter = 0.0f;
			counter = 0;
		}
		sprintf( fps_string, "%sFPS:%s %.1f", "[grey]", "[]", fps );
		g_mini_font_p->print_2d_text( fps_string, 8, 8, 1.0f, col );
	}

	counter++;

#ifdef _DEBUG

static float time_counter2 = 0.0f;
static float fps_update2 = 0.0f;
static int counter2 = 0;
static float fps2 = 0.0f;

	fps_update2 += m_execution_time;
	time_counter2 += m_execution_time;

	// num frames / time passed
	if( time_counter2 && fps_update2 > 0.007f )
	{
		fps2 = ( (float)counter2 / time_counter2 );
		fps_update2 = 0.0f;
	}
	
	char code_time[32];
	sprintf( code_time, "%cVFPS:%c %.1f", S_COL_GREY, 1, fps2 );
	g_mini_font_p->print_2d_text( code_time, 8, 8+10, 1.0f, 0xffe0e000 );

	if( time_counter2 >= 1.0f )
	{
		time_counter2 = 0.0f;
		counter2 = 0;
	}

	counter2++;
#endif
}