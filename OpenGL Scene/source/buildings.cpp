/*
Buildings

This class provides a series of tools to draw and display any of
the buildings and apply their textures.

it also contains algoritms to display house in streets.

*/



#include "buildings.h"

CBuilding::CBuilding()
{
	//initialise the brick texture for the
	//buildings
	brick_ambient[0] = 0.3f;
	brick_ambient[1] = 0.3f;
	brick_ambient[2] = 0.3f;

	brick_diffuse[0] = 0.20f;
	brick_diffuse[1] = 0.20f;
	brick_diffuse[2] = 0.20f;

	brick_specular[0] = 0.05f;
	brick_specular[1] = 0.05f;
	brick_specular[2] = 0.05f;

	brick_shininess = 1.0f;

	//load textures
	loadTextures();

}

CBuilding::~CBuilding(){

	delete &brickTexture;
	delete &houseTexture;
	delete &roofTexture;
	delete &gableTexture;
	delete &apartmentFrontTexture;
	delete &apartmentSideTexture;
	delete &apartmentRoofTexture;
	delete &shopFrontTexture;
	delete &shopSideTexture;
	delete &houseTexture2;
	delete &roofTexture2;
	delete &fenceTexture;
}

void CBuilding::loadTextures(){
	
	//BRICKS--------------------------------------------------------------------------------------------
	unsigned char *brickImage;
	int brickWidth, brickHeight;

	brickImage = LoadBMP("Textures/bricks.bmp",&brickWidth,&brickHeight,true,0,0,0);

	glGenTextures(1, &brickTexture);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, brickWidth, brickHeight, GL_RGBA, GL_UNSIGNED_BYTE,brickImage);

	free(brickImage);

	//HOUSE 1----------------------------------------------------------------------------------
	//front
	unsigned char *houseImage;
	int houseWidth, houseHeight;

	houseImage = LoadBMP("Textures/housefront.bmp",&houseWidth,&houseHeight,true,0,0,0);

	glGenTextures(1, &houseTexture);
	glBindTexture(GL_TEXTURE_2D, houseTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, houseWidth, houseHeight, GL_RGBA, GL_UNSIGNED_BYTE,houseImage);

	free(houseImage);

	//roof
	unsigned char *roofImage;
	int roofWidth, roofHeight;

	roofImage = LoadBMP("Textures/RoofTiles.bmp",&roofWidth,&roofHeight,true,0,0,0);

	glGenTextures(1, &roofTexture);
	glBindTexture(GL_TEXTURE_2D, roofTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, roofWidth, roofHeight, GL_RGBA, GL_UNSIGNED_BYTE,roofImage);

	free(roofImage);

	//gables
	unsigned char *gableImage;
	int gableWidth, gableHeight;

	gableImage = LoadBMP("Textures/gable.bmp",&gableWidth,&gableHeight,true,0,0,0);

	glGenTextures(1, &gableTexture);
	glBindTexture(GL_TEXTURE_2D, gableTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, gableWidth, gableHeight, GL_RGBA, GL_UNSIGNED_BYTE,gableImage);

	free(gableImage);

	//APARTMENTS---------------------------------------------------------------------------------------
	//side texture
	unsigned char *apSideImage;
	int apSideWidth, apSideHeight;

	apSideImage = LoadBMP("Textures/apSide.bmp",&apSideWidth,&apSideHeight,true,0,0,0);

	glGenTextures(1, &apartmentSideTexture);
	glBindTexture(GL_TEXTURE_2D, apartmentSideTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, apSideWidth, apSideHeight, GL_RGBA, GL_UNSIGNED_BYTE,apSideImage);

	free(apSideImage);

	//front and back texture
	unsigned char *apImage;
	int apWidth, apHeight;

	apImage = LoadBMP("Textures/apFront.bmp",&apWidth,&apHeight,true,0,0,0);

	glGenTextures(1, &apartmentFrontTexture);
	glBindTexture(GL_TEXTURE_2D, apartmentFrontTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, apWidth, apHeight, GL_RGBA, GL_UNSIGNED_BYTE,apImage);

	free(apImage);

	//SHOPS-------------------------------------------------------------------------------------------

	unsigned char *shopImage;
	int shopWidth, shopHeight;

	shopImage = LoadBMP("textures/Shops1.bmp",&shopWidth,&shopHeight,true,0,0,0);

	glGenTextures(1, &shopFrontTexture);
	glBindTexture(GL_TEXTURE_2D, shopFrontTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, shopWidth, shopHeight, GL_RGBA, GL_UNSIGNED_BYTE,shopImage);

	free(shopImage);

	//HOUSE TYPE 2
	//roof
	unsigned char *roofImage2;
	int roofWidth2, roofHeight2;

	roofImage2 = LoadBMP("Textures/RoofTiles2.bmp",&roofWidth2,&roofHeight2,true,0,0,0);

	glGenTextures(1, &roofTexture2);
	glBindTexture(GL_TEXTURE_2D, roofTexture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, roofWidth2, roofHeight2, GL_RGBA, GL_UNSIGNED_BYTE,roofImage2);

	free(roofImage2);

	//WOODEN FENCE
	unsigned char *fenceImage;
	int fenceWidth, fenceHeight;

	fenceImage = LoadBMP("Textures/woodFence.bmp",&fenceWidth, &fenceHeight,true,0,0,0);

	glGenTextures(1, &fenceTexture);
	glBindTexture(GL_TEXTURE_2D, fenceTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, fenceWidth, fenceHeight, GL_RGBA, GL_UNSIGNED_BYTE,fenceImage);

	free(fenceImage);

	//GARAGE DOOR
	unsigned char *garageImage;
	int garageWidth, garageHeight;

	garageImage = LoadBMP("Textures/garageDoor.bmp",&garageWidth, &garageHeight,true,0,0,0);

	glGenTextures(1, &garageTexture);
	glBindTexture(GL_TEXTURE_2D, garageTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, garageWidth, garageHeight, GL_RGBA, GL_UNSIGNED_BYTE,garageImage);

	free(garageImage);

	glDisable(GL_TEXTURE_2D);
}

