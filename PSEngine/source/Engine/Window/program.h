/*=========================================================*\
|	program.h - application management
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _PROGRAM_H
#define _PROGRAM_H

/*======================== INCLUDES =======================*/

#include "window.h"
#include "vector.h"
#include "input.h"
#include "scene.h"

/*======================== CONSTANTS ======================*/

#define		DISPLAY_MODE_PERSPECTIVE		0
#define		DISPLAY_MODE_ORTHOGRAPHIC		1

/*========================= CLASSES =======================*/

void non_callback_proc(unsigned int message, WPARAM wParam, LPARAM lParam );

class Program : public Window
{

public:

	Program( HWND window_handle );
	Program( HINSTANCE &app_handle, HWND &window_handle, WNDPROC window_message_callback, int show_cmd, Vector2i size );
	~Program();

	void init_gl();
	void init_global_content();
	void init_managers();

	bool update					( void );

	void render					( void );

	void display_mode			( int mode );

private:

	HWND m_window_handle;


};


/*================== EXTERNAL VARIABLES ===================*/

extern Program	*g_app_p;
extern Vector2i g_screen_size;

#endif //_PROGRAM_H