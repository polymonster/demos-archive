#include "Terrain.h"
#include "Fonts.h"
#include "TimerSingleton.h"
#include "modelManager.h"
#include "textureSingleton.h"
#include "ShaderManager.h"

int undos = 0;
float average = 0;

int g_terrainSculptMode;

CTerrain::CTerrain()
{
	nameIdentifier = "terrain";

	m_width = 110;
	m_height = 110;
	m_tileSize = 20;

	create();

}

CTerrain::~CTerrain()
{
	delete [] pickingIndices;
	deleteVBO();
}

void CTerrain::create()
{
	int x = -m_width * m_tileSize / 2;
	int z = -m_height * m_tileSize / 2;
	int zstart = -m_height * m_tileSize / 2;

	vertexCount = m_width * m_height;
	pickingIndexCount = vertexCount * 4;
	indexCount = (m_width * m_height * 2) + (m_height * 2);

	
	vboVertices = new SVector3[vertexCount];
	vboNormals = new SVector3[vertexCount];
	vboTextureWeightsFirst = new STextureWeight[vertexCount];
	vboTextureWeightsSecond = new STextureWeight[vertexCount];
	vboTextureCoords = new SVector2<float>[vertexCount];
	vboIndices = new unsigned int[indexCount];
	
	pickingIndices = new unsigned int[pickingIndexCount];
	
	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				m_weightsFirst[i][j].channel[k] = 0;
				m_weightsSecond[i][j].channel[k] = 0;
			}

			m_weightsFirst[i][j].channel[0] = 1;

			//set the vertex data to the current x and z position
			m_vertex[i][j].x = x;
			m_vertex[i][j].y = 0;
			m_vertex[i][j].z = z; 

			m_pickingColor[i][j].r = i;
			m_pickingColor[i][j].g = j;
			m_pickingColor[i][j].b = 0;

			m_normal[i][j].x = 0;
			m_normal[i][j].y = 1;
			m_normal[i][j].z = 0;

			//increment z by tile size every row
			z+=m_tileSize;
		}

		//increment x by tile size every colum
		//and reset z to the start.
		z = zstart;
		x+=m_tileSize;
	}

	fillVBO();

	glGenBuffersARB(1, &vertexVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboVertices) * vertexCount * 3,vboVertices, GL_STREAM_DRAW_ARB);

    glGenBuffersARB(1, &normalVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboNormals) * vertexCount * 3, vboNormals, GL_STREAM_DRAW_ARB);

	glGenBuffersARB(1, &textureWeightFirstVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureWeightFirstVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboTextureWeightsFirst) * vertexCount * 4, vboTextureWeightsFirst, GL_STREAM_DRAW_ARB);

	glGenBuffersARB(1, &textureWeightSecondVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureWeightSecondVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboTextureWeightsSecond) * vertexCount * 4, vboTextureWeightsSecond, GL_STREAM_DRAW_ARB);

	glGenBuffersARB(1, &textureCoordVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureCoordVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vboTextureCoords) * vertexCount * 2, vboTextureCoords, GL_STREAM_DRAW_ARB);

	glGenBuffersARB(1, &pickingIBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, pickingIBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(pickingIndices) * pickingIndexCount, pickingIndices, GL_STREAM_DRAW_ARB);

	glGenBuffersARB(1, &IBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(vboIndices) * indexCount, vboIndices, GL_STREAM_DRAW_ARB);

	m_selectedPolygon.x = -1;
	m_selectedPolygon.y = -1;

	delete [] vboVertices;
	delete [] vboNormals;
	delete [] vboTextureWeightsFirst;
	delete [] vboTextureWeightsSecond;
	delete [] vboTextureCoords;
	delete [] vboIndices;

	m_brush.radius = 100;
	m_brush.cursorID = CTextureMan::getInstance()->load("data/interface/brush.tga",true);
	m_brush.strength = 0.1f;
	m_brush.falloff = 1.0f;

	for(int i = 0; i < 8; i++)
	{
		m_selectedTexture[i] = false;
	}

	setupToolboxes();

	m_texturePallet[1] = 0;
	m_texturePallet[2] = 0;
	m_texturePallet[3] = 0;
	m_texturePallet[4] = 0;
	m_texturePallet[5] = 0;
	m_texturePallet[6] = 0;
	m_texturePallet[7] = 0;

}

