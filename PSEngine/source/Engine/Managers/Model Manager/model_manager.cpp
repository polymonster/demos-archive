/*=========================================================*\
|	model_manager.cpp -
|-----------------------------------------------------------|
|				Project :	PolySpoon Renderer
|				Coder	:	ADixon
|				Date	:	26/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "model_manager.h"
#include <fstream>
#include <string>
#include "polyspoon_math.h"
#include "timer.h"
#include "helpers.h"
#include "texture_manager.h"
#include "matrix.h"


/*======================== VARIABLES ======================*/

ModelManager *g_model_mgr_p;

Material dummy;

Matrix4 g_max_to_gl_axis4;
Matrix3 g_max_to_gl_axis3;

/*======================== FUNCTIONS ======================*/

ModelManager::ModelManager()
{
	g_max_to_gl_axis4.create_axis_swap(Vector3f(1.0f,0.0f,0.0f) ,Vector3f(0.0f,0.0f,-1.0f), Vector3f(0.0f, 1.0f,0.0f));
	g_max_to_gl_axis3 = Matrix3(g_max_to_gl_axis4);
}

ModelManager::~ModelManager()
{
	for(int i = m_models.size() - 1; i >= 0; i--)
	{
		delete m_models.at(i);
	}

	m_models.clear();

	for(int i = m_animated_models.size() - 1; i >= 0; i--)
	{
		delete m_animated_models.at(i);
	}

	m_animated_models.clear();
}

int ModelManager::load_mtl(char *filename)
{
	int current_model_id = m_models.size() -1;
	int material_count = 0;

	std::ifstream mtl_fin;

	//attempt to guess the mtl filename
	int end = strlen(filename);
	char mtl_filename[256] = { 0 };

	for(int i = 0; i < end - 3; i++)
	{
		mtl_filename[i] = filename[i];
	}

	strcat_s(mtl_filename,"mtl");

	mtl_fin.open(mtl_filename);

	//error handling
	if(!mtl_fin.is_open()) 
	{
		g_console_p->dbgmsg( "\t%s does not have an '.mtl' file, or the file is missing\n", filename);

		return material_count;
	}
	else
	{
		g_console_p->dbgmsg( "\treading material file %s\n", mtl_filename);
	}

	//file reading
	std::string material_reader;

	while(!mtl_fin.eof())
	{
		mtl_fin >> material_reader;

		Material *add_material;

		if(material_reader == "newmtl" || mtl_fin.eof())
		{
			if(material_count > 0)
			{
				m_models.at(current_model_id)->add_material(add_material);
			}

			char material_name[256] = { 0 };
			mtl_fin >> material_name;
			add_material = new Material(material_name);

			material_count++;
		}		
		else if(material_reader == "Ka" || material_reader == "ka" || material_reader == "KA")
		{
			//AMBIENT
			mtl_fin >> add_material->ambient.x >> add_material->ambient.y >> add_material->ambient.z;
		}
		else if(material_reader == "Kd" || material_reader == "kd" || material_reader == "KD")
		{
			//DIFFUSE
			mtl_fin >> add_material->diffuse.x >> add_material->diffuse.y >> add_material->diffuse.z;
		}
		else if(material_reader == "Ks" || material_reader == "ks" || material_reader == "KS")
		{
			//SPECULAR
			mtl_fin >> add_material->specular.x >> add_material->specular.y >> add_material->specular.z;
		}
		else if(material_reader == "d" || material_reader == "D" || 
				material_reader == "Tr" || material_reader == "tr" || material_reader == "TR")
		{
			//ALPHA / TRANSPARENCY
			mtl_fin >> add_material->transparency;
		}
		else if(material_reader == "Ns" || material_reader == "ns" || material_reader == "NS")
		{
			//SHININESS
			mtl_fin >> add_material->shininess;
		}
		else if(material_reader == "illum ")
		{
			//ILLUMINATION MODEL
			mtl_fin >> add_material->illumination_model;
		}
		else if(material_reader == "map_Kd")
		{
			//DIFFUSE TEXTURE
			char texture_file[256] = { 0 };
			mtl_fin >> texture_file;

			//png to tga convert
			int len = strlen(texture_file);
			if(texture_file[len-1] == 'g')
			{
				texture_file[len-1] = 'a';
				texture_file[len-2] = 'g';
				texture_file[len-3] = 't';
			}

			add_material->load_texture(texture_file,DIFFUSE);
		}
		else if(material_reader == "map_N" || material_reader == "map_bump")
		{
			//NORMAL MAP
			char texture_file[256] = { 0 };
			mtl_fin >> texture_file;

			//png to tga convert
			int len = strlen(texture_file);
			if(texture_file[len-1] == 'g')
			{
				texture_file[len-1] = 'a';
				texture_file[len-2] = 'g';
				texture_file[len-3] = 't';
			}

			add_material->load_texture(texture_file,NORMAL);
		}
		else if(material_reader == "map_S" || material_reader == "map_spec")
		{
			//SPECULAR MAP
			char texture_file[256] = { 0 };
			mtl_fin >> texture_file;

			//png to tga convert
			int len = strlen(texture_file);
			if(texture_file[len-1] == 'g')
			{
				texture_file[len-1] = 'a';
				texture_file[len-2] = 'g';
				texture_file[len-3] = 't';
			}

			add_material->load_texture(texture_file,SPECULAR);
		}
	}
	return material_count;
}

