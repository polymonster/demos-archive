#include "Helpers.h"
#include "../Input/Input.h"
#include "Fonts.h"

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

void quad()
{
	glBegin(GL_QUADS);
			glVertex2f(0,0);
			glVertex2f(0,-1);
			glVertex2f(1,-1);
			glVertex2f(1,0);
	glEnd();
}

void texturedQuad()
{
	glEnable(GL_TEXTURE_2D);

	glNormal3f(0,0,1);

	glBegin(GL_QUADS);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,0); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex2f(-1,-1);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,0);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);

			glVertex2f(1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex2f(1,1);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,1); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
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
			glNormal3f(boxNormals[i][0].x,boxNormals[i][0].y,boxNormals[i][0].z);
			glVertex3f(boxQuads[i][0].x,boxQuads[i][0].y,boxQuads[i][0].z);

			glNormal3f(boxNormals[i][1].x,boxNormals[i][1].y,boxNormals[i][1].z);
			glVertex3f(boxQuads[i][1].x,boxQuads[i][1].y,boxQuads[i][1].z);

			glNormal3f(boxNormals[i][2].x,boxNormals[i][2].y,boxNormals[i][2].z);
			glVertex3f(boxQuads[i][2].x,boxQuads[i][2].y,boxQuads[i][2].z);

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

SVector3 translateWidgetDepthTest(SVector3 center)
{
	float scale = distance(center,user::getInstance()->input.pos) / 100;
	bool resultBool[3] = {0,0,0};

	glLineWidth(10);

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);
		
		glEnable(GL_DEPTH_TEST);
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

	xline = 0;
	yline = 0;
	zline = 0;

	//z-y (ABOUT X)
	glColor3f(0.5,0,0);
	if(selected.x == 1) glColor3f(1,0.7,0);
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
	if(selected.z == 1) glColor3f(1,0.7,0);
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
}

/*void useWidget(SVector3 center, SVector3 scale, SVector3 rotation, SVector selectedAxis, int transformType)
{
	glEnable(GL_DEPTH_TEST);

	if(!user::getInstance()->input.leftMouse)
	{
		if(user::getInstance()->input.key['T'])
		{
			selectedTranslateAxis = translateWidgetDepthTest(m_center);
		}
		else if(user::getInstance()->input.key['Y'])
		{
			selectedTranslateAxis = translateWidgetDepthTest(m_center);
		}
		else if(user::getInstance()->input.key['U'])
		{
			selectedTranslateAxis = rotateWidgetDepthTest(m_center);
		}
	}
	else
	{
		float scale = distance(m_center,user::getInstance()->input.pos) / 100;

		float incx = user::getInstance()->input.mouseIncrement.x * scale / 10;
		float incy = user::getInstance()->input.mouseIncrement.y * scale / 10;

		double xrot = degreesToRadians(user::getInstance()->input.rot.x);
		double yrot = degreesToRadians(user::getInstance()->input.rot.y);

		if(user::getInstance()->input.key['T'])
		{
			center.x += m_selectedTranslateAxis.x * float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
			center.y -= m_selectedTranslateAxis.y * float( cos(xrot) * incy );
			center.z += m_selectedTranslateAxis.z * float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );
		}
		else if(user::getInstance()->input.key['Y'])
		{
			scale.x += m_selectedTranslateAxis.x * float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
			scale.y -= m_selectedTranslateAxis.y * float( cos(xrot) * incy );
			scale.z += m_selectedTranslateAxis.z * float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );
		}
		else if(user::getInstance()->input.key['U'])
		{
			rotation.x += m_selectedTranslateAxis.x * (incx + incy);
			rotation.y += m_selectedTranslateAxis.y * (incx + incy);
			rotation.z += m_selectedTranslateAxis.z * (incx + incy);
		}
	}

	glClearDepth(1);
	glClearColor (0.6f,0.6f,0.6f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
}*/

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