CTerrain::CTerrain(std::ifstream &fin)
{
	nameIdentifier = "terrain";

	fin >> m_width;
	fin >> m_height;
	fin >> m_tileSize;

	create();

	readIn(fin);
}

CTerrain::CTerrain(int width, int height, int tileSize)
{
	nameIdentifier = "terrain";

	m_width = width;
	m_height = height;
	m_tileSize = tileSize;

	create();
}
void CTerrain::fillVBO()
{
	int x = 0;
	int z = 0;

	int vertCounter = 0;

	for(int i = 0; i < m_width; i++)
	{
		for(int j = 0; j < m_height; j++)
		{
			vboVertices[vertCounter] = m_vertex[i][j];
			vboNormals[vertCounter] = m_normal[i][j];

			vboTextureWeightsFirst[vertCounter] = m_weightsFirst[i][j];
			vboTextureWeightsSecond[vertCounter] = m_weightsSecond[i][j];

			//Static Texture Coords
			vboTextureCoords[vertCounter].x = (float) i * 0.5;
			vboTextureCoords[vertCounter].y = (float) j * 0.5;

			vertCounter++;
		}
	}

	int indCount = 0;

	for(int i = 0; i < m_width - 1; i++)
	{
		for(int j = 0; j < m_height - 1; j++)
		{
			//index 1 is the first vertex to render
			//in a primitive
			int index1 = (i * m_width) + j;
			//index 2 is the vertex next to index 1
			int index2 = ((i + 1) * m_width) + j;

			//if we are at the start of a row we need to
			//render a degenerate polygon
			if(j == 0) vboIndices[indCount++] = index1;
				
			//add the first vertex of the primitive
			vboIndices[indCount++] = index1;
			//add the next vertex in the strip
			vboIndices[indCount++] = index2;
			
			if(j == m_width - 2)
			{
				//if we are at the end of a row
				//we need to render another vertex
				//to make a degenerate polygon
				vboIndices[indCount] = index2;
				indCount++;
			}
		}
	}

	indCount = 0;

	for(int i = 0; i < m_width - 1; i++)
	{
		for(int j = 0; j < m_height - 1; j++)
		{
			int index1 = (i * m_width) + j;
			int index2 = ((i + 1) * m_width) + j;

			pickingIndices[indCount] = index1;
			indCount++;
			pickingIndices[indCount] = index1 + 1;
			indCount++;
			pickingIndices[indCount] = index2 + 1;
			indCount++;
			pickingIndices[indCount] = index2;
			indCount++;
		}
	}

	

}

void CTerrain::clamp(STextureWeight *unit1, STextureWeight *unit2)
{
	float average = 0;
	
	for(int i = 0; i < 4; i++)
	{
		average += (float) unit1->channel[i] + unit2->channel[i];
	}
	
	for(int i = 0; i < 4; i++)
	{
		unit1->channel[i] /= average;
		unit2->channel[i] /= average;
	}
}

