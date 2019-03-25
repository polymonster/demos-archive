#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "globals.h"
#include "Helpers.h"
#include "Input.h"
#include "SceneObject.h"

//define for 255 x 255 which is the max for 
//one index of the colour buffer for picking
//ie selectPoly.xy = (255,255) so increment
//every channel in the z colour by this ammount
#define MAX_SINGLE_INDEX 6025 
#define PALLET_SIZE 8

//to store brush properties
typedef struct
{
	float strength;
	float falloff;
	int radius;
	int sqaureInfluence;
	int cursorID;
	float average;

	//active property will be a texture 
	//id OR a height edit mode
	int activeProperty; 

}brushProperties;

//to store texture weights
typedef struct
{
	float channel[4];

}STextureWeight;

extern int g_terrainSculptMode;

typedef enum
{
	SCULPT = 0,
	FLATTEN,
	NOISE,
	SMOOTH

}eSculptModes;

class CTerrain : public CSceneObject
{

private:

	//var storage **tidy these up - make dynamic**
	bool m_modifiedPosition[200][200];
	bool m_modifiedTexture[200][200];
	SVector3 m_textureWeight[200][200];
	SVector3 m_activeTexture;
	SVector3 m_vertex[200][200];
	SVector3 m_vertex_temp[200][200];
	SVector3 m_normal[200][200];
	SVector3 m_averageNormal[200][200];
	colorByte m_pickingColor[200][200];
	STextureWeight m_weightsFirst[200][200];
	STextureWeight m_weightsSecond[200][200];

	//vbo id's
	unsigned int vertexVBO;
	unsigned int normalVBO;
	unsigned int textureCoordVBO;
	unsigned int pickingVBO;
	unsigned int IBO;
	unsigned int pickingIBO;
	unsigned int textureWeightFirstVBO;
	unsigned int textureWeightSecondVBO;
	unsigned int *vboIndices;
	unsigned int *pickingIndices;

	//for VBO's storage (these are fine) 
	SVector3 *vboVertices;
	SVector3 *vboNormals;
	SVector3 *colourPicking;
	SVector2<float> *vboTextureCoords;
	colorByte *vboPickingID;
	STextureWeight *vboTextureWeightsFirst;
	STextureWeight *vboTextureWeightsSecond;
	
	//size variables
	int m_width, m_height;
	int m_tileSize;
	int vertexCount;
	int indexCount;
	int pickingIndexCount;

	//edit variables
	int m_editMode;
	brushProperties m_brush;
	int m_texturePallet[PALLET_SIZE];
	int m_selectedTextureId;

	//selection variables
	SVector2<int> m_selectedPolygon;
	bool m_selectedTexture[PALLET_SIZE];

	//undo/redo info
	terrainHeightUndo m_undoStack;
	terrainPaintUndo m_undoStackPaint;

public:
	
	CTerrain();
	CTerrain(int width, int height, int tileSize);
	CTerrain(std::ifstream &fin);
	~CTerrain();

	//initialisation
	void create();
	void fillVBO();

	//rendering
	void render();
	void renderVBO();
	void renderWireframe();
	void renderEdit();
	void drawBrush(int brushType);
	void backBufferRender();

	//update
	void updateVBO();
	void updateTextureWeights();
	void smoothen();

	//mathematical calculations
	SVector3 calculateSmoothNormals(int i, int j);
	SVector3 calculateNormals(int i, int j);
	float insideRadius(float x, float z, int radius);

	//saving/loading
	void output(std::ofstream &fout);
	void readIn(std::ifstream &fin);
	void export();
	void exportSF();

	//editing
	void execute();
	void edit();
	void height();
	void paint();
	bool getEditChunks(SVector2<int> *startChunk,SVector2<int> *endChunk);
	void setActiveTextureLayer(int id);
	void clamp(STextureWeight *unit1, STextureWeight *unit2);
	void mirrorVertices();

	//editing / window management
	void setBrushProperties();
	void setupToolboxes();
	void keyboardCommands();
	void controls();
	void renderToolbox();
	void hideWindows();
	void showWindows();
	
	//cleanup
	void deleteVBO();
	void cleanUp();

	//undo stacks **rename these**
	void command(commandList *commandStack,commandList *recieveStack);
	void addToUndoStack(unsigned int x, unsigned int y);
	void createUndoLevel();
	void addToUndoStack2(unsigned int x, unsigned int y);
	void createUndoLevel2();

};

#endif