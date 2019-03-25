/****************************************
/	Al (c) 2008 - d3dapp.cpp
/****************************************/


/*************** INCLUDES ***************/

#include "d3dapp.h"
#include "apptimer.h"


/*************** CONSTANTS **************/


/*********** GLOBAL VARIABLES ***********/

bool				g_widescreen	= 1;
bool				g_fullscreen	= 0;
bool				g_editor_mode	= 0;

int					g_window_x		= 0;
int					g_window_y		= 0;
int					g_window_w		= FULL_WIDTH;
int					g_window_h		= FULL_HEIGHT;

unsigned int		g_clear_col		= 0xff000000;

float	start_frame_time = 0.0f;

LPDIRECT3D9			g_d3d;
LPDIRECT3DDEVICE9	g_d3d_device;

HWND				g_hWnd;


/********* FUNCTION DEFINITIONS *********/

// this function initializes and prepares Direct3D for use
void init_d3d( HWND hWnd )
{
	// create the Direct3D interface
    g_d3d = Direct3DCreate9( D3D_SDK_VERSION );

	// create a struct to hold various device information
    D3DPRESENT_PARAMETERS d3dpp;

	// clear out the struct for use
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	// program windowed, not fullscreen
    d3dpp.Windowed = !g_fullscreen;

	// discard old frames
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if( !g_fullscreen )
		d3dpp.FullScreen_RefreshRateInHz = 0;

	// don't wait for vsync!
	d3dpp.PresentationInterval = D3DPRESENT_DONOTWAIT;

	// set the window to be used by Direct3D
    d3dpp.hDeviceWindow = hWnd;

	if( g_fullscreen )
	{
		// set the back buffer format to 32-bit
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

		// set the width of the buffer
		d3dpp.BackBufferWidth = g_window_w;

		// set the height of the buffer
		d3dpp.BackBufferHeight = g_window_h;
	}

    // create a device class using this information and information from the d3dpp stuct
    g_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_d3d_device );


	// initialise our 2D scene
	g_scene_p = new Scene2D();

	g_hWnd = hWnd;

	return;
}


// used to render a frame
void render_frame( void )
{
	g_d3d_device->Clear( 0, NULL, D3DCLEAR_TARGET, g_clear_col, 1.0f, 0 );
	//g_d3d_device->Clear( 0, NULL, D3DCLEAR_ZBUFFER, g_clear_col, 1.0f, 0 );

	g_scene_p->m_game_time = get_game_time();

	g_d3d_device->BeginScene();

		g_scene_p->render();

	g_d3d_device->EndScene();

	//g_scene_p->m_execution_time = get_game_time() - start_frame_time;

	g_d3d_device->Present( NULL, NULL, NULL, NULL );

	//g_scene_p->m_game_time = get_game_time() - start_frame_time;


	return;
}


// cleans up Direct3D and COM
void cleanup_d3d( void )
{
	// clean up our 2D scene
	delete( g_scene_p );

	// close and release the 3D device
	g_d3d_device->Release();

	// close and release Direct3D
	g_d3d->Release();

	return;
}