SVector3 CTerrain::calculateSmoothNormals(int x, int z)
{
	int rootX = x * m_tileSize;
	int rootZ = z * m_tileSize;

	/*
	e = i,heightMap[i][j],j the point to make a normal for

	a--b--c
	| \| /|
	d--e--f
	| /| \|
	g--h--i

	*/

	//store some variables to error handle the array if x < 0 or > size and same for z;

	int plusX = x + 1; 
	int plusZ = z + 1; 

	int minusX = x - 1; 
	int minusZ = z - 1; 

	if(plusX >= m_width) plusX = x;
	if(plusZ >= m_height) plusZ = z;

	if(minusX < 0) minusX = x;
	if(minusZ < 0) minusZ = z;

	SVector3 a(m_vertex[minusX][plusZ].x,m_vertex[minusX][plusZ].y,m_vertex[minusX][plusZ].z); //
	SVector3 b(m_vertex[x][plusZ].x,m_vertex[x][plusZ].y,m_vertex[x][plusZ].z); //
	SVector3 c(m_vertex[plusX][plusZ].x,m_vertex[plusX][plusZ].y,m_vertex[plusX][plusZ].z); //
	SVector3 d(m_vertex[minusX][z].x,m_vertex[minusX][z].y,m_vertex[minusX][z].z); //

	SVector3 e(m_vertex[x][z].x,m_vertex[x][z].y,m_vertex[x][z].z); //root--------------------------

	SVector3 f(m_vertex[plusX][z].x,m_vertex[plusX][z].y,m_vertex[plusX][z].z); //
	SVector3 g(m_vertex[minusX][minusZ].x,m_vertex[minusX][minusZ].y,m_vertex[minusX][minusZ].z); //
	SVector3 h(m_vertex[x][minusZ].x,m_vertex[x][minusZ].y,m_vertex[x][minusZ].z); //
	SVector3 i(m_vertex[plusX][minusZ].x,m_vertex[plusX][minusZ].y,m_vertex[plusX][minusZ].z); ; //

	//get all the surrounding normals and average them
	int numberOfNormals = 0;
	SVector3 normalArray[8];
	
	normalArray[numberOfNormals++] = cross((a - e),(b - e));
	normalArray[numberOfNormals++] = cross((b - e),(c - e));
	normalArray[numberOfNormals++] = cross((c - e),(f - e));
	normalArray[numberOfNormals++] = cross((f - e),(i - e));
	normalArray[numberOfNormals++] = cross((i - e),(h - e));
	normalArray[numberOfNormals++] = cross((h - e),(g - e));
	normalArray[numberOfNormals++] = cross((g - e),(d - e));
	normalArray[numberOfNormals++] = cross((d - e),(a - e));

	//try more smoothing
	SVector3 averageNormal(0,0,0);

	for(int i = 0; i < numberOfNormals; i ++)
	{
		averageNormal = averageNormal + normalArray[i];
	}

	averageNormal.x /= numberOfNormals;
	averageNormal.y /= numberOfNormals;
	averageNormal.z /= numberOfNormals;

	return averageNormal;
	//glNormal3f(averageNormal.x,averageNormal.y,averageNormal.z);
}

SVector3 CTerrain::calculateNormals(int i, int j)
{
	int plusX = i + 1;
	int plusZ = j + 1;

	if(plusX >= m_width) plusX = i;
	if(plusZ >= m_height) plusZ = j;

	SVector3 e(m_vertex[i][j].x,m_vertex[i][j].y,m_vertex[i][j].z); 
	SVector3 a(m_vertex[plusX][plusZ].x,m_vertex[plusX][plusZ].y,m_vertex[plusX][plusZ].z);
	SVector3 b(m_vertex[i][plusZ].x,m_vertex[i][plusZ].y,m_vertex[i][plusZ].z); 

	return cross((b - e),(a - e));
}

void CTerrain::backBufferRender()
{
	if((g_editMode == EDIT_TERRAIN) && (m_editMode != 0))
	{
		CTextureMan::getInstance()->disableAllUnits();
		glDisable(GL_LIGHTING);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);
		glVertexPointer(3, GL_FLOAT, 0, 0); 

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

		int index = 0;

		for(int i = 0; i < m_width - 1; i++)
		{
			for(int j = 0; j < m_height - 1; j++)
			{
				glColor3ub(m_pickingColor[i][j].r,m_pickingColor[i][j].g,m_pickingColor[i][j].b);

				glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, &pickingIndices[index]);

				index += 4;
			}
		}

		glDisableClientState( GL_VERTEX_ARRAY );

		colorByte pix = user::getInstance()->getPixelAtMouse();

		m_selectedPolygon.x = pix.r;
		m_selectedPolygon.y = pix.g;

		user::getInstance()->get3dMouseCoords();
	}
}