int ModelManager::load( std::string filename, bool convert_coordinate_system /*= false*/ )
{
	filename = MODEL_PATH + filename;

	int model_id = -1;

	char c_filename[256] = { 0 };
	strcpy(c_filename, filename.c_str());

	int len = strlen(c_filename);
	if(	(c_filename[len - 3] == 'O' || c_filename[len - 3] == 'o') &&
		(c_filename[len - 2] == 'B' || c_filename[len - 2] == 'b') &&
		(c_filename[len - 1] == 'J' || c_filename[len - 1] == 'j') )
	{
		g_console_p->dbgmsg( "\nloading '.obj' model %s\n", c_filename);

		//Load an OBJ
		model_id = load_obj(c_filename, convert_coordinate_system);
	}

	else if((c_filename[len - 3] == 'N' || c_filename[len - 3] == 'n') &&
			(c_filename[len - 2] == 'P' || c_filename[len - 2] == 'p') &&
			(c_filename[len - 1] == 'C' || c_filename[len - 1] == 'c') )
	{
		m_animated_models.push_back(new AnimatedModel(c_filename));
	}

	else if((c_filename[len - 3] == 'p' || c_filename[len - 3] == 'P') &&
		(c_filename[len - 2] == 's' || c_filename[len - 2] == 'S') &&
		(c_filename[len - 1] == 'm' || c_filename[len - 1] == 'M') )
	{
		model_id = load_psm(c_filename, convert_coordinate_system);
	}

	return model_id;
}

int ModelManager::load_obj_to_convex_hull(char *filename,CONVEX_HULL **convex_hull)
{
	std::ifstream fin;
	fin.open(filename);

	//error handling
	if(!fin.is_open()) 
	{
		g_console_p->dbgmsg( "\terror opening or finding '.obj' file %s \n", filename);

		return -1;
	}
	else
	{
		g_console_p->dbgmsg( "\treading model file %s\n", filename);
	}
	
	//file reading
	char buffer[256];

	Vector3fArray vertex_list;
	IndexArray index_list;

	int face_count = 0;

	while(!fin.eof())
	{
		fin >> buffer;

		switch(buffer[0])
		{
			case 'v':
			case 'V':
			{
				if(strlen(buffer) == 1)
				{
					//Vertices
					Vector3f vert;

					fin >> vert.x >> vert.y >> vert.z;

					vertex_list.push_back(vert);

				}
			}
			break;

			case 'g':
			case 'G':
			{
				if(strlen(buffer) == 1)
				{
					//Group (Mesh)
					std::string word;

					bool loop = true;

					Vector3fArray face_vertices;

					while(loop)
					{
						fin >> word;

						if(word == "f" || word == "F")
						{
							//Build Faces (Indices)
							for(int i = 0; i < 3; i++)
							{
								char face[256];

								char parse_v[32] = { 0 };
								char parse_t[32] = { 0 };
								char parse_n[32] = { 0 };

								fin >> face;

								int component = 0;
								int counter = 0;

								for(unsigned int i = 0; i < strlen(face); i++)
								{
									if(face[i] == '/')
									{
										component++;
										counter = 0;
									}
									else
									{
										switch(component)
										{
											case 0:
												
												parse_v[counter] = face[i];

											break;
										}

										counter++;
									}
								}
								
								int v_id = atoi(parse_v);

								index_list.push_back(v_id - 1);
							}
						}
						else if(word == "G" || word == "g")
						{
							loop = false;
						}
					}
				}

			}
			break;
		}
	}
	
	*convex_hull = new CONVEX_HULL(Vector3f(0,0,0),Vector3f(1,1,1),Vector3f(0,0,0),vertex_list,index_list);

	return 0;
}

