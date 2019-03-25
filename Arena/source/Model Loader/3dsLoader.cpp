#include"3dsloader.h"
#include"textureSingleton.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//										 INIT 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Init_3ds(char* filename)
{
	mLoad3ds.Import3DS(&m3DModel, filename);	
	
	for(int i = 0; i < m3DModel.numOfMaterials; i++)				
	{
		if(strlen(m3DModel.pMaterials[i].strFile) > 0)				
		{
			char filePath[256];

			strcpy(filePath,"data/texture/");
			strcat(filePath,m3DModel.pMaterials[i].strFile);
		
			textureArrayManager[i] = CTextureMan::getInstance()->add(filePath);
		}
		
		m3DModel.pMaterials[i].texureId = i;						
	}

	//polycounter
	m_polyCount = 0;

	for(i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DSObject *pObject = &m3DModel.pObject[i];

		m_polyCount += pObject->numOfFaces;
	}


}

void C3dsLoader::translateToVert(int i){

	t3DSObject *pObject = &m3DModel.pObject[0];

	if((i < pObject->numOfFaces) && (i >= 0) )
	{
		int index = pObject->pFaces[0].vertIndex[i];
		glTranslated(pObject->pVerts[ index ].x,pObject->pVerts[ index ].y,pObject->pVerts[ index ].z);
	}

}

void C3dsLoader::renderMappedQuad(){

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		t3DSObject *pObject = &m3DModel.pObject[i];

		glEnable(GL_TEXTURE_2D);		

		//glBindTexture(GL_TEXTURE_2D, TextureArray3ds[pObject->materialID]); 

		//cout << textureArrayManager[pObject->materialID];
		
		//CTextureMan::getInstance()->bindMeshTexture(textureArrayManager[pObject->materialID]);

	for(int j = 0; j < pObject->numOfFaces; j++)
	{
		glPushMatrix();

		glRotated(-90,1,0,0);
		int index = pObject->pFaces[j].vertIndex[0];
		glTranslated(pObject->pVerts[ index ].x,pObject->pVerts[ index ].y,pObject->pVerts[ index ].z);
		glRotated(90,1,0,0);
			
		billboardSphericalBegin();

			glDepthMask(GL_FALSE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_BLEND);

			glTranslated(0,0,500);

				glBegin(GL_QUADS);
					glTexCoord2f(0,0); glVertex2f(-1000,-1000);
					glTexCoord2f(0,1); glVertex2f(-1000,1000);
					glTexCoord2f(1,1); glVertex2f(1000,1000);
					glTexCoord2f(1,0); glVertex2f(1000,-1000);
				glEnd();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);

		billboardEnd();

		glPopMatrix();
	}

		glDisable(GL_TEXTURE_2D);

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										RENDER 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Render_3ds()
{
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DSObject *pObject = &m3DModel.pObject[i];	
		
		if(pObject->bHasTexture)									
		{		
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);

			CTextureMan::getInstance()->bindTexture(textureArrayManager[pObject->materialID]);
		} 
		else 
		{
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);								
		}
		
		glBegin(GL_TRIANGLES);

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				if(pObject->bHasTexture) {
					
					if(pObject->pTexVerts) {
						glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {
					
					if(m3DModel.pMaterials.size() < pObject->materialID) 
					{
						BYTE *pColor = m3DModel.pMaterials[pObject->materialID].color;
						
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}

				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}
		
		glEnd();
	}

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	
}

void C3dsLoader::RenderNoTexture()
{
	//glDisable(GL_TEXTURE_2D);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DSObject *pObject = &m3DModel.pObject[i];	
		
		glBegin(GL_TRIANGLES);

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}
		
		glEnd();
	}	
}

int C3dsLoader::returnClosestVert(SVector3 position)
{
	float closestDistance = 10000;
	int closest3DSFace = 0;
	SVector3 closestVert(10,10,10);
	SVector3 s(1,1,1);
	position = rotateAndScale(position,s,-90);

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DSObject *pObject = &m3DModel.pObject[i];

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			int index = pObject->pFaces[j].vertIndex[0];

			SVector3 newVector(pObject->pVerts[ index ].x,pObject->pVerts[ index ].y,pObject->pVerts[ index ].z);

			float d = distance(position,newVector);

			d = absolute(d);

			if(d < closestDistance) 
			{
				closestDistance = d;
				closest3DSFace = j;
			}
		}
	}

	return closest3DSFace;
}

void C3dsLoader::getVerts(int face,SVector3 &p1, SVector3 &p2, SVector3 &p3, SVector3 &p4)
{
	t3DSObject *pObject = &m3DModel.pObject[0];

	if((face < pObject->numOfFaces) && (face >= 0) )
	{
		int index = pObject->pFaces[face].vertIndex[0];
		p1.x = pObject->pVerts[ index ].x;
		p1.y = pObject->pVerts[ index ].y;
		p1.z = pObject->pVerts[ index ].z;

		index = pObject->pFaces[face].vertIndex[1];
		p2.x = pObject->pVerts[ index ].x;
		p2.y = pObject->pVerts[ index ].y;
		p2.z = pObject->pVerts[ index ].z;

		index = pObject->pFaces[face].vertIndex[2];
		p3.x = pObject->pVerts[ index ].x;
		p3.y = pObject->pVerts[ index ].y;
		p3.z = pObject->pVerts[ index ].z;

		index = pObject->pFaces[face].vertIndex[4];
		p4.x = pObject->pVerts[ index ].x;
		p4.y = pObject->pVerts[ index ].y;
		p4.z = pObject->pVerts[ index ].z;
	}
}

SVector3 C3dsLoader::getModelsNormal(int face)
{
	t3DSObject *pObject = &m3DModel.pObject[0];

	if((face < pObject->numOfFaces) && (face >= 0) )
	{
		SVector3 normal;

		int index = pObject->pFaces[face].vertIndex[0];
		normal.x = pObject->pNormals[ index ].x;
		normal.y = pObject->pNormals[ index ].y;
		normal.z = pObject->pNormals[ index ].z;

		return normal;
	}

	SVector3 normal(0,0,0);
	return normal;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										UNLOAD 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Unload_3ds()
{
	//clean up textures
	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{		
		t3DSObject *pObject = &m3DModel.pObject[i];	
		if(pObject->bHasTexture)
		{
			CTextureMan::getInstance()->destroyTexture(textureArrayManager[pObject->materialID]);
		}
	}

	//clear geometry data
	for(i = 0; i < m3DModel.numOfObjects; i++)	
	{
		delete [] m3DModel.pObject[i].pFaces;
		delete [] m3DModel.pObject[i].pNormals;
		delete [] m3DModel.pObject[i].pVerts;
		delete [] m3DModel.pObject[i].pTexVerts;
	}

	m3DModel.numOfMaterials = 0;
	m3DModel.numOfObjects = 0;

}

int C3dsLoader::returnNumberFaces(){

	t3DSObject *pObject = &m3DModel.pObject[0];
	return pObject->numOfFaces;

}

SVector3 getModelsNormal();












