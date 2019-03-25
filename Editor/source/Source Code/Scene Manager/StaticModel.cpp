#include "Scene.h"

#include "Fonts.h"
#include "TimerSingleton.h"
#include "modelManager.h"
#include "textureSingleton.h"

CStaticModel::CStaticModel(const char *file)
{
	//m_prop.modelManagerId = CModelManager::getInstance()->load("Evil_Tree.3ds");

	SVector3 startPos(0,0,0);
	SVector3 startScale(1,1,1);

	m_prop.pos = startPos;
	m_prop.scale = startScale;
}

CStaticModel::CStaticModel(int id)
{
	//m_prop.modelManagerId = CModelManager::getInstance()->load("Evil_Tree.3ds");

	SVector3 startPos(0,0,0);
	SVector3 startScale(1,1,1);

	m_prop.pos = startPos;
	m_prop.scale = startScale;
}

void CStaticModel::renderModel()
{
	glPushMatrix();

	//flip it 90 because of max's "z-up" system
	//glRotated(-90,1,0,0);

	quickTranslate(m_prop.pos);
	quickScale(m_prop.scale);

	CModelManager::getInstance()->render(m_prop.modelManagerId);

	glPopMatrix();
}