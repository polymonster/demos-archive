/*=========================================================*\
|	animated_model.cpp - animated model related classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Renderer
|				Coder	:	ADixon
|				Date	:	28/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "model.h"
#include "texture_manager.h"
#include "console.h"
#include "input.h"
#include "polyspoon_math.h"
#include "shader_manager.h"
#include "model_manager.h"
#include "timer.h"
#include <fstream>

/*======================== VARIABLES ======================*/

/*======================== FUNCTIONS ======================*/

/*=========================================================*\
| Animated Model Class - loads renders and updates
|						 skeletal animated models
\*=========================================================*/
AnimatedModel::~AnimatedModel()
{
	delete m_model;
	delete m_core_model;

	m_model = NULL;
	m_core_model = NULL;

	for(int i = 0; i < m_number_of_render_objects; i++)
	{
		delete m_vbo[i];
	}

	for(int i = 0; i < m_number_of_meshes; i++)
	{
		delete m_materials[i];
	}
}

AnimatedModel::AnimatedModel(char *filename) :
m_number_of_meshes( 0 ),
m_number_of_animations( 0 ),
m_number_of_render_objects( 0 )
{
	std::ifstream fin;

	fin.open(filename);

	if(fin.is_open())
	{
		g_console_p->dbgmsg( "\nloading animated model %s\n", filename);
	}
	else
	{
		g_console_p->dbgmsg( "could not find %s\n", filename);
		return;
	}

	std::string word;

	char file[1024];

	m_core_model = new CalCoreModel("yes");


	while(!fin.eof())
	{
		fin >> word;

		if(word == "skeleton")
		{
			fin >> file;

			if(!m_core_model->loadCoreSkeleton(file))
			{
				g_console_p->dbgmsg( "\tskeleton %s failed to load\n", file);
			}
			else
			{
				g_console_p->dbgmsg( "\tskeleton %s successfully loaded\n", file);
			}

		}
		else if(word == "mesh")
		{
			fin >> file;

			m_mesh_id[m_number_of_meshes] = m_core_model->loadCoreMesh(file);

			if(m_anim_id[m_number_of_meshes] == -1) 
			{
				g_console_p->dbgmsg( "\tmesh %d in %s failed to load\n", m_number_of_meshes, file);
			}
			else
			{
				g_console_p->dbgmsg( "\tmesh %s successfully loaded\n", file);
			}

			m_materials[m_number_of_meshes] = new Material();

			char texture_name[256] = { 0 };

			fin >> texture_name;
				
			if(strcmp(texture_name,"no_texture"))
			{
				m_materials[m_number_of_meshes]->load_texture(texture_name,DIFFUSE);
			}
			else
			{
				m_materials[m_number_of_meshes]->texture_id[DIFFUSE] = TextureDefaults::no_texture;
			}

			m_number_of_meshes++;

			//load textures
		}
		else if(word == "animation")
		{
			fin >> file;

			m_anim_id[m_number_of_animations] = m_core_model->loadCoreAnimation(file);

			if(m_anim_id[m_number_of_animations] == -1) 
			{
				g_console_p->dbgmsg( "\tanimation %d in %s failed to load\n", m_number_of_animations, filename);
			}
			else
			{
				g_console_p->dbgmsg( "\tanimation %s successfully loaded\n", file);
			}

			m_number_of_animations++;
		}
	}

	m_model = new CalModel(m_core_model);

	for(int i = 0; i < m_number_of_meshes; i++)
	{
		if(!m_model->attachMesh(m_mesh_id[i]))
		{
			g_console_p->dbgmsg( "\tmesh %d did not attach\n", i); 
		}
		else
		{
			g_console_p->dbgmsg( "\tmesh %d attached\n", i);
		}
	}

	m_model->setLodLevel(1.0f);

	create_render_object();
}

