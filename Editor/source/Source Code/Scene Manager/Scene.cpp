#include "Scene.h"
#include "Fonts.h"
#include "TimerSingleton.h"
#include "modelManager.h"
#include "textureSingleton.h"
#include "water.h"
#include "toolbars.h"
#include "atmosphere.h"
#include "lighting.h"
#include <iostream>

//Globals
int g_editMode;
SPolyDetails g_polyCounter;

CFBO *example;

bool g_loadNewScene = false;

int fu_id;

//Functions
CScene::CScene()
{
	initOpenGL();
	initSingletons();

	m_atmosphere = NULL;

	g_editMode = 0;
}

CScene::~CScene()
{
	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::vector<CSceneObject*>::iterator remove = m_sceneObject.begin() + i;
		delete m_sceneObject.at(i);
		m_sceneObject.erase(remove);
	}

	m_sceneObject.clear();

	CModelManager::getInstance()->cleanUp();
	CShaderManager::getInstance()->cleanUp();
}

void CScene::initSingletons()
{
	g_polyCounter.polyCount = 0;
	g_polyCounter.polyBudget = 100000;

	CText::getInstance()->create();
	CText::getInstance()->createFont("Raster Fonts",14,g_hDC);

	user::getInstance()->create();
	user::getInstance()->setStates();

	CTimer::getInstance()->create();
	CTimer::getInstance()->init();

	setLights();

	CShaderManager::getInstance()->create();

	CShaderManager::getInstance()->setup();

	// Bind the shaders variables to our variables.
	CModelManager::getInstance()->create();
	
	CTextureMan::getInstance()->create();
	CTextureMan::getInstance()->loadBasePack();

	lights.x = 0;
	lights.y = 100;
	lights.z = 0;

	m_sceneObject.resize(m_sceneObject.size() + 1);
	m_sceneObject.at(m_sceneObject.size() - 1) = new CView();
	m_sceneObject.resize(m_sceneObject.size() + 1);
	m_sceneObject.at(m_sceneObject.size() - 1) = new CPropList();

	octree = new COctreeNode(SVector3(0,0,0),SVector3(1000,1000,1000));

	createCubeVBO();

	example = new CFBO(0,1024,GL_CLAMP);

	fu_id = CTextureMan::getInstance()->load("data/fur.tga",0);
}


void CScene::initOpenGL()
{
	//set some generic OpenGL parameters
	glEnable (GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable (GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glShadeModel (GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
}

void CScene::update()
{
		CTimer::getInstance()->update();

		user::getInstance()->updatePos();
		user::getInstance()->mouseLook();

		if(g_input.key['1'])
		{
			g_editMode = 1;
		}

		if(g_input.key['2'])
		{
			g_editMode = 2;
		}

		if(g_input.key['3'])
		{
			g_editMode = 3;
		}

		if(g_input.key['4'])
		{
			g_editMode = 4;
		}

		if(g_input.key['5'])
		{
			g_editMode = 5;
		}

		if(g_input.key['0'])
		{
			g_editMode = 0;
		}

		if((g_input.key[VK_Z]) && (g_input.key[VK_CONTROL])
			&& (!g_input.keyLocked[VK_Z]))
		{
			CTerrain *terrain = (CTerrain*) m_sceneObject.at(0);
			terrain->command(&g_undoStack,&g_redoStack);
			g_input.keyLocked[VK_Z] = true;
		}

		/*if((g_input.key['C']) && (g_input.functionKey[KEY_CTRL])
			&& (!g_input.keyLocked['C']))
		{
			//m_terrain->command(&g_redoStack,&g_undoStack);
			//g_input.keyLocked['C'] = true;
		}*/
		
		for(int i = 0; i < m_sceneObject.size(); i++)
		{
			m_sceneObject.at(i)->execute();
		}

		octree->execute();

		addObjects();

	HWND button = GetDlgItem(g_hMainTool,IDC_LOAD);
	if(SendMessage(button,BM_GETSTATE,0,0) & BST_PUSHED)
	{
			load();
	}

	button = GetDlgItem(g_hMainTool,IDC_SAVE2);
	if(SendMessage(button,BM_GETSTATE,0,0) & BST_PUSHED)
	{
			save();
	}

	if(g_loadNewScene) 
	{
		g_loadNewScene = false;
		load();
	}
}

void CScene::buildOctree()
{
	CPropList *prop;

	bool gotProps = false;

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "props") 
		{
			prop = (CPropList*) m_sceneObject.at(i);
			gotProps = true;
		}
	}

	octree->clear();

	if(gotProps)
	{
		for(int i = 0; i < prop->m_nodes.size(); i++)
		{
			octree->addObject(prop->m_nodes.at(i));
		}
	}

	octree->subdivide();
}

void CScene::render()
{
	//set up the rendering environment
	float aspect = (float) g_screenWidth/g_screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect, 0.6f, 30000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear the buffers before drawing
	quickClear();

	glLoadIdentity();

	user::getInstance()->setCameraPos();

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		m_sceneObject.at(i)->backBufferRender();
	}

	octree->backBufferRender();

	quickClear();

	glLoadIdentity();

	//render the core scene

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	user::getInstance()->setCameraPos();

	
	renderCore();

	//"post-scene" no light or culling or shaders

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	renderAux();

}

