/*=========================================================*\
|	scene.h - scene class
|-----------------------------------------------------------|
|				Project :	PolySpoon Game
|				Coder	:	ADixon
|				Date	:	20/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/


#ifndef scene_h__
#define scene_h__

#include "light.h"
#include "fbo.h"
#include "shadow_map.h"

#include "prop.h"

class Scene
{
	public:

		Scene();
		~Scene();

		void update();
		void render();

	private:

		//creation
		void create_render_buffers();
		void add_props();

		//rendering passes
		void back_buffer_render();
		void deferred_render();
		void main_scene_render(bool animated, bool shadowed);
		void forward_render();
		void ui_render();
		void aux_render();
		void debug_render();
		void motion_blur();
		void depth_of_field();

		//final output passes
		void hdr_pass();
		void ldr_pass();

		//buffers
		FBO *gbuffer_fbo;
		FBO *downsample_hdr_fbo[2];
		FBO *dof_blur_fbo[2];
		FBO *downsample_luminance_fbo[4];
		FBO *current_luminance;
		FBO *adapted_luminance[2];
		FBO *fullscreen_fbo[3];
		ShadowMap *shadow_map[3];

		int current_fullscreen_fbo;

		//props
		PropArray m_props;

		//...
};

/*================== EXTERNAL VARIABLES ===================*/

extern Scene *g_scene_p;

#endif // scene_h__