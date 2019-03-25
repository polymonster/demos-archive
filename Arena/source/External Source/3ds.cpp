
#include "globals.h"
#include "3ds.h"
#include "3dmath.h"


//-------------------------------- CLOADS3DS ------------------------------------
//	This constructor initializes the tChunk data
//-------------------------------- CLOADS3DS ------------------------------------

CLoad3DS::CLoad3DS()
{
	m_CurrentChunk = new tChunk;				 
	m_TempChunk    = new tChunk;					
}

//---------------------------------- IMPORT 3DS ----------------------------------
//		This is called by the client to open the .3ds file, read it, then clean up
//---------------------------------- IMPORT 3DS -----------------------------------

bool CLoad3DS::Import3DS(t3DSModel *pModel, char *strFileName)
{

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
	
	ComputeNormals(pModel);
	
	CleanUp();
	
	return 0;
}

//---------------------------------- CLEAN UP ----------------------------------
//		This function cleans up our allocated memory and closes the file
//---------------------------------- CLEAN UP ----------------------------------

void CLoad3DS::CleanUp()
{
	fclose(m_FilePointer);						 
	
	delete m_CurrentChunk;						
	delete m_TempChunk;

	m_CurrentChunk = new tChunk;				 
	m_TempChunk    = new tChunk;
	
}


//---------------------------------- PROCESS NEXT CHUNK-----------------------------------
//	This function reads the main sections of the .3DS file, then dives deeper with recursion
//---------------------------------- PROCESS NEXT CHUNK-----------------------------------

void CLoad3DS::ProcessNextChunk(t3DSModel *pModel, tChunk *pPreviousChunk)
{
	t3DSObject newObject = {0};					 
	t3DSMaterialInfo newTexture = {0};				
	unsigned short version = 0;					
	int buffer[50000] = {0};					
	
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
			
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DSObject));
			
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
	m_CurrentChunk = pPreviousChunk;
}


//---------------------------------- PROCESS NEXT OBJECT CHUNK -----------------------------------
//		This function handles all the information about the objects in the file
//---------------------------------- PROCESS NEXT OBJECT CHUNK -----------------------------------

void CLoad3DS::ProcessNextObjectChunk(t3DSModel *pModel, t3DSObject *pObject, tChunk *pPreviousChunk)
{
	int buffer[50000] = {0};					
	
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
	m_CurrentChunk = pPreviousChunk;
}


//---------------------------------- PROCESS NEXT MATERIAL CHUNK -----------------------------------
//	This function handles all the information about the material (Texture)
//---------------------------------- PROCESS NEXT MATERIAL CHUNK -----------------------------------

void CLoad3DS::ProcessNextMaterialChunk(t3DSModel *pModel, tChunk *pPreviousChunk)
{
	int buffer[50000] = {0};					  
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
	m_CurrentChunk = pPreviousChunk;
}

//---------------------------------- READ CHUNK ----------------------------------
//		This function reads in a chunk ID and it's length in bytes
//---------------------------------- READ CHUNK -----------------------------------

void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
	
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

//---------------------------------- GET STRING -----------------------------------
//		This function reads in a string of characters
//---------------------------------- GET STRING -----------------------------------

int CLoad3DS::GetString(char *pBuffer)
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

void CLoad3DS::ReadColorChunk(t3DSMaterialInfo *pMaterial, tChunk *pChunk)
{
	ReadChunk(m_TempChunk);
	m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
	pChunk->bytesRead += m_TempChunk->bytesRead;
}


//---------------------------------- READ VERTEX INDECES ----------------------------------
//		This function reads in the indices for the vertex array
//---------------------------------- READ VERTEX INDECES -----------------------------------

void CLoad3DS::ReadVertexIndices(t3DSObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;					 
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);
	
	pObject->pFaces = new t3DSFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(t3DSFace) * pObject->numOfFaces);
	
	
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

void CLoad3DS::ReadUVCoordinates(t3DSObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);
	
	pObject->pTexVerts = new tVector2 [pObject->numTexVertex];
	
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


//---------------------------------- READ VERTICES -----------------------------------
//	This function reads in the vertices for the object
//---------------------------------- READ VERTICES -----------------------------------

void CLoad3DS::ReadVertices(t3DSObject *pObject, tChunk *pPreviousChunk)
{ 
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);
	
	pObject->pVerts = new tVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(tVector3) * pObject->numOfVerts);
	
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


//---------------------------------- READ OBJECT MATERIAL -----------------------------------
//	This function reads in the material name assigned to the object and sets the materialID
//---------------------------------- READ OBJECT MATERIAL -----------------------------------

void CLoad3DS::ReadObjectMaterial(t3DSModel *pModel, t3DSObject *pObject, tChunk *pPreviousChunk)
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


#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

tVector3 Vector(tVector3 vPoint1, tVector3 vPoint2)
{
	tVector3 vVector;							
	
	vVector.x = vPoint1.x - vPoint2.x;			
	vVector.y = vPoint1.y - vPoint2.y;			
	vVector.z = vPoint1.z - vPoint2.z;			
	
	return vVector;								
}

tVector3 AddVector(tVector3 vVector1, tVector3 vVector2)
{
	tVector3 vResult;							
	
	vResult.x = vVector2.x + vVector1.x;		
	vResult.y = vVector2.y + vVector1.y;		
	vResult.z = vVector2.z + vVector1.z;		
	
	return vResult;								
}

tVector3 DivideVectorByScaler(tVector3 vVector1, float Scaler)
{
	tVector3 vResult;							
	
	vResult.x = vVector1.x / Scaler;			
	vResult.y = vVector1.y / Scaler;			
	vResult.z = vVector1.z / Scaler;			
	
	return vResult;								
}

/*
// I have this function in my 3Dmath file
tVector3 Normalize(tVector3 vNormal)
{
	double Magnitude;							
	
	Magnitude = Mag(vNormal);					
	
	vNormal.x /= (float)Magnitude;				
	vNormal.y /= (float)Magnitude;				
	vNormal.z /= (float)Magnitude;				
	
	return vNormal;								
}

*/

//---------------------------------- COMPUTER NORMALS -----------------------------------
//		This function computes the normals and vertex normals of the objects
//---------------------------------- COMPUTER NORMALS -----------------------------------

void CLoad3DS::ComputeNormals(t3DSModel *pModel)
{
	tVector3 vVector1, vVector2, vNormal, vPoly[3];
	
	if(pModel->numOfObjects <= 0)
		return;
	
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		t3DSObject *pObject = &(pModel->pObject[index]);
		
		tVector3 *pNormals		= new tVector3 [pObject->numOfFaces];
		tVector3 *pTempNormals	= new tVector3 [pObject->numOfFaces];
		pObject->pNormals		= new tVector3 [pObject->numOfVerts];
		
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];
			
			
			vVector1 = vPoly[0] - vPoly[2];				
			vVector2 = vPoly[2] - vPoly[1];				
			
			vNormal  = Cross(vVector1, vVector2);		
			pTempNormals[i] = vNormal;					
			vNormal  = Normalize(vNormal);				
			
			pNormals[i] = vNormal;						
		}
		
		
		tVector3 vSum(0.0, 0.0, 0.0);
		tVector3 vZero = vSum;
		int shared=0;
		
		for (i = 0; i < pObject->numOfVerts; i++)			
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
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));
			
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	
			
			vSum = vZero;									 
			shared = 0;										
		}
		
		delete [] pTempNormals;
		delete [] pNormals;
	}
}


