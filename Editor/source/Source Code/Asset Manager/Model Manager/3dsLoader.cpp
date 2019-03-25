#include "input.h"
#include "textureSingleton.h"
#include "3dsloader.h"

C3dsLoader::C3dsLoader(t3DModel model,char* filename)
{
	m3DModel = model;

	//get the root for the texture directory
	char root[256];
	
	//clamp the name to start with "data\"
	int len= strlen(filename) - 1;
	char buf = filename[len];

	int count = 0;

	while(buf != '\\' && buf != '/')
	{
		count++;
		buf = filename[len - count];
	}
	
	int second = 0;

	for(int i = 0; i < len - count; i++)
	{
		root[i] = filename[i];
		second++;
	}

	root[second] = '\0';

	//load textures
	for(int i = 0; i < m3DModel.numOfMaterials; i++)				
	{
		if(strlen(m3DModel.pMaterials[i].strFile) > 0)				
		{
			//obtain the diffuse texture
			char texFile[256];
			strcpy(texFile,root);
			strcat(texFile,"\\texture\\");
			strcat(texFile,m3DModel.pMaterials[i].strFile);

			int len = strlen(texFile);
			len--;
			texFile[len--] = 'a';
			texFile[len--] = 'g';
			texFile[len--] = 't';

			textureArrayManager[i] = CTextureMan::getInstance()->load(texFile,true);
			
			//concatenate "N" to the file to check for normal maps of the diffuse version
			len = strlen(texFile);
			len--;
			texFile[len--] = 0;
			texFile[len--] = 0;
			texFile[len--] = 0;
			texFile[len--] = 0;

			strcat(texFile,"n.tga");

			normalMapArrayManager[i] = CTextureMan::getInstance()->load(texFile,false);
		}						
	}

	//polycounter
	m_polyCount = 0;

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DObject *pObject = &m3DModel.pObject[i];

		m_polyCount += pObject->numOfFaces;
	}

	//store all the data into vertex bufer objects
	initVBO();
	calculateTangentArray();

	//delete the temporary storage used to pass in vertex buffers
	delete [] vboVertices;
	delete [] vboNormals;
	delete [] vboTextureCoords;
	delete [] vboTangents;
}

void C3dsLoader::calculateTangentArray()
{
    SVector3 *tan1 = new SVector3[vertexCount];
    SVector3 *tan2 = new SVector3[vertexCount];

	for (long i = 0; i < vertexCount; i+=3)
    {
		long i1 = i;
        long i2 = i + 1;
        long i3 = i + 2;
        
        const SVector3& v1 = vboVertices[i1];
        const SVector3& v2 = vboVertices[i2];
        const SVector3& v3 = vboVertices[i3];
        
        const SVector2<float>& w1 = vboTextureCoords[i1];
        const SVector2<float>& w2 = vboTextureCoords[i2];
        const SVector2<float>& w3 = vboTextureCoords[i3];
        
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        SVector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);

        SVector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);
        
        tan1[i1] = sdir;
        tan1[i2] = sdir;
        tan1[i3] = sdir;
        
        tan2[i1] = tdir;
        tan2[i2] = tdir;
        tan2[i3] = tdir;
	}

	for (long a = 0; a < vertexCount; a++)
    {
		//vboNormals[a] = vboNormals[a] * -1;

        SVector3 n = vboNormals[a];
        SVector3 t = tan1[a];
        
        // Gram-Schmidt orthogonalize
        vboTangents[a] = normalize((t - n * dot(n, t)));

        // Calculate handedness
        float hand = (dot(cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    }
    
    delete[] tan1;

}

void C3dsLoader::drawVBO()
{
	//enable the vertex element arrays required
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//change texture coordinates to channel 1 (channel 0 is reserved for shadow maps)
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	int index = 0;
	int counter = 0;

	//loop through each model which may consist of many sub meshes
	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		//get a pointer to the current mesh
		t3DObject *pObject = &m3DModel.pObject[i];	

		//if the objects textured, texture it
		if(pObject->bHasTexture)
		{
			//notify the shader that it is textured
			CShaderManager::getInstance()->setUniform(g_shaderId.normalMap,"textureMapEnabled",1);

			//bind the texture this object needs
			CTextureMan::getInstance()->bindTexture(textureArrayManager[i],1);
			
			//bind texture coordinate buffer
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, texID);
			glTexCoordPointer(2, GL_FLOAT, 0, 0); 

			if(normalMapArrayManager[i] != -1)
			{
				//notify the shader that it is textured
				CShaderManager::getInstance()->setUniform(g_shaderId.normalMap,"normalMapEnabled",1);
				CTextureMan::getInstance()->bindTexture(normalMapArrayManager[i],2);
			}
			else
			{
				//if theres no normal map, notify shader
				glUniform1iARB(glGetUniformLocationARB(
				CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), 
				"normalMapEnabled"), 0);

				glActiveTextureARB(GL_TEXTURE2_ARB);	
				glDisable(GL_TEXTURE_2D);
			}
		}
		else
		{

			//if theres no texture then notify the shader there is no normal map or diffuse texture
			glUniform1iARB(glGetUniformLocationARB(
			CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), 
			"textureMapEnabled"), 0);

			glUniform1iARB(glGetUniformLocationARB(
			CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), 
			"normalMapEnabled"), 0);

			glActiveTextureARB(GL_TEXTURE1_ARB);
			glDisable(GL_TEXTURE_2D);
		}

		//calculate the range within the vertex array this sub object uses
		int currentArrayPos = (pObject->numOfFaces * 3);

		//bind the buffers (VBO's)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalID);
		glNormalPointer(GL_FLOAT, 0, 0); 

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboID);
		glVertexPointer(3, GL_FLOAT, 0, 0); 

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colID);
		glColorPointer(3, GL_FLOAT, 0, 0);

		//draw vertices in the VBO from index to currentArrayPos
		glDrawArrays(GL_TRIANGLES, index, currentArrayPos);

		//increment tracking variables
		index += currentArrayPos;
		counter++;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}



