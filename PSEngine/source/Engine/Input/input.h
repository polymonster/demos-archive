/*=========================================================*\
|	input.h - input handler
|-----------------------------------------------------------|
|				Project :	PolySpoon Input
|				Coder	:	ADixon
|				Date	:	21/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _INPUT_H
#define _INPUT_H

/*======================== INCLUDES =======================*/

#include <windows.h>
#include "XInput.h"
#include "virtual_codes.h"


#include "polyspoon_math.h"
#include "console.h"


#pragma comment ( lib, "XInput.lib" )

/*======================== CONSTANTS ======================*/

//enum for mouse buttons
typedef enum
{
	LEFT = 0,
	MIDDLE,
	RIGHT,

}e_mouse_buttons;

//enum for game pad buttons
typedef enum
{
	DPAD_UP = 0,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	PAD_START,
	PAD_BACK,
	PAD_LS,
	PAD_RS,
	PAD_LB,
	PAD_RB,
	PAD_A,
	PAD_B,
	PAD_X,
	PAD_Y,

}e_pad_buttons;


/*========================= CLASSES =======================*/

class Gamepad
{

public:

	XINPUT_STATE state;

	bool connected;
	bool button_pressed[14];
	bool button_held[14];

	float left_trigger;
	float right_trigger;

	Vector2f left_thumbstick;
	Vector2f right_thumbstick;
};


class Input
{

public:

	//keyboard
	bool			m_key_pressed[256];
	bool			m_key_held[256];

	//mouse
	Vector2i		m_mouse_position;
	Vector2i		m_last_mouse_position;
	float			m_wheel_movement;

	bool			m_mouse_button_pressed[3];
	bool			m_mouse_button_held[3];

	//game pad
	Gamepad			m_gamepad[4];

	//functions
	Input();
	void			update			( WPARAM wParam, LPARAM lParam, unsigned int message );
	void			update_gamepads	( void );
	void			check_held_keys	( void );

	void			set_pad_vibration(int pad, float left_motor, float right_motor);
	void			stop_vibration	(int pad);

	Vector3f		colour_picking  (       );

	//debug functions
	void			output_gamepads	( void );
	void			output_keyboard	( void );


};

/*================== EXTERNAL VARIABLES ===================*/

extern Input g_input_handler;


#endif //_INPUT_H


