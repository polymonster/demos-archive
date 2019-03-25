/*=========================================================*\
|	shadow_map.cpp - shadow map class
|-----------------------------------------------------------|
|				Project :	PolySpoon Lighting
|				Coder	:	ADixon
|				Date	:	01/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#include "shadow_map.h"
#include "helpers.h"
#include "shader_manager.h"
#include "texture_manager.h"
#include "camera.h"
#include <string>

float zoom = 600;

ShadowMap::ShadowMap(int shadow_map_type, int cascade)
{
	bool require_render_buffer = false;
	if(shadow_map_type == VARIANCE_SHADOW) require_render_buffer = true;

	m_map_type = shadow_map_type;

	switch(cascade)
	{
		case 0:
				m_dimensions = Vector2i(1024,1024);
		break;

		case 1:
		case 2:

				m_dimensions = Vector2i(1024,1024);
		break;
	}


	m_shadow_map_fbo[0] = new FBO(m_map_type, m_dimensions.x, m_dimensions.y, require_render_buffer);

	//set up a caster matrix set
	Matrix4 translation;
	Matrix4 rotation_x;
	Matrix4 rotation_y;

	translation.create_translation(Vector3f(-110, -1770, -150));
	rotation_x.create_cardinal_rotation(X_AXIS,84.0f);
	rotation_y.create_cardinal_rotation(Y_AXIS,324.0f);
	
	m_view_matrix = rotation_x * rotation_y * translation;

	m_cascade = cascade;

	m_caster_near = 100.0f;

	m_caster_direction = Vector3f(0.4,-0.85,0.0);
}

void ShadowMap::set_position()
{
	float depth = 0;

	switch(m_cascade)
	{
		case 0:

			//depth = 100;

		break;

		case 1:

			//depth = 1000;

		break;

		case 2:

			//depth = 2500;

		break;
	}

	//vector to store the calculated camera pos
	m_caster_position = g_camera_p->m_position + (g_camera_p->m_heading * depth);

	m_caster_rotation.x = psmath::rad_to_deg((float) asin(-m_caster_direction.y));
	m_caster_rotation.y = psmath::rad_to_deg((float) asin(m_caster_direction.x));

	Vector3f dir = m_caster_direction * -1.0f;
	//scale the unit vector by zoom
	Vector3f offset = dir * zoom;
	m_caster_position += offset;

	m_caster_position.x = (int)m_caster_position.x;
	m_caster_position.y = (int)m_caster_position.y;
	m_caster_position.z = (int)m_caster_position.z;

	//set up a caster matrix set
	Matrix4 translation;
	Matrix4 rotation_x;
	Matrix4 rotation_y;

	translation.create_translation(m_caster_position * -1.0f);
	rotation_x.create_cardinal_rotation(X_AXIS,m_caster_rotation.x);
	rotation_y.create_cardinal_rotation(Y_AXIS,m_caster_rotation.y);

	m_view_matrix = rotation_x * rotation_y * translation;
}

void ShadowMap::begin()
{
	if(!g_input_handler.m_key_held[VK_F]) set_position();

	float xy = 0;
	float farPlane = 0;

	switch(m_cascade)
	{
		case 0:

			xy = 256.0f;
			farPlane = 1500.0f;

		break;

		case 1:

			xy = 512.0f;
			farPlane = 2000.0f;

		break;

		case 2:

			xy = 2000.0f;
			farPlane = 6000.0f;

		break;
	}

	m_caster_dimensions = Vector3f(xy,xy, farPlane);

	//bind fbo and set matrix
	glMatrixMode( GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-m_caster_dimensions.x, m_caster_dimensions.x, -m_caster_dimensions.y, m_caster_dimensions.y ,m_caster_near, m_caster_dimensions.z );

	//get the projection matrix from gl
	float gl_projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX,gl_projection);
	m_projection_matrix.set_matrix_from_gl(gl_projection);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	m_view_matrix.multiply_with_gl_matrix();

	if(m_map_type == VARIANCE_SHADOW)
	{
		m_shadow_map_fbo[0]->bind_clear_depth_to_max();
	}
	else
	{
		ShaderManager::instance()->unbind();
		m_shadow_map_fbo[0]->bind();
	}


}

void ShadowMap::render_volume()
{
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glPushMatrix();

	m_view_matrix.multiply_with_gl_matrix();
	pshelpers::cube(m_caster_position,m_caster_dimensions);

	glPopMatrix();

	glColor4f(1.0f,0.0f,1.0f,1.0f);

	Vector3f extent = m_caster_position + (m_caster_direction * m_caster_dimensions.z);

	glBegin(GL_LINES);
	glVertex3f(m_caster_position.x,m_caster_position.y,m_caster_position.z);
	glVertex3f(extent.x,extent.y,extent.z);
	glEnd();
}

void ShadowMap::end()
{
	//reset states
	/*if(m_map_type == VARIANCE_SHADOW)
	{
		ShaderManager::instance()->unbind();
	}
	else
	{
		ShaderManager::instance()->unbind();
	}*/

	m_shadow_map_fbo[0]->unbind();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void ShadowMap::bind_texture()
{
	build_texture_matrix();

	//bind fbo as texture
	m_shadow_map_fbo[0]->bind_texture(7,0);

	//depth shadow requirements
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
}

