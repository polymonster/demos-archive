#include "projectile.h"
#include "dappinput.h"
#include "d3dapp.h"


Projectile::Projectile(vector2 position, vector2 direction, int type, char *name_p) :
	Quad(PATH_SPRITES,name_p,position.x,position.y,4,4,0xffffffff)
{
	m_direction_vector = direction;
	m_active = false;
	m_life = 0;
	m_type = type;
	m_turning_circle = 0.25f;
	m_rotation = 0;
	m_particle_timer = 0;
	m_emmision_rate = 0.07f;
	m_speed = 1.0f;
	set_priority( RENDER_NEAR_FRONT );
}

Projectile::~Projectile()
{

}

void Projectile::execute( void )
{
	if(m_active)
	{
		if(m_type == 0)
		{
			m_pos.x += (m_direction_vector.x * m_speed);
			m_pos.y += (m_direction_vector.y * m_speed);
		}
		else if(m_type == 1)
		{
			vector2 new_direction_vector;
			new_direction_vector.y = m_player_position.y - m_pos.y;
			new_direction_vector.x = m_player_position.x - m_pos.x;

			//normalise it
			float magnitude = (float) sqrt((new_direction_vector.x * new_direction_vector.x) + (new_direction_vector.y * new_direction_vector.y));
			new_direction_vector.x /= magnitude;
			new_direction_vector.y /= magnitude;

			//interpolate the rotation
			if(m_direction_vector.x != new_direction_vector.x)
			{
				if(m_direction_vector.x  < new_direction_vector.x)
				{
					m_direction_vector.x += 0.01f;
				}
				else if(m_direction_vector.x > new_direction_vector.x)
				{
					m_direction_vector.x -= 0.01f;
				}
			}

			if(m_direction_vector.y != new_direction_vector.y)
			{
				if(m_direction_vector.y  < new_direction_vector.y)
				{
					m_direction_vector.y += 0.01f;
				}
				else if(m_direction_vector.y > new_direction_vector.y)
				{
					m_direction_vector.y -= 0.01f;
				}
			}
	
			m_pos.x += (m_direction_vector.x * m_speed);
			m_pos.y += (m_direction_vector.y * m_speed);

			m_rotation = atan2(-m_direction_vector.x,m_direction_vector.y);

			vector2 middle_pos = m_pos;

			//strafe it to the center on the x
			m_particle_spawn_pos.x = middle_pos.x + sin(m_rotation) * 3;
			m_particle_spawn_pos.y = middle_pos.y - cos(m_rotation) * 3;

		}
		else if(m_type == 2)
		{
			if(m_move)
			{
				if(m_direction_vector.x > 0)
				{
					m_size.x += m_direction_vector.x * m_speed;
					m_pos.x = m_root_pos.x + (m_size.x * 0.5f);
				}
				else
				{
					m_size.x -= m_direction_vector.x * m_speed;
					m_pos.x = m_root_pos.x - (m_size.x * 0.5f);

					if(m_direction_vector.y > 0) m_pos.x += 3;
					else m_pos.x += 2;
				}

				if(m_direction_vector.y > 0)
				{
					m_size.y += m_direction_vector.y * m_speed;
					m_pos.y = m_root_pos.y + (m_size.y * 0.5f);
				}
				else
				{
					m_size.y -= m_direction_vector.y * m_speed;
					m_pos.y = m_root_pos.y - (m_size.y * 0.5f);

					if(m_direction_vector.x > 0) m_pos.y += 3;
					else m_pos.y += 4;
				}
			}
		}

		Quad::execute();

		m_life -= g_scene_p->m_game_time;
		m_particle_timer -= g_scene_p->m_game_time;

		checkCollisions();
	}
	else
	{
		Quad::m_hide = true;
	}

	if(m_life <= 0) m_active = false;
}


void Projectile::render( void )
{
	if(m_active) 
	{
		rotate( m_rotation * 180 / PI );

		Quad::render();
	}
}

