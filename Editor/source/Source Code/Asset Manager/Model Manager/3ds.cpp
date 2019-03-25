#include "Globals.h"
#include "3ds.h"


FILE *m_FilePointer;
tChunk *m_CurrentChunk;
tChunk *m_TempChunk;

//-------------------------------- CLOADS3DS ------------------------------------
//	This constructor initializes the tChunk data
//-------------------------------- CLOADS3DS ------------------------------------
//---------------------------------- IMPORT 3DS ----------------------------------
//		This is called by the client to open the .3ds file, read it, then clean up
//---------------------------------- IMPORT 3DS -----------------------------------

bool Import3DS(t3DModel *pModel, char *strFileName)
{
	m_CurrentChunk = new tChunk;
	m_TempChunk = new tChunk;

	char strMessage[255] = {0};
	
	m_FilePointer = fopen(strFileName, "rb");
	
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	
	ReadChunk(m_CurrentChunk);
	
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	
	ProcessNextChunk(pModel, m_CurrentChunk);
	
	ComputeSmoothNormals(pModel);

	fclose(m_FilePointer);						 
	
	delete m_CurrentChunk;						
	delete m_TempChunk;
	
	//CleanUp();
	
	return 0;
}

//---------------------------------- CLEAN UP ----------------------------------
//		This function cleans up our allocated memory and closes the file
//---------------------------------- CLEAN UP ----------------------------------

void CleanUp()
{

	//m_CurrentChunk = new tChunk;				 
	//m_TempChunk    = new tChunk;
	
}


//---------------------------------- PROCESS NEXT CHUNK-----------------------------------
//	This function reads the main sections of the .3DS file, then dives deeper with recursion
//---------------------------------- PROCESS NEXT CHUNK-----------------------------------

void ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = {0};					 
	tMaterialInfo newTexture = {0};				
	unsigned short version = 0;						
	int *buffer = new int[50000];
	
	m_CurrentChunk = new tChunk;				
	
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(m_CurrentChunk);
		
		switch (m_CurrentChunk->ID)
		{
		case VERSION:							
			
			m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			
			if (version > 0x03)
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			break;
			
		case OBJECTINFO:						
			ReadChunk(m_TempChunk);
			
			m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
			
			m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;
			
			ProcessNextChunk(pModel, m_CurrentChunk);
			break;
			
		case MATERIAL:						
			pModel->numOfMaterials++;
			
			pModel->pMaterials.push_back(newTexture);
			
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;
			
		case OBJECT:							
			pModel->numOfObjects++;
			
			pModel->pObject.push_back(newObject);
			
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));
			
			m_CurrentChunk->bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
			
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
			break;
			
		case EDITKEYFRAME:
			
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
			
		default: 
			
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
		
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	
	delete m_CurrentChunk;
	delete buffer;
	m_CurrentChunk = pPreviousChunk;
}


//---------------------------------- PROCESS NEXT OBJECT CHUNK -----------------------------------
//		This function handles all the information about the objects in the file
//---------------------------------- PROCESS NEXT OBJECT CHUNK -----------------------------------

void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	int *buffer = new int[50000];
	
	m_CurrentChunk = new tChunk;
	
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(m_CurrentChunk);
		
		switch (m_CurrentChunk->ID)
		{
		case OBJECT_MESH:		
			ProcessNextObjectChunk(pModel, pObject, m_CurrentChunk);
			break;
			
		case OBJECT_VERTICES:			 
			ReadVertices(pObject, m_CurrentChunk);
			break;
			
		case OBJECT_FACES:				 
			ReadVertexIndices(pObject, m_CurrentChunk);
			break;
			
		case OBJECT_MATERIAL:			 
			ReadObjectMaterial(pModel, pObject, m_CurrentChunk);			
			break;
			
		case OBJECT_UV:					
			ReadUVCoordinates(pObject, m_CurrentChunk);
			break;
			
		default: 
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
		
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	
	delete m_CurrentChunk;
	delete buffer;
	m_CurrentChunk = pPreviousChunk;
}


//---------------------------------- PROCESS NEXT MATERIAL CHUNK -----------------------------------
//	This function handles all the information about the material (Texture)
//---------------------------------- PROCESS NEXT MATERIAL CHUNK -----------------------------------

void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	int *buffer = new int[50000];

	m_CurrentChunk = new tChunk;
	
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(m_CurrentChunk);
		
		switch (pModel, m_CurrentChunk->ID)
		{
		case MATNAME:						 
			m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
			
		case MATDIFFUSE:					 
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrentChunk);
			break;
			
		case MATMAP:						 
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;
			
		case MATMAPFILE:						 
			m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
			
		default:  
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	
	delete m_CurrentChunk;
	delete buffer;
	m_CurrentChunk = pPreviousChunk;
}

//---------------------------------- READ CHUNK ----------------------------------
//		This function reads in a chunk ID and it's length in bytes
//---------------------------------- READ CHUNK -----------------------------------

