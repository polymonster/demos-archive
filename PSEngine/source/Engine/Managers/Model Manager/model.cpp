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

#include "model.h"
#include "texture_manager.h"
#include "console.h"
#include "shader_manager.h"
#include "render_object.h"
#include "physics_manager.h"
#include "helpers.h"
#include <fstream>

/*======================== VARIABLES ======================*/
const int max_joints = 78;

/*=========================================================*\
|	Model Class - Contains a collection of meshes
\*=========================================================*/
Model::Model()
{
	m_meshes.clear();
	m_AABB = new AABB3D( Vector3f( 0, 0, 0 ), Vector3f( 0, 0, 0 ) );

	m_skeleton = NULL;

	m_current_animation = 1;

	m_collision_shape_handle = -1;
}

Model::~Model()
{
	int meshes = m_meshes.size();
	for(int i = meshes - 1; i >= 0; i--)
	{
		delete m_meshes.at(i);
	}

	m_meshes.clear();

	int materials = m_materials.size();
	for(int i = materials - 1; i >= 0; i--)
	{
		delete m_materials.at(i);
	}

	m_materials.clear();

	if( m_AABB )
		delete m_AABB;
}

void Model::add_mesh(Mesh *mesh, int material_id)
{
	if(material_id != -1)
	{
		mesh->add_material(m_materials.at(material_id));
	}

	m_meshes.push_back(mesh);
}

void Model::add_material(Material *material)
{
	if(material != NULL)
	{
		m_materials.push_back(material);
	}
}

void Model::render()
{
	if(m_skins.size() > 0)
	{
		ShaderManager::instance()->set_uniform(Shaders::g_buffer_skinned,"axis_conversion", &g_max_to_gl_axis4);
	}

	int mesh_count = m_meshes.size();
	for(unsigned int i = 0; i < mesh_count; i++)
	{
		m_meshes[i]->render();
	}
}

void Model::render_debug()
{
	//skeleton debug
	glPushMatrix();

	if(m_skeleton)
	{
		m_skeleton->render_debug();
	}

	glPopMatrix();
}

int Model::find_material(std::string name)
{
	for(unsigned int i = 0; i < m_materials.size(); i++)
	{
		if(m_materials.at(i)->material_name == name)
		{
			return i;
		}
	}

	return -1;
}

void Model::create_collision_shape()
{
	Vector3f box_size = m_AABB->m_size;
	box_size.y = max(box_size.y, 1);

	m_collision_shape_handle = g_physics_mgr_p->add_collision_shape(box_size * 0.5);
}

void Model::calculate_aabb( void )
{
	Vector3f min = Vector3f( 9999.f, 9999.f, 9999.f );
	Vector3f max = Vector3f( -9999.f, -9999.f, -9999.f );

	for( int i = 0; i < m_meshes.size(); i++ )
	{
		Vector3fArray verts = m_meshes.at( i )->m_vertices;

		for( int j = 0; j < verts.size(); j++ )
		{
			if( min.x > verts[j].x ) min.x = verts[j].x;
			if( min.y > verts[j].y ) min.y = verts[j].y;
			if( min.z > verts[j].z ) min.z = verts[j].z;
			if( max.x < verts[j].x ) max.x = verts[j].x;
			if( max.y < verts[j].y ) max.y = verts[j].y;
			if( max.z < verts[j].z ) max.z = verts[j].z;
		}
	}

	m_AABB->m_size = max - min;
	m_AABB->m_position = ( max + min );
	m_AABB->m_position *= 0.5f;

	m_centre_of_mass_offset = m_AABB->m_position * -1;
	create_collision_shape();
}

void Model::add_skeleton( Skeleton *skeleton )
{
	m_skeleton = skeleton;
}

Joint * Model::find_joint( std::string name )
{
	return m_skeleton->find_joint(name);
}

void Model::add_animation( Animation *animation )
{
	m_animations.push_back(animation);

	m_animations.back()->calculate_frame_count();
}

