/****************************************
/	Al (c) 2008 - sprite.cpp
/****************************************
/	2D Sprite
/****************************************/


/*************** INCLUDES ***************/

#include "sprite.h"

#include "2dscene.h"

#include <stdio.h>
#include <iostream>
#include <fstream>


/*************** CONSTANTS **************/

#define		ANIM_SPEED		6.0f

/*********** GLOBAL VARIABLES ***********/

char g_anim_config_keywords[][16] =
{
	{"STAND"},
	{"WALK"},
	{"DEATH"},
	{"JUMP"},
	{"INAIR"},
	{"LAND"},
	{"SAVE"},
	{"SPAWN"},
	{"DEATH2"},
	{"DEATH3"}
};

/********* FUNCTION PROTOTYPES **********/


Sprite::Sprite( char *path_p, char *name_p, float x, float y, unsigned int col ) :
	Quad( path_p, name_p, x, y, 16, 16, col ),
	m_dir( 1 ),
	m_anim_state( ANIM_STATE_STAND ),
	m_last_state( ANIM_STATE_STAND ),
	m_ticker( 0 ),
	m_velocity_y( 0.0f ),
	m_current_frame( 0 )
{
	m_frame_origin.x = 0.0f;
	m_frame_origin.y = 0.0f;

	m_sprite.texture_size.x = (float)m_tex.w;
	m_sprite.texture_size.y = (float)m_tex.h;
	
	char filename[64];
	sprintf( filename, "data/sprite configs/%s.asc", name_p );
	read_script( filename );

	// ----------------------------------------

	m_texel_to_uv_x = 1.0f / m_sprite.texture_size.x;
	m_texel_to_uv_y = 1.0f / m_sprite.texture_size.y;
}


void Sprite::read_script( char *script_p )
{
	std::ifstream fin;

	fin.open(script_p);

	char command[32];
	int total_num_frames;

	while(!fin.eof())
	{
		fin >> command;

		if(!strcmp(command,"number_of_states"))
		{
			fin >>  m_sprite.anim_states;
		}
		else if(!strcmp(command,"state_frames"))
		{
			int stateID;
			char state[16];
			fin >> state;
			for( int i = 0; i < MAX_ANIM_STATES; i++ )
			{
				if( !strcmp( g_anim_config_keywords[i], state ) )
				{
					stateID = i;
					break;
				}
			}
			
			int frames;
			fin >> frames;
			m_sprite.frames[stateID] = frames;
		}
		else if(!strcmp(command,"total_frames"))
		{
			fin >> total_num_frames;

			m_sprite.frame_size = new vector2[total_num_frames];

			int frame_counter = 0;

			while(frame_counter != total_num_frames)
			{
				fin >> command;

				if(!strcmp(command,"for"))
				{
					int loop_count;

					fin >> loop_count;

					fin >> command;

					int x,y;
					fin >> x;
					fin >> y;

					for(int i = 0; i < loop_count; i++)
					{
						m_sprite.frame_size[frame_counter].x = x;
						m_sprite.frame_size[frame_counter].y = y;
						
						frame_counter++;
					}

					if(frame_counter != total_num_frames) fin >> command;
				}
			}
		}
		else if(!strcmp(command,"#end"))
		{
			fin.close();
			return;
		}
	}
}


Sprite::~Sprite()
{
	if( m_sprite.frame_size )
		delete [] m_sprite.frame_size;
}



