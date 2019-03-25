#include "globals.h"

class CRoads{

private:

unsigned int junctionTexture;
unsigned int cornerTexture;
unsigned int horizontalTexture;
unsigned int verticalTexture;
unsigned int cornerTexture2;
unsigned int grassTexture;

//Quadratics for Complex shapes
GLUquadricObj *quadratic;

	//MATERIAL PROPERTIES-----------------------------------------------
	float metal_ambient[3];
	float metal_diffuse[3];
	float metal_specular[3];
	float metal_shininess;

public:

CRoads();
~CRoads();
void loadTextures();
void drawPlane();
void drawRoads();
void drawHorizontalStreet(unsigned int endTexture,int zpos,int mode);
void drawVerticalStreet(int xpos);
void drawLampPost();
void drawTrafficLights();
void drawTrafficSigns();


};