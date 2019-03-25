#include "toyBlock.h"
#include "helpers.h"
#include "input.h"
#include "timerSingleton.h"
#include "textureSingleton.h"

CToyBlock::CToyBlock(unsigned int id)
{
	m_position.x = rand()%300 - 150.0f;
	m_position.z = rand()%300 - 150.0f;
	m_position.y = 100.0f;

	m_newPosition = m_position;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_newRotation.x = 0.0f;
	m_newRotation.y = 0.0f;
	m_newRotation.z = 0.0f;

	m_scale.x = 10.0f;
	m_scale.y = 10.0f;
	m_scale.z = 10.0f;

	m_selected = false;
	m_id = id;
	m_velocity = 0;

	m_colorId = rand()%10;
}

CToyBlock::~CToyBlock(){}

void CToyBlock::updateThisRecursion(bool update)
{
	m_updateThisRecursion = update;
}

void CToyBlock::collisionBelow(bool update,float offset)
{
	m_collisionBelow = update;
	m_collisionBelowOffset = offset;
}

void CToyBlock::ignoreYAxis(bool ignore)
{
	m_ignoreYAxis = ignore;
}

void CToyBlock::clearRenderFlags()
{
	m_renderedThisPass = false;
}

void CToyBlock::setRenderFlags(bool rendered)
{
	m_renderedThisPass = rendered;
}

void CToyBlock::render()
{

	SVector3 col = getColor(m_colorId);

	glUniform4fARB(glGetUniformLocationARB(
	CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "materialColor"), 
	col.x, col.y, col.z, 1.0f);

	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);
	glRotated(m_rotation.y,0,1,0);
	glScaled(m_scale.x,m_scale.y,m_scale.z);

	glColor3f(1,0,1);

	glEnable(GL_LIGHTING);

	CTextureMan::getInstance()->bindTexture(0,1);
	CTextureMan::getInstance()->bindTexture(1,2);

	cubeVBO();

	CTextureMan::getInstance()->disableUnit(2);

	glPopMatrix();
}

void CToyBlock::renderSelectedBox()
{
		glPushMatrix();

		glTranslated(m_position.x,m_position.y,m_position.z);
		glRotated(m_rotation.y,0,1,0);
		glScaled(m_scale.x,m_scale.y,m_scale.z);
		glScaled(1.25f,1.25f,1.25f);

		glDisable(GL_LIGHTING);

		glColor3f(1,1,1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		cubePickingVBO();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPopMatrix();

}

void CToyBlock::execute()
{
	if(m_selected)
	{
		
		if(user::getInstance()->input.key['T'])
		{
			useTranslateWidget(&m_selectedAxis,&m_newPosition);
		}
		else if(user::getInstance()->input.key['Y'])
		{
			useRotateWidget(&m_selectedAxis,&m_newPosition,&m_newRotation);
		}
	}
}

void CToyBlock::renderBackBuffer()
{
	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);
	glRotated(m_rotation.y,0,1,0);
	glScaled(m_scale.x,m_scale.y,m_scale.z);

	glColor3ub(m_id,0,0);

	glDisable(GL_LIGHTING);

	cubePickingVBO();

	glPopMatrix();

}

void CToyBlock::incrementPositions(SVector3 pos,SVector3 rot, SVector3 scale)
{
	m_newPosition += pos;
	m_rotation += rot;
	m_scale += scale;
}

void CToyBlock::updatePosition(bool update)
{
	if(m_updateThisRecursion) 
	{
		m_position = m_newPosition;
		m_rotation = m_newRotation;
	}
	else 
	{
		if(!m_ignoreYAxis)
		{
			if(m_collisionBelow)
			{
				m_position.y = m_newPosition.y - m_collisionBelowOffset;
				m_newPosition = m_position;
				m_newRotation = m_rotation;

				m_velocity = 0;
			}
			else
			{
				m_newPosition.x = m_position.x;
				m_newPosition.z = m_position.z;
			}
		}
		else
		{
			m_newPosition = m_position;
			m_newRotation = m_rotation;
			m_velocity = 0;
		}
	}

	if(m_position.y - m_scale.y < 0)
	{
		m_position.y = m_scale.y - 1;
		m_velocity = 0;
		m_newPosition = m_position;
	}
}

void CToyBlock::offsetY(SVector3 offset)
{
	if(offset.y != 0)
	{
		m_newPosition.y += offset.y;
		//m_position.y = m_newPosition.y;
		m_velocity = 0;
	}
}

void CToyBlock::updatePosition(SVector3 offset)
{
	m_position = m_newPosition + offset;

	if(m_position.y - m_scale.y < 0)
	{
		m_position.y = m_scale.y;
		m_velocity = 0;
		m_newPosition = m_position;
	}
}

void CToyBlock::gravity()
{
	if((!m_selected) || (!user::getInstance()->input.key['T']))
	{
		m_newPosition.y -= CTimer::getInstance()->animBaseSpeed() * m_velocity * GRAVITY;
		if(m_velocity < MAX_VELOCITY) m_velocity += CTimer::getInstance()->animBaseSpeed() * 25.0f;
	}
}

void CToyBlock::renderAux()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	if(m_selected)
	{
		if(user::getInstance()->input.key['T'])
		{
			renderSelectedBox();
			translateWidget(m_position,m_selectedAxis);
			
		}
		else if(user::getInstance()->input.key['Y'])
		{
			renderSelectedBox();
			rotateWidget(m_position,m_selectedAxis);
			
		}
		else if(user::getInstance()->input.key['R'])
		{
			renderSelectedBox();
		}
	}

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}