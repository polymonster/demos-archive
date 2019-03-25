#include "water.h"
#include "textureSingleton.h"
#include "timerSingleton.h"
#include "input.h"

int g_waterEditMode;

int g_selectedWaterPlane = -1;
int g_numberOfWaterPlanes = 0;
int g_waterSubEditMode = -1;

CWaterPlane::~CWaterPlane()
{
	delete reflectionFBO;
	delete refractionFBO;
}

CWaterPlane::CWaterPlane(int id)
{
	nameIdentifier = "water";

	m_textureOffset[0].x = 0;
	m_textureOffset[0].y = 0;
	m_position = SVector3(0,-10,0);
	m_scale = SVector3(500,500,500);

	m_reflectionDistortion.x = 5.0f;
	m_reflectionDistortion.y = 2.0f;

	m_refractionDistortion.x = -0.5f;
	m_refractionDistortion.y = -0.2f;

	m_animationSpeed.x = 0.02f;
	m_animationSpeed.y = 0.01f;

	reflectionFBO = new CFBO(0,1024,GL_CLAMP_TO_EDGE);
	refractionFBO = new CFBO(0,1024,GL_CLAMP_TO_EDGE);

	m_id = id;

	m_localReflections = 1;

	m_textureID = 6;

	m_transparency = 1;

	setToolbarValues();
}

void CWaterPlane::backBufferRender()
{
	CTextureMan::getInstance()->disableAllUnits();
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	if(g_waterSubEditMode == 0)
	{
		if(g_selectedWaterPlane == m_id)
		{
			switch(g_waterEditMode)
			{
				case TRANSLATE:
						
						useTranslateWidget(&m_selectedAxis,&m_position);

				break;

				case SCALE:
						
						useScaleWidget(&m_selectedAxis,&m_position,&m_scale);
				break;
			}
		}

		if(g_editMode == EDIT_WATER && g_waterEditMode == SELECT_WATER)
		{
			glDisable(GL_CULL_FACE);

			CTextureMan::getInstance()->disableAllUnits();
			glDisable(GL_LIGHTING);

			glColor3ub(m_id + 1,0,0);

			glBegin(GL_QUADS);
				glColor4ub(m_id + 1,0,0,255);
				glVertex3f(m_position.x - m_scale.x,m_position.y,m_position.z - m_scale.z);
				glColor4ub(m_id + 1,0,0,255);
				glVertex3f(m_position.x + m_scale.x,m_position.y,m_position.z - m_scale.z);
				glColor4ub(m_id + 1,0,0,255);
				glVertex3f(m_position.x + m_scale.x,m_position.y,m_position.z + m_scale.z);
				glColor4ub(m_id + 1,0,0,255);
				glVertex3f(m_position.x - m_scale.x,m_position.y,m_position.z + m_scale.z);
			glEnd();

			if(g_input.leftMouse)
			{
				if(user::getInstance()->getPixelAtMouse().r == m_id + 1)
				{
					if(g_selectedWaterPlane != m_id)
					{
						g_selectedWaterPlane = m_id;
						setToolbarValues();
					}
				}
			}
		}
	}
}

void CWaterPlane::execute()
{
	m_textureOffset[0].x += m_animationSpeed.x * CTimer::getInstance()->animBaseSpeed();
	m_textureOffset[0].y += m_animationSpeed.y * CTimer::getInstance()->animBaseSpeed();

	if(g_editMode == EDIT_WATER)
	{
		if(g_selectedWaterPlane == m_id)
		{
			dialogHandler();
		}	
	}
	else
	{
		ShowWindow(g_hWaterProp, SW_HIDE);
		ShowWindow(g_hWaterOptical, SW_HIDE);
		ShowWindow(g_hWaterTexture, SW_HIDE);
		ShowWindow(g_hWaterManagement, SW_HIDE);
	}
}