void Projectile::spawn(vector2 position, vector2 direction)
{
	m_size.x = 4;
	m_size.y = 4;


	m_pos = position;
	m_direction_vector = direction;
	m_active = true;
	Quad::m_hide = false;
	m_life = 5;
	m_particle_timer = m_emmision_rate;
	m_type = 0;
}

void Projectile::spawn(vector2 position, vector2 direction, int type)
{
	m_pos = position;
	m_direction_vector = direction;
	m_active = true;
	Quad::m_hide = false;
	m_life = 5;
	m_particle_timer = m_emmision_rate;
	m_type = type;

	m_size.x = 4;
	m_size.y = 4;
	m_default_ablend = true;
	m_speed = 1.0f;

	if(m_type == 1) 
	{
		m_size.y = 8;
	}
	else if(m_type == 2)
	{
		m_default_ablend = false;
		m_size.x = 3;
		m_size.y = 3;
		m_move = true;

		m_rotation = 0;
		m_root_pos = position;
		m_speed = 10.0f;
	}

	set_texture( g_level_p->m_projectile_tex[m_type] );
}

void Projectile::checkCollisions( void )
{
	//get necessary level info we need
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;
	int tile_id;

	potential_collider_s potential_collider;

	vector2 originalPos = m_pos;

	m_pos.x += m_size.x * 0.5f;
	m_pos.y += m_size.y * 0.5f;

	//m_pos.x += 2 * sin(m_rotation * 180 / PI);
	//m_pos.y -= 2 * cos(m_rotation * 180 / PI);

	if(m_type == 2)
	{
		if(m_direction_vector.x > 0)
			m_pos.x = m_root_pos.x + m_size.x;
		else if(m_direction_vector.x < 0)
			m_pos.x = m_root_pos.x - m_size.x; 
		else
			m_pos.x = m_root_pos.x + (m_size.x * 0.5f);

		if(m_direction_vector.y > 0)
			m_pos.y = m_root_pos.y + m_size.y;
		else if(m_direction_vector.y < 0)
			m_pos.y = m_root_pos.y - m_size.y + 4;
		else
			m_pos.y = m_root_pos.y + (m_size.y * 0.5f);
		
	}

	bool explode = false;

	vector2 collisionPos = m_pos;

	if(m_pos.x > 0 && m_pos.y > 0)
	{
		int m_origin = ( !g_editor_mode ? (int)( ( (float)g_window_h - 256.0f ) * 0.5f ) : 0 );

		level_x = g_level_p->get_grid_x( m_pos.x );
		level_y = g_level_p->get_grid_y( m_pos.y );

		potential_collider_s potential_collider;

		potential_collider.id = level_x + (level_y * level_width); 
		potential_collider.start.x = level_x * tile_size;
		potential_collider.start.y = level_y * tile_size + m_origin;
		potential_collider.end.x = level_x * tile_size + tile_size;
		potential_collider.end.y = level_y * tile_size + m_origin + tile_size;

		int id = level_x + (level_y * level_width); 
		
		tile_id = g_level_p->get_tile( id );

		if( g_level_p->hit_collidable_tile( tile_id ) == 1)
		{
			m_active = false;
			explode = true;

			//TO get super tight lazer collisions
			if(m_type == 2)
			{
				//going left to right
				if(m_direction_vector.x > 0)
				{
					float diff = potential_collider.start.x - m_pos.x;
					float offset = diff * 0.5f;

					m_size.x += offset;
					originalPos.x += (offset * 0.5f);
				}

				//going top to bottom
				if(m_direction_vector.y > 0)
				{
					float diff = (potential_collider.start.y - m_pos.y) + 0.5;
					float offset = diff * 0.5f;

					m_size.y += offset;
					originalPos.y += (offset * 0.5f);
				}

				//right to left
				if(m_direction_vector.x < 0)
				{
					float diff = (potential_collider.end.x - m_pos.x) - 1;
					float offset = diff * 0.5f;

					m_size.x -= offset;
					originalPos.x += (offset * 0.5f);
				}

				//bottom to top
				if(m_direction_vector.y < 0)
				{
					float diff = (potential_collider.end.y - m_pos.y) + 1;
					float offset = diff * 0.5f;

					m_size.y -= offset;
					originalPos.y += (offset * 0.5f);
				}
			}
		}

		if( g_level_p->hit_collidable_tile( tile_id ) == 2)
		{
			int slope_formula_index = tile_id - 17;

			float m = slope_formula[slope_formula_index].m;
			float c = slope_formula[slope_formula_index].c_ratio * tile_size;

			float x = m_pos.x - potential_collider.end.x;
			float base = potential_collider.end.y;

			float eqr = m * x + c;
			float slope_pos = base - eqr;
			
			if(m_pos.y > slope_pos) 
			{
				m_active = false;
				explode = true;
				collisionPos.y = slope_pos;
			}
		}

		if( g_level_p->hit_collidable_tile( tile_id ) == 3)
		{
			//check
			int slope_formula_index = tile_id - 17;

			float m = slope_formula[slope_formula_index].m;
			float c = slope_formula[slope_formula_index].c_ratio * tile_size;

			float x = m_pos.x - potential_collider.end.x;
			float top = potential_collider.start.y;

			float eqr = m * x + c;
			float slope_pos = top - eqr;
			
			if(m_pos.y < slope_pos) 
			{
				m_active = false;
				explode = true;
				collisionPos.y = slope_pos;
			}
		}
	}

	int number_of_gates = g_level_p->number_of_gates();

	for(int i = 0; i < number_of_gates; i++)
	{
		vector2 gatePos;
		vector2 gateSize;

		g_level_p->obtainGates(&gatePos,&gateSize,i);

		if(m_pos.x > gatePos.x && m_pos.x < gatePos.x + gateSize.x
			&& m_pos.y > gatePos.y && m_pos.y < gatePos.y + gateSize.y)
		{
			m_active = false;
			explode = true;

						//TO get super tight lazer collisions
			if(m_type == 2)
			{
				//going left to right
				if(m_direction_vector.x > 0)
				{
					float diff = gatePos.x - m_pos.x;
					float offset = diff * 0.5f;

					m_size.x += offset;
					originalPos.x += offset * 0.5f;
				}

				//going top to bottom
				if(m_direction_vector.y > 0)
				{
					float diff = gatePos.y - m_pos.y;
					float offset = diff * 0.5f;

					m_size.y += offset;
					originalPos.y += offset * 0.5f;
				}

				//right to left
				if(m_direction_vector.x < 0)
				{
					float diff = gatePos.x + gateSize.x - m_pos.x;
					float offset = diff * 0.5f;

					m_size.x -= offset;
					originalPos.x += offset * 0.5f;
				}

				//bottom to top
				if(m_direction_vector.y < 0)
				{
					float diff = gatePos.y + gateSize.y - m_pos.y;
					float offset = diff * 0.5f;

					m_size.y -= offset;
					originalPos.y += offset * 0.5f;
				}
			}
		}

	}

	m_pos = originalPos;

	if((!m_active) && (m_type == 2))
	{
		m_active = true;
		explode = false;
		m_move = false;

		if(m_life <= 0)
		{
			m_active = true;
		}
	}

	if(explode) 
	{
		g_sound_mgr_p->play(S_EXPLOSION,0);
		g_level_p->spawn_particle(m_pos,1);
	}
}

void Projectile::notify_player_position(vector2 position)
{
	m_player_position = position;
}

bool Projectile::emmit_particle( void )
{
	if(m_active)
	{
		if((m_type == 1) && (m_particle_timer <= 0))
		{
			m_particle_timer = m_emmision_rate;
			return true;
		}
	}

	return false;
}