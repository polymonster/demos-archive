/****************************************
/	Al (c) 2008 - dappinput.h
/****************************************
/	DirectInput handler
/****************************************/

#ifndef _DAPPINPUT_H_
#define _DAPPINPUT_H_

/*************** INCLUDES ***************/

#include <windows.h>
#include <windowsx.h>

#define DIRECTINPUT_VERSION		0x0800

#include <dinput.h>
#include <XInput.h>

#pragma comment ( lib, "dinput8.lib" )
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "XInput.lib" )


/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/

extern	LPDIRECTINPUT8				g_din;
extern	LPDIRECTINPUTDEVICE8		g_dinkeyboard;
extern	LPDIRECTINPUTDEVICE8		g_dinmouse;

extern	unsigned char				g_keypress[256];
extern	unsigned char				g_keyheld[256];

typedef struct
{
	XINPUT_STATE	state;
	bool connected;

}gamepad_s;

typedef struct
{
	int jump;
	int left;
	int right;
	int interact;

}s_player_keymap;

extern	gamepad_s					g_gamepad[4];
extern  s_player_keymap				g_player_keymap[2];




/********* FUNCTION PROTOTYPES **********/

void		init_dinput			( HINSTANCE hInstance, HWND hWnd );
void		detect_keys			( HWND hWnd );
void		detect_mouse		( HWND hWnd );
bool		mouse_button_down	( int button );
bool		mouse_button_click	( int button );
void		get_mouse_pos		( int &x, int &y );
bool		key_down			( unsigned int key );
bool		key_hit				( unsigned int key );
void		get_gamepad_states	( HWND hWnd );

void		cleanup_dinput		( void );

#endif