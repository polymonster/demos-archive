/*=========================================================*\
|	model.cpp - static model related classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Renderer
|				Coder	:	ADixon
|				Date	:	26/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "bounding_volumes.h"
#include "program.h"
#include "helpers.h"
#include "input.h"

/*======================== VARIABLES ======================*/

/*=========================================================*\
|	SPHERE - 3D Sphere
\*=========================================================*/
SPHERE::SPHERE() :
m_position(0.0f,0.0f,0.0f),
m_radius(100.0f),
m_widget_selected_axis(0.0f,0.0f,0.0f),
m_under_collision( false )
{ }

SPHERE::SPHERE(Vector3f position, float radius ) : 
m_position( position ), 
m_radius( radius ),
m_widget_selected_axis(0.0f,0.0f,0.0f), 
m_under_collision( false )
{ }

void SPHERE::render()
{
	pshelpers::sphere(m_position, m_radius);
}

void SPHERE::aux_render()
{
	pshelpers::axis_widget_render(m_position, m_widget_selected_axis, TRANSLATE_WIDGET);
}

void SPHERE::back_buffer_render()
{
	pshelpers::axis_widget_picking(m_position, &m_widget_selected_axis);
}

void SPHERE::update()
{
	pshelpers::axis_widget_update(&m_position,new Vector3f(),m_widget_selected_axis);
}

/*=========================================================*\
|	AA_ELLIPSOID - 3D Axially Aligned Ellipsoid (e-space)
\*=========================================================*/
AA_ELLIPSOID::AA_ELLIPSOID() : 
m_position(0.0f,0.0f,0.0f), 
m_radii(50.0f,100.0f,50.0f), 
m_widget_selected_axis(0.0f,0.0f,0.0f), 
m_under_collision( false )
{}

AA_ELLIPSOID::AA_ELLIPSOID(Vector3f position, Vector3f radii ) : 
m_position( position ), 
m_radii( radii ),
m_widget_selected_axis(0.0f,0.0f,0.0f), 
m_under_collision( false )
{}

void AA_ELLIPSOID::update()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_update(&m_position,new Vector3f(),m_widget_selected_axis);
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_update(new Vector3f(),&m_radii,m_widget_selected_axis);
	}
}

void AA_ELLIPSOID::render()
{
	glColor4f(0.0f,1.0f,1.0f,1.0f);
	if(m_under_collision) glColor4f(0.5f,0.0f,0.0f,1.0f);

	pshelpers::ellipsoid(m_position, m_radii);
}

void AA_ELLIPSOID::aux_render()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, TRANSLATE_WIDGET);
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, SCALE_WIDGET);
	}
}

void AA_ELLIPSOID::back_buffer_render()
{
	if((g_input_handler.m_key_held[VK_T]) || (g_input_handler.m_key_held[VK_E]))
	{
		pshelpers::axis_widget_picking(m_position, &m_widget_selected_axis);
	}
}

/*=========================================================*\
|	TRIANGLE - Triangle made from vertices
\*=========================================================*/
TRIANGLE::TRIANGLE() :
m_widget_selected_axis(0.0f,0.0f,0.0f)
{ }

TRIANGLE::TRIANGLE(Vector3fArray vertices) :
m_widget_selected_axis(0.0f,0.0f,0.0f)
{
	for(int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertices.at(i);
	}

}

void TRIANGLE::update()
{
	Vector3f position = m_vertices[0];

	pshelpers::axis_widget_update(&position,new Vector3f(),m_widget_selected_axis);

	Vector3f diff = position - m_vertices[0];

	for(int i = 0; i < 3; i++)
		m_vertices[i] += diff;


}

void TRIANGLE::render()
{
	glColor4f(0.0f,1.0f,0.0f,1.0f);

	glBegin(GL_TRIANGLES);

	for(int i = 0; i < 3; i++) 
		glVertex3f(m_vertices[i].x,m_vertices[i].y,m_vertices[i].z);

	glEnd();
}

void TRIANGLE::aux_render()
{
	pshelpers::axis_widget_render(m_vertices[0], m_widget_selected_axis, TRANSLATE_WIDGET);
}

void TRIANGLE::back_buffer_render()
{
	pshelpers::axis_widget_picking(m_vertices[0], &m_widget_selected_axis);
}

