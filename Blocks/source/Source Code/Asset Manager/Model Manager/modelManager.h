#ifndef _MODEL_SINGLETON_H
#define _MODEL_SINGLETON_H

#include "Globals.h"
#include "myMath.h"

#include <fstream>
#include <string>

#include "3dsLoader.h"
#include "3ds.h"

class CModelManager {

public:

	//singleton specific things
	static void create();

	static CModelManager* getInstance(){return m_instance;};

	static void destroy();

	//model management
	int open(const char *filename);
	int load(char *file);

	void render(int id);
	void renderNoTex(int id);

	//void printName(ofstream &fout,int model);

	void parseName(char *root, int model);

	//int checkCollision(CPenguin &player,int id);

	int loadCharacter(const char *filename);
	
	void renderCharacter(int id);
	void updateCharacter(int id);

	void translateToVertex(int i,int id);

	void renderLightmap(int id);

	//void checkAllCollision(CPenguin &player,int id,bool justHeight);

	void setCollisionGrid(int id);
	bool checkFrustum(int id,CFrustum frustum);

	void destroyObjects();

	void deleteVBO(int id);

	void destroyCharacters();

	void destroyModel(int id);

	int polyCounter(){return m_polyCounter;};

	void drawDebug(int id);


private:

	//all initialisation in here
	//so oly on instance can call them

	//the instance itself
	static CModelManager *m_instance;

	CModelManager(){
					m_numberOfModels = 0;
					m_numberOfCharacters = 0;
					m_deletedModelStart = 0;
					m_polyCounter = 0;
					};

	~CModelManager(){/*nothing*/};
	
	int m_numberOfModels;
	int m_numberOfCharacters;

	//string m_filename[32];

	int surroundingColsest[32];

	int m_deletedModelStart;

	int m_polyCounter;
};


#endif