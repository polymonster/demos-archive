#include "Helpers.h"
#include "Input.h"
#include "Fonts.h"

SVector3 colors[10] = 
{
	SVector3(1,0,0),
	SVector3(0,1,0),
	SVector3(0,0,1),
	SVector3(1,1,0),
	SVector3(1,0,1),
	SVector3(0,1,1),
	SVector3(0.5,0,1),
	SVector3(0.5,0.8,0),
	SVector3(1,1,1),
	SVector3(0.2,1,0.3),
};

SVector3 cubeVertices[24] = 
{
	//FRONT
	SVector3(-1,-1,1),
	SVector3(1,-1,1),
	SVector3(1,1,1), 
	SVector3(-1,1,1), 

	//LEFT
	SVector3(-1,-1,1),
	SVector3(-1,1,1), 
	SVector3(-1,1,-1),
	SVector3(-1,-1,-1),
		
	//BACK
	SVector3(-1,-1,-1),
	SVector3(-1,1,-1),
	SVector3(1,1,-1),
	SVector3(1,-1,-1),

	//RIGHT
	SVector3(1,-1,-1),
	SVector3(1,1,-1),
	SVector3(1,1,1),
	SVector3(1,-1,1),

	//TOP
	SVector3(1,1,-1),
	SVector3(-1,1,-1),
	SVector3(-1,1,1),
	SVector3(1,1,1), 

	//BOTTOM
	SVector3(-1,-1,-1),
	SVector3(1,-1,-1),
	SVector3(1,-1,1),
	SVector3(-1,-1,1),
};

SVector3 cubeTangents[24] = 
{
	SVector3(1,0,0),
	SVector3(1,0,0),
	SVector3(1,0,0),
	SVector3(1,0,0),

	SVector3(0,0,1),
	SVector3(0,0,1),
	SVector3(0,0,1),
	SVector3(0,0,1),

	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),

	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),

	SVector3(0,0,1),
	SVector3(0,0,1),
	SVector3(0,0,1),
	SVector3(0,0,1),

	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),
};

SVector2<float> cubeTexCoords[24] =
{
	SVector2<float>(0,0),
	SVector2<float>(1,0),
	SVector2<float>(1,1),
	SVector2<float>(0,1),

	SVector2<float>(0,0),
	SVector2<float>(1,0),
	SVector2<float>(1,1),
	SVector2<float>(0,1),

	SVector2<float>(0,0),
	SVector2<float>(1,0),
	SVector2<float>(1,1),
	SVector2<float>(0,1),

	SVector2<float>(0,0),
	SVector2<float>(1,0),
	SVector2<float>(1,1),
	SVector2<float>(0,1),

	SVector2<float>(0,0),
	SVector2<float>(1,0),
	SVector2<float>(1,1),
	SVector2<float>(0,1),

	SVector2<float>(0,0),
	SVector2<float>(1,0),
	SVector2<float>(1,1),
	SVector2<float>(0,1),
	
};

SVector3 cubeNormals[24] = 
{
	SVector3(0,0,1),
	SVector3(0,0,1),
	SVector3(0,0,1),
	SVector3(0,0,1),

	SVector3(-1,0,0),
	SVector3(-1,0,0),
	SVector3(-1,0,0),
	SVector3(-1,0,0),

	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),
	SVector3(0,0,-1),

	SVector3(1,0,0),
	SVector3(1,0,0),
	SVector3(1,0,0),
	SVector3(1,0,0),

	SVector3(0,1,0),
	SVector3(0,1,0),
	SVector3(0,1,0),
	SVector3(0,1,0),

	SVector3(0,-1,0),
	SVector3(0,-1,0),
	SVector3(0,-1,0),
	SVector3(0,-1,0),
};

unsigned int cubeIndices[24] =
{	
	0,3,2,1,
	0,1,5,4,
	4,5,6,7,
	7,6,2,3,
	6,5,1,2,
	4,7,3,0,
};

unsigned int cubeVertexVBO;
unsigned int cubeNormalVBO;
unsigned int cubeIBO;
unsigned int cubeTextureVBO;
unsigned int tangentsVBO;

