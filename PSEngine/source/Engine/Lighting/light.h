/*=========================================================*\
|	light.h - light class
|-----------------------------------------------------------|
|				Project :	PolySpoon Lighting
|				Coder	:	ADixon
|				Date	:	01/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/


#ifndef _LIGHT_H
#define _LIGHT_H

#include "window.h"
#include "helpers.h"

class EditableProperties
{
	public:

		EditableProperties()
			: m_selected(false)
		{};

		bool m_selected;
};

class LightParameters
{
	public:

		std::vector<Vector4f> position;

		std::vector<Vector3f> diffuse;
		std::vector<Vector3f> spot_direction;

		std::vector<float>	 area_of_effect;

		int visible_light_count;

		void clear();
};

class Light
{
	private:

		Vector3f m_ambient;
		Vector3f m_direction;
		Vector3f m_position;
		Vector3f m_diffuse;
		Vector3f m_specular;

		int m_light_type;
		int m_spot_exponent;
		int m_spot_cutoff;

		unsigned int m_light_id;

		SPHERE *widget;

	public:

		enum
		{
			POINT,
			SPOT,
			DIRECTIONAL

		}e_light_types;

		EditableProperties m_properties;

		Light(){};
		Light(Vector3f position, Vector3f ambient, Vector3f diffuse, Vector3f specular, int light_type);

		void enable(LightParameters *paramater_array);
		void update();

		void render();
		void controller_render();
		void picking_render(int id);
		void aux_render();
};

#endif