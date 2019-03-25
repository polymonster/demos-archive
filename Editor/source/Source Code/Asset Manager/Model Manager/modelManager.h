#ifndef _MODEL_SINGLETON_H
#define _MODEL_SINGLETON_H

#include "Globals.h"
#include "myMath.h"

#include <fstream>
#include <string>

#include "3dsLoader.h"
#include "3ds.h"

typedef struct
{
	C3dsLoader *mesh;
	C3dsLoader *lod[2];
	C3dsLoader *collision;

	std::string name;
	std::string filename;

	int cullMode;
	bool collisionMesh;
	int numberOfLod;

	int raw_id;

}SModelContainer;

typedef enum
{
	LOD_1 = 0,
	LOD_2,
	LOD_3,
	COLLISION,

}eChildTypes;

class CModelManager {

public:

	//singleton specific things
	static void create();
	static void destroy();
	static CModelManager* getInstance(){return m_instance;};

	//model management
	int  load(char *file, bool parse = true, int cullMode = 0);
	void loadChild(char *file, int childType);
	void cleanUp();
	void destroyModel(int id);
	void destroyObjects();
	void deleteVBO(int id);
	void drawDebug(int id);
	void drawBB(int id);

	//rendering
	void render(int id);
	void renderPicking(int id,colorByte color);
	void renderLOD(int id, int LOD);

	//operations
	void parseName(SModelContainer *nameModel, char *file);
	void parseFilename(SModelContainer *nameModel, char *file);
	void setCollisionGrid(int id);
	bool checkFrustum(int id,CFrustum frustum);
	void getBox(int id, SVector3 *pos,SVector3 *scale);
	int polyCounter(){return m_polyCounter;};
	bool selectedIsValid();
	int idLookup(int position);
	void addToLookup(int position, int id);
	void updateLookup(int deleted_position);
	int checkForDuplicates(SModelContainer *model);

	void updateListBox(int removal = 0);

	//model viewer / manager front end
	void enumerateModels();
	void renderModelViewer();
	void fillModelList();

	int selectedModel();
	void setSelected(int id);
	void checkRemoval();
	int getArrayLocation(int listID);

	std::string getModelName(int id);

	//saving / loading
	void export();
	void output(std::ofstream &fout);

	void import();
	void readIn(std::ifstream &fin);

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

	int surroundingColsest[32];

	int m_deletedModelStart;

	int m_polyCounter;

	std::vector<SModelContainer> modelContainer;

	int m_selectedModel;
	int m_selectedListPos;

	int m_lookupTable[1024];
};


#endif