#include "buildings.h"
#include "skyBox.h"
#include "roadways.h"

//GLOBAL VARIABLES-------------------------------------------------
//For the movement system
float xpos = 0, ypos = 10, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;
bool jumping;
//Screen varaibles
int xScreenSize = 1024;
int yScreenSize = 768;
int cameraMode = 0;

//LIGHTING VARIABLES-----------------------------------------------
//SUNLIGHT
float light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
float light0_ambient[] = {1.0, 1.0, 1.0, 1.0};
float light0_specular[] ={1.0, 1.0, 1.0, 1.0};
float light0_position[] ={0,0,0,1};
//NIGHT TIME
float light1_diffuse[] = {0.3, 0.3, 0.3, 0.3};
float light1_ambient[] = {0.3, 0.3, 0.3, 0.3};
float light1_specular[] ={0.3, 0.3, 0.3, 0.3};
float light1_position[] ={0,0,0,1};

//STREET LIGHTS
//LAMP POST 1
float light2_diffuse[] = {0.7, 0.5, 0.0, 1};
float light2_ambient[] = {0.7, 0.5, 0.0, 1};
float light2_specular[] ={0.7, 0.5, 0.0, 1};
float light2_position[] ={-85,22,-77,1};
float light2_direction[] = {0.0,-1.0,0.0};

//LAMP POST 2
float light3_diffuse[] = {0.7, 0.5, 0.0, 1};
float light3_ambient[] = {0.7, 0.5, 0.0, 1};
float light3_specular[] ={0.7, 0.5, 0.0, 1};
float light3_position[] ={85,22,5,1};
float light3_direction[] = {0.0,-1.0,0.0};

//LAMP POST 3
float light4_diffuse[] = {0.7, 0.5, 0.0, 1};
float light4_ambient[] = {0.7, 0.5, 0.0, 1};
float light4_specular[] ={0.7, 0.5, 0.0, 1};
float light4_position[] ={-85,22,85,1};
float light4_direction[] = {0.0,-1.0,0.0};

//LAMP POST 4
float light5_diffuse[] = {0.7, 0.5, 0.0, 1};
float light5_ambient[] = {0.7, 0.5, 0.0, 1};
float light5_specular[] ={0.7, 0.5, 0.0, 1};
float light5_position[] ={85,22,95,1};
float light5_direction[] = {0.0,-1.0,0.0};

//LAMP POST 5
float light6_diffuse[] = {0.7, 0.5, 0.0, 1};
float light6_ambient[] = {0.7, 0.5, 0.0, 1};
float light6_specular[] ={0.7, 0.5, 0.0, 1};
float light6_position[] ={-85,22,15,1};
float light6_direction[] = {0.0,-1.0,0.0};

//LAMP POST 6
float light7_diffuse[] = {0.7, 0.5, 0.0, 1};
float light7_ambient[] = {0.7, 0.5, 0.0, 1};
float light7_specular[] ={0.7, 0.5, 0.0, 1};
float light7_position[] ={85,22,-65,1};
float light7_direction[] = {0.0,-1.0,0.0};

bool key[256];

void movement();

CBuilding *building;
CSkyBox *skybox;
CRoads *roadSystem;


