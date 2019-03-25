#include "atmosphere.h"
#include "helpers.h"
#include "textureSingleton.h"
#include "timerSingleton.h"
#include "shaderManager.h"

int g_atmosphereEditMode = -1;

CAtmosphere::CAtmosphere()
{
	nameIdentifier = "atmosphere";
	m_pushed = false;
	m_renderBloom = false;
	m_realTimeShadows = false;
	m_blurPasses = 1;
	m_renderInitialShadow = false;

	m_shadowMap = new CFBO(1,1024,GL_CLAMP_TO_BORDER);

	m_blur[0] = new CFBO(0,512,GL_CLAMP_TO_EDGE);
	m_blur[1] = new CFBO(0,512,GL_CLAMP_TO_EDGE);

	m_shadowOffset = 0.2f;

	m_shadowMapSize = 1024;

	m_color = SVector3(1,1,1);

	m_position = SVector3(0,0,0);
	m_selectedAxis = SVector3(0,0,0);
	m_scale = SVector3(1000,1000,1000);

	m_cubeMapID = -1;
}

CAtmosphere::CAtmosphere(std::ifstream &fin)
{
	nameIdentifier = "atmosphere";
	m_pushed = false;
	m_selectedAxis = SVector3(0,0,0);

	char temp[255];

	fin >> m_position.x >> m_position.y >> m_position.z;
	fin >> m_scale.x >> m_scale.y >> m_scale.z;
	fin >> temp;
	fin >> m_color.x >> m_color.y >> m_color.z >> m_luminance >> m_bloomSize >> m_renderBloom;
	fin >> m_shadowPosition.x >> m_shadowPosition.y >> m_shadowPosition.z;
	fin >> m_shadowRotation.x >> m_shadowRotation.y;
	fin >> m_shadowOffset >> m_shadowMapSize >> m_realTimeShadows;

	m_shadowMap = new CFBO(1,m_shadowMapSize,GL_CLAMP_TO_BORDER);
	m_blur[0] = new CFBO(0,m_bloomSize,GL_CLAMP_TO_EDGE);
	m_blur[1] = new CFBO(0,m_bloomSize,GL_CLAMP_TO_EDGE);

	m_renderBloom = true;

	m_renderInitialShadow = true;

	m_blurPasses = 1;

	m_cubeMapID = 0;

	HWND shadowOffset = GetDlgItem(g_hAtmosphereEnv, IDC_SHADOW_OFFSET);
	int pos = m_shadowOffset * 50.0f;
	SendMessage(shadowOffset, TBM_SETPOS, 1, pos);
}

CAtmosphere::~CAtmosphere()
{
	delete m_shadowMap;
	delete m_blur[0];
	delete m_blur[1];
}

void CAtmosphere::render()
{
	//dayNightCycle();

	//skyDome();

	skyBox();
}

void CAtmosphere::skyDome()
{
	glDisable(GL_LIGHTING);

	CShaderManager::getInstance()->attachShader(g_shaderId.skydome);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	CTextureMan::getInstance()->disableAllUnits();
	CTextureMan::getInstance()->bindTexture(9,1);
	CTextureMan::getInstance()->bindTexture(10,2);

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.skydome), "sky"), 1);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.skydome), "stars"), 2);

	glPushMatrix();

	glTranslated(0,-50,0);

	glRotated(90,0,0,1);

	glScaled(1000,1000,1000);

	float skyboxblend = m_daylight;
	if(skyboxblend < 0.4) skyboxblend = 0.4;

	glColor4f(m_atmosphereColour.x,m_atmosphereColour.y,m_atmosphereColour.z,skyboxblend);

	sphere();

	//GenerateDome(1000.0f, 5.0f, 5.0f, 1.0f, 1.0f);

	glPopMatrix();
	

	glCullFace(GL_BACK);
}

void CAtmosphere::skyBox()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	CTextureMan::getInstance()->disableAllUnits();

	if(m_cubeMapID != -1)
	{
		CShaderManager::getInstance()->attachShader(g_shaderId.skybox);

		CTextureMan::getInstance()->bindCubeMap(m_cubeMapID);

		glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.skybox), "cubeMap"), 7);
	}
	else
	{	
		CShaderManager::getInstance()->detachShader();
		glColor4f(0,0,0,1);
	}

	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);
	glScaled(m_scale.x,m_scale.y,m_scale.z);

	cubeVBO();

	glPopMatrix();

	glCullFace(GL_BACK);

	CShaderManager::getInstance()->detachShader();
	CTextureMan::getInstance()->unbindCubeMap();
}

