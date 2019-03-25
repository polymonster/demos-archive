/*=========================================================*\
|	program.cpp - application management
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "program.h"
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
#include "gl_context.h"

/*======================== CONSTANTS ======================*/


/*======================== VARIABLES ======================*/

Program *g_app_p = NULL;
Vector2i g_screen_size;

/*================= PROTOTYPE DEFINITIONS =================*/

Program::Program( HINSTANCE &app_handle, HWND &window_handle, WNDPROC window_message_callback, int show_cmd, Vector2i size ) :
	Window( app_handle, window_handle, window_message_callback, show_cmd, size )
{
	// if the window was set up successfully..
	if( m_window_success )
	{
		m_window_handle = window_handle;

		g_screen_size = size;

		init_gl();

		init_managers();

		init_global_content();

		g_scene_p = new Scene();
	}
}

Program::Program( HWND window_handle )
{
	g_renderer_p = new PSRenderer(window_handle);

	g_console_p = new Console();

	g_screen_size = Vector2i(1024,768);

	init_gl();

	init_managers();

	init_global_content();

	g_scene_p = new Scene();
}

void Program::init_gl()
{
	g_console_p->dbgmsg( "Setting Default OpenGL States\n" );

	glViewport( 0, 0, g_screen_size.x, g_screen_size.y );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective( 60.0f, (float)g_screen_size.x/(float)g_screen_size.y, 1.0f, 100.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
		glDisable(GL_BLEND);
	glEnable( GL_ALPHA_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// set clear colour
	glClearColor( 0.6f, 0.6f, 0.6f, 1.0f );

	// depth testing for 3D
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	glDisable( GL_LIGHTING );

	// start in orthographic screen space for now..
	display_mode( DISPLAY_MODE_PERSPECTIVE );
}

void Program::init_managers()
{
	//helpers
	g_console_p->dbgmsg( "Initialising Render Helpers\n" );
	pshelpers::initialise_render_helpers();

	//texture
	g_console_p->dbgmsg( "Initialising Texture Manager\n" );
	g_texture_mgr.init();

	//shader
	g_console_p->dbgmsg( "Initialising Shader Manager\n" );
	ShaderManager::instance()->create();

	//model
	g_console_p->dbgmsg( "Initialising Model Manager\n" );
	g_model_mgr_p = new ModelManager();

	//camera
	g_camera_p = new ModellingCamera();
}

void Program::init_global_content()
{
	g_console_p->dbgmsg( "\nLoading Global Content\n" );

	//global textures
	g_console_p->dbgmsg( "Loading Global Textures\n" );
	TextureDefaults::no_texture = g_texture_mgr.load_texture( "black.tga" );
	TextureDefaults::normal_map = g_texture_mgr.load_texture( "default_normal.tga" );
	TextureDefaults::parallax_map = g_texture_mgr.load_texture( "default_spec.tga" );
	TextureDefaults::specular_map = g_texture_mgr.load_texture( "default_spec.tga" );

	TextureDefaults::test_cube = g_texture_mgr.load_cubemap("cubemap\\sahara.txt");
	TextureDefaults::materials_lookup = g_texture_mgr.load_texture("packed_mats.tga");

	//shaders are constantly loaded
	g_console_p->dbgmsg( "Loading Shaders\n" );
	Shaders::deferred = ShaderManager::instance()->load("deferred\\deferred.glsl");
	Shaders::punchout = ShaderManager::instance()->load("punchout\\p.glsl");
	Shaders::gaussian = ShaderManager::instance()->load("gaussian\\gaussian.glsl");
	Shaders::g_buffer = ShaderManager::instance()->load("g_buffer\\g_buffer.glsl");
	Shaders::g_buffer_skinned = ShaderManager::instance()->load("g_buffer\\g_buffer_skinned.glsl");
	Shaders::shadow_depth = ShaderManager::instance()->load("shadow_depth\\shadow_depth.glsl");
	Shaders::shadow_depth_skinned = ShaderManager::instance()->load("shadow_depth\\shadow_depth_skinned.glsl");
	Shaders::hdr_final = ShaderManager::instance()->load("hdr_final\\hdr_final.glsl");
	Shaders::bright_pass = ShaderManager::instance()->load("luminance\\bright_pass.glsl");
	Shaders::luminance_initial = ShaderManager::instance()->load("luminance\\luminance_initial.glsl");
	Shaders::luminance_iterate = ShaderManager::instance()->load("luminance\\luminance_iterate.glsl");
	Shaders::luminance_final = ShaderManager::instance()->load("luminance\\luminance_final.glsl");
	Shaders::luminance_adapt = ShaderManager::instance()->load("luminance\\luminance_adapt.glsl");
	Shaders::motion_blur = ShaderManager::instance()->load("motion_blur\\motion_blur.glsl");
	Shaders::edge_detect = ShaderManager::instance()->load("edge_detect\\edge_detect.glsl");
	Shaders::depth_of_field = ShaderManager::instance()->load("depth_of_field\\depth_of_field.glsl");
	Shaders::depth_peel_initial = ShaderManager::instance()->load("depth_peel\\depth_peel_initial.glsl");
	Shaders::depth_peel_iterate = ShaderManager::instance()->load("depth_peel\\depth_peel_iterate.glsl");
	Shaders::depth_peel_combine = ShaderManager::instance()->load("depth_peel\\depth_peel_combine.glsl");

}

Program::~Program()
{
	if(g_scene_p)
		delete g_scene_p;

	ShaderManager::instance()->clean_up();

	pshelpers::clean_up_render_helpers();

	if( !UnregisterClass( m_window_class.lpszClassName, m_app_handle ) )
	{
		//MessageBoxA( NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		m_app_handle = NULL;
	}

	if(g_renderer_p)
		delete g_renderer_p;
}

bool Program::update()
{
	// program logic
	g_input_handler.update_gamepads();

	if( g_scene_p )
		g_scene_p->update();

	g_game_timer.update();

	g_console_p->process_gl_errors();

	if(!g_input_handler.m_key_pressed[VK_ESCAPE]) return true;

	return false;
}

void Program::render( void )
{
	if(g_game_timer.time_to_render())
	{
		if( g_scene_p )
			g_scene_p->render();

		g_renderer_p->swap_buffers();
	}

	//post update (flag held keys etc)
	g_input_handler.check_held_keys();
}

void Program::display_mode( int mode )
{
	glPushMatrix();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	switch( mode )
	{
		case DISPLAY_MODE_PERSPECTIVE:
			gluPerspective( 60.0f, (float)g_screen_size.x/(float)g_screen_size.y, 1.0f, 10000.0f );
			break;
		case DISPLAY_MODE_ORTHOGRAPHIC:
			glOrtho( 0.0f, (float)g_screen_size.x, 0.0f, (float)g_screen_size.y, -1000.0f, 1000.0f );
			break;
	}

	glMatrixMode( GL_MODELVIEW );

	glPopMatrix();
}