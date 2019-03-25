/*=========================================================*\
|	timer.h - timer class
|-----------------------------------------------------------|
|				Project :	PolySpoon Utilities
|				Coder	:	ADixon
|				Date	:	20/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef timer_h__
#define timer_h__

#define FRAME_RATE 30.0
#define ANIM_RATE 24.0

class Timer
{
	public:

		Timer();

		void update();
		bool time_to_render();
		bool time_to_animate(){return m_anim_time;};

		float get_game_time(){ return m_time_step; };
		int get_fps(){ return m_fps; };

		long total_time(){return m_total_time; };

	private:

		float m_time_step;
		float m_last_time;

		float m_elapsed_time;
		float second_counter;

		int m_frame_counter;
		int m_fps;

		bool m_anim_time;
		float m_elapsed_time_anim;

		long m_total_time;
};

extern Timer g_game_timer;


#endif // timer_h__