/****************************************
/	Al (c) 2008 - dappinput.cpp
/****************************************/


/*************** INCLUDES ***************/

#include "dappinput.h"
#include "d3dapp.h"
#include "font.h"
#include "2dscene.h"


/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/

LPDIRECTINPUT8				g_din;
LPDIRECTINPUTDEVICE8		g_dinkeyboard;
LPDIRECTINPUTDEVICE8		g_dinmouse;

unsigned char				g_keypress[256];
unsigned char				g_keyheld[256];
bool						g_button_down[8];
bool						g_button_clicked[8];
int							g_mousepos[2];

gamepad_s					g_gamepad[4];
s_player_keymap				g_player_keymap[2];

/********* FUNCTION DEFINITIONS *********/

// this is the function that initializes DirectInput
void init_dinput( HINSTANCE hInstance, HWND hWnd )
{
	// create the DirectInput interface
	DirectInput8Create(	hInstance,					// the handle to the application
						DIRECTINPUT_VERSION,		// the compatible version
						IID_IDirectInput8,			// the DirectInput interface version
						(void**)&g_din,				// the pointer to the interface
						NULL );						// COM stuff, so we'll set it to NULL

	// create the keyboard device
	g_din->CreateDevice(	GUID_SysKeyboard,		// the default keyboard ID being used
							&g_dinkeyboard,			// the pointer to the device interface
							NULL );					// COM stuff, so we'll set it to NULL

	// create the mouse device
	g_din->CreateDevice(	GUID_SysMouse,			// the default mouse ID being used
							&g_dinmouse,			// the pointer to the device interface
							NULL);					// COM stuff, so we'll set it to NULL

	g_dinkeyboard->SetDataFormat( &c_dfDIKeyboard ); // set the data format to keyboard format
	g_dinmouse->SetDataFormat(&c_dfDIMouse);    // set the data format to mouse format

	// set the control you will have over the keyboard
	g_dinkeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );

	// set the control you will have over the mouse
	g_dinmouse->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );

	g_dinmouse->Acquire();	// Acquire the mouse only once


	g_mousepos[0] = 0;
	g_mousepos[1] = 0;

	g_player_keymap[0].jump = DIK_SPACE;
	g_player_keymap[0].left = DIK_A;
	g_player_keymap[0].right = DIK_D;
	g_player_keymap[0].interact = DIK_W;

	g_player_keymap[1].jump = DIK_NUMPAD0;
	g_player_keymap[1].left = DIK_NUMPAD4;
	g_player_keymap[1].right = DIK_NUMPAD6;
	g_player_keymap[1].interact = DIK_NUMPAD8;

}

void detect_mouse( HWND hWnd )
{
static DIMOUSESTATE mousestate;
		HRESULT hr; 

		hr = g_dinmouse->Poll(); 

		if( FAILED(hr) )
		{
			hr = g_dinmouse->Acquire();
			
			while( hr == DIERR_INPUTLOST ) 
				hr = g_dinmouse->Acquire();
		}

		g_dinmouse->GetDeviceState( sizeof( DIMOUSESTATE ), &mousestate );

		POINT pos;
		// get cursor pos, gets absolute, not relative!
		GetCursorPos( &pos );

		RECT rectangle;
		GetWindowRect( hWnd, &rectangle );

		g_mousepos[0] = pos.x;
		g_mousepos[1] = pos.y;

		rectangle.top += 32;

		if( g_fullscreen )
			rectangle.top -= 32;

		for( int i = 0; i < 4; i++ )
		{
			if( g_button_clicked[i] && mouse_button_down(i) )
				g_button_down[i] = true;
			else
				g_button_down[i] = false;

			if( !g_button_down[i] )
				g_button_clicked[i] = false;

		}

		if( g_mousepos[0] < rectangle.left )		g_mousepos[0] = rectangle.left;
		if( g_mousepos[0] > rectangle.right )		g_mousepos[0] = rectangle.right;
		if( g_mousepos[1] < rectangle.top )			g_mousepos[1] = rectangle.top;
		if( g_mousepos[1] > rectangle.bottom )		g_mousepos[1] = rectangle.bottom;

		g_mousepos[0] -= rectangle.left;
		g_mousepos[1] -= rectangle.top;
		g_mousepos[0] = (int)(( (float)g_mousepos[0] / ( rectangle.right - rectangle.left ) ) * g_window_w ); 
		g_mousepos[1] = (int)(( (float)g_mousepos[1] / ( rectangle.bottom - rectangle.top ) ) * g_window_h ); 
}


