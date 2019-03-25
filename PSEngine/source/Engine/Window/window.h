/*=========================================================*\
|	window.h - application window management
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _WINDOW_H
#define _WINDOW_H

/*======================== INCLUDES =======================*/

#include "gl_context.h"
#include "common.h"

/*======================== CONSTANTS ======================*/

/*========================= CLASSES =======================*/

class Window
{
public:

	Window(){};
	Window( HINSTANCE &app_handle, HWND &window_handle, WNDPROC window_message_callback, int show_cmd, Vector2i size );
	~Window();

	bool		create_window			( Vector2i size, bool fullscreen, LPCWSTR window_title = L"Game Title", char *window_icon = NULL, char *game_cursor = NULL );
	bool		window_init_successful	( void )		{ return m_window_success; }
	void		enum_gl_context ( );

protected:

	HINSTANCE	m_app_handle;
	HWND		m_window_handle;
	WNDCLASSEX	m_window_class;
	int			m_show_cmd;

	WNDPROC		m_window_message_callback;

	Vector2i	m_size;
	Vector2i	m_pos;

	Vector2i	m_desktop_resolution;
	int			m_refresh_rate;
	int			m_desktop_color_bpp;
	int			m_desktop_display_flags;

	bool		m_window_success;

};


/*================== EXTERNAL VARIABLES ===================*/

extern Window	*g_window_p;
extern PSRenderer *g_renderer_p;
extern bool g_fullscreen;

#endif //_WINDOW_H