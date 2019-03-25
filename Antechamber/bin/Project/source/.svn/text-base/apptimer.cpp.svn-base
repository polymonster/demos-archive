/****************************************
/	Al (c) 2008 - apptimer.cpp
/****************************************
/	frame stuffs
/****************************************/


/*************** INCLUDES ***************/

#include "apptimer.h"

/*************** CONSTANTS **************/


/*********** GLOBAL VARIABLES ***********/

float  timeAtGameStart;
UINT64 ticksPerSecond;

float m_currentTime;
float m_lastTime;


/********* FUNCTION PROTOTYPES **********/



// Called once at the start of thegame
void init_game_time( void )
{
  // We need to know how often the clock is updated
  if( !QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond) )
    ticksPerSecond = 1000;
  // If timeAtGameStart is 0 then we get the time since
  // the start of the computer when we call GetGameTime()
  timeAtGameStart = 0;
  timeAtGameStart = get_game_time();

  m_lastTime = GetTickCount() * 0.001f;
}


// Called every time you need the current game time
float get_game_time( void )
{
  /*UINT64 ticks;
  float time;
  // This is the number of clock ticks since start
  if( !QueryPerformanceCounter((LARGE_INTEGER *)&ticks) )
    ticks = 0;
  // Divide by frequency to get the time in seconds
  time = (float)(__int64)ticks/(float)(__int64)ticksPerSecond;
  // Subtract the time at game start to get
  // the time since the game started
  time -= timeAtGameStart;
  return( time );*/

	float m_timeBasedSpeed;

	//keep track of time passed and frame count
	m_currentTime = GetTickCount() * 0.001f;	
	m_timeBasedSpeed = (m_currentTime - m_lastTime);
	m_lastTime = m_currentTime;

	return m_timeBasedSpeed;
}
