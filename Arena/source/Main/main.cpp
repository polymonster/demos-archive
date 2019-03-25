#include "main.h"
#include <time.h>
#include "textureSingleton.h"
#include "timerSingleton.h"
#include "windowsThings.h"
#include "player.h"
#include "ModelManager.h"
#include "map.h"
#include "networking.h"
#include "soundSingleton.h"
#include "game.h"

int xScreenSize, yScreenSize;
float aspectRatio;

unsigned int glslDecal;
int glslScale;

CGame *myGameLoop;

bool revert = false;

void networkLoop()
{
	myGameLoop->networkChoice();
}

void reshape (int w, int h) {
	//reset the x and y screen size to update where the
	//mouse pointer is warped to the middle of the screen
	xScreenSize = w;
	yScreenSize = h;

	aspectRatio = (float) xScreenSize / yScreenSize;

	//Resize the window
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
	gluPerspective (60, aspectRatio , 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void initialise(){

	//Enable GL features----------------------------------------
	glEnable (GL_DEPTH_TEST); //enable the depth testing
	glDepthFunc(GL_LEQUAL); //set the depth function
	glEnable (GL_LIGHTING); //enable the lighting
	glEnable(GL_NORMALIZE); //enable normalise for lighting
	glEnable(GL_BLEND); //enable blend for belnding colours
	glShadeModel (GL_SMOOTH); //set the shader to smooth shader
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing

	myGameLoop = new CGame();

	if(revert) myGameLoop->revertSettingsMessage();

	cameraMode = 0;

	aspectRatio = float(xScreenSize / yScreenSize);

	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)networkLoop, NULL, 0, NULL);

}

void updateFrame() {

	CTimer::getInstance()->update();

	myGameLoop->gameUpdateLoop();
}

void display(){

	myGameLoop->gameRenderLoop();

	glutPostRedisplay();// Invoke display callback function
}


void glLoop(int argc, char **argv){

	//register a new user
	user::getInstance()->create();
	user::getInstance()->clearStates();
	
	//first get the preferences file
	ifstream fin;

	fin.open("Data\\Settings\\settings.dat");

	string word;

	while(!fin.eof())
	{
		fin >> word;

		if(word == "resolution")
		{
			fin >> user::getInstance()->settings.screenWidth;
			fin >> word; // "x"
			fin >> user::getInstance()->settings.screenHeight;
			fin >> user::getInstance()->settings.resolutionID;
		}
		else if(word == "shadow_quality")
		{
			fin >> word;
			fin >> user::getInstance()->settings.shadowQualityID;
		}
		else if(word == "texture_quality")
		{
			fin >> word;
			fin >> user::getInstance()->settings.textureQualityID;
		}
	}

	fin.close();
	
	//--------------------------------------------------------------------------
	//INITIALISE SCENE//--------------------------------------------------------
	//--------------------------------------------------------------------------
	//glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);

	//xScreenSize = 1024, yScreenSize = 768;
	//glutInitWindowSize(xScreenSize,yScreenSize);
	//glutCreateWindow("Super Arena!!");
	//glutFullScreen();

	//setup game mode
	xScreenSize = user::getInstance()->settings.screenWidth;
	yScreenSize = user::getInstance()->settings.screenHeight;

	char initString[32];

	sprintf(initString,"%dx%d:32@60",xScreenSize,yScreenSize);

	glutGameModeString(initString); //the settings for fullscreen mode
	if(glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) glutEnterGameMode();
	else
	{
		revert = true;

		glutGameModeString("1024x768:32@60");
		if(glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) glutEnterGameMode();
		else
		{
			glutGameModeString("800x600:16@60");
			glutEnterGameMode();
		}

	}

	//just double check the values in glut game mode and screen dimensions
	//completely match.
	xScreenSize = glutGameModeGet(GLUT_GAME_MODE_WIDTH);
	yScreenSize = glutGameModeGet(GLUT_GAME_MODE_HEIGHT);

	int d;
	cout << "OpenGL - " << glGetString(GL_VERSION) << "\n";
	cout << glGetString(GL_RENDERER) << " - ";
	cout << glGetString(GL_VENDOR) << "\n";
	glGetIntegerv(GL_DEPTH_BITS, &d);
	cout << "Depth Buffer " << d << "\n";
	glGetIntegerv(GL_ALPHA_BITS, &d);
	cout << "Alpha Buffer " << d << "\n";
	glGetIntegerv(GL_STENCIL_BITS, &d);
	cout << "Stencil Buffer " << d << "\n";
	cout << "----------------------\n";
	cout << "Initialising Game\n";
	cout << "----------------------\n";

	//--------------------------------------------------------------------------
	//CALLBACK FUNCTIONS//------------------------------------------------------
	//--------------------------------------------------------------------------
	initialise();

	//DISPLAY
	//glutTimerFunc(update_interval, updateFrame, 1);
	glutDisplayFunc(display); 
	glutIdleFunc(updateFrame);
	glutReshapeFunc (reshape); 

	//HUMAN INTERFACE CALLBACKS
	glutPassiveMotionFunc(readPassiveMouse); //check for mouse movement
	glutMotionFunc(readActiveMouse); //check for mouse movement
	glutMouseFunc(readMouseButton); //buttons
	glutKeyboardFunc(readKeyboard);
	glutKeyboardUpFunc(readKeyUp);
	glutSpecialFunc(specialFunc);
	glutSpecialUpFunc(specialUpFunc);

	//--------------------------------------------------------------------------
	//MAIN//--------------------------------------------------------------------
	//--------------------------------------------------------------------------
	glutMainLoop(); 
	//deleteObjs
}

int main(int argc, char **argv){

	glHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) glLoop, NULL, 0, NULL);
	WaitForSingleObject(glHandle, INFINITE);
    return 0;
}




