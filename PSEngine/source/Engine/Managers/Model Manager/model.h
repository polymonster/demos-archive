/*=========================================================*\
|	model.h - model related classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Input
|				Coder	:	ADixon
|				Date	:	25/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _MODEL_H
#define _MODEL_H

/*======================== INCLUDES =======================*/

#include "window.h"
#include "render_object.h"
#include "bounding_volumes.h"
#include <vector>
#include <string>

//Cal3d for skeletal animation
#include "cal3d\\cal3d.h"

#ifdef _DEBUG

	#ifdef _ENGINE

		#pragma comment ( lib, "..\\..\\Libs\\Debug Lib\\cal3d_d.lib" )

	#else

		#pragma comment ( lib, "..\\..\\Libs\\Debug Lib\\cal3d_d.lib" )

	#endif

#else

	#ifdef _ENGINE

		#pragma comment ( lib, "..\\..\\Libs\\Release Lib\\cal3d.lib" )

	#else

		#pragma comment ( lib, "..\\..\\Libs\\Release Lib\\cal3d.lib" )

	#endif

#endif


/*======================== CONSTANTS ======================*/

#define MAX_TEXTURES 8
#define MAX_ANIMATIONS 32
#define MAX_SUB_MESHES 32

#define DIFFUSE 0
#define NORMAL 1
#define SPECULAR 2

/*========================= CLASSES =======================*/

class Mesh;
class Model;
class Material;
class Skeleton;
class Joint;
class SkinnedJoint;
class SkinnedVertex;
class Skin;
class MeshCluster;

typedef std::vector< unsigned int > IndexArray;

class SkinnedVertex
{
	public:

		SkinnedVertex(){};
		void add_joint(int joint_id, float weight);

		std::vector<int> m_joint_ids;
		std::vector<SkinnedJoint> m_skinned_joints;
		std::vector<float> m_weights;
};

class SkinnedJoint
{
	public:

		SkinnedJoint(Matrix4 inv_bind, std::string joint_name);
		void fixup_joints(Skeleton *skeleton_p);

		Matrix4 m_inverse_bind_matrix;
		Matrix4 m_joint_matrix;
		Joint *m_joint_p;
		std::string m_joint_name;

};

class Skin
{
	public:

		Skin(){};
		void add_skinned_joint(SkinnedJoint *sj);
		void add_skinned_vertex(SkinnedVertex *sv);
		void fixup_joints(Skeleton *skeleton_p);
		Vector3f vertex_transform(int id, Vector3f v, Vector3f &n, Vector3f &t);
		void update_joints();

		Matrix4 m_bind_shape_matrix;
		Vector3f m_bind_shape_offset;

		std::vector<SkinnedJoint*> m_skinned_joints;
		std::vector<SkinnedVertex*> m_skinned_vertices;
};

class AnimationChannel
{
	public:

		AnimationChannel(Joint *joint);

		void set_joint_matrix(int frame);
		void add_matrix(Matrix4 mat);

		int get_frame_count();

	private:

		std::vector<Matrix4> m_frame_matrix;
		std::vector<float> m_times;

		Joint *m_joint_p;
};

class Animation
{
	public:

		Animation();

		void add_channel(AnimationChannel* channel);
		void update();
		void calculate_frame_count();

	private:

		std::vector<AnimationChannel*> m_channels;
		int m_current_frame;
		int m_start;
		int m_end;
};

class Joint
{
	public:

		Joint(){};
		Joint(std::string name, std::string target_name, Matrix4 matrix, Joint* parent);
		bool find_by_name(std::string name);
		bool find_by_target( std::string name );
		bool is_root();
		void add_child(Joint *child_p);

		Matrix4 m_local_matrix;
		Matrix4 m_world_matrix;
		Joint* m_parent_p;
		std::vector<Joint *> m_children_p;
		std::string m_name;
		std::string m_target_name;
};

class Skeleton
{
	public:
		
		Skeleton(){};
		~Skeleton();

		void add_joint(Joint *j);
		Joint* find_joint(std::string name);
		void render_debug();

		void update();
		void update_recursive(Joint *joint_p);

	private:

		std::vector<Joint*> m_joints;
};

class Model
{
	public:

		Model();
		~Model();

		void add_mesh(Mesh *mesh, int material_id);
		void add_skeleton(Skeleton *skeleton);
		void add_material(Material *material);
		void add_animation(Animation *animation);
		void add_skin(Skin *skin, std::string name);

		void create_collision_shape();
		void create_clusters();

