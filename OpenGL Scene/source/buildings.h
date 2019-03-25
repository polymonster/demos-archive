#include "globals.h"

class CBuilding{
private:

	//MATERIAL PROPERTIES-----------------------------------------------
	float brick_ambient[3];
	float brick_diffuse[3];
	float brick_specular[3];
	float brick_shininess;

	//TEXTURE VARIABLES-------------------------------------------------
	//Generic textures
	unsigned int floortexture;
	unsigned int brickTexture;
	//house 1 textures
	unsigned int houseTexture;
	unsigned int roofTexture;
	unsigned int gableTexture;
	//apartemnt textures
	unsigned int apartmentFrontTexture;
	unsigned int apartmentSideTexture;
	unsigned int apartmentRoofTexture;
	//shop textures
	unsigned int shopFrontTexture;
	unsigned int shopSideTexture;
	//house 2 testures
	unsigned int houseTexture2;
	unsigned int roofTexture2;
	//fence texture
	unsigned int fenceTexture;
	//garage texture
	unsigned int garageTexture;

public:

CBuilding();
~CBuilding();

void drawRoof(unsigned int tileTexture,unsigned int frontTexture);
void drawCubes(unsigned int leftTexture,unsigned int rightTexture,unsigned int topTexture,unsigned int bottomTexture,unsigned int frontTexture,unsigned int backTexture);

void drawShops();
void drawApartments();
void loadTextures();
void drawHouse(int type);
void drawGarage();
void drawStreets();
void houseFence();


};

