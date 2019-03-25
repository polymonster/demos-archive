#include "timerSingleton.h"

CTimer* CTimer::m_instance = NULL; 

void CTimer::create(){

	if(m_instance == NULL)
	{
		m_instance = new CTimer;
	}
}

void CTimer::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void CTimer::update(){

	//keep track of time passed and frame count
	m_currentTime=timeGetTime()*0.001f;
	m_timeBasedSpeed = (m_currentTime - m_lastTime);
	m_lastTime=m_currentTime;
	++m_frame;

	//If a second has passed
	if(m_currentTime-m_secondsTime>1.0f)
	{
		m_fps=m_frame/(m_currentTime-m_secondsTime);	//update the number of frames per second
		m_secondsTime=m_currentTime;				//set time for the start of the next count
		m_frame=0;					//reset fps for this second
		m_gravity = 10;

		updateGameTimer();
	}

	m_dayTime += m_timeBasedSpeed / 15.0f;
	

}

float CTimer::getGravity(){

	float ammount = -m_gravity;

	if(m_gravity > 0)
	{
		m_gravity = 0;
	}

	return ammount;

}

void CTimer::init(){

	m_dayTime = 8;
	m_day = 1;
	m_timeBasedSpeed = 0;
	m_lastTime = timeGetTime()*0.001f;

}

float CTimer::dayTime(){

	if(m_dayTime >= 24) 
	{
		m_day++;
		if(m_day > 7) m_day = 0;

		m_dayTime = 0;
	}

	return m_dayTime;

}

void CTimer::jumpTime(int hours)
{
	m_dayTime += hours;
}

void CTimer::startGameTimer(float minutes)
{
	m_gameTimer[0] = 0;
	m_gameTimer[1] = minutes;
	m_gameTimer[2] = 0;
	m_gameTimer[3] = 0;

	m_timerActive = true;
}

void CTimer::updateGameTimer()
{

			if(m_gameTimer[3] > 0) 
			{
				m_gameTimer[3]--;
			}
			else
			{
				m_gameTimer[3] = 9;

				if(m_gameTimer[2] > 0) 
				{
					m_gameTimer[2]--;
				}
				else
				{
					m_gameTimer[2] = 5;

					if(m_gameTimer[1] > 0) 
					{
						m_gameTimer[1]--;
					}
					else
					{
						m_gameTimer[1] = 9;

						if(m_gameTimer[0] > 0) 
						{
							m_gameTimer[0]--;
						}
						else
						{
							//over = true;	
							m_timerActive = false;
						}

					}
				}
			}
}


