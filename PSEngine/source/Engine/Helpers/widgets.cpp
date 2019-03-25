/*=========================================================*\
|	widgets.cpp - helpers for widgets to move things
|-----------------------------------------------------------|
|				Project :	PolySpoon Helpers
|				Coder	:	ADixon
|				Date	:	02/06/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "helpers.h"
#include "camera.h"
#include "polyspoon_math.h"

/*======================== VARIABLES ======================*/


/*=========================================================*\
| axis_widget_picking
|				- a scale or translate widget, representing
|				- 3 global axes (x,y,z), uses colour picking
|				- to find the selected axis (or axes).
\*=========================================================*/
void pshelpers::axis_widget_picking(Vector3f center, Vector3f *selected_axis)
{
	if(g_input_handler.m_mouse_button_pressed[LEFT] || g_input_handler.m_mouse_button_held[LEFT]) return;

	float scale = psmath::distance(center,g_camera_p->m_position) / 100;
	
	selected_axis->x = 0;
	selected_axis->y = 0;
	selected_axis->z = 0;

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glLineWidth(10);

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);
		
		//glEnable(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);
		//x-y
		glColor3f(1,1,0);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,10,0);
		glEnd();

		//x-z
		glColor3f(1,0,1);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,0,10);
		glEnd();

		//y-z
		glColor3f(0,1,1);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
		glVertex3f(0,0,10);
		glEnd();

		glDisable(GL_DEPTH_TEST);
		//x-axis
		glPushMatrix();
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20,0,0);
			glEnd();
		glPopMatrix();
		//y axis
		glPushMatrix();
			glColor3f(0,1,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
			glEnd();
		glPopMatrix();
		//z axis
		glPushMatrix();
			glColor3f(0,0,1);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
			glEnd();
		glPopMatrix();

		//all
		glColor3f(1,1,1);
		cube();

	glPopMatrix();

	Vector3f pixel_colour = g_input_handler.colour_picking();

	if(pixel_colour.x / 255.0f == 1.0f) selected_axis->x = 1.0f;
	if(pixel_colour.y / 255.0f == 1.0f) selected_axis->y = 1.0f;
	if(pixel_colour.z / 255.0f == 1.0f) selected_axis->z = 1.0f;

	glLineWidth(1);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/*=========================================================*\
| axis_widget_render
|				- a scale or translate widget, representing
|				- 3 global axes (x,y,z), scale widget has
|				- small cubes at the tip of each axis
|				- translate is represented by arrows on each axis
\*=========================================================*/
void pshelpers::axis_widget_render(Vector3f center, Vector3f selected, int type)
{
	float scale = psmath::distance(center,g_camera_p->m_position) / 100;

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glLineWidth(1.5);

		//x-y
		glColor4f(0.4f,0.4f,0.4f,0.2f);
		if((selected.x == 1) && (selected.y == 1)) glColor4f(1.0f,0.7f,0.0f,0.2f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(10.0f,0.0f,0.0f);
			glVertex3f(0.0f,10.0f,0.0f);
		glEnd();

		glColor3f(0.7f,0.7f,0.7f);
		if((selected.x == 1) && (selected.y == 1)) glColor3f(1.0f,0.7f,0.0f);
		glBegin(GL_LINES);
			glVertex3f(10.0f,0.0f,0.0f);
			glVertex3f(0.0f,10.0f,0.0f);
			glVertex3f(8.0f,0.0f,0.0f);
			glVertex3f(0.0f,8.0f,0.0f);
		glEnd();

		//x-z
		glColor4f(0.4f,0.4f,0.4f,0.2f);
		if((selected.x == 1) && (selected.z == 1)) glColor4f(1.0f,0.7f,0.0f,0.2f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(10.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,10.0f);
		glEnd();

		glColor3f(0.7f,0.7f,0.7f);
		if((selected.x == 1) && (selected.z == 1)) glColor3f(1.0f,0.7f,0.0f);
		glBegin(GL_LINES);
			glVertex3f(10.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,10.0f);
			glVertex3f(8.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,8.0f);
		glEnd();

		//y-z
		glColor4f(0.4f,0.4f,0.4f,0.2f);
		if((selected.y == 1) && (selected.z == 1)) glColor4f(1.0f,0.7f,0.0f,0.2f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,10.0f,0.0f);
			glVertex3f(0.0f,0.0f,10.0f);
		glEnd();

		glColor3f(0.7f,0.7f,0.7f);
		if((selected.z == 1) && (selected.y == 1)) glColor3f(1.0f,0.7f,0.0f);
		glBegin(GL_LINES);
			glVertex3f(0.0f,10.0f,0.0f);
			glVertex3f(0.0f,0.0f,10.0f);
			glVertex3f(0.0f,8.0f,0.0f);
			glVertex3f(0.0f,0.0f,8.0f);
		glEnd();
		
		//x-axis
		glPushMatrix();
			if(selected.x == 1) glColor3f(1.0f,0.7f,0.0f);
			else glColor3f(0.5f,0.0f,0.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(20.0f,0.0f,0.0f);
			glEnd();
		glPopMatrix();

		if(type == 1) 
		{
			glPushMatrix();
				glTranslated(20,0,0) ;
				glScaled(0.5f,0.5f,0.5f);
			cube();
			glPopMatrix();
		}
		else
		{
			glBegin(GL_LINES);
				glVertex3f(20.0f,0.0f,0.0f);
				glVertex3f(19.0f,1.0f,0.0f);
			glEnd();

			glBegin(GL_LINES);
				glVertex3f(20.0f,0.0f,0.0f);
				glVertex3f(19.0f,-1.0f,0.0f);
			glEnd();
		}

		//y axis
		glPushMatrix();
			if(selected.y == 1) glColor3f(1.0f,0.7f,0.0f);
			else glColor3f(0.0f,0.5f,0.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,20.0f,0.0f);
			glEnd();
		glPopMatrix();

		if(type == 1) 
		{
			glPushMatrix();
				glTranslated(0,20,0) ;
				glScaled(0.5f,0.5f,0.5f);
			cube();
			glPopMatrix();
		}
		else
		{

			glBegin(GL_LINES);
				glVertex3f(0.0f,20.0f,0.0f);
				glVertex3f(1.0f,19.0f,0.0f);
			glEnd();

			glBegin(GL_LINES);
				glVertex3f(0.0f,20.0f,0.0f);
				glVertex3f(-1.0f,19.0f,0.0f);
			glEnd();
		}

		//z axis
		glPushMatrix();
			if(selected.z == 1) glColor3f(1.0f,0.7f,0.0f);
			else glColor3f(0.0f,0.0f,0.5f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,0.0f,20.0f);
			glEnd();
		glPopMatrix();

		if(type == 1) 
		{
			glPushMatrix();
				glTranslated(0.0f,0.0f,20.0f) ;
				glScaled(0.5f,0.5f,0.5f);
			cube();
			glPopMatrix();
		}
		else
		{
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,20.0f);
				glVertex3f(0.0f,1.0f,19.0f);
			glEnd();

			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,20.0f);
				glVertex3f(0.0f,-1.0f,19.0f);
			glEnd();
		}


	glPopMatrix();

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
}

/*=========================================================*\
| axis_widget_update
|				- the mechanics of using a translate or scale
|				- widget. translates mouse movements (screen space)
|				- into 3D actions (world space) using trigenometry
\*=========================================================*/
void pshelpers::axis_widget_update(Vector3f *position, Vector3f *scale, Vector3f selected_axis)
{
	if(g_input_handler.m_mouse_button_pressed[LEFT] || g_input_handler.m_mouse_button_held[LEFT])
	{
		float widget_scale = psmath::distance(*position,g_camera_p->m_position) * 0.01f * 0.145f;

		float incy = (g_input_handler.m_mouse_position.y - g_input_handler.m_last_mouse_position.y) * psmath::absolute(widget_scale);
		float incx = (g_input_handler.m_mouse_position.x - g_input_handler.m_last_mouse_position.x) * psmath::absolute(widget_scale);

		float xrotrad = psmath::deg_to_rad(g_camera_p->m_rotation.x);
		float yrotrad = psmath::deg_to_rad(g_camera_p->m_rotation.y);

		position->x += selected_axis.x * float( (cos(yrotrad) * incx) + (-(sin(xrotrad) * sin(yrotrad)) * incy ));
		position->y -= selected_axis.y * float( cos(xrotrad) * incy );
		position->z += selected_axis.z * float( (sin(yrotrad) * incx) + ((sin(xrotrad) * cos(yrotrad)) * incy ));

		scale->x += selected_axis.x * float( (cos(yrotrad) * incx) + (-(sin(xrotrad) * sin(yrotrad)) * incy ));
		scale->y -= selected_axis.y * float( cos(xrotrad) * incy );
		scale->z += selected_axis.z * float( (sin(yrotrad) * incx) + ((sin(xrotrad) * cos(yrotrad)) * incy ));
	}
}

/*=========================================================*\
| rotate_widget_picking
|				- a rotate widget, representing
|				- 3 global rotation axes (x,y,z) 
|				- uses colour picking to find the selected axis.
\*=========================================================*/
void pshelpers::rotate_widget_picking(Vector3f center, Vector3f *selected_axis)
{
	if(g_input_handler.m_mouse_button_pressed[LEFT] || g_input_handler.m_mouse_button_held[LEFT]) return;

	float scale = psmath::distance(center,g_camera_p->m_position) / 100;

	selected_axis->x = 0;
	selected_axis->y = 0;
	selected_axis->z = 0;

	glLineWidth(10);

	glPushMatrix();
	
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

	int segments = 30;

	float xline = 0;
	float yline = 0;
	float zline = 0;

	float rot = 0;
	int size = 10;

	glDisable(GL_BLEND);

	//x-z (ABOUT Y)
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		float rad_rot = psmath::deg_to_rad(rot);

		xline = float( (cos(rad_rot) * size) );
		zline = float( (sin(rad_rot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	xline = 0;
	yline = 0;
	zline = 0;

	//z-y (ABOUT X)
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		float rad_rot = psmath::deg_to_rad(rot);

		yline = float( (cos(rad_rot) * size) );
		zline = float( (sin(rad_rot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	xline = 0;
	yline = 0;
	zline = 0;

	//x-y (ABOUT Z)
	glColor3f(0,0,1);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		float rad_rot = psmath::deg_to_rad(rot);

		yline = float( (cos(rad_rot) * size) );
		xline = float( (sin(rad_rot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	glPopMatrix();

	glLineWidth(1);

	Vector3f pixel_colour = g_input_handler.colour_picking();

	if(pixel_colour.x / 255.0f == 1.0f) selected_axis->x = 1.0f;
	if(pixel_colour.y / 255.0f == 1.0f) selected_axis->y = 1.0f;
	if(pixel_colour.z / 255.0f == 1.0f) selected_axis->z = 1.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/*=========================================================*\
| rotate_widget_render
|					- renders the 3 rotation axes as circles
|					- selected appears in orange
\*=========================================================*/
void pshelpers::rotate_widget_render(Vector3f center, Vector3f selected)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glLineWidth(1.5f);

	float scale = psmath::distance(center,g_camera_p->m_position) / 100;

	glPushMatrix();
	
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

	int segments = 20;

	float xline = 0;
	float yline = 0;
	float zline = 0;

	float rot = 0;
	int size = 10;

	//x-z (ABOUT Y)
	glColor3f(0.0f,0.5f,0.0f);
	if(selected.y == 1.0f) glColor3f(1.0f,0.7f,0.0f);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		float rad_rot = psmath::deg_to_rad(rot);

		xline = float( (cos(rad_rot) * size) );
		zline = float( (sin(rad_rot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	xline = 0;
	yline = 0;
	zline = 0;

	//z-y (ABOUT X)
	glColor3f(0.5f,0.0f,0.0f);
	if(selected.x == 1.0f) glColor3f(1.0f,0.7f,0.0f);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		float rad_rot = psmath::deg_to_rad(rot);

		yline = float( (cos(rad_rot) * size) );
		zline = float( (sin(rad_rot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	xline = 0;
	yline = 0;
	zline = 0;

	//x-y (ABOUT Z)
	glColor3f(0.0f,0.0f,0.5f);
	if(selected.z == 1.0f) glColor3f(1.0f,0.7f,0.0f);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		float rad_rot = psmath::deg_to_rad(rot);

		yline = float( (cos(rad_rot) * size) );
		xline = float( (sin(rad_rot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	glPopMatrix();

	glLineWidth(1);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void pshelpers::rotate_widget_update(Vector3f position, Vector3f *rotation, Vector3f selected_axis)
{
	if(g_input_handler.m_mouse_button_pressed[LEFT] || g_input_handler.m_mouse_button_held[LEFT])
	{
		float widget_scale = psmath::distance(position,g_camera_p->m_position) * 0.01f * 0.145f;

		float incy = (g_input_handler.m_mouse_position.y - g_input_handler.m_last_mouse_position.y) * psmath::absolute(widget_scale);
		float incx = (g_input_handler.m_mouse_position.x - g_input_handler.m_last_mouse_position.x) * psmath::absolute(widget_scale);

		float xrotrad = psmath::deg_to_rad(g_camera_p->m_rotation.x);
		float yrotrad = psmath::deg_to_rad(g_camera_p->m_rotation.y);

		rotation->x += selected_axis.x * (incx + incy);
		rotation->y += selected_axis.y * (incx + incy);
		rotation->z += selected_axis.z * (incx + incy);
	}
}

Vector3f pshelpers::get_colour_from_id(unsigned int id)
{
	Vector3f colour_id(0.0f,0.0f,0.0f);

	if(id > 65025)
	{
		//65025 - ???????
		int multiplier = id / 65025;
		colour_id.z = multiplier;

		int remainder = id - (multiplier * 65025);

		if(remainder > 255)
		{
			multiplier = remainder / 255;
			colour_id.y = multiplier;

			remainder = remainder - (multiplier * 255);
			colour_id.x = remainder;

		}
		else
		{
			colour_id.x = remainder;
		}

	}
	else if(id > 255)
	{
		//256 - 65025
		int multiplier = id / 255;
		colour_id.y = multiplier;

		int remainder = id - (multiplier * 255);
		colour_id.x = remainder;

	} 
	else
	{
		//0 - 255;
		colour_id.x = id;
	}

	return colour_id;
}

unsigned int pshelpers::get_id_from_colour(Vector3f colour_id)
{
	return(colour_id.x + (colour_id.y * 255) + (colour_id.z * 65025) );

}

