/*=========================================================*\
|	render_object.cpp - VBO's stuff
|-----------------------------------------------------------|
|				Project :	PolySpoon Renderer
|				Coder	:	ADixon
|				Date	:	26/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "render_object.h"
#include "shader_manager.h"

/*======================== VARIABLES ======================*/

/*=========================================================*\
|	Render Object Class - Contains a collection of VBO's
\*=========================================================*/
VertexBufferContainer::VertexBufferContainer() :
vertex( -1 ),
normal( -1 ),
index( -1 ),
tangent( -1 )
{ 
	for(int i = 0; i < 8; i++) texcoord[i] = -1;
}

RenderObject::RenderObject(int draw_mode, int primitive_type) :
m_has_vertices( false ), 
m_has_indices( false ),
m_has_normals( false ), 
m_has_tangents( false ),
m_has_weights( false ),
m_number_of_texture_coords( 0 ),
m_number_of_vertices( 0 ),
m_number_of_indices( 0 ),
m_draw_mode( draw_mode ),
m_primitive_type( primitive_type )
{	}

RenderObject::~RenderObject()
{
	glDeleteBuffersARB( 1, &m_buffers.vertex );
	glDeleteBuffersARB( 1, &m_buffers.normal );
	glDeleteBuffersARB( 1, &m_buffers.tangent );
	glDeleteBuffersARB( 1, &m_buffers.index );

	for(int i = 0; i < 3; i++)
	{
		glDeleteBuffersARB( 1, &m_buffers.weights[i] );
		glDeleteBuffersARB( 1, &m_buffers.joint_indices[i] );
	}

	glDeleteBuffersARB( 1, &m_buffers.index );


	for(int i = 0; i < 8; i++)
	{
		glDeleteBuffersARB( 1, &m_buffers.texcoord[i] );
	}
}

void RenderObject::add_vertex_buffer(Vector3fArray vertices)
{
	m_has_vertices = true;

	glGenBuffersARB(1, &m_buffers.vertex);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.vertex);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 3 * vertices.size(), &vertices[0], m_draw_mode);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	m_number_of_vertices = vertices.size();

}

void RenderObject::add_texcoord_buffer(Vector2fArray texcoords)
{
	glGenBuffersARB(1, &m_buffers.texcoord[m_number_of_texture_coords]);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.texcoord[m_number_of_texture_coords]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 2 * texcoords.size(), &texcoords[0], m_draw_mode);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	m_number_of_texture_coords++;
}

void RenderObject::add_normal_buffer(Vector3fArray normals)
{
	m_has_normals = true;

	glGenBuffersARB(1, &m_buffers.normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.normal);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 3 * normals.size(), &normals[0], m_draw_mode);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void RenderObject::add_tangent_buffer( Vector4fArray tangents )
{
	m_has_tangents = true;

	glGenBuffersARB(1, &m_buffers.tangent);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.tangent);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 4 * tangents.size(), &tangents[0], m_draw_mode);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}



void RenderObject::add_index_buffer(unsigned int *indices, int index_count)
{
	m_has_indices = true;

	glGenBuffersARB(1, &m_buffers.index);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_buffers.index);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * index_count, &indices[0], m_draw_mode);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_number_of_indices = index_count;
}

void RenderObject::add_index_buffer(std::vector< unsigned int > index_array)
{
	m_has_indices = true;

	glGenBuffersARB(1, &m_buffers.index);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_buffers.index);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_array.size(), &index_array[0], m_draw_mode);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_number_of_indices = index_array.size();
}