void ReadChunk(tChunk *pChunk)
{
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
	
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

//---------------------------------- GET STRING -----------------------------------
//		This function reads in a string of characters
//---------------------------------- GET STRING -----------------------------------

int GetString(char *pBuffer)
{
	int index = 0;
	
	fread(pBuffer, 1, 1, m_FilePointer);
	
	while (*(pBuffer + index++) != 0) 
	{
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}
	
	return strlen(pBuffer) + 1;
}


//---------------------------------- READ COLOR ----------------------------------
//		This function reads in the RGB color data
//---------------------------------- READ COLOR -----------------------------------

void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
	ReadChunk(m_TempChunk);
	m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
	pChunk->bytesRead += m_TempChunk->bytesRead;
}


//---------------------------------- READ VERTEX INDECES ----------------------------------
//		This function reads in the indices for the vertex array
//---------------------------------- READ VERTEX INDECES -----------------------------------

void ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;					 
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);
	
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);
	
	
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);
			
			if(j < 3)
			{
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


//---------------------------------- READ UV COORDINATES -----------------------------------
//		This function reads in the UV coordinates for the object
//---------------------------------- READ UV COORDINATES -----------------------------------

void ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);
	
	pObject->pTexVerts = new SVector2<float>[pObject->numTexVertex];
	
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


//---------------------------------- READ VERTICES -----------------------------------
//	This function reads in the vertices for the object
//---------------------------------- READ VERTICES -----------------------------------

void ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{ 
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);
	
	pObject->pVerts = new SVector3 [pObject->numOfVerts];

	//memset(pObject->pVerts, 0, sizeof(SVector3) * pObject->numOfVerts);
	
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


//---------------------------------- READ OBJECT MATERIAL -----------------------------------
//	This function reads in the material name assigned to the object and sets the materialID
//---------------------------------- READ OBJECT MATERIAL -----------------------------------

void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};			
	int buffer[50000] = {0};				
	
	pPreviousChunk->bytesRead += GetString(strMaterial);
	
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			pObject->materialID = i;
			
			if(strlen(pModel->pMaterials[i].strFile) > 0) {
				
				pObject->bHasTexture = true;
			}	
			break;
		}
	}
	
	pPreviousChunk->bytesRead += fread(buffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}			

//---------------------------------- COMPUTER NORMALS -----------------------------------
//		This function computes the normals and vertex normals of the objects
//---------------------------------- COMPUTER NORMALS -----------------------------------

void ComputeSmoothNormals(t3DModel *pModel)
{
	SVector3 vVector1, vVector2, vNormal, vPoly[3];
	
	if(pModel->numOfObjects <= 0)
		return;
	
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		t3DObject *pObject = &(pModel->pObject[index]);
		
		SVector3 *pNormals		= new SVector3 [pObject->numOfFaces];
		SVector3 *pTempNormals	= new SVector3 [pObject->numOfFaces];
		pObject->pNormals		= new SVector3 [pObject->numOfVerts];
		
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];
			
			vVector1 = vPoly[0] - vPoly[2];				
			vVector2 = vPoly[2] - vPoly[1];				
			
			vNormal = cross(vVector1,vVector2);

			pTempNormals[i] = vNormal;

			vNormal  = normalize(vNormal);				
			pNormals[i] = vNormal;						
		}
		
		
		SVector3 vSum(0.0, 0.0, 0.0);
		SVector3 vZero = vSum;
		int shared=0;
		
		for (int i = 0; i < pObject->numOfVerts; i++)			
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	
			{												
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = vSum + pTempNormals[j];			
					shared++;								
				}
			}      
			
			pObject->pNormals[i] = divideVectorByScalar(vSum, float(-shared));
			pObject->pNormals[i] = normalize(pObject->pNormals[i]);	
			
			vSum = vZero;									 
			shared = 0;										
		}
		
		delete [] pTempNormals;
		delete [] pNormals;
	}
}

void ComputeNormals(t3DModel *pModel)
{
	SVector3 vVector1, vVector2, vNormal, vPoly[3];

	if(pModel->numOfObjects <= 0)
		return;

	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		if(pModel->pObject.size() <= 0) break;	

	    t3DObject *pObject = &pModel->pObject[i];

		pObject->pNormals = new SVector3 [pObject->numOfVerts];

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
				vPoly[0] = pObject->pVerts[pObject->pFaces[j].vertIndex[0]];
				vPoly[1] = pObject->pVerts[pObject->pFaces[j].vertIndex[1]];
				vPoly[2] = pObject->pVerts[pObject->pFaces[j].vertIndex[2]];
			
				vVector1 = vPoly[0] - vPoly[2];				
				vVector2 = vPoly[0] - vPoly[1];				
			
				vNormal = cross(vVector1,vVector2);

				pObject->pNormals[pObject->pFaces[j].vertIndex[0]] = normalize(vNormal);
				pObject->pNormals[pObject->pFaces[j].vertIndex[1]] = normalize(vNormal);	
				pObject->pNormals[pObject->pFaces[j].vertIndex[2]] = normalize(vNormal);	
		}
	}

}


