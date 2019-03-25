/*=========================================================*\
|	input.cpp - input handler
|-----------------------------------------------------------|
|				Project :	PolySpoon Input
|				Coder	:	ADixon
|				Date	:	21/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "input.h"
#include "program.h"

/*======================== VARIABLES ======================*/

Input g_input_handler;

/*=========================================================*\
|	constructor - set states to false
\*=========================================================*/
Input::Input()
{
	for(int i = 0; i < 256; i++)
	{
		m_key_held[i] = false;
		m_key_pressed[i] = false;
	}

	for(int i = 0; i < 4; i++)
	{
		m_gamepad[i].connected = false;
		ZeroMemory( &m_gamepad[i].state, sizeof(XINPUT_STATE) );
	}

	m_wheel_movement = 0;
}

/*=========================================================*\
|	update - Gets all current input device states
\*=========================================================*/
void Input::update( WPARAM wParam, LPARAM lParam, unsigned int message )
{
	switch(message)
	{
		case WM_KEYDOWN:

			if(!m_key_held[wParam])
				m_key_pressed[wParam] = true;

		return;

		break;

		case WM_KEYUP:

			m_key_pressed[wParam] = false;
			m_key_held[wParam] = false;

			return;

		break;

		case WM_LBUTTONDOWN:

			m_mouse_button_pressed[LEFT] = true;

			return;

		break;

		case WM_LBUTTONUP:

			m_mouse_button_pressed[LEFT] = false;
			m_mouse_button_held[LEFT] = false;

			return;

		break;

		case WM_RBUTTONDOWN:

			m_mouse_button_pressed[RIGHT] = true;

			return;

		break;

		case WM_RBUTTONUP:

			m_mouse_button_pressed[RIGHT] = false;
			m_mouse_button_held[RIGHT] = false;

			return;

		break;

		case WM_MBUTTONDOWN:

			m_mouse_button_pressed[MIDDLE] = true;

			return;

		break;

		case WM_MBUTTONUP:

			m_mouse_button_pressed[MIDDLE] = false;
			m_mouse_button_held[MIDDLE] = false;

			return;

		break;

		case WM_MOUSEMOVE:

			m_last_mouse_position = g_input_handler.m_mouse_position;

			m_mouse_position.x = LOWORD(lParam);
			m_mouse_position.y = HIWORD(lParam);

			return;

		break;

		case WM_MOUSEWHEEL:

			m_wheel_movement -= (short) HIWORD(wParam);

        break;

	}
}

/*=========================================================*\
|	check_held_keys - Checks for held keys to seperate
|	key presses and key holds.
\*=========================================================*/
void Input::check_held_keys()
{
	for(int i = 0; i < 256; i++)
	{
		if(m_key_pressed[i])
		{
			m_key_pressed[i] = false;
			m_key_held[i] = true;
		}
	}

	for(int i = 0; i < 3; i++)
	{
		if(m_mouse_button_pressed[i])
		{
			m_mouse_button_pressed[i] = false;
			m_mouse_button_held[i] = true;
		}
	}


	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 14; j++)
		{
			if(m_gamepad[i].connected)
			{
				if(m_gamepad[i].button_pressed[j])
				{
					m_gamepad[i].button_pressed[j] = false;
					m_gamepad[i].button_held[j] = true;
				}
			}
		}
	}

	//flag back to zero each frame;
	m_wheel_movement = 0;

	m_last_mouse_position = m_mouse_position;
}

