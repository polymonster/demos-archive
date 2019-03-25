/*=========================================================*\
|	window.cpp - application window management
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "window.h"

/*======================== CONSTANTS ======================*/

#define			WINDOW_MODE		( WS_EX_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX )
#define			FULLSCREEN_MODE ( WS_POPUP )

/*======================== VARIABLES ======================*/

Window *g_window_p = NULL;
PSRenderer *g_renderer_p = NULL;


/*================= PROTOTYPE DEFINITIONS =================*/

Window::Window( HINSTANCE &app_handle, HWND &window_handle, WNDPROC window_message_callback, int show_cmd, Vector2i size ) :
	m_window_handle( window_handle ),
	m_app_handle( app_handle ),
	m_window_message_callback( window_message_callback ),
	m_show_cmd( show_cmd )
{
	g_console_p->dbgmsg( "Creating Window\n" );

	if( create_window( size, g_fullscreen, L"PolySpoon Engine v0.01a" ) )
	{
		m_window_success = true;
	}
	else
	{
		g_console_p->dbgmsg( "Failed\n" );
		m_window_success = false;
	}

	//get the native desktop settings
	DEVMODE display_settings;

	EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&display_settings);

	m_desktop_resolution = Vector2i(display_settings.dmPelsWidth,display_settings.dmPelsHeight);
	m_refresh_rate = display_settings.dmDisplayFrequency;
	m_desktop_color_bpp = display_settings.dmBitsPerPel;
	m_desktop_display_flags = display_settings.dmDisplayFlags;

	g_console_p->dbgmsg( "\nEnumerating Graphics Device Properties\n" );
	g_console_p->dbgmsg( "Resolution : %d,%d \n",m_desktop_resolution.x, m_desktop_resolution.y);
	g_console_p->dbgmsg( "Refresh Rate : %d \n", m_refresh_rate );
	g_console_p->dbgmsg( "Color BPP : %d \n", m_desktop_color_bpp );
	g_console_p->dbgmsg( "Display Flags : %d \n\n", m_desktop_display_flags );
}

Window::~Window()
{

}

bool Window::create_window( Vector2i size, bool fullscreen, LPCWSTR window_title, char *window_icon, char *game_cursor )
{
	DWORD dw_style = WINDOW_MODE;

	// set window spatial params
	if( fullscreen )
	{
		g_console_p->dbgmsg( "Entering fullscreen mode...\n" );
		m_pos.x			= 0;
		m_pos.y			= 0;
		m_size.x		= size.x;
		m_size.y		= size.y;

		dw_style = FULLSCREEN_MODE;
	}
	else
	{
		g_console_p->dbgmsg( "Entering windowed mode...\n" );
		m_pos.x			= 20;
		m_pos.y			= 20;
		m_size.x		= size.x;
		m_size.y		= size.y;
	}

	// clear out the window class for use
	ZeroMemory( &m_window_class, sizeof( WNDCLASSEXA ) );

	// fill in the struct with the needed information
	m_window_class.cbSize = sizeof( WNDCLASSEX );
	m_window_class.style = CS_HREDRAW | CS_VREDRAW;
	m_window_class.lpfnWndProc = m_window_message_callback;
	m_window_class.hInstance = m_app_handle;
	m_window_class.hCursor = LoadCursor(NULL, IDC_ARROW);

	if( !fullscreen )
		m_window_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
	
	m_window_class.lpszClassName = L"PSWindowClass1";

	// register the window class
	RegisterClassEx( &m_window_class );

	// Adjust window size to include window frame and title bar
	RECT window_rect;
	SetRect( &window_rect, m_pos.x, m_pos.y, m_pos.x+m_size.x, m_pos.y+m_size.y );
	AdjustWindowRect( &window_rect, dw_style, false );

	Vector2i dims( window_rect.right-window_rect.left, window_rect.bottom-window_rect.top );

	// create the window and use the result as the handle
	m_window_handle = CreateWindowExW(	NULL,
										(LPCWSTR) m_window_class.lpszClassName,
										(LPCWSTR) window_title,
										dw_style,
										m_pos.x,
										m_pos.y,
										dims.x,
										dims.y,
										NULL,					// we have no parent window, NULL
										NULL,					// we aren't using menus, NULL
										m_app_handle,			// application handle
										NULL );					// used with multiple windows, NULL

	// display the window on the screen
	ShowWindow( m_window_handle, m_show_cmd );

	g_renderer_p = new PSRenderer(m_window_handle);

	enum_gl_context();

	return true;
}

void Window::enum_gl_context()
{
	//verify PFD
	g_console_p->dbgmsg( "\nEnumerating OpenGL Context Paramaters\n");
	g_console_p->dbgmsg( "OpenGL Version : %s\n", glGetString(GL_VERSION));
	g_console_p->dbgmsg( "GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	g_console_p->dbgmsg( "Renderer : %s\n", glGetString(GL_RENDERER));
	g_console_p->dbgmsg( "Vendor : %s\n", glGetString(GL_VENDOR));
	
	//g_console_p->dbgmsg( "Supported Extensions : %s\n", glGetString(GL_EXTENSIONS));

	int v;
	glGetIntegerv(GL_DEPTH_BITS,&v);
	g_console_p->dbgmsg( "Depth Bits : %d\n", v);

	glGetIntegerv(GL_ALPHA_BITS,&v);
	g_console_p->dbgmsg( "Alpha Bits : %d\n", v);

	glGetIntegerv(GL_STENCIL_BITS, &v);
	g_console_p->dbgmsg( "Stencil Bits : %d\n", v);
}