Vector4fArray ModelManager::calculate_tangents( Vector3fArray vertices, Vector2fArray tex_coords, Vector3fArray normals )
{
	Vector3fArray tan1;
	Vector3fArray tan2;

	Vector4fArray vbo_tangents;

	int missed_faces = 0;

	for (long i = 0; i < vertices.size(); i+=3)
	{
		if(i + 2 < vertices.size())
		{
			long i1 = i;
			long i2 = i + 1;
			long i3 = i + 2;

			const Vector3f& v1 = vertices.at(i1);
			const Vector3f& v2 = vertices.at(i2);
			const Vector3f& v3 = vertices.at(i3);

			const Vector2f& w1 = tex_coords.at(i1);
			const Vector2f& w2 = tex_coords.at(i2);
			const Vector2f& w3 = tex_coords.at(i3);

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			Vector3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r);

			Vector3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r);

			tan1.push_back(sdir);
			tan1.push_back(sdir);
			tan1.push_back(sdir);

			tan2.push_back(tdir);
			tan2.push_back(tdir);
			tan2.push_back(tdir);
		}
		else
		{
			missed_faces += 3;
		}
	}

	for (long i = 0; i < vertices.size() - missed_faces; i++)
	{
		Vector3f n = normals.at(i);
		Vector3f t = tan1.at(i);

		//Orthogonalize
		t = psmath::normalise((t - n * psmath::dot(n, t)));

		//Calculate handedness which way? possibly only for opposite face culling
		float handedness = (psmath::dot(psmath::cross(n, t), tan2.at(i) ) < 0.0F) ? -1.0F : 1.0F;

		Vector4f final_tan = Vector4f(t, handedness);

		vbo_tangents.push_back( final_tan );
	}

	//note parallax mapping likes tan1 = sdir, tan2 = tdir, handedness < 0 = -1.0f
	//this is exactly as the code example specifies, normal maps like to have opposite handedness
	//verify with properly aligned normal map

	return vbo_tangents;
}

Vector4fArray ModelManager::calculate_indexed_tangents( Vector3fArray vertices, Vector2fArray tex_coords, Vector3fArray normals, IndexArray indices)
{
	Vector3fArray tan1;
	Vector3fArray tan2;

	Vector4fArray vbo_tangents;

	int missed_faces = 0;

	for (long i = 0; i < indices.size(); i+=3)
	{
		if(i + 2 < indices.size())
		{
			long i1 = indices.at(i);
			long i2 = indices.at(i + 1);
			long i3 = indices.at(i + 2);

			const Vector3f& v1 = vertices.at(i1);
			const Vector3f& v2 = vertices.at(i2);
			const Vector3f& v3 = vertices.at(i3);

			const Vector2f& w1 = tex_coords.at(i1);
			const Vector2f& w2 = tex_coords.at(i2);
			const Vector2f& w3 = tex_coords.at(i3);

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0f / (s1 * t2 - s2 * t1);
			Vector3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r);

			Vector3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r);

			tan1.push_back(sdir);
			tan1.push_back(sdir);
			tan1.push_back(sdir);

			tan2.push_back(tdir);
			tan2.push_back(tdir);
			tan2.push_back(tdir);
		}
		else
		{
			missed_faces += 3;
		}
	}

	for (long i = 0; i < indices.size() - missed_faces; i++)
	{
		long i1 = indices.at(i);

		Vector3f n = normals.at(i1);
		Vector3f t = tan1.at(i1);

		//Orthogonalize
		t = psmath::normalise((t - n * psmath::dot(n, t)));

		//Calculate handedness which way? possibly only for opposite face culling
		float handedness = (psmath::dot(psmath::cross(n, t), tan2.at(i1) ) < 0.0F) ? -1.0F : 1.0F;

		Vector4f final_tan = Vector4f(t, handedness);

		vbo_tangents.push_back( final_tan );
	}

	//note parallax mapping likes tan1 = sdir, tan2 = tdir, handedness < 0 = -1.0f
	//this is exactly as the code example specifies, normal maps like to have opposite handedness
	//verify with properly aligned normal map

	return vbo_tangents;
}

