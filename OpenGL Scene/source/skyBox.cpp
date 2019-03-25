//CSKYBOX Class=======================================================================
//Responsible for drawing the sky around the scene
//create using a cube with mapped sky textures
//textures are clamped to edge to make the edges of the cube seamless
//and appear as one large sky
//====================================================================================
#include "skyBox.h"

CSkyBox::CSkyBox(){

	//Load up all skybox textures
	loadTextures();
}

CSkyBox::~CSkyBox(){

	//Delete all the created textures
	delete &skyLeftTexture;
	delete &skyRightTexture;
	delete &skyFrontTexture;
	delete &skyBackTexture;
	delete &skyUpTexture;
	delete &skyDownTexture;
}

void CSkyBox::loadTextures(){

	//Load all sky textures, one for each face of a cube
	//Left side of box
	unsigned char *skyleft;
	int skyleftWidth, skyleftHeight;

	skyleft = LoadBMP("Textures/skyleft.bmp",&skyleftWidth,&skyleftHeight,true,0,0,0);

	glGenTextures(1, &skyLeftTexture);
	glBindTexture(GL_TEXTURE_2D, skyLeftTexture);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skyleftWidth,skyleftHeight, GL_RGBA, GL_UNSIGNED_BYTE,skyleft);
	//clamp to edge to make the cube edges "invisible"
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	free(skyleft);

	//Right side of box
	unsigned char *skyright;
	int skyrightWidth, skyrightHeight;

	skyright = LoadBMP("Textures/skyright.bmp",&skyrightWidth,&skyrightHeight,true,0,0,0);

	glGenTextures(1, &skyRightTexture);
	glBindTexture(GL_TEXTURE_2D, skyRightTexture);
	//clamp to edge to make the cube edges "invisible"
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skyrightWidth,skyrightHeight, GL_RGBA, GL_UNSIGNED_BYTE,skyright);

	free(skyright);

	//Front side of box
	unsigned char *skyfront;
	int skyfrontWidth, skyfrontHeight;

	skyfront = LoadBMP("Textures/skyfront.bmp",&skyfrontWidth,&skyfrontHeight,true,0,0,0);

	glGenTextures(1, &skyFrontTexture);
	glBindTexture(GL_TEXTURE_2D, skyFrontTexture);
	//clamp to edge to make the cube edges "invisible"
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skyfrontWidth,skyfrontHeight, GL_RGBA, GL_UNSIGNED_BYTE,skyfront);

	free(skyfront);

	//Back side of box
	unsigned char *skyback;
	int skybackWidth, skybackHeight;

	skyback = LoadBMP("Textures/skyback.bmp",&skybackWidth,&skybackHeight,true,0,0,0);

	glGenTextures(1, &skyBackTexture);
	glBindTexture(GL_TEXTURE_2D, skyBackTexture);
	//clamp to edge to make the cube edges "invisible"
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skybackWidth,skybackHeight, GL_RGBA, GL_UNSIGNED_BYTE,skyback);

	free(skyback);

	//Top side of box
	unsigned char *skyup;
	int skyupWidth, skyupHeight;

	skyup = LoadBMP("Textures/skyup.bmp",&skyupWidth,&skyupHeight,true,0,0,0);

	glGenTextures(1, &skyUpTexture);
	glBindTexture(GL_TEXTURE_2D, skyUpTexture);
	//clamp to edge to make the cube edges "invisible"
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skyupWidth,skyupHeight, GL_RGBA, GL_UNSIGNED_BYTE,skyup);

	free(skyup);

	//Bottom side of box
	unsigned char *skydown;
	int skydownWidth, skydownHeight;

	skydown = LoadBMP("Textures/skydown.bmp",&skydownWidth,&skydownHeight,true,0,0,0);

	glGenTextures(1, &skyDownTexture);
	glBindTexture(GL_TEXTURE_2D, skyDownTexture);
	//clamp to edge to make the cube edges "invisible"
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skydownWidth,skydownHeight, GL_RGBA, GL_UNSIGNED_BYTE,skydown);

	free(skydown);

}

void drawBox(unsigned int leftTexture,unsigned int rightTexture,unsigned int topTexture,unsigned int bottomTexture,unsigned int frontTexture,unsigned int backTexture){
//Draw a cube applying appropriate textures to each face	
glEnable(GL_TEXTURE_2D);
// Back Face
glBindTexture(GL_TEXTURE_2D, backTexture);
glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
glEnd();
// Top Face
glBindTexture(GL_TEXTURE_2D, topTexture);
glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
glEnd();
// Bottom Face
glBindTexture(GL_TEXTURE_2D, bottomTexture);
glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
glEnd();
// Right face
glBindTexture(GL_TEXTURE_2D, rightTexture);
glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f,  1.0f, -1.0f);
glEnd();
// Left Face
glBindTexture(GL_TEXTURE_2D, leftTexture);
glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
glEnd();
// Front Face
glBindTexture(GL_TEXTURE_2D, frontTexture);
glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
glEnd();
glDisable(GL_TEXTURE_2D);
}

void CSkyBox::drawSkyBox(){

	glPushMatrix();	
		glScaled(300,300,300); //Scale the cube so it is large and will engulf the whole scene
		//draw a cube with the input textures
		drawBox(skyLeftTexture,skyRightTexture,skyUpTexture,skyDownTexture,skyBackTexture,skyFrontTexture);
	glPopMatrix();
}
