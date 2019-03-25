#ifndef _PROPS_H
#define _PROPS_H

#include "globals.h"
#include "sceneObject.h"

extern int g_propEditMode;

typedef struct
{
	int modelID;
	SVector3 pos;
	SVector3 rot;
	SVector3 scale;

}SCopyData;

class CProp : public CSceneObject
{
	public:

		void execute();

		void render();
		void renderAux(bool widgets);
		void backBufferRender();
		void renderPicking(colorByte color);

		void cleanUp();

		SVector3 getPos(){return m_position;};
		SCopyData getCopyData();

		void increment(SVector3 pos, SVector3 rot, SVector3 scale);

		CProp(){};
		CProp(int modelID);
		CProp(int modelID,SVector3 pos,SVector3 scale, SVector3 rotation);
		~CProp(){};

		int m_modelID;
		SVector3 m_position;
		SVector3 m_rotation;
		SVector3 m_scale;
		bool toDelete;

	private:

		SVector3 m_selectedAxis;
		
};

class CPropList : public CSceneObject
{
	public:

		CPropList();
		~CPropList();

		void cleanUp();
		
		//rendering
		void render();
		void renderAux();
		void backBufferRender();
		void renderModelViewer();

		//event handlers
		void execute();
		void dialogHandler();
		void enableControls(bool enable);

		//additions/removals
		void selectModel();
		void addModel();
		void deleteModel();
		void cloneModel();
		void foliageRandomisation(SVector3 *pos, SVector3 *scale, SVector3 *rotation);

		//node storage
		std::vector<CProp*> m_nodes;

		//saving
		void exportToTanky();
		void output(std::ofstream &fout);
		void readIn(std::ifstream &fin);
		void buildUndo();

	private:
		
		int m_numberOfProps;
		int m_selectedProp;

		//multiple selections
		std::vector<int>m_selectionList;

		SVector3 m_sharedWidgetPos;
		SVector3 m_sharedWidgetSelectedAxis;

		SVector2 <int> m_selectionRegionStart;
		SVector2 <int> m_selectionRegionDimensions;

		bool m_selectionBox;
		bool m_addedProp;
};


#endif