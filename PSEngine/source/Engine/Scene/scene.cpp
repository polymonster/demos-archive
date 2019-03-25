/*=========================================================*\
|	scene.h - scene class
|-----------------------------------------------------------|
|				Project :	PolySpoon Game
|				Coder	:	ADixon
|				Date	:	20/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#include "scene.h"

#include "console.h"
#include "polyspoon_math.h"
#include "helpers.h"
#include "camera.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include "model_manager.h"
#include "light.h"
#include "FBO.H"
#include "shadow_map.h"
#include "timer.h"
#include "light_manager.h"
#include "editor_states.h"
#include "program.h"

#include "physics_manager.h"

Scene *g_scene_p;

int g_total_accum = 0;
int g_number_of_layers = 1;

AnimatedProp *anim;

Prop *sky;

Scene::Scene()
{
	g_editor_states.m_main_mode = g_editor_states.LIGHTING;

	Vector3f pos(-1600, 200, -1600);
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			g_lighting_manager.add_light( pos );

			pos.x += 800;
		}

		pos.x = -1600;
		pos.z += 800;
	}

	add_props();

	create_render_buffers();
}

void Scene::add_props()
{
	//probs move this to a more static place
	g_physics_mgr_p = new PhysicsManager();

	//floor
	g_model_mgr_p->load("small_plane.obj");
	m_props.push_back(new Prop( 0 , Vector3f(0, 0, 0), 0.0f  ));

	//boxes
	g_model_mgr_p->load("techcon2.obj");
	Vector3f box_pos = Vector3f(-200,500,-200);
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int k = 0; k < 10; k++)
			{
				m_props.push_back(new Prop( 1 , box_pos, 50.0f  ));
				box_pos.y += 250;
			}

			box_pos.y = 500;
			box_pos.x += 200;
		}

		box_pos.z += 70;
		box_pos.x = -200;
	}

	//croaks
	g_model_mgr_p->load("croak.obj");
	box_pos = Vector3f(-400,500,-200);
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			for(int k = 0; k < 1; k++)
			{
				m_props.push_back(new Prop( 2 , box_pos, 50.0f  ));
				box_pos.y += 250;
			}

			box_pos.y = 500;
			box_pos.x += 200;
		}

		box_pos.z += 70;
		box_pos.x = -400;
	}

	g_model_mgr_p->load("swat_materialised.psm", false);
//	g_model_mgr_p->load("marty.psm", false);

	Matrix4 mat;
	mat.create_translation(Vector3f(100.0f,0.0f,100.0f));
	m_props.push_back(new PlayerCharacter( 3 , &mat ));

//	mat.create_translation(Vector3f(100.0f,0.0f,100.0f));
//	m_props.push_back(new PlayerCharacter( 4 , &mat ));

	int sid = g_model_mgr_p->load("skydome.obj");
	sky = new Prop(sid, Vector3f(0.0,0.0,0.0), 0.0, false);
}

void Scene::create_render_buffers()
{
	//gbuffer
	gbuffer_fbo = new FBO(GBUFFER,g_screen_size.x,g_screen_size.y,true);

	//2 down sample buffers
	downsample_hdr_fbo[0] = new FBO(COLOUR,g_screen_size.x / 2,g_screen_size.y / 2,false);
	downsample_hdr_fbo[1] = new FBO(COLOUR,g_screen_size.x / 2,g_screen_size.y / 2,false);

	//2 depth of field buffers
	dof_blur_fbo[0] = new FBO(COLOUR_HALF_FLOAT,g_screen_size.x / 2,g_screen_size.y / 2,false);
	dof_blur_fbo[1] = new FBO(COLOUR_HALF_FLOAT,g_screen_size.x / 2,g_screen_size.y / 2,false);

	//4 downsample luminance buffers
	for(int i = 0; i < 4; i++)
	{
		int sample_scale = 1 << 2 * (i + 1);
		downsample_luminance_fbo[i] = new FBO(COLOUR_HALF_FLOAT,g_screen_size.x / sample_scale,g_screen_size.x / sample_scale,false);
	}

	//2 tiny buffers for adapted and current luminance
	adapted_luminance[0] = new FBO(COLOUR_HALF_FLOAT, 1, 1, false);
	adapted_luminance[1] = new FBO(COLOUR_HALF_FLOAT, 1, 1, false);
	current_luminance = new FBO(COLOUR_HALF_FLOAT, 1, 1, false);

	//3 fullscreen fbo's! outlandish where the hell did 3 come from
	fullscreen_fbo[0] = new FBO(COLOUR_FLOAT,g_screen_size.x,g_screen_size.y,false);
	fullscreen_fbo[1] = new FBO(COLOUR_FLOAT,g_screen_size.x,g_screen_size.y,false);
	fullscreen_fbo[2] = new FBO(COLOUR_FLOAT,g_screen_size.x,g_screen_size.y,false);

	//3 shadow maps for csm
	shadow_map[0] = new ShadowMap(VARIANCE_SHADOW,0);
	shadow_map[1] = new ShadowMap(VARIANCE_SHADOW,1);
	shadow_map[2] = new ShadowMap(VARIANCE_SHADOW,2);

	//set some vars
	current_fullscreen_fbo = 1;

}

bool done = true;

void Scene::update()
{
	g_camera_p->update();

	g_physics_mgr_p->update();

	g_lighting_manager.update();

	int prop_count = m_props.size();
	for( int i = 0; i < prop_count; i++ )
	{
		m_props[i]->update();
	}
}

void Scene::render()
{
	back_buffer_render();

	deferred_render();

	depth_of_field();

	motion_blur();

 	if(g_input_handler.m_key_held[VK_T])
 	{
		forward_render();
	}

	hdr_pass();

	aux_render();
}

void Scene::back_buffer_render()
{
	//start with a clean frame
	glClearColor(0.9f,0.9f,0.9f,1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	glPushMatrix();

		g_camera_p->set_position();
		g_lighting_manager.back_buffer_render();

	glPopMatrix();

	//clear the buffer before proceeding, no rendering needs to be swapped to the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Scene::main_scene_render(bool animated, bool shadowed)
{	
	glPushMatrix();

	int prop_count = m_props.size();

	if(!shadowed)
	{
		int shader_id = Shaders::g_buffer;
		if(animated) shader_id = Shaders::g_buffer_skinned;

		//bind gbuffer shader
		ShaderManager::instance()->bind(shader_id);

		//set global shader parameters
		ShaderManager::instance()->set_uniform(shader_id,"diffuse_map",0);
		ShaderManager::instance()->set_uniform(shader_id,"normal_map",1);
		ShaderManager::instance()->set_uniform(shader_id,"specular_map",2);

		Matrix4 rotation_matrix = g_camera_p->get_rotation_matrix();
		Matrix4 view_matrix = g_camera_p->get_matrix();

		//the actual model rendering
		for( int i = 0; i < prop_count; i++ )
		{	
			if(m_props[i]->animated() == animated)
			{
				m_props[i]->render();
			}
		}
	}
	else
	{
		if(animated)
		{
			//bind gbuffer shader
			ShaderManager::instance()->bind(Shaders::shadow_depth_skinned);

			//the actual model rendering
			for( int i = 0; i < prop_count; i++ )
			{	
				if(m_props[i]->animated() == animated)
				{
					m_props[i]->render(true);
				}
			}
		}
		else
		{
			ShaderManager::instance()->bind(Shaders::shadow_depth);

			for( int i = 0; i < prop_count; i++ )
			{
				if(m_props[i]->animated() == animated)
				{
					m_props[i]->render(true);
				}
			}
		}
	}

	glPopMatrix();
}

float rot = 0.0f;

void Scene::deferred_render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	//main scene 3d render
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Render into a simple shader
	shadow_map[0]->begin();
		main_scene_render(true, true);
		main_scene_render(false, true);	
	shadow_map[0]->end();

	shadow_map[1]->begin();
		main_scene_render(true, true);
		main_scene_render(false, true);
	shadow_map[1]->end();

	shadow_map[2]->begin();
		main_scene_render(true, true);
		main_scene_render(false, true);
	shadow_map[2]->end();

	//bind the mrt fbo
	gbuffer_fbo->bind_mrt( 4 );

		//Main Scene Render
		glPushMatrix();

			//lights, camera...
			g_camera_p->set_position();

				main_scene_render(true, false);
				main_scene_render(false, false);

			sky->render();

		glPopMatrix();

	//tidy up
	gbuffer_fbo->unbind();

	g_texture_mgr.disable_all_units();

	//output to floating point buffer
	fullscreen_fbo[0]->bind(1.0f);

		gbuffer_fbo->bind_texture(0,0);
		gbuffer_fbo->bind_texture(1,1);
		gbuffer_fbo->bind_texture(2,2);

		ShaderManager::instance()->bind(Shaders::deferred);

		ShaderManager::instance()->set_uniform(Shaders::deferred,"normal_buffer",0);
		ShaderManager::instance()->set_uniform(Shaders::deferred,"position_buffer",1);
		ShaderManager::instance()->set_uniform(Shaders::deferred,"colour_buffer",2);

		ShaderManager::instance()->set_uniform(Shaders::deferred,"cubemap",3);

		ShaderManager::instance()->set_uniform(Shaders::deferred,"materials_lookup",4);

		g_texture_mgr.bind_cubemap(TextureDefaults::test_cube, 3);
		g_texture_mgr.bind_texture(TextureDefaults::materials_lookup, 4);

		shadow_map[0]->bind_texture_deferred(5);
		shadow_map[1]->bind_texture_deferred(6);
		shadow_map[2]->bind_texture_deferred(7);

		g_lighting_manager.enable_lights();

		ShaderManager::instance()->set_uniform(Shaders::deferred,"camera_pos",g_camera_p->m_position);

		pshelpers::screen_space_quad(g_screen_size);

		g_texture_mgr.unbind_cubemap(3);

		ShaderManager::instance()->unbind();

	fullscreen_fbo[0]->unbind();

	//clean up states
	g_texture_mgr.disable_all_units();
}

void Scene::aux_render()
{
	ShaderManager::instance()->unbind();

	glDisable(GL_DEPTH_TEST);

	//render deferred image to the screen and do lighting
	if(g_input_handler.m_key_held[VK_V])
	{
		int tex = 0;
		if(g_input_handler.m_key_held[VK_B])tex = 1;
		if(g_input_handler.m_key_held[VK_N])tex = 2;

		gbuffer_fbo->bind_texture(0,tex);

		pshelpers::screen_space_quad(g_screen_size);
	}

	g_texture_mgr.disable_all_units();

	//draw debug stuff
	g_lighting_manager.aux_render();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	g_texture_mgr.disable_all_units();
}

void Scene::forward_render()
{
	//begin
	g_texture_mgr.disable_all_units();

	gbuffer_fbo->bind(0.0f,false,true);

		//no shader
		ShaderManager::instance()->unbind();
		glEnable(GL_DEPTH_TEST);

		//set camera
		glPushMatrix();

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			g_camera_p->set_position();

			//lighting
			g_lighting_manager.render();

			//debug layers (physics..etc)
			//debug_render();

		glPopMatrix();

	//end
	gbuffer_fbo->unbind();

	g_texture_mgr.disable_all_units();

	fullscreen_fbo[0]->bind(0.0f, false, false);

		//screen space output
		ShaderManager::instance()->bind(Shaders::punchout);
		ShaderManager::instance()->set_uniform(Shaders::punchout,"texture",0);

		gbuffer_fbo->bind_texture(0,0);
		pshelpers::screen_space_quad(g_screen_size);

		ShaderManager::instance()->unbind();

	fullscreen_fbo[0]->unbind();
}

void Scene::debug_render()
{
	if(g_input_handler.m_key_held[VK_D] || true)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		g_texture_mgr.disable_all_units();
		ShaderManager::instance()->unbind();

		//Physics world render (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(1.0f,1.0f,1.0f,1.0f);

			int prop_count = m_props.size();
			for( int i = 0; i < prop_count; i++ )
			{
				m_props[i]->physics_world_render();
			}

			g_model_mgr_p->render_debug(3);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		g_model_mgr_p->render_animated_debug(0);
	}

	if (g_input_handler.m_key_held[VK_F])
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	
		g_texture_mgr.disable_all_units();
		ShaderManager::instance()->unbind();
	
		//Physics world render (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
		for(int i = 0; i < 1; i++)
		{
			shadow_map[i]->render_volume();
		}
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Scene::motion_blur()
{
	//edge detection for AA
	/*ShaderManager::instance()->bind(Shaders::edge_detect);

	fullscreen_fbo[2]->bind();

		//screen space output
		ShaderManager::instance()->bind(Shaders::edge_detect);
		ShaderManager::instance()->set_uniform(Shaders::edge_detect,"normal_depth_texture",0);

		Vector2f pixel_size(1.0f / (float)g_screen_size.x, 1.0f / (float)g_screen_size.y);
		ShaderManager::instance()->set_uniform(Shaders::edge_detect,"px_size",pixel_size);
		ShaderManager::instance()->set_uniform(Shaders::edge_detect,"dilation",1.0f);

		gbuffer_fbo->bind_texture(0,0);

		pshelpers::screen_space_quad(g_screen_size);

		ShaderManager::instance()->unbind();

	fullscreen_fbo[2]->unbind();

	//motion blur
	ShaderManager::instance()->bind(Shaders::motion_blur);*/

	fullscreen_fbo[0]->bind();

		//screen space output
		ShaderManager::instance()->bind(Shaders::motion_blur);
		ShaderManager::instance()->set_uniform(Shaders::motion_blur,"image",0);
		ShaderManager::instance()->set_uniform(Shaders::motion_blur,"pixel_velocity_image",1);
		ShaderManager::instance()->set_uniform(Shaders::motion_blur,"number_of_samples",4);
		ShaderManager::instance()->set_uniform(Shaders::motion_blur,"pixel_blur_amount",0.5f);

		gbuffer_fbo->bind_texture(1,3);
		fullscreen_fbo[1]->bind_texture(0);

		//fullscreen_fbo[2]->bind_texture(2);

		pshelpers::screen_space_quad(g_screen_size);

		ShaderManager::instance()->unbind();

	fullscreen_fbo[0]->unbind();
}

