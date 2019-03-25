#ifndef FOLIAGE_H
#define FOLIAGE_H

#include "globals.h"
#include "sceneObject.h"

class CFoliageNode : public CSceneObject
{
	public:

	CFoliageNode();
	CFoliageNode(SVector3 position,int id);

	void render();

	private:

	SVector3 m_position;
	int m_id;
};

class CFoliage : public CSceneObject
{
	public:

	CFoliage();
	~CFoliage();

	void render();
	void execute();

	std::vector<CSceneObject*> m_nodes;

	private:

	int m_currentModel;
};

#endif