/*=========================================================*\
|	AABB3D - Axially Aligned Bounding Box!
\*=========================================================*/
AABB3D::AABB3D() : 
m_under_collision(false),
m_position(0.0f,0.0f,0.0f), 
m_size(100.0f,100.0f,100.0f) 
{ }

AABB3D::AABB3D(Vector3f position, Vector3f size ) : 
m_under_collision(false),
m_position( position ), 
m_size( size ) 
{ }

void AABB3D::update()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_update(&m_position,new Vector3f(),m_widget_selected_axis);
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_update(new Vector3f(),&m_size,m_widget_selected_axis);
	}
}

void AABB3D::render()
{
	glColor4f(1.0f,0.1f,0.0f,1.0f);
	if(m_under_collision) glColor4f(0.5f,0.0f,0.0f,1.0f);

	pshelpers::cube(m_position, m_size);
}

void AABB3D::aux_render()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, TRANSLATE_WIDGET);
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, SCALE_WIDGET);
	}
}

void AABB3D::back_buffer_render()
{
	if((g_input_handler.m_key_held[VK_T]) || (g_input_handler.m_key_held[VK_E]))
	{
		pshelpers::axis_widget_picking(m_position, &m_widget_selected_axis);
	}
}

/*=========================================================*\
|	OBB3D - Oriented Bounding Box!
\*=========================================================*/

OBB3D::OBB3D() :
m_under_collision(false)
{
	m_size = Vector3f(100.0f,100.0f,100.0f);
	m_rotation = Vector3f(0.0f,0.0f,0.0f);

	Matrix4 scale_matrix;
	scale_matrix.create_scale(m_size);
	m_orientation_matrix.identity();
	m_orientation_matrix = m_orientation_matrix * scale_matrix;
	m_position = m_orientation_matrix.get_translation();
}

OBB3D::OBB3D(Vector3f position, Vector3f size, Vector3f rotation) :
m_under_collision(false)
{
	m_position = position;
	m_size = size;
	m_rotation = rotation;

	build_matrix();
}

OBB3D::OBB3D(Matrix4 orientation_matrix) :
m_under_collision(false)
{
	m_orientation_matrix = orientation_matrix;

	m_position = m_orientation_matrix.get_translation();
}

void OBB3D::build_matrix()
{
	Matrix4 translation_matrix;
	Matrix4 scale_matrix;
	Matrix4 rotation_matrix[3];

	translation_matrix.create_translation(m_position);
	scale_matrix.create_scale(m_size);

	rotation_matrix[0].create_cardinal_rotation(X_AXIS,m_rotation.x); 
	rotation_matrix[1].create_cardinal_rotation(Y_AXIS,m_rotation.y); 
	rotation_matrix[2].create_cardinal_rotation(Z_AXIS,m_rotation.z); 

	m_orientation_matrix = 
		translation_matrix * rotation_matrix[0] * rotation_matrix[1] * rotation_matrix[2] * scale_matrix;

}

void OBB3D::update()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_update(&m_position,new Vector3f(),m_widget_selected_axis);

		build_matrix();
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_update(new Vector3f(),&m_size, m_widget_selected_axis);

		build_matrix();
	}
	else if(g_input_handler.m_key_held[VK_R])
	{
		pshelpers::rotate_widget_update(m_position,&m_rotation,m_widget_selected_axis);

		build_matrix();
	}
}

void OBB3D::render()
{
	glPushMatrix();

		glColor3f(1.0f,0.5f,0.0f);
		if(m_under_collision) glColor4f(0.5f,0.0f,0.0f,1.0f);

		m_orientation_matrix.multiply_with_gl_matrix();

		pshelpers::cube(Vector3f(0.0f,0.0f,0.0f), Vector3f(1.0f,1.0f,1.0f));

	glPopMatrix();

}

void OBB3D::aux_render()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, TRANSLATE_WIDGET);
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, SCALE_WIDGET);
	}
	else if(g_input_handler.m_key_held[VK_R])
	{
		pshelpers::rotate_widget_render(m_position, m_widget_selected_axis);
	}
}

void OBB3D::back_buffer_render()
{
	if((g_input_handler.m_key_held[VK_T]) || (g_input_handler.m_key_held[VK_E]))
	{
		pshelpers::axis_widget_picking(m_position, &m_widget_selected_axis);
	}
	else if(g_input_handler.m_key_held[VK_R])
	{
		pshelpers::rotate_widget_picking(m_position, &m_widget_selected_axis);
	}
}