void C3dsLoader::renderPicking(colorByte color)
{
	glDisable(GL_LIGHTING);

	glEnableClientState(GL_VERTEX_ARRAY);

	int index = 0;

	glColor4ub(color.r,color.g,color.b,255);

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		t3DObject *pObject = &m3DModel.pObject[i];	

		int currentArrayPos = (pObject->numOfFaces * 3);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalID);
		glNormalPointer(GL_FLOAT, 0, 0); 

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboID);
		glVertexPointer(3, GL_FLOAT, 0, 0); 

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colID);
		glColorPointer(3, GL_FLOAT, 0, 0);

		glDrawArrays(GL_TRIANGLES, index, currentArrayPos);

		index += currentArrayPos;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glDisableClientState( GL_VERTEX_ARRAY );
}


void C3dsLoader::deleteVBO()
{
	glDeleteBuffersARB(1, &vboID);
	glDeleteBuffersARB(1, &texID);
	glDeleteBuffersARB(1, &normalID);
	glDeleteBuffersARB(1, &colID);
}

void C3dsLoader::calculateNormals()
{
	SVector3 tri[3];
	SVector3 vector1, vector2;

	for(int i = 0; i < vertexCount; i+=3)
	{
		tri[0] = vboVertices[i];
		tri[1] = vboVertices[i + 1];
		tri[2] = vboVertices[i + 2];

		vector1 = tri[0] - tri[1];
		vector2 = tri[1] - tri[2];

		vboNormals[i] = normalize(cross(vector1,vector2));
		vboNormals[i + 1] = normalize(cross(vector1,vector2));
		vboNormals[i + 2] = normalize(cross(vector1,vector2));
	}

}

void C3dsLoader::drawTangents()
{
	glColor3f(1,0,1);

	for(int i = 0; i < vertexCount; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(vboVertices[i].x,vboVertices[i].y,vboVertices[i].z);
		glVertex3f(vboVertices[i].x + (vboTangents[i].x),vboVertices[i].y + (vboTangents[i].y),vboVertices[i].z + (vboTangents[i].z));
		glEnd();
	}

}


void C3dsLoader::drawNormals()
{
	glColor3f(0,1,0);

	for(int i = 0; i < vertexCount; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(vboVertices[i].x,vboVertices[i].y,vboVertices[i].z);
		glVertex3f(vboVertices[i].x + (vboNormals[i].x),vboVertices[i].y + (vboNormals[i].y),vboVertices[i].z + (vboNormals[i].z));
		glEnd();
	}
}