void CTerrain::drawBrush(int brushType)
{
	float trueRatio = float(m_brush.radius) / float(m_tileSize);
	m_brush.sqaureInfluence = (int) trueRatio;

	if(brushType == 1) m_brush.sqaureInfluence *= m_brush.falloff;

	SVector2<int> startChunk;
	SVector2<int> endChunk;
	SVector2<float> cursorTexCoords;
	SVector2<float> offset;
	float texCoordStep = 1.0f / (m_brush.sqaureInfluence * 2);

	glDisable(GL_LIGHTING);

	CTextureMan::getInstance()->disableAllUnits();

	CTextureMan::getInstance()->bindTexture(m_brush.cursorID,0);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);

	if(getEditChunks(&startChunk,&endChunk))
	{
		cursorTexCoords.x = 0;
		cursorTexCoords.y = 0;

		offset.x = g_input.mouse3dPos.x - m_vertex[m_selectedPolygon.x][m_selectedPolygon.y].x;
		offset.y = g_input.mouse3dPos.z - m_vertex[m_selectedPolygon.x][m_selectedPolygon.y].z;

		SVector2<float> size;
		size.x = (endChunk.x - startChunk.x) / 2;
		size.y = (endChunk.y - startChunk.y) / 2;

		int ystart = 0;

		if(size.x < m_brush.sqaureInfluence)
		{
			if(startChunk.x == 0)
			{
				int diff = m_brush.sqaureInfluence - size.x;
				cursorTexCoords.x = texCoordStep * (diff * 2);
			}

		}

		if(size.y < m_brush.sqaureInfluence)
		{
			if(startChunk.y == 0)
			{
				int diff = m_brush.sqaureInfluence - size.y;
				ystart = texCoordStep * (diff * 2); 
				cursorTexCoords.y = ystart; 
			}
		}

		for(int i = startChunk.x; i < endChunk.x; i++)
		{
			for(int j = startChunk.y; j < endChunk.y; j++)
			{
					glColor4f(1,1,1,1);
					if(brushType == 1) glColor4f(1,0,0,1);

					glBegin(GL_QUADS);

					SVector2<float> storeOffset = offset;
					if(i == 0 || i == m_width - 1) storeOffset.x = 0;
					if(j == 0 || j == m_height - 1) storeOffset.y = 0;


					float y = 0;
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB,cursorTexCoords.x,cursorTexCoords.y);
					y = m_vertex[i][j + 1].y - m_vertex[i][j].y;
					glVertex3f(m_vertex[i][j].x + storeOffset.x,m_vertex[i][j].y,m_vertex[i][j].z + storeOffset.y);

					glMultiTexCoord2fARB(GL_TEXTURE0_ARB,cursorTexCoords.x,cursorTexCoords.y + texCoordStep);
					y = m_vertex[i + 1][j + 1].y - m_vertex[i][j + 1].y;
					glVertex3f(m_vertex[i][j + 1].x + offset.x,m_vertex[i][j + 1].y,m_vertex[i][j + 1].z + offset.y);

					glMultiTexCoord2fARB(GL_TEXTURE0_ARB,cursorTexCoords.x + texCoordStep,cursorTexCoords.y + texCoordStep); 
					y = m_vertex[i + 1][j].y - m_vertex[i + 1][j + 1].y;
					glVertex3f(m_vertex[i + 1][j + 1].x + offset.x,m_vertex[i + 1][j + 1].y,m_vertex[i + 1][j + 1].z + offset.y);

					glMultiTexCoord2fARB(GL_TEXTURE0_ARB,cursorTexCoords.x + texCoordStep,cursorTexCoords.y); 
					y = m_vertex[i][j].y - m_vertex[i + 1][j].y;
					glVertex3f(m_vertex[i + 1][j].x + offset.x,m_vertex[i + 1][j].y,m_vertex[i + 1][j].z + offset.y);

					glEnd();
				
				cursorTexCoords.y += texCoordStep;
			}

			cursorTexCoords.x += texCoordStep;
			cursorTexCoords.y = ystart;
		}
	}

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void CTerrain::render()
{
	CShaderManager::getInstance()->attachShader(g_shaderId.terrain);
	glUniform4fARB(glGetUniformLocationARB(
		CShaderManager::getInstance()->getShaderProgram(g_shaderId.terrain), "lightPos"), 0, 100.0f, 0, 1.0f);

	for(int i = 1; i < 8; i++)
	{
		CTextureMan::getInstance()->bindTexture(m_texturePallet[i],i);
	}

	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet2",1);
	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet3",2);
	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet4",3);
	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet5",4);
	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet6",5);
	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet7",6);
	CShaderManager::getInstance()->setUniform(g_shaderId.terrain,"texture_pallet8",7);


	glEnable(GL_LIGHTING);

	renderVBO();

	user::getInstance()->terrainMouseCoords();

	std::ofstream fout;
	fout.open("trace.txt");
	fout << g_input.mouse3dPos.x << "\n";
	fout << g_input.mouse3dPos.y << "\n";
	fout << g_input.mouse3dPos.z << "\n";
	fout.close();

	g_input.mouse3dPos;

	CTextureMan::getInstance()->disableAllUnits();
}


