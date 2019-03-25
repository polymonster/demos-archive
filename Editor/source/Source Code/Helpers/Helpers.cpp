#include "Helpers.h"
#include "Input.h"
#include "Fonts.h"
#include "timerSingleton.h"

#define DTOR (PI/180.0f)
#define SQR(x) (x*x)

float offy = 0;

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

SVector3 cubeNormals[8] = 
{
	SVector3(-0.3f,-0.3f,-0.3f),
	SVector3(-0.3f,-0.3f,0.3f),
	SVector3(0.3f,-0.3f,0.3f),
	SVector3(0.3f,-0.3f,-0.3f),

	SVector3(-0.3f,0.3f,-0.3f),
	SVector3(-0.3f,0.3f,0.3f),
	SVector3(0.3f,0.3f,0.3f),
	SVector3(0.3f,0.3f,-0.3f),
};

unsigned int cubeIndices[24] =
{	
	0,3,2,1,
	4,5,6,7,

	0,1,5,4,
	2,3,7,6,

	1,2,6,5,
	0,4,7,3,
};

SVector3 cubeVertices[8] = 
{
	SVector3(-1,-1,-1),
	SVector3(-1,-1,1),
	SVector3(1,-1,1),
	SVector3(1,-1,-1),

	SVector3(-1,1,-1),
	SVector3(-1,1,1),
	SVector3(1,1,1),
	SVector3(1,1,-1),
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

	/*glGenBuffersARB(1, &tangentsVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, tangentsVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeTangents) * 3,cubeTangents, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &cubeTextureVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeTextureVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeTexCoords) * 2,cubeTexCoords, GL_STATIC_DRAW_ARB);*/
	
	glGenBuffersARB(1, &cubeIBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW_ARB);
}

void cubeVBO()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeVertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeNormalVBO);
	glNormalPointer(GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);

	//glDrawArrays(GL_QUADS,0,0);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );

	
	/*glEnable(GL_CULL_FACE);

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
	glDisableClientState( GL_COLOR_ARRAY );*/
}

void cubePickingVBO()
{
	/*glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeVertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); 

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDrawArrays(GL_QUADS,0,24);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_CULL_FACE);*/
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
			glVertex2f(0,0);
			glVertex2f(0,-1);
			glVertex2f(1,-1);
			glVertex2f(1,0);
	glEnd();
}

void centeredQuad()
{
	glBegin(GL_QUADS);
			glVertex2f(-1,-1);
			glVertex2f(-1,1);
			glVertex2f(1,1);
			glVertex2f(1,-1);
	glEnd();
}

