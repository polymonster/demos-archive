#include "main.h"

void jumpRoutine(){

	//jump routine if player has jumped
	if(cameraMode == 1) //if camera mode is "walk"
	{
		if((jumping) && (ypos <=15)) // player is jumping and has not reached the peak of their jump
		{
			ypos++; //increase the ypos
			if((ypos == 15)) //when the player reaches the max of their jump
			{
				jumping = false;//set jumping to false
			}
		}

		if((!jumping) && (ypos > 5))//players not jumping and is off the ground
		{
			ypos--; //move the player down
		}
	}
}

void reshape (int w, int h) {
	//reset the x and y screen size to update where the
	//mouse pointer is warped to the middle of the screen
	xScreenSize = w;
	yScreenSize = h;
	//Resize the window
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void initLights(){
	//Initialise Ligting----------------------------------------
	//SUN
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_LIGHT0);
	//MOON
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glDisable(GL_LIGHT1);
	//STREET LIGHT 1
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,light2_direction); 
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 5.0f);
	//STREET LIGHT 2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION,light3_direction); 
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 5.0f);
	//STREET LIGHT 3
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
	glLightfv(GL_LIGHT4, GL_AMBIENT, light4_ambient);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light4_specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION,light4_direction); 
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 5.0f);
	//STREET LIGHT 4
	glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
	glLightfv(GL_LIGHT5, GL_AMBIENT, light5_ambient);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light5_specular);
	glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION,light5_direction); 
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 5.0f);
	//STREET LIGHT 5
	glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);
	glLightfv(GL_LIGHT6, GL_AMBIENT, light6_ambient);
	glLightfv(GL_LIGHT6, GL_SPECULAR, light6_specular);
	glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION,light6_direction); 
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 5.0f);
	//STREET LIGHT 6
	glLightfv(GL_LIGHT7, GL_DIFFUSE, light7_diffuse);
	glLightfv(GL_LIGHT7, GL_AMBIENT, light7_ambient);
	glLightfv(GL_LIGHT7, GL_SPECULAR, light7_specular);
	glLightfv(GL_LIGHT7, GL_POSITION, light7_position);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 5.0f);
}

void enable (void) {
	//Enable GL features----------------------------------------
	glEnable (GL_DEPTH_TEST); //enable the depth testing
	glEnable (GL_LIGHTING); //enable the lighting
	glEnable(GL_NORMALIZE); //enable normalise for lighting
	glEnable(GL_BLEND); //enable blend for belnding colours
	glShadeModel (GL_SMOOTH); //set the shader to smooth shader
	
	initLights();

	//Construct classes for drawing buildings-------------------
	building = new CBuilding();
	skybox = new CSkyBox();
	roadSystem = new CRoads();

	//Set mouse cursor to none----------------------------------
	glutSetCursor(GLUT_CURSOR_NONE);

}

//SET CAMERA
void camera (void) {
	//Set the camera position
	glRotatef(xrot,1.0,0.0,0.0);  //rotate camera on the x-axis (left and right)
	glRotatef(yrot,0.0,1.0,0.0);  //rotate camera on the y-axis (up and down)
	glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

void display (void) {
	
	//Clear the buffers before drawing
	glClearDepth(1);
	glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLoadIdentity();
	//Translate to camera position
	camera();

	//Reset position and direction of the spotlights so they stay static
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,light2_direction);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION,light3_direction);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION,light4_direction);
	glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION,light5_direction);
	glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION,light6_direction);
	glLightfv(GL_LIGHT7, GL_POSITION, light7_position);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION,light7_direction);
	//-------------------------------------------------------------------

	//Draw Elements of the scene
	roadSystem->drawPlane();
	skybox->drawSkyBox();
	roadSystem->drawRoads();
	building->drawApartments();
	building->drawShops();
	building->drawStreets();
	
	//Reset The Matrix and swap buffers
	glLoadIdentity();
	glutSwapBuffers();

	movement();
	//do the jump routine each loop to move the player if needed
	jumpRoutine();
}

