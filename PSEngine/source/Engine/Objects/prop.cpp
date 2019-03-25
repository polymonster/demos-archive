/*=========================================================*\
|	prop.cpp - model instance scene object
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	16/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#include "prop.h"
#include "model_manager.h"
#include "physics_manager.h"
#include "shader_manager.h"
#include "camera.h"
#include "timer.h"

Prop::Prop( int handle, Matrix4* mat )
{
	m_model_handle = handle;
	m_transformation = *mat;
	m_physics_handle = -1;
	m_animated = false;
}

Prop::Prop( int handle, Vector3f origin, float mass, bool add_to_physics)
{
	m_model_handle = handle;

	if(add_to_physics)
	{
		int collision_shape = g_model_mgr_p->Models(m_model_handle)->collision_shape_handle();
		m_centre_of_mass_offset = g_model_mgr_p->Models(m_model_handle)->centre_of_mass_offset();
		m_physics_handle = g_physics_mgr_p->add_rigid_body(mass, origin, collision_shape );
	}
	else
	{
		m_transformation.identity();
	}

	m_animated = false;

	update();
}

Prop::~Prop( void )
{

}

void Prop::update( void )
{
	if(m_physics_handle != -1)
	{
		g_physics_mgr_p->extract_matrix(&m_transformation, m_physics_handle);

		Matrix4 offset_matrix;
		offset_matrix.identity();

		//hack to rectify origin
		if(m_physics_handle != -1)
		{
			offset_matrix.create_translation(m_centre_of_mass_offset);
		}

		m_transformation = m_transformation * offset_matrix;
	}
}


void Prop::render()
{
	render(false);
}

void Prop::render(bool shadow_pass)
{
	float gl_mvp[16];

		if(!shadow_pass)
		{
			glPushMatrix();

			float gl_mv[16];
			glGetFloatv(GL_MODELVIEW_MATRIX,gl_mv);

			float gl_p[16];
			glGetFloatv(GL_PROJECTION_MATRIX,gl_p);

			float gl_fin[16];
			m_transformation.gl_compliant_matrix(gl_fin);

			glLoadMatrixf(gl_p);
			glMultMatrixf(gl_mv);
			glMultMatrixf(gl_fin);

			glGetFloatv(GL_MODELVIEW_MATRIX,gl_mvp);

			glPopMatrix();

			Matrix4 *model_matrix = get_matrix();
			Matrix4 orientation = model_matrix->get_orientation();

			ShaderManager::instance()->set_uniform(Shaders::g_buffer,"previous_matrix",&m_father_transformation);
			ShaderManager::instance()->set_uniform(Shaders::g_buffer,"grandfather_matrix",&m_grandfather_transformation);
			ShaderManager::instance()->set_uniform(Shaders::g_buffer,"model_matrix", model_matrix );
			ShaderManager::instance()->set_uniform(Shaders::g_buffer,"rotation_matrix", &orientation);
		}

	glPushMatrix();	

		m_transformation.multiply_with_gl_matrix();
		g_model_mgr_p->render( m_model_handle );

	glPopMatrix();

	if(!shadow_pass)
	{
		m_grandfather_transformation = m_father_transformation;
		m_father_transformation.set_matrix_from_gl(gl_mvp);
	}
}

void Prop::physics_world_render()
{
	/*glPushMatrix();

		m_transformation.multiply_with_gl_matrix();

		Vector3f size = g_model_mgr_p->Models(m_model_handle)->get_aabb()->m_size * 0.5f;

		pshelpers::cube(m_centre_of_mass_offset * -1,size);

	glPopMatrix();*/

}

AnimatedProp::AnimatedProp(int handle, Matrix4* mat)
{
	m_state = 0;
	m_transformation = *mat;
	m_model_handle = handle;
	m_animated = true;
}

void AnimatedProp::update()
{
	g_model_mgr_p->update_animated(m_model_handle);
}

void AnimatedProp::render(bool shadow_pass)
{
	float gl_mvp[16];

	if(!shadow_pass)
	{
		glPushMatrix();

		float gl_mv[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,gl_mv);

		float gl_p[16];
		glGetFloatv(GL_PROJECTION_MATRIX,gl_p);

		float gl_fin[16];
		m_transformation.gl_compliant_matrix(gl_fin);

		glLoadMatrixf(gl_p);
		glMultMatrixf(gl_mv);
		glMultMatrixf(gl_fin);

		glGetFloatv(GL_MODELVIEW_MATRIX,gl_mvp);

		glPopMatrix();

		Matrix4 *model_matrix = get_matrix();
		Matrix4 orientation = model_matrix->get_orientation();

		ShaderManager::instance()->set_uniform(Shaders::g_buffer_skinned,"previous_matrix",&m_father_transformation);
		ShaderManager::instance()->set_uniform(Shaders::g_buffer_skinned,"grandfather_matrix",&m_grandfather_transformation);
		ShaderManager::instance()->set_uniform(Shaders::g_buffer_skinned,"model_matrix", model_matrix );

		ShaderManager::instance()->set_uniform(Shaders::g_buffer_skinned,"rotation_matrix", &orientation );
	}

	glPushMatrix();

		m_transformation.multiply_with_gl_matrix();
		g_model_mgr_p->render( m_model_handle );

	glPopMatrix();

	if(!shadow_pass)
	{
		m_grandfather_transformation = m_father_transformation;
		m_father_transformation.set_matrix_from_gl(gl_mvp);
	}
}

