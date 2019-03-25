#include "Terrain.h"
#include "Fonts.h"
#include "TimerSingleton.h"
#include "modelManager.h"
#include "textureSingleton.h"
#include "ShaderManager.h"
#include "toolbars.h"
#include <iostream>

bool pushedMirror = false;

void CTerrain::execute()
{
	if(g_editMode == EDIT_TERRAIN)
	{
		showWindows();
		edit();
	}
	else
	{
		hideWindows();
	}
}

void CTerrain::output(std::ofstream &fout)
{
	fout << "terrain" << " " << m_width << " " << m_height << " " << m_tileSize << "\n";

	for(int i = 1; i < 8; i++)
	{
		fout << m_texturePallet[i] << " ";
	}

	fout << "\n";

	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			fout << m_vertex[i][j].x << " "
				 << m_vertex[i][j].y << " "
				 << m_vertex[i][j].z << " ";

			for(int k = 0; k < 4; k++)
			{
				 fout << m_weightsFirst[i][j].channel[k] << " "
					  << m_weightsSecond[i][j].channel[k] << " ";
			}

			fout << "\n";			
		}
	}

}

void CTerrain::readIn(std::ifstream &fin)
{
	for(int i = 1; i < 8; i++)
	{
		fin >> m_texturePallet[i];
	}

	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			fin >> m_vertex[i][j].x
				 >> m_vertex[i][j].y
				 >> m_vertex[i][j].z;

			for(int k = 0; k < 4; k++)
			{
				 fin >> m_weightsFirst[i][j].channel[k]
					  >> m_weightsSecond[i][j].channel[k];
			}					
		}
	}

	updateVBO();
	updateTextureWeights();
}

void CTerrain::exportSF()
{
	std::ofstream fout;

	char *file = saveFile("export map","txt");

	if(file == NULL) return;

	fout.open(file);


	fout << "terrain" << " " << m_width << " " << m_height << "\n";

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);

	SVector3 *normalPtr = (SVector3*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			float texs = i * 0.5f, text = j * 0.5f;

			if(i == 0 || i == m_width - 1)
			{
				if(i == 0) m_vertex[i][j].x -= 5000.0f;
				else m_vertex[i][j].x += 5000.0f;

				if(i == 0) texs = -100;
				else texs = 100;
			}

			fout << m_vertex[i][j].x << " "
				 << m_vertex[i][j].y << " "
				 << m_vertex[i][j].z << " ";

			fout << normalPtr->x << " "
				 << normalPtr->y << " "
				 << normalPtr->z << " ";

			normalPtr++;

			fout << m_weightsFirst[i][j].channel[0] << " "
				 << m_weightsFirst[i][j].channel[1] << " "
				 << m_weightsFirst[i][j].channel[2] << " "
				 << m_weightsFirst[i][j].channel[3] << " ";

			fout << texs << " "
				 << text << "\n";
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	fout.close();

}

void CTerrain::export()
{
	std::ofstream fout;

	char *file = saveFile("export map","txt");

	if(file == NULL) return;

	fout.open(file);

	fout << "terrain" << " " << m_width << " " << m_height << "\n";

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);

	SVector3 *normalPtr = (SVector3*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			float texs = i * 0.5f, text = j * 0.5f;

			/*if(i == 0 || i == m_width - 1)
			{
				m_vertex[i][j].x *= 100.0f;

				if(i == 0) texs = -50;
				else texs = i + 50;
			}*/

			fout << m_vertex[i][j].x << " "
				 << m_vertex[i][j].y << " "
				 << m_vertex[i][j].z << " ";

			fout << normalPtr->x << " "
				 << normalPtr->y << " "
				 << normalPtr->z << " ";

			normalPtr++;

			fout << m_weightsFirst[i][j].channel[0] << " "
				 << m_weightsFirst[i][j].channel[1] << " "
				 << m_weightsFirst[i][j].channel[2] << " "
				 << m_weightsFirst[i][j].channel[3] << " ";

			fout << texs << " "
				 << text << "\n";
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	fout.close();
}


void CTerrain::controls()
{
	setBrushProperties();
	keyboardCommands();
}

