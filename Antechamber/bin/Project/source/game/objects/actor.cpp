/****************************************
/	Al (c) 2008 - actor.cpp
/****************************************
/	Sprite based character
/****************************************/


/*************** INCLUDES ***************/

#include "actor.h"
#include "dappinput.h"
#include "d3dapp.h"

#include "level.h"
#include "font.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

bool d = false;


/*************** CONSTANTS **************/

#define		RUN_SPEED			200.0f
#define		WALK_SPEED			100.0f
#define		ACCELERATION		18.0f

#define		JUMP_VELOCITY		-3.2f
#define		FALL_ACCELERATION	11.0f
#define		TERMINAL_VELOCITY	220.0f

#define		JUMP_MOVE_DRIFT		0.8f

/*********** GLOBAL VARIABLES ***********/



/********* FUNCTION DEFINITIONS *********/


Actor::Actor( char *name_p, float x, float y, bool user_control, int player_number, unsigned int col ) :
	Sprite( PATH_SPRITES, name_p, x, y, col ),
	m_jumps( 0 ),
	m_user_control( user_control ),
	m_gravity( TERMINAL_VELOCITY ),
	m_velocity_x( 0.0f ),
	m_origin( 0 ),
	m_state( ACTOR_STATE_INIT )
{
	m_direction_timer = 0;
	m_pressed_jump = false;
	m_pressed_interact = false;

	m_lives = 5;
	m_score = 0;

	m_emmit_particle = false;

	set_priority( RENDER_FRONT );

	m_dead = false;
	m_jumping = false;
	m_in_air = false;
	m_landing = false;
	m_saving = false;

	m_respawn_timer = 0;

	m_control_scheme = player_number - 1;

	int p = g_active_profile;
	int type = p<3 ? 0 : 1;
	int slot = p<3 ? p : p-3;

	if( m_user_control && g_profile[type][slot].inuse )
		load_save_data();

	m_display_tooltip = false;

}

void Actor::load_save_data()
{
	int p = g_active_profile;
	int type = p<3 ? 0 : 1;
	int slot = p<3 ? p : p-3;
	m_pos = g_profile[type][slot].position;
	m_lives = g_profile[type][slot].lives;
	m_score = g_profile[type][slot].score;

}


Actor::~Actor()
{
}

void Actor::save_game()
{
	if(!m_saving)
	{
		vector2 current_map;
		g_level_p->get_current_map(&current_map);

		std::ofstream fout;

		char file[32];
		sprintf( file, "save/save%d%d.txt", g_active_profile<3?0:1, g_active_profile>=3?g_active_profile-3:g_active_profile );
		fout.open( file );

		fout << "position" << " : " << m_pos.x << " , " << m_pos.y << "\n";
		fout << "map" << " : " << current_map.x << " , " << current_map.y << "\n";
		fout << "score" << " : " << m_score << "\n";
		fout << "lives" << " : " << m_lives;

		fout.close();
		
		m_anim_state = ANIM_STATE_SAVE;
		m_saving = true;
	}
}

void Actor::jump()
{
	if(m_jumps == 0) 
	{
		g_sound_mgr_p->play(g_sound_id.jump);
		m_anim_state = ANIM_STATE_TAKE_OFF;
		m_velocity_y = JUMP_VELOCITY;
	
	}
	else
	{
		g_sound_mgr_p->play(g_sound_id.double_jump);
		m_anim_state = ANIM_STATE_TAKE_OFF;
		m_velocity_y = JUMP_VELOCITY;
		m_in_air = false;
	}

	m_jumping = true;

	m_jumps++;
}

void Actor::land()
{
	if(m_in_air)
	{
		m_anim_state = ANIM_STATE_LAND;
		m_in_air = false;
		m_jumping = false;
		m_landing = true;

		vector2 particlePos;
		particlePos.x = m_pos.x + (m_size.x * 0.5f);
		particlePos.y = m_pos.y + m_size.y;

		g_level_p->spawn_particle(particlePos);
		g_sound_mgr_p->play(g_sound_id.land,0);
	}

}

