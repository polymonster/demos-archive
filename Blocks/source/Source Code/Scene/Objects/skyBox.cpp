#include "skyBox.h"
#include "helpers.h"
#include "textureSingleton.h"
#include "shaderManager.h"

void skyBox()
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

void CSkyBox::render()
{
	int mode;

	glGetIntegerv(GL_CULL_FACE_MODE,&mode);

	if(mode == GL_BACK)
	{
		glCullFace(GL_FRONT);
	}
	else
	{
		glCullFace(GL_BACK);
	}

	CShaderManager::getInstance()->attachShader(g_shaderId.skyBox);

	glActiveTextureARB(GL_TEXTURE7_ARB);
    glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindCubeMap();

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.skyBox), "cubeMap"), 7);

	glPushMatrix();

	glScaled(3000,3000,3000);

	skyBox();

	glPopMatrix();

	CShaderManager::getInstance()->detachShader();

	glCullFace(mode);

}

