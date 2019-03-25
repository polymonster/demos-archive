#include "textureSingleton.h"

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

int CTextureMan::load(const char *filename)
{
	STextureContainer newTexture;
	
	newTexture.filename = filename;

	m_textureStorage.push_back(newTexture);

	if(loadTga(filename,m_textureStorage.back().texture) == - 1) return -1;

	m_textureStorage.back().id = m_textureStorage.size() -1;

	return m_textureStorage.back().id;
}

int CTextureMan::loadTga(const char *filename, unsigned int &image){

	//get a pointer to a TGA file structure which will store the pixels data
	tImageTGA *pImage = LoadMyTGA(filename);

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
	
	//set up texture filtering so we get smoother textures at distance
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	//if we loaded an imahe then we can free the data now
	if (pImage)										
	{
		if (pImage->data)							
		{
			delete[] pImage->data;				
		}

		free(pImage);								
	}

	//return 0 so we know we have loaded the file successfully.
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

void CTextureMan::bindTexture(int id){

	glBindTexture(GL_TEXTURE_2D,m_textureStorage[id].texture);
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

void CTextureMan::deleteAll(){

	for(int i = 0; i < m_textureStorage.size();i++)
	{
		glDeleteTextures(1,&m_textureStorage.at(i).texture);
	}
}

void CTextureMan::loadCubeMap(char *front,char *back,char *left,char *right,char *top,char *bottom)
{
	glGenTextures(1, &m_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,m_cubeMap);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	int textureType = GL_RGB;


	//FRONT
	//tgaInfo *f;
	//f = tgaLoad(left);
	tImageTGA *f = LoadMyTGA(back);
	textureType = GL_RGB;
	if(f->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, textureType, f->sizeX, f->sizeY, 0, textureType, GL_UNSIGNED_BYTE, f->data);

	//BACK
	//tgaInfo *b;
	//b = tgaLoad(right);
	tImageTGA *b = LoadMyTGA(front);
	textureType = GL_RGB;
	if(b->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, textureType, b->sizeX, b->sizeY, 0, textureType, GL_UNSIGNED_BYTE, b->data); 

	//LEFT
	//tgaInfo *l;
	//l = tgaLoad(back);
	tImageTGA *l = LoadMyTGA(left);
	textureType = GL_RGB;
	if(l->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, textureType, l->sizeX, l->sizeY, 0, textureType, GL_UNSIGNED_BYTE, l->data); 
	
	//RIGHT
	//tgaInfo *r;
	//r = tgaLoad(front);
	tImageTGA *r = LoadMyTGA(right);
	textureType = GL_RGB;
	if(r->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, textureType, r->sizeX, r->sizeY, 0, textureType, GL_UNSIGNED_BYTE, r->data); 

	//TOP
	//tgaInfo *t;
	//t = tgaLoad(bottom);
	tImageTGA *t = LoadMyTGA(top);
	textureType = GL_RGB;
	if(t->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, textureType, t->sizeX, t->sizeY, 0, textureType, GL_UNSIGNED_BYTE, t->data); 
	
	//BOTTOM
	//tgaInfo *u;
	//u = tgaLoad(top);
	tImageTGA *u = LoadMyTGA(bottom);
	textureType = GL_RGB;
	if(u->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, textureType, b->sizeX, b->sizeY, 0, textureType, GL_UNSIGNED_BYTE, u->data); 

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Set far filtering mode
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     // Set near filtering mode
}

void CTextureMan::bindCubeMap()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_cubeMap);
}