void CScene::waterRenders()
{
	int numberOfWater = 0;
	int waterId[32];

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::string name = m_sceneObject.at(i)->nameIdentifier;

		if(name== "water")
		{
			waterId[numberOfWater++] = i;
		}
	}


	for(int i = 0; i < numberOfWater; i++)
	{
		CWaterPlane *water = (CWaterPlane*) m_sceneObject.at(waterId[i]);

		if(water->m_localReflections == 1)
		{
			//REFLECTION

			water->reflectionBegin();

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				std::string name = m_sceneObject.at(i)->nameIdentifier;

				if(name == "terrain" || name == "atmosphere")
				{
					m_sceneObject.at(i)->render();
				}
			}

			renderProps();

			water->reflectionEnd();

			//REFRACTION

			water->refractionBegin();

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				std::string name = m_sceneObject.at(i)->nameIdentifier;

				if(name == "terrain")
				{
					m_sceneObject.at(i)->render();
				}
			}

			renderProps();

			water->refractionEnd();
		}
	}
}

void CScene::shadowRenders()
{
	if(m_atmosphere != NULL)
	{	
		if(!m_atmosphere->renderShadowMap()) return;
		m_atmosphere->shadowMapBegin();
	}

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::string name = m_sceneObject.at(i)->nameIdentifier;

		if(name== "terrain")
		{
			m_sceneObject.at(i)->render();
		}
	}

	renderProps();

	if(m_atmosphere != NULL)
	{
		m_atmosphere->shadowMapEnd();
	}
}

void CScene::renderCore()
{
	waterRenders();

	shadowRenders();

	//render main scene
	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::string name = m_sceneObject.at(i)->nameIdentifier;

			if(name== "terrain")
			{
				glDisable(GL_BLEND);
				
				if(m_atmosphere != NULL)
				{
					m_atmosphere->bindShadowMap();
				}

				m_sceneObject.at(i)->render();

				if(m_atmosphere != NULL)
				{
					m_atmosphere->unbindShadowMap();
				}
			}
			else if(name == "atmosphere")
			{
				m_sceneObject.at(i)->render();
			}
	}

	if(m_atmosphere != NULL)
	{
		m_atmosphere->bindShadowMap();
	}

	renderProps();

	if(m_atmosphere != NULL)
	{
		m_atmosphere->unbindShadowMap();
	}

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::string name = m_sceneObject.at(i)->nameIdentifier;

		if(name == "water")
		{
			m_sceneObject.at(i)->render();
		}
	}

	if(m_atmosphere != NULL)
	{
		if(g_editMode == EDIT_ATMOSPHERE || g_editMode == NO_EDIT_MODE) 
		{
			if(m_atmosphere->m_renderBloom)
			{
				testBloom();
			}
		}
	}

	CShaderManager::getInstance()->detachShader();

	//fur test
	/*glScaled(20,20,20);

		CTextureMan::getInstance()->bindTexture(0,0);
		CTextureMan::getInstance()->bindTexture(0,1);

		cube();

	glPopMatrix();

	for(int i = 0; i < 20; i++)
	{
		glPushMatrix();
		
		glScaled(20 + (i * 0.1),20 + (i * 0.1),20 + (i * 0.1));

		CTextureMan::getInstance()->bindTexture(fu_id,0);
		CTextureMan::getInstance()->bindTexture(fu_id,1);

		cube();

		glPopMatrix();
	}*/
}

