/*=========================================================*\
|	model_manager.h -
|-----------------------------------------------------------|
|				Project :	PolySpoon Renderer
|				Coder	:	ADixon
|				Date	:	26/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _MODELMGR_H
#define _MODELMGR_H

/*======================== INCLUDES =======================*/

#include "model.h"
#include "render_object.h"
#include "polyspoon_math.h"

#include <vector>

/*======================== CONSTANTS ======================*/

#define		MAX_LOADED_MODELS		256

/*========================= CLASSES =======================*/
class ModelManager
{
public:

	ModelManager();
	~ModelManager();

	int load(std::string filename, bool convert_coordinate_system = false);
	Vector4fArray calculate_tangents(Vector3fArray vertices, Vector2fArray tex_coords, Vector3fArray normals);
	Vector4fArray calculate_indexed_tangents( Vector3fArray vertices, Vector2fArray tex_coords, Vector3fArray normals, IndexArray indices);

	int load_mtl(char *filename);
	int load_cal3d(char *filename);
	int load_obj_to_convex_hull(char *filename,CONVEX_HULL **convex_hull);

	int load_obj(char *filename, bool convert_coordinate_system);
	int load_psm(char *filename, bool convert_coordinate_system);

	void render(int model_id);
	void render_debug(int model_id);
	void render_animated(int model_id);
	void render_animated_debug(int model_id);

	void update_animated(int model_id);
	void set_anim_state(int model_id, int state_id);
	void execute_anim_state(int model_id, int state_id);
	void clear_anim_state(int model_id, int state_id);

	void delete_models();

	Model* Models(int id) const { return m_models.at(id); }
	AnimatedModel* Animated_models(int id) const { return m_animated_models.at(id); }

private:

	std::vector<Model*> m_models;
	std::vector<AnimatedModel*> m_animated_models;
};

/*================== EXTERNAL VARIABLES ===================*/

extern ModelManager *g_model_mgr_p;
extern Matrix4 g_max_to_gl_axis4;
extern Matrix3 g_max_to_gl_axis3;

#endif //_MODELMGR_H