#include "shader_manager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

ShaderManager* ShaderManager::m_instance = NULL;

//static int's to track shader names
int Shaders::g_buffer;
int Shaders::punchout;
int Shaders::deferred;
int Shaders::gaussian;
int Shaders::shadow_depth;
int Shaders::shadow_depth_skinned;
int Shaders::bright_pass;
int Shaders::hdr_final;
int Shaders::luminance_initial;
int Shaders::luminance_iterate;
int Shaders::luminance_final;
int Shaders::luminance_adapt;
int Shaders::motion_blur;
int Shaders::edge_detect;
int Shaders::depth_of_field;
int Shaders::depth_peel_initial;
int Shaders::depth_peel_iterate;
int Shaders::depth_peel_combine;
int Shaders::g_buffer_skinned;

void ShaderManager::clean_up()
{
	for(int i = 0; i < m_number_of_shaders; i++)
	{
		glDeleteObjectARB(vertex_shader[i]);
		glDeleteObjectARB(fragment_shader[i]);
		glDeleteObjectARB(shader_program[i]);
	}
}

void ShaderManager::delete_shader(int id)
{
	glDeleteObjectARB(shader_program[id]);
}

void ShaderManager::create(){

	if(m_instance == NULL)
	{
		m_instance = new ShaderManager;
	}
}

void ShaderManager::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void ShaderManager::unbind()
{
	if(m_active_shader != 0)
	{
		m_active_shader = -1;
		m_active_shader_id = -1;
		glUseProgram(0);
	}
}

void ShaderManager::bind(int shaderId)
{
	if(m_active_shader != shader_program[shaderId])
	{
		m_active_shader = shader_program[shaderId];
		m_active_shader_id = shaderId;
		glUseProgram(shader_program[shaderId]);
	}
}

#define printOpenGLError() print_compile_errors(__FILE__, __LINE__)

int print_compile_errors(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();

		PostQuitMessage(0);
    }
    return retCode;
}

void print_info_log(GLhandleARB obj, char *shaderCompilerMessage)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);
    
	if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);

		g_console_p->dbgmsg("%s\n",infoLog);

		if(shaderCompilerMessage != NULL) strcat(shaderCompilerMessage,infoLog);
		
        free(infoLog);
    }

}

int ShaderManager::load( std::string shader_config_file )
{
	std::ifstream fin;
	std::string word;
	std::string vertex_shader_file;
	std::string fragment_shader_file;
	std::string short_name = shader_config_file;

	shader_config_file = SHADER_PATH + shader_config_file;

	//read in the shader config file
	fin.open(shader_config_file.c_str());

	//verify the shader config is legitimate
	fin >> word;

	if(word != "shader_config") 
	{
		MessageBoxA(NULL, (LPCSTR) "Not A Valid Shader File", (LPCSTR) "Error!", MB_OK);
		return -1;
	}

	//obtain the vertex shader name
	fin >> word;
	if(word == "vertex") fin >> vertex_shader_file;
	vertex_shader_file = SHADER_PATH + vertex_shader_file;

	//obtain the framgent shader name
	fin >> word;
	if(word == "fragment") fin >> fragment_shader_file;
	fragment_shader_file = SHADER_PATH + fragment_shader_file;
	
	//close the shader config
	fin.close();

	//create a blank shader program
	shader_program[m_number_of_shaders] = glCreateProgramObjectARB();

	//create a blank vertex shader and fragment shader
	vertex_shader[m_number_of_shaders] = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragment_shader[m_number_of_shaders] = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	//read the vertex and fragment source code
	std::ifstream vs(vertex_shader_file.c_str());
	std::string vertex_shader_code((std::istreambuf_iterator<char>(vs)), std::istreambuf_iterator<char>());

	std::ifstream fs(fragment_shader_file.c_str());
	std::string fragment_shader_code((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

	const char * vv = vertex_shader_code.c_str();
	const char * ff = fragment_shader_code.c_str();

	if(vv == NULL) MessageBoxA(NULL, (LPCSTR) "Couldn't find vertex file", (LPCSTR) "Error!", MB_OK);
	if(ff == NULL) MessageBoxA(NULL, (LPCSTR) "Couldn't find fragment file", (LPCSTR) "Error!", MB_OK);

	//compile the vertex shader
	glShaderSourceARB(vertex_shader[m_number_of_shaders], 1, &vv, NULL);
	glCompileShaderARB(vertex_shader[m_number_of_shaders]);
	glAttachObjectARB(shader_program[m_number_of_shaders],vertex_shader[m_number_of_shaders]);

	//compile the fragment shader
	glShaderSourceARB(fragment_shader[m_number_of_shaders], 1, &ff, NULL);
	glCompileShaderARB(fragment_shader[m_number_of_shaders]);
	glAttachObjectARB(shader_program[m_number_of_shaders],fragment_shader[m_number_of_shaders]);

	//link the program
	glLinkProgramARB(shader_program[m_number_of_shaders]);

	bind(m_number_of_shaders);
		create_constant_table(vertex_shader_code);
		create_constant_table(fragment_shader_code);
	unbind();

	//print error reports
	g_console_p->dbgmsg("%s\n",short_name.c_str());

	print_info_log(vertex_shader[m_number_of_shaders],NULL);
	print_info_log(fragment_shader[m_number_of_shaders],NULL);
	print_info_log(shader_program[m_number_of_shaders],NULL);

	m_number_of_shaders++;
	return m_number_of_shaders - 1;
}

GLhandleARB ShaderManager::get_shader_program(int id)
{
	return shader_program[id];
}

void ShaderManager::set_uniform(int shader_id, char* name, float value)
{
	int location;
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	glUniform1fARB(location,value);
}


void ShaderManager::set_uniform(int shader_id, char* name, Vector2f value)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	glUniform2fARB(location,value.x,value.y);
}

void ShaderManager::set_uniform(int shader_id, char* name, int value)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	glUniform1iARB(location,value);
}


