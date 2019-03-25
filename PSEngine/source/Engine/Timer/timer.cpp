/*=========================================================*\
|	timer.cpp - timer class
|-----------------------------------------------------------|
|				Project :	PolySpoon Utilities
|				Coder	:	ADixon
|				Date	:	20/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#include "timer.h"
#include "window.h"

Timer g_game_timer;

Timer::Timer()
{
	m_last_time = GetTickCount() * 0.001f;
	second_counter = 0.0f;
}

void Timer::update()
{
	float current_time = GetTickCount() * 0.001f;
	m_time_step = current_time - m_last_time;

	m_total_time += (current_time * 1000.0f);

	second_counter += m_time_step;
	m_elapsed_time += m_time_step;

	m_frame_counter++;

	if(second_counter > 1.0f)
	{
		m_fps = m_frame_counter;

		//fps count reset
		second_counter = 0.0f;
		m_fps = 0;
	}

	m_anim_time = false;
	m_elapsed_time_anim += m_time_step;
	if(m_elapsed_time_anim > 1.0f / (float) ANIM_RATE * 0.5f)
	{
		m_elapsed_time_anim = 0.0f;
		m_anim_time = true;
	}

	m_last_time = current_time;
}

bool Timer::time_to_render()
{
	if(m_elapsed_time > 1.0f / (float) FRAME_RATE * 0.5f )
	{
		m_elapsed_time = 0.0f;
		return true;
	}

	return false;
}