void ModelManager::render(int model_id)
{
	if(model_id >= 0 && model_id < m_models.size())
	{
		m_models[model_id]->render();
	}
}


void ModelManager::render_debug( int model_id )
{
	if(model_id >= 0 && model_id < m_models.size())
	{
		m_models.at(model_id)->render_debug();
	}
}

void ModelManager::render_animated(int model_id)
{
	if(model_id >= 0 && model_id < m_animated_models.size())
	{
		m_animated_models.at(model_id)->render();
	}
}

void ModelManager::update_animated(int model_id)
{
	if(model_id >= 0 && model_id < m_models.size() && g_game_timer.time_to_animate())
	{
		m_models.at(model_id)->update();
	}
}

void ModelManager::set_anim_state(int model_id, int state_id)
{
	if(model_id >= 0 && model_id < m_animated_models.size())
	{
		m_animated_models.at(model_id)->set_state(state_id);
	}

}

void ModelManager::execute_anim_state(int model_id, int state_id)
{
	if(model_id >= 0 && model_id < m_animated_models.size())
	{
		m_animated_models.at(model_id)->execute_state(state_id);
	}
}

void ModelManager::clear_anim_state(int model_id, int state_id)
{
	if(model_id >= 0 && model_id < m_animated_models.size())
	{
		m_animated_models.at(model_id)->clear_state(state_id);
	}
}

