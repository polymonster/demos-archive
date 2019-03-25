#include "traps.h"
#include "dappinput.h"
#include "d3dapp.h"


Trap::Trap(vector2 position, int type, vector2 activate_range, vector2 speed, char *name_p) :
	Quad(PATH_SPRITES,name_p,position.x,position.y,10,10,0xffffffff)
{
	m_activate = false;
	m_type = type;
	m_original_pos = position;
	m_dir = 0;

	m_up_speed = speed.x;
	m_down_speed = speed.y;
	
	m_range = activate_range;

}

Trap::~Trap( void )
{

}

void Trap::execute( void )
{
	if(m_player_position.x > m_pos.x - (m_size.x * m_range.x) && m_player_position.x < m_pos.x + (m_size.x * (m_range.x + 1)) )
		//&& m_player_position.y > m_pos.y - (m_size.y * m_range.y) && m_player_position.y > m_pos.y + (m_size.y * (m_range.y + 1)))
	{
		m_activate = true;
	}

	if(m_dir == 0 && m_activate)
	{
		m_dir = 1 * m_type;
	}

	m_pos.y += m_dir * 5;

	if(m_type == 1)
	{
		if(m_dir == -1)
		{
			if(m_pos.y < m_original_pos.y)
			{
				m_pos.y = m_original_pos.y;
				m_activate = false;	
				m_dir = 0;
			}
		}
	}
	else if(m_type == -1)
	{
		if(m_dir == 1)
		{
			if(m_pos.y > m_original_pos.y)
			{
				m_pos.y = m_original_pos.y;
				m_activate = false;	
				m_dir = 0;
			}
		}
		
	}

	if(m_activate) check_collision();

	Quad::execute();
}

void Trap::render( void )
{
	Quad::render();
}

void Trap::notify_player_position(vector2 position)
{
	m_player_position = position;
}

void Trap::check_collision()
{
	//get necessary level info we need
	int tile_size = g_level_p->get_tile_size();
	int level_width = g_level_p->get_map_width();
	int level_x;
	int level_y;
	int tile_id;

	potential_collider_s potential_collider;

	vector2 test_pos = m_pos;

	if(m_type == 1) test_pos.y += m_size.y;

	if(test_pos.x > 0 && test_pos.y > 0)
	{
		int m_origin = ( !g_editor_mode ? (int)( ( (float)g_window_h - 256.0f ) * 0.5f ) : 0 );

		level_x = g_level_p->get_grid_x( test_pos.x );
		level_y = g_level_p->get_grid_y( test_pos.y );

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
			m_dir *= -1;
		}

		if( g_level_p->hit_collidable_tile( tile_id ) == 2)
		{
			int slope_formula_index = tile_id - 17;

			float m = slope_formula[slope_formula_index].m;
			float c = slope_formula[slope_formula_index].c_ratio * tile_size;

			float x = test_pos.x - potential_collider.end.x;
			float base = potential_collider.end.y - m_size.y;

			float eqr = m * x + c;
			float slope_pos = base - eqr;
			
			if(test_pos.y > slope_pos) 
			{
				m_dir *= -1;
			}
		}

		if( g_level_p->hit_collidable_tile( tile_id ) == 3)
		{
			//check
			int slope_formula_index = tile_id - 17;

			float m = slope_formula[slope_formula_index].m;
			float c = slope_formula[slope_formula_index].c_ratio * tile_size;

			float x = test_pos.x - potential_collider.end.x;
			float top = potential_collider.start.y;

			float eqr = m * x + c;
			float slope_pos = top - eqr;
			
			if(test_pos.y < slope_pos) 
			{
				m_dir *= -1;
			}
		}
	}

	int number_of_gates = g_level_p->number_of_gates();

	for(int i = 0; i < number_of_gates; i++)
	{
		vector2 gatePos;
		vector2 gateSize;

		g_level_p->obtainGates(&gatePos,&gateSize,i);

		if(test_pos.x > gatePos.x && test_pos.x < gatePos.x + gateSize.x
			&& test_pos.y > gatePos.y && test_pos
			.y < gatePos.y + gateSize.y)
		{
			m_dir *= -1;
		}

	}

}

