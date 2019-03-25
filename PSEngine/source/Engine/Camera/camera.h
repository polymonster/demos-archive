/*=========================================================*\
|	camera.h - camera classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Camera
|				Coder	:	ADixon
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _CAMERA_H
#define _CAMERA_H

/*======================== INCLUDES =======================*/

#include "input.h"
#include "polyspoon_math.h"
#include "console.h"

/*======================== CONSTANTS ======================*/
const float velocity_threshold = 0.001f;
const float camera_movement_threshold = 0.001f;

/*========================= CLASSES =======================*/

class Camera
{
public:

	Camera();

	virtual void set_position();
	virtual void update();
	virtual Matrix4 get_matrix();
	virtual Matrix4 get_rotation_matrix();

	Vector2f m_rotation;
	Vector3f m_position;

	Vector2f m_rotation_velocity;
	Vector3f m_movement_velocity;

	Vector3f m_heading;

	Matrix4 m_projection_matrix;
	Matrix4 m_camera_matrix;
};

class FirstPersonCamera : public Camera
{
public:

	FirstPersonCamera();

	void set_position();
	void update();

private:

	float m_speed;
	float m_look_speed;
};

class ModellingCamera : public Camera
{

public:

	ModellingCamera();

	void set_position();
	void update();

private:

	int m_speed;
	float m_zoom;

	Vector3f m_focus_point;
};

/*================== EXTERNAL VARIABLES ===================*/

extern Camera *g_camera_p;

#endif //_CAMERA_H