int ModelManager::load_obj( char *filename, bool convert_coordinate_system )
{
	m_models.push_back(new Model());
	load_mtl(filename);

	std::ifstream fin;
	fin.open( filename );

	//error handling
	if(!fin.is_open()) 
	{
		g_console_p->dbgmsg( "\terror opening or finding '.obj' file %s \n", filename);

		return -1;
	}
	else
	{
		g_console_p->dbgmsg( "\treading model file %s\n", filename);
	}

	//file reading
	char buffer[256];

	Vector3fArray vertex_list;
	Vector3fArray normal_list;
	Vector2fArray tex_coord_list;

	Vector3fArray face_vertex_list;
	Vector3fArray face_normal_list;
	Vector2fArray face_tex_coord_list;

	std::vector<int> index_list;

	int face_count = 0;
	int texture_id = -1;
	int current_model_id = m_models.size() -1;
	int current_sub_mesh = 0;
	int material = -1;
	int mesh_count = 0;

	while(!fin.eof())
	{
		fin.getline(buffer, 256);

		switch(buffer[0])
		{
			//Vertex Element
			case 'V':
			case 'v':
			{
				//Check for which vertex elements
				switch( buffer[1] )
				{
					//Position
					case ' ':
					{
						std::string str( &buffer[2] );
						std::stringstream s( str );

						Vector3f position;

						s >> position.x;
						s >> position.y;
						s >> position.z;

						if(convert_coordinate_system)
						{
							Vector3f aux = position;
							aux.z = -position.y;
							aux.y = position.z;

							position = aux;
						}

						vertex_list.push_back(position);
					}
					break;

					//Texture Coordinate
					case 'T':
					case 't':
					{
						std::string str( &buffer[3] );
						std::stringstream s( str );

						Vector2f tex_coord;
						float w;

						s >> tex_coord.x;
						s >> tex_coord.y;
						s >> w;

						tex_coord_list.push_back(tex_coord);
					}
					break;

					//Normal
					case 'N':
					case 'n':
					{
						std::string str( &buffer[3] );
						std::stringstream s( str );

						Vector3f normal;

						s >> normal.x;
						s >> normal.y;
						s >> normal.z;

						if(convert_coordinate_system)
						{
							Vector3f aux = normal;
							aux.z = -normal.y;
							aux.y = normal.z;

							normal = aux;
						}

						normal_list.push_back(normal);
					}
					break;

				}
			}
			break;

			case 'G':
			case 'g':
			{
				//Check for which vertex elements
				switch( buffer[1] )
				{
					//Position
					case ' ':
					{
					}

				}
			}
			break;

			case 'U':
			case 'u':
				{
					if(mesh_count > 0)
					{
						Mesh *add_mesh = new Mesh();
						Vector4fArray face_tangents = calculate_tangents(face_vertex_list, face_tex_coord_list, face_normal_list);

						add_mesh->add_vertices(face_vertex_list);
						add_mesh->add_texture_coords(face_tex_coord_list);
						add_mesh->add_normals(face_normal_list);
						add_mesh->add_tangents(face_tangents);

						face_vertex_list.clear();
						face_normal_list.clear();
						face_tex_coord_list.clear();

						m_models.at(current_model_id)->add_mesh(add_mesh,material);
					}


					std::string str( &buffer[0] );
					std::stringstream s( str );

					char material_name[256];
					s >> material_name; //skip
					s >> material_name;

					material = m_models.at(current_model_id)->find_material(material_name);

					mesh_count++;
				}
				break;

			case 'f':
			case 'F':
			{
				//Build Faces (Indices)
				char *face = &buffer[2];

				char parse_v[32] = { 0 };
				char parse_t[32] = { 0 };
				char parse_n[32] = { 0 };

				int component = 0;
				int counter = 0;
				int index = 0;

				for(unsigned int i = 0; i < strlen(face); i++)
				{
					if(face[i] == '/')
					{
						component++;
						counter = 0;
					}
					else if(face[i] == ' ')
					{
						index++;
						counter = 0;
						component = 0;

						int v_id = atoi(parse_v);
						int t_id = atoi(parse_t);
						int n_id = atoi(parse_n);

						if(v_id > 0 && t_id > 0 && n_id > 0)
						{
							face_vertex_list.push_back(vertex_list.at(v_id - 1));
							face_normal_list.push_back(normal_list.at(n_id - 1));
							face_tex_coord_list.push_back(tex_coord_list.at(t_id - 1));
						}

					}
					else
					{
						switch(component)
						{

							case 0:

								parse_v[counter] = face[i];
								parse_v[counter + 1] = '\0';

								break;

							case 1:

								parse_t[counter] = face[i];
								parse_t[counter + 1] = '\0';

								break;

							case 2:

								parse_n[counter] = face[i];
								parse_n[counter + 1] = '\0';

								break;
						}

						counter++;
					}

					if(i == strlen(face) - 1 && index < 3)
					{
						int v_id = atoi(parse_v);
						int t_id = atoi(parse_t);
						int n_id = atoi(parse_n);

						face_vertex_list.push_back(vertex_list.at(v_id - 1));
						face_normal_list.push_back(normal_list.at(n_id - 1));
						face_tex_coord_list.push_back(tex_coord_list.at(t_id - 1));
					}
				}
			}
			break;
		}
	}

	//anything left can be bunged into a mesh
	Mesh *add_mesh = new Mesh();
	Vector4fArray face_tangents = calculate_tangents(face_vertex_list, face_tex_coord_list, face_normal_list);

	add_mesh->add_vertices(face_vertex_list);
	add_mesh->add_texture_coords(face_tex_coord_list);
	add_mesh->add_normals(face_normal_list);
	add_mesh->add_tangents(face_tangents);

	m_models.at(current_model_id)->add_mesh(add_mesh,material);
	m_models.at(current_model_id)->calculate_aabb();
	m_models.at(current_model_id)->create_clusters();

	return current_model_id;
}

void ModelManager::render_animated_debug( int model_id )
{
	//m_animated_models.at(model_id)->render_debug();
}

