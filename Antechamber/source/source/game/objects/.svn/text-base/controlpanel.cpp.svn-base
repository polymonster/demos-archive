#include "controlpanel.h"
#include "dappinput.h"
#include "d3dapp.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

ControlPanel::ControlPanel(vector2 position, int gate_id, char *name_p, int type) :
	Quad(PATH_SPRITES,name_p,position.x-4,position.y-4,16,16,0xffffffff)
{
	set_priority( RENDER_NEAR_FRONT );
	m_tex2 = load_d3d_texture( "images/sprites/cp1.tga" );
	m_gate_id = gate_id;
	m_intercom = false;
	m_playing = false;
	m_type = type;

	if( m_type == 2 )
	{
		m_size.x = 24;
		m_size.y = 24;
		set_texcoords( 0.0f, 0.75f, 0.0f, 0.75f );
		m_pos.y -= 12;
	}

}

ControlPanel::~ControlPanel()
{
	g_sound_mgr_p->stopStream(stream);
}

void ControlPanel::execute( void )
{
	Quad::execute();
}


void ControlPanel::render( void )
{
	if(m_playing)
	{
		m_dlg_len -= g_scene_p->m_game_time * 1000;

		if(m_dlg_len <= 0)
		{
			g_sound_mgr_p->stopStream(stream);
			m_playing = false;
		}

		int yoff = 0;

		for(int i = 0; i < m_dlg_lines; i++)
		{
			float size = g_mini_font_p->get_text_width(m_dialogue_lines[i]);
			g_mini_font_p->print_2d_text( m_dialogue_lines[i], 320 - (size*0.5f), 390 + yoff, 1.0f, 0xffe0e0e0 );
			yoff+=8;
		}
	}

	Quad::render();
}

bool ControlPanel::activate( int *gate_id )
{
	*gate_id = m_gate_id;

	switch(m_type)
	{
		case GATE_CONTROL:

			tga_texture_s temp = m_tex;
			m_tex.texture = m_tex2.texture;
			m_tex2.texture = temp.texture;

		break;
	}

	return true;
}

void ControlPanel::assign_sound(char *file)
{
	strcpy(m_text_file,file);
	strcpy(m_sound_file,file);
	m_intercom = true;
	m_playing = false;
	stream = NULL;

	int len = strlen(m_text_file);
	m_text_file[len - 1] = 't';
	m_text_file[len - 2] = 'x';
	m_text_file[len - 3] = 't';
}

bool ControlPanel::play_dialogue()
{
	if(m_type == INTERCOM)
	{
		if(!m_playing)
		{
			g_sound_mgr_p->stopStream(stream);

			m_dlg_len = g_sound_mgr_p->loadAndPlay(m_sound_file,g_dialog_volume,&stream);
			
			m_playing = true;

			std::ifstream fin;

			fin.open(m_text_file);

			m_dlg_lines = 0;

			while(!fin.eof())
			{
				fin.getline(m_dialogue_lines[m_dlg_lines++],128);
			}

			fin.close();

			return true;
		}
		else
		{
			g_sound_mgr_p->stopStream(stream);
			
			m_playing = false;

			return false;
		}
	}

	return true;
}

void ControlPanel::stop()
{
	//wsssg_sound_mgr_p->stopStream(stream);

	m_playing = false;
}