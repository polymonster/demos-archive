#include "roadways.h"

CRoads::CRoads(){

	//quadratics for cylinders and spheres
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals 
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords
	//load the road and floor textures
	loadTextures();

	//metal material for the lamp posts and traffic lights
	metal_ambient[0] = 0.05f;
	metal_ambient[1] = 0.05f;
	metal_ambient[2] = 0.05f;

	metal_diffuse[0] = 0.05f;
	metal_diffuse[1] = 0.05f;
	metal_diffuse[2] = 0.05f;

	metal_specular[0] = 0.05f;
	metal_specular[1] = 0.05f;
	metal_specular[2] = 0.05f;

	metal_shininess = 5.0f;
}

CRoads::~CRoads(){

	gluDeleteQuadric(quadratic);

	delete &junctionTexture;
	delete &cornerTexture;
	delete &horizontalTexture;
	delete &verticalTexture;
	delete &cornerTexture2;
	delete &grassTexture;
}

void drawCube(unsigned int leftTexture,unsigned int rightTexture,unsigned int topTexture,unsigned int bottomTexture,unsigned int frontTexture,unsigned int backTexture){
glEnable(GL_TEXTURE_2D);
// Back Face
glBindTexture(GL_TEXTURE_2D, backTexture);
glBegin(GL_QUADS);
glNormal3f( 0.0f, 0.0f,-1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
glEnd();
// Top Face
glBindTexture(GL_TEXTURE_2D, topTexture);
glBegin(GL_QUADS);
glNormal3f( 0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
glEnd();
// Bottom Face
glBindTexture(GL_TEXTURE_2D, bottomTexture);
glBegin(GL_QUADS);
glNormal3f( 0.0f,-1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
glEnd();
// Right face
glBindTexture(GL_TEXTURE_2D, rightTexture);
glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f,  1.0f, -1.0f);
glEnd();
// Left Face
glBindTexture(GL_TEXTURE_2D, leftTexture);
glBegin(GL_QUADS);
glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
glEnd();
// Front Face
glBindTexture(GL_TEXTURE_2D, frontTexture);
glBegin(GL_QUADS);
glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
glEnd();
glDisable(GL_TEXTURE_2D);
}

void CRoads::loadTextures(){

	unsigned char *floorImage;
	int floortexturewidth, floortextureheight;

	floorImage = LoadBMP("Textures/grasstexture2.bmp", &floortexturewidth, &floortextureheight,true,0,0,0);

	glGenTextures(1, &grassTexture);
	glBindTexture(GL_TEXTURE_2D, grassTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, floortexturewidth, floortextureheight, GL_RGBA, GL_UNSIGNED_BYTE, floorImage);

	free(floorImage);

	//HORIZONTAL ROAD PIECE

	unsigned char *horizImage;
	int horizWidth, horizHeight;

	horizImage = LoadBMP("Textures/Horiz1.bmp",&horizWidth, &horizHeight,true,0,0,0);

	glGenTextures(1, &horizontalTexture);
	glBindTexture(GL_TEXTURE_2D, horizontalTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, horizWidth, horizHeight, GL_RGBA, GL_UNSIGNED_BYTE,horizImage);

	free(horizImage);

	//JUNCTION ROAD PIECE

	unsigned char *juncImage;
	int juncWidth, juncHeight;

	juncImage = LoadBMP("Textures/Crossing1.bmp",&juncWidth, &juncHeight,true,0,0,0);

	glGenTextures(1, &junctionTexture);
	glBindTexture(GL_TEXTURE_2D, junctionTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, juncWidth, juncHeight, GL_RGBA, GL_UNSIGNED_BYTE,juncImage);

	free(juncImage);

	//CORNER ROAD PIECE 1

	unsigned char *cornerImage;
	int cornerWidth, cornerHeight;

	cornerImage = LoadBMP("Textures/Corner1.bmp",&cornerWidth, &cornerHeight,true,0,0,0);

	glGenTextures(1, &cornerTexture);
	glBindTexture(GL_TEXTURE_2D, cornerTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cornerWidth, cornerHeight, GL_RGBA, GL_UNSIGNED_BYTE,cornerImage);

	free(cornerImage);

	//CORNER ROAD PIECE 2

	unsigned char *cornerImage2;
	int cornerWidth2, cornerHeight2;

	cornerImage2 = LoadBMP("Textures/Corner2.bmp",&cornerWidth2, &cornerHeight2,true,0,0,0);

	glGenTextures(1, &cornerTexture2);
	glBindTexture(GL_TEXTURE_2D, cornerTexture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cornerWidth2, cornerHeight2, GL_RGBA, GL_UNSIGNED_BYTE,cornerImage2);

	free(cornerImage2);
}

void CRoads::drawPlane(void){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grassTexture);

	glPushMatrix();
	glTranslated(0.0,-3,0.0);
	
	int d = 160;		// floor size (-d,-d) -> (d,d)
	int tile_size =	10;	// dimension of each tile

	for (int i=-d;i<d;i++) {
		for (int j=-d;j<d;j++) {

			double startx = (double)(i)/tile_size;
			double starty = (double)(j)/tile_size;
			double finishx = (double)(i+1)/tile_size;
			double finishy = (double)(j+1)/tile_size;

			glNormal3f(0.0,1.0,0.0);
			glBegin(GL_POLYGON);
				glTexCoord2d(startx, starty);glVertex3d(i,0.0,j);
				glTexCoord2d(startx, finishy);glVertex3d(i,0.0,j+1);
				glTexCoord2d(finishx, finishy);glVertex3d(i+1,0.0,j+1);
				glTexCoord2d(finishx, starty);glVertex3d(i+1,0.0,j);
			glEnd();
		}
	}

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void CRoads::drawHorizontalStreet(unsigned int endTexture, int zpos,int mode){
//HORIZONTAL STREET
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslated(-110,0,zpos);
	glNormal3f(0.0,1.0,0.0);

		//Left Corner
		glBindTexture(GL_TEXTURE_2D, endTexture);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 1.0f);glVertex3d(0,0,0);
			glTexCoord2f(1.0f, 1.0f);glVertex3d(20,0,0);
			glTexCoord2f(1.0f, 0.0f);glVertex3d(20,0,20);
			glTexCoord2f(0.0f, 0.0f);glVertex3d(0,0,20);
		glEnd();
		glTranslated(20,0,0);
		//Main Strip of road
		glBindTexture(GL_TEXTURE_2D, horizontalTexture);
		for(int i = 0;i < 9;i++)
		{
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f);glVertex3d(0,0,0);
				glTexCoord2f(1.0f, 0.0f);glVertex3d(20,0,0);
				glTexCoord2f(1.0f, 1.0f);glVertex3d(20,0,20);
				glTexCoord2f(0.0f, 1.0f);glVertex3d(0,0,20);
			glEnd();
			glTranslated(20,0,0);
		}


		//Right Corner
		//mode effects the way the texture points due to a slight bug the
		//texture for one corner would always appear the wrong way so the 
		//else if statement is required to rectify this
		glBindTexture(GL_TEXTURE_2D, endTexture);
		if(mode == 0)
		{
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);glVertex3d(0,0,0);
			glTexCoord2f(0.0f, 1.0f);glVertex3d(20,0,0);
			glTexCoord2f(1.0f, 1.0f);glVertex3d(20,0,20);
			glTexCoord2f(1.0f, 0.0f);glVertex3d(0,0,20);
		glEnd();
		}
		else if(mode == 1){
		glPushMatrix();
			glRotated(90,0,1,0);
			glTranslated(-20,0,0);
			glBegin(GL_POLYGON);
				glTexCoord2f(1.0f, 0.0f);glVertex3d(0,0,0);
				glTexCoord2f(1.0f, 1.0f);glVertex3d(20,0,0);
				glTexCoord2f(0.0f, 1.0f);glVertex3d(20,0,20);
				glTexCoord2f(0.0f, 0.0f);glVertex3d(0,0,20);
			glEnd();
		glPopMatrix();
		}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

