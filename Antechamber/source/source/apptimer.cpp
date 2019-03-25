/****************************************
/	Al (c) 2008 - apptimer.cpp
/****************************************
/	frame stuffs
/****************************************/


/*************** INCLUDES ***************/

#include "apptimer.h"

/*************** CONSTANTS **************/

//#define		QUERY_TIMER_MODE

/*********** GLOBAL VARIABLES ***********/

#if QUERY_TIMER_MODE

float  timeAtGameStart;
UINT64 ticksPerSecond;

#else

float g_current_time;
float g_last_time;

#endif

/********* FUNCTION PROTOTYPES **********/



// Called once at the start of thegame
void init_game_time( void )
{
#if QUERY_TIMER_MODE

	if( !QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond) )
		ticksPerSecond = 1000;

	timeAtGameStart = get_game_time();

#else

	g_last_time = GetTickCount() * 0.001f;

#endif
}


// Called every time you need the current game time
float get_game_time( void )
{
#if QUERY_TIMER_MODE

	UINT64 ticks;
	float time;

	if( !QueryPerformanceCounter((LARGE_INTEGER *)&ticks) )
	ticks = 0;

	time = (float)(__int64)ticks/(float)(__int64)ticksPerSecond;

	time -= timeAtGameStart;
	return( time );

#else

	float time_based_speed;

	//keep track of time passed and frame count
	g_current_time = GetTickCount() * 0.001f;	
	time_based_speed = (g_current_time - g_last_time);
	g_last_time = g_current_time;

	return time_based_speed;

#endif
}