void CTerrain::edit()
{
	switch(m_editMode)
	{
		case 0:
		{
			//SMOOTHEN BUTTON
			HWND mirror = GetDlgItem(g_hTerrainValues,IDC_TERRAIN_MIRROR);
			if(SendMessage(mirror,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				if(!pushedMirror)
				{
					pushedMirror = true;
					mirrorVertices();
				}
			}
			else
			{
				pushedMirror = false;
			}
		}
		break;


		case 1:
		{
			height();
			drawBrush(1);
			drawBrush(0);
			
			//SIZE SLIDER
			HWND radius = GetDlgItem(g_hTerrainHeight, IDC_SIZE_SLIDER);
			int newRadius = SendMessage(radius, TBM_GETPOS, 0, 0);
			m_brush.radius = newRadius * 4; 

			//STR SLIDER
			HWND strength = GetDlgItem(g_hTerrainHeight, IDC_STR_SLIDER);
			int newStrength = SendMessage(strength, TBM_GETPOS, 0, 0);
			m_brush.strength = newStrength * 0.004f; 

			//FADE SLIDER
			HWND fade = GetDlgItem(g_hTerrainHeight, IDC_FADE_SLIDER);
			int newFade = SendMessage(fade, TBM_GETPOS, 0, 0);
			m_brush.falloff = newFade * 0.01f; 

			//SMOOTHEN BUTTON
			HWND smooth = GetDlgItem(g_hTerrainHeight,IDC_RECALCULATE_NORMALS);
			if(SendMessage(smooth,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				smoothen();
			}
	
			HWND mode = GetDlgItem(g_hTerrainHeight,IDC_SCULPT_MODES);
			g_terrainSculptMode = SendMessage(mode, CB_GETCURSEL, 0, 0);
			
		}
		break;

		case 2:
		{
			paint();
			drawBrush(0);

			//SIZE SLIDER
			HWND radius = GetDlgItem(g_hTerrainPaint, IDC_SIZE_SLIDER);
			int newRadius = SendMessage(radius, TBM_GETPOS, 0, 0);
			m_brush.radius = newRadius * 4; 

			//STR SLIDER
			HWND strength = GetDlgItem(g_hTerrainPaint, IDC_STR_SLIDER);
			int newStrength = SendMessage(strength, TBM_GETPOS, 0, 0);
			m_brush.strength = newStrength * 0.004f; 

			//FADE
			HWND fade = GetDlgItem(g_hTerrainPaint, IDC_FADE_SLIDER);
			int newFade = SendMessage(fade, TBM_GETPOS, 0, 0);
			m_brush.falloff = newFade * 0.01f; 

			//CHANGE TEXTURE
			HWND newTexture = GetDlgItem(g_hTerrainPaint,ID_CHANGE_TEXTURE);
			if(SendMessage(newTexture,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				m_texturePallet[m_selectedTextureId] = CTextureMan::getInstance()->getSelectedTexture();
			}

			//OPEN TEXTURE MANAGER
			HWND texManager = GetDlgItem(g_hTerrainPaint,ID_TEXTURE_MANAGER);
			if(SendMessage(texManager,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				ShowWindow(g_hTextureManager, SW_SHOW);
			}
			
		}
		break;
	}

	controls();
	renderToolbox();

}

void CTerrain::mirrorVertices()
{
	for(int i = 0; i < m_width; i++)
	{
		for(int j = m_height - 1; j >= 0; j--)
		{
			m_vertex_temp[i][(m_height - 1) - j].y = m_vertex[i][j].y;
		}
	}

	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			m_vertex[i][j].y = m_vertex_temp[i][j].y;
		}
	}

	updateVBO();
}


void CTerrain::setBrushProperties()
{
	HWND combo = GetDlgItem(g_hTerrainPaint, IDC_COMBO1);
	int id = SendMessage(combo, CB_GETCURSEL, 0, 0);
	setActiveTextureLayer(id);


}




void CTerrain::setupToolboxes()
{
	int start = 25;

	//SIZE SLIDER
	HWND radius = GetDlgItem(g_hTerrainPaint, IDC_SIZE_SLIDER);
	SendMessage(radius, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)start);

	//STR SLIDER
	HWND strength = GetDlgItem(g_hTerrainPaint, IDC_STR_SLIDER);
	SendMessage(strength, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)start);

	//FALLOFF SLIDER
	HWND falloff = GetDlgItem(g_hTerrainPaint, IDC_FADE_SLIDER);
	SendMessage(falloff, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)50);
}

void CTerrain::keyboardCommands()
{
	// if '[' or ']' to increase radius
	if((g_input.key[219]) && (!g_input.keyLocked[219]))
	{
		m_brush.radius-=4;
		HWND radius = GetDlgItem(g_hTerrainPaint, IDC_SIZE_SLIDER);
		SendMessage(radius, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_brush.radius*0.25);
	}
	else if((g_input.key[221]) && (!g_input.keyLocked[221]))
	{
		m_brush.radius+=4;
		HWND radius = GetDlgItem(g_hTerrainPaint, IDC_SIZE_SLIDER);
		SendMessage(radius, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_brush.radius*0.25);
	}
}

void CTerrain::renderToolbox()
{
	wglMakeCurrent(g_hDCPaintTexture, g_hRCPaintTexture);

		quickClear();

		orthoMode(0,0,1024,1024);
		
		glDisable(GL_LIGHTING);

		glColor4f(1,1,1,1);

		glPushMatrix();

		glTranslated(512,512,0);
		glScaled(512,512,1);
		CTextureMan::getInstance()->bindTexture(m_texturePallet[m_selectedTextureId],0);
		texturedQuad();

		glPopMatrix();

		glColor3f(0.3,0.3,0.3);
		orthoBorder(5,1204);

		glLoadIdentity();

	SwapBuffers(g_hDCPaintTexture);
	wglMakeCurrent(g_hDC, g_hRC);
}

void CTerrain::hideWindows()
{
	ShowWindow(g_hTerrainProp, SW_HIDE);
	ShowWindow(g_hTerrainHeight, SW_HIDE);
	ShowWindow(g_hTerrainPaint, SW_HIDE);
	ShowWindow(g_hTerrainValues, SW_HIDE);
}

void CTerrain::showWindows()
{
	HWND tab = GetDlgItem(g_hTerrainProp,IDC_TAB_TERRAIN_OPTIONS);
	ShowWindow(g_hTerrainProp, SW_SHOW);
	int i = TabCtrl_GetCurSel(tab);

	if(i == 1)
	{
		m_editMode = 1;
		ShowWindow(g_hTerrainPaint, SW_HIDE);
		ShowWindow(g_hTerrainValues, SW_HIDE);
		ShowWindow(g_hTerrainHeight, SW_SHOW);
	}
	else if(i == 2) 
	{
		m_editMode = 2;
		ShowWindow(g_hTerrainValues, SW_HIDE);
		ShowWindow(g_hTerrainHeight, SW_HIDE);
		ShowWindow(g_hTerrainPaint, SW_SHOW);
	}
	else
	{
		m_editMode = 0;
		ShowWindow(g_hTerrainHeight, SW_HIDE);
		ShowWindow(g_hTerrainPaint, SW_HIDE);
		ShowWindow(g_hTerrainValues, SW_SHOW);
	}
}