void Model::update()
{
	if(m_animations.size() > 0)
	{
		if(g_input_handler.m_key_pressed[VK_L])
		{
			m_current_animation++;
		}

		if(m_current_animation >= m_animations.size()) m_current_animation = 0;
		m_animations[m_current_animation]->update();

		m_skeleton->update();

		int skincount = m_skins.size();
		for(int i = 0; i < skincount; i++)
		{
			m_skins[i]->update_joints();
		}
	}
}

void Model::add_skin(Skin *skin , std::string name)
{
	for(int i = 0; i < m_meshes.size(); i++)
	{
		if(m_meshes.at(i)->find_by_name(name))
		{
			m_meshes.at(i)->add_skin(skin, m_skeleton);
		}
	}

	bool exists = false;
	for(int i = 0; i < m_skins.size(); i++)
	{
		if(skin == m_skins[i])
		{
			exists = true;
		}
	}
	if(!exists) m_skins.push_back(skin);
}

void Model::create_clusters()
{
	for(int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes.at(i)->create_clusters();
	}
}

/*=========================================================*\
| MeshCluster Class - Contains a collection of verts attached to a maximum of 8 bones
\*=========================================================*/

MeshCluster::MeshCluster()
{
	m_vbo = new RenderObject(GL_STATIC_DRAW,GL_TRIANGLES);
	m_skin_p = NULL;
}

MeshCluster::~MeshCluster()
{
	delete m_vbo;
}

void MeshCluster::render()
{
	int active_shader = ShaderManager::instance()->active_shader();

	if(m_skin_p && (active_shader == Shaders::g_buffer_skinned || active_shader == Shaders::shadow_depth_skinned) )
	{
		m_joint_matrices.clear();

		int joint_size = m_joint_ids.size();
		
		for(int i = 0; i < /*max_joints*/ joint_size; i++)
		{
			Matrix4 mat;
			mat.identity();
			m_joint_matrices.push_back( mat );
		}

		for(int i = 0; i < joint_size; i++)
		{
			Matrix4 joint_matrix = m_skin_p->m_skinned_joints[ m_joint_ids[i] ]->m_joint_matrix;
			m_joint_matrices[i] = joint_matrix;
		}

		ShaderManager::instance()->set_uniform_array(active_shader,"joint_matrices", m_joint_matrices);
		ShaderManager::instance()->set_uniform(active_shader, "bind_shape_matrix", &m_skin_p->m_bind_shape_matrix);
	}

	m_vbo->render();
}

void MeshCluster::add_vertices(Vector3fArray vertices)
{
	m_vbo->add_vertex_buffer(vertices);
}

void MeshCluster::add_indices(IndexArray index_array)
{
	m_vbo->add_index_buffer(index_array);
}

void MeshCluster::add_texture_coords(Vector2fArray texture_coords)
{
	m_vbo->add_texcoord_buffer(texture_coords);
}

void MeshCluster::add_normals(Vector3fArray normals)
{
	m_vbo->add_normal_buffer(normals);
}

void MeshCluster::add_tangents( Vector4fArray tangents )
{
	m_vbo->add_tangent_buffer(tangents);
}

void MeshCluster::add_joint( int joint_id)
{
	bool exists = false;

	for(int i = 0; i < m_joint_ids.size(); i++)
	{
		if(m_joint_ids[i] == joint_id)
		{
			exists = true;
		}
	}

	if(!exists) m_joint_ids.push_back(joint_id);
}

void MeshCluster::remap_vertex_buffer( Vector3fArray verts )
{
	m_vbo->remap_vertex_buffer( verts );
}

void MeshCluster::remap_normal_buffer( Vector3fArray normals )
{
	m_vbo->remap_normal_buffer( normals );
}

void MeshCluster::remap_tangent_buffer( Vector4fArray tangents )
{
	m_vbo->remap_tangent_buffer( tangents );
}

void MeshCluster::add_vertex( Vector3f vertex )
{
	m_vertices.push_back(vertex);
}

void MeshCluster::add_texture_coord( Vector2f texture_coord )
{
	m_texture_coords.push_back(texture_coord);
}