//--------------------------------------------------------------------------------------
//MOUSE AND KEYBOARD CONTROL SYSTEMS
//--------------------------------------------------------------------------------------
//MOUSE - Look and aim camera
//KEY W - Move forward
//KEY S - Move backwards
//KEY A - Strafe left
//KEY D - Strafe right
//--------------------------------------------------------------------------------------
//MOVEMENT
void processKeyUp( unsigned char keyID, int X, int Y ) {

	key[keyID] = false; 
}
void movement()
{
	//Perform movement controls depending on camera mode
	if(cameraMode == 0){	
		//FREE FLY CAMERA MODE-----------------------
		//Allow movement in any direction on any axis
		//-------------------------------------------

		//Forward movement
		if (key['w'] == true)
		{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f); 
		xpos += float(sin(yrotrad)) *2;
		zpos -= float(cos(yrotrad)) *2;
		ypos -= float(sin(xrotrad)) *2;
		}

		//Backwards Movement
		if (key['s'] == true)
		{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f); 
		xpos -= float(sin(yrotrad))*2;
		zpos += float(cos(yrotrad))*2;
		ypos += float(sin(xrotrad))*2;
		}

	}
	else if(cameraMode == 1){
		//WALK CAMERA MODE---------------------------
		//Prevent movement on y-axis so user can not "fly"
		//-------------------------------------------
		ypos = 5; //ypos is always "head height"

		//Forward movement
		if (key['w'] == true)
		{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f); 
		xpos += float(sin(yrotrad)) *2;
		zpos -= float(cos(yrotrad)) *2;
		}

		//Backwards Movement
		if (key['s'] == true)
		{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f); 
		xpos -= float(sin(yrotrad))*2;
		zpos += float(cos(yrotrad))*2;
		}

		//crouch
		if(key['c'] == true)
		{
			ypos = 2; //crouch
		}
		else
		{
			ypos = 5; //ypos is always "head height"
		}
		//jump
		if((key['z'] == true) && (!jumping))
		{
			jumping = true;
		}
	}

		//Strafe controls are the same regardless of camera mode
		//strafe right
		if (key['d'] == true)
		{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += float(cos(yrotrad)) * 2;
		zpos += float(sin(yrotrad)) * 2;
		}


		//strafe left
		if (key['a'] == true)
		{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= float(cos(yrotrad)) * 2;
		zpos -= float(sin(yrotrad)) * 2;
		}

}
//KEYBOARD
void keyboard (unsigned char keyID, int x, int y) {	

	key[keyID] = true;

	//.Escape key to exit
	if (keyID==27)
	{
	exit(0);
	}
}

//MOUSE============================================================================================
void mouseMovement(int x, int y) {
	
	glutSetCursor(GLUT_CURSOR_NONE); //set no cyrsor so its hidden when moving camera

	//check for movement to calculate viewing angle
	int diffx=(x-lastx)/2; //check the difference between the current x and the last x position
	int diffy=(y-lasty)/2; //check the difference between the current y and the last y position
	
	//warp the mouse pointer so its always in the center of the screen for controlling the camera
	if((x != xScreenSize / 2) || (y != yScreenSize / 2))
	{
		glutWarpPointer(xScreenSize / 2,yScreenSize / 2);
		x = xScreenSize / 2;
		y = yScreenSize / 2;
	}
	
	//Set last positions to current positions
	lastx=x; //set lastx to the current x position
	lasty=y; //set lasty to the current y position

	//increment the xrotation and yrotation positions
	xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
	yrot += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position

	//Prevent the user looking beyond 90 degrees, so mouse movements and viewing is not inverted
	if(xrot > 90) xrot = 90;
	if(xrot < -90) xrot = -90;	
}

//----------------------------------------------------------------------------
//MENU STRUCTURE FOR USER-DEFINED OPTIONS
//----------------------------------------------------------------------------

void menuSystem(int menuChoice){

	if(menuChoice == 1) 
	{
		cameraMode = 0;
		ypos = 5; //ypos is always "head height"
	}

	if(menuChoice == 2) cameraMode = 1;

	if(menuChoice == 3) //DAY
	{
		//disable street lights
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
		//enable sunlight
		glEnable(GL_LIGHT0);
	}

	if(menuChoice == 4) //NIGHT
	{
		//disable sunlight
		glDisable(GL_LIGHT0);
		//enable street lights
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glEnable(GL_LIGHT6);
		glEnable(GL_LIGHT7);
	}

	//exit menu choice
	if(menuChoice == 0) exit(0);

}

//----------------------------------------------------------------------------
//DELETE FUNCTION TO FREE MEMORY SPACE
//----------------------------------------------------------------------------
void deleteObjects()
{
	delete building;
	delete roadSystem;
	delete skybox;
}

int main(int argc, char **argv){
    
	//Set up openGL scene
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); 
	glutInitWindowSize (xScreenSize,yScreenSize); 
	glutInitWindowPosition (100, 100);
    glutCreateWindow ("A Little 3d World"); 
	glutWarpPointer(xScreenSize/2,yScreenSize/2);
	enable();
    
	//Display idle and reshape callback functions
	glutDisplayFunc (display); 
	glutIdleFunc (display); 
	glutReshapeFunc (reshape); 

	//HUMAN INTERFACE CALLBACKS
	glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	glutKeyboardFunc (keyboard); 
	glutKeyboardUpFunc(processKeyUp);

	//MENU SYSTEM
	glutCreateMenu(menuSystem);
	glutAddMenuEntry("Fly", 1);
	glutAddMenuEntry("Walk", 2);
	glutAddMenuEntry("Day", 3);
	glutAddMenuEntry("Night", 4);
	glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Main
	glutMainLoop (); 

	//Delete objects to free up memeory space
	deleteObjects();
    return 0;
}