void CRoads::drawVerticalStreet(int xpos){
	//VERTICAL STREET
	glPushMatrix();
	glTranslated(xpos,0,-60);
	glEnable(GL_TEXTURE_2D);
	//Main Strip of road	
	glBindTexture(GL_TEXTURE_2D, horizontalTexture);
	for(int i = 0;i < 2;i++){ 
		for(int j = 0;j < 3;j++){
			glBegin(GL_POLYGON);
			glNormal3f(0.0,1.0,0.0);
				glTexCoord2f(1.0f, 0.0f);glVertex3d(0,0,0);
				glTexCoord2f(1.0f, 1.0f);glVertex3d(20,0,0);
				glTexCoord2f(0.0f, 1.0f);glVertex3d(20,0,20);
				glTexCoord2f(0.0f, 0.0f);glVertex3d(0,0,20);
			glEnd();
			glTranslated(0,0,20);
		}
		glTranslated(0,0,20);//skip the "junction" tile drawn in "horizontal road"
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void CRoads::drawLampPost()
{
	//Load up metal material 
	glMaterialfv(GL_FRONT, GL_AMBIENT, metal_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, metal_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, metal_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, metal_shininess);

	glPushMatrix();
		glColor3f(0.4,0.4,0.4);	
		//top "junction"
		glTranslated(0,25,0);
		gluSphere(quadratic,0.7f,32,32);
		//horizontal bar
		gluCylinder(quadratic,0.7f,0.7f,10.0f,32,32);
		//main lamp post shaft	
	glPushMatrix();	
		glRotated(90,1,0,0);
		gluCylinder(quadratic,0.7f,0.7f,25.0f,32,32);
	glPopMatrix();
		//end curve
		glTranslated(0,0,10);
		gluSphere(quadratic,0.7f,32,32);
		//bulb
		glColor3f(1.0,0.5,0);
		glTranslated(0,-0.7,-6);
		gluCylinder(quadratic,0.7f,0.7f,5.0f,32,32);
		gluSphere(quadratic,0.7f,32,32);
		glTranslated(0,0,5);
		gluSphere(quadratic,0.7f,32,32);
	glPopMatrix();
}

void CRoads::drawTrafficLights()
{		
	//Load up metal material 
	glMaterialfv(GL_FRONT, GL_AMBIENT, metal_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, metal_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, metal_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, metal_shininess);
	
	glTranslated(0,15,0);

	//main traffic light shaft
	glPushMatrix();
		glColor3f(0.4,0.4,0.4);
		glRotated(90,1,0,0);
		gluCylinder(quadratic,0.7f,0.7f,15.0f,32,32);
	glPopMatrix();

	//The Bulbs (one set for each side)
		glPushMatrix();
			//Red
			glTranslated(0,1.5,-1.5);
			glColor3f(0.8,0.2,0.2);
			gluSphere(quadratic,0.7f,32,32);
			//Amber
			glTranslated(0,1.5,0);
			glColor3f(1.0,0.5,0);
			gluSphere(quadratic,0.7f,32,32);
			//Green
			glTranslated(0,1.5,0);
			glColor3f(0.2,0.8,0.2);
			gluSphere(quadratic,0.7f,32,32);
		glPopMatrix();		

	//The box the lights 'sit' in
	glPushMatrix();
		glTranslated(0,3,0);
		glColor3f(0.2,0.2,0.2);
		glScaled(1.5,3,1.5);
		drawCube(NULL,NULL,NULL,NULL,NULL,NULL);
	glPopMatrix();
}

void CRoads::drawTrafficSigns()
{
	//north facing lamps
	glPushMatrix();	
		glTranslated(-85,0,-85);
		drawLampPost();
		glTranslated(170,0,80);
		drawLampPost();
		glTranslated(-170,0,80);
		drawLampPost();
	glPopMatrix();

	//south facing lamps
	glPushMatrix();
		//rotate 180 degrees to face south
		glRotated(180,0,1,0);
		glTranslated(-85,0,-105);
		drawLampPost();
		glTranslated(170,0,80);
		drawLampPost();
		glTranslated(-170,0,80);
		drawLampPost();
	glPopMatrix();

	//north facing traffic lights
	glPushMatrix();
		glTranslated(85,0,25);
		drawTrafficLights();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-115,0,25);
		drawTrafficLights();
	glPopMatrix();

	//south facing traffic lights
	glPushMatrix();
		glTranslated(-85,0,-5);
		//rotate 180 degrees to face south
		glRotated(180,0,1,0);
		drawTrafficLights();
	glPopMatrix();

	glPushMatrix();
		glTranslated(115,0,-5);
		//rotate 180 degrees to face south
		glRotated(180,0,1,0);
		drawTrafficLights();
	glPopMatrix();
}

void CRoads::drawRoads(){

	/*
	===================<----Street 1
	=				  =		
	=				  =
	=				  =
	===================<----Street 2
	=				  =		
	=				  =
	=				  =
	===================<-----Street 3
	^				  ^
	|				  |
	Street 4		  Street 5	
	*/
	
	glTranslated(0,-2.9,0); //translate the roads to just above the main floor plane

	drawHorizontalStreet(cornerTexture2,-80,0); //street 1 with corners
	drawHorizontalStreet(junctionTexture,0,0); // street 2 with junction
	drawHorizontalStreet(cornerTexture,80,1); //street 3 with corners

	drawVerticalStreet(-110); //street 4
	drawVerticalStreet(90);//street 5

	//the street lights and traffic lights do not have textures
	//they have colours so enable colour material
	glEnable(GL_COLOR_MATERIAL);
	drawTrafficSigns(); // lamp posts and traffic lights
	glDisable(GL_COLOR_MATERIAL); //diable colour material for anyting texture based following this

}