void MeshCluster::add_normal( Vector3f normal )
{
	m_normals.push_back(normal);
}

void MeshCluster::add_tangent( Vector4f tangent )
{
	m_tangents.push_back(tangent);
}

void MeshCluster::build()
{
	add_vertices(m_vertices);
	add_normals(m_normals);
	add_tangents(m_tangents);
	add_texture_coords(m_texture_coords);

	for(int i = 0; i < 3; i++)
		m_vbo->add_weight_buffer(m_weights[i], m_joint_indices[i], i);
}

void MeshCluster::add_weights( std::vector<float> weights, std::vector<int> joint_ids)
{
	Vector4f *weights_vector = new Vector4f[3];
	Vector4f *joint_indices_vector = new Vector4f[3];

	for(int i = 0; i < 3; i++)
	{
		weights_vector[i] = Vector4f(Vector3f(0.0f,0.0f,0.0f),0.0f);
		joint_indices_vector[i] = Vector4f(Vector3f(0.0f,0.0f,0.0f),0.0f);
	}

	for(int i = 0; i < weights.size(); i++)
	{
		int weight_array_index = (int) i / 4;
		int weight_component_index = (int) i % 4;

		for(int j = 0; j < m_joint_ids.size(); j++)
		{
			if(m_joint_ids[j] == joint_ids[i] && weights[i] > 0.0f)
			{
				switch(weight_component_index)
				{
					case 0:
						weights_vector[ weight_array_index ].x = weights[i];
						joint_indices_vector[ weight_array_index ].x = j;
						break;
					case 1:
						weights_vector[ weight_array_index ].y = weights[i];
						joint_indices_vector[ weight_array_index ].y = j;
						break;
					case 2:
						weights_vector[ weight_array_index ].z = weights[i];
						joint_indices_vector[ weight_array_index ].z = j;
						break;
					case 3:
						weights_vector[ weight_array_index ].w = weights[i];
						joint_indices_vector[ weight_array_index ].w = j;
						break;
				}
			}
		}
	}

	for(int i = 0; i < 3; i++)
	{
		m_weights[i].push_back(weights_vector[i]);
		m_joint_indices[i].push_back(joint_indices_vector[i]);
	}

	delete joint_indices_vector;
	delete weights_vector;
}

/*=========================================================*\
| Mesh Class - Contains a collection of render objects
\*=========================================================*/

Mesh::Mesh()
{

	m_material = NULL;
	m_has_material = false;
}

Mesh::~Mesh()
{
	for(int i = 0; i < m_clusters.size(); i++)
	{
		delete m_clusters[i];
	}
}

void Mesh::add_vertices(Vector3fArray vertices)
{
	m_vertices = vertices;
}

void Mesh::add_indices(IndexArray index_array)
{
	m_indices = index_array;
}

void Mesh::add_texture_coords(Vector2fArray texture_coords)
{
	m_tex_coords = texture_coords;
}

void Mesh::add_normals(Vector3fArray normals)
{
	m_normals = normals;
}

void Mesh::add_tangents( Vector4fArray tangents )
{
	m_tangents = tangents;
}

void Mesh::add_material(Material *material)
{
	if(material != NULL)
	{
		m_material = material;
		m_has_material = true;
	}
}

void Mesh::render()
{
	if(m_has_material)
	{
		if(m_material->texture_id[DIFFUSE] > -1) g_texture_mgr.bind_texture( m_material->texture_id[DIFFUSE], 0);
		else g_texture_mgr.bind_texture( TextureDefaults::specular_map, 0);

		if(m_material->texture_id[NORMAL] > -1) g_texture_mgr.bind_texture( m_material->texture_id[NORMAL], 1);
		else g_texture_mgr.bind_texture( TextureDefaults::normal_map, 1);

		if(m_material->texture_id[SPECULAR] > -1) g_texture_mgr.bind_texture( m_material->texture_id[SPECULAR], 2);
		else  g_texture_mgr.bind_texture( TextureDefaults::specular_map, 2);

		if(m_material->shininess == 1.0f)
		{
			glColor4f(0,0,0,2.0f);
		}
		else
		{
			glColor4f(0,0,0,1.0f);
		}
	}

	int cluster_count = m_clusters.size();
	for(int i = 0; i < cluster_count; i++)
	{
		m_clusters[i]->render();
	}
}