void ShaderManager::set_uniform(int shader_id, char* name, Vector3f value)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	glUniform3fARB(location,value.x,value.y,value.z);

}

void ShaderManager::set_uniform(int shader_id, char* name, Vector3f value, float w)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	glUniform4fARB(location,value.x,value.y,value.z,w);
}

void ShaderManager::set_uniform(int shader_id, char* name, Matrix4 *matrix)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float mat[16];
	matrix->gl_compliant_matrix( mat );

	//set the uniform
	glUniformMatrix4fvARB(location, 1, 1, mat);
}

void ShaderManager::set_uniform( int shader_id, char* name, Matrix3 *matrix )
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float mat[9];
	matrix->gl_compliant_matrix( mat );

	//set the uniform
	glUniformMatrix3fvARB(location, 1, 1, mat);
}

void ShaderManager::set_uniform(int shader_id, char* name, float *matrix)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	//set the uniform
	glUniformMatrix4fvARB(location, 1, 1, matrix);
}

void ShaderManager::set_uniform_array( int shader_id, char* name, Vector3fArray values )
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float *gl_values = new float[ (int) values.size() * 3 ];

	for(int i = 0; i < values.size(); i++)
	{
		int index = i * 3;

		gl_values[index    ] = values[i].x;
		gl_values[index + 1] = values[i].y;
		gl_values[index + 2] = values[i].z;
	}

	glUniform3fvARB(location, values.size(), gl_values);

	delete gl_values;
}

void ShaderManager::set_uniform_array( int shader_id, char* name, Matrix4Array values )
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float *gl_values = new float[ (int) values.size() * 16 ];

	for(int i = 0; i < values.size(); i++)
	{
		float mat[16];
		values[i].gl_compliant_matrix( mat );

		for(int j = 0; j < 16; j++)
		{
			int index = (i * 16) + j;

			gl_values[index] = mat[j];
		}
	}

	glUniformMatrix4fvARB(location, values.size(), 1, gl_values);

	delete gl_values;
}

void ShaderManager::set_uniform_array( int shader_id, char* name, float *values, int count)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	glUniform1fvARB(location,count,values);
}
void ShaderManager::set_uniform_array( int shader_id, char* name, std::vector<float> values)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float *gl_values = new float[ (int) values.size() ];

	for(int i = 0; i < values.size(); i++)
	{
		gl_values[i] = values.at(i);
	}

	glUniform1fvARB(location,values.size(), gl_values);

	delete gl_values;
}


void ShaderManager::set_uniform_array(int shader_id, char* name, Vector4fArray values)
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float *gl_values = new float[ (int) values.size() * 4 ];

	for(int i = 0; i < values.size(); i++)
	{
		int index = i * 4;

		gl_values[index    ] = values[i].x;
		gl_values[index + 1] = values[i].y;
		gl_values[index + 2] = values[i].z;
		gl_values[index + 3] = values[i].w;
	}

	glUniform4fvARB(location, values.size(), gl_values);

	delete gl_values;

}

void ShaderManager::set_uniform_array( int shader_id, char* name, Matrix3Array values )
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	float *gl_values = new float[ (int) values.size() * 9 ];

	for(int i = 0; i < values.size(); i++)
	{
		float mat[9];
		values[i].gl_compliant_matrix( mat );

		for(int j = 0; j < 9; j++)
		{
			int index = (i * 9) + j;

			gl_values[index] = mat[j];
		}
	}

	glUniformMatrix3fvARB(location, values.size(), 1, gl_values);

	delete gl_values;
}

int ShaderManager::get_attribute_location( int shader_id, char* name )
{
	int location; 
	e_scopes scope;
	lookup_constant_table( shader_id, name, &location, &scope);

	return location;
}

void ShaderManager::lookup_constant_table(int shader_id, std::string variable_name, int *location, e_scopes *scope)
{
	ConstantTableEntry shader_constant = constant_table[shader_id][variable_name];

	*scope = shader_constant.scope;
	*location = shader_constant.location;
}

void ShaderManager::create_constant_table(std::string shader_code )
{
	std::stringstream str_stream( shader_code );

	ConstantTableEntry new_constant;

 	while(!str_stream.eof())
	{
		std::string word;

		str_stream >> word;

		if(word == "uniform" || word == "attribute")
		{
			if(word == "uniform")
			{
				new_constant.scope = e_scopes::glsl_uniform;
			}
			else
			{
				new_constant.scope = e_scopes::glsl_attribute;
			}

			//now read the type of the variable
			str_stream >> word;

			if(word == "int") 
			{
				int i = 0;
			}

			//now the name of the variable
			str_stream >> new_constant.name;

			//check for semicolon
			int end = new_constant.name.size() - 1;

			if(new_constant.name.at(end) == ';')
			{
				//remove the semi colon
				new_constant.name.erase(end,end);

				int new_end = new_constant.name.size() - 1;
				if(new_constant.name.at(new_end) == ']' )
				{
					//we remove the array
					int j = new_end;
					while(new_constant.name.at(j) != '[')
					{
						j--;
					}

					new_constant.name.erase(j,new_end);
				}

				//now find it by name
				if(new_constant.scope == e_scopes::glsl_attribute)
				{
					new_constant.location = glGetAttribLocationARB(get_shader_program(m_number_of_shaders),new_constant.name.c_str());
				}
				else
				{
					new_constant.location = glGetUniformLocationARB(get_shader_program(m_number_of_shaders),new_constant.name.c_str());
				}

				constant_table[m_number_of_shaders][new_constant.name] = new_constant;
			}
		}	
	}
}