void CWaterPlane::render()
{
	//Water
	if(g_waterSubEditMode != 0 || g_editMode != EDIT_WATER)
	{
		//bind water shader
		CShaderManager::getInstance()->attachShader(g_shaderId.water);
		
		//disable all textures
		CTextureMan::getInstance()->disableAllUnits();

		//bind fresh ones
		CTextureMan::getInstance()->bindCubeMap(0);
		CTextureMan::getInstance()->bindTexture(m_textureID,1);
		CTextureMan::getInstance()->bindTexture(m_textureID,2);

		//bind the reflections / refractions fbo
		reflectionFBO->bindAsTexture(0);
		refractionFBO->bindAsTexture(3);

		CShaderManager::getInstance()->setUniform(g_shaderId.water,"cubeMap",7);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"normalMap",1);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"normalMap2",2);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"local",0);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"underwater",3);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"cameraPos", g_input.pos, 1.0f);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"localReflections",m_localReflections);
		CShaderManager::getInstance()->setUniform(g_shaderId.water,"transparency",(float)m_transparency);

		CShaderManager::getInstance()->setUniform(g_shaderId.water,"reflectionDistortionWeight",
			m_reflectionDistortion.x,m_reflectionDistortion.y);

		CShaderManager::getInstance()->setUniform(g_shaderId.water,"refractionDistortionWeight",
			m_refractionDistortion.x,m_refractionDistortion.y);
		
		
		//Render
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);

		glNormal3f(0,1,0);

		glBegin(GL_QUADS);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,0 + m_textureOffset[0].x); 
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB,10 + m_textureOffset[0].y,10); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 

			glVertex3f(m_position.x - m_scale.x,m_position.y,m_position.z - m_scale.z);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,10,0 + m_textureOffset[0].x); 
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB,0 + m_textureOffset[0].y,10);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 

			glVertex3f(m_position.x + m_scale.x,m_position.y,m_position.z - m_scale.z);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,10,10 + m_textureOffset[0].x); 
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB,0 + m_textureOffset[0].y,0); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 

			glVertex3f(m_position.x + m_scale.x,m_position.y,m_position.z + m_scale.z);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,10 + m_textureOffset[0].x); 
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB,10 + m_textureOffset[0].y,0); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 

			glVertex3f(m_position.x - m_scale.x,m_position.y,m_position.z + m_scale.z);

		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);

		CShaderManager::getInstance()->detachShader();
		CTextureMan::getInstance()->disableAllUnits();

		CTextureMan::getInstance()->unbindCubeMap();
	}
	else
	{
		glDisable(GL_CULL_FACE);

		CShaderManager::getInstance()->detachShader();

		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		CTextureMan::getInstance()->disableAllUnits();

		glColor4f(0.0f,0.0f,0.5f,1.0f);

		glBegin(GL_QUADS);
			glVertex3f(m_position.x - m_scale.x,m_position.y,m_position.z - m_scale.z);
			glVertex3f(m_position.x + m_scale.x,m_position.y,m_position.z - m_scale.z);
			glVertex3f(m_position.x + m_scale.x,m_position.y,m_position.z + m_scale.z);
			glVertex3f(m_position.x - m_scale.x,m_position.y,m_position.z + m_scale.z);;
		glEnd();
	}

}

void CWaterPlane::reflectionBegin()
{
	glPushMatrix();

	double eqr[4];

	eqr[0] = 0.0f;
	eqr[1] = -1.0f;
	eqr[2] = 0.0f;
	eqr[3] = m_position.y + (m_reflectionDistortion.y);

	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqr);	

	glFrontFace(GL_CW);

	glTranslated(0,m_position.y * 2,0);

	glScaled(1,-1,1);

	//REFLECTION
	reflectionFBO->bindFBO();

}

void CWaterPlane::reflectionEnd()
{
	reflectionFBO->unbindFBO();

	glDisable(GL_CLIP_PLANE0);
	glFrontFace(GL_CCW);
	glPopMatrix();
}

void CWaterPlane::refractionBegin()
{
	refractionFBO->bindFBO();

	glEnable(GL_CLIP_PLANE0);

	glDisable(GL_BLEND);

}

