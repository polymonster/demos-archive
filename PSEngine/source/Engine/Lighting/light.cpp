/*=========================================================*\
|	light.cpp - light class
|-----------------------------------------------------------|
|				Project :	PolySpoon Lighting
|				Coder	:	ADixon
|				Date	:	01/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/


#include "light.h"

Light::Light(Vector3f position, Vector3f ambient, Vector3f diffuse, Vector3f specular, int light_type)
{
	//position and colour
	m_position = position;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;

	//type
	m_light_type = light_type;

	//editor stuff
	widget = new SPHERE(m_position, 7.0f);
}


void Light::enable(LightParameters *paramater_array)
{
	//set the lights parameters
	Vector4f position_type_packed = Vector4f(m_position, m_light_type);

	m_direction = Vector3f(0.2,0.8,1.0);

	paramater_array->position.push_back(position_type_packed);
	paramater_array->diffuse.push_back(m_diffuse);
	paramater_array->spot_direction.push_back(m_direction);
	paramater_array->area_of_effect.push_back(400.0f);

	paramater_array->visible_light_count++;
}

void Light::update()
{
	if(m_properties.m_selected)
	{
		widget->update();
		m_position = widget->m_position;
	}
}

void Light::render()
{
	//render the light in a nice colour to represent its handle
	glColor4f(m_diffuse.x * 2.0f,m_diffuse.y * 2.0f,m_diffuse.z * 2.0f,0.5f);
	widget->render();
}

void Light::picking_render(int id)
{
	//render as unique colour for picking
	Vector3f colour_id = pshelpers::get_colour_from_id(id);
	
	colour_id.x /= 255.0f;
	colour_id.y /= 255.0f;
	colour_id.z /= 255.0f;

	glColor4f(colour_id.x,colour_id.y,colour_id.z,1.0f);

	widget->render();
}

void Light::controller_render()
{
	if(m_properties.m_selected)
	{
		//render widgets into back buffer for picking
		widget->back_buffer_render();
	}
}

void Light::aux_render()
{
	if(m_properties.m_selected)
	{
		//render widgets into back buffer for picking
		widget->aux_render();
	}
}