int ModelManager::load_psm( char *filename, bool convert_coordinate_system )
{
	Model *new_model = new Model();

	Matrix4 axis_system;

	if(convert_coordinate_system) axis_system = g_max_to_gl_axis4;
	else axis_system.identity();

	std::ifstream fin;
	fin.open( filename );

	//error handling
	if(!fin.is_open()) 
	{
		g_console_p->dbgmsg( "\terror opening or finding '.obj' file %s \n", filename);
		return -1;
	}
	else
	{
		g_console_p->dbgmsg( "\treading model file %s\n", filename);
	}

	std::string word;

	int mesh_id = 0;

	while(!fin.eof())
	{
		fin >> word;

		if (word == "material_begin")
		{
			Material *new_material = new Material();
			float a;

			while(word != "material_end" && !fin.eof())
			{
				fin >> word;

				float elements = 0;

				if(word == "name")
				{
					fin >> new_material->material_name;
				}

				if(word == "TEXTURE_NAME")
				{
					char fname[128];
					fin >> fname;

					new_material->load_texture(fname, DIFFUSE);
				}

				if(word == "NORMAL_MAP_NAME")
				{
					char fname[128];
					fin >> fname;

					new_material->load_texture(fname, NORMAL);
				}

				if(word == "SPECULAR_MAP_NAME")
				{
					char fname[128];
					fin >> fname;

					new_material->load_texture(fname, SPECULAR);
				}

				if(word == "AMBIENT")
				{
					fin >> new_material->ambient.x;
					fin >> new_material->ambient.y;
					fin >> new_material->ambient.z;
					fin >> a;
				}

				if(word == "DIFFUSE")
				{
					fin >> new_material->diffuse.x;
					fin >> new_material->diffuse.y;
					fin >> new_material->diffuse.z;
					fin >> a;
				}

				if(word == "SPECULAR")
				{
					fin >> new_material->specular.x;
					fin >> new_material->specular.y;
					fin >> new_material->specular.z;
					fin >> a;
				}

				if(word == "SHININESS")
				{
					fin >> new_material->shininess;
				}

				if(word == "TRANSPARENCY")
				{
					fin >> new_material->transparency;
				}
			}

			new_model->add_material(new_material);
		}
		else if(word == "mesh_begin")
		{
			Mesh *new_mesh = new Mesh();

			Vector3fArray vertices;
			Vector3fArray normals;
			Vector2fArray tex_coords;
			Vector4fArray tangents;
			IndexArray indices;

			int material_id;

			while(word != "mesh_end" && !fin.eof())
			{
				fin >> word;

				float elements = 0;

				if(word == "num_elements")
				{
					fin >> elements;
				}

				if(word == "name")
				{
					fin >> word;
					new_mesh->set_name(word);
				}

				if(word == "material_id")
				{
					fin >> material_id;
				}

				if(word == "INDICES")
				{
					int count = 0;
					fin >> count;

					for(int i = 0; i < count; i++)
					{
						int index;
						fin >> index;

						indices.push_back(index);
					}
				}

				if(word == "VERTEX")
				{
					int count = 0;
					fin >> count;


					for(int i = 0; i < count; i++)
					{
						Vector3f v;

						fin >> v.x;
						fin >> v.y;
						fin >> v.z;

						v = axis_system * v;

						vertices.push_back(v);
					}
				}

				if(word == "NORMAL")
				{
					int count = 0;
					fin >> count;

					Vector3fArray vertices;

					for(int i = 0; i < count; i++)
					{
						Vector3f n;

						fin >> n.x;
						fin >> n.y;
						fin >> n.z;

						n = axis_system * n;

						normals.push_back(n);
					}
				}

				if(word == "TEXCOORD")
				{
					int count = 0;
					fin >> count;

					Vector3fArray vertices;

					for(int i = 0; i < count; i++)
					{
						Vector2f t;
						float dummy;

						fin >> t.x;
						fin >> t.y;
						fin >> dummy;

						tex_coords.push_back(t);
					}
				}

				if(word == "TEXTANGENT")
				{
					int count = 0;
					fin >> count;

					char buffer[256];
					buffer[0] = '\0';

					for(int i = 0; i < count; i++)
					{
						Vector3f t;

						fin >> buffer;
						if(strcmp(buffer, "NaN"))
						{
							t.x = atof(buffer);
						}

						fin >> buffer;
						if(strcmp(buffer, "NaN"))
						{
							t.x = atof(buffer);
						}

						fin >> buffer;
						if(strcmp(buffer, "NaN"))
						{
							t.x = atof(buffer);
						}

						/*fin >> t.x;
						fin >> t.y;
						fin >> t.z;*/

						t = axis_system * t;

						Vector4f t4 = Vector4f(t,1.0f);

						tangents.push_back(t4);
					}
				}
			}

			Vector4fArray calculated_tangents = calculate_tangents(vertices, tex_coords, normals);

			new_mesh->add_vertices(vertices);
			new_mesh->add_normals(normals);
			new_mesh->add_texture_coords(tex_coords);

			//new_mesh->add_tangents(tangents);
			new_mesh->add_tangents(calculated_tangents);

			new_mesh->add_indices(indices);

			new_model->add_mesh(new_mesh, material_id);

			mesh_id++;
		}
		else if(word == "skeleton_begin")
		{
			Skeleton *new_skeleton = new Skeleton();

			while(word != "skeleton_end")
			{
				fin >> word;

				if(word == "joint")
				{
					Joint *parent = NULL;
					std::string name;
					std::string parent_name;
					std::string target_name;

					float raw_matrix[16];

					//name
					fin >> word;
					fin >> name;

					//target name
					fin >> word;
					fin >> target_name;

					//parent name
					fin >> word;
					fin >> parent_name;

					parent = new_skeleton->find_joint(parent_name);

					//matrix
					fin >> word;
					for(int i = 0; i < 16; i++)
					{
						fin >> raw_matrix[i];
					}

					Matrix4 mat;
					mat.set_matrix_from_raw( raw_matrix );

					if(parent_name == "root") mat = axis_system * mat;

					Joint *new_joint = new Joint(name, target_name, mat, parent);

					new_skeleton->add_joint(new_joint);
				}
			}

			new_model->add_skeleton(new_skeleton);
		}
		else if(word == "animation_begin")
		{
			Animation *new_animation = new Animation();

			while(word != "animation_end")
			{
				fin >> word;

				if(word == "channel_target")
				{
					std::string joint_name;

					fin >> joint_name;

					int frame_count;

					Joint *j = new_model->find_joint( joint_name );

					if(j)
					{
						AnimationChannel *new_channel = new AnimationChannel(j);

						fin >> word;
						fin >> frame_count;

						fin >> word;
						for(int i = 0; i < frame_count; i++)
						{
							float time;
							fin >> time;
						}

						fin >> word;
						float raw_matrix[16];
						for(int i = 0; i < frame_count; i++)
						{
							for(int i = 0; i < 16; i++)
							{
								fin >> raw_matrix[i];
							}

							Matrix4 mat;
							mat.set_matrix_from_raw( raw_matrix );

							new_channel->add_matrix( mat );
						}

						new_animation->add_channel( new_channel );
					}
				}
			}

			new_model->add_animation(new_animation);
		}
		else if(word == "skinning_info_begin")
		{
			Skin *new_skin = new Skin();
			std::string mesh_target;
			float raw_matrix[16];

			while(word != "skinning_info_end")
			{
				fin >> word;

				if(word == "skin_target")
				{
					fin >> mesh_target;
				}

				if(word == "bind_shape_matrix")
				{
					for(int i = 0; i < 16; i++)
					{
						fin >> raw_matrix[i];
					}

					Matrix4 mat;
					mat.set_matrix_from_raw( raw_matrix );

					new_skin->m_bind_shape_matrix = mat;

					//new_skin->m_bind_shape_matrix = Matrix3( mat );
					//new_skin->m_bind_shape_offset = mat.get_translation();
				}

				if(word == "num_skinned_joints")
				{
					int count = 0;
					fin >> count;

					for(int i = 0; i < count; i++)
					{
						fin >> word;

						std::string joint_name;
						fin >> joint_name;

						for(int j = 0; j < 16; j++)
						{
							fin >> raw_matrix[j];
						}

						Matrix4 mat;
						mat.set_matrix_from_raw( raw_matrix );

						new_skin->add_skinned_joint( new SkinnedJoint(mat, joint_name) );

					}
				}

				if(word == "num_skinned_verts")
				{
					int count = 0;
					fin >> count;

					for(int i = 0; i < count; i++)
					{
						SkinnedVertex *sv = new SkinnedVertex();

						fin >> word;

						int joint_count = 0;
						fin >> joint_count;

						for(int j = 0; j < joint_count; j++)
						{
							int new_joint;
							fin >> new_joint;

							float new_weight;
							fin >> new_weight;
							sv->add_joint(new_joint, new_weight);
						}

						new_skin->add_skinned_vertex(sv);
					}
				}
			}

			new_model->add_skin(new_skin, mesh_target);
		}
	}

	new_model->calculate_aabb();
	new_model->create_clusters();

	m_models.push_back(new_model);

	return m_models.size() - 1;
}

void ModelManager::delete_models()
{
	for(int i = 0; i < m_models.size(); i++)
	{
		delete m_models[i];
	}

	m_models.clear();
}