float g_focal_plane = 0.001f;
//float g_focus_range = 0.4f;
float g_focus_range = 1.0f;

void Scene::depth_of_field()
{
	if(g_input_handler.m_key_held[VK_8])g_focal_plane += 0.001f;
	if(g_input_handler.m_key_held[VK_7])g_focal_plane -= 0.001f;
	if(g_input_handler.m_key_held[VK_6])g_focus_range += 0.001f;
	if(g_input_handler.m_key_held[VK_5])g_focus_range -= 0.001f;

	g_focal_plane = max(g_focal_plane,0.0);
	g_focal_plane = min(g_focal_plane,1.0);
	g_focus_range = max(g_focus_range,0.000001);
	
	//take output screen and down sample
	ShaderManager::instance()->unbind();

	dof_blur_fbo[0]->bind();

	fullscreen_fbo[0]->bind_texture(0);
	pshelpers::screen_space_quad(g_screen_size);

	dof_blur_fbo[0]->unbind();

	//blur the down sampled image
	Vector2f pixel_size;
	pixel_size.x = (float) (1.0f / (float)(g_screen_size.x / 2));
	pixel_size.y = (float) (1.0f / (float)(g_screen_size.y / 2));

	ShaderManager::instance()->bind(Shaders::gaussian);
	ShaderManager::instance()->set_uniform(Shaders::gaussian,"image", 0);

	for(int i = 0; i < 2; i++)
	{
		//horizontal
		ShaderManager::instance()->set_uniform(Shaders::gaussian,"u_scale",Vector2f(pixel_size.x,0.0f));
		dof_blur_fbo[1]->bind();

			dof_blur_fbo[0]->bind_texture(0);
			pshelpers::screen_space_quad(g_screen_size);

		dof_blur_fbo[1]->unbind();

		//vertical
		ShaderManager::instance()->set_uniform(Shaders::gaussian,"u_scale",Vector2f(0.0f,pixel_size.y));
		dof_blur_fbo[0]->bind();

			dof_blur_fbo[1]->bind_texture(0);
			pshelpers::screen_space_quad(g_screen_size);

		dof_blur_fbo[0]->unbind();
	}

	//apply it
	ShaderManager::instance()->bind(Shaders::depth_of_field);

	fullscreen_fbo[1]->bind();

		//screen space output
		ShaderManager::instance()->bind(Shaders::depth_of_field);
		ShaderManager::instance()->set_uniform(Shaders::depth_of_field,"image",0);
		ShaderManager::instance()->set_uniform(Shaders::depth_of_field,"blurred_image",1);
		ShaderManager::instance()->set_uniform(Shaders::depth_of_field,"depth_texture",2);
		ShaderManager::instance()->set_uniform(Shaders::depth_of_field,"focal_plane",g_focal_plane);
		ShaderManager::instance()->set_uniform(Shaders::depth_of_field,"focus_range",g_focus_range);

		fullscreen_fbo[0]->bind_texture(0);
		dof_blur_fbo[0]->bind_texture(1);
		gbuffer_fbo->bind_texture(3,0);

		pshelpers::screen_space_quad(g_screen_size);

		ShaderManager::instance()->unbind();

	fullscreen_fbo[1]->unbind();
}