void RenderObject::remap_vertex_buffer(Vector3fArray vertices)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.vertex);

	Vector3f *vertex_buffer_p = (Vector3f *) glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	if(vertex_buffer_p)
	{
		for(unsigned int i = 0; i < vertices.size(); i++)
		{
			*vertex_buffer_p++ = vertices.at(i);
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

}

void RenderObject::get_vertex_buffer( Vector3fArray *vertices )
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.vertex);

	Vector3f *vertex_buffer_p = (Vector3f *) glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_ONLY_ARB);

	if(vertex_buffer_p)
	{
		vertices->clear();
		for(unsigned int i = 0; i < m_number_of_vertices; i++)
		{
			vertices->push_back( *vertex_buffer_p++ );
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}


void RenderObject::remap_normal_buffer(Vector3fArray normals)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.normal);

	Vector3f *normal_buffer_p = (Vector3f *) glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	if(normal_buffer_p)
	{
		for(unsigned int i = 0; i < normals.size(); i++)
		{
			*normal_buffer_p++ = normals.at(i);
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}

void RenderObject::remap_tangent_buffer(Vector4fArray tangents)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.tangent);

	Vector4f *tangents_buffer_p = (Vector4f *) glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	if(tangents_buffer_p)
	{
		for(unsigned int i = 0; i < tangents.size(); i++)
		{
			*tangents_buffer_p++ = tangents.at(i);
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}

void RenderObject::render()
{	
	int active_shader = ShaderManager::instance()->active_shader();

	//just vertices for now
	if(m_has_vertices)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.vertex);
		glVertexPointer(3, GL_FLOAT, 0, 0);
			
		if(m_has_normals)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.normal);
			glNormalPointer(GL_FLOAT, 0, 0); 
		}

		if(m_number_of_texture_coords > 0)
		{
			for(int i = 0; i < m_number_of_texture_coords; i++)
			{
				glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.texcoord[i]);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
			}
		}

		if(m_has_tangents)
		{
			if(active_shader != -1 && active_shader == Shaders::g_buffer || active_shader == Shaders::g_buffer_skinned)
			{
				int loc = ShaderManager::instance()->get_attribute_location(active_shader, "tangent");
				glEnableVertexAttribArrayARB(loc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.tangent);
				glVertexAttribPointerARB(loc,4,GL_FLOAT,0,0,0);
			}
		}

		if(m_has_weights)
		{
			if(active_shader != -1)
			{
				for(int i = 0; i < 3; i++)
				{
					char name[64];
					char name2[64];
					sprintf(name,"bone_weights%d",i);
					sprintf(name2,"bone_indices%d",i);

					int loc = ShaderManager::instance()->get_attribute_location(active_shader, name);
					glEnableVertexAttribArrayARB(loc);

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.weights[i]);
					glVertexAttribPointerARB(loc,4,GL_FLOAT,0,0,0);

					int loc2 = ShaderManager::instance()->get_attribute_location(active_shader, name2);
					glEnableVertexAttribArrayARB(loc2);

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.joint_indices[i]);
					glVertexAttribPointerARB(loc2,4,GL_FLOAT,0,0,0);
				}
			}
		}

		if(m_has_indices)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_buffers.index);

			glDrawElements(m_primitive_type, m_number_of_indices, GL_UNSIGNED_INT, 0);	
		}
		else
		{
			glDrawArrays(m_primitive_type, 0, m_number_of_vertices);
		}
	}

	if(m_has_vertices) 
	{
		glDisableClientState(GL_VERTEX_ARRAY );
	}

	if(m_has_normals) 
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(m_has_indices)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if(m_number_of_texture_coords > 0)
	{
		for(int i = 0; i < m_number_of_texture_coords; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	if(m_has_weights)
	{
		if(active_shader != -1)
		{
			for(int i = 0; i < 3; i++)
			{
				char name[64];
				char name2[64];
				sprintf(name,"bone_weights%d",i);
				sprintf(name2,"bone_indices%d",i);

				int loc = ShaderManager::instance()->get_attribute_location(active_shader, name);
				glDisableVertexAttribArrayARB(loc);

				int loc2 = ShaderManager::instance()->get_attribute_location(active_shader, name2);
				glDisableVertexAttribArrayARB(loc2);
			}
		}
	}

	if(m_has_tangents)
	{
		if(active_shader != -1 && active_shader == Shaders::g_buffer || active_shader == Shaders::g_buffer_skinned)
		{
			int loc = ShaderManager::instance()->get_attribute_location(active_shader, "tangent");
			glDisableVertexAttribArrayARB(loc);
		}
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void RenderObject::add_weight_buffer( Vector4fArray weights, Vector4fArray joint_indices, int index )
{
	m_has_weights = true;

	glGenBuffersARB(1, &m_buffers.weights[index]);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.weights[index]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 4 * weights.size(), &weights[0], m_draw_mode);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glGenBuffersARB(1, &m_buffers.joint_indices[index]);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffers.joint_indices[index]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 4 * joint_indices.size(), &joint_indices[0], m_draw_mode);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}