void CAtmosphere::dayNightCycle()
{
	float time = CTimer::getInstance()->dayTime();

	m_actualLight = m_daylight;

	if((time > 6) && (time < 20))
	{
		m_daylight = 1;
	}

	if((time > 6) && (time < 17))
	{
		m_atmosphereColour.x = m_daylight;
		m_atmosphereColour.y = m_daylight;
		m_atmosphereColour.z = m_daylight;
	}

	if((time >= 3) && (time <= 4))
	{
		m_atmosphereColour.x = (time - 3) / 5;
		m_atmosphereColour.y = (time - 3) / 4;
		m_atmosphereColour.z = (time - 3) / 3;
	}

	if((time >= 4) && (time <= 6))
	{
		float oldLight = m_daylight;

		m_daylight = (time - 4) / 2;

		float diff = m_daylight - oldLight;

		m_atmosphereColour.x += (float) diff;
		m_atmosphereColour.y += (float) diff;
		m_atmosphereColour.z += (float) diff;
	}

	if((time >= 17) && (time <= 20))
	{
		m_atmosphereColour.x = 1 - ((time - 17) / 3.5);
		m_atmosphereColour.y = 1 - ((time - 17) / 2.2);
		m_atmosphereColour.z = 1 - ((time - 17) / 1.4);

		if(m_atmosphereColour.x  < 0.9) m_atmosphereColour.x = 0.88;
		if(m_atmosphereColour.y  < 0.68) m_atmosphereColour.y = 0.68;
		if(m_atmosphereColour.z  < 0.55) m_atmosphereColour.z = 0.55;
	}

	//fade to night
	if((time >= 20) && (time <= 22))
	{
		float oldLight = m_daylight;

		m_daylight = 1 - ((time - 20) / 2);

		float diff = m_daylight - oldLight;

		m_atmosphereColour.x += (float) diff;
		m_atmosphereColour.y += (float) diff;
		m_atmosphereColour.z += (float) diff;
	}
		

	if((time > 22) && (time + 24 < 28))
	{
		m_daylight = 0;

		m_atmosphereColour.x = m_daylight;
		m_atmosphereColour.y = m_daylight;
		m_atmosphereColour.z = m_daylight;
	}

	if(m_actualLight < 0.4) m_actualLight = 0.55;

	if(m_atmosphereColour.x  < 0.5) m_atmosphereColour.x = 0.5;
	if(m_atmosphereColour.y  < 0.5) m_atmosphereColour.y = 0.5;
	if(m_atmosphereColour.z  < 0.5) m_atmosphereColour.z = 0.5;

	if(m_atmosphereColour.x  > 1) m_atmosphereColour.x = 1;
	if(m_atmosphereColour.y  > 1) m_atmosphereColour.y = 1;
	if(m_atmosphereColour.z  > 1) m_atmosphereColour.z = 1;

	SVector3 amb(m_atmosphereColour.x,m_atmosphereColour.y,m_atmosphereColour.z);
	//addAmbient(amb);

	m_sunRot = (360 * float(time / 24)) - 90; 
}