void C3dsLoader::initVBO()
{
	int i;
	vertexCount = 0;

    for(i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;	

	    t3DObject *pObject = &m3DModel.pObject[i];

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				vertexCount++;
			}
		}
	}

	vboVertices = new SVector3[vertexCount];
	vboNormals = new SVector3[vertexCount];
	vboTextureCoords = new SVector2<float>[vertexCount];
	vboTangents = new SVector3[vertexCount];

	vertexCount = 0;

	int face;

	SVector3 extentMax;
	SVector3 extentMin;

    for(i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;	

	    t3DObject *pObject = &m3DModel.pObject[i];

		face = pObject->numOfFaces;

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				vboVertices[vertexCount].x = pObject->pVerts[ index ].x;

				//flip the z and y coords because of Max's Z-UP coordinate system;
				vboVertices[vertexCount].y = (pObject->pVerts[ index ].z);
				vboVertices[vertexCount].z = -pObject->pVerts[ index ].y;

				vboNormals[vertexCount] = pObject->pNormals[index];

				//get the extents of the model so we can construct a bounding box
				if(vertexCount == 0) 
				{
					extentMax = vboVertices[vertexCount];
					extentMin = vboVertices[vertexCount];
				}
				else
				{
					//min
					if(vboVertices[vertexCount].x < extentMin.x) extentMin.x = vboVertices[vertexCount].x;
					if(vboVertices[vertexCount].y < extentMin.y) extentMin.y = vboVertices[vertexCount].y;
					if(vboVertices[vertexCount].z < extentMin.z) extentMin.z = vboVertices[vertexCount].z;

					//max
					if(vboVertices[vertexCount].x > extentMax.x) extentMax.x = vboVertices[vertexCount].x;
					if(vboVertices[vertexCount].y > extentMax.y) extentMax.y = vboVertices[vertexCount].y;
					if(vboVertices[vertexCount].z > extentMax.z) extentMax.z = vboVertices[vertexCount].z;
				}
				
				vboTextureCoords[vertexCount] = pObject->pTexVerts[ index ];
				vboTextureCoords[vertexCount].y = vboTextureCoords[vertexCount].y;
				vboNormals[vertexCount] = pObject->pNormals[ index ];

				vertexCount++;
				
			}
		}
	}

	m_boundingBoxDimensions = (extentMax - extentMin) * 0.5f;
	m_boundingBoxPos = extentMin + m_boundingBoxDimensions;
	
	glGenBuffersARB(1, &vboID);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboID);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboVertices) * vertexCount * 3, vboVertices, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1, &texID);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, texID);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboTextureCoords) * vertexCount * 2, vboTextureCoords, GL_STATIC_DRAW_ARB);

	//calculateNormals();
	calculateTangentArray();

	glGenBuffersARB(1, &normalID);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalID);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboNormals) * vertexCount * 3, vboNormals, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &colID);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, colID);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboTangents) * vertexCount * 3, vboTangents, GL_STATIC_DRAW_ARB);

	m_polyCount = vertexCount / 3;
}

void C3dsLoader::drawBB()
{
	glColor4f(1,0,1,0.5f);

	glPushMatrix();
	quickTranslate(m_boundingBoxPos);
	quickScale(m_boundingBoxDimensions);
	cube();
	glPopMatrix();
}

void C3dsLoader::translateToVert(int i){

	t3DObject *pObject = &m3DModel.pObject[0];

	if((i < pObject->numOfFaces) && (i >= 0) )
	{
		int index = pObject->pFaces[0].vertIndex[i];
		glTranslated(pObject->pVerts[ index ].x,pObject->pVerts[ index ].y,pObject->pVerts[ index ].z);
	}

}

int C3dsLoader::returnClosestVert(SVector3 position)
{
	float closestDistance = 10000;
	int closestFace = 0;
	SVector3 closestVert(10,10,10);
	SVector3 s(1,1,1);
	position = rotateAndScale(position,s,-90);

	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DObject *pObject = &m3DModel.pObject[i];

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			int index = pObject->pFaces[j].vertIndex[0];

			SVector3 newVector(pObject->pVerts[ index ].x,pObject->pVerts[ index ].y,pObject->pVerts[ index ].z);

			float d = distance(position,newVector);

			d = absolute(d);

			if(d < closestDistance) 
			{
				closestDistance = d;
				closestFace = j;
			}
		}
	}

	return closestFace;
}

void C3dsLoader::getVerts(int face,SVector3 &p1, SVector3 &p2, SVector3 &p3, SVector3 &p4)
{
	t3DObject *pObject = &m3DModel.pObject[0];

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


void C3dsLoader::Unload_3ds()
{
	//clear geometry data
	for(int i = 0; i < m3DModel.numOfObjects; i++)	
	{	
		delete [] m3DModel.pObject[i].pFaces;
		delete [] m3DModel.pObject[i].pNormals;
		delete [] m3DModel.pObject[i].pVerts;
		delete [] m3DModel.pObject[i].pTexVerts;
	}

	m3DModel.numOfMaterials = 0;
	m3DModel.numOfObjects = 0;

	deleteVBO();
}

int C3dsLoader::returnNumberFaces(){

	t3DObject *pObject = &m3DModel.pObject[0];
	return pObject->numOfFaces;

}

void C3dsLoader::getBox(SVector3 *pos,SVector3 *scale)
{
	*pos = m_boundingBoxPos;
	*scale = m_boundingBoxDimensions;
}












