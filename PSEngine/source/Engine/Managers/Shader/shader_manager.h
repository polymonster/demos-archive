#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

#include "common.h"
#include <map>
#include <string>

#define MAX_SHADERS 64

enum e_types 
{
	glsl_int = 0, 
	glsl_vec3,
	glsl_vec4,
	glsl_vec2,
	glsl_mat4,
	glsl_mat3
};

enum e_shader
{
	glsl_vertex = 0,
	glsl_fragment
};

enum e_scopes
{
	glsl_uniform = 0,
	glsl_attribute
};

class ConstantTableEntry
{
public:
	int location;
	std::string name;
	e_types type;
	e_shader shader;
	e_scopes scope;
};

class Shaders
{
public:

	static int g_buffer;
	static int g_buffer_skinned;
	static int punchout;
	static int deferred;
	static int gaussian;
	static int shadow_depth;
	static int shadow_depth_skinned;
	static int bright_pass;
	static int hdr_final;
	static int luminance_initial;
	static int luminance_iterate;
	static int luminance_final;
	static int luminance_adapt;
	static int motion_blur;
	static int edge_detect;
	static int depth_of_field;
	static int depth_peel_initial;
	static int depth_peel_iterate;
	static int depth_peel_combine;
};

class ShaderManager
{

public:

	static void create();
	static ShaderManager* instance(){return m_instance;};
	static void destroy();

	int load(std::string shader_config_file);
	void create_constant_table(std::string shader_code);
	void lookup_constant_table(int shader_id, std::string variable_name, int *location, e_scopes *scope);

	void unbind();
	void bind(int shaderId);

	GLhandleARB get_shader_program(int id);

	void set_uniform(int shader_id, char* name, int value);
	void set_uniform(int shader_id, char* name, float value);
	void set_uniform(int shader_id, char* name, Vector2f value);
	void set_uniform(int shader_id, char* name, Vector3f value);
	void set_uniform(int shader_id, char* name, Vector3f value, float w);
	void set_uniform(int shader_id, char* name, float *matrix);
	void set_uniform(int shader_id, char* name, Matrix4 *matrix);
	void set_uniform(int shader_id, char* name, Matrix3 *matrix);

	int get_attribute_location( int shader_id, char* name );

	void set_uniform_array(int shader_id, char* name, Vector3fArray values);
	void set_uniform_array(int shader_id, char* name, Vector4fArray values);
	void set_uniform_array( int shader_id, char* name, Matrix4Array values);
	void set_uniform_array( int shader_id, char* name, Matrix3Array values);
	void set_uniform_array(int shader_id, char* name, float *values, int count);
	void set_uniform_array(int shader_id, char* name, std::vector<float> values);

	void clean_up();
	void delete_shader(int id);

	int active_shader(){ return m_active_shader_id; };

private:

	int m_number_of_shaders;
	int m_active_shader;
	int m_active_shader_id;

	GLhandleARB vertex_shader[MAX_SHADERS],fragment_shader[MAX_SHADERS],shader_program[MAX_SHADERS];

	std::map<std::string, ConstantTableEntry> constant_table[MAX_SHADERS];

	static ShaderManager *m_instance;
	ShaderManager(){m_number_of_shaders = 0;};
	~ShaderManager(){};

	
};

#endif