void CTerrain::renderVBO()
{
	glEnable(GL_CULL_FACE);

	//enable vertex, normal, colour and texture arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	int loc = glGetAttribLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.terrain),"uTextureWeights2");
	glEnableVertexAttribArrayARB(loc);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureWeightSecondVBO);
	glVertexAttribPointerARB(loc,4,GL_FLOAT,0,0,0);

	//bind texture coordniate buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureCoordVBO);
	glTexCoordPointer(2, GL_FLOAT, 0, 0); 

	//bind colour buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureWeightFirstVBO);
	glColorPointer(4, GL_FLOAT, 0, 0);

	//bind normal buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);
	glNormalPointer(GL_FLOAT, 0, 0); 

	//bind vertex buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); 

	//bind index buffer
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//render elemets to the count of index count
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

	//unbind and disable the buffers
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );

	glActiveTextureARB(GL_TEXTURE1_ARB);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);

	CTextureMan::getInstance()->disableAllUnits();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableVertexAttribArrayARB(loc);
}

void CTerrain::renderWireframe()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor4f(0.8,0.8,0.8,1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); 

	glDrawArrays(GL_QUADS, 0, vertexCount);
	
	glDisableClientState( GL_VERTEX_ARRAY );

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
}

float CTerrain::insideRadius(float x, float z, int radius)
{
	//ignore the y component of the vertex, just check the x-z plane
	SVector3 vertex(x,0,z);

	SVector3 cursor = g_input.mouse3dPos;

	SVector3 center(0,0,0);

	if((cursor.x == 0) && (cursor.y == 0) && (cursor.z == 0)) 
	{
		return 0;
	}

	cursor.y = 0;

	//apply distance formula to the 2 vectors ovtaining distance (d)
	float d = distance(vertex,cursor);

	//if d is inisde the radius
	if(d < radius)
	{
		//calculate the strength of the brush at that vertex
		float st;

		st = radius - d;

		if(st < 0) st = 0;

		return st;
	}
	else return 0;
}
void CTerrain::addToUndoStack(unsigned int x, unsigned int y)
{
	bool enter = true;

	for(int i = 0; i < m_undoStack.heights.size(); i++)
	{
		if((m_undoStack.idx[i] == x) && (m_undoStack.idy[i] == y))
		{
			enter = false;
		}
	}

	if(enter)
	{
		m_undoStack.idx.push_back(x);
		m_undoStack.idy.push_back(y);
		m_undoStack.heights.push_back(m_vertex[x][y]);
	}
}

void CTerrain::createUndoLevel()
{
	g_undoStack.commandNameList.push_back("height");
	g_undoStack.terrainHeight.push_back(m_undoStack);

	m_undoStack.idx.clear();
	m_undoStack.idy.clear();
	m_undoStack.heights.clear();
}

void CTerrain::addToUndoStack2(unsigned int x, unsigned int y)
{
	bool enter = true;

	for(int i = 0; i < m_undoStackPaint.weights.size(); i++)
	{
		if((m_undoStackPaint.idx[i] == x) && (m_undoStackPaint.idy[i] == y))
		{
			enter = false;
		}
	}

	if(enter)
	{
		m_undoStackPaint.idx.push_back(x);
		m_undoStackPaint.idy.push_back(y);
		m_undoStackPaint.weights.push_back(m_textureWeight[x][y]);
	}

}
	
void CTerrain::createUndoLevel2()
{
	g_undoStack.commandNameList.push_back("paint");
	g_undoStack.terrainPaint.push_back(m_undoStackPaint);

	m_undoStackPaint.idx.clear();
	m_undoStackPaint.idy.clear();
	m_undoStackPaint.weights.clear();

}