void Actor::control()
{
	// player jump keyboard
	if( key_hit(g_player_keymap[m_control_scheme].jump) )
	{
		if( m_jumps < 2 )
		{
			jump();
		}
	}

	// player jump xbox pad
	if(g_gamepad[m_control_scheme].state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if(!m_pressed_jump)
		{
			if( m_jumps < 2 )
			{	
				jump();

				m_pressed_jump = true;
			}
		}
	}
	else
	{
		m_pressed_jump = false;
	}

	//switch run/walk
	float v, a;

	if(!m_jumping && !m_landing)
	{
		m_anim_state = ANIM_STATE_WALK;
	}

	v = WALK_SPEED * g_scene_p->m_game_time;
	a = ACCELERATION;

	//xbox pad conversion
	int analog_stick = 0;

	// Apply deadzone if centered
	if( (g_gamepad[m_control_scheme].state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 
		&& g_gamepad[m_control_scheme].state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
	    (g_gamepad[m_control_scheme].state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 
		&& g_gamepad[m_control_scheme].state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ) 
	{	
	    g_gamepad[m_control_scheme].state.Gamepad.sThumbLX = 0;
	    g_gamepad[m_control_scheme].state.Gamepad.sThumbLY = 0;
	}

	analog_stick = (g_gamepad[m_control_scheme].state.Gamepad.sThumbLX / 32767.0f);


	d = false;
		// player movement
		if( key_down( g_player_keymap[m_control_scheme].left) || 
			(g_gamepad[m_control_scheme].state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
		{
			d = true;

			if(m_dir == 0 || m_velocity_x == 0) 
			{
				if(m_jumps == 0)
				{
					vector2 particlePos;
					particlePos.x = m_pos.x + (m_size.x * 0.5f);
					particlePos.y = m_pos.y + m_size.y;

					g_level_p->spawn_particle(particlePos);
				}
			}

			if( m_velocity_x > -v )
				m_velocity_x -= a * g_scene_p->m_game_time;
			if( m_velocity_x < -v )
				m_velocity_x = -v;
			
			m_dir = 1;
		}
		else if(analog_stick != 0)
		{
			int old_dir = m_dir;

			if(analog_stick > 0) m_dir = 1;
			else m_dir = 0;

			if(m_velocity_x == 0) 
			{
				if(m_jumps == 0)
				{
					vector2 particlePos;
					particlePos.x = m_pos.x + (m_size.x * 0.5f);
					particlePos.y = m_pos.y + m_size.y;

					g_level_p->spawn_particle(particlePos);
				}
			}

			if(analog_stick < 0) m_dir = 1;
			else m_dir = 0;

			if(m_dir == 1)
			{
				if( m_velocity_x > -v )
					m_velocity_x -= a * g_scene_p->m_game_time * (-analog_stick);
				if( m_velocity_x < -v )
					m_velocity_x = -v;
			}
			else
			{
				if( m_velocity_x < v )
				m_velocity_x += a * g_scene_p->m_game_time * (analog_stick);
				if( m_velocity_x > v )
					m_velocity_x = v;
			}
			


		}
		else
		if( key_down( g_player_keymap[m_control_scheme].right ) 
			|| (g_gamepad[m_control_scheme].state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			if(m_dir == 1 || m_velocity_x == 0) 
			{
				if(m_jumps == 0)
				{
					vector2 particlePos;
					particlePos.x = m_pos.x + (m_size.x * 0.5f);
					particlePos.y = m_pos.y + m_size.y;

					g_level_p->spawn_particle(particlePos);
				}
			}

			if( m_velocity_x < v )
				m_velocity_x += a * g_scene_p->m_game_time;
			if( m_velocity_x > v )
				m_velocity_x = v;

			m_dir = 0;
		}
		else
		{
			if(!m_jumping && !m_landing && m_anim_state != ANIM_STATE_SPAWN)
			{
				m_anim_state = ANIM_STATE_STAND;
			}
			else if( m_anim_state != ANIM_STATE_STAND )
			{
				//// reduce velocity until almost stopped
				if( fabs( m_velocity_x ) > 1.0f )
					m_velocity_x *= JUMP_MOVE_DRIFT;
				else
 					m_velocity_x = 0.0f;
			}


		}
}

void Actor::execute( void )
{
	switch( m_state )
	{
		case ACTOR_STATE_INIT:
		{
			m_origin = ( !g_editor_mode ? (int)( ( (float)g_window_h - 256.0f ) * 0.5f ) : 0 );

			if( g_level_p )
			{
				int tile_size = g_level_p->get_tile_size();
				int level_width = g_level_p->get_map_width();
				m_last_x = (int)( m_pos.x / tile_size );
				m_last_y = (int)( ( m_pos.y - (float)m_origin ) / tile_size );

				m_state = ACTOR_STATE_IDLE;
			}
		}
		break;


		case ACTOR_STATE_IDLE:
		{
			m_pos.x += m_velocity_x;
			m_pos.y += m_velocity_y;

			if(!m_dead && !m_jumping && !m_landing && !m_saving && m_anim_state != ANIM_STATE_SPAWN) m_anim_state = ANIM_STATE_STAND;

			if( m_pos.y < g_window_h - m_size.y - m_origin - 1 )
			{
				if( m_velocity_y < m_gravity )
					m_velocity_y += FALL_ACCELERATION * g_scene_p->m_game_time;
			}
			else
			{
				m_pos.y = g_window_h - m_size.y - m_origin - 1;
				m_velocity_y = 0.0f;
				m_jumps = 0;
			}

			if( g_level_p )
			{
				check_level_collision();
			}

			// stick this in a player class at some point...
			if( m_user_control )
			{
				if(!m_dead && !m_saving && m_anim_state != ANIM_STATE_SPAWN)
				{
					control();
				}
			}
			else
			{
					if(m_direction_timer > 0)
					{
						m_direction_timer -= g_scene_p->m_game_time;
					}

					float v, a;

					m_anim_state = ANIM_STATE_WALK;
					v = WALK_SPEED * g_scene_p->m_game_time;
					a = ACCELERATION;

					if(m_dir)
					{
						if( m_velocity_x > -v )
							m_velocity_x -= a * g_scene_p->m_game_time;
						if( m_velocity_x < -v )
							m_velocity_x = -v;
					}
					else
					{
						if( m_velocity_x < v )
							m_velocity_x += a * g_scene_p->m_game_time;
						if( m_velocity_x > v )
							m_velocity_x = v;
					}
			}
		}

		break;
	}

	if( m_anim_state == ANIM_STATE_STAND )
	{
		// half velocity until almost stopped
		if( fabs( m_velocity_x ) > 1.0f )
			m_velocity_x *= 0.5f;
		else
			m_velocity_x = 0.0f;
	}

	if(m_saving)
	{
		if(m_anim_state == ANIM_STATE_STAND)
		{
			m_saving = false;
		}
	}

	if(m_dead)
	{
		m_landing = false;
		m_in_air = false;
		m_jumping = false;

		if(m_anim_state == ANIM_STATE_FINISHED)
		{
			m_respawn_timer -= g_scene_p->m_game_time;

			if(m_respawn_timer <= 0)
			{
				m_anim_state = ANIM_STATE_SPAWN;
				m_dead = false;
				m_pos = g_level_p->get_spawn_pos();
				m_pos.y -= 12;
				m_pos.x += 3;
				
				g_sound_mgr_p->play(g_sound_id.respawn,15);
				
			}
		}
		else
		{
			m_respawn_timer = RESPAWN_RATE;
			m_anim_state = ANIM_STATE_DEATH;
		}
	}

	if(m_jumping)
	{
		if(!m_in_air)
		{
			if(m_anim_state == ANIM_STATE_IN_AIR)
			{
				m_in_air = true;
			}
		}
	}

	if(m_landing)
	{
		if(m_anim_state == ANIM_STATE_STAND)
		{
			m_landing = false;
		}
	}

	// animated sprite
	Sprite::execute();

}


void Actor::render( void )
{
	if(m_user_control)
	{
		if(m_control_scheme == 0)
		{
			char output[64];
			sprintf( output, "%cLives%c %d", S_COL_GREY, 1, m_lives );
			g_mini_font_p->print_2d_text( output, 8, 68, 1.0f, 0xffe0e000 );
			sprintf( output, "%cScore%c %d", S_COL_GREY, 1, m_score );
			g_mini_font_p->print_2d_text( output, 8, 78, 1.0f, 0xffe0e000 );

			if(d) g_mini_font_p->print_2d_text( "on", 8, 88, 1.0f, 0xffe0e000 );

			if(m_display_tooltip)
			{
				char text[64];
				sprintf( text, "[Press Keyboard[W] or Xbox Pad[X] to Interact]" );
				g_mini_font_p->print_2d_text( text, 320-g_mini_font_p->get_text_width( text )*0.5f, 68, 1.0f, 0xff32e00a );
			}

		}
		else if(m_control_scheme == 1)
		{
			char output[64];
			sprintf( output, "%cLives%c %d", S_COL_GREY, 1, m_lives );
			g_mini_font_p->print_2d_text( output, 600, 68, 1.0f, 0xffe0e000 );
			sprintf( output, "%cScore%c %d", S_COL_GREY, 1, m_score );
			g_mini_font_p->print_2d_text( output, 600, 78, 1.0f, 0xffe0e000 );

			if(m_display_tooltip)
			{
				char text[64];
				sprintf( text, "[Press Keyboard[NUMPAD 8] or Xbox Pad[X] to Interact]" );
				g_mini_font_p->print_2d_text( text, 320-g_mini_font_p->get_text_width( text )*0.5f, 78, 1.0f, 0xff32e00a );
			}
		}
	}

	// animated sprite render
	Sprite::render();
}


void Actor::check_level_collision( void )
{
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x = g_level_p->get_grid_x( m_pos.x);
	int level_y = g_level_p->get_grid_y( m_pos.y + m_size.y );

	if( m_user_control )
	{
		bool playerLock = true;
		bool moveOther = false;
		
		int other_player = 1;
		if(m_control_scheme == 1) other_player = 0;

		if(g_player[other_player] != NULL)
		{
			vector2 p1, p2, v;

			p1 = g_player[0]->get_pos();
			p2 = g_player[1]->get_pos();

			v.x = p1.x - p2.x;
			v.y = p1.y - p2.y;

			float distance = sqrt((v.x * v.x) + (v.y * v.y));

			if(distance < 50 && distance > -50) playerLock = false;

			moveOther = true;
		}
		else
		{
			playerLock = false;
		}

		if(!playerLock)
		{
			// MAP CHANGE
			if( (m_pos.x + (m_size.x / 2)) > ( level_width * tile_size ) )
			{
				g_level_p->set_new_map( MAP_SCROLL_RIGHT );
				m_pos.x = 0.0f;
				if(moveOther) g_player[other_player]->set_pos(m_pos);
				level_x = g_level_p->get_grid_x( m_pos.x );
			}
			else
			if( m_pos.x + (m_size.x / 2) < -m_size.x )
			{
				g_level_p->set_new_map( MAP_SCROLL_LEFT );
				m_pos.x = ( level_width * tile_size ) - ( m_size.x);
				if(moveOther) g_player[other_player]->set_pos(m_pos);
				level_x = g_level_p->get_grid_x( m_pos.x );
			}
		}
		else
		{
			if(m_pos.x + (m_size.x) > 640)
			{
				m_pos.x = (640 - (m_size.x));
			}
			else if(m_pos.x < 0)
			{
				m_pos.x = 0;
			}
		}
	}

	new_collision_check();
}

player_bb_s	Actor::build_bounding_box( void )
{
	/*calculate the players bounries

		player sprite
		|    | T  |   |
		---------------
		| Bk |    | F |
		---------------
		|    | B  |   |
	*/

	player_bb_s player_bb;

	player_bb.bottom.x = m_pos.x + (m_size.x * 0.5f);
	player_bb.bottom.y = m_pos.y + m_size.y;

	player_bb.top.x = m_pos.x + (m_size.x * 0.5f);
	player_bb.top.y = m_pos.y;

	player_bb.back.x = m_pos.x;
	player_bb.back.y = m_pos.y + (m_size.y * 0.5f);

	player_bb.front.x = m_pos.x + m_size.x;
	player_bb.front.y = m_pos.y + (m_size.y * 0.5f);

	return player_bb;
}

void Actor::new_collision_check ( void )
{
	player_bb_s player_bb = build_bounding_box();
	
	slope_collision(player_bb);
	underside_slope_collision(player_bb);

	player_bb = build_bounding_box();

	gate_collision(player_bb);

	if(m_velocity_y > 5)
	{
		//check floors as a priority

		floor_collision(player_bb);

		player_bb = build_bounding_box();

		//then check the tile components for collision
		wall_collision(player_bb, (float)m_size.y * 0.25f);
		wall_collision(player_bb, (float)m_size.y * -0.25f);

	}
	else
	{
		//check walls as priority

		//then check the tile components for collision
		wall_collision(player_bb, (float)m_size.y * 0.25f);
		wall_collision(player_bb, (float)m_size.y * -0.25f);

		player_bb = build_bounding_box();

		floor_collision(player_bb);

	}

	if(!m_user_control)
	{
		player_bb = build_bounding_box();
		check_for_ledge(player_bb);
	}
	else
	{
		pool_collision(player_bb);

		//interact with level objects
		//update boundries
		player_bb = build_bounding_box();

		m_display_tooltip = false;
	
		//check agaisnt control panels
		check_control_panels(player_bb);

		//get the center of the player
		vector2 center = m_pos;
		center.x += m_size.x * 0.5f;
		center.y += m_size.y * 0.5f;

		//notify the leve where we are at, so AI can respond accordingly
		g_level_p->notify_player_position(center);

		//get an old-school boundry box
		vector2 min_extent;
		vector2 max_extent;

		min_extent.x = m_pos.x;
		min_extent.y = m_pos.y;

		max_extent.x = m_pos.x + m_size.x;
		max_extent.y = m_pos.y + m_size.y;

		vector2 offey;

		if(!m_dead)
		{
			//check agaisnt little objects like projectiles and collecatbles
			if(g_level_p->projectileCollision(min_extent,max_extent,&offey))
			{

					m_lives--;

					m_velocity_x = 0;
					m_velocity_y = 0;

					m_dead = true;
					m_anim_state = ANIM_STATE_DEATH;

					m_landing = false;
					m_in_air = false;
					m_jumping = false;

					m_current_frame = 0;

					g_sound_mgr_p->play(g_sound_id.death,25);
			}

			actor_collision();
		}

		int points = g_level_p->collectableCollision(min_extent,max_extent);
		m_score += 10 * points;

		if(m_velocity_y > 3) 
		{
			m_in_air = true;
			m_jumping = true;
			m_anim_state = ANIM_STATE_IN_AIR;
		}
	}
	
}

void Actor::respawn()
{
	m_pos.x = g_level_p->get_spawn_pos().x;
	m_pos.y = g_level_p->get_spawn_pos().y;
}

void Actor::wall_collision( player_bb_s player_boundry, float additional_offset)
{
	/*
	wall collisions we check the players
	left and right boundries agaisnt blocks
	that are not slopes and offset the x-axis
	position to respond to a collision

	**additional_offset**
	is used so we can check more points along the 
	front or back of the player, so instead of just the middle we could do
	0.25 from the middle or -0.25 (for instance) to get more accurate results
	just use 0 for a standard collision
	*/

	//get necessary level info we need
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;
	int tile_id;

	potential_collider_s potential_collider;

	if(player_boundry.front.x < 640)
	{
		//firstly check the right (front) boundry
		level_x = g_level_p->get_grid_x( player_boundry.front.x);
		level_y = g_level_p->get_grid_y( player_boundry.front.y + additional_offset);

		potential_collider.id = level_x + (level_y * level_width); 
		potential_collider.start.x = level_x * tile_size;
		potential_collider.start.y = level_y * tile_size + m_origin;
		potential_collider.end.x = level_x * tile_size + tile_size;
		potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

		tile_id = g_level_p->get_tile( potential_collider.id );

		if( g_level_p->hit_collidable_tile( tile_id ) == 1)
		{
			m_pos.x = potential_collider.start.x - m_size.x;

			if((!m_user_control) && (m_direction_timer <= 0)) 
			{
				m_dir = !m_dir;
				m_direction_timer = 2;
			}
		}
	}


	//now check the left (back) boundry
	level_x = g_level_p->get_grid_x( player_boundry.back.x);
	level_y = g_level_p->get_grid_y( player_boundry.back.y + additional_offset);

	potential_collider.id = level_x + (level_y * level_width); 
	potential_collider.start.x = level_x * tile_size;
	potential_collider.start.y = level_y * tile_size + m_origin;
	potential_collider.end.x = level_x * tile_size + tile_size;
	potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

	tile_id = g_level_p->get_tile( potential_collider.id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 1)
	{
		m_pos.x = potential_collider.end.x;

		if((!m_user_control) && (m_direction_timer <= 0)) 
		{
			m_dir = !m_dir;
			m_direction_timer = 2;
		}
	}

}

void Actor::floor_collision( player_bb_s player_boundry)
{
	int poor = 1;
	/*
	floor collisions we check the players
	top and bottom boundries agaisnt blocks
	that are not slopes and offset the y-axis
	position to respond to a collision
	*/

	//get neccessary level info
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;

	potential_collider_s potential_collider;

	//firstly check top boundry
	level_x = g_level_p->get_grid_x( player_boundry.top.x);
	level_y = g_level_p->get_grid_y( player_boundry.top.y);

	potential_collider.id = level_x + (level_y * level_width); 
	potential_collider.start.x = level_x * tile_size;
	potential_collider.start.y = level_y * tile_size + m_origin;
	potential_collider.end.x = level_x * tile_size + tile_size;
	potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

	int tile_id = g_level_p->get_tile( potential_collider.id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 1)
	{
		m_pos.y = potential_collider.end.y;
		if(m_velocity_y < 0) m_velocity_y = 0;
	}

	//secondly check bottom boundry
	level_x = g_level_p->get_grid_x( player_boundry.bottom.x);
	level_y = g_level_p->get_grid_y( player_boundry.bottom.y);

	potential_collider.id = level_x + (level_y * level_width); 
	potential_collider.start.x = level_x * tile_size;
	potential_collider.start.y = level_y * tile_size + m_origin;

	tile_id = g_level_p->get_tile( potential_collider.id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 1)
	{
		m_pos.y= potential_collider.start.y - m_size.y;
		m_velocity_y = 0;
		m_jumps = 0;
		m_rotation = 0;

		land();
	}
}

void Actor::slope_collision( player_bb_s player_boundry)
{
	/*
	slope collisions we check the players
	bottom boundry agaisnt blocks that
	are slopes and offset the y-axis using y = mx + c
	where m is the gradient and c is the start of the slope
	*/

	//get neccessary level info
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;
	int tile_id;

	potential_collider_s potential_collider;

	//slopes
	level_x = g_level_p->get_grid_x( player_boundry.bottom.x);
	level_y = g_level_p->get_grid_y( player_boundry.bottom.y - 1);

	potential_collider.id = level_x + (level_y * level_width); 
	potential_collider.start.x = level_x * tile_size;
	potential_collider.start.y = level_y * tile_size + m_origin;
	potential_collider.end.x = level_x * tile_size + tile_size;
	potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

	tile_id = g_level_p->get_tile( potential_collider.id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 2)
	{
			int slope_formula_index = tile_id - 17;

			float m = slope_formula[slope_formula_index].m;
			float c = slope_formula[slope_formula_index].c_ratio * tile_size;

			float x = player_boundry.bottom.x - potential_collider.end.x;
			float base = potential_collider.end.y - m_size.y;

			float eqr = m * x + c;
			float slope_pos = base - eqr;
			
			if(m_pos.y >= slope_pos) 
			{
				m_pos.y = slope_pos;
				m_velocity_y = 0;
				m_jumps = 0;

				land();
			}
	}
}

void Actor::underside_slope_collision( player_bb_s player_boundry)
{
	/*
	slope collisions we check the players
	bottom boundry agaisnt blocks that
	are slopes and offset the y-axis using y = mx + c
	where m is the gradient and c is the start of the slope
	*/

	//get neccessary level info
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;
	int tile_id;

	potential_collider_s potential_collider;

	//slopes
	level_x = g_level_p->get_grid_x( player_boundry.top.x);
	level_y = g_level_p->get_grid_y( player_boundry.top.y + 1);

	potential_collider.id = level_x + (level_y * level_width); 
	potential_collider.start.x = level_x * tile_size;
	potential_collider.start.y = level_y * tile_size + m_origin;
	potential_collider.end.x = level_x * tile_size + tile_size;
	potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

	tile_id = g_level_p->get_tile( potential_collider.id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 3)
	{
			//check
			int slope_formula_index = tile_id - 17;

			float m = slope_formula[slope_formula_index].m;
			float c = slope_formula[slope_formula_index].c_ratio * tile_size;

			float x = player_boundry.top.x - potential_collider.end.x;
			float top = potential_collider.start.y;

			float eqr = m * x + c;
			float slope_pos = top - eqr;
			
			if(m_pos.y < slope_pos) 
			{
				m_pos.y = slope_pos;
				if(m_velocity_y < 0) m_velocity_y = 0;
			}
	}
}

void Actor::pool_collision(player_bb_s player_boundry)
{
	/*
	pool collisions we check the players
	bottom boundry agaisnt blocks that
	are pool and offset the y-axis using y = mx + c
	where m is the gradient and c is the start of the slope
	then the player dies, and anim takes place to make it 
	look like he sinks and melts away
	*/

	//get neccessary level info
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;
	int tile_id;

	potential_collider_s potential_collider;

	//slopes
	level_x = g_level_p->get_grid_x( player_boundry.bottom.x);
	level_y = g_level_p->get_grid_y( player_boundry.bottom.y - 1);

	potential_collider.id = level_x + (level_y * level_width); 
	potential_collider.start.x = level_x * tile_size;
	potential_collider.start.y = level_y * tile_size + m_origin;
	potential_collider.end.x = level_x * tile_size + tile_size;
	potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

	tile_id = g_level_p->get_tile( potential_collider.id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 4)
	{

		float m = 0;
		float c = 0.6875f * tile_size;

		float x = player_boundry.bottom.x - potential_collider.end.x;
		float base = potential_collider.end.y - m_size.y;

		float eqr = m * x + c;
		float slope_pos = base - eqr;
		
		if(m_pos.y > slope_pos) 
		{
			m_pos.y = slope_pos;
			m_velocity_y = 0;
			m_jumps = 0;
			
			if(!m_dead)
			{
				m_lives--;

				m_velocity_x = 0;
				m_velocity_y = 0;

				m_dead = true;

				m_anim_state = ANIM_STATE_DEATH;

				m_landing = false;
				m_in_air = false;
				m_jumping = false;

				m_current_frame = 0;

				g_sound_mgr_p->play(g_sound_id.death,25);
			}
		}
	}

}

void Actor::set_pos(vector2 position)
{
	m_pos = position;
}


void Actor::gate_collision( player_bb_s player_boundry )
{
	int number_of_gates = g_level_p->number_of_gates();

	for(int i = 0; i < number_of_gates; i++)
	{
		vector2 gatePos;
		vector2 gateSize;

		g_level_p->obtainGates(&gatePos,&gateSize,i);

		if(player_boundry.back.x > gatePos.x && player_boundry.back.x < gatePos.x + gateSize.x
			&& player_boundry.back.y > gatePos.y && player_boundry.back.y < gatePos.y + gateSize.y)
		{
			m_pos.x = gatePos.x + gateSize.x;
		}

		if(player_boundry.front.x > gatePos.x && player_boundry.front.x < gatePos.x + gateSize.x
			&& player_boundry.front.y > gatePos.y && player_boundry.front.y < gatePos.y + gateSize.y)
		{
			m_pos.x = gatePos.x - m_size.x;
		}

		if(player_boundry.bottom.x > gatePos.x && player_boundry.bottom.x < gatePos.x + gateSize.x
			&& player_boundry.bottom.y > gatePos.y && player_boundry.bottom.y < gatePos.y + gateSize.y)
		{
			m_pos.y = gatePos.y - m_size.y;
			m_velocity_y = 0;
			m_jumps = 0;

			land();
		}

		if(player_boundry.top.x > gatePos.x && player_boundry.top.x < gatePos.x + gateSize.x
			&& player_boundry.top.y > gatePos.y && player_boundry.top.y < gatePos.y + gateSize.y)
		{
			m_pos.y = gatePos.y + gateSize.y;
		}
	}
}

void Actor::check_for_ledge( player_bb_s player_boundry )
{
	int level_width = g_level_p->get_map_width();

	float tile_size_check = g_level_p->get_tile_size() * 0.5f;
	float tile_size_check_x = tile_size_check;

	if(m_dir) tile_size_check_x *= -1;

	int level_x = g_level_p->get_grid_x( player_boundry.bottom.x + tile_size_check_x);
	int level_y = g_level_p->get_grid_y( player_boundry.bottom.y + tile_size_check * 2);

	int id = level_x + (level_y * level_width);

	int tile_id = g_level_p->get_tile( id );

	if( g_level_p->hit_collidable_tile( tile_id ) == 0)
	{
		m_dir = !m_dir;
	}

}

void Actor::check_control_panels( player_bb_s player_boundry )
{
	int number_of_panels = g_level_p->number_of_control_panels();

	for(int i = 0; i < number_of_panels; i++)
	{
		vector2 panelPos;
		vector2 panelSize;

		g_level_p->obtainControlPanels(&panelPos,&panelSize,i);

		bool in = false;

		if(player_boundry.back.x > panelPos.x && player_boundry.back.x < panelPos.x + (panelSize.x)
			&& player_boundry.back.y > panelPos.y && player_boundry.back.y < panelPos.y + (panelSize.y))
		{
			in = true;
		}

		if(player_boundry.front.x > panelPos.x && player_boundry.front.x < panelPos.x + (panelSize.x)
			&& player_boundry.front.y > panelPos.y && player_boundry.front.y < panelPos.y + (panelSize.y))
		{
			in = true;
		}

		if(player_boundry.bottom.x > panelPos.x && player_boundry.bottom.x < panelPos.x + (panelSize.x)
			&& player_boundry.bottom.y > panelPos.y && player_boundry.bottom.y < panelPos.y + (panelSize.y))
		{
			in = true;
		}

		if(player_boundry.top.x > panelPos.x && player_boundry.top.x < panelPos.x + (panelSize.x)
			&& player_boundry.top.y > panelPos.y && player_boundry.top.y < panelPos.y + (panelSize.y))
		{
			in = true;	
		}

		if(in)
		{
			m_display_tooltip = true;

			int type = g_level_p->useControlPanel(i,false);

			if(type == SAVE_POS)
			{

				float middle = panelPos.x + (panelSize.x / 2);

				if(!m_jumping)
				{
					if(player_boundry.top.x > middle - 8 && player_boundry.top.x < middle + 8 && player_boundry.bottom.y > panelPos.y + panelSize.y / 2)
					{
						m_pos.y = panelPos.y + 1;
						
						m_velocity_y = 0;
						m_jumps = 0;
						land();
					}
				}
			}
		}

		
		if((g_gamepad[m_control_scheme].state.Gamepad.wButtons & XINPUT_GAMEPAD_X && !m_pressed_interact) ||
			(key_hit(g_player_keymap[m_control_scheme].interact)))
		{
			if(in)
			{
				int type = g_level_p->useControlPanel(i,true);

				if(type == SAVE_POS)
				{
					save_game();
					g_sound_mgr_p->play(g_sound_id.save,25);
					
				}

				m_pressed_interact = true;
			}	
		}
	}

	if(g_gamepad[m_control_scheme].state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		return;
	}

	m_pressed_interact = false;
}

void Actor::actor_collision()
{
	int number_of_smartballs = g_level_p->number_of_smartballs();

	vector2 min_extent;
	vector2 max_extent;

	min_extent.x = m_pos.x - (m_size.x / 2);
	max_extent.x = m_pos.x + (m_size.x / 2);

	min_extent.y = m_pos.y - (m_size.y / 2);
	max_extent.y = m_pos.y + (m_size.y / 2);

	for(int i = 0; i < number_of_smartballs; i++)
	{
		vector2 pos;
		vector2 size;
		g_level_p->obtainSmartBalls(&pos,&size,i);

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
			m_dead = true;

			m_jumping = false;
			m_in_air = false;
			m_landing = false;

			m_current_frame = 0;

			m_anim_state = ANIM_STATE_DEATH;

			m_velocity_x = 0;

			m_lives--;

			g_sound_mgr_p->play(g_sound_id.death,25);

			return;
		}
	}
}
