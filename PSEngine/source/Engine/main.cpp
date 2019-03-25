/*=========================================================*\
|	main.cpp - application entry point
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include <windows.h>

#include "window.h"
#include "console.h"
#include "program.h"
#include "input.h"
#include "model_manager.h"
#include "gl_context.h"

#include "polyspoon_math.h"

/*======================== CONSTANTS ======================*/


/*======================== VARIABLES ======================*/

Vector2i g_window_size( 1280, 720 );
bool g_fullscreen = false;


/*=========================================================*\
|	WindowProc() - win32 app message handler
\*=========================================================*/
LRESULT CALLBACK WindowProc( HWND window_handle, unsigned int message, WPARAM wParam, LPARAM lParam );

/*=========================================================*\
|	WinMain() - win32 app entry point
\*=========================================================*/
int WINAPI WinMain( HINSTANCE app_handle, HINSTANCE prev_app_handle, char *command_line_p, int show_cmd )
{
	HWND window_handle = NULL;

	// console object
	g_console_p = new Console();

	g_console_p->dbgmsg( "======== PolySpoon Engine ========\n" );
	g_console_p->dbgmsg( "Program Start\n" );

	// create window object
	// TODO - obtain data from application config file

	// create main window
	g_app_p = new Program( app_handle, window_handle, (WNDPROC)WindowProc, show_cmd, g_window_size );

	if( g_app_p->window_init_successful() )
	{
		// this struct holds Windows event messages
		MSG window_message;

		g_console_p->dbgmsg( "\nEntering Main Loop\n" );

		// program loop
		while( 1 )
		{
			// Check to see if any messages are waiting in the queue
			if( PeekMessage( &window_message, NULL, 0, 0, PM_REMOVE ) )
			{
				// If the message is WM_QUIT, exit the while loop
				if ( window_message.message == WM_QUIT )
					break;

				// Translate the message and dispatch it to WindowProc()
				TranslateMessage( &window_message );
				DispatchMessage( &window_message );
			}
			else
			{
				if( !g_app_p->update() )
				{
					PostMessage( window_handle, WM_DESTROY, 0, 0 );
					break;
				}

				// render scene
				g_app_p->render();

			}
		}
	}
	
	g_console_p->dbgmsg( "Program Close\n" );
	g_console_p->dbgmsg( "==================================\n" );

	// clear program object
	delete g_app_p;

	// clear console object
	delete g_console_p;

	//clean up models
	delete g_model_mgr_p;

	return 0;

}

/*=========================================================*\
|	WindowProc() - win32 app message handler
\*=========================================================*/
LRESULT CALLBACK WindowProc( HWND window_handle, unsigned int message, WPARAM wParam, LPARAM lParam )
{
	//update the input
	g_input_handler.update(wParam,lParam,message);

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

		case WM_SIZE:

			g_window_size.x = (int) LOWORD(lParam);
			g_window_size.y = (int) HIWORD(lParam);

			g_renderer_p->size_viewport(g_window_size);

		break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc( window_handle, message, wParam, lParam );
}

void non_callback_proc(unsigned int message, WPARAM wParam, LPARAM lParam )
{
	g_input_handler.update(wParam,lParam,message);

	switch( message )
	{
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return;
		}
		break;

		case WM_SIZE:

			g_window_size.x = (int) LOWORD(lParam);
			g_window_size.y = (int) HIWORD(lParam);

			g_renderer_p->size_viewport(g_window_size);

		break;
	}
}
