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



//////////////////////////////////////
//The tFace Struct
//////////////////////////////////////
struct t3DSFace
{
	int vertIndex[3];							
	int coordIndex[3];							
};

//////////////////////////////////////
//The t3DSMaterialInfo Struct
//////////////////////////////////////
struct t3DSMaterialInfo
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
//The t3DSObject Struct
//////////////////////////////////////
struct t3DSObject 
{
	int  numOfVerts;			
	int  numOfFaces;			
	int  numTexVertex;			
	int  materialID;			
	bool bHasTexture;			
	char strName[255];			
	UINT      *pIndices;		
	tVector3  *pVerts;			
	tVector3  *pNormals;		
	tVector2  *pTexVerts;		
	t3DSFace *pFaces;				
};

//////////////////////////////////////
//The t3DSModel Struct
//////////////////////////////////////
struct t3DSModel 
{
	int numOfObjects;							
	int numOfMaterials;							
	vector<t3DSMaterialInfo> pMaterials;			
	vector<t3DSObject> pObject;					
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
class CLoad3DS
{
public:
	CLoad3DS();								

	bool Import3DS(t3DSModel *pModel, char *strFileName);

private:
	int GetString(char *);

	void ReadChunk(tChunk *);

	void ProcessNextChunk(t3DSModel *pModel, tChunk *);

	void ProcessNextObjectChunk(t3DSModel *pModel, t3DSObject *pObject, tChunk *);

	void ProcessNextMaterialChunk(t3DSModel *pModel, tChunk *);

	void ReadColorChunk(t3DSMaterialInfo *pMaterial, tChunk *pChunk);

	void ReadVertices(t3DSObject *pObject, tChunk *);

	void ReadVertexIndices(t3DSObject *pObject, tChunk *);

	void ReadUVCoordinates(t3DSObject *pObject, tChunk *);

	void ReadObjectMaterial(t3DSModel *pModel, t3DSObject *pObject, tChunk *pPreviousChunk);
	
	void ComputeNormals(t3DSModel *pModel);

	void CleanUp();

	FILE *m_FilePointer;
	
	tChunk *m_CurrentChunk;
	tChunk *m_TempChunk;
};

#endif



