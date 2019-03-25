/*=========================================================*\
|	helpers.h - various helper functions
|-----------------------------------------------------------|
|				Project :	PolySpoon Camera
|				Coder	:	ADixon
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _HELPERS_H
#define _HELPERS_H

/*======================== INCLUDES =======================*/

#include "input.h"
#include "window.h"
#include "polyspoon_math.h"
#include "console.h"
#include "render_object.h"
#include "model_manager.h"


/*======================== CONSTANTS ======================*/

typedef enum
{
	TRANSLATE_WIDGET = 0,
	SCALE_WIDGET,
	ROTATE_WIDGET,

}e_widget_types;

/*========================= CLASSES =======================*/

namespace pshelpers
{
	void initialise_render_helpers();
	void initialise_cube();
	void initialise_quad();
	void initialise_sphere();
	void clean_up_render_helpers();

	void grid(int cell_size, int grid_dimesion, int divisions);
	void quad(Vector3f position, Vector3f scale, int axis);
	void screen_space_quad(Vector2i size);

	void cube(Vector3f position = Vector3f(0,0,0) , Vector3f scale = Vector3f(1,1,1));
	void sphere(Vector3f position = Vector3f(0,0,0), float radius = 1);
	void ellipsoid(Vector3f position = Vector3f(0,0,0), Vector3f radii = Vector3f(1,1,1));

	void ps_translate(Vector3f position);
	void ps_rotate(Vector3f rotation);
	void ps_scale(Vector3f scale);

	void axis_widget_picking(Vector3f center, Vector3f *selected_axis);
	void axis_widget_render(Vector3f center, Vector3f selected, int type);
	void axis_widget_update(Vector3f *position, Vector3f *scale, Vector3f selected_axis);

	void rotate_widget_picking(Vector3f center, Vector3f *selected_axis);
	void rotate_widget_render(Vector3f center, Vector3f selected);
	void rotate_widget_update(Vector3f position, Vector3f *rotation, Vector3f selected_axis);

	Vector3f get_colour_from_id(unsigned int id);
	unsigned int get_id_from_colour(Vector3f colour_id);
};


/*================== EXTERNAL VARIABLES ===================*/

#endif //_HELPERS_H