void CTerrain::height()
{
	//renderWireframe();
	if((g_input.leftMouse) || (g_input.rightMouse))
	{
		int direction = 1;
		if(g_input.rightMouse)direction = -1;
		
		float pressure = 1;

		#ifdef _USE_TAB

			pressure = g_input.tabletPacket.pkNormalPressure;
			pressure *= CTimer::getInstance()->animBaseSpeed();

		#endif

		
		int x = 0;
		int z = 0;

		SVector2<int> startChunk;
		SVector2<int> endChunk;

		if(getEditChunks(&startChunk,&endChunk))
		{
			for(int i = startChunk.x; i <= endChunk.x; i++)
			{
				for(int j = startChunk.y; j <= endChunk.y; j++)
				{
					if(!m_undoStack.modified)
					{
						m_brush.average = g_input.mouse3dPos.y;
						m_undoStack.modified = true;
					}

					float ratio = insideRadius(m_vertex[i][j].x,m_vertex[i][j].z,m_brush.radius);
					float ratio2 = insideRadius(m_vertex[i][j].x,m_vertex[i][j].z,m_brush.radius * m_brush.falloff);

					switch(g_terrainSculptMode)
					{
						case FLATTEN:
						{
							if(ratio2 != 0)
							{
								m_vertex[i][j].y = (m_brush.average);
							}
							else
							{
								if(m_vertex[i][j].y != (m_brush.average))
								{
									float current_y = m_vertex[i][j].y;
									float target_y = (m_brush.average) * direction;
									float diff = target_y - current_y;
								
									float ammount = (ratio / m_brush.radius) * m_brush.falloff;
									
									if(ammount > 1) ammount = 1;
									if(ammount < 0) ammount = 0;

									m_vertex[i][j].y += diff * ammount;
								}
							}
						}
						break;

						case SCULPT:
						{
							if(ratio2 != 0)
							{
								float smoothen = 1.0f - ((ratio) / m_brush.radius);
								float old_pos = m_vertex[i][j].y;
								

								m_vertex[i][j].y += ratio * m_brush.strength * pressure * direction;
								
								float diff = m_vertex[i][j].y - old_pos;

								m_vertex[i][j].y += (diff * smoothen) * 0.5f;
							}
							else
							{
								m_vertex[i][j].y += ratio * m_brush.strength * pressure * direction;
							}

						}
						break;

						case NOISE:
						{
							if(ratio != 0)
							{
								m_vertex[i][j].y += (float(rand()%100 / 10.0f)) * m_brush.strength * pressure * direction;;
							}
						}

						break;


					}
				}
			}

			updateVBO();
		}
	}
	else
	{
		if(m_undoStack.modified)
		{
			//createUndoLevel();
			//m_undoStack.modified = false;
			m_undoStack.modified = false;
		}
	}
}

void CTerrain::command(commandList *commandStack, commandList *recieveStack)
{
	std::string name = commandStack->commandNameList.back();

		if(name == "height")
		{
			terrainHeightUndo test;

			for(int i = 0; i < commandStack->terrainHeight.back().heights.size(); i++)
			{
				int x = commandStack->terrainHeight.back().idx[i];
				int y = commandStack->terrainHeight.back().idy[i];

				test.idx.push_back(x);
				test.idy.push_back(y);
				test.heights.push_back(m_vertex[x][y]);

				m_vertex[x][y] = commandStack->terrainHeight.back().heights[i];
			}

			recieveStack->commandNameList.push_back("height");
			recieveStack->terrainHeight.push_back(test);

			if(commandStack->terrainHeight.size() > 1) 
			{
				commandStack->terrainHeight.pop_back();
				if(commandStack->commandNameList.size() > 1) commandStack->commandNameList.pop_back();
			}

			updateVBO();
		}
		else if(name == "paint")
		{
			
			for(int i = 0; i < commandStack->terrainPaint.back().weights.size(); i++)
			{
				int x = commandStack->terrainPaint.back().idx[i];
				int y = commandStack->terrainPaint.back().idy[i];

				m_textureWeight[x][y] = commandStack->terrainPaint.back().weights[i];
			}

			if(commandStack->terrainPaint.size() > 1) 
			{
				commandStack->terrainPaint.pop_back();
				if(commandStack->commandNameList.size() > 1) commandStack->commandNameList.pop_back();
			}

			updateTextureWeights();
			
		}
}

void CTerrain::updateVBO()
{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);

		SVector3 *ptr = (SVector3*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

		if(ptr)
		{
			for(int i = 0; i < m_width; i++)
			{
				for(int j = 0; j < m_height; j++)
				{
					*ptr++ = m_vertex[i][j];
				}
			}
		}

		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);

		SVector3 *normalPtr = (SVector3*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

		if(normalPtr)
		{
			for(int i = 0; i < m_width; i++)
			{
				for(int j = 0; j < m_height; j++)
				{
					*normalPtr++ = calculateNormals(i,j);
				}
			}
		}

		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}

