#include "timerSingleton.h"
#include "toolbars.h"
#include "input.h"

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

float CTimer::dayTime()
{
	if(m_dayTime >= 24) 
	{
		m_dayTime = 0;
	}

	return m_dayTime;

}

void CTimer::update(){

	//keep track of time passed and frame count
	m_currentTime = GetTickCount() * 0.001f;	
	m_timeBasedSpeed = (m_currentTime - m_lastTime);
	m_lastTime = m_currentTime;
	++m_frame;

	//If a second has passed
	if(m_currentTime-m_secondsTime>1.0f)
	{
		m_fps=m_frame/(m_currentTime-m_secondsTime);	//update the number of frames per second
		m_secondsTime=m_currentTime;				//set time for the start of the next count
		m_frame=0;					//reset fps for this second

		char fpstr[10];
		sprintf(fpstr,"%d",m_fps);
		HWND hSb = GetDlgItem(g_hWnd, IDC_MAIN_STATUS);
		SendMessage(hSb, SB_SETTEXT, 0, (LPARAM)fpstr);
	}
	
	m_dayTime += m_timeBasedSpeed / 15.0f;

	if(g_input.key['J'])
	{
		m_dayTime += 0.3;
	}

	//Sleep(200);
}

int CTimer::frameCounter(){return m_frame;};


void CTimer::init(){

	m_timeBasedSpeed = 0;
	m_lastTime = GetTickCount() * 0.001f;	
}