float g_middle_grey = 0.19f;
int g_current_adapt_tex = 0;
int g_second_adapt_tex = 1;
bool g_first = true;

void Scene::hdr_pass()
{
	//take output screen and down sample
	ShaderManager::instance()->unbind();

	downsample_hdr_fbo[0]->bind();

	fullscreen_fbo[0]->bind_texture(0);
	pshelpers::screen_space_quad(g_screen_size);

	downsample_hdr_fbo[0]->unbind();

	//measure luminance
	//initial luminance
	ShaderManager::instance()->bind(Shaders::luminance_initial);
	ShaderManager::instance()->set_uniform(Shaders::luminance_initial,"image", 0);

	downsample_luminance_fbo[0]->bind();
	
	downsample_hdr_fbo[0]->bind_texture(0);
	pshelpers::screen_space_quad(g_screen_size);

	downsample_luminance_fbo[0]->unbind();

	//iterate luminance average
	ShaderManager::instance()->bind(Shaders::luminance_iterate);
	for(int i = 1; i < 4; i++)
	{
		int sample_scale = 1 << 2 * ( (4 - i) + 1);

		ShaderManager::instance()->set_uniform(Shaders::luminance_iterate,"image",0);
		ShaderManager::instance()->set_uniform(Shaders::luminance_iterate,"texel_offset",1.0f / (float)sample_scale);

		downsample_luminance_fbo[i]->bind();

		downsample_luminance_fbo[i - 1]->bind_texture(0);
		pshelpers::screen_space_quad(g_screen_size);

		downsample_luminance_fbo[i]->unbind();
	}

	//obtain average luminance 1x1
	ShaderManager::instance()->bind(Shaders::luminance_final);
	ShaderManager::instance()->set_uniform(Shaders::luminance_final,"image",0);
	
	current_luminance->bind();

	downsample_luminance_fbo[3]->bind_texture(0);

	pshelpers::screen_space_quad(g_screen_size);

	current_luminance->unbind();

	if(g_first)
	{	
		g_first = false;

		adapted_luminance[g_second_adapt_tex]->bind();

		downsample_luminance_fbo[3]->bind_texture(0);

		pshelpers::screen_space_quad(g_screen_size);

		adapted_luminance[g_second_adapt_tex]->unbind();
	}

	//update the viewer exposure
	ShaderManager::instance()->bind(Shaders::luminance_adapt);
	ShaderManager::instance()->set_uniform(Shaders::luminance_adapt,"current_lum_tex",0);
	ShaderManager::instance()->set_uniform(Shaders::luminance_adapt,"current_adapted_lum_tex",1);
	ShaderManager::instance()->set_uniform(Shaders::luminance_adapt,"ellapsed_time",g_game_timer.get_game_time());

	adapted_luminance[g_current_adapt_tex]->bind();

	current_luminance->bind_texture(0);
	adapted_luminance[g_second_adapt_tex]->bind_texture(1);

	pshelpers::screen_space_quad(g_screen_size);

	adapted_luminance[g_current_adapt_tex]->unbind();

	//bright pass / high pass filter the down sampled image
	ShaderManager::instance()->bind(Shaders::bright_pass);
	ShaderManager::instance()->set_uniform(Shaders::bright_pass,"image",0);
	ShaderManager::instance()->set_uniform(Shaders::bright_pass,"average_luminance",1);
	ShaderManager::instance()->set_uniform(Shaders::bright_pass,"middle_grey",g_middle_grey);

	downsample_hdr_fbo[1]->bind();

	downsample_hdr_fbo[0]->bind_texture(0);
	adapted_luminance[g_current_adapt_tex]->bind_texture(1);

	pshelpers::screen_space_quad(g_screen_size);

	downsample_hdr_fbo[1]->unbind();

	//toggle the buffers
	g_current_adapt_tex++;
	if(g_current_adapt_tex > 1) g_current_adapt_tex = 0;

	g_second_adapt_tex++;
	if(g_second_adapt_tex > 1) g_second_adapt_tex = 0;


	//blur the down sampled image
	Vector2f pixel_size;
	pixel_size.x = (float) (1.0f / (float)(g_screen_size.x / 2));
	pixel_size.y = (float) (1.0f / (float)(g_screen_size.y / 2));

	ShaderManager::instance()->bind(Shaders::gaussian);
	ShaderManager::instance()->set_uniform(Shaders::gaussian,"image", 0);

	for(int i = 0; i < 3; i++)
	{
		//horizontal
		ShaderManager::instance()->set_uniform(Shaders::gaussian,"u_scale",Vector2f(pixel_size.x,0.0f));
		downsample_hdr_fbo[0]->bind();


		downsample_hdr_fbo[1]->bind_texture(0);
		pshelpers::screen_space_quad(g_screen_size);

		downsample_hdr_fbo[0]->unbind();

		//vertical
		ShaderManager::instance()->set_uniform(Shaders::gaussian,"u_scale",Vector2f(0.0f,pixel_size.y));
		downsample_hdr_fbo[1]->bind();

		downsample_hdr_fbo[0]->bind_texture(0);
		pshelpers::screen_space_quad(g_screen_size);

		downsample_hdr_fbo[1]->unbind();
	}

	//tone map the final scene and output
	g_texture_mgr.disable_all_units();

	ShaderManager::instance()->bind(Shaders::hdr_final);
	ShaderManager::instance()->set_uniform(Shaders::hdr_final,"bloom_scale",3.0f);
	ShaderManager::instance()->set_uniform(Shaders::hdr_final,"middle_grey",g_middle_grey);
	ShaderManager::instance()->set_uniform(Shaders::hdr_final,"hdr_image",0);
	ShaderManager::instance()->set_uniform(Shaders::hdr_final,"bloom_image",1);
	ShaderManager::instance()->set_uniform(Shaders::hdr_final,"adapted_luminance_image",2);

	adapted_luminance[g_current_adapt_tex]->bind_texture(2);

	fullscreen_fbo[0]->bind_texture(0);
	downsample_hdr_fbo[1]->bind_texture(1);

	pshelpers::screen_space_quad(g_screen_size);
}

