/*=========================================================*\
|	lighting_manager.cpp - light class
|-----------------------------------------------------------|
|				Project :	PolySpoon Lighting
|				Coder	:	ADixon
|				Date	:	01/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#include "light_manager.h"
#include "camera.h"
#include "shader_manager.h"
#include "polyspoon_math.h"
#include "editor_states.h"

LightManager g_lighting_manager;

int g_number_of_lights = 0;

const Vector3f random_diffuse[] =
{
	Vector3f(1,0,1),
	Vector3f(0,1,1),
	Vector3f(1,1,0),
	Vector3f(0,1,1),

	Vector3f(0.8f,0.5f,0.0f),
	Vector3f(0.5f,0.0f,1.0f),
	Vector3f(0.2f,1.0f,0.0f),
	Vector3f(1.0f,0.2f,0.0f)
};

const Vector3f random_ambient[] =
{
	Vector3f(0.1f, 0.1f, 0.1f)
};

const Vector3f random_specular[] =
{
	Vector3f(0.9,0.6,0.9),
	Vector3f(0.6,0.9,0.9),
	Vector3f(0.9,0.9,0.6),
	Vector3f(0.6,0.9,0.9),

	Vector3f(0.9,0.5,0.0),
	Vector3f(0.5,0.6,9.0),
	Vector3f(0.8,0.9,0.6),
	Vector3f(0.9,0.8,0.6)
};

const int random_count = 8;

void LightParameters::clear()
{
	position.clear();
	diffuse.clear();
	area_of_effect.clear();
	spot_direction.clear();
	visible_light_count = 0;
}

void LightManager::enable_lights()
{
	m_light_parameters.clear();

	if(g_number_of_lights < 0) g_number_of_lights = 0;
	if(g_number_of_lights > m_lights.size()) g_number_of_lights = m_lights.size();

	m_light_parameters.visible_light_count = 0;

	for(int i = 0; i < m_lights.size(); i++)
	{
		m_lights.at(i)->enable(&m_light_parameters);
	}

	ShaderManager::instance()->set_uniform(Shaders::deferred,"number_of_lights",(int)m_lights.size());
	ShaderManager::instance()->set_uniform_array(Shaders::deferred,"light_pos", m_light_parameters.position);
	ShaderManager::instance()->set_uniform_array(Shaders::deferred,"light_diffuse", m_light_parameters.diffuse);
	ShaderManager::instance()->set_uniform_array(Shaders::deferred,"spot_direction", m_light_parameters.spot_direction);
	ShaderManager::instance()->set_uniform_array(Shaders::deferred,"area_of_effect", m_light_parameters.area_of_effect);
}

void LightManager::add_light( Vector3f position, Vector3f ambient, Vector3f diffuse, Vector3f specular,  int type)
{
	m_lights.push_back(new Light(position, ambient, diffuse, specular, type));
}

void LightManager::add_light(Vector3f position, int type)
{
	int random = rand()%random_count;

	Vector3f ambient = random_ambient[0];
	Vector3f diffuse = random_diffuse[random];
	Vector3f specular = random_specular[random];
	
	m_lights.push_back(new Light(position, ambient, diffuse, specular, type));
}

void LightManager::update()
{
	for(int i = 0; i < m_lights.size(); i++) 
	{
		m_lights.at(i)->update();
	}

	if(g_input_handler.m_key_pressed[VK_M]) g_number_of_lights++;
	if(g_input_handler.m_key_pressed[VK_N]) g_number_of_lights--;
}

void LightManager::back_buffer_render()
{
	//light handle
	for(int i = 0; i < m_lights.size(); i++) 
	{
		m_lights.at(i)->picking_render(i);
	}

	//now pick to see whats selected
	if(g_input_handler.m_mouse_button_pressed[LEFT])
	{
		Vector3f colour_id = g_input_handler.colour_picking();
		unsigned int id_number = pshelpers::get_id_from_colour(colour_id);

		if(id_number < m_lights.size())
		{
			//control will allow selection concatenation
			if(!g_input_handler.m_key_held[VK_CONTROL] && !g_input_handler.m_key_held[VK_SHIFT])
			{
				for(int i = 0; i < m_lights.size(); i++) 
				{
					m_lights.at(i)->m_properties.m_selected = false;
				}
			}

			//set the picked light to selected shift will unselect
			bool select = true; 
			if(g_input_handler.m_key_held[VK_SHIFT]) select = false;

			m_lights.at(id_number)->m_properties.m_selected = select;
		}
	}
	
	//clear that stuff for the next picking operation
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//widget
	for(int i = 0; i < m_lights.size(); i++) 
	{
		m_lights.at(i)->controller_render();
	}
}

void LightManager::aux_render()
{
	glPushMatrix();

		g_camera_p->set_position();

		for(int i = 0; i < m_lights.size(); i++) 
		{
			m_lights.at(i)->aux_render();
		}

	glPopMatrix();

}

void LightManager::render()
{
	if(g_editor_states.m_main_mode == g_editor_states.LIGHTING)
	{
		for(int i = 0; i < m_lights.size(); i++) 
		{
			m_lights.at(i)->render();
		}

	}
}