		void render();
		void render_debug();
		void update();

		int find_material(std::string name);

		void calculate_aabb( void );

		int collision_shape_handle() const { return m_collision_shape_handle; }
		Vector3f centre_of_mass_offset() const { return m_centre_of_mass_offset; }
		AABB3D* get_aabb() const {return m_AABB; }

		Joint *find_joint(std::string name);

	private: 

		std::vector< Mesh* > m_meshes;
		std::vector< Material* > m_materials;
		std::vector< Animation* > m_animations;
		std::vector< Skin* > m_skins;

		Skeleton	*m_skeleton;

		AABB3D		*m_AABB;
		int			m_collision_shape_handle;
		Vector3f	m_centre_of_mass_offset;

		int			m_current_animation;
};

class MeshCluster
{
public:

	MeshCluster();
	~MeshCluster();

	void add_vertices(Vector3fArray vertices);
	void add_vertex(Vector3f vertex);

	void add_texture_coords(Vector2fArray texture_coords);
	void add_texture_coord(Vector2f texture_coord);

	void add_normals(Vector3fArray normals);
	void add_normal(Vector3f normal);

	void add_tangents(Vector4fArray tangents);
	void add_tangent(Vector4f tangent);

	void add_weights(std::vector<float> weights, std::vector<int> joint_ids);

	void add_indices(IndexArray index_array);

	void add_bone_matrix(Matrix4Array matrices);
	void add_joint(int joint_id);
	void render();

	RenderObject*	get_vbo( void )				{ return m_vbo; }

	void remap_vertex_buffer(Vector3fArray verts);
	void remap_normal_buffer(Vector3fArray normals);
	void remap_tangent_buffer(Vector4fArray tangents);

	void build();

	std::vector<int> m_joint_ids;
	Skin *m_skin_p;
	Matrix4Array m_joint_matrices;

private:

	RenderObject *m_vbo;

	Vector3fArray m_vertices;
	Vector3fArray m_normals;
	Vector4fArray m_tangents;
	Vector2fArray m_texture_coords;

	Vector4fArray m_weights[3];
	Vector4fArray m_joint_indices[3];
};

class Mesh
{
	public:

		Mesh();
		~Mesh();

		void add_vertices(Vector3fArray vertices);
		void add_texture_coords(Vector2fArray texture_coords);
		void add_normals(Vector3fArray normals);
		void add_tangents(Vector4fArray tangents);
		void add_indices(IndexArray index_array);

		void add_material(Material *material);

		void add_skin(Skin *skin_p, Skeleton *skeleton_p);
		Skin* get_skin(int id);
		void update_skins();

		bool find_by_name(std::string name);
		void set_name(std::string name);

		void create_clusters();
		MeshCluster *find_best_cluster(std::vector<int> joint_list, Skin *skin_p);
		void build_clusters();

		void remap_vertex_buffer(Vector3fArray verts);
		void remap_normal_buffer(Vector3fArray normals);
		void remap_tangent_buffer(Vector4fArray tangents);

		void render();

		IndexArray m_indices;
		Vector3fArray m_vertices;
		Vector2fArray m_tex_coords;
		Vector3fArray m_normals;
		Vector4fArray m_tangents;

	private:

		Material *m_material;
		bool m_has_material;
		std::string m_name;

		std::vector<Skin *> m_skins;
		std::vector<MeshCluster *> m_clusters;
};

class Material
{
	public:

		Material();
		~Material();
		Material(char *filename);

		void load_texture(char *filename,int map_type);

		std::string texture_name;
		std::string material_name;

		int texture_id[3];
		int shader_id;

		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;
		float transparency;
		float shininess;
		float illumination_model;
};

class AnimatedModel
{

public:

	AnimatedModel(char *filename);
	~AnimatedModel();

	void create_render_object();

	void render();

	void render_debug();

	void update(int id);
	void update_render_objects();

	void set_state(int id);
	void execute_state(int id);
	void clear_state(int id);

private:

	CalCoreModel *m_core_model;
	CalModel *m_model;

	int m_number_of_animations;
	int m_number_of_meshes;
	int m_number_of_render_objects;

	int m_mesh_id[MAX_SUB_MESHES];
	Material *m_materials[MAX_SUB_MESHES];

	int m_anim_id[MAX_ANIMATIONS];

	RenderObject *m_vbo[MAX_SUB_MESHES];

};


/*================== EXTERNAL VARIABLES ===================*/


#endif //_MODEL_H