void Mesh::add_skin( Skin *skin_p, Skeleton *skeleton_p)
{
	skin_p->fixup_joints( skeleton_p );

	m_skins.push_back(skin_p);
}

bool Mesh::find_by_name( std::string name )
{
	if(m_name == name) return true;

	return false;
}

void Mesh::set_name( std::string name )
{
	m_name = name;
}

Skin* Mesh::get_skin( int id )
{
	if(id >= 0 && id < m_skins.size())
		return m_skins.at(id);

	return NULL;
}

MeshCluster * Mesh::find_best_cluster( std::vector<int> joint_list , Skin *skin_p )
{
	//remove duplicates from the joint list
	std::vector<int> unique_joints;
	for(int i = 0; i < joint_list.size(); i++)
	{
		bool exists = false;
		for(int j = 0; j < unique_joints.size(); j++)
		{
			if(unique_joints[j] == joint_list[i])
			{
				exists = true;
			}
		}

		if(!exists)
		{
			unique_joints.push_back( joint_list[i] );
		}
	}

	//search through existing clusters for perfect match
	int target_joint_matches = unique_joints.size();

	for(int i = 0; i < m_clusters.size(); i++)
	{
		int found_matches = 0;

		for(int j = 0; j < m_clusters[i]->m_joint_ids.size(); j++)
		{
			for(int k = 0; k < target_joint_matches; k++)
			{
				if(m_clusters[i]->m_joint_ids[j] == unique_joints[k]) found_matches++;
			}
		}

		if(found_matches == target_joint_matches)
		{
			return m_clusters[i];
		}
	}

	//see if it can be squeezed into an existing cluster with existing matches
	MeshCluster *matched_cluster = NULL;
	int lowest_remaining = max_joints;

	for(int i = 0; i < m_clusters.size(); i++)
	{
		int remaining_slots = (max_joints - m_clusters[i]->m_joint_ids.size());
		int remaining_joints = target_joint_matches;

		for(int j = 0; j < m_clusters[i]->m_joint_ids.size(); j++)
		{
			for(int k = 0; k < target_joint_matches; k++)
			{
				if(m_clusters[i]->m_joint_ids[j] == unique_joints[k]) remaining_joints--;
			}
		}

		if( remaining_slots > remaining_joints && remaining_joints < lowest_remaining)
		{
			matched_cluster = m_clusters[i];
			lowest_remaining = remaining_joints;
		}
	}

	if(matched_cluster)
	{
		for(int i =  0; i < unique_joints.size(); i++)
		{
			matched_cluster->add_joint(unique_joints[i]);
		}

		return matched_cluster;
	}

	//squeeze in the bones into a suitible chunk with no existing matches
	int lowest_count = max_joints;
	for(int i = 0; i < m_clusters.size(); i++)
	{
		int remaining_slots = (max_joints - m_clusters[i]->m_joint_ids.size());

		if( remaining_slots > target_joint_matches && remaining_slots < lowest_count)
		{
			matched_cluster = m_clusters[i];
			lowest_count = remaining_slots;
		}
	}

	if(matched_cluster)
	{
		for(int i =  0; i < unique_joints.size(); i++)
		{
			matched_cluster->add_joint(unique_joints[i]);
		}

		return matched_cluster;
	}

	//add data to the a new cluster
	MeshCluster *new_cluster = new MeshCluster();

	new_cluster->m_skin_p = skin_p;

	for(int i =  0; i < unique_joints.size(); i++)
	{
		new_cluster->add_joint(unique_joints[i]);
	}

	m_clusters.push_back( new_cluster );
	return m_clusters.back();
}

