#ifndef _MODEL_SINGLETON_H
#define _MODEL_SINGLETON_H

#include "globals.h"
#include "windowsThings.h"
#include "myMath.h"
#include "player.h"
#include "md3.h"

#include "3dsLoader.h"
#include "3ds.h"

#include <fstream>

class CAnimatedModel{

private:

	int m_meshID[32];
	int m_animID[32];
	int m_textureID[32];

	int m_numberOfMeshes;
	int m_numberOfAnimations;

	CalCoreModel *coreModel;
	CalModel *model;

	GLuint vbo;

public:

	CAnimatedModel(const char *filename);
	~CAnimatedModel();
	void renderCal(int id);

	void update(int id);
	void setState(int id);
	void executeState(int id);
	void clear(int id);
};


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

	void printName(ofstream &fout,int model);

	void parseName(char *root, int model);

	int checkCollision(CPlayer *player,int id);
	bool lineCollision(SVector3 line[],int id,SVector3 *result, STriangle *tri, SVector3 *normal);

	int loadCharacter(const char *filename);
	
	void renderCharacter(int id);
	void updateCharacter(int id);

	void translateToVertex(int i,int id);

	void renderLightmap(int id);

	void checkAllCollision(CPlayer *player,int id,bool justHeight);
	void setCollisionGrid(int id);
	bool checkFrustum(int id,CFrustum frustum);

	CAnimatedModel *character[12];

	void destroyObjects();

	void destroyCharacters();

	void destroyModel(int id);

	int polyCounter(){return m_polyCounter;};

	int loadMd3(char *filename,char *characterName);
	void linkMd3(CModelMD3 &character, int id);
	void destroyMd3(int id);


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
					m_numberOfMd3 = 0;
					};

	~CModelManager(){/*nothing*/};

	/*CalCoreModel *coreModel[10];
	CalModel *calModel[10];

	int meshID[10][18];
	int animID[10][10];*/
	
	int m_numberOfModels;
	int m_numberOfCharacters;

	//string m_filename[32];

	int surroundingColsest[32];

	int m_deletedModelStart;

	int m_polyCounter;

	int m_numberOfMd3;

	CModelMD3 md3MeshModel[64];
};


#endif