/****************************************
/	Al (c) 2008 - level.cpp
/****************************************
/	Draw 2D level
/****************************************/


/*************** INCLUDES ***************/

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "d3dapp.h"
#include "d3dtex.h"
#include "dappinput.h"
#include "paths.h"

#include "level.h"
#include "soundManager.h"


/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/

LPDIRECT3DVERTEXBUFFER9		g_level_buffer	= NULL;

Level	*g_level_p;

char g_type_commands[][32] =
{
	"energy_cell",
	"intercom",
	"gate",
	"control_panel",
	"turret",
	"smartball",
	"spawn",

};

/********* FUNCTION DEFINITIONS *********/

Level::Level() {}

Level::Level( char *tileset_p, char *map_filename_p, vector2 room, int origin_y, int size, int tiles, bool editor ) :
	m_current_map_x( (int)room.x ),
	m_current_map_y( (int)room.y ),
	m_tile_size( size ),
	m_edit_mode( editor ),
	m_editor_mode( EDIT_MODE_LEVEL_BUILDER ),
	m_num_nodes( 0 ),
	m_save( false ),
	m_edit_cursor( 1 ),
	m_edit_cursor_p( NULL ),
	m_hard_tileset_handle( -1 ),
	m_soft_tileset_handle( -1 ),
	m_num_tex_tiles( tiles ),
	m_origin_y( origin_y ),
	m_mapdata_p( NULL ),
	m_gate_type( 0 ),
	m_col( 0xffffffff )
{

	set_priority( RENDER_MIDDLE );

	m_number_of_collectables = 0;
	m_number_of_turrets = 0;
	m_number_of_control_panels = 0;
	m_number_of_gates = 0;
	m_number_of_traps = 0;
	m_number_of_smartballs = 0;
	m_number_of_intercoms = 0;

	Quad *backdrop_p = new Quad( PATH_IMG, "backdrop2", 0.0f, origin_y, (float)g_window_w, 256.0f, 0xffffffff );
	backdrop_p->set_texcoords( 0.0f, 10.0f, 0.0f, 4.0f );
	m_backdrop_handle = backdrop_p->get_handle();

	char fullpath[64];
	sprintf( fullpath, "%s%s.tga", PATH_TILESETS, tileset_p );
	m_tileset = load_d3d_texture( fullpath );

	sprintf( fullpath, "%s%s.tga", PATH_GUI, "editcurs" );
	m_edit_cursors = load_d3d_texture( fullpath );

	sprintf( m_map_filename, map_filename_p );

	memset( m_edit_node_p, 0, 4 * MAX_NODES );

	if( !m_edit_mode )
		init_objects();

	// load up the first level
	set_new_map( MAP_SCROLL_NONE );

	if( m_edit_mode )
	{

		sprintf( m_turret_options[TURRET_TYPE_CANNON], "Cannon" );
		sprintf( m_turret_options[TURRET_TYPE_ROCKET], "Rocket Launcher" );
		sprintf( m_turret_options[TURRET_TYPE_LAZER], "Lazer" );
		char *turrets_p = (char *)m_turret_options;

		sprintf( m_gate_options[GATE_BRIDGE], "Bridge" );
		sprintf( m_gate_options[GATE_DOOR], "Door" );
		char *cps_p = (char *)m_gate_options;

		int doit = 0;
		m_edit_buttons_p[EDIT_MODE_PLACE_ENERGY_CELL] = new GUINode( "Energy Cell", 55, 280, (int*)&m_editor_mode, 0 );
		m_edit_buttons_p[EDIT_MODE_PLACE_INTERCOM] = new GUINode( "Intercom", 55, 300, (int*)&m_editor_mode, 1 );
		m_edit_buttons_p[EDIT_MODE_PLACE_GATE] = new GUINode( "Gate", 55, 320, (int*)&m_editor_mode, 2, 2, cps_p );
		m_edit_buttons_p[EDIT_MODE_PLACE_CP] = new GUINode( "Control Panel", 55, 340, (int*)&m_editor_mode, 3 );
		m_edit_buttons_p[EDIT_MODE_PLACE_TURRET] = new GUINode( "Turret", 55, 360, (int*)&m_editor_mode, 4, 3, turrets_p );
		m_edit_buttons_p[EDIT_MODE_PLACE_ELECTROBALL] = new GUINode( "Smartball", 55, 380, (int*)&m_editor_mode, 5 );
		m_edit_buttons_p[EDIT_MODE_PLACE_SPAWN] = new GUINode( "Spawn", 55, 400, (int*)&m_editor_mode, 6 );

		// create some tileset panels for selecting texture frames
		float wh = size * size;
		Quad *fghard_tileset_edit_p = new Quad( PATH_TILESETS, tileset_p, g_window_w - wh - size, origin_y+(MAP_HEIGHT*size)+(size*2), wh, wh/2.0f );
		Quad *fgsoft_tileset_edit_p = new Quad( PATH_TILESETS, tileset_p, g_window_w - (wh*2.0f), origin_y+(MAP_HEIGHT*size)+size, wh, wh/2.0f );

		fghard_tileset_edit_p->set_texcoords( 0.0f, 1.0f, 0.0f, 0.5f );
		fgsoft_tileset_edit_p->set_texcoords( 0.0f, 1.0f, 0.5f, 1.0f );

		m_hard_tileset_handle = fghard_tileset_edit_p->get_handle();
		m_soft_tileset_handle = fgsoft_tileset_edit_p->get_handle();

		// create tile mapping cursor at mouse position
		int mx, my;
		get_mouse_pos( mx, my );
		m_edit_cursor_p = new Quad( PATH_TILESETS, "", mx, my, 16, 16 );
		m_edit_cursor_p->set_priority( RENDER_NEAR_FRONT );
		m_edit_cursor_p->set_texture( m_tileset );

		// read correct UVs for m_edit_cursor index
		int tiles_per_row = (int)sqrtf( (float)m_num_tex_tiles );
		float uvsize = 1.0f / (float)tiles_per_row;
		float pos = ( (float)(m_edit_cursor-1) / (float)tiles_per_row );
		float v = (int)pos * uvsize;
		float u = pos - (int)pos;
		m_edit_cursor_p->set_texcoords( u, u + uvsize, v, v + uvsize );
	}

	g_d3d_device->SetVertexShader( NULL );

	int verts = ( ( MAP_WIDTH * 4 ) * MAP_HEIGHT ) + ( 4 * MAP_HEIGHT );
    // create a vertex buffer interface
    g_d3d_device->CreateVertexBuffer( verts * sizeof( level_vert_s ), 0, CUSTOM_STRIP_FVF, D3DPOOL_MANAGED, &g_level_buffer, NULL );

	set_slope_formulas();

	sprintf( fullpath, "%s%s.tga", PATH_SPRITES, "bullet" );
	m_projectile_tex[0] = load_d3d_texture( fullpath );
	sprintf( fullpath, "%s%s.tga", PATH_SPRITES, "missile" );
	m_projectile_tex[1] = load_d3d_texture( fullpath );
	sprintf( fullpath, "%s%s.tga", PATH_SPRITES, "lazer" );
	m_projectile_tex[2] = load_d3d_texture( fullpath );

	acid_anim_timer = 0;


}