bool g_indexed = false;
void AnimatedModel::create_render_object()
{
	update(0);

	int vertex_count = 0;
	int face_count = 0;
	m_number_of_render_objects = 0;

	// get the renderer of the model
	CalRenderer *cal_renderer_p;
	cal_renderer_p = m_model->getRenderer();

	if(cal_renderer_p->beginRendering())
	{
		int mesh_count = cal_renderer_p->getMeshCount();

		for(int i = 0; i < mesh_count; i++)
		{
			int sub_mesh_count = cal_renderer_p->getSubmeshCount(i);

			for(int j = 0; j < sub_mesh_count; j++)
			{
				if(cal_renderer_p->selectMeshSubmesh(i, j))
				{
					m_vbo[m_number_of_render_objects] = new RenderObject(GL_STREAM_DRAW,GL_TRIANGLES);

					IndexArray vbo_indices;
					Vector3fArray vbo_vertices;
					Vector3fArray vbo_normals;
					Vector2fArray vbo_texture_coords;

					// get the faces of the submesh
					static CalIndex mesh_indices[50000][3];
					int f_count = cal_renderer_p->getFaces(&mesh_indices[0][0]);

					for(int k = 0; k < f_count; k++)
					{
						vbo_indices.push_back(mesh_indices[k][0]);
						vbo_indices.push_back(mesh_indices[k][1]);
						vbo_indices.push_back(mesh_indices[k][2]);
					}


					if(g_indexed)
					{
						// get the transformed vertices of the submesh
						static float mesh_verts[30000][3];
						int v_count = cal_renderer_p->getVertices(&mesh_verts[0][0]);

						for(int k = 0; k < v_count; k++)
						{
							//format x = cal_x, y = cal_z, z = -cal_y (rectify z-up from max)
							vbo_vertices.push_back(Vector3f(mesh_verts[k][0],mesh_verts[k][2],-mesh_verts[k][1]));
						}

						// get the transformed normals of the submesh
						static float mesh_normals[30000][3];
						cal_renderer_p->getNormals(&mesh_normals[0][0]);

						for(int k = 0; k < v_count; k++)
						{
							vbo_normals.push_back(Vector3f(mesh_normals[k][0],mesh_normals[k][2],-mesh_normals[k][1]));
						}

						// get the texture coordinates of the submesh
						static float texture_coords[30000][2];
						int t_count = cal_renderer_p->getTextureCoordinates(0, &texture_coords[0][0]);

						for(int k = 0; k < v_count; k++)
						{
							vbo_texture_coords.push_back(Vector2f(texture_coords[k][0],-texture_coords[k][1]));
						}

						Vector4fArray vbo_tangents = g_model_mgr_p->calculate_indexed_tangents(vbo_vertices, vbo_texture_coords, vbo_normals, vbo_indices);

						m_vbo[m_number_of_render_objects]->add_vertex_buffer(vbo_vertices);
						m_vbo[m_number_of_render_objects]->add_normal_buffer(vbo_normals);
						m_vbo[m_number_of_render_objects]->add_texcoord_buffer(vbo_texture_coords);
						m_vbo[m_number_of_render_objects]->add_tangent_buffer(vbo_tangents);
						m_vbo[m_number_of_render_objects]->add_index_buffer(vbo_indices);
					}
					else
					{
						// get the transformed vertices of the submesh
						static float mesh_verts[30000][3];
						int v_count = cal_renderer_p->getVertices(&mesh_verts[0][0]);

						// get the transformed normals of the submesh
						static float mesh_normals[30000][3];
						cal_renderer_p->getNormals(&mesh_normals[0][0]);

						// get the texture coordinates of the submesh
						static float texture_coords[30000][2];
						int t_count = cal_renderer_p->getTextureCoordinates(0, &texture_coords[0][0]);

						for(int i = 0; i < vbo_indices.size(); i++)
						{
							int index = vbo_indices.at(i);

							vbo_vertices.push_back(Vector3f(mesh_verts[index][0],mesh_verts[index][2],-mesh_verts[index][1]));
							vbo_normals.push_back(Vector3f(mesh_normals[index][0],mesh_normals[index][2],-mesh_normals[index][1]));
							vbo_texture_coords.push_back(Vector2f(texture_coords[index][0],-texture_coords[index][1]));
						}

						Vector4fArray vbo_tangents = g_model_mgr_p->calculate_tangents(vbo_vertices, vbo_texture_coords, vbo_normals);

						m_vbo[m_number_of_render_objects]->add_vertex_buffer(vbo_vertices);
						m_vbo[m_number_of_render_objects]->add_normal_buffer(vbo_normals);
						m_vbo[m_number_of_render_objects]->add_texcoord_buffer(vbo_texture_coords);
						m_vbo[m_number_of_render_objects]->add_tangent_buffer(vbo_tangents);
					}

					m_number_of_render_objects++;
				}
			}
		}
	}
}