void ShadowMap::bind_texture_deferred(int unit)
{
	build_texture_matrix();

	char uniform_name[32];
	sprintf(uniform_name,"shadow_map%i",m_cascade);

	m_shadow_map_fbo[0]->bind_texture(unit,0);
	ShaderManager::instance()->set_uniform(Shaders::deferred,uniform_name,unit);

	sprintf(uniform_name,"shadow_map_texel_size%i",m_cascade);
	ShaderManager::instance()->set_uniform(Shaders::deferred,uniform_name,1.0f / m_dimensions.x);
}


void ShadowMap::blur()
{
	//simple gaussian filter for now (image is bound to unit 0)
	ShaderManager::instance()->bind(Shaders::gaussian);
	ShaderManager::instance()->set_uniform(Shaders::gaussian,"image",0);

	float pixel_size = (float) (1.0f / m_dimensions.x);

	const int passes = 1;
	for(int i = 0; i < passes; i++)
	{
		//blur horizontally
		//bind shadow texture 0 to fbo 1
		ShaderManager::instance()->set_uniform(Shaders::gaussian,"u_scale",Vector2f(pixel_size,0.0f));
		m_shadow_map_fbo[1]->bind();

			m_shadow_map_fbo[0]->bind_texture(0,0);
			pshelpers::screen_space_quad(m_dimensions);

		m_shadow_map_fbo[1]->unbind();

		//blur vertically
		//bind shadow texture 1 to fbo 0
		ShaderManager::instance()->set_uniform(Shaders::gaussian,"u_scale",Vector2f(0.0f,pixel_size));
		m_shadow_map_fbo[0]->bind();

			m_shadow_map_fbo[1]->bind_texture(0,0);
			pshelpers::screen_space_quad(m_dimensions);

		m_shadow_map_fbo[0]->unbind();
	}
	
	ShaderManager::instance()->unbind();
}


void ShadowMap::build_texture_matrix()
{
	//bias matrix transforms from world space to texture space
	Matrix4 bias_matrix;
	bias_matrix.create_bias();

	Matrix4 shadow_projection_martix;
	shadow_projection_martix = bias_matrix * m_projection_matrix * m_view_matrix;

	float gl_shadow_projection_martix[16];
	shadow_projection_martix.gl_compliant_matrix(gl_shadow_projection_martix);

 	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glLoadIdentity();

	glMultMatrixf(gl_shadow_projection_martix);

	glMatrixMode(GL_MODELVIEW);

	char uniform_name[32];
	sprintf(uniform_name,"shadow_matrix%i",m_cascade);

	ShaderManager::instance()->set_uniform(Shaders::deferred,uniform_name,gl_shadow_projection_martix);
}

void ShadowMap::build_mipmaps()
{

}

void ShadowMap::display()
{
	m_shadow_map_fbo[0]->bind_texture(0,0);
	pshelpers::screen_space_quad(Vector2i(1024,168));
}

ShadowMap::~ShadowMap()
{
	delete m_shadow_map_fbo[0];
}