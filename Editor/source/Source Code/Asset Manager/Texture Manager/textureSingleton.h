#ifndef _TEXTURE_SINGLETON_H
#define _TEXTURE_SINGLETON_H

#include "globals.h"
#include "Helpers.h"
#include "myTga.h"
#include "input.h"

#include <fstream>

typedef struct
{
	//OpenGL texture handle
	unsigned int texture;
	//TExtures filename
	std::string filename;
	//applications internal texture ID system
	int id;

}STextureContainer;



class CTextureMan {

public:

	//singleton specific things
	static void create();
	static CTextureMan* getInstance(){return m_instance;};
	static void destroy();

	//data loaders
	int loadTga(const char *filename, unsigned int &image,bool warning);
	void loadBasePack();

	//texture creators
	void cubeMapConfig(char *file);
	void loadCubeMap(char *front,char *back,char *left,char *right,char *top,char *bottom,char *name);
	int load(char *filename, bool warning);

	//binders
	void bindCubeMap(int id);	
	void bindTexture(int id);
	void bindTexture(int id,int unit);
	void unbindCubeMap();

	//accessibility
	std::string printTextureName(int id);
	int checkForDuplicates(STextureContainer *newTexture);
	void parseName(STextureContainer *newTexture, char *file);

	//manager
	void disableAllUnits();
	void disableUnit(int id);

	//clean up
	void deleteTexture(int id);
	void deleteAll();

	//enumeration
	void textureManagerWindow();
	void renderEnumeratedTextures();
	int getSelectedTexture(){return m_selectedTextureID;};
	int getSelectedCubemap(){return m_selectedCubemapID;};
	void renderEnumeratedCubemaps();

	inputReader input;

	void export();
	void output(std::ofstream &fout);

	void import();
	void readIn(std::ifstream &fin);

private:

	//all initialisation in here
	//so only on instance can call them

	//the instance itself
	static CTextureMan *m_instance;

	CTextureMan()
	{
		m_numberOfTextures = 0;
		m_enumertionSize = 128;
		m_selectedTextureID = 0;
		m_selectedCubemapID = -1;

		m_enumerationSizes[0] = 32;
		m_enumerationSizes[1] = 64;
		m_enumerationSizes[2] = 128;
		m_enumerationSizes[3] = 256;

		input.leftMouse = false;

		m_textureStorage.clear();
	};

	~CTextureMan(){/*nothing*/};

	int m_numberOfTextures;
	std::string m_filename[64];
	unsigned int m_texture[64];

	std::vector<STextureContainer> m_textureStorage;
	std::vector<STextureContainer> m_cubeMapStorage;
	unsigned int m_cubeMap;

	int m_enumertionSize;
	int m_selectedTextureID;
	int m_enumerationSizes[4];
	int m_selectedCubemapID;

	SVector2<float> calculateRelativeMousePosition();
};


#endif