void AnimatedModel::update_render_objects()
{
	int vertex_count = 0;
	int face_count = 0;
	int render_object_id = 0;

	CalRenderer *cal_renderer_p;
	cal_renderer_p = m_model->getRenderer();

	if(cal_renderer_p->beginRendering())
	{
		int mesh_count = cal_renderer_p->getMeshCount();

		for(int i = 0; i < mesh_count; i++)
		{
			int sub_mesh_count = cal_renderer_p->getSubmeshCount(i);

			for(int j = 0; j < sub_mesh_count; j++)
			{
				if(cal_renderer_p->selectMeshSubmesh(i, j))
				{
					static float mesh_verts[30000][3];
					static float normal_verts[30000][3];

					int v_count = cal_renderer_p->getVertices(&mesh_verts[0][0]);
					cal_renderer_p->getNormals(&normal_verts[0][0]);

					Vector3fArray vbo_vertices;
					Vector3fArray vbo_normals;

					if(g_indexed)
					{
						for(int k = 0; k < v_count; k++)
						{
							//format x = cal_x, y = cal_z, z = -cal_y (rectify z-up from max)
							vbo_vertices.push_back(Vector3f(mesh_verts[k][0],mesh_verts[k][2],-mesh_verts[k][1]));
							vbo_normals.push_back(Vector3f(normal_verts[k][0],normal_verts[k][2],-normal_verts[k][1]));
						}

						m_vbo[render_object_id]->remap_vertex_buffer(vbo_vertices);
						m_vbo[render_object_id]->remap_normal_buffer(vbo_normals);
					}
					else
					{
						static CalIndex mesh_indices[50000][3];
						int f_count = cal_renderer_p->getFaces(&mesh_indices[0][0]);

						for(int k = 0; k < f_count; k++)
						{
							for(int l = 0; l < 3; l++)
							{
								int index = mesh_indices[k][l];

								vbo_vertices.push_back(Vector3f(mesh_verts[index][0],mesh_verts[index][2],-mesh_verts[index][1]));
								vbo_normals.push_back(Vector3f(normal_verts[index][0],normal_verts[index][2],-normal_verts[index][1]));
							}
						}

						m_vbo[render_object_id]->remap_vertex_buffer(vbo_vertices);
						m_vbo[render_object_id]->remap_normal_buffer(vbo_normals);
					}
			
					render_object_id++;
				}
			}
		}
	}
}

void AnimatedModel::update(int id)
{
	if(g_game_timer.time_to_animate()) 
	{
		m_model->update(g_game_timer.get_game_time());
		update_render_objects();
	}
}

void AnimatedModel::set_state(int id){

	m_model->getMixer()->blendCycle(m_anim_id[id], 0.8f, 0.3f);
}

void AnimatedModel::clear_state(int id){

	m_model->getMixer()->clearCycle(m_anim_id[id], 0.3f);
}

void AnimatedModel::execute_state(int id){

	m_model->getMixer()->executeAction(m_anim_id[id], 0.3f, 1.5f);

}