void CScene::testBloom()
{
	//TEST BLUR
	m_atmosphere->m_blur[0]->bindFBO();

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::string name = m_sceneObject.at(i)->nameIdentifier;

		if(name== "water" || name== "terrain" || name == "atmosphere")
		{
			m_sceneObject.at(i)->render();
		}
	}

	renderProps();

	m_atmosphere->m_blur[0]->unbindFBO();

	//m_blur[0]->copyScreenBuffer();

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	CShaderManager::getInstance()->attachShader(g_shaderId.brightPass);

	m_atmosphere->m_blur[1]->bindFBO();

		glPushMatrix();
		
		orthoMode(0,1024,1024,0);

		m_atmosphere->m_blur[0]->bindAsTexture(0);
		m_atmosphere->m_blur[0]->bindAsTexture(1);

		glDisable(GL_LIGHTING);

		glUniform1iARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.brightPass), 
		"decal"), 0);

		glPushMatrix();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glTranslated(1024 / 2,1024 / 2,0);
		glScaled(1024 / 2,1024 / 2,1);
		texturedQuad();
		glPopMatrix();

	m_atmosphere->m_blur[1]->unbindFBO();

	CShaderManager::getInstance()->detachShader();

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	CShaderManager::getInstance()->attachShader(g_shaderId.bloom);

	for(int i = 0; i < m_atmosphere->m_blurPasses; i ++)
	{
		m_atmosphere->m_blur[0]->bindFBO();
					
			m_atmosphere->m_blur[1]->bindAsTexture(0);
			m_atmosphere->m_blur[1]->bindAsTexture(1);

			glDisable(GL_LIGHTING);

			glUniform1iARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"decal"), 0);

			glUniform2fARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"u_scale"), 0,1.0f / 1024.0f);

			glPushMatrix();
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glTranslated(1024 / 2,1024 / 2,0);
			glScaled(1024 / 2,1024 / 2,1);
			texturedQuad();
			glPopMatrix();

		m_atmosphere->m_blur[0]->unbindFBO();

		m_atmosphere->m_blur[1]->bindFBO();
					
			m_atmosphere->m_blur[0]->bindAsTexture(0);
			m_atmosphere->m_blur[0]->bindAsTexture(1);

			glDisable(GL_LIGHTING);

			glUniform1iARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"decal"), 0);

			glUniform2fARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"u_scale"), 1.0f / 1024.0f,0);

			glPushMatrix();
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glTranslated(1024 / 2,1024 / 2,0);
			glScaled(1024 / 2,1024 / 2,1);
			texturedQuad();
			glPopMatrix();

		m_atmosphere->m_blur[1]->unbindFBO();
	}

	CShaderManager::getInstance()->detachShader();

	CTextureMan::getInstance()->disableAllUnits();

	m_atmosphere->m_blur[1]->bindAsTexture(1);
	m_atmosphere->m_blur[1]->bindAsTexture(0);

	glBlendFunc(GL_ONE,GL_ONE);
	glEnable(GL_BLEND);

	SVector3 colour = m_atmosphere->m_color;
				
	glPushMatrix();
	glColor4f(colour.x,colour.y,colour.z,m_atmosphere->m_luminance);
	glTranslated(1024 / 2,1024 / 2,0);
	glScaled(1024 / 2,1024 / 2,1);
	texturedQuad();
	glPopMatrix();

	perspectiveMode();

	glPopMatrix();

	CTextureMan::getInstance()->disableAllUnits();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

}
void CScene::renderProps()
{
	if(octree->m_use)
	{
		//if we are using the octree, then get the current 
		//view frustum to test against
		g_frustumCulling.CalculateFrustum();

		//clear render flags to false, so we can remove redundant renders
		//of multiple objects
		octree->clearRenderFlags();

		//finally call the octree, it will recurse though itself rendering
		//every node
		octree->render();
	}
	else
	{
		for(int i = 0; i < m_sceneObject.size(); i++)
		{
			std::string name = m_sceneObject.at(i)->nameIdentifier;

			//if were not using the octree then just render
			//props and foliage by brute force
			if(name == "props")
			{
				m_sceneObject.at(i)->render();
			}
		}

	}
}

void CScene::renderAux()
{
	CShaderManager::getInstance()->detachShader();
	CTextureMan::getInstance()->disableAllUnits();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		m_sceneObject.at(i)->renderAux();
	}

	if(octree->m_draw)
	{
		octree->draw();
	}

	octree->renderAux();

	if(octree->m_build)
	{
		octree->m_build = false;
		buildOctree();
	}
}

void CScene::cleanUp()
{
}