/*=========================================================*\
|	Convex Hull - 3D Convex shape
\*=========================================================*/
CONVEX_HULL::CONVEX_HULL(Vector3f position, Vector3f size, Vector3f rotation, Vector3fArray vertices, IndexArray indices):
m_position(position),
m_size(size),
m_under_collision(false),
m_rotation(rotation)
{
	for(int i = 0; i < vertices.size(); i++)
	{
		m_vertices.push_back(vertices.at(i));
	}

	for(int i = 0; i < indices.size(); i++)
	{
		m_indices.push_back(indices.at(i));
	}

	for(int i = 0; i < m_indices.size(); i+=3)
	{
		int index[3];
		Vector3fArray tri_verts;

		for(int j = 0; j < 3; j++)
		{
			index[j] = i + j;

			tri_verts.push_back(m_vertices.at(m_indices.at(index[j])));
		}

		Vector3f normal = psmath::get_normal(TRIANGLE(tri_verts));

		m_axes.push_back(normal);
	}

	//need to delete duplicate axes

	build_matrix();
}

void CONVEX_HULL::update()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_update(&m_position,new Vector3f(),m_widget_selected_axis);

		build_matrix();
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_update(new Vector3f(),&m_size, m_widget_selected_axis);

		build_matrix();
	}
	else if(g_input_handler.m_key_held[VK_R])
	{
		pshelpers::rotate_widget_update(m_position,&m_rotation,m_widget_selected_axis);

		build_matrix();
	}
}

void CONVEX_HULL::render()
{
	glPushMatrix();

		glColor3f(1.0f,1.0f,0.0f);
		if(m_under_collision) glColor4f(0.5f,0.0f,0.0f,1.0f);

		m_orientation_matrix.multiply_with_gl_matrix();

		for(int i = 0; i < m_indices.size(); i+=3)
		{
			int index[3];

			for(int j = 0; j < 3; j++)
			{
				index[j] = i + j;
			}

			glBegin(GL_TRIANGLES);
				for(int j = 0; j < 3; j++)
				{
					glVertex3f(	m_vertices.at(m_indices.at(index[j])).x,
								m_vertices.at(m_indices.at(index[j])).y,
								m_vertices.at(m_indices.at(index[j])).z);
				}
			glEnd();
		}

	glPopMatrix();

	glColor3f(0.2f,0.0f,1.0f);

	for(int i = 0; i < m_vertices.size(); i++)
	{
		Vector3f vert_pos = m_orientation_matrix * m_vertices.at(i);
		pshelpers::cube(vert_pos, Vector3f(0.5f,0.5f,0.5f));
	}
}

void CONVEX_HULL::aux_render()
{
	if(g_input_handler.m_key_held[VK_T])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, TRANSLATE_WIDGET);
	}
	else if(g_input_handler.m_key_held[VK_E])
	{
		pshelpers::axis_widget_render(m_position, m_widget_selected_axis, SCALE_WIDGET);
	}
	else if(g_input_handler.m_key_held[VK_R])
	{
		pshelpers::rotate_widget_render(m_position, m_widget_selected_axis);
	}
}

void CONVEX_HULL::back_buffer_render()
{
	if((g_input_handler.m_key_held[VK_T]) || (g_input_handler.m_key_held[VK_E]))
	{
		pshelpers::axis_widget_picking(m_position, &m_widget_selected_axis);
	}
	else if(g_input_handler.m_key_held[VK_R])
	{
		pshelpers::rotate_widget_picking(m_position, &m_widget_selected_axis);
	}
}

void CONVEX_HULL::build_matrix()
{
	Matrix4 translation_matrix;
	Matrix4 scale_matrix;
	Matrix4 rotation_matrix[3];

	translation_matrix.create_translation(m_position);
	scale_matrix.create_scale(m_size);

	rotation_matrix[0].create_cardinal_rotation(X_AXIS,m_rotation.x); 
	rotation_matrix[1].create_cardinal_rotation(Y_AXIS,m_rotation.y); 
	rotation_matrix[2].create_cardinal_rotation(Z_AXIS,m_rotation.z); 

	m_orientation_matrix = 
		translation_matrix * rotation_matrix[0] * rotation_matrix[1] * rotation_matrix[2] * scale_matrix;

	m_rotation_matrix =  rotation_matrix[0] * rotation_matrix[1] * rotation_matrix[2];

}

