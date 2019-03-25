#include "blocker.h"
#include "dappinput.h"
#include "2dscene.h"

int dir = 1;
int dir2 = 1;


Blocker::Blocker(vector2 position, vector2 dimensions, int dir, int type, int cp, char *name_p) :
Quad(PATH_SPRITES,name_p,position.x,position.y,dimensions.x,dimensions.y,0xffffffff)
{
	set_priority( RENDER_MID_BACK );
	m_type = type;
	m_open = false;
	m_cp_id = cp;

	switch(m_type)
	{
		case BRIDGE :

			m_pos.y += 3;
			m_to_position = m_pos;
			m_closed_position = m_pos;
			m_to_position.x = position.x - (dimensions.x * dir);
			set_texcoords( 0.0f, 1.0f, 0.0f, 10.0f/16.0f );

		break;

		case DOOR :

			m_pos.x += 3;
			m_to_position = m_pos;
			m_closed_position = m_pos;
			m_to_position.y = position.y - (dimensions.y * dir);
			set_texcoords( 0.0f, 10.0f/16.0f, 0.0f, 1.0f );

		break;
	}

	m_inmotion = false;
	m_channelID = -1;
}

Blocker::~Blocker()
{
	g_sound_mgr_p->stopLoop(S_GATE_MOTION);
}

void Blocker::execute( void )
{
	m_inmotion = false;

	if(m_open)
	{
				if(m_pos.x != m_to_position.x)
				{
					if(m_to_position.x < m_pos.x)
					{
						m_pos.x--;
					}
					else
					{
						m_pos.x++;
					}

					m_inmotion = true;
				}

				if(m_pos.y != m_to_position.y)
				{
					if(m_to_position.y < m_pos.y)
					{
						m_pos.y--;
					}
					else
					{
						m_pos.y++;
					}

					m_inmotion = true;
				}
	}
	else
	{
				if(m_pos.x != m_closed_position.x)
				{
					if(m_closed_position.x < m_pos.x)
					{
						m_pos.x--;
					}
					else
					{
						m_pos.x++;
					}

					m_inmotion = true;
				}

				if(m_pos.y != m_closed_position.y)
				{
					if(m_closed_position.y < m_pos.y)
					{
						m_pos.y--;
					}
					else
					{
						m_pos.y++;
					}

					m_inmotion = true;
				}

	}

	if(m_inmotion)
	{
		if(m_channelID == -1)
		{
			m_channelID = g_sound_mgr_p->playLoop(S_GATE_MOTION);
		}
	}
	else
	{
		if(m_channelID != -1)
		{
			g_sound_mgr_p->stopLoop(S_GATE_MOTION);
			g_sound_mgr_p->play(S_GATE_STOP,55);
			m_channelID = -1;
		}
	}
	
	Quad::execute();
}


void Blocker::render( void )
{
	Quad::render();
}

void Blocker::activate( void )
{
	if(m_open)
	{
		g_sound_mgr_p->play(S_GATE_CLOSE,25);
	}
	else
	{
		g_sound_mgr_p->play(S_GATE_OPEN,25);
	}

	m_inmotion = true;
	m_open = !m_open;
}