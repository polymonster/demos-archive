#include "Helpers.h"
#include "Input.h"
#include "TimerSingleton.h"
#include "Fonts.h"

CConsole::CConsole()
{
	rootPos.x = 0;
	rootPos.y = 0;

	m_consoleOpen = false;
}

void CConsole::updateConsole()
{
	if((user::getInstance()->input.key['P']) && (!user::getInstance()->input.keyLocked['P']))
	{
		m_consoleOpen = !m_consoleOpen;
		user::getInstance()->input.keyLocked['P'] = true;
	}

	if(m_consoleOpen) displayConsole();
}

void CConsole::displayConsole()
{
	glColor3f(0.8, 0.8, 0.8);
	CText::getInstance()->output(100,50,"FPS : %d", CTimer::getInstance()->getFps());
	glColor3f(0.8, 0.8, 0.8);
	CText::getInstance()->output(100,60,"POS : %f%f", user::getInstance()->input.pos.x,user::getInstance()->input.pos.y);
}