void PlayerCharacter::update()
{
	m_speed = 100.0f;
	m_look_speed = 1.0f;

	//to be replace with time based delta later on
	float time_step = g_game_timer.get_game_time();

	float xrotrad = psmath::deg_to_rad(m_rotation.x);
	float yrotrad = psmath::deg_to_rad(m_rotation.y);

	//Forward movement
	if(g_input_handler.m_key_held[VK_W] || g_input_handler.m_key_pressed[VK_W] ) 
	{
		m_movement_velocity.x += float(sin(yrotrad)) * time_step * m_speed;
		m_movement_velocity.z -= float(cos(yrotrad)) * time_step * m_speed;
	}

	//Backwards Movement
	if(g_input_handler.m_key_held[VK_S] || g_input_handler.m_key_pressed[VK_S])
	{
		m_movement_velocity.x -= float(sin(yrotrad))* time_step * m_speed;
		m_movement_velocity.z += float(cos(yrotrad))* time_step * m_speed;
	}

	//strafe right
	if (g_input_handler.m_key_held[VK_D] || g_input_handler.m_key_pressed[VK_D])
	{
		m_movement_velocity.x += float(cos(yrotrad)) * time_step * m_speed;
		m_movement_velocity.z += float(sin(yrotrad)) * time_step * m_speed;
	}

	//strafe left
	if (g_input_handler.m_key_held[VK_A] || g_input_handler.m_key_pressed[VK_A])
	{
		m_movement_velocity.x -= float(cos(yrotrad)) * time_step * m_speed;
		m_movement_velocity.z -= float(sin(yrotrad)) * time_step * m_speed;
	}

	m_movement_velocity.x = min(m_movement_velocity.x, 20.0f);
	m_movement_velocity.x = max(m_movement_velocity.x, -20.0f);
	m_movement_velocity.z = min(m_movement_velocity.z, 20.0f);
	m_movement_velocity.z = max(m_movement_velocity.z, -20.0f);

	if(m_movement_velocity.x > velocity_threshold || m_movement_velocity.x < -velocity_threshold )
	{
		float diff = m_movement_velocity.x - velocity_threshold;
		m_movement_velocity.x = m_movement_velocity.x - (diff * (10.0f * time_step));
	}
	else m_movement_velocity.x = 0.0f;

	if(m_movement_velocity.y > velocity_threshold || m_movement_velocity.y < -velocity_threshold )
	{
		float diff = m_movement_velocity.y - velocity_threshold;
		m_movement_velocity.y = m_movement_velocity.y - (diff * (10.0f * time_step));
	}
	else m_movement_velocity.y = 0.0f;

	if(m_movement_velocity.z > velocity_threshold || m_movement_velocity.z < -velocity_threshold )
	{
		float diff = m_movement_velocity.z - velocity_threshold;
		m_movement_velocity.z = m_movement_velocity.z - (diff * (10.0f * time_step));
	}
	else m_movement_velocity.z = 0.0f;

	m_position += m_movement_velocity;

	//look movement
	if(g_input_handler.m_mouse_button_pressed[MIDDLE] || g_input_handler.m_mouse_button_held[MIDDLE])
	{
		float diffx = g_input_handler.m_mouse_position.y - g_input_handler.m_last_mouse_position.y;
		float diffy = g_input_handler.m_mouse_position.x - g_input_handler.m_last_mouse_position.x;

		if(diffx < camera_movement_threshold && diffx > -camera_movement_threshold) diffx = 0.0f;
		if(diffy < camera_movement_threshold && diffy > -camera_movement_threshold) diffy = 0.0f;

		m_rotation_velocity.x += (diffx) * time_step * m_look_speed;
		m_rotation_velocity.y += (diffy) * time_step * m_look_speed;

		m_rotation_velocity.x = min(m_rotation_velocity.x, 60.0f);
		m_rotation_velocity.x = max(m_rotation_velocity.x, -60.0f);
		m_rotation_velocity.y = min(m_rotation_velocity.y, 60.0f);
		m_rotation_velocity.y = max(m_rotation_velocity.y, -60.0f);
	}

	if(m_rotation_velocity.x > velocity_threshold || m_rotation_velocity.x < -velocity_threshold )
	{
		float diff = m_rotation_velocity.x - velocity_threshold;
		m_rotation_velocity.x = m_rotation_velocity.x - (diff * (10.0f * time_step));
	}
	else m_rotation_velocity.x = 0.0f;

	if(m_rotation_velocity.y > velocity_threshold || m_rotation_velocity.y < -velocity_threshold )
	{
		float diff = m_rotation_velocity.y - velocity_threshold;
		m_rotation_velocity.y = m_rotation_velocity.y - (diff * (10.0f * time_step));
	}
	else m_rotation_velocity.y = 0.0f;

	m_rotation += m_rotation_velocity;

	Matrix4 translate, rotate_x, rotate_y;
	translate.create_translation(m_position);
	rotate_x.create_cardinal_rotation(0, m_rotation.x);
	rotate_y.create_cardinal_rotation(1, m_rotation.y);
	m_transformation = translate;

	AnimatedProp::update();
}

void PlayerCharacter::set_camera_position( Camera *p_camera )
{

}

PlayerCharacter::PlayerCharacter( int handle, Matrix4* mat )
{
	m_state = 0;
	m_transformation = *mat;
	m_model_handle = handle;
	m_animated = true;
}