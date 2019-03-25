/*=========================================================*\
|	render_object.h - model related classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Renderer
|				Coder	:	ADixon
|				Date	:	26/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _RENDER_OBJECT_H
#define _RENDER_OBJECT_H

/*======================== INCLUDES =======================*/

#include "window.h"
#include <vector.h>

/*======================== CONSTANTS ======================*/


/*========================= CLASSES =======================*/

typedef std::vector< float > WeightArray;

class VertexBufferContainer
{
	public:

		VertexBufferContainer();

		unsigned int vertex;
		unsigned int texcoord[8];
		unsigned int tangent;
		unsigned int normal;
		unsigned int index;
		unsigned int weights[4];
		unsigned int joint_indices[4];
};

class RenderObject
{

public:

	RenderObject(int draw_mode = GL_STATIC_DRAW_ARB, int primitive_type = GL_TRIANGLES);
	~RenderObject();

	void add_vertex_buffer(Vector3fArray vertices);
	void add_texcoord_buffer(Vector2fArray texcoords);
	void add_normal_buffer(Vector3fArray normals);
	void add_tangent_buffer(Vector4fArray tangents);
	void add_weight_buffer(Vector4fArray weights, Vector4fArray joint_indices, int index);

	void remap_vertex_buffer(Vector3fArray vertices);
	void remap_normal_buffer(Vector3fArray normals);
	void remap_tangent_buffer(Vector4fArray tangents);

	void add_index_buffer(unsigned int *indices, int index_count);
	void add_index_buffer(std::vector< unsigned int > index_array);

	void render();

	void get_vertex_buffer( Vector3fArray *vertices );

private:

	bool m_has_vertices;
	bool m_has_normals;
	bool m_has_indices;
	bool m_has_tangents;
	bool m_has_weights;

	int m_number_of_texture_coords;
	int m_number_of_vertices;
	int m_number_of_indices;

	VertexBufferContainer m_buffers;

	int m_draw_mode;
	int m_primitive_type;
};


/*================== EXTERNAL VARIABLES ===================*/

#endif //_RENDER_OBJECT_H