void CWaterPlane::refractionEnd()
{
	glDisable(GL_CLIP_PLANE0);

	refractionFBO->unbindFBO();
}

void CWaterPlane::dialogHandler()
{
	HWND tab = GetDlgItem(g_hWaterProp,IDC_TAB_WATER_OPTIONS);
	ShowWindow(g_hWaterProp, SW_SHOW);
	int i = TabCtrl_GetCurSel(tab);

	g_waterSubEditMode = i;

	char text[255];

	switch(i)
	{
		case 0:
		{
			ShowWindow(g_hWaterOptical, SW_HIDE);
			ShowWindow(g_hWaterTexture, SW_HIDE);
			ShowWindow(g_hWaterManagement, SW_SHOW);

			//check translate mode
			HWND translate = GetDlgItem(g_hWaterManagement,IDC_WATER_TRANSLATE);
			if(SendMessage(translate,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				g_waterEditMode = TRANSLATE;
			}

			//check scale mode
			HWND scale = GetDlgItem(g_hWaterManagement,IDC_WATER_SCALE2);
			if(SendMessage(scale,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				g_waterEditMode = SCALE;
			}

			HWND select = GetDlgItem(g_hWaterManagement,IDC_WATER_SELECT);
			if(SendMessage(select,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				g_waterEditMode = SELECT_WATER;
			}
		}	
		break;

		case 1:
		{
	   		ShowWindow(g_hWaterOptical, SW_SHOW);
			ShowWindow(g_hWaterTexture, SW_HIDE);
			ShowWindow(g_hWaterManagement, SW_HIDE);

			//reflection
			GetDlgItemText(g_hWaterOptical, IDC_REFDIS_X,text,255); 
			m_reflectionDistortion.x = atof(text);
			GetDlgItemText(g_hWaterProp, IDC_REFDIS_Y,text,255); 
			m_reflectionDistortion.y = atof(text);

			//refraction
			GetDlgItemText(g_hWaterOptical, IDC_RAFDIS_X,text,255); 
			m_refractionDistortion.x = atof(text);
			GetDlgItemText(g_hWaterProp, IDC_RAFDIS_Y,text,255); 
			m_refractionDistortion.y = atof(text);

			HWND trans = GetDlgItem(g_hWaterOptical, IDC_TRANSPARENCY_SLIDER); 
			int pos = SendMessage(trans, TBM_GETPOS, 0, 0);
			m_transparency = (float)pos * 0.01f;

			HWND local = GetDlgItem(g_hWaterOptical,IDC_COMBO_WATER_TYPES);
			m_localReflections = SendMessage(local,CB_GETCURSEL,0,0);
		}
		break;

		case 2:
		{
	   		ShowWindow(g_hWaterOptical, SW_HIDE);
			ShowWindow(g_hWaterTexture, SW_SHOW);
			ShowWindow(g_hWaterManagement, SW_HIDE);
			renderToolboxTexture();

			//CHANGE TEXTURE
			HWND newTexture = GetDlgItem(g_hWaterTexture,ID_CHANGE_WATER_TEXTURE);
			if(SendMessage(newTexture,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				m_textureID = CTextureMan::getInstance()->getSelectedTexture();
			}

			//OPEN TEXTURE MANAGER
			HWND texManager = GetDlgItem(g_hWaterTexture,ID_TEXTURE_MANAGER);
			if(SendMessage(texManager,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				ShowWindow(g_hTextureManager, SW_SHOW);
			}

			//animation
			GetDlgItemText(g_hWaterTexture, IDC_ANIM_SPEED_X,text,255); 
			m_animationSpeed.x = atof(text);
			GetDlgItemText(g_hWaterTexture, IDC_ANIM_SPEED_Y,text,255); 
			m_animationSpeed.y = atof(text);
		}
		break;
	}
}

void CWaterPlane::setToolbarValues()
{
	char buffer[255];

	//reflection
	sprintf(buffer,"%f",m_reflectionDistortion.x);
	SetDlgItemText(g_hWaterOptical, IDC_REFDIS_X,buffer); 

	sprintf(buffer,"%f",m_reflectionDistortion.y);
	SetDlgItemText(g_hWaterOptical, IDC_REFDIS_Y,buffer); 

	//animation
	sprintf(buffer,"%f",m_animationSpeed.x);
	SetDlgItemText(g_hWaterTexture, IDC_ANIM_SPEED_X,buffer); 

	sprintf(buffer,"%f",m_animationSpeed.y);
	SetDlgItemText(g_hWaterTexture, IDC_ANIM_SPEED_Y,buffer); 

	//refraction
	sprintf(buffer,"%f",m_refractionDistortion.x);
	SetDlgItemText(g_hWaterOptical, IDC_RAFDIS_X,buffer); 

	sprintf(buffer,"%f",m_refractionDistortion.y);
	SetDlgItemText(g_hWaterOptical, IDC_RAFDIS_Y,buffer); 

	HWND trans = GetDlgItem(g_hWaterOptical, IDC_TRANSPARENCY_SLIDER); 
	int pos = m_transparency * 100.0f;
	SendMessage(trans, TBM_SETPOS, 1, pos);
	
	HWND local = GetDlgItem(g_hWaterOptical,IDC_COMBO_WATER_TYPES);
	SendMessage(local,CB_SETCURSEL,m_localReflections,0);
}

void CWaterPlane::renderAux()
{
	if(g_editMode == EDIT_WATER  && g_selectedWaterPlane == m_id)
	{
		glColor4f(1,1,1,1);

		CTextureMan::getInstance()->disableAllUnits();
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		if(g_waterSubEditMode == 0)
		{
			switch(g_waterEditMode)
			{
				case TRANSLATE:

					translateWidget(m_position,m_selectedAxis,0);

				break;

				case SCALE:

					translateWidget(m_position,m_selectedAxis,1);

				break;
			}
		}

		glColor4f(1,1,1,1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPushMatrix();
			glTranslated(m_position.x,m_position.y,m_position.z);
			glScaled(m_scale.x, 10 ,m_scale.z);
			cube();
		glPopMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		CTextureMan::getInstance()->disableAllUnits();
	}
}

void CWaterPlane::renderToolboxTexture()
{
	wglMakeCurrent(g_hDCWaterTexture, g_hRCWaterTexture);

		quickClear();

		orthoMode(0,0,1024,1024);
		
		glDisable(GL_LIGHTING);

		glColor4f(1,1,1,1);

		glPushMatrix();

		glTranslated(512,512,0);
		glScaled(512,512,1);
		CTextureMan::getInstance()->bindTexture(m_textureID,0);
		texturedQuad();

		glPopMatrix();

		glColor3f(0.3,0.3,0.3);
		orthoBorder(5,1204);

		glLoadIdentity();

	SwapBuffers(g_hDCWaterTexture);
	wglMakeCurrent(g_hDC, g_hRC);
}

void CWaterPlane::reposition(SVector3 increment)
{
	m_position += increment;
}

void CWaterPlane::rescale(SVector3 increment)
{
	m_scale += increment;
}

void CWaterPlane::output(std::ofstream &fout)
{
	fout << "water" << " ";
	fout << m_position.x << " " << m_position.y << " " << m_position.z << " ";
	fout << m_scale.x << " " << m_scale.y << " " << m_scale.z << " ";
	fout << m_reflectionDistortion.x << " " << m_reflectionDistortion.y << " ";
	fout << m_refractionDistortion.x << " " << m_refractionDistortion.y << " ";
	fout << m_animationSpeed.x << " " << m_animationSpeed.y << "\n";
}

void CWaterPlane::readIn(std::ifstream &fin)
{
	fin >> m_position.x >> m_position.y >> m_position.z;
	fin >> m_scale.x >> m_scale.y >> m_scale.z;
	fin >> m_reflectionDistortion.x >> m_reflectionDistortion.y;
	fin >> m_refractionDistortion.x >> m_refractionDistortion.y;
	fin >> m_animationSpeed.x >> m_animationSpeed.y;
}





