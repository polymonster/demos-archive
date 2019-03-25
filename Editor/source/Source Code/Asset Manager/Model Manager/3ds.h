#ifndef _3DS_H
#define _3DS_H

// Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

// Main Chunks
#define OBJECTINFO    0x3D3D				 
#define VERSION       0x0002				
#define EDITKEYFRAME  0xB000				

// Sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				
#define OBJECT		  0x4000				

// Sub defines of MATERIAL
#define MATNAME       0xA000				
#define MATDIFFUSE    0xA020				
#define MATMAP        0xA200				
#define MATMAPFILE    0xA300				

#define OBJECT_MESH   0x4100				

// Sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			
#define OBJECT_FACES		0x4120			
#define OBJECT_MATERIAL		0x4130			
#define OBJECT_UV			0x4140

#include "Globals.h"
#include "myMath.h"
//////////////////////////////////////
//The tFace Struct
//////////////////////////////////////
struct tFace
{
	int vertIndex[3];												
};

//////////////////////////////////////
//The tMaterialInfo Struct
//////////////////////////////////////
struct tMaterialInfo
{
	char  strName[255];							
	char  strFile[255];							
	BYTE  color[3];								
	int   texureId;								
	float uTile;								
	float vTile;								
	float uOffset;								
	float vOffset;									
};

//////////////////////////////////////
//The t3DObject Struct
//////////////////////////////////////
struct t3DObject 
{
	int  numOfVerts;			
	int  numOfFaces;			
	int  numTexVertex;			
	int  materialID;			
	bool bHasTexture;			
	char strName[255];			

	SVector3  *pVerts;			
	SVector3  *pNormals;		
	SVector2 <float>  *pTexVerts;		
	tFace *pFaces;				
};

//////////////////////////////////////
//The t3DModel Struct
//////////////////////////////////////
struct t3DModel 
{
	int numOfObjects;							
	int numOfMaterials;							
	std::vector<tMaterialInfo> pMaterials;			
	std::vector<t3DObject> pObject;	

	t3DModel(t3DModel & ref)
	{
		numOfObjects = ref.numOfObjects;
		numOfMaterials = ref.numOfMaterials;

		pMaterials.resize(numOfMaterials);
		pObject.resize(numOfObjects);

		for(int i = 0; i < numOfMaterials; i++)
		{
			strcpy(pMaterials.at(i).strFile,ref.pMaterials.at(i).strFile); 
		}

		for(int i = 0; i < numOfObjects; i++)
		{ 
			pObject.at(i).numOfFaces = ref.pObject.at(i).numOfFaces;
			pObject.at(i).numOfVerts = ref.pObject.at(i).numOfVerts;
			pObject.at(i).numTexVertex = ref.pObject.at(i).numTexVertex;
			pObject.at(i).bHasTexture = ref.pObject.at(i).bHasTexture;
			pObject.at(i).materialID = ref.pObject.at(i).materialID;

			pObject.at(i).pFaces = new tFace [ref.pObject.at(i).numOfFaces];

			for(int j = 0; j < ref.pObject.at(i).numOfFaces; j++)
			{
				pObject.at(i).pFaces[j].vertIndex[0] = ref.pObject.at(i).pFaces[j].vertIndex[0];
				pObject.at(i).pFaces[j].vertIndex[1] = ref.pObject.at(i).pFaces[j].vertIndex[1];
				pObject.at(i).pFaces[j].vertIndex[2] = ref.pObject.at(i).pFaces[j].vertIndex[2];
			}
			
			pObject.at(i).pVerts = new SVector3 [ref.pObject.at(i).numOfVerts];
			pObject.at(i).pNormals = new SVector3 [ref.pObject.at(i).numOfVerts];

			for(int j = 0; j < ref.pObject.at(i).numOfVerts; j++)
			{
				pObject.at(i).pVerts[j] =  ref.pObject.at(i).pVerts[j];
				pObject.at(i).pNormals[j] =  ref.pObject.at(i).pNormals[j];
			}

			pObject.at(i).pTexVerts = new SVector2<float>[ref.pObject.at(i).numTexVertex];

			for(int j = 0; j < ref.pObject.at(i).numTexVertex; j++)
			{
				pObject.at(i).pTexVerts[j] =  ref.pObject.at(i).pTexVerts[j];
			}

		}
	};

	t3DModel(){}; 
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct tIndices {							

	unsigned short a, b, c, bVisible;		
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This holds the chunk info
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct tChunk
{
	unsigned short int ID;					
	unsigned int length;					
	unsigned int bytesRead;					
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This class handles all of the loading code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class CLoad3DS
//{
//public:
	
	//CLoad3DS();								

	bool Import3DS(t3DModel *pModel, char *strFileName);

//private:
	int GetString(char *);

	void ReadChunk(tChunk *);

	void ProcessNextChunk(t3DModel *pModel, tChunk *);

	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);

	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);

	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);

	void ReadVertices(t3DObject *pObject, tChunk *);

	void ReadVertexIndices(t3DObject *pObject, tChunk *);

	void ReadUVCoordinates(t3DObject *pObject, tChunk *);

	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);
	
	void ComputeNormals(t3DModel *pModel);

	void ComputeSmoothNormals(t3DModel *pModel);

	void CleanUp();
//};

#endif



