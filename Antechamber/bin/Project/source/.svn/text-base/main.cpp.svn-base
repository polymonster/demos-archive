/****************************************
/	Al (c) 2008 - main.cpp
/****************************************
/	Its the main function...
/****************************************/


/*************** INCLUDES ***************/

#include <windows.h>
#include <windowsx.h>
#include <time.h>

#include "matrices.h"
#include "dxio.h"

#include "dappinput.h"
#include "d3dapp.h"
#include "apptimer.h"

/*************** CONSTANTS **************/

#define			AL_WINDOW		( WS_EX_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX )
#define			GAME_VERSION	"v0.61b"


/*********** GLOBAL VARIABLES ***********/

//float	start_frame_time = 0.0f;

/********* FUNCTION PROTOTYPES **********/

// the WindowProc function prototype
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


/********* FUNCTION DEFINITIONS *********/

// the entry point for any Windows program
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	if( g_fullscreen )
	{
		dbgmsg( "Entering fullscreen mode...\n" );
		g_window_x		= 0;
		g_window_y		= 0;
		g_window_w		= FULL_WIDTH;
		g_window_h		= FULL_HEIGHT;	
	}
	else
	{
		dbgmsg( "Entering windowed mode...\n" );
		g_window_x		= 200;
		g_window_y		= 200;
		g_window_w		= WINDOWED_WIDTH;
		g_window_h		= WINDOWED_HEIGHT;
	}
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

	// fill in the struct with the needed information
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursorFromFile( L"images\\sprites\\cur1.cur" );
	if( !g_fullscreen )
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// Adjust window size to include window frame and title bar
	RECT rc;
	SetRect( &rc, g_window_x, g_window_y, g_window_x+g_window_w, g_window_y+g_window_h );
	AdjustWindowRect( &rc, AL_WINDOW, false );
	int w=(rc.right-rc.left);
	int h=(rc.bottom-rc.top);

	char title[64];
	sprintf( title, "ANTECHAMBER %s", GAME_VERSION );

	// create the window and use the result as the handle
	hWnd = CreateWindowExA(	NULL,
							"WindowClass1",				// name of the window class
							title,						// title of the window
                            AL_WINDOW,					// window style
							g_window_x,
							g_window_y,
							w,
							h,
							NULL,					// we have no parent window, NULL
							NULL,					// we aren't using menus, NULL
							hInstance,				// application handle
							NULL );					// used with multiple windows, NULL

	// display the window on the screen
	ShowWindow( hWnd, nCmdShow );

	// set up and initialize DirectInput
	init_dinput( hInstance, hWnd );

	// set up and initialize Direct3D
	init_d3d( hWnd );
	
	// set up orthograpic matrix for projection
	D3DXMatrixOrthoOffCenterLH( &g_ortho_2d, 0.0f, (float)g_window_w, (float)g_window_h, 0.0f, 0.0f, 1.0f );
	g_d3d_device->SetTransform( D3DTS_PROJECTION, &g_ortho_2d );

	// reset world and view matrices
	D3DXMatrixIdentity( &g_identity );

	g_d3d_device->SetTransform( D3DTS_PROJECTION, &g_ortho_2d );
	g_d3d_device->SetTransform( D3DTS_WORLD, &g_identity );
	g_d3d_device->SetTransform( D3DTS_VIEW, &g_identity );

	// this struct holds Windows event messages
	MSG msg;

	unsigned int seed = (int)time( 0 );
	srand( seed );

	init_game_time();

	// Enter the infinite message loop
	while( 1 )
	{

		// Check to see if any messages are waiting in the queue
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			// If the message is WM_QUIT, exit the while loop
			if ( msg.message == WM_QUIT )
				break;

			// Translate the message and dispatch it to WindowProc()
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// do stuffs

		if( !g_scene_p->execute() )
		{
			PostMessage( hWnd, WM_DESTROY, 0, 0 );
			return( (int)msg.wParam );
		}

		render_frame();

		detect_keys( hWnd );
		detect_mouse( hWnd );
		get_gamepad_states( hWnd );

	}

	// clean up DirectX and COM
	cleanup_d3d();

	// clean up DirectInput
	cleanup_dinput();

	// return this part of the WM_QUIT message to Windows
	return( (int)msg.wParam );
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	// sort through and find what code to run for the message given
	switch( message )
	{
		// this message is read when the window is closed
		case WM_DESTROY:
			{
				// close the application entirely
				PostQuitMessage( 0 );
				return( 0 );
			}
			break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc( hWnd, message, wParam, lParam );
}
