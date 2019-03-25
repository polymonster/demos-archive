#include "particles.h"
#include "dappinput.h"
#include "d3dapp.h"


Particle::Particle(vector2 position, char *name_p) :
	Quad(PATH_SPRITES,name_p,position.x,position.y,6,6,0xffffffff)
{
	set_priority( RENDER_NEAR_FRONT );
	m_active = false;
	m_life = 0;
	m_absolute_size = m_size;

	explosion = new Quad(PATH_SPRITES,"explosion",position.x, position.y,11,11,0xffffffff);
	explosion->m_hide = true;
}

Particle::~Particle()
{

}

void Particle::execute( void )
{
	if(m_active)
	{

		Quad::execute();

		m_life -= g_scene_p->m_game_time;

		vector2 new_size;

		if(m_type == 0)
		{
			float scale_factor = (1 - m_life) * 8.0f;

			new_size.x = m_absolute_size.x + scale_factor;
			new_size.y = m_absolute_size.y + scale_factor;

			vector2 new_pos;

			new_pos.x = m_absolute_pos.x - (scale_factor * 0.5f);
			new_pos.y = m_absolute_pos.y - (scale_factor * 0.5f);

			m_pos = new_pos;
			m_size = new_size;

			int alpha = (255 * m_life) * 0.6;
			set_alpha( alpha );


		}
		else
		{
			m_size.x += g_scene_p->m_game_time * 10;
			m_size.y += g_scene_p->m_game_time * 10;

			m_pos.x = m_absolute_pos.x - (m_size.x * 0.5f);
			m_pos.y = m_absolute_pos.y - (m_size.y * 0.5f); 

			int alpha = (255 * (m_life / 1)) * 0.6;
			set_alpha( alpha );

			alpha = (255 * (m_life / 1)) * 0.6;
			if(alpha <= 0) alpha = 0;
			explosion->set_alpha( alpha );

			m_rotation.x += 10 * g_scene_p->m_game_time;
			m_rotation.y -= 10 * g_scene_p->m_game_time;
		}

	}
	else
	{
		Quad::m_hide = true;
	}

	if(m_life <= 0) 
	{
		m_size = m_absolute_size;
		m_active = false;
	}
}


void Particle::render( void )
{
	if(m_active) 
	{
		Quad::render();

		if(m_type == 1)
		{
			explosion->render();
		}
	}
	else
	{
		explosion->m_hide = true;
	}
}

void Particle::spawn(vector2 position)
{
	m_pos.x = position.x - (m_size.x * 0.5f);
	m_pos.y = position.y - (m_size.y * 0.5f);

	m_active = true;
	Quad::m_hide = false;
	m_life = 1;

	m_absolute_pos = m_pos;
	m_absolute_size = m_size;

	m_type = 0;

}

void Particle::spawn(vector2 position, int type)
{
	m_pos.x = position.x;
	m_pos.y = position.y;

	m_active = true;
	Quad::m_hide = false;
	m_life = 1;

	m_absolute_pos = m_pos;
	m_absolute_size = m_size;

	if(type == 1)
	{
		m_size.x *= 2;
		m_size.y *= 2;
		m_life = 1;

		m_pos.x = position.x - (m_size.x * 0.5f);
		m_pos.y = position.y - (m_size.y * 0.5f);

		explosion->m_hide = false;
		vector2 sizer = explosion->get_size();
		explosion->set_pos(m_pos.x,m_pos.y);
	}

	m_type = type;
}