void Mesh::create_clusters()
{
	if(m_skins.size() > 0)
	{
		std::vector<int> joint_list;
		SkinnedVertex *skinned_vert[3];
		MeshCluster *cluster;

		for(int i = 0; i < m_skins.size(); i++)
		{
			for(int j = 0; j < m_vertices.size(); j += 3)
			{
				skinned_vert[0] = m_skins[i]->m_skinned_vertices[ m_indices[j] ];
				skinned_vert[1] = m_skins[i]->m_skinned_vertices[ m_indices[j + 1] ];
				skinned_vert[2] = m_skins[i]->m_skinned_vertices[ m_indices[j + 2] ];

				joint_list.clear();
				for(int k = 0; k < 3; k++)
				{
					for(int l = 0; l < skinned_vert[k]->m_joint_ids.size(); l++)
					{
						if(skinned_vert[k]->m_weights[l] > 0.0f)
						{
							joint_list.push_back( skinned_vert[k]->m_joint_ids[l] );
						}
					}
				}

				cluster = find_best_cluster( joint_list, m_skins[i]);

				for(int k = 0; k < 3; k++)
				{
					cluster->add_vertex(m_vertices[j + k]);
					cluster->add_normal(m_normals[j + k]);
					cluster->add_tangent(m_tangents[j + k]);
					cluster->add_texture_coord(m_tex_coords[j + k]);
					cluster->add_weights(skinned_vert[k]->m_weights, skinned_vert[k]->m_joint_ids);
				}
			}
		}

		build_clusters();
	}
	else
	{
		//use 1 cluster
		MeshCluster *new_cluster = new MeshCluster();

		new_cluster->add_vertices(m_vertices);
		new_cluster->add_normals(m_normals);
		new_cluster->add_tangents(m_tangents);
		new_cluster->add_texture_coords(m_tex_coords);

		m_clusters.push_back(new_cluster);
	}
}

void Mesh::remap_vertex_buffer( Vector3fArray verts )
{
	m_clusters[0]->remap_vertex_buffer(verts);
}

void Mesh::remap_normal_buffer( Vector3fArray verts )
{
	m_clusters[0]->remap_normal_buffer(verts);
}

void Mesh::remap_tangent_buffer( Vector4fArray verts )
{
	m_clusters[0]->remap_tangent_buffer(verts);
}

void Mesh::build_clusters()
{
	for(int i = 0; i < m_clusters.size(); i++)
	{
		m_clusters[i]->build();
	}
}
/*=========================================================*\
| Material Class - Stores textures and material information
\*=========================================================*/

Material::Material()
{
	texture_id[3] = -1;
	transparency = -1;
	shininess = -1;
	illumination_model = -1;

	material_name = "not_loaded";
	texture_name = "not_loaded";
}

Material::~Material()
{
	//delete textures
}


Material::Material(char *filename)
{
	material_name = filename;
}

void Material::load_texture( char *filename,int map_type)
{
	texture_id[map_type] = g_texture_mgr.load_texture(filename);
	texture_name = filename;
}


Joint* Skeleton::find_joint( std::string name )
{
	for(int i = 0; i < m_joints.size(); i++)
	{
		if(m_joints[i]->find_by_name( name ))
		{
			return m_joints[i];
		}

		if(m_joints[i]->find_by_target( name ))
		{
			return m_joints[i];
		}
	}

	return NULL;
}

