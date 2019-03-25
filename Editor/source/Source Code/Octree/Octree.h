#ifndef OCTREE_H
#define OCTREE_H

#include "globals.h"
#include "sceneObject.h"

extern int g_octreeEditMode;
extern int g_octreeMaxObjects;
extern int g_octreeMinSize;

class COctreeNode
{
	public:

	COctreeNode(SVector3 pos,SVector3 scale);
	COctreeNode(std::ifstream &fin);

	SVector3 nodePos;
	SVector3 nodeScale;
	SVector3 m_selectedAxis;

	void addObject(CSceneObject *object);
	void subdivide();
	void enumerate();
	void clear();
	void checkCollisions();
	void draw();

	void render();
	void execute();

	void backBufferRender();
	void renderAux();

	void clearRenderFlags();

	std::vector<CSceneObject*> objects;
	std::vector<COctreeNode*> childNodes;
	bool children;

	bool built;

	bool m_draw;
	bool m_build;
	bool m_use;

	void output(std::ofstream &fout);
	void readIn(std::ifstream &fin);

	private:

	bool m_pushed[3];
};

#endif