void texturedQuad()
{
	glEnable(GL_TEXTURE_2D);

	glNormal3f(0,0,1);

	glBegin(GL_QUADS);
			//glColor3f(0.5,0.5,0.5);
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
	SVector3 vertex[8];
	SVector3 normal[8];
	SVector3 boxQuads[6][4];
	SVector3 boxNormals[6][4];

	SVector2<float> texCoords[4];

	texCoords[0].x = 0;
	texCoords[0].y = 0;

	texCoords[1].x = 0;
	texCoords[1].y = 1;

	texCoords[2].x = 1;
	texCoords[2].y = 1;

	texCoords[3].x = 1;
	texCoords[3].y = 0;


	vertex[0] = SVector3(-1,-1,-1);
	normal[0] = SVector3(-0.3,-0.3,-0.3);

	vertex[1] = SVector3(1,-1,-1);
	normal[1] = SVector3(0.3,-0.3,-0.3);

	vertex[2] = SVector3(1,1,-1);
	normal[2] = SVector3(0.3,0.3,-0.3);

	vertex[3] = SVector3(-1,1,-1);
	normal[3] = SVector3(-0.3,0.3,-0.3);

	vertex[4] = SVector3(1,1,1);
	normal[4] = SVector3(0.3,0.3,0.3);

	vertex[5] = SVector3(1,-1,1);
	normal[5] = SVector3(0.3,-0.3,0.3);

	vertex[6] = SVector3(-1,1,1);
	normal[6] = SVector3(-0.3,0.3,0.3);

	vertex[7] = SVector3(-1,-1,1);
	normal[7] = SVector3(-0.3,-0.3,0.3);

	//front
	boxQuads[0][0] = SVector3(vertex[7].x,vertex[7].y,vertex[7].z);
	boxQuads[0][1] = SVector3(vertex[5].x,vertex[5].y,vertex[5].z);
	boxQuads[0][2] = SVector3(vertex[4].x,vertex[4].y,vertex[4].z);
	boxQuads[0][3] = SVector3(vertex[6].x,vertex[6].y,vertex[6].z);

	boxNormals[0][0] = normal[7];
	boxNormals[0][1] = normal[5];
	boxNormals[0][2] = normal[4];
	boxNormals[0][3] = normal[6];

	//back
	boxQuads[1][0] = SVector3(vertex[0].x,vertex[0].y,vertex[0].z);
	boxQuads[1][1] = SVector3(vertex[3].x,vertex[3].y,vertex[3].z);
	boxQuads[1][2] = SVector3(vertex[2].x,vertex[2].y,vertex[2].z);
	boxQuads[1][3] = SVector3(vertex[1].x,vertex[1].y,vertex[1].z);

	boxNormals[1][0] = normal[0];
	boxNormals[1][1] = normal[3];
	boxNormals[1][2] = normal[2];
	boxNormals[1][3] = normal[1];

	//left
	boxQuads[2][0] = SVector3(vertex[7].x,vertex[7].y,vertex[7].z);
	boxQuads[2][1] = SVector3(vertex[6].x,vertex[6].y,vertex[6].z);
	boxQuads[2][2] = SVector3(vertex[3].x,vertex[3].y,vertex[3].z);
	boxQuads[2][3] = SVector3(vertex[0].x,vertex[0].y,vertex[0].z);

	boxNormals[2][0] = normal[7];
	boxNormals[2][1] = normal[6];
	boxNormals[2][2] = normal[3];
	boxNormals[2][3] = normal[0];

	//right
	boxQuads[3][0] = SVector3(vertex[1].x,vertex[1].y,vertex[1].z);
	boxQuads[3][1] = SVector3(vertex[2].x,vertex[2].y,vertex[2].z);
	boxQuads[3][2] = SVector3(vertex[4].x,vertex[4].y,vertex[4].z);
	boxQuads[3][3] = SVector3(vertex[5].x,vertex[5].y,vertex[5].z);

	boxNormals[3][0] = normal[1];
	boxNormals[3][1] = normal[2];
	boxNormals[3][2] = normal[4];
	boxNormals[3][3] = normal[5];

	//top
	boxQuads[4][0] = SVector3(vertex[3].x,vertex[3].y,vertex[3].z);
	boxQuads[4][1] = SVector3(vertex[6].x,vertex[6].y,vertex[6].z);
	boxQuads[4][2] = SVector3(vertex[4].x,vertex[4].y,vertex[4].z);
	boxQuads[4][3] = SVector3(vertex[2].x,vertex[2].y,vertex[2].z);

	boxNormals[4][0] = normal[3];
	boxNormals[4][1] = normal[6];
	boxNormals[4][2] = normal[4];
	boxNormals[4][3] = normal[2];

	//bottom
	boxQuads[5][0] = SVector3(vertex[1].x,vertex[1].y,vertex[1].z);
	boxQuads[5][1] = SVector3(vertex[5].x,vertex[5].y,vertex[5].z);
	boxQuads[5][2] = SVector3(vertex[7].x,vertex[7].y,vertex[7].z);
	boxQuads[5][3] = SVector3(vertex[0].x,vertex[0].y,vertex[0].z);

	boxNormals[5][0] = normal[1];
	boxNormals[5][1] = normal[5];
	boxNormals[5][2] = normal[7];
	boxNormals[5][3] = normal[0];

	for(int i = 0; i < 6; i++)
	{
		//SVector3 normal = getNormal(m_boxQuads[i]);
		//normal = normal * -1;
		//glNormal3f(normal.x,normal.y,normal.z);

		glBegin(GL_QUADS);
			glTexCoord2f(texCoords[0].x,texCoords[0].y);
			glNormal3f(boxNormals[i][0].x,boxNormals[i][0].y,boxNormals[i][0].z);
			glVertex3f(boxQuads[i][0].x,boxQuads[i][0].y,boxQuads[i][0].z);

			glTexCoord2f(texCoords[1].x,texCoords[1].y);
			glNormal3f(boxNormals[i][1].x,boxNormals[i][1].y,boxNormals[i][1].z);
			glVertex3f(boxQuads[i][1].x,boxQuads[i][1].y,boxQuads[i][1].z);

			glTexCoord2f(texCoords[2].x,texCoords[2].y);
			glNormal3f(boxNormals[i][2].x,boxNormals[i][2].y,boxNormals[i][2].z);
			glVertex3f(boxQuads[i][2].x,boxQuads[i][2].y,boxQuads[i][2].z);

			glTexCoord2f(texCoords[3].x,texCoords[3].y);
			glNormal3f(boxNormals[i][3].x,boxNormals[i][3].y,boxNormals[i][3].z);
			glVertex3f(boxQuads[i][3].x,boxQuads[i][3].y,boxQuads[i][3].z);
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

void setCullMode(int mode)
{
	switch(mode)
	{
		case 0:

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		break;

		case 1:

		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);

		break;

		case 2:

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);

		break;


	}
}

