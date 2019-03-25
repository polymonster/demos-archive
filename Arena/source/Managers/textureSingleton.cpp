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

int CTextureMan::add(const char *filename){

	m_filename[m_numberOfTextures] = filename;

	for(int i = 0; i < m_numberOfTextures;i++)
	{
		if(m_filename[i].compare(filename) == 0)
		{
			return i;
		}
	}

	loadBitmap(filename,m_textureArray[m_numberOfTextures],false);

	m_numberOfTextures++;

	return m_numberOfTextures - 1;
}

int CTextureMan::addSkin(const char *filename, int mode){

	m_skinFilename[m_numberOfSkins] = filename;

	for(int i = 0; i < m_numberOfSkins;i++)
	{
		if(m_skinFilename[i].compare(filename) == 0)
		{
			return i;
		}
	}

	if(mode == 0) loadSkin(m_skinArray[m_numberOfSkins],filename);
	if(mode == 1) loadSkin(m_skinArray[m_numberOfSkins],filename);

	m_numberOfSkins++;

	return m_numberOfSkins - 1;

}

int CTextureMan::addParticle(const char *filename){

	m_particleFilename[m_numberOfParticles] = filename;

	for(int i = 0; i < m_numberOfParticles;i++)
	{
		if(m_particleFilename[i].compare(filename) == 0)
		{
			return i;
		}
	}

	loadBitmap(filename,m_particleArray[m_numberOfParticles],false);

	m_numberOfParticles++;

	return m_numberOfParticles - 1;

}

int CTextureMan::addCrosshair(const char *filename){

	loadBitmap(filename,m_crosshairArray[m_numberOfCrosshairs],false);

	m_numberOfCrosshairs++;

	return m_numberOfCrosshairs - 1;

}

int CTextureMan::add(const char *filename,int r, int g, int b)
{
	m_filename[m_numberOfTextures] = filename;

	for(int i = 0; i < m_numberOfTextures;i++)
	{
		if(m_filename[i].compare(filename) == 0)
		{
			return i;
		}
	}

	loadBitmap(filename,m_textureArray[m_numberOfTextures],true);

	m_numberOfTextures++;

	return m_numberOfTextures - 1;
}


void CTextureMan::loadBitmap(const char *filename, unsigned int &image,bool alpha){

	int height,width;
	unsigned char *imageData;

	imageData = LoadBMP(filename,&width,&height,alpha,255,0,255);

	glGenTextures(1, &image);
	glBindTexture(GL_TEXTURE_2D,image);

    gluBuild2DMipmaps( GL_TEXTURE_2D, 4,width,height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

	free(imageData);
}

void CTextureMan::returnTextue(const char *filename,unsigned int &uint){

	loadBitmap(filename,uint,true);
}

void CTextureMan::bindTexture(int id){

	glBindTexture(GL_TEXTURE_2D,m_textureArray[id]);
}

void CTextureMan::bindSkin(int id){

	glBindTexture(GL_TEXTURE_2D,m_skinArray[id]);
}

void CTextureMan::bindParticle(int id){

	glBindTexture(GL_TEXTURE_2D,m_particleArray[id]);
}

void CTextureMan::bindCrosshair(int id){

	glBindTexture(GL_TEXTURE_2D,m_crosshairArray[id]);
}


void CTextureMan::save(ofstream &file){

	for(int i =0; i < m_numberOfTextures; i++)	
	{
		file << "textures " << i << "\n" << m_filename[i] << "\n";
	}

}

void CTextureMan::destroyTexture(int id)
{
	m_filename[id] = "";
	glDeleteTextures(1,&m_textureArray[id]);
}

void CTextureMan::freeAll(){

	for(int i = 0; i < m_numberOfTextures;i++)
	{
		glDeleteTextures(1,&m_textureArray[i]);
	}

	for(i = 0; i < m_numberOfSkins;i++)
	{
		glDeleteTextures(1,&m_skinArray[i]);
	}

	for(i = 0; i < m_numberOfParticles;i++)
	{
		glDeleteTextures(1,&m_particleArray[i]);
	}
}

void CTextureMan::loadSkin(UINT &texture,const char *filename)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!filename)									// Return from the function if no file name was passed in
		return;

	pBitmap = auxDIBImageLoad(filename);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	// Generate a texture with the associated texture variable
	glGenTextures(1, &texture);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture variable passed in
	glBindTexture(GL_TEXTURE_2D, texture);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}

void CTextureMan::loadTga(const char *filename, unsigned int &image){

	tgaInfo *imageTga;

	char name[32];

	strcpy(name,filename);

	imageTga = tgaLoad(name);

	glGenTextures(1, &image);
	glBindTexture(GL_TEXTURE_2D,image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageTga->width, imageTga->height, 
				0, GL_RGBA, GL_UNSIGNED_BYTE, imageTga->imageData);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

}

int CTextureMan::addMenuTexture(const char *filename)
{
	loadBitmap(filename,m_menuTextures[m_numberOfMenuTextures],true);
	
	return m_numberOfMenuTextures++;
}

void CTextureMan::bindMenuTexture(int id)
{
	glBindTexture(GL_TEXTURE_2D,m_menuTextures[id]);
}
