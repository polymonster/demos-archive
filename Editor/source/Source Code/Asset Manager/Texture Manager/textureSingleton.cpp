#include "textureSingleton.h"
#include "toolbars.h"
#include "helpers.h"
#include <iostream>

//static member var
CTextureMan* CTextureMan::m_instance = NULL; 

void CTextureMan::create(){

	if(m_instance == NULL)
	{
		m_instance = new CTextureMan;
	}
}

void CTextureMan::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void CTextureMan::loadBasePack()
{
	load("data/interface/base/check1.tga",true);
	load("data/interface/base/check2.tga",true);
	load("data/interface/base/check3.tga",true);
	load("data/interface/base/check4.tga",true);
	load("data/interface/base/check5.tga",true);
	load("data/interface/brush.tga",true);
	load("data/texture/water/waternormal.tga",true);
}

void CTextureMan::parseName(STextureContainer *newTexture, char *file)
{
	//Parse the FILENAME file/file
	bool keepGoing = true;
	
	char mask[5];
	char root[256];

		int len = strlen(file);

		int readPos = len - 1;

		while(keepGoing)
		{
			for(int i = 5; i >= 0; i--)
			{
				int offset = 5 - i; 
				int file_pos = readPos - offset;

				if(offset < 0 || file_pos < 0 || file_pos > len) mask[i] = NULL;
				else 
				{
					mask[i] = file[file_pos];
				}
			}

			readPos--;

			if
			(
				   (mask[0] == '\\' || mask[0] == '/' || mask[0] == NULL)
				&& (mask[1] == 'd' || mask[1] == 'D')
				&& (mask[2] == 'a' || mask[2] == 'A')
				&& (mask[3] == 't' || mask[3] == 'T')
				&& (mask[4] == 'a' || mask[4] == 'A')
				&& (mask[5] == '\\' || mask[5] == '/')
			)
			{
				keepGoing = false;
			}
		}
		
		int fromPos = readPos - 3;

		int counter = 0;

		//read it into root
		for(int i = fromPos; i < len; i++)
		{
			root[counter++] = file[i];
		}

		root[counter] = '\0';

		newTexture->filename = root;
}

int CTextureMan::checkForDuplicates(STextureContainer *newTexture)
{
	int size = m_textureStorage.size();

	for(int i = 0; i < size; i++)
	{
		if(newTexture->filename == m_textureStorage.at(i).filename)
		{
			return i;
		}
	}

	return -1;
}

int CTextureMan::load(char *filename, bool warning)
{
	if(filename == NULL) return - 1;

	STextureContainer newTexture;
	
	newTexture.filename = filename;

	parseName(&newTexture,filename);

	int duplicateID = checkForDuplicates(&newTexture);

	if(duplicateID != -1)
	{
		return duplicateID;
	}

	m_textureStorage.push_back(newTexture);

	if(loadTga(filename,m_textureStorage.back().texture,warning) == - 1) 
	{
		m_textureStorage.pop_back();
		return -1;
	}

	m_textureStorage.back().id = m_textureStorage.size() -1;

	return m_textureStorage.back().id;
}

int CTextureMan::loadTga(const char *filename, unsigned int &image,bool warning){

	//pass in the TGA structure to be filled with pixel data
	tImageTGA *pImage = LoadMyTGA(filename,warning);

	// If we can't load the file, quit!
	if(pImage == NULL)									
	return -1;

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &image);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, image);

	// Assume that the texture is a 24 bit RGB texture (We convert 16-bit ones to 24-bit)
	int textureType = GL_RGB;

	// If the image is 32-bit (4 channels), then we need to specify GL_RGBA for an alpha
	if(pImage->channels == 4)
		textureType = GL_RGBA;
		
	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX, 
					  pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);
		
	//setup linear filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	

	//delete the image data from system RAM
	if (pImage)										
	{
		if (pImage->data)							
		{
			delete[] pImage->data;				
		}

		free(pImage);								
	}

	return 0;
}

void CTextureMan::disableAllUnits()
{
	for(int i = 0; i < 8; i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glDisable(GL_TEXTURE_2D);
	}
}

void CTextureMan::disableUnit(int id)
{
	glActiveTextureARB(GL_TEXTURE0_ARB + id);
	glDisable(GL_TEXTURE_2D);
}