SVector3 translateWidgetDepthTest(SVector3 center)
{
	float scale = distance(center,g_input.pos) / 100;
	bool resultBool[3] = {0,0,0};

	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glLineWidth(10);

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);
		
		//glEnable(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);
		//x-y
		glColor3f(1,1,0);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,10,0);
		glEnd();

		//x-z
		glColor3f(1,0,1);
		glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,0,10);
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

void translateWidget(SVector3 center, SVector3 selected, int type)
{
	float scale = distance(center,g_input.pos) / 100;

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glLineWidth(1.5);

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
		if(type == 1) 
		{
			glPushMatrix();
			glTranslated(20,0,0) ;
			glScaled(0.5,0.5,0.5);
			cube();
			glPopMatrix();
		}
		//y axis
		glPushMatrix();
			if(selected.y == 1) glColor3f(1,0.7,0);
			else glColor3f(0,0.5,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
			glEnd();
		glPopMatrix();
		if(type == 1) 
		{
			glPushMatrix();
			glTranslated(0,20,0) ;
			glScaled(0.5,0.5,0.5);
			cube();
			glPopMatrix();
		}
		//z axis
		glPushMatrix();
			if(selected.z == 1) glColor3f(1,0.7,0);
			else glColor3f(0,0,0.5);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
			glEnd();
		glPopMatrix();
		if(type == 1) 
		{
			glPushMatrix();
			glTranslated(0,0,20) ;
			glScaled(0.5,0.5,0.5);
			cube();
			glPopMatrix();
		}

	glPopMatrix();

	glLineWidth(1);

}

void rotateWidget(SVector3 center, SVector3 selected)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glLineWidth(1.5);

	float scale = distance(center,g_input.pos) / 100;

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

		xline = 0;
	yline = 0;
	zline = 0;

	//z-y (ABOUT X)
	glColor3f(0.5,0,0);
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
	glColor3f(0,0,0.5);
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

	//glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);

	glLineWidth(1);
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
	float scale = distance(center,g_input.pos) / 100;
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

	if(!g_input.leftMouse)
	{
		*selectedAxis = translateWidgetDepthTest(*position);
	}
	else
	{
		float widgetScale = distance(*position,g_input.pos) / 100;

		float incx = g_input.mouseIncrement.x * widgetScale / 10;
		float incy = g_input.mouseIncrement.y * widgetScale / 10;

		double xrot = degreesToRadians(g_input.rot.x);
		double yrot = degreesToRadians(g_input.rot.y);

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

	if(!g_input.leftMouse)
	{
		*selectedAxis = translateWidgetDepthTest(*position);
	}
	else
	{
		float widgetScale = distance(*position,g_input.pos) / 100;

		float incx = g_input.mouseIncrement.x * widgetScale / 10;
		float incy = g_input.mouseIncrement.y * widgetScale / 10;

		double xrot = degreesToRadians(g_input.rot.x);
		double yrot = degreesToRadians(g_input.rot.y);

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

	if(!g_input.leftMouse)
	{
		*selectedAxis = rotateWidgetDepthTest(*position);
	}
	else
	{
		float widgetScale = distance(*position,g_input.pos) / 100;

		float incx = g_input.mouseIncrement.x * widgetScale / 10;
		float incy = g_input.mouseIncrement.y * widgetScale / 10;

		double xrot = degreesToRadians(g_input.rot.x);
		double yrot = degreesToRadians(g_input.rot.y);

		rotation->x += selectedAxis->x * (incx + incy);
		rotation->y += selectedAxis->y * (incx + incy);
		rotation->z += selectedAxis->z * (incx + incy);
	}

	glClearDepth(1);
	glClearColor (0.6f,0.6f,0.6f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
}



void GenerateDome(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	int theta, phi;
	
	int NumVertices = 0;

	// Initialize our Vertex array
	NumVertices = (int)((360/dtheta)*(90/dphi)*4);
	SVector3 Vertices[6000];
	SVector3 texCoords[6000]; 

	// Used to calculate the UV coordinates
	float vx, vy, vz, mag;

	// Generate the dome
	int n = 0;
	for (phi=0; phi <= 90 - dphi; phi += (int)dphi)
	{
		for (theta=0; theta <= 360 - dtheta; theta += (int)dtheta)
		{
			// Calculate the vertex at phi, theta
			Vertices[n].x = radius * sinf(phi*DTOR) * cosf(DTOR*theta);
			Vertices[n].y = radius * sinf(phi*DTOR) * sinf(DTOR*theta);
			Vertices[n].z = radius * cosf(phi*DTOR);

			// Create a vector from the origin to this vertex
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			// Normalize the vector
			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			// Calculate the spherical texture coordinates
			texCoords[n].x = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			texCoords[n].y = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi+dphi, theta
			Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(theta*DTOR);
			Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(theta*DTOR);
			Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
			
			// Calculate the texture coordinates
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			// Calculate the spherical texture coordinates
			texCoords[n].x = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			texCoords[n].y = vTile * (float)(asinf(vy) / PI) + 0.5f;			
			n++;

			// Calculate the vertex at phi, theta+dtheta
			Vertices[n].x = radius * sinf(DTOR*phi) * cosf(DTOR*(theta+dtheta));
			Vertices[n].y = radius * sinf(DTOR*phi) * sinf(DTOR*(theta+dtheta));
			Vertices[n].z = radius * cosf(DTOR*phi);
			
			// Calculate the texture coordinates
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			// Calculate the spherical texture coordinates
			texCoords[n].x = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			texCoords[n].y = vTile * (float)(asinf(vy) / PI) + 0.5f;			
			n++;

			if (phi > -90 && phi < 90)
			{
				// Calculate the vertex at phi+dphi, theta+dtheta
				Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta));
				Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta));
				Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
				
				// Calculate the texture coordinates
				vx = Vertices[n].x;
				vy = Vertices[n].y;
				vz = Vertices[n].z;

				mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
				vx /= mag;
				vy /= mag;
				vz /= mag;

				// Calculate the spherical texture coordinates
				texCoords[n].x = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
				texCoords[n].y = vTile * (float)(asinf(vy) / PI) + 0.5f;			
				n++;
			}
		}
	}

	// Fix the problem at the seam
	for (int i=0; i < NumVertices-3; i++)
	{
		if (texCoords[i].x - texCoords[i+1].x  > 0.9f)
			texCoords[i+1].x  += 1.0f;

		if (texCoords[i+1].x - texCoords[i].x  > 0.9f)
			texCoords[i].x  += 1.0f;

		if (texCoords[i].x - texCoords[i+2].x  > 0.9f)
			texCoords[i+2].x  += 1.0f;

		if (texCoords[i+2].x - texCoords[i].x  > 0.9f)
			texCoords[i].x  += 1.0f;

		if (texCoords[i+1].x - texCoords[i+2].x  > 0.9f)
			texCoords[i+2].x  += 1.0f;

		if (texCoords[i+2].x - texCoords[i+1].x  > 0.9f)
			texCoords[i+1].x  += 1.0f;

		if (texCoords[i].y - texCoords[i+1].y  > 0.9f)
			texCoords[i+1].y  += 1.0f;

		if (texCoords[i+1].y - texCoords[i].y  > 0.9f)
			texCoords[i].y  += 1.0f;

		if (texCoords[i].y - texCoords[i+2].y > 0.9f)
			texCoords[i+2].y  += 1.0f;

		if (texCoords[i+2].y - texCoords[i].y > 0.9f)
			texCoords[i].y  += 1.0f;

		if (texCoords[i+1].y - texCoords[i+2].y  > 0.9f)
			texCoords[i+2].y  += 1.0f;

		if (texCoords[i+2].y - texCoords[i+1].y > 0.9f)
			texCoords[i+1].y  += 1.0f;
	}

	glBegin(GL_TRIANGLE_STRIP);

	for (int i=0; i < NumVertices; i++)
	{
		glMultiTexCoord2f(GL_TEXTURE1_ARB,texCoords[i].y,texCoords[i].x);
		glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
	}

	glEnd();

}

