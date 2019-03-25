#ifndef _TEXTURE_SINGLETON_H
#define _TEXTURE_SINGLETON_H

#include "globals.h"
#include "windowsThings.h"

#include "resource files/imageio/bmp.h"
#include "resource files/imageio/tga.h"
#include "resource files/imageio/pcx.h"
#include "loadBMP.h"

#include "tga.h"

#include <fstream>

class CTextureMan {

public:

	//singleton specific things
	static void create();

	static CTextureMan* getInstance(){return m_instance;};

	static void destroy();

	//texture management
	int add(const char *filename);
	int add(const char *filename,int r, int g, int b);
	int addSkin(const char *filename,int mode);
	int addParticle(const char *filename);

	unsigned int getTexture(int id){return m_textureArray[id];};
	void destroyTexture(int id);
	void freeAll();
	void bindTexture(int id);
	void bindSkin(int id);
	void bindParticle(int id);

	void bindCrosshair(int id);
	int addCrosshair(const char *filename);

	void returnTextue(const char *filename,unsigned int &uint);

	void save(ofstream &file);
	void load();

	void HFlip(unsigned char *ImageData, int ImageWidth, int ImageHeight);
	//file i/o and texture application
	void loadBitmap(const char *filename, unsigned int &image,bool alpha);
	void loadJpeg(const char *filename, unsigned int &image,bool alpha);
	void loadTga(const char *filename, unsigned int &image);
	void loadPcx(const char *filename, unsigned int &image,bool alpha);
	void loadSkin(UINT &texture,const char *filename);

	int addMenuTexture(const char *filename);
	void bindMenuTexture(int id);

private:

	//all initialisation in here
	//so oly on instance can call them

	//the instance itself
	static CTextureMan *m_instance;

	CTextureMan(){m_numberOfTextures = 0;
				  m_numberOfSkins = 0;
				  m_numberOfParticles = 0;
				  m_numberOfCrosshairs = 0;
				  m_menuTextures = new unsigned int[64];
				  m_numberOfMenuTextures = 0;};

	~CTextureMan(){/*nothing*/};

	//data struct to store the images
	unsigned int m_textureArray[128];
	string m_filename[128];
	int m_numberOfTextures;

	unsigned int m_skinArray[64];
	string m_skinFilename[64];
	int m_numberOfSkins;

	string m_particleFilename[32];
	unsigned int m_particleArray[32];
	int m_numberOfParticles;

	string m_interfacefilename[8];
	unsigned int m_interfaceArray[8];
	int m_numberOfInterfaces;

	unsigned int m_crosshairArray[8];
	int m_numberOfCrosshairs;

	unsigned int *m_menuTextures;
	int m_numberOfMenuTextures;

};


#endif