void Level::init_objects()
{
	vector2 pos;
	pos.x = 0;
	pos.y = 0;

	vector2 sizew;
	sizew.x = 0;
	sizew.y = 0;

	for(int i = 0; i < MAX_PROJECTILES; i++)
	{
		m_projectile[i] = new Projectile(pos,sizew,1,"missile");
	}

	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		m_particle[i] = new Particle(pos,"smoke");
	}

	m_spawn_pos.x = 60;
	m_spawn_pos.y = 140;

	FSOUND_STREAM *m = NULL;
	g_sound_mgr_p->playMusic("sound/music/musics",g_music_volume,&m);

}


Level::~Level()
{
	if( m_mapdata_p )
		delete [] m_mapdata_p;

	for( int i = 0; i < 3; i++ )
		if( m_projectile_tex[i].texture )
			m_projectile_tex[i].texture->Release();

	m_tileset.texture->Release();

	if( g_level_buffer )
		g_level_buffer->Release();

}

void Level::spawn_particle(vector2 position)
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(!m_particle[i]->active())
		{
			m_particle[i]->spawn(position);
			i = MAX_PARTICLES;
		}
	}
}

void Level::spawn_particle(vector2 position, int type)
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(!m_particle[i]->active())
		{
			m_particle[i]->spawn(position,type);
			i = MAX_PARTICLES;
		}
	}
}