void sphere() 
{
	int space = 5;

	int vertexCount = 0;
	SVector3 vertices[10000];
	SVector2<float> textureCoords[10000];

	for(float b = 0; b <= 180 - space; b+=space)
	{
		for(float a = 0; a <= 180 - space; a+=space)
		{
			//first point
			vertices[vertexCount].x = float(sin((a) / 180 * PI) * sin((b) / 180 * PI));
			vertices[vertexCount].y = float(cos((a) / 180 * PI) * sin((b) / 180 * PI));
			vertices[vertexCount].z = float(cos((b) / 180 * PI));

			//textureCoords[vertexCount].x = (2 * b) / 180;
			//textureCoords[vertexCount].y = (a) / 180;

			SVector3 v = normalize(vertices[vertexCount]);
			textureCoords[vertexCount].x = (float)(atan2(v.y, v.z)/(PI*2)) + 0.5f;
			textureCoords[vertexCount].y = (float)(asinf(-v.x) / PI) * 2;	

			//textureCoords[vertexCount].x = (2 * b) / 360;//(asin(vertices[vertexCount].y) - acos(vertices[vertexCount].z)) / 360.0f;
			//textureCoords[vertexCount].y = -vertices[vertexCount].x;

			vertexCount++;

			//second point
			vertices[vertexCount].x = sin((a) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].y = cos((a) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].z = cos((b + space) / 180 * PI);

			//textureCoords[vertexCount].x = (2 * (b + space)) / 360;
			//textureCoords[vertexCount].y = (a) / 180;

			v = normalize(vertices[vertexCount]);
			textureCoords[vertexCount].x = (float)(atan2(v.y, v.z)/(PI*2)) + 0.5f;
			textureCoords[vertexCount].y = (float)(asinf(-v.x) / PI) * 2;	

			//textureCoords[vertexCount].x = (2 * (b + space)) / 360;//(asin(vertices[vertexCount].y) - acos(vertices[vertexCount].z)) / 360.0f;
			//textureCoords[vertexCount].y = -vertices[vertexCount].x;

			vertexCount++;

			//third point
			vertices[vertexCount].x = sin((a + space) / 180 * PI) * sin((b) / 180 * PI);
			vertices[vertexCount].y = cos((a + space) / 180 * PI) * sin((b) / 180 * PI);
			vertices[vertexCount].z = cos((b) / 180 * PI);

			//textureCoords[vertexCount].x = (2 * b) / 360;
			//textureCoords[vertexCount].y = (a + space) / 180;

			v = normalize(vertices[vertexCount]);
			textureCoords[vertexCount].x = (float)(atan2(v.y, v.z)/(PI*2)) + 0.5f;
			textureCoords[vertexCount].y = (float)(asinf(-v.x) / PI) * 2;

			//textureCoords[vertexCount].x = (2 * b) / 360;//(asin(vertices[vertexCount].y) - acos(vertices[vertexCount].z)) / 360.0f;
			//textureCoords[vertexCount].y = -vertices[vertexCount].x;

			vertexCount++;

			//fourth point
			vertices[vertexCount].x = sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].y = cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI);
			vertices[vertexCount].z = cos((b + space) / 180 * PI);

			//textureCoords[vertexCount].x = (2 * (b + space)) / 360;
			//textureCoords[vertexCount].y = (a + space) / 180;

			v = normalize(vertices[vertexCount]);
			textureCoords[vertexCount].x = (float)(atan2(v.y, v.z)/(PI*2)) + 0.5f;
			textureCoords[vertexCount].y = (float)(asinf(-v.x) / PI) * 2;	

			//textureCoords[vertexCount].x = (2 * (b + space)) / 360;//(asin(vertices[vertexCount].y) - acos(vertices[vertexCount].z)) / 360.0f;
			//textureCoords[vertexCount].y = -vertices[vertexCount].x;

			vertexCount++;
		}
	}

		// Fix the problem at the seam
	for (int i=0; i < vertexCount-2; i++)
	{
		if (textureCoords[i].x - textureCoords[i+1].x  > 0.9f)
			textureCoords[i+1].x  += 1.0f;

		if (textureCoords[i+1].x - textureCoords[i].x  > 0.9f)
			textureCoords[i].x  += 1.0f;

		if (textureCoords[i].x - textureCoords[i+2].x  > 0.9f)
			textureCoords[i+2].x  += 1.0f;

		if (textureCoords[i+2].x - textureCoords[i].x  > 0.9f)
			textureCoords[i].x  += 1.0f;

		if (textureCoords[i+1].x - textureCoords[i+2].x  > 0.9f)
			textureCoords[i+2].x  += 1.0f;

		if (textureCoords[i+2].x - textureCoords[i+1].x  > 0.9f)
			textureCoords[i+1].x  += 1.0f;

		if (textureCoords[i].y - textureCoords[i+1].y  > 0.9f)
			textureCoords[i+1].y  += 1.0f;

		if (textureCoords[i+1].y - textureCoords[i].y  > 0.9f)
			textureCoords[i].y  += 1.0f;

		if (textureCoords[i].y - textureCoords[i+2].y  > 0.9f)
			textureCoords[i+2].y  += 1.0f;

		if (textureCoords[i+2].y - textureCoords[i].y  > 0.9f)
			textureCoords[i].y  += 1.0f;

		if (textureCoords[i+1].y - textureCoords[i+2].y  > 0.9f)
			textureCoords[i+2].y  += 1.0f;

		if (textureCoords[i+2].y - textureCoords[i+1].y  > 0.9f)
			textureCoords[i+1].y  += 1.0f;
	}

	glBegin (GL_TRIANGLE_STRIP);

		for (int i = 0; i < vertexCount; i++)
		{
			SVector3 normal = normalize(vertices[i]);

			glNormal3f(normal.x,normal.y,normal.z);

			glMultiTexCoord2f(GL_TEXTURE1_ARB,textureCoords[i].x + offy,textureCoords[i].y);
			glMultiTexCoord2f(GL_TEXTURE2_ARB,textureCoords[i].x,textureCoords[i].y);
			glVertex3f (vertices[i].x, vertices[i].y, -vertices[i].z);
		}

		offy+= 0.02 * CTimer::getInstance()->animBaseSpeed();


	glEnd();

}

