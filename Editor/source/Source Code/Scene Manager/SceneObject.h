#ifndef _SCENE_OBJECT
#define _SCENE_OBJECT

typedef enum
{
	NO_EDIT_MODE = 0,
	EDIT_VIEW,
	EDIT_TERRAIN,
	EDIT_PROPS,
	EDIT_FOLIAGE,
	EDIT_WATER,
	EDIT_ATMOSPHERE,
	EDIT_OCTREE

}eEditModes;

typedef enum
{
	SELECT = 0,
	TRANSLATE,
	ROTATE,
	SCALE,
	SELECT_PROPS,
	SELECT_WATER,

}eSubEditModes;

class CSceneObject
{
	public:

		CSceneObject(){};
		virtual ~CSceneObject(){};

		virtual void render(){};
		virtual void backBufferRender(){};
		virtual void renderAux(){};
		virtual void execute(){};
		virtual void cleanUp(){};

		virtual void output(std::ofstream &fout){};
		virtual void readIn(std::ifstream &fin){};

		void clearRenderFlags(){m_renderedThisPass = false;};
		void setRenderFlags(bool rendered){m_renderedThisPass = rendered;};
		bool renderedThisPass(){return m_renderedThisPass;};

		SVector3 boxPos(){return m_boundingBoxPos;};
		SVector3 boxOrientation(){return m_boundingBoxOrientation;};
		SVector3 boxScale(){return m_boundingBoxScale;};

		std::string nameIdentifier;

		void lod(int lod){m_lod = lod;};

	protected:

		bool m_renderedThisPass;

		SVector3 m_boundingBoxPos;
		SVector3 m_boundingBoxOrientation;
		SVector3 m_boundingBoxScale;

		int m_lod;

};


#endif