void Sprite::execute( void )
{
	int last_frame = m_current_frame;
	if( m_last_state != m_anim_state )
	{
		m_ticker = ANIM_SPEED;
		m_last_state = m_anim_state;
	}

	switch( m_anim_state )
	{
	case ANIM_STATE_STAND:
		m_current_frame = 0;
		if( m_current_frame != last_frame )
			update_animation();
		m_ticker = 0.0f;
		break;

	case ANIM_STATE_WALK:
		m_ticker += g_scene_p->m_game_time * 160.0f;
		if( m_ticker >= ANIM_SPEED )
		{
			int final_frame = 0;
			for( int i = 0; i < m_anim_state+1; i++ )
				final_frame += m_sprite.frames[i];

			int start_frame = 0;
			for( int i = 0; i < m_anim_state; i++ )
				start_frame += m_sprite.frames[i];

			m_current_frame++;
			if( m_current_frame >= final_frame || m_current_frame < start_frame)
				m_current_frame = start_frame;

			m_ticker = 0.0f;

			update_animation();

		}
		break;

	case ANIM_STATE_DEATH:
	case ANIM_STATE_DEATH2:
	case ANIM_STATE_DEATH3:
	{

		m_ticker += g_scene_p->m_game_time * 160.0f;
		if( m_ticker >= ANIM_SPEED )
		{
			int final_frame = 0;
			for( int i = 0; i < m_anim_state+1; i++ )
				final_frame += m_sprite.frames[i];

			int start_frame = 0;
			for( int i = 0; i < m_anim_state; i++ )
				start_frame += m_sprite.frames[i];

			m_current_frame++;
			if(m_current_frame < start_frame)
				m_current_frame = start_frame;

			bool update = true;

			if(m_current_frame >= final_frame)
			{
				m_current_frame = final_frame - 1;

				update = false;

				m_anim_state = ANIM_STATE_FINISHED;
			}

			m_ticker = 0.0f;
			if(update) update_animation();
		}
	}
	break;

	case ANIM_STATE_SPAWN:
	case ANIM_STATE_SAVE:
	{
		m_ticker += g_scene_p->m_game_time * 160.0f;
		if( m_ticker >= ANIM_SPEED )
		{
			int final_frame = 0;
			for( int i = 0; i < m_anim_state+1; i++ )
				final_frame += m_sprite.frames[i];

			int start_frame = 0;
			for( int i = 0; i < m_anim_state; i++ )
				start_frame += m_sprite.frames[i];

			m_current_frame++;
			if(m_current_frame < start_frame)
				m_current_frame = start_frame;

			bool update = true;

			if(m_current_frame >= final_frame)
			{
				m_current_frame = final_frame - 1;

				update = false;

				m_anim_state = ANIM_STATE_STAND;
			}

			m_ticker = 0.0f;
			if(update) update_animation();
		}

	}
	break;

	case ANIM_STATE_TAKE_OFF:

		m_ticker += g_scene_p->m_game_time * 160.0f;
		if( m_ticker >= ANIM_SPEED )
		{
			int final_frame = 0;
			for( int i = 0; i < m_anim_state+1; i++ )
				final_frame += m_sprite.frames[i];

			int start_frame = 0;
			for( int i = 0; i < m_anim_state; i++ )
				start_frame += m_sprite.frames[i];

			m_current_frame++;
			if(m_current_frame < start_frame)
				m_current_frame = start_frame;

			bool update = true;

			if(m_current_frame >= final_frame)
			{
				m_current_frame = final_frame - 1;

				update = false;

				m_anim_state = ANIM_STATE_IN_AIR;
			}

			m_ticker = 0.0f;
			if(update) update_animation();
		}
	break;

	case ANIM_STATE_IN_AIR:
	{
		
		int final_frame = 0;
		for( int i = 0; i < m_anim_state+1; i++ )
				final_frame += m_sprite.frames[i];

		int start_frame = 0;
		for( int i = 0; i < m_anim_state; i++ )
				start_frame += m_sprite.frames[i];

		if(m_current_frame < start_frame || m_velocity_y < 0.0f )
			m_current_frame = start_frame;
		else if( m_velocity_y >= 0.0f )
			m_current_frame = final_frame - 1;

		update_animation();
	}
	break;

	case ANIM_STATE_LAND:
	{

		m_ticker += g_scene_p->m_game_time * 160.0f;
		if( m_ticker >= ANIM_SPEED )
		{
			int final_frame = 0;
			for( int i = 0; i < m_anim_state+1; i++ )
				final_frame += m_sprite.frames[i];

			int start_frame = 0;
			for( int i = 0; i < m_anim_state; i++ )
				start_frame += m_sprite.frames[i];

			m_current_frame++;
			if(m_current_frame < start_frame)
				m_current_frame = start_frame;

			bool update = true;

			if(m_current_frame >= final_frame)
			{
				m_current_frame = final_frame - 1;

				update = false;

				m_anim_state = ANIM_STATE_STAND;
			}

			m_ticker = 0.0f;
			if(update) update_animation();
		}
	}
	break;

	}

	Quad::execute();
}


void Sprite::render( void )
{

	if( m_dir )
	{
		m_texcoords.u0 = ( m_frame_origin.x * m_texel_to_uv_x );
		m_texcoords.u1 = m_texcoords.u0 + (m_sprite.frame_size[m_current_frame].x * m_texel_to_uv_x );
	}
	else
	{
		m_texcoords.u1 = ( m_frame_origin.x * m_texel_to_uv_x );
		m_texcoords.u0 = m_texcoords.u1 + (m_sprite.frame_size[m_current_frame].x * m_texel_to_uv_x );
	}

	m_texcoords.v0 = m_frame_origin.y * m_texel_to_uv_y;
	m_texcoords.v1 = m_texcoords.v0 + ( m_sprite.frame_size[m_current_frame].y * m_texel_to_uv_y );

	m_size = m_sprite.frame_size[m_current_frame];

	Quad::draw_textured_quad();

}


void Sprite::update_animation( void )
{
	float max_y = 0;
	vector2 new_pos;
	new_pos.x = 0;
	new_pos.y = 0;

	// find the current frame in our sprite texture
	for( int i = 0; i < m_current_frame; i++ )
	{
		if( m_sprite.frame_size[i].y > max_y )
			max_y = m_sprite.frame_size[i].y;

		new_pos.x += m_sprite.frame_size[i].x;

		if( new_pos.x > m_sprite.texture_size.x - m_sprite.frame_size[i+1].x )
		{
			new_pos.x = 0;
			new_pos.y += max_y;
		}
	}

	m_frame_origin.x = new_pos.x;
	m_frame_origin.y = new_pos.y;
}