void CTerrain::smoothen()
{
	//bind the normal buffer
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);

	SVector3 *normalPtr = (SVector3*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	if(normalPtr)
	{
		for(int i = 0; i < m_width; i++)
		{
			for(int j = 0; j < m_height; j++)
			{
				*normalPtr++ = calculateSmoothNormals(i,j);
			}
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}
void CTerrain::paint()
{
	if((g_input.leftMouse) || (g_input.rightMouse))
	{
		int texture = 0;
		if(g_input.rightMouse)texture = 1;

		int x = 0;
		int z = 0;

		SVector2<int> startChunk;
		SVector2<int> endChunk;

		float pressure = 1;

		#ifdef _USE_TAB

			pressure = g_input.tabletPacket.pkNormalPressure;
			
		#endif

		pressure *= 0.0001f;


		if(getEditChunks(&startChunk,&endChunk))
		{
			for(int i = startChunk.x; i <= endChunk.x; i++)
			{
				for(int j = startChunk.y; j <= endChunk.y; j++)
				{
					m_undoStackPaint.modified = true;

					addToUndoStack2(i,j);
					
					for(int k = 0; k < 4; k++)
					{
						float ratio = insideRadius(m_vertex[i][j].x,m_vertex[i][j].z,m_brush.radius);
						m_weightsFirst[i][j].channel[k] += (float) ratio * m_brush.strength * pressure * m_selectedTexture[k];
						m_weightsSecond[i][j].channel[k] += (float) ratio * m_brush.strength * pressure * m_selectedTexture[k + 4];
					}
					
					clamp(&m_weightsFirst[i][j],&m_weightsSecond[i][j]);
				}
			}

			updateTextureWeights();
		}
	}
	else
	{
			if(m_undoStackPaint.modified)
			{
				createUndoLevel2();
				m_undoStackPaint.modified = false;
			}
	}
}

void CTerrain::updateTextureWeights()
{
	//first channel 1 - 4 
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureWeightFirstVBO);

	STextureWeight *ptr = (STextureWeight*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	if(ptr)
	{
		for(int i = 0; i < m_width; i++)
		{
			for(int j = 0; j < m_height; j++)
			{
				*ptr++ = m_weightsFirst[i][j];
			}
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	//second channel 4 - 8 
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textureWeightSecondVBO);

	STextureWeight *ptr2 = (STextureWeight*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);

	if(ptr)
	{
		for(int i = 0; i < m_width; i++)
		{
			for(int j = 0; j < m_height; j++)
			{
				*ptr2++ = m_weightsSecond[i][j];
			}
		}
	}

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}

bool CTerrain::getEditChunks(SVector2<int> *startChunk,SVector2<int> *endChunk)
{
	if((m_selectedPolygon.x > m_width) || (m_selectedPolygon.x < 0) ||
		(m_selectedPolygon.y > m_height) || (m_selectedPolygon.y < 0))
	{
		return false;
	}

	startChunk->x = m_selectedPolygon.x - m_brush.sqaureInfluence;
	startChunk->y = m_selectedPolygon.y - m_brush.sqaureInfluence;

	endChunk->x = m_selectedPolygon.x + m_brush.sqaureInfluence;
	endChunk->y = m_selectedPolygon.y + m_brush.sqaureInfluence;

	if(startChunk->x < 0) startChunk->x = 0;
	if(startChunk->y < 0) startChunk->y = 0;

	if(endChunk->x >= m_width) endChunk->x = m_width - 1;
	if(endChunk->y >= m_height) endChunk->y = m_height - 1;

	return true;
}

void CTerrain::cleanUp()
{
	deleteVBO();
}

void CTerrain::deleteVBO()
{
	glDeleteBuffersARB(1, &vertexVBO);
	glDeleteBuffersARB(1, &normalVBO);
	glDeleteBuffersARB(1, &textureWeightFirstVBO);
	glDeleteBuffersARB(1, &textureWeightSecondVBO);
	glDeleteBuffersARB(1, &textureCoordVBO);
	glDeleteBuffersARB(1, &pickingIBO);
	glDeleteBuffersARB(1, &IBO);
	
}

void CTerrain::setActiveTextureLayer(int id)
{
	m_selectedTextureId = id + 1;

	for(int i = 1; i < 8; i++) m_selectedTexture[i] = false;

	m_selectedTexture[id] = true;
}


