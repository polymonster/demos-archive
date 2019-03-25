#ifndef _TEXTURE_SINGLETON_H
#define _TEXTURE_SINGLETON_H

#include "globals.h"
#include "Helpers.h"
#include "tga.h"
#include "myTga.h"
#include "input.h"

#include <fstream>

typedef struct
{
	unsigned int texture;
	std::string filename;
	int id;

}STextureContainer;

class CTextureMan {

public:

	//singleton specific things
	static void create();
	static CTextureMan* getInstance(){return m_instance;};
	static void destroy();

	//data loaders
	int loadTga(const char *filename, unsigned int &image);

	//texture creators
	void loadCubeMap(char *front,char *back,char *left,char *right,char *top,char *bottom);
	int load(const char *filename);

	//binders
	void bindCubeMap();	
	void bindTexture(int id);
	void bindTexture(int id,int unit);

	//accessibility
	std::string printTextureName(int id);

	//manager
	void disableAllUnits();
	void disableUnit(int id);

	//clean up
	void deleteTexture(int id);
	void deleteAll();

	//enumeration
	void renderEnumeratedTextures();
	int getSelectedTexture(){return m_selectedTextureID;};

	inputReader input;

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

		m_enumerationSizes[0] = 32;
		m_enumerationSizes[1] = 64;
		m_enumerationSizes[2] = 128;
		m_enumerationSizes[3] = 256;

		input.leftMouse = false;
	};

	~CTextureMan(){/*nothing*/};

	int m_numberOfTextures;
	std::string m_filename[64];
	unsigned int m_texture[64];

	std::vector<STextureContainer> m_textureStorage;
	unsigned int m_cubeMap;

	int m_enumertionSize;
	int m_selectedTextureID;
	int m_enumerationSizes[4];

	SVector2<int> calculateRelativeMousePosition();
};

/*class CSingleton
{
	public:
	
	//create function used to make an instance
	static void create();

	//getInstance returns a pointer to our one instance 
	//of CSingleton
	static CSingleton* getInstance(){return m_instance;};

	//this is a normal public function we may want to do something
	void doSomething();

	private:

	//this is the instance of the class itself
	static CSingleton *m_instance;

	//the private constructor
	CSingleton(){/*intiialise here};
}*/


#endif