// this is the function that detects keystrokes and displays them in a message box
void detect_keys( HWND hWnd )
{
	// create a static storage for the key-states
static unsigned char keystate[256];

	// get access if we don't have it already
	g_dinkeyboard->Acquire();

	// fill keystate with values
	g_dinkeyboard->GetDeviceState( 256, (LPVOID)keystate );

	// EXIT
	if( keystate[DIK_ESCAPE] & 0x80 )
	{
		// kill that shit
		PostMessage( hWnd, WM_DESTROY, 0, 0 );
	}

	unsigned int key = 0x02;

	while( key < 0xDC )
	{
		// key pressed?
		if( keystate[key] & 0x80 )
		{
			// key already being pressed?
			if( g_keypress[key] == 1 )
				g_keyheld[key] = 1;
			else
				g_keyheld[key] = NULL;

			g_keypress[key] = 1;
		}
		else
			g_keypress[key] = NULL;
		key += 0x01;
	}
}

void get_gamepad_states( HWND hWnd )
{
	for (DWORD i=0; i< 4; i++ )
	{
		ZeroMemory( &g_gamepad[i].state, sizeof(XINPUT_STATE) );

        // Simply get the state of the controller from XInput.
        DWORD dwResult = XInputGetState( i, &g_gamepad[i].state );

        if( dwResult == ERROR_SUCCESS )
		{ 
			// Controller is connected 
			g_gamepad[i].connected = true;
			//PostMessage( hWnd, WM_DESTROY, 0, 0 );
		}
        else
		{
            // Controller is not connected 
			g_gamepad[i].connected = false;
		}

		
}

}

// this is the function that detets mouse movements and mouse buttons
bool mouse_button_down( int button )
{
static DIMOUSESTATE mousestate;
	g_dinmouse->Acquire();
	g_dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);

	if( mousestate.rgbButtons[button] & 0x80 )
		return( true );
	return( false );
}

bool mouse_button_click( int button )
{
static DIMOUSESTATE mousestate;
	g_dinmouse->Acquire();
	g_dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);

	if( !g_button_down[button] && mousestate.rgbButtons[button] & 0x80 )
	{
		g_button_clicked[button] = true;
		return( true );
	}
	return( false );
}

void get_mouse_pos( int &x, int &y )
{
static DIMOUSESTATE mousestate;
	g_dinmouse->Acquire();
	g_dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);

	x = g_mousepos[0];
	y = g_mousepos[1];

#ifdef _DEBUG
	char msg[64];
	sprintf( msg, "%cMouse X:%c %d", S_COL_GREY, 1, g_mousepos[0] );
	g_mini_font_p->print_2d_text( msg, 8, 28, 1.0f, 0xffe0e000 );
	sprintf( msg, "%cMouse Y:%c %d", S_COL_GREY, 1, g_mousepos[1] );
	g_mini_font_p->print_2d_text( msg, 8, 38, 1.0f, 0xffe0e000 );
#endif
}


bool key_hit( unsigned int key )
{
	HWND focus = GetFocus();
	
	if(focus == g_hWnd)
	{
		if( g_keyheld[key] )
			return false;
		if( g_keypress[key] )
			return true;
		return false;
	}
}

bool key_down( unsigned int key )
{
	HWND focus = GetFocus();
	
	if(focus == g_hWnd)
	{
		if( g_keypress[key] )
			return true;
		return false;
	}
}


// close DirectInput
void cleanup_dinput( void )
{
	g_dinkeyboard->Unacquire();
	g_dinmouse->Unacquire();
	g_din->Release();

	return;
}