void orthoBorder(int lineWidth, int squareOrthoSize)
{
	lineWidth = 1;
	int offey = 0;

	if(squareOrthoSize == 1024) offey = 1;
	/*glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,lineWidth);
	glVertex2f(squareOrthoSize,lineWidth);
	glVertex2f(squareOrthoSize,0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(0,squareOrthoSize - lineWidth);
	glVertex2f(0,squareOrthoSize);
	glVertex2f(squareOrthoSize,squareOrthoSize);
	glVertex2f(squareOrthoSize,squareOrthoSize - lineWidth);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,squareOrthoSize);
	glVertex2f(lineWidth,squareOrthoSize);
	glVertex2f(lineWidth,0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(squareOrthoSize - lineWidth,0);
	glVertex2f(squareOrthoSize - lineWidth,squareOrthoSize);
	glVertex2f(squareOrthoSize,squareOrthoSize);
	glVertex2f(squareOrthoSize,0);
	glEnd();*/

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glLineWidth(lineWidth);

	glBegin(GL_QUADS);
	glVertex2f(0,0 + offey);
	glVertex2f(0,squareOrthoSize);
	glVertex2f(squareOrthoSize - offey,squareOrthoSize);
	glVertex2f(squareOrthoSize - offey,0 + offey);
	glEnd();

	glLineWidth(1);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}