void Scene::ldr_pass()
{
	g_texture_mgr.disable_all_units();

	ShaderManager::instance()->bind(Shaders::punchout);
	ShaderManager::instance()->set_uniform(Shaders::punchout,"texture",0);

	fullscreen_fbo[1]->bind_texture(0);

	pshelpers::screen_space_quad(g_screen_size);
}

Scene::~Scene()
{
	//free up models
	g_model_mgr_p->delete_models();

	//free up fbos
	delete gbuffer_fbo;
	delete downsample_hdr_fbo[0];
	delete downsample_hdr_fbo[1];
	delete dof_blur_fbo[0];
	delete dof_blur_fbo[1];
	delete downsample_luminance_fbo[0];
	delete downsample_luminance_fbo[1];
	delete downsample_luminance_fbo[2];
	delete downsample_luminance_fbo[3];
	delete current_luminance;
	delete adapted_luminance[0];
	delete adapted_luminance[1];
	delete fullscreen_fbo[0];
	delete fullscreen_fbo[1];
	delete fullscreen_fbo[2];
	delete shadow_map[0];
	delete shadow_map[1];
	delete shadow_map[2];
}


/*
void Scene::geom()
{
	glPushMatrix();

	glTranslatef(0.0f,100.0f,0.0f);

	for(int i = 0; i < 3; i++)
	{
	glTranslatef(0.0f,0.0f,-110.0f);

	g_model_mgr_p->render(1);
	}

	glTranslatef(100.0f,0.0f,100.0f);

	g_model_mgr_p->render(1);

	glTranslatef(-200.0f,0.0f,100.0f);

	g_model_mgr_p->render(1);

	glPopMatrix();
}

void Scene::forward_depth_peel()
{
	//debug this wank

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//start off depth peeling
	ShaderManager::instance()->bind(Shaders::depth_peel_initial);

	fullscreen_fbo[0]->bind(1.0,true,true);

	geom();

	fullscreen_fbo[0]->unbind();

	//clear the 2nd buffer
	fullscreen_fbo[1]->bind();
	fullscreen_fbo[1]->unbind();

	//begin accumulating
	ShaderManager::instance()->bind(Shaders::depth_peel_combine);

	fullscreen_fbo[2]->bind();

	//accumulate
	ShaderManager::instance()->set_uniform(Shaders::depth_peel_combine,"depth_layer",0);
	ShaderManager::instance()->set_uniform(Shaders::depth_peel_combine,"accumulation",1);

	fullscreen_fbo[0]->bind_texture(0);
	fullscreen_fbo[1]->bind_texture(1);

	pshelpers::screen_space_quad(g_screen_size);

	fullscreen_fbo[2]->unbind();

	//peel through layers

	int front_buffer = 0;
	int back_buffer = 1;
	int accumulation_buffer = 2;

	for(int i = 0; i < g_number_of_layers; i++)
	{
		//peel down to the next layer
		ShaderManager::instance()->bind(Shaders::depth_peel_iterate);

		fullscreen_fbo[back_buffer]->bind(1.0,true,true);

		fullscreen_fbo[front_buffer]->bind_texture(7);
		ShaderManager::instance()->set_uniform(Shaders::depth_peel_iterate,"depth_layer",7);

		geom();

		fullscreen_fbo[back_buffer]->unbind();

		//accumulate depth layers
		ShaderManager::instance()->bind(Shaders::depth_peel_combine);

		fullscreen_fbo[front_buffer]->bind();

		//accumulate
		ShaderManager::instance()->set_uniform(Shaders::depth_peel_combine,"depth_layer",0);
		ShaderManager::instance()->set_uniform(Shaders::depth_peel_combine,"accumulation",1);

		fullscreen_fbo[back_buffer]->bind_texture(0);
		fullscreen_fbo[accumulation_buffer]->bind_texture(1);

		pshelpers::screen_space_quad(g_screen_size);

		fullscreen_fbo[front_buffer]->unbind();

		//swap buffers
		g_total_accum = front_buffer;

		int prevaccum = accumulation_buffer;
		accumulation_buffer = front_buffer;

		front_buffer = back_buffer;
		back_buffer = prevaccum;
	}

	ShaderManager::instance()->unbind();
}*/
