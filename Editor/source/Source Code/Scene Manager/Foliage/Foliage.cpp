#include "props.h"
#include "input.h"
#include "modelManager.h"
#include "shaderManager.h"
#include "helpers.h"
#include "foliage.h"
#include "textureSingleton.h"

CFoliageNode::CFoliageNode(){};

CFoliageNode::CFoliageNode(SVector3 position,int id)
{
	m_position = position;
	m_id = id;
	nameIdentifier = "foliage";
	m_lod = 0;
}

void CFoliageNode::render()
{
	SVector3 camPos = user::getInstance()->input.pos;

	CShaderManager::getInstance()->attachShader(g_shaderId.normalMap);

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "decal"), 1);
    glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "shadowMap"), 0);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "normalMap"), 2);
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "lightPos"), 0,0,0, 1.0f);
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "cameraPos"), camPos.x, camPos.y, camPos.z, 1.0f);

	glUniform3fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "translate"), m_position.x, m_position.y, m_position.z);
	glUniform3fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "scale"), 1, 1, 1);

	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);

	CModelManager::getInstance()->render(m_id + m_lod);

	glPopMatrix();

	CModelManager::getInstance()->getBox(m_id,&m_boundingBoxPos,&m_boundingBoxScale);

	m_boundingBoxPos = m_boundingBoxPos + m_position;
	m_boundingBoxScale = m_boundingBoxScale;

	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
		glTranslated(m_boundingBoxPos.x,m_boundingBoxPos.y,m_boundingBoxPos.z);
		glScaled(m_boundingBoxScale.x,m_boundingBoxScale.y,m_boundingBoxScale.z);
		cube();
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

}

CFoliage::CFoliage()
{
	m_currentModel = 0;
	nameIdentifier = "foliage";
}

void CFoliage::render()
{
	for(int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes.at(i)->render();
	}

	if(user::getInstance()->input.key['V'])
	{
		std::ofstream fout;
			fout.open("size.txt");
			fout << m_nodes.size();
		fout.close();
	}

}

void CFoliage::execute()
{

}


