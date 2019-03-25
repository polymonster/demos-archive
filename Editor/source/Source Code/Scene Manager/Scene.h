#ifndef _SCENE_H
#define _SCENE_H

#include "Globals.h"
#include "Helpers.h"
#include "Input.h"
#include "Terrain.h"
#include "View.h"
#include "SceneObject.h"
#include "Props.h"
#include "octree.h"
#include "atmosphere.h"

typedef struct
{
	std::string name;

	int modelManagerId;

	SVector3 pos;
	SVector3 scale;
	SVector3 rot;
	
	bool textured;
	bool normalMapped;
	bool parallaxMapped;

}SModelProperties;

extern bool g_loadNewScene;

class CScene{

private:

	//Variable output
	CConsole variableConsole;

	//Rendering specifics
	//Core for main 3d objects lit, texture, depth test
	//Aux for ghost objects, widgets and anything ontop of the main scene (edit related)
	void renderCore();
	void renderAux();
	void renderProps();
	void testBloom();

	std::vector<CSceneObject*> m_sceneObject;

	COctreeNode *octree;

	SVector3 lights;

	void save();
	void load();
	void newScene();

	CAtmosphere *m_atmosphere;
	
public:

	//class initialisation
	CScene();
	~CScene();
	
	//engine initialisation
	void initOpenGL();
	void initSingletons();
	void cleanUp();

	//rendering
	void render();
	void setOpenGLStates();
	void shadowRenders();
	void waterRenders();

	//updating
	void update();
	void buildOctree();
	void addObjects();

	void buildShadowTextureMatrix();
	void shadowCasterPos();

	//option handlers
	void sceneOptionHandler(WPARAM &wParam);
};

#endif