void Skeleton::add_joint( Joint *j )
{
	m_joints.push_back(j);
}
void Skeleton::render_debug()
{
	Matrix4 axis_system;
	axis_system.create_axis_swap(Vector3f(1.0f,0.0f,0.0f) ,Vector3f(0.0f,0.0f,-1.0f), Vector3f(0.0f, 1.0f,0.0f));

	glColor4f(0.0f,1.0f,0.0f,1.0f);

	glPushMatrix();

	axis_system.multiply_with_gl_matrix();

	Matrix4 prev;
	bool first = true;

	for(int i = 0; i < m_joints.size(); i++)
	{
		glColor4f(0.0f,1.0f,0.0f,1.0f);

		glPushMatrix();

			m_joints.at(i)->m_world_matrix.multiply_with_gl_matrix();
			pshelpers::cube(Vector3f(0.0f,0.0f,0.0f), Vector3f(1.0f,1.0f,1.0f));

		glPopMatrix();

		if(m_joints.at(i)->m_parent_p)
		{
			Matrix4 current = m_joints.at(i)->m_world_matrix;
			Matrix4 prev = m_joints.at(i)->m_parent_p->m_world_matrix;

			glColor4f(1.0f,0.0f,1.0f,1.0f);

			glBegin(GL_LINES);
				glVertex3f(prev.get_translation().x,prev.get_translation().y,prev.get_translation().z);
				glVertex3f(current.get_translation().x,current.get_translation().y,current.get_translation().z);
			glEnd();
		}
	}

	glPopMatrix();
}

void Skeleton::update_recursive(Joint *joint_p)
{
	if(joint_p->m_parent_p)
	{
		joint_p->m_world_matrix = joint_p->m_parent_p->m_world_matrix * joint_p->m_local_matrix;
	}
	else
	{
		joint_p->m_world_matrix = joint_p->m_local_matrix;
	}

	int child_count = joint_p->m_children_p.size();
	for(int i = 0; i < child_count; i++)
	{
		update_recursive(joint_p->m_children_p[i]);
	}
}

void Skeleton::update()
{
	//find root joints
	std::vector<Joint *> roots;
	int joint_count = m_joints.size();
	for(int i = 0; i < joint_count; i++)
	{
		if(m_joints[i]->is_root())
		{
			roots.push_back( m_joints[i] );
		}
	}

	int root_count = roots.size();
	for(int i = 0; i < root_count; i++)
	{
		update_recursive(roots[i]);
	}

}


bool Joint::find_by_name( std::string name )
{
	if(m_name == name)
	{
		return true;
	}

	return false;
}

bool Joint::find_by_target( std::string name )
{
	if(m_target_name == name)
	{
		return true;
	}

	return false;
}

Joint::Joint( std::string name, std::string target_name, Matrix4 matrix, Joint* parent )
{
	m_name = name;
	m_target_name = target_name;
	m_local_matrix = matrix;
	m_parent_p = parent;

	if(m_parent_p)
		m_parent_p->add_child(this);
}

void Joint::add_child( Joint *child_p )
{
	m_children_p.push_back(child_p);
}

bool Joint::is_root()
{
	if(!m_parent_p) return true;

	return false;
}

void Animation::add_channel( AnimationChannel* channel )
{
	m_channels.push_back( channel );
}

void Animation::update()
{
	int channel_count = m_channels.size();
	for(int i = 0; i < channel_count; i++)
	{
		m_channels[i]->set_joint_matrix(m_current_frame);
	}

	m_current_frame = m_current_frame + 1 > m_end ? m_start : m_current_frame + 1;
}

Animation::Animation()
{
	m_current_frame = 0;
}

void Animation::calculate_frame_count()
{
	m_start = 0;

	int frame_count = 0;

	for(int i = 0; i < m_channels.size(); i++)
	{
		if(i == 0)
		{
			frame_count = m_channels.at(i)->get_frame_count();
		}
		else
		{
			int c = m_channels.at(i)->get_frame_count();

			if(c != frame_count)
			{
				g_console_p->dbgmsg("animation has mismatched frame counts\n");

				if(c < frame_count)
				{
					frame_count = c;
				}
			}
		}
	}

	m_end = frame_count - 1;
}

void AnimationChannel::set_joint_matrix( int frame )
{
	m_joint_p->m_local_matrix = m_frame_matrix[frame];
}

void AnimationChannel::add_matrix( Matrix4 mat )
{	
	m_frame_matrix.push_back( mat );
}

AnimationChannel::AnimationChannel( Joint *joint )
{
	m_joint_p = joint;
}

int AnimationChannel::get_frame_count()
{
	return m_frame_matrix.size();
}

void Skin::add_skinned_joint( SkinnedJoint *sj )
{
	m_skinned_joints.push_back(sj);
}

