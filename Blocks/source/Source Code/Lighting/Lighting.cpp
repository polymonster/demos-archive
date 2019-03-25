#include "Lighting.h"

void setLights()
{
	float m_lightColor[3][4];

	m_lightColor[0][0] = 0.8;
	m_lightColor[0][1] = 0.6;
	m_lightColor[0][2] = 0.8;
	m_lightColor[0][3] = 1;

	m_lightColor[1][0] = 0.7;
	m_lightColor[1][1] = 0.6;
	m_lightColor[1][2] = 0.6;
	m_lightColor[1][3] = 1;

	m_lightColor[1][0] = 0.9;
	m_lightColor[1][1] = 0.9;
	m_lightColor[1][2] = 0.9;
	m_lightColor[1][3] = 1;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightColor[0]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightColor[1]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightColor[2]);

	float pos[4] = { 0, 50, 30, 1};

	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glEnable(GL_LIGHT0);

}