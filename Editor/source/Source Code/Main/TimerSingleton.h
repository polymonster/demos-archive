#ifndef _TIMER_SINGLETON_H
#define _TIMER_SINGLETON_H

#include <time.h>
#include "globals.h"

class CTimer{

public:

	static void create();
	static CTimer* getInstance(){return m_instance;};
	static void destroy();

	int getFps(){return m_fps;};
	float animBaseSpeed(){return m_timeBasedSpeed;};
	float time(){return m_currentTime;};
	void update();
	void init();

	float dayTime();

	int frameCounter();

private:

	CTimer(){m_secondsTime = 0,
			 m_lastTime = 0,
			 m_currentTime = 0,
			 m_fps = 0,
			 m_frame = 0;
			 m_sleep = 0;
			 m_dayTime = 0;
	};

	~CTimer(){};

	static CTimer *m_instance;

	float m_timeBasedSpeed;
	int m_fps;
	int m_frame;
	float m_secondsTime;
	float m_lastTime;
	float m_currentTime;
	int m_sleep;
	float m_dayTime;

	int m_frameCounter;

};


#endif 