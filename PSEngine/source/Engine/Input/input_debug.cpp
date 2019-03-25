/*=========================================================*\
|	input_debug.cpp - console output to debug input devices
|-----------------------------------------------------------|
|				Project :	PolySpoon Input
|				Coder	:	ADixon
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/
#include "input.h"

/*=========================================================*\
|	output_gamepads - output gamepad presses
\*=========================================================*/
void Input::output_gamepads()
{
	if(m_gamepad[0].button_pressed[PAD_Y])
	{
		g_console_p->dbgmsg( "pressed Y\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_X])
	{
		g_console_p->dbgmsg( "pressed X\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_A])
	{
		g_console_p->dbgmsg( "pressed A\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_B])
	{
		g_console_p->dbgmsg( "pressed B\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_RS])
	{
		g_console_p->dbgmsg( "pressed RS\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_LS])
	{
		g_console_p->dbgmsg( "pressed LS\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_RB])
	{
		g_console_p->dbgmsg( "pressed RB\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_LB])
	{
		g_console_p->dbgmsg( "pressed LB\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_START])
	{
		g_console_p->dbgmsg( "pressed Start\n" );
	}

	if(m_gamepad[0].button_pressed[PAD_BACK])
	{
		g_console_p->dbgmsg( "pressed Back\n" );
	}

	if(m_gamepad[0].button_pressed[DPAD_UP])
	{
		g_console_p->dbgmsg( "pressed DUP\n" );
	}

	if(m_gamepad[0].button_pressed[DPAD_DOWN])
	{
		g_console_p->dbgmsg( "pressed DDOWN\n" );
	}

	if(m_gamepad[0].button_pressed[DPAD_LEFT])
	{
		g_console_p->dbgmsg( "pressed DLEFT\n" );
	}

	if(m_gamepad[0].button_pressed[DPAD_RIGHT])
	{
		g_console_p->dbgmsg( "pressed DRIGHT\n" );
	}

	if(m_gamepad[0].left_thumbstick.x != 0 || m_gamepad[0].left_thumbstick.y != 0
		|| m_gamepad[0].right_thumbstick.x != 0 || m_gamepad[0].right_thumbstick.y != 0)
	{
		g_console_p->dbgmsg( "thumbs : L = %4g,%4g :: R = %4g,%4g\n", 
			m_gamepad[0].left_thumbstick.x,
			m_gamepad[0].left_thumbstick.y,
			m_gamepad[0].right_thumbstick.x,
			m_gamepad[0].right_thumbstick.y);
	}

	if(m_gamepad[0].left_trigger != 0 ||  m_gamepad[0].right_trigger != 0)
	{
		g_console_p->dbgmsg( "LT : %4g\n", m_gamepad[0].left_trigger);
		g_console_p->dbgmsg( "RT : %4g\n", m_gamepad[0].right_trigger);
	}
}