void CTextureMan::bindTexture(int id)
{
	if(id >= 0 && id < m_textureStorage.size())
	{
		glBindTexture(GL_TEXTURE_2D,m_textureStorage[id].texture);
	}
}

void CTextureMan::bindTexture(int id, int unit)
{
	glActiveTextureARB(GL_TEXTURE0_ARB + unit);
	glEnable(GL_TEXTURE_2D);
	bindTexture(id);
}


std::string CTextureMan::printTextureName(int id)
{
	return m_filename[id];
}

void CTextureMan::deleteTexture(int id)
{
	glDeleteTextures(1,&m_textureStorage.at(id).texture);
}

void CTextureMan::deleteAll()
{
	for(int i = m_textureStorage.size() - 1; i >= 0; i--)
	{
		glDeleteTextures(1,&m_textureStorage.at(i).texture);

		std::vector<STextureContainer>::iterator remove = m_textureStorage.begin() + i;
		m_textureStorage.erase(remove); 
	}

	m_numberOfTextures = 0;
}

void CTextureMan::cubeMapConfig(char *file)
{
	if(file == NULL) return;

	std::ifstream fin;

	fin.open(file);

	char name[64];
	char front[64];
	char back[64];
	char left[64];
	char right[64];
	char top[64];
	char bottom[64];

	std::string word;

	while(!fin.eof())
	{
		fin >> word;

		if(word == "name")
		{
			fin >> name;
		}

		if(word == "front")
		{
			fin >> front;
		}

		if(word == "back")
		{
			fin >> back;
		}

		if(word == "left")
		{
			fin >> left;
		}

		if(word == "right")
		{
			fin >> right;
		}

		if(word == "top")
		{
			fin >> top;
		}

		if(word == "bottom")
		{
			fin >> bottom;
		}
	}

	loadCubeMap(front,back,left,right,top,bottom,name);
}