void AnimatedModel::render()
{
	if(g_input_handler.m_key_pressed[VK_Z]) create_render_object();

	for(int i = 0; i < m_number_of_render_objects; i++)
	{
		g_texture_mgr.bind_texture( m_materials[i]->texture_id[DIFFUSE] , 0 );
		g_texture_mgr.bind_texture( TextureDefaults::normal_map, 1 );
		g_texture_mgr.bind_texture( TextureDefaults::specular_map, 2 );

		m_vbo[i]->render();
	}
}

void AnimatedModel::render_debug()
{
	int vertex_count = 0;
	int face_count = 0;
	m_number_of_render_objects = 0;

	// get the renderer of the model
	CalRenderer *cal_renderer_p;
	cal_renderer_p = m_model->getRenderer();

	if(cal_renderer_p->beginRendering())
	{
		int mesh_count = cal_renderer_p->getMeshCount();

		for(int i = 0; i < mesh_count; i++)
		{
			int sub_mesh_count = cal_renderer_p->getSubmeshCount(i);

			for(int j = 0; j < sub_mesh_count; j++)
			{
				if(cal_renderer_p->selectMeshSubmesh(i, j))
				{
					m_vbo[m_number_of_render_objects] = new RenderObject(GL_STREAM_DRAW,GL_TRIANGLES);

					IndexArray vbo_indices;
					Vector3fArray vbo_vertices;
					Vector3fArray vbo_normals;
					Vector2fArray vbo_texture_coords;

					// get the faces of the submesh
					static CalIndex mesh_indices[50000][3];
					int f_count = cal_renderer_p->getFaces(&mesh_indices[0][0]);

					for(int k = 0; k < f_count; k++)
					{
						vbo_indices.push_back(mesh_indices[k][0]);
						vbo_indices.push_back(mesh_indices[k][1]);
						vbo_indices.push_back(mesh_indices[k][2]);
					}

					// get the transformed vertices of the submesh
					static float mesh_verts[30000][3];
					int v_count = cal_renderer_p->getVertices(&mesh_verts[0][0]);

					// get the transformed normals of the submesh
					static float mesh_normals[30000][3];
					cal_renderer_p->getNormals(&mesh_normals[0][0]);

					// get the texture coordinates of the submesh
					static float texture_coords[30000][2];
					int t_count = cal_renderer_p->getTextureCoordinates(0, &texture_coords[0][0]);

					for(int i = 0; i < vbo_indices.size(); i++)
					{
						int index = vbo_indices.at(i);

						vbo_vertices.push_back(Vector3f(mesh_verts[index][0],mesh_verts[index][2],-mesh_verts[index][1]));
						vbo_normals.push_back(Vector3f(mesh_normals[index][0],mesh_normals[index][2],-mesh_normals[index][1]));
						vbo_texture_coords.push_back(Vector2f(texture_coords[index][0],-texture_coords[index][1]));
					}

					Vector4fArray vbo_tangents = g_model_mgr_p->calculate_tangents(vbo_vertices, vbo_texture_coords, vbo_normals);

					for(int i = 0; i < vbo_vertices.size(); i++)
					{
						Vector3f n = (Vector3f)vbo_normals.at(i);
						Vector3f v = (Vector3f)vbo_vertices.at(i);
						Vector4f t = (Vector4f)vbo_tangents.at(i);

						//normal
						glColor4f(0.0f,1.0f,0.0f,1.0f);
						glBegin(GL_LINES);
						glVertex3f(v.x,v.y,v.z);
						glVertex3f(
							v.x + n.x * 2,
							v.y + n.y * 2,
							v.z + n.z * 2);
						glEnd();

						//tangent
						glColor4f(1.0f,0.0f,0.0f,1.0f);
						glBegin(GL_LINES);
						glVertex3f(v.x,v.y,v.z);
						glVertex3f(
							v.x + t.x * 2,
							v.y + t.y * 2,
							v.z + t.z * 2);
						glEnd();
					}
				}
			}
		}
	}
}
