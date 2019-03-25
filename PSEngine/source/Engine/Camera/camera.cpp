/*=========================================================*\
|	camera.h - camera classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Camera
|				Coder	:	ADixon
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "camera.h"
#include "program.h"
#include "timer.h"

/*======================== VARIABLES ======================*/

Camera *g_camera_p = NULL;

/*=========================================================*\
|	Generic Camera 
\*=========================================================*/
Camera::Camera(){}
void Camera::set_position(){}

Matrix4 Camera::get_matrix()
{
	return m_camera_matrix;
}

Matrix4 Camera::get_rotation_matrix()
{
	Matrix4 rot_mat = m_camera_matrix;

	rot_mat.m14 = 0.0f;
	rot_mat.m24 = 0.0f;
	rot_mat.m34 = 0.0f;
	rot_mat.m44 = 1.0f;

	return rot_mat;
}

void Camera::update()
{
	float xrotrad = psmath::deg_to_rad(m_rotation.x);
	float yrotrad = psmath::deg_to_rad(m_rotation.y);

	m_heading.x =  float(sin(yrotrad));
	m_heading.y = -float(sin(xrotrad));
	m_heading.z = -float(cos(yrotrad));

	m_heading = psmath::normalise(m_heading);

	float gl_mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,gl_mv);
	Matrix4 mv;
	mv.set_matrix_from_gl(gl_mv);

	m_camera_matrix = mv;
}

/*=========================================================*\
|	FPS Camera - First person style camera movement
\*=========================================================*/
FirstPersonCamera::FirstPersonCamera()
{
	m_speed = 30.0f;
	m_look_speed = 10.0f;
}

void FirstPersonCamera::set_position()
{
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0f, (float)g_screen_size.x/(float)g_screen_size.y, 1.0f, 10000.0f );

	glMatrixMode( GL_MODELVIEW );
	glRotated(m_rotation.x,1,0,0);
	glRotated(m_rotation.y,0,1,0);
	glTranslated(-m_position.x,-m_position.y,-m_position.z);

	Camera::update();
}

void FirstPersonCamera::update()
{
	//to be replace with time based delta later on
	float time_step = g_game_timer.get_game_time();

	float xrotrad = psmath::deg_to_rad(m_rotation.x);
	float yrotrad = psmath::deg_to_rad(m_rotation.y);

	//Forward movement
	if(g_input_handler.m_key_held[VK_UP] || g_input_handler.m_key_pressed[VK_UP] ) 
	{
		m_movement_velocity.x += float(sin(yrotrad)) * time_step * m_speed;
		m_movement_velocity.z -= float(cos(yrotrad)) * time_step * m_speed;
		m_movement_velocity.y -= float(sin(xrotrad)) * time_step * m_speed;
	}

	//Backwards Movement
	if(g_input_handler.m_key_held[VK_DOWN] || g_input_handler.m_key_pressed[VK_DOWN])
	{
		m_movement_velocity.x -= float(sin(yrotrad))* time_step * m_speed;
		m_movement_velocity.z += float(cos(yrotrad))* time_step * m_speed;
		m_movement_velocity.y += float(sin(xrotrad))* time_step * m_speed;
	}

	//strafe right
	if (g_input_handler.m_key_held[VK_RIGHT] || g_input_handler.m_key_pressed[VK_RIGHT])
	{
		m_movement_velocity.x += float(cos(yrotrad)) * time_step * m_speed;
		m_movement_velocity.z += float(sin(yrotrad)) * time_step * m_speed;
	}

	//strafe left
	if (g_input_handler.m_key_held[VK_LEFT] || g_input_handler.m_key_pressed[VK_LEFT])
	{
		m_movement_velocity.x -= float(cos(yrotrad)) * time_step * m_speed;
		m_movement_velocity.z -= float(sin(yrotrad)) * time_step * m_speed;
	}

	m_movement_velocity.x = min(m_movement_velocity.x, 20.0f);
	m_movement_velocity.x = max(m_movement_velocity.x, -20.0f);
	m_movement_velocity.y = min(m_movement_velocity.y, 20.0f);
	m_movement_velocity.y = max(m_movement_velocity.y, -20.0f);
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
}

/*=========================================================*\
|	Modelling Camera - Max / Maya Style Camera
\*=========================================================*/
ModellingCamera::ModellingCamera()
{
	m_speed = 10;
	m_zoom = 100.0f;
}

void ModellingCamera::set_position()
{
	//vector to store the calculated camera pos
	Vector3f camera_position = m_focus_point;

	//get the rotations in radians
	float xrotrad = psmath::deg_to_rad(m_rotation.x);
	float yrotrad = psmath::deg_to_rad(m_rotation.y);

	Vector3f unit_vector;

	//calculate the unit sphere position
	unit_vector.x += float( -cos(xrotrad) * sin(yrotrad) );
	unit_vector.y += float(  sin(xrotrad)				 );
	unit_vector.z += float(  cos(xrotrad) * cos(yrotrad) );

	//scale the unit vector by zoom
	camera_position +=  (unit_vector * float(m_zoom));

	m_position = camera_position;

	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0f, (float)g_screen_size.x/(float)g_screen_size.y, 1.0f, 10000.0f );

	glMatrixMode( GL_MODELVIEW );
	glRotated(m_rotation.x,1,0,0);
	glRotated(m_rotation.y,0,1,0);
	glTranslated(-m_position.x,-m_position.y,-m_position.z);
}

void ModellingCamera::update()
{
	if(g_input_handler.m_mouse_button_pressed[MIDDLE] || g_input_handler.m_mouse_button_held[MIDDLE])
	{
		if(g_input_handler.m_key_pressed[VK_SHIFT] || g_input_handler.m_key_held[VK_SHIFT])
		{
			float incy = (g_input_handler.m_mouse_position.y - g_input_handler.m_last_mouse_position.y) * (m_zoom * 0.005f);
			float incx = (g_input_handler.m_mouse_position.x - g_input_handler.m_last_mouse_position.x) * (m_zoom * 0.005f);

			float xrotrad = psmath::deg_to_rad(m_rotation.x);
			float yrotrad = psmath::deg_to_rad(m_rotation.y);

			m_focus_point.x += float( (cos(yrotrad) * incx) + (-(sin(xrotrad) * sin(yrotrad)) * incy ) );
			m_focus_point.y -= float(  cos(xrotrad) * incy );
			m_focus_point.z += float( (sin(yrotrad) * incx) + ((sin(xrotrad) * cos(yrotrad)) * incy )  );
		}
		else
		{
			m_rotation.x += (g_input_handler.m_mouse_position.y - g_input_handler.m_last_mouse_position.y);
			m_rotation.y += (g_input_handler.m_mouse_position.x - g_input_handler.m_last_mouse_position.x);
		}
	}

	m_zoom += g_input_handler.m_wheel_movement;

	if(g_input_handler.m_key_pressed[VK_Z] || g_input_handler.m_key_held[VK_Z])
	{
		m_focus_point = Vector3f(0,0,0);
		m_zoom = 100.0f;
	}

	Camera::update();
}
