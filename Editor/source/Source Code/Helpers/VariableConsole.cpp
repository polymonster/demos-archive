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
	if((g_input.key['P']) && (!g_input.keyLocked['P']))
	{
		m_consoleOpen = !m_consoleOpen;
		g_input.keyLocked['P'] = true;
	}

	if(m_consoleOpen) displayConsole();
}

void CConsole::displayConsole()
{

	glColor3f(0.8, 0.8, 0.8);

	CText::getInstance()->output(100,50,"FPS : %d", CTimer::getInstance()->getFps());

	CText::getInstance()->output(10,70,"Mouse Pos :%d,%d", g_input.mousePos.x, g_input.mousePos.y);

	CText::getInstance()->output(10,90,"Mouse 3D Pos :%f,%f,%f", g_input.mouse3dPos.x, 
														   g_input.mouse3dPos.y,
														   g_input.mouse3dPos.z);

	CText::getInstance()->output(10,110,"Screen Dims :%d,%d",g_screenWidth,g_screenHeight);

	CText::getInstance()->output(10,120,"Mouse Button :%i | %i | %i", g_input.leftMouse,
																	g_input.middleMouse,
																	g_input.rightMouse);

	CText::getInstance()->output(10,130,"Inc :%f",g_input.mouseIncrement.x);

	CText::getInstance()->output(10,140,"3D Camera Pos :%f,%f,%f", g_input.pos.x, 
														   g_input.pos.y,
														   g_input.pos.z);
}