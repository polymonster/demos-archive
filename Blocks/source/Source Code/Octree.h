#ifndef OCTREE_H
#define OCTREE_H

#include "globals.h"
#include "toyBlock.h"

class COctreeNode
{
	public:

	COctreeNode(SVector3 pos,SVector3 scale);

	SVector3 nodePos;
	SVector3 nodeScale;

	void addObject(CToyBlock *object);
	void subdivide();
	void enumerate();
	void clear();
	void checkCollisions();
	void draw();

	void render();

	void clearRenderFlags();

	std::vector<CToyBlock*> objects;
	std::vector<COctreeNode*> childNodes;
	bool children;
};

#endif