void CTextureMan::loadCubeMap(char *front,char *back,char *left,char *right,char *top,char *bottom,char *name)
{
	STextureContainer newCubeMap;

	newCubeMap.filename = name;

	glGenTextures(1, &newCubeMap.texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,newCubeMap.texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	int textureType = GL_RGB;

	//FRONT
	//tgaInfo *f;
	//f = tgaLoad(left);
	tImageTGA *f = LoadMyTGA(back,true);
	textureType = GL_RGB;
	if(f->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, textureType, f->sizeX, f->sizeY, 0, textureType, GL_UNSIGNED_BYTE, f->data);		

	//BACK
	//tgaInfo *b;
	//b = tgaLoad(right);
	tImageTGA *b = LoadMyTGA(front,true);
	textureType = GL_RGB;
	if(b->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, textureType, b->sizeX, b->sizeY, 0, textureType, GL_UNSIGNED_BYTE, b->data); 

	//LEFT
	//tgaInfo *l;
	//l = tgaLoad(back);
	tImageTGA *l = LoadMyTGA(left,true);
	textureType = GL_RGB;
	if(l->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, textureType, l->sizeX, l->sizeY, 0, textureType, GL_UNSIGNED_BYTE, l->data); 
	
	//RIGHT
	//tgaInfo *r;
	//r = tgaLoad(front);
	tImageTGA *r = LoadMyTGA(right,true);
	textureType = GL_RGB;
	if(r->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, textureType, r->sizeX, r->sizeY, 0, textureType, GL_UNSIGNED_BYTE, r->data); 

	//TOP
	//tgaInfo *t;
	//t = tgaLoad(bottom);
	tImageTGA *t = LoadMyTGA(top,true);
	textureType = GL_RGB;
	if(t->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, textureType, t->sizeX, t->sizeY, 0, textureType, GL_UNSIGNED_BYTE, t->data); 
	
	//BOTTOM
	//tgaInfo *u;
	//u = tgaLoad(top);
	tImageTGA *u = LoadMyTGA(bottom,true);
	textureType = GL_RGB;
	if(u->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, textureType, b->sizeX, b->sizeY, 0, textureType, GL_UNSIGNED_BYTE, u->data); 

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	m_cubeMapStorage.push_back(newCubeMap);

	HWND list = GetDlgItem(g_hTextureManager,IDC_CUBEMAP_LIST);
	SendMessage(list,  LB_INSERTSTRING, -1, (LPARAM)newCubeMap.filename.c_str());	

	delete[] u->data;
	delete[] t->data;
	delete[] r->data;
	delete[] l->data;
	delete[] b->data;
	delete[] f->data;


}

void CTextureMan::bindCubeMap(int id)
{
	glActiveTextureARB(GL_TEXTURE7_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_cubeMapStorage.at(id).texture);
}

void CTextureMan::unbindCubeMap()
{
	glActiveTextureARB(GL_TEXTURE7_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
}

SVector2<float> CTextureMan::calculateRelativeMousePosition()
{
	//get rects of window position
	RECT rManagerWindow;
	RECT rGlWindow;
	HWND hGlWindow = GetDlgItem(g_hTextureManager,IDC_TEXTURE_WINDOW);
	GetWindowRect(g_hTextureManager, &rManagerWindow);
	GetWindowRect(hGlWindow, &rGlWindow);

	SVector2<int> diff;
	SVector2<int> size;
	SVector2<float> newMouse;
	SVector2<float> ratio;

	diff.x = 9;//rGlWindow.left - rManagerWindow.left;
	diff.y = 12;//rGlWindow.top - rManagerWindow.top;

	size.x = rGlWindow.right - rGlWindow.left;
	size.y = rGlWindow.bottom - rGlWindow.top;

	newMouse.x = input.mousePos.x - diff.x;
	newMouse.y = input.mousePos.y - diff.y;

	ratio.x = (float) newMouse.x / (float) size.x;
	ratio.y = (float) newMouse.y / (float) size.y;

	newMouse.x = ratio.x * 1024;
	newMouse.y = ratio.y * 1024;

	return newMouse;
}

void CTextureMan::textureManagerWindow()
{
	HWND tab = GetDlgItem(g_hTextureManager,IDC_TEXTURE_TYPES);
	int i = TabCtrl_GetCurSel(tab);

	switch(i)
	{
		case 0:
		{
			
			HWND tex = GetDlgItem(g_hTextureManager,IDC_TEXTURE_WINDOW);
			HWND cube = GetDlgItem(g_hTextureManager,IDC_CUBEMAP_LIST_2);
			HWND scroll = GetDlgItem(g_hTextureManager,IDC_TEXTURE_SCROLLBAR);
			HWND skybox = GetDlgItem(g_hTextureManager,IDC_SKYBOX_WINDOW);
			
			ShowWindow(cube, SW_HIDE);
			ShowWindow(skybox, SW_HIDE);
			ShowWindow(tex, SW_SHOW);
			ShowWindow(scroll, SW_SHOW);

			HWND load = GetDlgItem(g_hTextureManager,ID_ADD_TEXTURE);
			if(SendMessage(load,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				CTextureMan::getInstance()->load(openFile("Texture file","tga"),true);
			}
			
			wglMakeCurrent(g_hDCTextureManager, g_hRCTextureManager);

			renderEnumeratedTextures();

			SwapBuffers(g_hDCTextureManager);
		}
		break;

		case 1:
		{

			HWND tex = GetDlgItem(g_hTextureManager,IDC_TEXTURE_WINDOW);
			HWND list = GetDlgItem(g_hTextureManager,IDC_CUBEMAP_LIST_2);
			HWND scroll = GetDlgItem(g_hTextureManager,IDC_TEXTURE_SCROLLBAR);
			HWND skybox = GetDlgItem(g_hTextureManager,IDC_SKYBOX_WINDOW);
			

			ShowWindow(list, SW_SHOW);
			ShowWindow(skybox, SW_SHOW);
			ShowWindow(tex, SW_HIDE);
			ShowWindow(scroll, SW_HIDE);

			HWND load = GetDlgItem(g_hTextureManager,ID_ADD_TEXTURE);
			if(SendMessage(load,BM_GETSTATE,0,0) & BST_PUSHED)
			{
				CTextureMan::getInstance()->cubeMapConfig(openFile("cube map file","txt"));
			}

			wglMakeCurrent(g_hDCSkybox, g_hRCSkybox);

			renderEnumeratedCubemaps();

			SwapBuffers(g_hDCSkybox);
		}
		break;
	}

}

void CTextureMan::renderEnumeratedTextures()
{
	CShaderManager::getInstance()->detachShader();

	//Clear the buffers before drawing
	quickClear();

	char help[32];

	HWND combo = GetDlgItem(g_hTextureManager, IDC_COMBO_SIZES);
	int id = SendMessage(combo, CB_GETCURSEL, 0, 0);
	SendMessage(combo,CB_GETLBTEXT,id,(LPARAM)(LPCTSTR)help);

	m_enumertionSize = atoi(help);

	int orthoSize = 1024;

	orthoMode(0,0,orthoSize,orthoSize);
	
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);

	glColor4f(1,1,1,1);

	int offset = 0;
	if(m_enumertionSize != 0) offset = m_enumertionSize / (orthoSize / m_enumertionSize);
	int initialOffset = m_enumertionSize / 2 + offset;

	int x = initialOffset;
	int y = initialOffset;

	SVector2<float> mouse = calculateRelativeMousePosition();

	disableAllUnits();

	for(int i = 0; i < m_textureStorage.size(); i++)
	{
		glColor3f(1,1,1);

		if((mouse.x >= x - (m_enumertionSize/2)) && (mouse.x <= (x + m_enumertionSize/2))
			&& (mouse.y >= (y - m_enumertionSize/2)) && (mouse.y <= (y + m_enumertionSize/2)))
		{
			glColor4f(0.9,0.9,0.9,1);

			glPushMatrix();

			glTranslated(x,y,0);
			glScaled((m_enumertionSize/2) + offset,(m_enumertionSize/2) + offset,1);
			centeredQuad();

			glPopMatrix();

			if(input.leftMouse) 
			{
				m_selectedTextureID = i;
			}
		}

		if(m_selectedTextureID == i)
		{
			glColor4f(0.5,0.1,0.1,1);

			glPushMatrix();

			glTranslated(x,y,0);
			glScaled((m_enumertionSize/2) + offset,(m_enumertionSize/2) + offset,1);
			centeredQuad();

			glPopMatrix();
		}

		glColor4f(1,1,1,1);

		glPushMatrix();

			glTranslated(x,y,0);
			glScaled(m_enumertionSize/2,m_enumertionSize/2,1);
			bindTexture(i,0);
			texturedQuad();

		glPopMatrix();

		x+= m_enumertionSize + offset;

		if(x >= 1024)
		{
			x = initialOffset;
			y += m_enumertionSize + offset;
		}

		glColor4f(0.3,0.3,0.3,1);
		orthoBorder(1,1024);
	}
}

void CTextureMan::export()
{
	char *file = saveFile("Texture Manager Library","otl");
	if(file == NULL) return;

	std::ofstream fout;

	fout.open(file);

	output(fout);

	fout.close();
}

void CTextureMan::output(std::ofstream &fout)
{
	fout << "texture_manager" << " " << m_textureStorage.size() << "\n";

	for(int i = 0; i < m_textureStorage.size(); i++)
	{
		fout << m_textureStorage.at(i).filename << "\n";
	}
}

void CTextureMan::import()
{
	char *file = openFile("Texture Manager Library","otl");
	if(file == NULL) return;

	std::ifstream fin;

	fin.open(file);

	std::string word;

	fin >> word;

	if(word == "texture_manager") readIn(fin);

	fin.close();
}

void CTextureMan::readIn(std::ifstream &fin)
{
	int quantity;

	fin >> quantity;

	for(int i = 0; i < quantity; i++)
	{
		char file[256];
	
		fin >> file;

		load(file,true);
	}
}

void CTextureMan::renderEnumeratedCubemaps()
{
	float aspect = (float) g_screenWidth/g_screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect, 1.0f, 30000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	quickClear();

	user::getInstance()->viewerPositionLookAt();

	HWND list = GetDlgItem(g_hTextureManager,IDC_CUBEMAP_LIST_2);
	int pos = (int)SendMessage(list, LB_GETCURSEL, 0, 0); 
	
	if(pos < m_cubeMapStorage.size() && pos >= 0)
	{
		m_selectedCubemapID = pos;
	}
	else
	{
		m_selectedCubemapID = -1;
	}

	if(m_selectedCubemapID != -1)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		disableAllUnits();

		CShaderManager::getInstance()->attachShader(g_shaderId.skybox);

		bindCubeMap(m_selectedCubemapID);

		glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.skybox), "cubeMap"), 7);

		glPushMatrix();

		glScaled(100,100,100);

		cubeVBO();

		glPopMatrix();

		glCullFace(GL_BACK);

		unbindCubeMap();

		CShaderManager::getInstance()->detachShader();
	}

	orthoMode(0,0,1024,1024);

	glColor4f(0.3,0.3,0.3,1);
	orthoBorder(1,1024);
}



