/*=========================================================*\
|	shadow_map.h - shadow map class
|-----------------------------------------------------------|
|				Project :	PolySpoon Lighting
|				Coder	:	ADixon
|				Date	:	01/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/


#ifndef _SHADOW_MAP_H
#define _SHADOW_MAP_H

#include "window.h"
#include "FBO.h"
#include "vector.h"
#include "matrix.h"

class ShadowMap
{
	public:
		
		ShadowMap(int shadow_map_type, int cascade = 0);
		~ShadowMap();

		void begin();
		void end();
		void bind_texture();
		void blur();
		void display();

		void render_volume();

		void bind_texture_deferred(int unit);

	private:

		void build_texture_matrix();
		void build_mipmaps();
		void set_position();

		Matrix4 m_view_matrix;
		Matrix4 m_projection_matrix;

		Vector2i m_dimensions;

		Vector3f m_caster_dimensions;
		Vector3f m_caster_position;
		Vector3f m_caster_direction;
		Vector3f m_caster_rotation;
		float m_caster_near;


		FBO *m_shadow_map_fbo[2];

		int m_map_type;
		int m_cascade;

};

#endif