void createCubeVBO()
{
	
	glGenBuffersARB(1, &cubeVertexVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeVertexVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeVertices) * 3,cubeVertices, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &cubeNormalVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeNormalVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeNormals) * 3,cubeNormals, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &tangentsVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, tangentsVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeTangents) * 3,cubeTangents, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &cubeTextureVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeTextureVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeTexCoords) * 2,cubeTexCoords, GL_STATIC_DRAW_ARB);
	
	glGenBuffersARB(1, &cubeIBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW_ARB);
}

void cubeVBO()
{
	
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeTextureVBO);
	glTexCoordPointer(2, GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeVertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeNormalVBO);
	glNormalPointer(GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, tangentsVBO);
	glColorPointer(3,GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDrawArrays(GL_QUADS,0,24);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}

void cubePickingVBO()
{
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeVertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDrawArrays(GL_QUADS,0,24);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_CULL_FACE);
}

SVector3 getColor(int id)
{
	return colors[id];
}

void quickClear()
{
	glClearDepth(1);
	glClearColor(0.6f,0.6f,0.6f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void quickTranslate(SVector3 pos)
{
	glTranslated(pos.x,pos.y,pos.z);
}

void quickScale(SVector3 scale)
{
	glScaled(scale.x,scale.y,scale.z);
}

void quad()
{
	glBegin(GL_QUADS);
			glVertex2f(-1,-1);
			glVertex2f(1,-1);
			glVertex2f(1,1);
			glVertex2f(-1,1);
	glEnd();
}

void texturedQuad()
{
	glEnable(GL_TEXTURE_2D);

	glNormal3f(0,0,1);

	glBegin(GL_QUADS);
			glColor3f(0.5,0.5,0.5);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,0); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor4f(0,0,0,1);
			//glColor3f(1.0f, 0.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(-1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,0);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 0.0f, 1.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(1,1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,1); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(-1,1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void tangentQuad()
{
	glEnable(GL_TEXTURE_2D);

	glNormal3f(0,0,1);

	glBegin(GL_QUADS);
			glColor3f(0,0,1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,0); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor4f(0,0,0,1);
			//glColor3f(1.0f, 0.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(-1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,0);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 0.0f, 1.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(1,1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,1); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(-1,1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void grassQuad()
{
	//for rendering punchout textures like
	//grass and trees, normal is always up (or possibly on heightmaps normal)
	//Y-texture coords are flipped in this instance

	glEnable(GL_TEXTURE_2D);

	glNormal3f(0,1,0);

	glBegin(GL_QUADS);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 
			glVertex2f(-1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 
			glVertex2f(1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 
			glVertex2f(1,1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 
			glVertex2f(-1,1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void cube()
{
	SVector3 m_vertex[8];
	SVector3 m_boxQuads[6][4];

	m_vertex[0] = SVector3(-1,-1,-1);
	m_vertex[1] = SVector3(1,-1,-1);
	m_vertex[2] = SVector3(1,1,-1);
	m_vertex[3] = SVector3(-1,1,-1);
	m_vertex[4] = SVector3(1,1,1);
	m_vertex[5] = SVector3(1,-1,1);
	m_vertex[6] = SVector3(-1,1,1);
	m_vertex[7] = SVector3(-1,-1,1);

	//front
	m_boxQuads[0][0] = SVector3(m_vertex[7].x,m_vertex[7].y,m_vertex[7].z);
	m_boxQuads[0][1] = SVector3(m_vertex[5].x,m_vertex[5].y,m_vertex[5].z);
	m_boxQuads[0][2] = SVector3(m_vertex[4].x,m_vertex[4].y,m_vertex[4].z);
	m_boxQuads[0][3] = SVector3(m_vertex[6].x,m_vertex[6].y,m_vertex[6].z);

	//back
	m_boxQuads[1][0] = SVector3(m_vertex[0].x,m_vertex[0].y,m_vertex[0].z);
	m_boxQuads[1][1] = SVector3(m_vertex[3].x,m_vertex[3].y,m_vertex[3].z);
	m_boxQuads[1][2] = SVector3(m_vertex[2].x,m_vertex[2].y,m_vertex[2].z);
	m_boxQuads[1][3] = SVector3(m_vertex[1].x,m_vertex[1].y,m_vertex[1].z);

	//left
	m_boxQuads[2][0] = SVector3(m_vertex[7].x,m_vertex[7].y,m_vertex[7].z);
	m_boxQuads[2][1] = SVector3(m_vertex[6].x,m_vertex[6].y,m_vertex[6].z);
	m_boxQuads[2][2] = SVector3(m_vertex[3].x,m_vertex[3].y,m_vertex[3].z);
	m_boxQuads[2][3] = SVector3(m_vertex[0].x,m_vertex[0].y,m_vertex[0].z);

	//right
	m_boxQuads[3][0] = SVector3(m_vertex[1].x,m_vertex[1].y,m_vertex[1].z);
	m_boxQuads[3][1] = SVector3(m_vertex[2].x,m_vertex[2].y,m_vertex[2].z);
	m_boxQuads[3][2] = SVector3(m_vertex[4].x,m_vertex[4].y,m_vertex[4].z);
	m_boxQuads[3][3] = SVector3(m_vertex[5].x,m_vertex[5].y,m_vertex[5].z);

	//top
	m_boxQuads[4][0] = SVector3(m_vertex[3].x,m_vertex[3].y,m_vertex[3].z);
	m_boxQuads[4][1] = SVector3(m_vertex[6].x,m_vertex[6].y,m_vertex[6].z);
	m_boxQuads[4][2] = SVector3(m_vertex[4].x,m_vertex[4].y,m_vertex[4].z);
	m_boxQuads[4][3] = SVector3(m_vertex[2].x,m_vertex[2].y,m_vertex[2].z);

	//bottom
	m_boxQuads[5][0] = SVector3(m_vertex[1].x,m_vertex[1].y,m_vertex[1].z);
	m_boxQuads[5][1] = SVector3(m_vertex[5].x,m_vertex[5].y,m_vertex[5].z);
	m_boxQuads[5][2] = SVector3(m_vertex[7].x,m_vertex[7].y,m_vertex[7].z);
	m_boxQuads[5][3] = SVector3(m_vertex[0].x,m_vertex[0].y,m_vertex[0].z);

	for(int i = 0; i < 6; i++)
	{
		SVector3 normal = getNormal(m_boxQuads[i]);
		normal = normal * -1;
		glNormal3f(normal.x,normal.y,normal.z);

		glBegin(GL_QUADS);
			glVertex3f(m_boxQuads[i][0].x,m_boxQuads[i][0].y,m_boxQuads[i][0].z);
			glVertex3f(m_boxQuads[i][1].x,m_boxQuads[i][1].y,m_boxQuads[i][1].z);
			glVertex3f(m_boxQuads[i][2].x,m_boxQuads[i][2].y,m_boxQuads[i][2].z);
			glVertex3f(m_boxQuads[i][3].x,m_boxQuads[i][3].y,m_boxQuads[i][3].z);
		glEnd();
	}
}

void grid(int cellSize, int dimension, int divisions)
{
	int gridSize = dimension * cellSize;

	int x = - ((dimension/2) * cellSize);
	int y = 0;
	int z = - ((dimension/2) * cellSize);

	int divisionCounter = divisions;

	for(int i = 0; i <= dimension; i++)
	{
		if((divisionCounter == 0) && (i != dimension)) 
		{
			divisionCounter = divisions;
			glColor3f(0.1,0.1,0.1);
		}
		else glColor3f(0.7,0.7,0.7);

		glBegin(GL_LINES);
			glVertex3i(x,y,z);
			glVertex3i(x + gridSize,y,z);
		glEnd();

		z += cellSize;

		divisionCounter--;
	}

	z = - ((dimension/2) * cellSize);
	divisionCounter = divisions;

	for(int i = 0; i <= dimension; i++)
	{
		if((divisionCounter == 0) && (i != dimension))
		{
			divisionCounter = divisions;
			glColor3f(0.1,0.1,0.1);
		}
		else glColor3f(0.7,0.7,0.7);

		glBegin(GL_LINES);
			glVertex3f(x,y,z);
			glVertex3f(x,y,z + gridSize);
		glEnd();

		x += cellSize;

		divisionCounter--;
	}

}

SVector3 translateWidgetDepthTest(SVector3 center)
{
	float scale = distance(center,user::getInstance()->input.pos) / 100;
	bool resultBool[3] = {0,0,0};

	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glLineWidth(10);

	glColor4f(1,1,1,1);

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);
		
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_TEST);

		//x-z
		glColor3f(1,0,1);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,0,10);
		glEnd();

		//x-y
		glColor3f(1,1,0);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,10,0);
		glEnd();

		//y-z
		glColor3f(0,1,1);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
		glVertex3f(0,0,10);
		glEnd();

		glDisable(GL_DEPTH_TEST);
		//x-axis
		glPushMatrix();
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20,0,0);
			glEnd();
		glPopMatrix();
		//y axis
		glPushMatrix();
			glColor3f(0,1,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
			glEnd();
		glPopMatrix();
		//z axis
		glPushMatrix();
			glColor3f(0,0,1);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
			glEnd();
		glPopMatrix();

		//all
		glColor3f(1,1,1);
		cube();

	glPopMatrix();

	colorByte pickID = user::getInstance()->getPixelAtMouse();

	resultBool[0] = pickID.r / 255;
	resultBool[1] = pickID.g / 255;
	resultBool[2] = pickID.b / 255;

	glLineWidth(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	SVector3 result(resultBool[0],resultBool[1],resultBool[2]);
	return result;
}

void translateWidget(SVector3 center, SVector3 selected)
{
	float scale = distance(center,user::getInstance()->input.pos) / 100;

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glLineWidth(4);

		//x-y
		glColor4f(0.4,0.4,0.4,0.2);
		if((selected.x == 1) && (selected.y == 1)) glColor4f(1,0.7,0,0.2);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,10,0);
		glEnd();

		glColor3f(0.7,0.7,0.7);
		if((selected.x == 1) && (selected.y == 1)) glColor3f(1,0.7,0);
		glBegin(GL_LINES);
		glVertex3f(10,0,0);
		glVertex3f(0,10,0);
		glVertex3f(8,0,0);
		glVertex3f(0,8,0);
		glEnd();

		//x-z
		glColor4f(0.4,0.4,0.4,0.2);
		if((selected.x == 1) && (selected.z == 1)) glColor4f(1,0.7,0,0.2);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,0,10);
		glEnd();

		glColor3f(0.7,0.7,0.7);
		if((selected.x == 1) && (selected.z == 1)) glColor3f(1,0.7,0);
		glBegin(GL_LINES);
		glVertex3f(10,0,0);
		glVertex3f(0,0,10);
		glVertex3f(8,0,0);
		glVertex3f(0,0,8);
		glEnd();

		//y-z
		glColor4f(0.4,0.4,0.4,0.2);
		if((selected.y == 1) && (selected.z == 1)) glColor4f(1,0.7,0,0.2);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
		glVertex3f(0,0,10);
		glEnd();

		glColor3f(0.7,0.7,0.7);
		if((selected.z == 1) && (selected.y == 1)) glColor3f(1,0.7,0);
		glBegin(GL_LINES);
		glVertex3f(0,10,0);
		glVertex3f(0,0,10);
		glVertex3f(0,8,0);
		glVertex3f(0,0,8);
		glEnd();
		
		//x-axis
		glPushMatrix();
			if(selected.x == 1) glColor3f(1,0.7,0);
			else glColor3f(0.5,0,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20,0,0);
			glEnd();
		glPopMatrix();
		//y axis
		glPushMatrix();
			if(selected.y == 1) glColor3f(1,0.7,0);
			else glColor3f(0,0.5,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
			glEnd();
		glPopMatrix();
		//z axis
		glPushMatrix();
			if(selected.z == 1) glColor3f(1,0.7,0);
			else glColor3f(0,0,0.5);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
			glEnd();
		glPopMatrix();

	glPopMatrix();

}

void rotateWidget(SVector3 center, SVector3 selected)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glLineWidth(4);

	float scale = distance(center,user::getInstance()->input.pos) / 100;

	glPushMatrix();
	
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

	int segments = 20;

	float xline = 0;
	float yline = 0;
	float zline = 0;

	float rot = 0;
	int size = 10;

	//x-z (ABOUT Y)
	glColor3f(0,0.5,0);
	if(selected.y == 1) glColor3f(1,0.7,0);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		double yrot = degreesToRadians(rot);

		xline = float( (cos(yrot) * size) );
		zline = float( (sin(yrot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	glPopMatrix();

	//glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
}

void circle(float radius,int segments, bool x, bool y, bool z)
{
	float xline = 0;
	float yline = 0;
	float zline = 0;

	float rot = 0;

	if(y)
	{
		//x-z (ABOUT Y)
		glBegin(GL_LINE_STRIP);

		for(int i = 0; i <= segments; i++)
		{
			double yrot = degreesToRadians(rot);

			xline = float( (cos(yrot) * radius) );
			zline = float( (sin(yrot) * radius) );

			glVertex3f(xline,yline,zline);

			rot += 360.0f / segments;
		}

		glEnd();
	}
	else if(x)
	{
		//z-y (ABOUT X)
		glBegin(GL_LINE_STRIP);

		for(int i = 0; i <= segments; i++)
		{
			double radRot = degreesToRadians(rot);

			yline = float( (cos(radRot) * radius) );
			zline = float( (sin(radRot) * radius) );

			glVertex3f(xline,yline,zline);

			rot += 360.0f / segments;
		}

		glEnd();
	}
	else
	{
		//x-y (ABOUT Z)
		glBegin(GL_LINE_STRIP);

		for(int i = 0; i <= segments; i++)
		{
			double radRot = degreesToRadians(rot);

			yline = float( (cos(radRot) * radius) );
			xline = float( (sin(radRot) * radius) );

			glVertex3f(xline,yline,zline);

			rot += 360.0f / segments;
		}

		glEnd();
	}
}

SVector3 rotateWidgetDepthTest(SVector3 center)
{
	float scale = distance(center,user::getInstance()->input.pos) / 100;
	bool resultBool[3] = {0,0,0};

	glLineWidth(10);

	glPushMatrix();
	
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

	int segments = 30;

	float xline = 0;
	float yline = 0;
	float zline = 0;

	float rot = 0;
	int size = 10;

	glDisable(GL_BLEND);

	//x-z (ABOUT Y)
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		double yrot = degreesToRadians(rot);

		xline = float( (cos(yrot) * size) );
		zline = float( (sin(yrot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	xline = 0;
	yline = 0;
	zline = 0;

	//z-y (ABOUT X)
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		double radRot = degreesToRadians(rot);

		yline = float( (cos(radRot) * size) );
		zline = float( (sin(radRot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	xline = 0;
	yline = 0;
	zline = 0;

	//x-y (ABOUT Z)
	glColor3f(0,0,1);
	glBegin(GL_LINE_STRIP);

	for(int i = 0; i <= segments; i++)
	{
		double radRot = degreesToRadians(rot);

		yline = float( (cos(radRot) * size) );
		xline = float( (sin(radRot) * size) );

		glVertex3f(xline,yline,zline);

		rot += 360.0f / segments;
	}

	glEnd();

	glPopMatrix();

	glLineWidth(1);

	colorByte pickID = user::getInstance()->getPixelAtMouse();

	resultBool[0] = pickID.r / 255;
	resultBool[1] = pickID.g / 255;
	resultBool[2] = pickID.b / 255;

	SVector3 result(resultBool[0],resultBool[1],resultBool[2]);
	return result;

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void orthoMode(int left, int top, int right, int bottom)
{
	// Switch to our projection matrix so that we can change it to ortho mode, not perspective.
	glMatrixMode(GL_PROJECTION);						

	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();									
	
	// Reset the current matrix to our identify matrix
	glLoadIdentity();								

	//Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho( left, right, bottom, top, 0, 1 );	
	
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);								

	// Initialize the current model view matrix with the identity matrix
	glLoadIdentity();										
}

void perspectiveMode()										// Set Up A Perspective View
{
	// Enter into our projection matrix mode
	glMatrixMode( GL_PROJECTION );							

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();											

	// Go back to our model view matrix like normal
	glMatrixMode( GL_MODELVIEW );							

	// We should be in the normal 3D perspective mode now
}

void useTranslateWidget(SVector3 *selectedAxis, SVector3 *position)
{
	glEnable(GL_DEPTH_TEST);

	if(!user::getInstance()->input.leftMouse)
	{
		*selectedAxis = translateWidgetDepthTest(*position);
	}
	else
	{
		float widgetScale = distance(*position,user::getInstance()->input.pos) / 100;

		float incx = user::getInstance()->input.mouseIncrement.x * widgetScale / 10;
		float incy = user::getInstance()->input.mouseIncrement.y * widgetScale / 10;

		double xrot = degreesToRadians(user::getInstance()->input.rot.x);
		double yrot = degreesToRadians(user::getInstance()->input.rot.y);

		position->x += selectedAxis->x * float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
		position->y -= selectedAxis->y * float( cos(xrot) * incy );
		position->z += selectedAxis->z * float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );
	}

	glClearDepth(1);
	glClearColor (0.6f,0.6f,0.6f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
}

void useScaleWidget(SVector3 *selectedAxis, SVector3 *position,SVector3 *scale)
{
	glEnable(GL_DEPTH_TEST);

	if(!user::getInstance()->input.leftMouse)
	{
		*selectedAxis = translateWidgetDepthTest(*position);
	}
	else
	{
		float widgetScale = distance(*position,user::getInstance()->input.pos) / 100;

		float incx = user::getInstance()->input.mouseIncrement.x * widgetScale / 10;
		float incy = user::getInstance()->input.mouseIncrement.y * widgetScale / 10;

		double xrot = degreesToRadians(user::getInstance()->input.rot.x);
		double yrot = degreesToRadians(user::getInstance()->input.rot.y);

		scale->x += selectedAxis->x * float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
		scale->y -= selectedAxis->y * float( cos(xrot) * incy );
		scale->z += selectedAxis->z * float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );
	}

	glClearDepth(1);
	glClearColor (0.6f,0.6f,0.6f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
}

void useRotateWidget(SVector3 *selectedAxis, SVector3 *position,SVector3 *rotation)
{
	glEnable(GL_DEPTH_TEST);

	if(!user::getInstance()->input.leftMouse)
	{
		*selectedAxis = rotateWidgetDepthTest(*position);
	}
	else
	{
		float widgetScale = distance(*position,user::getInstance()->input.pos) / 100;

		float incx = user::getInstance()->input.mouseIncrement.x * widgetScale / 10;
		float incy = user::getInstance()->input.mouseIncrement.y * widgetScale / 10;

		double xrot = degreesToRadians(user::getInstance()->input.rot.x);
		double yrot = degreesToRadians(user::getInstance()->input.rot.y);

		rotation->x += selectedAxis->x * (incx + incy);
		rotation->y += selectedAxis->y * (incx + incy);
		rotation->z += selectedAxis->z * (incx + incy);
	}

	glClearDepth(1);
	glClearColor (0.6f,0.6f,0.6f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
}

void sphere() 
{
	int space = 20;

	int vertexCount = 0;
	SVector3 vertices[1024];
	SVector2<float> textureCoords[1024];

	for(float b = 0; b <= 180 - space; b+=space)
	{
		for(float a = 0; a <= 360 - space; a+=space)
		{
			//first point
			vertices[vertexCount].x = float(sin((a) / 180 * PI) * sin((b) / 180 * PI));
			vertices[vertexCount].y = float(cos((a) / 180 * PI) * sin((b) / 180 * PI));
			vertices[vertexCount].z = float(cos((b) / 180 * PI));

			textureCoords[vertexCount].x = (2 * b) / 360;
			textureCoords[vertexCount].y = (a) / 360;

			vertexCount++;

			//second point
			vertices[vertexCount].x = sin((a) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].y = cos((a) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].z = cos((b + space) / 180 * PI);

			textureCoords[vertexCount].x = (2 * (b + space)) / 360;
			textureCoords[vertexCount].y = (a) / 360;

			vertexCount++;

			//third point
			vertices[vertexCount].x = sin((a + space) / 180 * PI) * sin((b) / 180 * PI);
			vertices[vertexCount].y = cos((a + space) / 180 * PI) * sin((b) / 180 * PI);
			vertices[vertexCount].z = cos((b) / 180 * PI);

			textureCoords[vertexCount].x = (2 * b) / 360;
			textureCoords[vertexCount].y = (a + space) / 360;

			vertexCount++;

			//fourth point
			vertices[vertexCount].x = sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].y = cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].z = cos((b + space) / 180 * PI);

			textureCoords[vertexCount].x = (2 * (b + space)) / 360;
			textureCoords[vertexCount].y = (a + space) / 360;

			vertexCount++;
		}
	}

	glBegin (GL_TRIANGLE_STRIP);

		for (int i = 0; i < vertexCount; i++)
		{
			SVector3 normal = normalize(vertices[i]);

			glNormal3f(normal.x,normal.y,normal.z);

			glMultiTexCoord2f(GL_TEXTURE1_ARB,textureCoords[i].x,textureCoords[i].y);
			glVertex3f (vertices[i].x, vertices[i].y, -vertices[i].z);
		}

	glEnd();

}