void CBuilding::drawApartments(){

	
	glPushMatrix();
	glTranslated(40,40,-100);
	glScaled(30,40,10);
	drawCubes(apartmentSideTexture,apartmentSideTexture,brickTexture,brickTexture,apartmentFrontTexture,apartmentFrontTexture);
	glPopMatrix();
}

void CBuilding::drawShops(){	
	//Load up brick material 
	glMaterialfv(GL_FRONT, GL_AMBIENT, brick_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brick_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, brick_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, brick_shininess);

	glPushMatrix();
	glTranslated(-40,10,-100);
	glScaled(30,10,10);
	//Main Shop Building
	drawCubes(brickTexture,brickTexture,brickTexture,brickTexture,shopFrontTexture,brickTexture);
	//Shop Roof
	glEnable(GL_TEXTURE_2D);
		//right gable
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		glBegin(GL_TRIANGLES);
			glNormal3f(1.0,0.0,0.0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f,1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f,-1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.7f,-1.0f);
		glEnd();
		//left gable
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		glBegin(GL_TRIANGLES);

			glNormal3f(-1.0,0.0,0.0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.7f,-1.0f);
		glEnd();
		//back face
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		glBegin(GL_QUADS);
			glNormal3f(0.0,0.0,-1.0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.7f,-1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.7f,  -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glEnd();
		glTranslated(0,1.2,0);
		//roof slant
		glBindTexture(GL_TEXTURE_2D, roofTexture2);
		glBegin(GL_QUADS);
			glNormal3f(0.5,0.5,0.0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.1f, -0.3f, 1.4f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.1f, -0.3f,1.4f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.1f, 0.5f,  -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.1f, 0.5f, -1.0f);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

void CBuilding::houseFence(){

glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, fenceTexture);
// Right face
glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f,  0.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f,  0.0f, -1.0f);
glEnd();
// Left Face
glBegin(GL_QUADS);
glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  0.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  0.0f, -1.0f);
glEnd();
// Back Face
glBegin(GL_QUADS);
glNormal3f( 0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  0.0f,  -1.0f);
glEnd();
glDisable(GL_TEXTURE_2D);


}
void CBuilding::drawHouse(int type) {
	//Load up brick material 
	glMaterialfv(GL_FRONT, GL_AMBIENT, brick_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brick_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, brick_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, brick_shininess);
	
	if(type == 0){ //HOUSE TYPE 1
	glPushMatrix();
		glScaled(10,10,10);	
		//Main Part of House
		drawCubes(brickTexture,brickTexture,brickTexture,brickTexture,houseTexture,brickTexture);
		//House Roof
		drawRoof(roofTexture,gableTexture);
		//fence for garden
		glTranslated(0,0,-2);
		houseFence();
	glPopMatrix();
	glPushMatrix();
		//Chimney
		glTranslated(7,12,0);
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		drawCubes(brickTexture,brickTexture,brickTexture,brickTexture,brickTexture,brickTexture);
	glPopMatrix();
	} else if(type == 1) { //HOUSE TYPE 2
		glPushMatrix();
			glScaled(15,10,10);	
			//Main Part of House
			drawCubes(brickTexture,brickTexture,brickTexture,brickTexture,houseTexture,brickTexture);
			//House Roof
			drawRoof(roofTexture2,gableTexture);
			//fence
			glTranslated(0,0,-2);
			houseFence();
		glPopMatrix();
		glPushMatrix();
		//PORCH	
		glTranslated(0,-6,14);
			glScaled(7.5,4,4);
			drawCubes(brickTexture,brickTexture,brickTexture,brickTexture,brickTexture,brickTexture);
			//roof
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, roofTexture2);
			glBegin(GL_QUADS);
				glNormal3f( 0.5f, 0.5f, 1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.5f,-1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.5f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	
			glEnd();
			//right gable
			glBindTexture(GL_TEXTURE_2D, brickTexture);
			glBegin(GL_TRIANGLES);
				glNormal3f(1.0,0.0,0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f,1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f,-1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.5f,-1.0f);
			glEnd();
			//left gable
			glBindTexture(GL_TEXTURE_2D, brickTexture);
			glBegin(GL_TRIANGLES);
				glNormal3f(-1.0,0.0,0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.5f,-1.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

void CBuilding::drawGarage(){
glPushMatrix();	
	glScaled(7,7,10);
	//Load up brick material 
	glMaterialfv(GL_FRONT, GL_AMBIENT, brick_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brick_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, brick_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, brick_shininess);	
	//Main Part of Garage
	drawCubes(brickTexture,brickTexture,brickTexture,brickTexture,garageTexture,brickTexture);
	//Garage Roof
	drawRoof(roofTexture,brickTexture);
glPopMatrix();
}

void CBuilding::drawCubes(unsigned int leftTexture,unsigned int rightTexture,unsigned int topTexture,unsigned int bottomTexture,unsigned int frontTexture,unsigned int backTexture){

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

void CBuilding::drawRoof(unsigned int tileTexture,unsigned int frontTexture)
{
	glEnable(GL_TEXTURE_2D);
	//Roof and Gables
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, frontTexture);
		glTranslated(0,1,0);
		//Gables
		glBegin(GL_TRIANGLES);
			//Front Gable
			glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 0.0f,  -1.0f);
			glTexCoord2f(0.5f, 0.0f);glVertex3f( 0.0f, 0.5f,  -1.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f, 0.0f,  -1.0f);
			//Back Gable
			glNormal3f( 0.0f, 0.0f,1.0f);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 0.0f,  1.0f);
			glTexCoord2f(0.5f, 0.0f);glVertex3f( 0.0f, 0.5f,  1.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f, 0.0f,  1.0f);
		glEnd();	
		//roof
		glBindTexture(GL_TEXTURE_2D, tileTexture);
		glBegin(GL_QUADS);
			//right slant
			glNormal3f( 0.5f, 0.5f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.5f, -1.2f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.5f,	1.2f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2f, -0.1f,  1.2f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2f, -0.1f, -1.2f);
			//left slant
			glNormal3f( -0.5f, 0.5f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.5f, -1.2f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.5f,	1.2f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.2f, -0.1f,  1.2f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.2f, -0.1f, -1.2f);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void CBuilding::drawStreets()
{
//STREET 1 HOUSES
glPushMatrix();
	glTranslated(60,10,120);
	glRotated(180,0,1,0);
		for(int i = 0;i < 5;i++)
		{
			drawHouse(0);
			glTranslated(30,0,0);
		}
glPopMatrix();
//STREET 2 HOUSES
glPushMatrix();
glTranslated(-67.5,10,55);
	for(i = 0;i < 4;i++)
	{
		drawHouse(1);
		glTranslated(45,0,0);
	}
glPopMatrix();
//STREET 3 HOUSES
//Big Houses
glPushMatrix();
	glRotated(180,0,1,0);
	glTranslated(-67.5,10,35);
	drawHouse(1);
	glTranslated(135,0,0);
	drawHouse(1);
glPopMatrix();
//Small Houses
glPushMatrix();
	glRotated(180,0,1,0);
	glTranslated(-30,10,35);	
	for(i = 0;i < 3;i++)
	{
		drawHouse(0);
		glTranslated(30,0,0);
	}
glPopMatrix();
//GARAGES
glPushMatrix();	
	glTranslated(80,7,120);
	glRotated(180,0,1,0);
	drawGarage();
	glTranslated(160,0,0);
	drawGarage();
glPopMatrix();
}