void Skin::add_skinned_vertex( SkinnedVertex *sv )
{
	m_skinned_vertices.push_back(sv);
}

void Skin::fixup_joints( Skeleton *skeleton_p )
{
	for(int i = 0; i < m_skinned_joints.size(); i++)
	{
		m_skinned_joints[i]->fixup_joints( skeleton_p );
	}
}

void Skin::update_joints()
{
	int joint_count = m_skinned_joints.size();
	for(int i = 0; i < joint_count; i++)
	{
		SkinnedJoint *sj = m_skinned_joints[i];
		sj->m_joint_matrix = sj->m_joint_p->m_world_matrix * sj->m_inverse_bind_matrix;
	}
}

void SkinnedVertex::add_joint( int joint_id, float weight )
{
	m_joint_ids.push_back(joint_id);
	m_weights.push_back(weight);
}

SkinnedJoint::SkinnedJoint( Matrix4 inv_bind, std::string joint_name )
{
	m_inverse_bind_matrix = inv_bind;
	m_joint_name = joint_name;
	m_joint_p = NULL;
}

void SkinnedJoint::fixup_joints(Skeleton *skeleton_p)
{
	m_joint_p = skeleton_p->find_joint(m_joint_name);
}


//software skinning referenece
/*for( int i = 0; i < m_meshes.size(); i++ )
{
Skin *skin = m_meshes.at( i )->get_skin(0);

if(skin)
{		
Vector3fArray trans_verts = m_meshes.at(i)->m_vertices;
Vector3fArray trans_norms = m_meshes.at(i)->m_normals;
Vector4fArray trans_tangents = m_meshes.at(i)->m_tangents;

int vert_count = m_meshes.at(i)->m_vertices.size();

for(int j = 0; j < vert_count; j++)
{
int index = m_meshes[i]->m_indices[j];

Vector3f t = Vector3f(trans_tangents[j].x, trans_tangents[j].y, trans_tangents[j].z);
trans_verts[j] = skin->vertex_transform(index, trans_verts[j], trans_norms[j], t);

trans_tangents.push_back(Vector4f(t,1.0f));
}

m_meshes.at( i )->remap_vertex_buffer(trans_verts);
m_meshes.at( i )->remap_normal_buffer(trans_norms);
m_meshes.at( i )->remap_tangent_buffer(trans_tangents);
}
}

Vector3f Skin::vertex_transform( int id , Vector3f v, Vector3f &n, Vector3f &t)
{
Vector3f total = Vector3f(0.0f,0.0f,0.0f);
Vector3f totaln = Vector3f(0.0f,0.0f,0.0f);
Vector3f totalt = Vector3f(0.0f,0.0f,0.0f);

Matrix4 axis_system;
axis_system.create_axis_swap(Vector3f(1.0f,0.0f,0.0f) ,Vector3f(0.0f,0.0f,-1.0f), Vector3f(0.0f, 1.0f,0.0f));

for(int i = 0; i < m_skinned_vertices.at( id )->m_joint_ids.size(); i++)
{
int joint_id = m_skinned_vertices.at( id )->m_joint_ids.at(i);

Matrix4 ibm = m_skinned_joints.at( joint_id )->m_inverse_bind_matrix;
Matrix4 jm = m_skinned_joints.at( joint_id )->m_joint_p->m_world_matrix;
float weight = m_skinned_vertices.at( id )->m_weights.at( i );

Matrix4 transform_matrix = jm * ibm;

Vector3f vres = m_bind_shape_matrix * v;
Vector3f nres = m_bind_shape_matrix.get_orientation() * n;
Vector3f tres = m_bind_shape_matrix.get_orientation() * t;

vres = transform_matrix * vres;
nres = transform_matrix.get_orientation() * nres;
tres = transform_matrix.get_orientation() * tres;

total += vres * weight;
totaln += nres * weight;
totalt += tres * weight;
}

n = axis_system * totaln;
t = axis_system * totalt;
return axis_system * total;
}

*/