bool CAtmosphere::shadowCasterPos()
{
	bool render = false;

	HWND shad = GetDlgItem(g_hAtmosphereEnv,IDC_SHADOW_POSITION);
	if(SendMessage(shad,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		glPushMatrix();
	
		glLoadIdentity();
		gluPerspective(60.0f, 1.0f, 50.0f, 3000.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_shadowProj);
			
		glLoadIdentity();
		glRotated(g_input.rot.x,1,0,0);
		glRotated(g_input.rot.y,0,1,0);
		glTranslated(-g_input.pos.x,-g_input.pos.y,-g_input.pos.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_shadowView);

		glPopMatrix();

		m_shadowPosition = g_input.pos;
		m_shadowRotation = g_input.rot;

		render = true;
	}

	HWND gen = GetDlgItem(g_hAtmosphereEnv,IDC_SHADOW_GENERATE);
	if(SendMessage(shad,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		render = true;
	}

	HWND combo = GetDlgItem(g_hAtmosphereEnv, IDC_SHADOW_REAL_TIME);
	int id = SendMessage(combo, CB_GETCURSEL, 0, 0);

	if(id == 1)
	{
		m_realTimeShadows = true;
		render = true;
	}
	else
	{
		m_realTimeShadows = false;
	}

	if(m_renderInitialShadow)
	{
		glPushMatrix();
	
		glLoadIdentity();
		gluPerspective(60.0f, 1.0f, 50.0f, 3000.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_shadowProj);
			
		glLoadIdentity();
		glRotated(m_shadowRotation.x,1,0,0);
		glRotated(m_shadowRotation.y,0,1,0);
		glTranslated(-m_shadowPosition.x,-m_shadowPosition.y,-m_shadowPosition.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_shadowView);

		glPopMatrix();

		m_renderInitialShadow = false;
		render = true;
	}

	if(render)
	{
		return true;
	}

	return false;
}

void CAtmosphere::shadowMapBegin()
{
	m_shadowMap->bindFBO();

	glGetFloatv(GL_MODELVIEW_MATRIX, m_currentProj);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_currentView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(m_shadowProj);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(m_shadowView);

	glColorMask(0,0,0,1);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(m_shadowOffset,-m_shadowOffset);
}

void CAtmosphere::shadowMapEnd()
{
	//unbind the shadow map depth component FBO texture
	m_shadowMap->unbindFBO();

	//disable polygon offset
	glDisable(GL_POLYGON_OFFSET_FILL);

	//restore colour to full colour rendering
	glColorMask(1,1,1,1);

	//revert view matrix back to the camera view
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void CAtmosphere::bindShadowMap()
{
	m_shadowMap->bindAsTexture(0);
	buildShadowTextureMatrix(m_shadowProj,m_shadowView,&m_textureDefault);
}

void CAtmosphere::unbindShadowMap()
{
	glActiveTexture(GL_TEXTURE0_ARB);
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(m_textureDefault);
	glMatrixMode(GL_MODELVIEW);
}

void CAtmosphere::execute()
{
	if(g_editMode == EDIT_ATMOSPHERE)
	{
		dialogHandler();
	}
	else
	{
		ShowWindow(g_hAtmosphereEnv, SW_HIDE);
		ShowWindow(g_hAtmosphereSky, SW_HIDE);
		ShowWindow(g_hAtmosphereManagement, SW_HIDE);
	}
}

void CAtmosphere::dialogHandler()
{
		HWND tab = GetDlgItem(g_hAtmosphereProp,IDC_TAB_ATMOSPHERE_OPTIONS);
		ShowWindow(g_hAtmosphereProp, SW_SHOW);
		int i = TabCtrl_GetCurSel(tab);

		switch(i)
		{
			case 0:

			ShowWindow(g_hAtmosphereEnv, SW_HIDE);
			ShowWindow(g_hAtmosphereSky, SW_HIDE);
			ShowWindow(g_hAtmosphereManagement, SW_SHOW);
				
			break;

			case 1:

			ShowWindow(g_hAtmosphereEnv, SW_SHOW);
			ShowWindow(g_hAtmosphereSky, SW_HIDE);
			ShowWindow(g_hAtmosphereManagement, SW_HIDE);

			environmentOptions();

			break;

			case 2:

			ShowWindow(g_hAtmosphereEnv, SW_HIDE);
			ShowWindow(g_hAtmosphereSky, SW_SHOW);
			ShowWindow(g_hAtmosphereManagement, SW_HIDE);

			skyBoxOptions();

			break;
		}
}

void CAtmosphere::environmentOptions()
{
	HWND newAtmosphere = GetDlgItem(g_hAtmosphereEnv,IDC_TOGGLE_BLOOM);
	if(SendMessage(newAtmosphere,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		if(!m_pushed)
		{
			m_renderBloom = !m_renderBloom;
			m_pushed = true;
		}
	}
	else
	{
		m_pushed = false;
	}

	char text[255];

	GetDlgItemText(g_hAtmosphereEnv, IDC_BLOOM_PASSES ,text,255); 
	m_blurPasses = atoi(text);

	HWND shadowOffset = GetDlgItem(g_hAtmosphereEnv, IDC_SHADOW_OFFSET);
	float silderPos = SendMessage(shadowOffset, TBM_GETPOS, 0, 0);
	m_shadowOffset = silderPos / 50.0f;

	HWND combo = GetDlgItem(g_hAtmosphereEnv, IDC_SHADOW_SIZE);
	int id = SendMessage(combo, CB_GETCURSEL, 0, 0);

	int newSize = 512;

	for(int i = 0; i < id; i++)
	{
		newSize *= 2;
	}

	if(m_shadowMapSize != newSize)
	{
		delete m_shadowMap;
		m_shadowMap = new CFBO(1,newSize,GL_CLAMP_TO_BORDER);
		m_shadowMapSize = newSize;
	}

	combo = GetDlgItem(g_hAtmosphereEnv, IDC_BLOOM_SIZE);
	id = SendMessage(combo, CB_GETCURSEL, 0, 0);

	newSize = 128;

	for(int i = 0; i < id; i++)
	{
		newSize *= 2;
	}

	if(m_bloomSize != newSize)
	{
		delete m_blur[0];
		delete m_blur[1];

		m_blur[0] = new CFBO(0,512,GL_CLAMP_TO_EDGE);
		m_blur[1] = new CFBO(0,512,GL_CLAMP_TO_EDGE);

		m_bloomSize = newSize;
	}

	HWND lumin = GetDlgItem(g_hAtmosphereEnv, IDC_BLOOM_LUMINANCE);
	silderPos = SendMessage(lumin, TBM_GETPOS, 0, 0);
	m_luminance = silderPos / 100.0f;

	HWND bloomCol = GetDlgItem(g_hAtmosphereEnv,IDC_BLOOM_COLOR_B);
	if(SendMessage(bloomCol,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		m_color = initColorChooser();
	}

	renderToolbox();

}

void CAtmosphere::skyBoxOptions()
{
	HWND translate = GetDlgItem(g_hAtmosphereSky,IDC_ATMOSPHERE_TRANSLATE);
	if(SendMessage(translate,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_atmosphereEditMode = TRANSLATE;
	}

	HWND scale = GetDlgItem(g_hAtmosphereSky,IDC_ATMOSPHERE_SCALE);
	if(SendMessage(scale,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_atmosphereEditMode = SCALE;
	}

	HWND change = GetDlgItem(g_hAtmosphereSky,IDC_CHANGE_BOX);
	if(SendMessage(change,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		m_cubeMapID = CTextureMan::getInstance()->getSelectedCubemap();
	}

	HWND tex = GetDlgItem(g_hAtmosphereSky,IDC_SKY_TEXTURE_MANAGER);
	if(SendMessage(tex,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		ShowWindow(g_hTextureManager, SW_SHOW);
	}

	renderSkyBoxViewer();
}

void CAtmosphere::renderSkyBoxViewer()
{
	wglMakeCurrent(g_hDCSkyboxViewer, g_hRCSkyboxViewer);

	int id = CTextureMan::getInstance()->getSelectedCubemap();

	float aspect = (float) g_screenWidth/g_screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect, 1.0f, 30000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	quickClear();

	user::getInstance()->viewerMouseLook();
	user::getInstance()->viewerPositionLookAt();

	if(id != -1)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		CTextureMan::getInstance()->disableAllUnits();

		CShaderManager::getInstance()->attachShader(g_shaderId.skybox);

		CTextureMan::getInstance()->bindCubeMap(id);

		glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.skybox), "cubeMap"), 7);

		glPushMatrix();

		glScaled(100,100,100);

		cubeVBO();

		glPopMatrix();

		glCullFace(GL_BACK);

		CTextureMan::getInstance()->unbindCubeMap();
	}

	SwapBuffers(g_hDCSkyboxViewer);
	wglMakeCurrent(g_hDC, g_hRC);
}

void CAtmosphere::renderToolbox()
{
	wglMakeCurrent(g_hDCBloomColor, g_hRCBloomColor);

	glClearColor(m_color.x,m_color.y,m_color.z,1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

	SwapBuffers(g_hDCBloomColor);
	wglMakeCurrent(g_hDC, g_hRC);
}

bool CAtmosphere::renderShadowMap()
{
	if(shadowCasterPos())
	{
		return true;
	}
	
	return false;
}

void CAtmosphere::backBufferRender()
{
	if(g_editMode == EDIT_ATMOSPHERE)
	{
		switch(g_atmosphereEditMode)
		{
			case TRANSLATE:
						
					useTranslateWidget(&m_selectedAxis,&m_position);

			break;

			case SCALE:
						
					useScaleWidget(&m_selectedAxis,&m_position,&m_scale);
			break;
		}
	}
}

void CAtmosphere::renderAux()
{
	if(g_editMode == EDIT_ATMOSPHERE)
	{
		switch(g_atmosphereEditMode)
		{
			case TRANSLATE:

				translateWidget(m_position,m_selectedAxis,0);

			break;

			case SCALE:

				translateWidget(m_position,m_selectedAxis,1);

			break;
		}
	}
}

void CAtmosphere::output(std::ofstream &fout)
{
	fout << "atmosphere" << "\n";
	fout << m_position.x << " " << m_position.y << " " << m_position.z << "\n";
	fout << m_scale.x << " " << m_scale.y << " " << m_scale.z << "\n";
	fout << "this_will_be_the_cubemap_ID" << "\n";
	fout << m_color.x << " " << m_color.y << " " << m_color.z << " " << m_luminance << " " << m_bloomSize << " " << m_renderBloom << "\n";
	fout << m_shadowPosition.x << " " << m_shadowPosition.y << " " << m_shadowPosition.z << "\n";
	fout << m_shadowRotation.x << " " << m_shadowRotation.y << "\n";
	fout << m_shadowOffset << " " << m_shadowMapSize << " " << m_realTimeShadows << "\n";
}


