void Input::update_gamepads()
{
	for(int i = 0; i < 4; i++)
	{
		DWORD result;
	 
		ZeroMemory( &m_gamepad[i].state, sizeof(XINPUT_STATE) );
	 
		result = XInputGetState( i, &m_gamepad[i].state );
	 
		if( result == ERROR_SUCCESS )
		{
			m_gamepad[i].connected = true;

			//DPAD
			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) 
			{
				if(!m_gamepad[i].button_held[DPAD_UP])
				{
					m_gamepad[i].button_pressed[DPAD_UP] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[DPAD_UP] = false;
				m_gamepad[i].button_held[DPAD_UP] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) 
			{
				if(!m_gamepad[i].button_held[DPAD_DOWN])
				{
					m_gamepad[i].button_pressed[DPAD_DOWN] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[DPAD_DOWN] = false;
				m_gamepad[i].button_held[DPAD_DOWN] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) 
			{
				if(!m_gamepad[i].button_held[DPAD_LEFT])
				{
					m_gamepad[i].button_pressed[DPAD_LEFT] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[DPAD_LEFT] = false;
				m_gamepad[i].button_held[DPAD_LEFT] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) 
			{
				if(!m_gamepad[i].button_held[DPAD_RIGHT])
				{
					m_gamepad[i].button_pressed[DPAD_RIGHT] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[DPAD_RIGHT] = false;
				m_gamepad[i].button_held[DPAD_RIGHT] = false;
			}

			//start / back
			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_START) 
			{
				if(!m_gamepad[i].button_held[PAD_START])
				{
					m_gamepad[i].button_pressed[PAD_START] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_START] = false;
				m_gamepad[i].button_held[PAD_START] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) 
			{
				if(!m_gamepad[i].button_held[PAD_BACK])
				{
					m_gamepad[i].button_pressed[PAD_BACK] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_BACK] = false;
				m_gamepad[i].button_held[PAD_BACK] = false;
			}

			//thumbstick buttons
			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) 
			{
				if(!m_gamepad[i].button_held[PAD_LS])
				{
					m_gamepad[i].button_pressed[PAD_LS] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_LS] = false;
				m_gamepad[i].button_held[PAD_LS] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) 
			{
				if(!m_gamepad[i].button_held[PAD_RS])
				{
					m_gamepad[i].button_pressed[PAD_RS] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_RS] = false;
				m_gamepad[i].button_held[PAD_RS] = false;
			}

			//bumpers
			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) 
			{
				if(!m_gamepad[i].button_held[PAD_LB])
				{
					m_gamepad[i].button_pressed[PAD_LB] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_LB] = false;
				m_gamepad[i].button_held[PAD_LB] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
			{
				if(!m_gamepad[i].button_held[PAD_RB])
				{
					m_gamepad[i].button_pressed[PAD_RB] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_RB] = false;
				m_gamepad[i].button_held[PAD_RB] = false;
			}

			//normal buttons
			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_A) 
			{
				if(!m_gamepad[i].button_held[PAD_A])
				{
					m_gamepad[i].button_pressed[PAD_A] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_A] = false;
				m_gamepad[i].button_held[PAD_A] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_B) 
			{
				if(!m_gamepad[i].button_held[PAD_B])
				{
					m_gamepad[i].button_pressed[PAD_B] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_B] = false;
				m_gamepad[i].button_held[PAD_B] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_X) 
			{
				if(!m_gamepad[i].button_held[PAD_X])
				{
					m_gamepad[i].button_pressed[PAD_X] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_X] = false;
				m_gamepad[i].button_held[PAD_X] = false;
			}

			if(m_gamepad[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) 
			{
				if(!m_gamepad[i].button_held[PAD_Y])
				{
					m_gamepad[i].button_pressed[PAD_Y] = true;
				}
			}
			else
			{
				m_gamepad[i].button_pressed[PAD_Y] = false;
				m_gamepad[i].button_held[PAD_Y] = false;
			}

			//triggers
			m_gamepad[i].right_trigger = m_gamepad[i].state.Gamepad.bRightTrigger;
			m_gamepad[i].left_trigger = m_gamepad[i].state.Gamepad.bLeftTrigger;

			//thumb sticks
			if((m_gamepad[i].state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_gamepad[i].state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
            (	m_gamepad[i].state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_gamepad[i].state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
            {    
                  m_gamepad[i].state.Gamepad.sThumbLX = 0;
                  m_gamepad[i].state.Gamepad.sThumbLY = 0;
            }
 
			if((m_gamepad[i].state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				m_gamepad[i].state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
            (	m_gamepad[i].state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				m_gamepad[i].state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
            {
                  m_gamepad[i].state.Gamepad.sThumbRX = 0;
                  m_gamepad[i].state.Gamepad.sThumbRY = 0;
            }

			m_gamepad[i].left_thumbstick.x = m_gamepad[i].state.Gamepad.sThumbLX;
			m_gamepad[i].left_thumbstick.y = m_gamepad[i].state.Gamepad.sThumbLY;

			m_gamepad[i].right_thumbstick.x = m_gamepad[i].state.Gamepad.sThumbRX;
			m_gamepad[i].right_thumbstick.y = m_gamepad[i].state.Gamepad.sThumbRY;
		}
		else
		{
			m_gamepad[i].connected = false;
		}
	}
}

/*=========================================================*\
|	set_pad_vibration - set vibrations of either pad motor
|	use values between 0 and 1
\*=========================================================*/
void Input::set_pad_vibration(int pad, float left_motor, float right_motor)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	
	left_motor *= 65535.0f;
	right_motor *= 65535.0f;

	vibration.wLeftMotorSpeed = (WORD) left_motor;      
	vibration.wRightMotorSpeed = (WORD) right_motor; 

	XInputSetState( pad, &vibration );
}

void Input::stop_vibration(int pad)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );

	vibration.wLeftMotorSpeed = 0;      
	vibration.wRightMotorSpeed = 0; 

	XInputSetState( pad, &vibration );
}

Vector3f Input::colour_picking()
{
	Vector3f pixel_color;
	//create an unsigned char to store the pixel
	unsigned char pixel[3];

	//get the viewport dimensions from OpenGL
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	//read the pixels and store the result in the pixel unsigned char
	glReadPixels(m_mouse_position.x, (viewport[3]) - m_mouse_position.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	//store the pixel colour for later use
	pixel_color.x = pixel[0];
	pixel_color.y = pixel[1];
	pixel_color.z = pixel[2];

	return pixel_color;
}