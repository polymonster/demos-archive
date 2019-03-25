#include "turret.h"
#include "dappinput.h"
#include "d3dapp.h"
#include "time.h"

#include "level.h"


Turret::Turret(vector2 position, float fire_rate, int type, int projectile_type, int rotation_range,  char *name_p) :
	Quad(PATH_SPRITES,"door",position.x,position.y,3,8,0xffffffff)
{
	set_priority( RENDER_NEAR_FRONT );
	vector2 baseOffsets;

	//srand(time(NULL));

	m_fire_rate = fire_rate;
	m_fire_timer =  1 + (rand()%5);
	m_rotation = 0;
	m_projType = projectile_type;
	m_type = type;

	if(m_projType == 2) 
	{
		m_fire_rate = 10;
		rotation_range = 0;
	}

	m_inverse = false;

	char sprite[32];

	strcpy(sprite,name_p);

	switch(type)
	{
		case 0:

		m_origin_rotation = 0;
		baseOffsets.x = 8;//6;
		baseOffsets.y = 8;//0;

		break;

		case 1:

		m_origin_rotation = 90;

		baseOffsets.x = 8;//8;
		baseOffsets.y = 8;//5;

		strcat(sprite,"1");

		break;

		case 2:

		m_origin_rotation = 180;

		baseOffsets.x = 8;//5;
		baseOffsets.y = 8;//8;

		m_inverse = true;

		strcat(sprite,"2");

		break;

		case 3:

		m_origin_rotation = 270;

		baseOffsets.x = 8;//-1;
		baseOffsets.y = 8;//4;

		m_inverse = true;

		strcat(sprite,"3");

		break;
	}

	m_rotation_lock_max = m_origin_rotation + rotation_range;
	m_rotation_lock_min = m_origin_rotation - rotation_range;
	m_rotation = m_origin_rotation;

	m_pos.x += baseOffsets.x;
	m_pos.y += baseOffsets.y;

	m_cannon_quad = new Quad(PATH_SPRITES,sprite,position.x, position.y,16,16,0xffffffff);
	m_cannon_quad->set_priority( get_priority() - 0.1f );

	m_sound_channel = -1;
}

Turret::~Turret()
{
	if(m_sound_channel != -1)
	{
		g_sound_mgr_p->stopLoop(g_sound_id.beam);
		m_sound_channel = -1;
	}

	int handle = m_cannon_quad->get_handle();
	g_object_list_p->remove( handle );
}

void Turret::execute( void )
{
	if(m_fire_timer > 0)
	{
		m_fire_timer -= g_scene_p->m_game_time;
	}

	if(m_cooldown_timer > 0)
	{	
		m_cooldown_timer -= g_scene_p->m_game_time;
	}
	else
	{
		if(m_sound_channel != -1)
		{
			g_sound_mgr_p->stopLoop(g_sound_id.beam);
			m_sound_channel = -1;
		}
	}

	Quad::execute();
}


void Turret::render( void )
{
	translate( 0,-3 );

	rotate( m_rotation );

	Quad::render();

	m_cannon_quad->render();
}

int Turret::shoot( vector2 *pos, vector2 *dir )
{
	if(m_fire_timer <= 0)
	{
		*pos = m_pos;
		pos->y -= 1;
		*dir = m_aim_vector;

		if(m_projType == 2) 
		{
			m_cooldown_timer = 4;
			m_sound_channel = g_sound_mgr_p->playLoop(g_sound_id.beam,15);
		}
		else if(m_projType == 1) g_sound_mgr_p->play(g_sound_id.missile);
		else g_sound_mgr_p->play(g_sound_id.bullet);

		m_fire_timer = m_fire_rate;
		return m_projType;
	}
	else
	{
		return -1;
	}
}

void Turret::notify_player_position(vector2 position)
{
	//get the aim vector
	m_aim_vector.x = (position.x - m_pos.x);
	m_aim_vector.y = (position.y - m_pos.y);

	//normalise it
	float magnitude = (float) sqrt((m_aim_vector.x * m_aim_vector.x) + (m_aim_vector.y * m_aim_vector.y));
	m_aim_vector.x /= magnitude;
	m_aim_vector.y /= magnitude;

	//calculate the rotation the turrent needs to be based on the desired aim
	if(m_inverse) 
	{
		m_new_rotation = atan2(-m_aim_vector.x,m_aim_vector.y);
		m_new_rotation *= 180.0f / PI;
		m_new_rotation += 180.0f;
	}
	else 
	{
		m_new_rotation = atan2(m_aim_vector.x,-m_aim_vector.y);
		m_new_rotation *= 180 / PI;
	}

	//clamp the rotations to certain ranges depending on turret types
	if(m_new_rotation > m_rotation_lock_max) m_new_rotation = m_rotation_lock_max;
	else if(m_new_rotation < m_rotation_lock_min) m_new_rotation = m_rotation_lock_min;

	//interpolate the rotation
	if(m_rotation != m_new_rotation)
	{
		if(m_rotation < m_new_rotation)
		{
			m_rotation++;
		}
		else if(m_rotation > m_new_rotation)
		{
			m_rotation--;
		}
	}

	//finally now recaluclate the aim vector based on the turrets
	//actual rotation (not its desired one)
	float radRot = m_rotation * PI / 180;
	m_aim_vector.x = sin(radRot);
	m_aim_vector.y = -cos(radRot);

	if(m_projType == 2)
	{
		switch(m_type)
		{

		case 0:
			
			m_aim_vector.x = 0;
			m_aim_vector.y = -1;

		break;

		case 1:

			m_aim_vector.x = 1;
			m_aim_vector.y = 0;

		break;

		case 2:

			m_aim_vector.x = 0;
			m_aim_vector.y = 1;

		break;

		case 3:

			m_aim_vector.x = -1;
			m_aim_vector.y = 0;

		break;

		}
	}
}