void Level::execute( void )
{
	//HERE WE NEED TO CHECK IF WE NEED TO SPAWN A NEW PROJECTILES

	if( !m_edit_mode )
	{
		vector2 pos;
		vector2 dir;

		for(int i = 0; i < m_number_of_turrets; i++)
		{
			int res = m_turrets[i]->shoot(&pos, &dir);

			if(res != -1)
			{
				for(int j = 0; j < MAX_PROJECTILES; j++)
				{
					if(!m_projectile[j]->active()) 
					{
						m_projectile[j]->spawn(pos,dir,res);
						j = MAX_PROJECTILES;
					}
				}
			}
		}

		//HERE WE CHECK TO SPAWN PARTICLES FOR THE MISSILE "TAILS"
		for(int i = 0; i < MAX_PROJECTILES; i++)
		{
			if(m_projectile[i]->active()) 
			{
				if(m_projectile[i]->emmit_particle())
				{
					for(int j = 0; j < MAX_PARTICLES; j++)
					{
						if(!m_particle[j]->active())
						{
							m_particle[j]->spawn(m_projectile[i]->particle_pos());
							j = MAX_PARTICLES;
						}
					}
				}
			}
		}

		acid_anim_timer -= g_scene_p->m_game_time;

		if(acid_anim_timer <= 0)
		{
			animate_acid();
			acid_anim_timer = 0.1f;
		}

		g_sound_mgr_p->checkMusicForEnd();
	}
	else
	{

		int mx, my;
		get_mouse_pos( mx, my );

		if( m_editor_mode != EDIT_MODE_LEVEL_BUILDER && m_edit_cursor_p->get_texture().texture != m_edit_cursors.texture )
			m_edit_cursor_p->set_texture( m_edit_cursors );

		if( mouse_button_click( 0 ) )
		{
			Quad *hard_set_p = (Quad *)g_object_list_p->get_obj( m_hard_tileset_handle );
			Quad *soft_set_p = (Quad *)g_object_list_p->get_obj( m_soft_tileset_handle );

			vector2 h_pos = hard_set_p->get_pos();
			vector2 h_size = hard_set_p->get_size();
			if( mx > h_pos.x && my > h_pos.y && mx < h_pos.x + h_size.x && my < h_pos.y + h_size.y )
			{
				int idx = ( mx - h_pos.x ) / m_tile_size;
				int idy = ( my - h_pos.y ) / m_tile_size;
				m_edit_cursor = idx + ( idy * m_tile_size ) + 1;
				m_editor_mode = EDIT_MODE_LEVEL_BUILDER;
				m_edit_cursor_p->set_texture( m_tileset );
			}
			else
			{
				vector2 h_pos = soft_set_p->get_pos();
				vector2 h_size = soft_set_p->get_size();
				if( mx > h_pos.x && my > h_pos.y && mx < h_pos.x + h_size.x && my < h_pos.y + h_size.y )
				{
					int idx = ( mx - h_pos.x ) / m_tile_size;
					int idy = ( my - h_pos.y ) / m_tile_size;
					m_edit_cursor = idx + ( idy * m_tile_size ) + 129;
					m_editor_mode = EDIT_MODE_LEVEL_BUILDER;
					m_edit_cursor_p->set_texture( m_tileset );
				}
			}

			// read correct UVs for m_edit_cursor index
			int tiles_per_row = (int)sqrtf( (float)m_num_tex_tiles );
			float uvsize = 1.0f / (float)tiles_per_row;
			float pos = ( (float)(m_edit_cursor-1) / (float)tiles_per_row );
			float v = (int)pos * uvsize;
			float u = pos - (int)pos;
			m_edit_cursor_p->set_texcoords( u, u + uvsize, v, v + uvsize );

		}

		// check top and bottom planes to show tile
		if( my > MAP_HEIGHT * m_tile_size - 5 )
			m_edit_cursor_p->m_hide = true;
		else
			m_edit_cursor_p->m_hide = false;

		// place stuff!
		if( m_edit_cursor_p && !m_edit_cursor_p->m_hide )
		{
			int x,y, index;
			bool bail = false;
			if( m_editor_mode != EDIT_MODE_LEVEL_BUILDER )
			{
				// set to current tile under mouse
				vector2 loc;
				loc.x = (int)mx / m_tile_size;
				loc.y = (int)my / m_tile_size;
				x = loc.x;
				y = loc.y;
				index = (y*MAP_WIDTH)+x;

				for( int i = 0; i < MAX_NODES; i++ )
				{
					if( m_edit_node_p[i] && m_edit_node_p[i]->m_data.pos == loc )
					{
						bail = true;
						if( mouse_button_down( 1 ) )
						{
							switch( m_edit_node_p[i]->m_type )
							{
							case NODE_CP:
								m_number_of_control_panels--;
								break;
							case NODE_INTERCOM:
								m_number_of_intercoms--;
							}

							int handle = m_edit_node_p[i]->get_handle();
							g_object_list_p->remove( handle );
							m_edit_node_p[i] = NULL;
							break;
						}
					}
				}

			}

			switch( m_editor_mode )
			{
				case EDIT_MODE_PLACE_SPAWN:
				{

					if( !bail && !m_tilemap[index] )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							for( int i = 0; i < MAX_NODES; i++ )
							{
								if( m_edit_node_p[i] && m_edit_node_p[i]->m_type == NODE_SPAWN )
								{
									int handle = m_edit_node_p[i]->get_handle();
									g_object_list_p->remove( handle );
									m_edit_node_p[i] = NULL;
									break;
								}
							}
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							m_edit_node_p[free] = new EditNode( NODE_SPAWN, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					m_edit_cursor_p->set_pos( (x * m_tile_size) + ( m_edit_cursor_p->get_size().x * 0.5 ), (y * m_tile_size) + ( m_edit_cursor_p->get_size().y * 0.5 ) );
					m_edit_cursor_p->set_texcoords( 0.75f, 1.0f, 0.25f, 0.5f );
					m_edit_cursor_p->set_size( 8.0f, 8.0f );

				}
				break;

				case EDIT_MODE_PLACE_TURRET:
				{
					float rot = m_edit_cursor_p->get_rotation();
					if(  mouse_button_click( 2 ) )
						m_edit_cursor_p->set_rotation( rot >= 250.0f ? 0.0f : rot + 90.0f );

					m_edit_cursor_p->rotate( m_edit_cursor_p->get_rotation() );

					if( !bail && !m_tilemap[index] )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							data.turret_direction = (int)(m_edit_cursor_p->get_rotation() / 90);
							data.turret_type = m_edit_buttons_p[EDIT_MODE_PLACE_TURRET]->m_selection;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							m_edit_node_p[free] = new EditNode( NODE_TURRET, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					m_edit_cursor_p->set_pos( (x * m_tile_size) + ( m_edit_cursor_p->get_size().x * 0.5 ), (y * m_tile_size) + ( m_edit_cursor_p->get_size().y * 0.5 ) );
					m_edit_cursor_p->set_texcoords( 0.25f, 0.5f, 0.0f, 0.25f );
					m_edit_cursor_p->set_size( 16.0f, 16.0f );

				}
				break;

				case EDIT_MODE_PLACE_ENERGY_CELL:
				{
					if( !bail && !m_tilemap[index] )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							m_edit_node_p[free] = new EditNode( NODE_ENERGY_CELL, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					m_edit_cursor_p->set_pos( x * m_tile_size + 4, y * m_tile_size + 4 );
					m_edit_cursor_p->set_texcoords( 0.0f, 0.25f, 0.0f, 0.25f );
					m_edit_cursor_p->set_size( 8.0f, 8.0f );

				}
				break;

				case EDIT_MODE_PLACE_CP:
				{
					if( m_number_of_control_panels >= MAX_CPS )
						bail = true;

					if( !bail && !m_tilemap[index] )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							data.gate_cp_handle = m_number_of_control_panels++;
							m_edit_node_p[free] = new EditNode( NODE_CP, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					m_edit_cursor_p->set_pos( x * m_tile_size + 4, y * m_tile_size + 4 );
					m_edit_cursor_p->set_texcoords( 0.5f, 0.75f, 0.0f, 0.25f );
					m_edit_cursor_p->set_size( 8.0f, 8.0f );

				}
				break;

				case EDIT_MODE_PLACE_GATE:
				{
					bail = false;

					if(  mouse_button_click( 2 ) )
						m_gate_type = !m_gate_type;

					if( mouse_button_click( 0 ) )
					{
						for( int i = 0; i < MAX_NODES; i++ )
						{
							if( m_edit_node_p[i] && m_edit_node_p[i]->m_data.pos.x == x && m_edit_node_p[i]->m_data.pos.y == y )
							{
								bail = true;
								if( m_edit_node_p[i]->m_data.gate_cp_handle < m_number_of_control_panels-1 )
									m_edit_node_p[i]->m_data.gate_cp_handle++;
								else
									m_edit_node_p[i]->m_data.gate_cp_handle = 0;
								break;
							}
						}
					}

					if( m_number_of_gates >= MAX_GATES )
						bail = true;

					if( !bail )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							data.gate_cp_handle = 0;
							data.gate_type = (int)m_gate_type;
							m_edit_node_p[free] = new EditNode( NODE_GATE, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					vector2 new_size;
					new_size.x = m_gate_type ? 64 : 10;
					new_size.y = !m_gate_type ? 64 : 10;
					m_edit_cursor_p->set_texcoords( 0.5f, 0.75f, 0.25f, 0.5f );
					m_edit_cursor_p->set_size( new_size.x, new_size.y );
					m_edit_cursor_p->set_pos( x * m_tile_size + 4, y * m_tile_size + 4 );

				}
				break;

				case EDIT_MODE_PLACE_INTERCOM:
				{
					if( m_number_of_intercoms >= MAX_INTERCOMS )
						bail = true;

					if( !bail && !m_tilemap[index] )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							data.intercom_id = m_number_of_intercoms++;
							m_edit_node_p[free] = new EditNode( NODE_INTERCOM, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					m_edit_cursor_p->set_pos( x * m_tile_size + 4, y * m_tile_size + 4 );
					m_edit_cursor_p->set_texcoords( 0.25f, 0.5f, 0.25f, 0.5f );
					m_edit_cursor_p->set_size( 8.0f, 8.0f );

				}
				break;

				case EDIT_MODE_PLACE_ELECTROBALL:
				{
					if( !bail && !m_tilemap[index] )
					{
						m_edit_cursor_p->set_col( 0xffffffff );
						if( mouse_button_click( 0 ) )
						{
							node_data_s data;
							data.pos.x = x;
							data.pos.y = y;
							int free = 0;
							while( m_edit_node_p[free] ) free++;
							m_edit_node_p[free] = new EditNode( NODE_ELECTROBALL, data );
						}
					}
					else
						m_edit_cursor_p->set_col( 0x40ff1010 );

					m_edit_cursor_p->set_pos( x * m_tile_size + 4, y * m_tile_size + 4 );
					m_edit_cursor_p->set_texcoords( 0.0f, 0.25f, 0.25f, 0.5f );
					m_edit_cursor_p->set_size( 8.0f, 8.0f );

				}
				break;

				case EDIT_MODE_LEVEL_BUILDER:
				{
					m_edit_cursor_p->set_col( 0xffffffff );
					// set to current tile under mouse
					int x = mx / m_tile_size;
					int y = my / m_tile_size;
					m_edit_cursor_p->set_pos( x * m_tile_size, y * m_tile_size );
					m_edit_cursor_p->set_size( 16.0f, 16.0f );

					int index = x + ( y * MAP_WIDTH );
					if( mouse_button_down( 0 ) )
					{
						m_tilemap[index] = m_edit_cursor;
					}
					else
					if( mouse_button_down( 1 ) )
					{
						m_tilemap[index] = 0;
					}
				}
				break;

			}
		}

		if( key_hit( DIK_M ) )
		{
			save_map();
			set_new_map( MAP_SCROLL_RIGHT );
			if( m_current_map_x > 126+m_map_rooms )
				m_map_rooms++;
		}
		else
		if( key_hit( DIK_N ) )
		{
			if( m_current_map_x > 127 )
			{
				save_map();
				set_new_map( MAP_SCROLL_LEFT );
			}

		}
	}

}


void Level::save_map( void )
{

	std::ofstream fout;

	char path[64];
	sprintf( path, "%s%s_%d-%d.hmc", PATH_LEVELS, m_map_filename, m_current_map_x, m_current_map_y );
	fout.open( path );

	for( int i = 0; i < MAX_NODES; i++ )
	{
		if( !m_edit_node_p[i] )
			continue;

		node_type_e type = m_edit_node_p[i]->m_type;
		node_data_s *data_p = &m_edit_node_p[i]->m_data;

		fout << g_type_commands[type] << " ";
		fout << data_p->pos.x << " " << data_p->pos.y << " ";

		switch( type )
		{
		case NODE_ENERGY_CELL:
			break;
		case NODE_INTERCOM:
			fout << data_p->intercom_id << " ";
			break;
		case NODE_GATE:
			fout << data_p->gate_type << " " << data_p->gate_cp_handle << " ";
			break;
		case NODE_CP:
			fout << data_p->gate_cp_handle << " ";
			break;
		case NODE_TURRET:
			fout << data_p->turret_direction << " " << data_p->turret_type << " ";
			break;
		case NODE_ELECTROBALL:
			break;
		}

		fout << ";" << "\n";

		int handle = m_edit_node_p[i]->get_handle();
		g_object_list_p->remove( handle );
		m_edit_node_p[i] = NULL;

	}

	fout << "tilemap";
	for( int i= 0; i < MAP_SIZE; i++ )
		fout << " " << (int)m_tilemap[i];
	

	dbgmsg( "Map config saved\n" );

}


void Level::render( void )
{
	int num_prims = 0;

	level_vert_s *vert_p = NULL;
	level_vert_s *vertzero_p = vert_p;

	int tiles_per_row = (int)sqrtf( (float)m_num_tex_tiles );
	float uvsize = 1.0f / (float)tiles_per_row;

	int num_tiles = MAP_WIDTH * MAP_HEIGHT;
	int x, y;
	x = y = 0;
	float v0, v1, u0, u1;

	int i = 0;
	while( !m_tilemap[i] )
		i++;

	// set first position
	set_vert_pos( x, y, i-1 );

	m_col = 0xffffffff;

    // lock buffer and load the vertices into it
	g_level_buffer->Lock( 0, 0, (void**)&vert_p, 0 );

	while( i < num_tiles )
	{

		int x_grid = (int)( (x+m_tile_size) / m_tile_size );
		if( ( x_grid < MAP_WIDTH ) && m_tilemap[i] )
		{
			x += m_tile_size;

			// first set of verts for quad

			// set new UVs
			v0 = ( ( m_tilemap[i]-1 ) / tiles_per_row ) * uvsize;
			v1 = v0 + uvsize;
			float upos = ( (float)( m_tilemap[i]-1 ) / (float)tiles_per_row );
			u0 = ( upos - (int)upos );
			u1 = u0 + uvsize;

			set_vert( vert_p, x, y, u0, v0, 0 );
			vert_p++;
			set_vert( vert_p, x, y + m_tile_size, u0, v1, 0 );
			vert_p++;

			num_prims += 2;
		}
		else
		{
			// find the next vert, increment iterator
			int c = i;
			while( !m_tilemap[c] && c < num_tiles ) c++;
			if( c == num_tiles )
				break;
				
			// degens

			u0 = v0 = 0;
			u1 = v1 = 1;

			// previous vert copy
			set_vert( vert_p, x + m_tile_size, y + m_tile_size, u1, v1, 0 );
			vert_p++;

			i = c;

			set_vert_pos( x, y, i );

			// new vert copy
			set_vert( vert_p, x, y, u0, v1, 0 );
			vert_p++;

			// set new UVs
			v0 = ( ( m_tilemap[i]-1 ) / tiles_per_row ) * uvsize;
			v1 = v0 + uvsize;
			float upos = ( (float)( m_tilemap[i]-1 ) / (float)tiles_per_row );
			u0 = ( upos - (int)upos );
			u1 = u0 + uvsize;

			set_vert( vert_p, x, y, u0, v0, 0 );
			vert_p++;
			set_vert( vert_p, x, y + m_tile_size, u0, v1, 0 );
			vert_p++;

			num_prims += 4;

		}

		// second pair in quad
		set_vert( vert_p, x + m_tile_size, y, u1, v0, 0 );
		vert_p++;
		set_vert( vert_p, x + m_tile_size, y + m_tile_size, u1, v1, 0 );
		vert_p++;

		i++;

		num_prims += 2;

	}

	g_level_buffer->Unlock();


	g_d3d_device->SetTexture( 0, m_tileset.texture );

	g_d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	g_d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

	g_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_d3d_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_d3d_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	g_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

	g_d3d_device->SetStreamSource( 0, g_level_buffer, 0, sizeof( level_vert_s ) );
	g_d3d_device->SetFVF( CUSTOM_STRIP_FVF );

	// copy the vertex buffer to the back buffer
	g_d3d_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, num_prims );

	//*********************************************************************//

	if( m_edit_mode )
	{
		char mode[32];
		if( m_editor_mode != EDIT_MODE_LEVEL_BUILDER )
			sprintf( mode, "[red]INTERIOR MODE" );
		else
			sprintf( mode, "[red]MAP EDITOR MODE" );
		g_mini_font_p->print_2d_text( mode, 420, 450 );

		g_mini_font_p->print_2d_text( "Tileset", 368.0f, 270.0f );


	}

	//render these objects **they render with out this but wasnt sure on how
	//to order them correctly (they were drawing behind)

	// LIST NOW SORTED. Left these in for reference, if they need render priority sorted.
	// usage: Stick a "set_priority( float priority );" in the object's constructor, or anywhere you need.
	// There are defines for the parameter, such as RENDER_NEAR_FRONT. -AB

	//if( !m_edit_mode )
	//{
	//	for(int i = 0; i < MAX_PARTICLES; i++)
	//	{
	//		m_particle[i]->render();
	//	}

	//	for(int i = 0; i < MAX_PROJECTILES; i++)
	//	{
	//		m_projectile[i]->render();
	//	}

	//	for(int i = 0; i < m_number_of_turrets; i++)
	//	{
	//		m_turrets[i]->render();
	//	}

	//	for(int i = 0; i < m_number_of_collectables; i++)
	//	{
	//		m_collectables[i]->render();
	//	}

	//	for(int i = 0; i < m_number_of_traps; i++)
	//	{
	//		m_traps[i]->render();
	//	}
	//}
}


void Level::set_vert( level_vert_s *fvf_struct_p, int x, int y, float u, float v, int iterator )
{
	fvf_struct_p[iterator].x	= (float)x;
	fvf_struct_p[iterator].y	= (float)y;
	fvf_struct_p[iterator].z	= 0.0f;
	fvf_struct_p[iterator].rhw	= 1.0f;
	fvf_struct_p[iterator].col	= m_col;
	fvf_struct_p[iterator].u	= u;
	fvf_struct_p[iterator].v	= v;
}

void Level::set_vert_pos( int &x, int &y, int tile )
{
	int idx = tile;
	int idy = 0;
	while( idx >= MAP_WIDTH )
	{
		idx -= MAP_WIDTH;
		idy++;
	}
	x = m_tile_size * idx;
	y = m_origin_y + ( m_tile_size * idy );
}

void Level::cleanup_level( void )
{
	if( m_edit_mode )
	{
		int handle;
		for( int i = 0; i < 7; i++ )
		{
			handle = m_edit_buttons_p[i]->get_handle();
			g_object_list_p->remove( handle );
		}
		g_object_list_p->remove( m_hard_tileset_handle );
		g_object_list_p->remove( m_soft_tileset_handle );

		handle = m_edit_cursor_p->get_handle();
		g_object_list_p->remove( handle );

		for( int i = 0; i < MAX_NODES; i++ )
		{
			if( m_edit_node_p[i] )
			{
				handle = m_edit_node_p[i]->get_handle();
				g_object_list_p->remove( handle );
				m_edit_node_p[i] = NULL;
			}
		}
	}
	else
	{
		int h;
		for(int i = 0; i < MAX_PROJECTILES; i++)
		{
			if( m_projectile[i] )
			{
				h = m_projectile[i]->get_handle();
				g_object_list_p->remove( h );
				m_projectile[i] = NULL;
			}
		}
		for(int i = 0; i < MAX_PARTICLES; i++)
		{
			if( m_particle[i] )
			{
				h = m_particle[i]->get_handle();
				g_object_list_p->remove( h );
				m_particle[i] = NULL;
			}
		}
	}

	g_object_list_p->remove( m_backdrop_handle );	
}

void Level::cleanup_map( void )
{
	if( !m_edit_mode )
	{
		for( int i = 0; i < m_number_of_collectables; i++ )
		{
			int handle = m_collectables[i]->get_handle();
			g_object_list_p->remove( handle );
			m_collectables[i] = NULL;
		}

		for( int i = 0; i < m_number_of_turrets; i++ )
		{
			int handle = m_turrets[i]->get_handle();
			g_object_list_p->remove( handle );
			m_turrets[i] = NULL;
		}

		for( int i = 0; i < m_number_of_smartballs; i++ )
		{
			int handle = m_smartballs[i]->get_handle();
			g_object_list_p->remove( handle );
			m_smartballs[i] = NULL;
		}

		for( int i = 0; i < m_number_of_control_panels+m_number_of_intercoms; i++ )
		{
			int handle = m_control_panels[i]->get_handle();
			g_object_list_p->remove( handle );
			m_control_panels[i] = NULL;
		}

		for( int i = 0; i < m_number_of_gates; i++ )
		{
			int handle = m_blockers[i]->get_handle();
			g_object_list_p->remove( handle );
			m_blockers[i] = NULL;
		}
	}

	m_number_of_gates =
	m_number_of_collectables =
	m_number_of_turrets =
	m_number_of_smartballs =
	m_number_of_control_panels =
	m_number_of_intercoms = 0;

	for( int i= 0; i < MAP_SIZE; i++ )
		m_tilemap[i] = 1;
}

void Level::set_new_map( int scroll_dir )
{
	vector2 new_map;

	new_map.x = m_current_map_x;
	new_map.y = m_current_map_y;

	// set new map location
	if( scroll_dir != -1 )
	{
		scroll_dir > MAP_SCROLL_RIGHT ?
			( scroll_dir == MAP_SCROLL_UP ? new_map.y-- : new_map.y++ ) :
			( scroll_dir == MAP_SCROLL_LEFT ? new_map.x-- : new_map.x++ );
	}
	
	set_new_map(new_map);
}


void Level::set_new_map( vector2 new_map )
{

	if( !m_edit_mode )
	{
		for(int i = 0; i < MAX_PROJECTILES; i++)
			m_projectile[i]->kill();
		for(int i = 0; i < MAX_PARTICLES; i++)
			m_particle[i]->m_hide = true;
	}

	m_current_map_x = new_map.x;
	m_current_map_y = new_map.y;


	// ----- CLEANUP MAP ELEMENTS -----

	cleanup_map();

	std::ifstream fin;

	char path[64];
	sprintf( path, "%s%s_%d-%d.hmc", PATH_LEVELS, m_map_filename, m_current_map_x, m_current_map_y );
	fin.open( path );

	// ----- LOAD MAP ELEMENTS -----

	char command[32];
	for( int i = 0; i < MAX_NODES; i++ )
	{
		fin >> command;

		if( !command[0] )
			continue;

		if( !strcmp( command, "tilemap" ) )
		{
			for( int i= 0; i < MAP_SIZE; i++ )
				fin >> m_tilemap[i];
			continue;
		}

		node_data_s data;
		node_type_e type;
		for( int i = 0; i < NUM_TYPES; i++ )
		{
			if( !strcmp( command, g_type_commands[i] ) )
			{
				type = (node_type_e)i;
				break;
			}
		}

		fin >> data.pos.x >> data.pos.y;

		if( !m_edit_mode )
		{
			vector2 pos = data.pos;
			pos.x = ( data.pos.x * m_tile_size ) + ( m_tile_size - g_nodedims[type].x );
			pos.y = ( data.pos.y * m_tile_size ) + ( m_tile_size - g_nodedims[type].y );
			pos.y += m_origin_y;

			switch( type )
			{
			case NODE_ENERGY_CELL:
				m_collectables[m_number_of_collectables++] = new Collectable( pos, 1, "collectable" );
				break;
			case NODE_INTERCOM:
				{
					fin >> data.intercom_id;
					int id = m_number_of_intercoms+m_number_of_control_panels;
					m_control_panels[id] = new ControlPanel( pos, data.intercom_id, "intercom", 1);
					char soundfile[32];
					sprintf( soundfile, "sound/dialog/com_%d_%d.mp3", m_current_map_x - 127, data.intercom_id );
					m_control_panels[id]->assign_sound( soundfile );
					m_number_of_intercoms++;
				}
				break;
			case NODE_GATE:
				fin >> data.gate_type;
				fin >> data.gate_cp_handle;
				vector2 dims;
				if( data.gate_type )
				{
					dims.x = 64;
					dims.y = 10;
				}
				else
				{
					dims.x = 10;
					dims.y = 64;
				}

				pos.x = ( data.pos.x * m_tile_size );
				pos.y = ( data.pos.y * m_tile_size );
				pos.y += m_origin_y;

				m_blockers[m_number_of_gates++] = new Blocker( pos, dims, 1, data.gate_type, data.gate_cp_handle, data.gate_type == DOOR ? "door" : "bridge" );
				break;
			case NODE_CP:
				fin >> data.gate_cp_handle;
				m_control_panels[m_number_of_control_panels+m_number_of_intercoms] = new ControlPanel( pos, data.gate_cp_handle, "cp0" , 0 );
				m_number_of_control_panels++;
			break;
			case NODE_TURRET:
				{
					fin >> data.turret_direction >> data.turret_type;
					int rate = data.turret_type == TURRET_TYPE_CANNON ? 2 : 5;
					m_turrets[m_number_of_turrets++] = new Turret( pos, rate, data.turret_direction, data.turret_type, 60, "turret" );
				}
				break;
			case NODE_ELECTROBALL:
				m_smartballs[m_number_of_smartballs++] = new Actor( "fizzball", pos.x, pos.y );
				break;
			case NODE_SPAWN:
				int p = g_active_profile;
				int type = p<3 ? 0 : 1;
				int slot = p<3 ? p : p-3;

				if( !g_profile[type][slot].inuse )
				{
					g_player[0]->set_pos( pos );
					if( g_player[1] )
						g_player[1]->set_pos( pos );
					g_profile[type][slot].inuse = true;
				}
				m_spawn_pos = pos;
				m_control_panels[m_number_of_control_panels+m_number_of_intercoms] = new ControlPanel( pos, data.gate_cp_handle, "spawnpoint" , 2);
				m_number_of_control_panels++;
				break;
			}
		}
		else
		{

			switch( type )
			{
			case NODE_ENERGY_CELL:
				break;
			case NODE_INTERCOM:
				fin >> data.intercom_id;
				m_number_of_intercoms++;
				break;
			case NODE_GATE:
				fin >> data.gate_type;
				fin >> data.gate_cp_handle;
				break;
			case NODE_CP:
				fin >> data.gate_cp_handle;
				m_number_of_control_panels++;
				break;
			case NODE_TURRET:
				fin >> data.turret_direction >> data.turret_type;
				break;
			case NODE_ELECTROBALL:
				break;
			case NODE_SPAWN:
				break;
			}

			m_edit_node_p[i] = new EditNode( type, data );
			
		}

		fin >> command;

	}

	fin.close();

	// -----------------------------

}

int Level::get_grid_x( int pos )
{
	return( (int)( pos / m_tile_size ) );
}

int Level::get_grid_y( int pos )
{
	return( (int)( ( pos - (float)m_origin_y ) / m_tile_size ) );
}

int Level::hit_collidable_tile	( int tile	)
{ 
	if(tile == 0) return 0;
	else if(tile < 17) return 1;
	else if(tile > 16 && tile < 36) return 2;
	else if(tile >= 36 && tile <= 42) return 3;
	else if(tile >= 43 && tile <= 47) return 4;
	else if(tile >= 91 && tile <= 95) return 4;
	else return 0;
}

void Level::obtainGates(vector2 *position, vector2 *dimensions,int id)
{
	*position = m_blockers[id]->get_pos();
	*dimensions = m_blockers[id]->get_size();
}

void Level::animate_acid()
{
	for(int i = 0; i < MAP_SIZE; i++)
	{
		if(m_tilemap[i] >= 43 && m_tilemap[i] <= 47)
		{
			m_tilemap[i]++;
			if(m_tilemap[i] > 47) m_tilemap[i] = 43;
		}

		if(m_tilemap[i] >= 91 && m_tilemap[i] <= 95)
		{
			m_tilemap[i]++;
			if(m_tilemap[i] > 95) m_tilemap[i] = 91;
		}
	}
}

int Level::useControlPanel(int id, bool use)
{
	int gate_id;

	if(use)
	{
		g_sound_mgr_p->play(S_INTERACT,-5);

		if(m_control_panels[id]->activate(&gate_id))
		{
			if( m_control_panels[id]->get_type() == INTERCOM )
			{
				if(m_control_panels[id]->play_dialogue())
				{
					for(int i = 0; i < m_number_of_control_panels; i++)
					{
						if(i != id) m_control_panels[i]->stop();
					}
				}

				return m_control_panels[id]->get_type();
			}
			else if( m_control_panels[id]->get_type() == SAVE_POS )
			{
				return m_control_panels[id]->get_type();
			}
			else
			{
				for( int i = 0; i < m_number_of_gates; i++ )
				{
					if( m_blockers[i]->m_cp_id == gate_id )
					{
						m_blockers[i]->activate();
					}
				}
			}
		}
	}

	return m_control_panels[id]->get_type();
}

void Level::obtainControlPanels(vector2 *position, vector2 *dimensions,int id)
{
	*position = m_control_panels[id]->get_pos();
	*dimensions = m_control_panels[id]->get_size();
}

void Level::obtainSmartBalls(vector2 *position, vector2 *dimensions,int id)
{
	*position = m_smartballs[id]->get_pos();
	*dimensions = m_smartballs[id]->get_size();
}

void Level::notify_player_position (vector2 position)
{
	vector2 p1,p2;

	p1 = g_player[0]->get_pos();
	
	if(g_player[1] != NULL)
	{
		p2 = g_player[1]->get_pos();
	}
	else
	{
		p2.x = 10000;
		p2.y = 10000;
	}

	for(int i = 0; i < m_number_of_turrets; i++)
	{
		vector2 t = m_turrets[i]->get_pos();
		float d1, d2;
		vector2 v;
		vector2 closest;

		v.x = p1.x - t.x;
		v.y = p1.y - t.y;
		d1 = sqrt((v.x * v.x) + (v.y * v.y));

		v.x = p2.x - t.x;
		v.y = p2.y - t.y;
		d2 = sqrt((v.x * v.x) + (v.y * v.y));

		if(fabs(d1) < fabs(d2)) closest = p1;
		else closest = p2;

		m_turrets[i]->notify_player_position(closest);
	}

	for(int i = 0; i < m_number_of_traps; i++)
	{
		m_traps[i]->notify_player_position(position);
	}

	for(int i = 0; i < MAX_PROJECTILES; i++)
	{
		if(m_projectile[i]->active())
		{
			vector2 t = m_projectile[i]->get_pos();
			float d1, d2;
			vector2 v;
			vector2 closest;

			v.x = p1.x - t.x;
			v.y = p1.y - t.y;
			d1 = sqrt((v.x * v.x) + (v.y * v.y));

			v.x = p2.x - t.x;
			v.y = p2.y - t.y;
			d2 = sqrt((v.x * v.x) + (v.y * v.y));

			if(fabs(d1) < fabs(d2)) closest = p1;
			else closest = p2;

			m_projectile[i]->notify_player_position(closest);
		}
	}
}

int Level::collectableCollision(vector2 min_extent, vector2 max_extent)
{
	bool collect = false;

	for(int i = 0; i < m_number_of_collectables; i++)
	{
		if(m_collectables[i]->active())
		{
			vector2 pos = m_collectables[i]->get_pos();
			vector2 size = m_collectables[i]->get_size();

				bool collect = true;

				if(	   (pos.x - (size.x / 2) >= min_extent.x && pos.x - (size.x / 2) <= max_extent.x)
					|| (pos.x + (size.x / 2) >= min_extent.x && pos.x + (size.x / 2) <= max_extent.x)
					|| (min_extent.x >= pos.x - (size.x / 2) && min_extent.x <= pos.x + (size.x / 2))
					|| (max_extent.x >= pos.x - (size.x / 2) && max_extent.x <= pos.x + (size.x / 2)))
				{
				}
				else
				{
					collect = false;
				}

				if(	   (pos.y - (size.y / 2) >= min_extent.y && pos.y - (size.y / 2) <= max_extent.y) 
					|| (pos.y + (size.y / 2) >= min_extent.y && pos.y + (size.y / 2) <= max_extent.y)
					|| (min_extent.y >= pos.y - (size.y / 2) && min_extent.y <= pos.y + (size.y / 2))
 					|| (max_extent.y >= pos.y - (size.y / 2) && max_extent.y <= pos.y + (size.y / 2)))
				{
				}
				else
				{
					collect = false;
				}

			if(collect)
			{
				g_sound_mgr_p->play(S_COLLECTABLE,-15);
				m_collectables[i]->collect();
				return m_collectables[i]->type();
				
			}
		}
	}

	return 0;

}

bool Level::projectileCollision(vector2 min_extent, vector2 max_extent, vector2 *offset)
{
	offset->x = 0;
	offset->y = 0;

	for(int i = 0; i < MAX_PROJECTILES; i++)
	{
		if(m_projectile[i]->active())
		{
			vector2 pos = m_projectile[i]->get_pos();

			int type = m_projectile[i]->get_type();

			if(type == 2)
			{
				vector2 size = m_projectile[i]->get_size();

				bool collided = true;

				if(	   (pos.x - (size.x / 2) >= min_extent.x && pos.x - (size.x / 2) <= max_extent.x)
					|| (pos.x + (size.x / 2) >= min_extent.x && pos.x + (size.x / 2) <= max_extent.x)
					|| (min_extent.x >= pos.x - (size.x / 2) && min_extent.x <= pos.x + (size.x / 2))
					|| (max_extent.x >= pos.x - (size.x / 2) && max_extent.x <= pos.x + (size.x / 2)))
				{
				}
				else
				{
					collided = false;
				}

				if(	   (pos.y - (size.y / 2) >= min_extent.y && pos.y - (size.y / 2) <= max_extent.y) 
					|| (pos.y + (size.y / 2) >= min_extent.y && pos.y + (size.y / 2) <= max_extent.y)
					|| (min_extent.y >= pos.y - (size.y / 2) && min_extent.y <= pos.y + (size.y / 2))
 					|| (max_extent.y >= pos.y - (size.y / 2) && max_extent.y <= pos.y + (size.y / 2)))
				{
				}
				else
				{
					collided = false;
				}

				if(collided)
				{
					return true;
				}
			}
			else
			{
				if(pos.x > min_extent.x && pos.x < max_extent.x 
					&& pos.y > min_extent.y && pos.y < max_extent.y)
				{
					
					m_projectile[i]->kill();
					return true;
				}
			}
		}
	}

	return false;
}

short Level::get_tile(int index)
{ 
	if(index < 640 && index > 0)
	{
		return( m_tilemap[index] );
	}
	
	return 0;
}

void Level::get_current_map(vector2 *pos)
{
	pos->x = m_current_map_x;
	pos->y = m_current_map_y;
}