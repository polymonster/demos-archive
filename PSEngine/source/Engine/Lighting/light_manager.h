/*=========================================================*\
|	lighting_manager.h - light class
|-----------------------------------------------------------|
|				Project :	PolySpoon Lighting
|				Coder	:	ADixon
|				Date	:	01/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef light_manager_h__
#define light_manager_h__

#include "light.h"
#include <vector>

class LightManager
{
public:

	void enable_lights();
	void add_light(Vector3f position, Vector3f ambient, Vector3f diffuse, Vector3f specular, int type = 0);
	void add_light(Vector3f position, int type = 0);

	//scene loop calls
	void update();
	void back_buffer_render();
	void aux_render();
	void render();

private:

	std::vector<Light *> m_lights;

	LightParameters m_light_parameters;

};

extern LightManager g_lighting_manager;


#endif // light_manager_h__