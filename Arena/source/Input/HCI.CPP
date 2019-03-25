#include "HCI.h"
#include "outputText.h"
#include "textureSingleton.h"
#include "timerSingleton.h"

extern int xScreenSize, yScreenSize;

COutput out;
SControls con;

CHCI::CHCI(){

	con.newypos = 1000;
	createUserSingleton();
}

void CHCI::createUserSingleton()
{
	//user::getInstance()->create();
}

void CHCI::updateUserSingleton()
{
	alignMouseToAspect();
	user::getInstance()->input = con;
}

void CHCI::resyncUserSingleton(){

	con = user::getInstance()->input;

}

void CHCI::setStart(float y, float yrot)
{
	con.newypos = y;
	con.ypos = y;
	con.yrot = yrot;
}

void CHCI::readKeyboard(unsigned char keyID, int x, int y){

	con.key[keyID] = true;

	if(con.key[27]) exit(0);

}
void CHCI::readKeyUp(unsigned char keyID, int x, int y){
	
	con.key[keyID] = false;

}
void CHCI::readPassiveMouse(int xpos, int ypos){

	con.mousex = xpos;
	con.mousey = ypos;

}
void CHCI::readActiveMouse(int xpos, int ypos){
	
	con.mousex = xpos;
	con.mousey = ypos;
}

void CHCI::alignMouseToAspect(){

	//float xratio = float(1024.0f / xScreenSize);
	//float yratio = float(768.0f / yScreenSize);

	float xratio = 1;
	float yratio = 1;

	con.interfaceMousex = con.mousex * xratio;
	con.interfaceMousey = con.mousey * yratio;
}

void CHCI::specialFunc(int key, int x, int y){

	if(key == GLUT_KEY_UP) con.specialKey[0] = true;
	if(key == GLUT_KEY_DOWN) con.specialKey[1] = true;
	if(key == GLUT_KEY_LEFT) con.specialKey[2] = true;
	if(key == GLUT_KEY_RIGHT) con.specialKey[3] = true;
	if(key == GLUT_KEY_F1) con.specialKey[4] = true;
	if(key == GLUT_KEY_F2) con.specialKey[5] = true;
	if(key == GLUT_KEY_F3) con.specialKey[6] = true;

};

void CHCI::specialUpFunc(int key, int x, int y){

	if(key == GLUT_KEY_UP) con.specialKey[0] = false;
	if(key == GLUT_KEY_DOWN) con.specialKey[1] = false;
	if(key == GLUT_KEY_LEFT) con.specialKey[2] = false;
	if(key == GLUT_KEY_RIGHT) con.specialKey[3] = false;
	if(key == GLUT_KEY_F1) con.specialKey[4] = false;
	if(key == GLUT_KEY_F2) con.specialKey[5] = false;
	if(key == GLUT_KEY_F3) con.specialKey[6] = false;
};

void CHCI::readMouseButton(int button, int state, int xpos, int ypos){
	
	con.mousex = xpos;
	con.mousey = ypos;

	if((state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON))
	{
		con.leftMouse = true;
	}
	else if((state == GLUT_UP) && (button == GLUT_LEFT_BUTTON))
	{
		con.leftMouse = false;
	}

	if((state == GLUT_DOWN) && (button == GLUT_RIGHT_BUTTON))
	{
		con.rightMouse = true;
	}
	else if((state == GLUT_UP) && (button == GLUT_RIGHT_BUTTON))
	{
		con.rightMouse = false;
	}

	if((state == GLUT_DOWN) && (button == GLUT_MIDDLE_BUTTON))
	{
		con.middleMouse = true;
	}
	else if((state == GLUT_UP) && (button == GLUT_MIDDLE_BUTTON))
	{
		con.middleMouse = false;
	}

	if(button != GLUT_WHEEL_UP) con.wheelUp = false;
	if(button != GLUT_WHEEL_DOWN) con.wheelDown = false;

	if (state == GLUT_UP )
	{
		if(button == GLUT_WHEEL_UP) con.wheelUp = true;
		if(button== GLUT_WHEEL_DOWN) con.wheelDown = true;
	}
}

void CHCI::outputVars(){

	glMatrixMode(GL_PROJECTION);			//set projection matrix
	glLoadIdentity();						//reset the matrix
	gluOrtho2D(0, 1024, 0, 768);

	out.output(100,750,"mousex : ",con.mousex);
	out.output(100,740,"mousey : ",con.mousey);

	out.output(100,720,"XROT : ",con.xrot);
	out.output(100,710,"YROT : ",con.yrot);
	
	out.output(100,700,"XPOS : ",con.xpos);
	out.output(100,690,"YPOS : ",con.ypos);
	out.output(100,680,"ZPOS : ",con.zpos);


	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)xScreenSize / (GLfloat)yScreenSize, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode (GL_MODELVIEW); //set the matrix back to model

}

void CHCI::gameMode(){


		con.increment = 0;
		con.mousemovex = 0, con.mousemovey = 0;

		int diffx=(con.mousex-con.lastx)*20; //check the difference between the current x and the last x position
		int diffy=(con.mousey-con.lasty)*20; //check the difference between the current y and the last y position

		con.incy=(con.mousex-con.lastx);
		con.incx=(con.mousey-con.lasty);
					
			//increment the xrotation and yrotation positions
		con.increment += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
		con.increment += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position

		con.mousemovex += (float) diffy * CTimer::getInstance()->animBaseSpeed(); //set the xrot to xrot with the addition of the difference in the y position
		con.mousemovey += (float) diffx * CTimer::getInstance()->animBaseSpeed();	//set the xrot to yrot with the addition of the difference in the x position

		if((con.mousex != xScreenSize / 2) || (con.mousey != yScreenSize / 2))
		{
			glutWarpPointer(xScreenSize / 2,yScreenSize / 2);
			con.mousex = xScreenSize / 2;
			con.mousey = yScreenSize / 2;
		}

		con.lastx=con.mousex; //set lastx to the current x position
		con.lasty=con.mousey; //set lasty to the current y position

}

void CHCI::viewMode(){

	//FREE FLY CAMERA MODE-----------------------
		//Allow movement in any direction on any axis
		//-------------------------------------------

	float timeSpeed = CTimer::getInstance()->animBaseSpeed();
	timeSpeed*=100;

	int boost = 1;
	if(con.key['e']) boost = 5;

		//Forward movement
		if (con.key['w']) 
		{

		float xrotrad, yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		xrotrad = (con.xrot / 180 * 3.141592654f); 
		con.xpos += float(sin(yrotrad)) * boost * timeSpeed;
		con.zpos -= float(cos(yrotrad)) * boost * timeSpeed ;
		con.ypos -= float(sin(xrotrad)) * boost * timeSpeed;
		}

		//Backwards Movement
		if (con.key['s'])
		{
		float xrotrad, yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		xrotrad = (con.xrot / 180 * 3.141592654f); 
		con.xpos -= float(sin(yrotrad))* boost * timeSpeed;
		con.zpos += float(cos(yrotrad))* boost * timeSpeed;
		con.ypos += float(sin(xrotrad))* boost * timeSpeed;
		}

		//Strafe controls are the same regardless of camera mode
		//strafe right
		if (con.key['d'])
		{
		float yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		con.xpos += float(cos(yrotrad)) * boost * timeSpeed;
		con.zpos += float(sin(yrotrad)) * boost * timeSpeed;
		}

		//strafe left
		if (con.key['a'])
		{
		float yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		con.xpos -= float(cos(yrotrad)) * boost * timeSpeed;
		con.zpos -= float(sin(yrotrad)) * boost * timeSpeed;
		}

	//MOUSE MOVEMENT
	int diffx=(con.mousex-con.lastx)/2; //check the difference between the current x and the last x position
	int diffy=(con.mousey-con.lasty)/2; //check the difference between the current y and the last y position

	con.incy=(con.mousex-con.lastx);
	con.incx=(con.mousey-con.lasty);
	
	if(con.middleMouse)
	{
		//increment the xrotation and yrotation positions
		con.xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
		con.yrot += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position
	}

	//warp the mouse pointer so its always in the center of the screen for controlling the camera
	/*if((con.mousex != xScreenSize / 2) || (con.mousey != yScreenSize / 2))
	{
		glutWarpPointer(xScreenSize / 2,yScreenSize / 2);
		con.mousex = xScreenSize / 2;
		con.mousey = yScreenSize / 2;
	}*/

	//Set last positions to current positions
	con.lastx=con.mousex; //set lastx to the current x position
	con.lasty=con.mousey; //set lasty to the current y position

	//Prevent the user looking beyond 90 degrees, so mouse movements and viewing is not inverted
	if(con.xrot > 55) con.xrot = 55;
	if(con.xrot < -55) con.xrot = -55;
}

void CHCI::editMode(){

	float animSpeed = CTimer::getInstance()->animBaseSpeed();

		int boost = 1;
		int baseSpeed = 20;
		if(con.key['e']) boost = 5;
	
		//Forward movement
		if (con.key['w'])
		{

			float xrotrad, yrotrad;
			yrotrad = (con.yrot / 180 * 3.141592654f);
			xrotrad = (con.xrot / 180 * 3.141592654f); 
			con.newxpos += float(sin(yrotrad)) * baseSpeed * animSpeed * boost;
			con.newzpos -= float(cos(yrotrad)) * baseSpeed * animSpeed * boost;
			con.newypos -= float(sin(xrotrad)) * baseSpeed * animSpeed * boost;
		}

		//Backwards Movement
		if (con.key['s'])
		{
		float xrotrad, yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		xrotrad = (con.xrot / 180 * 3.141592654f); 
		con.newxpos -= float(sin(yrotrad))* baseSpeed * animSpeed;
		con.newzpos += float(cos(yrotrad))* baseSpeed * animSpeed;
		con.newypos += float(sin(xrotrad))* baseSpeed * animSpeed;
		}

		//Strafe controls are the same regardless of camera mode
		//strafe right
		if (con.key['d'])
		{
		float yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		con.newxpos += float(cos(yrotrad)) * baseSpeed * animSpeed;
		con.newzpos += float(sin(yrotrad)) * baseSpeed * animSpeed;
		}

		//strafe left
		if (con.key['a'])
		{
		float yrotrad;
		yrotrad = (con.yrot / 180 * 3.141592654f);
		con.newxpos -= float(cos(yrotrad)) * baseSpeed * animSpeed;
		con.newzpos -= float(sin(yrotrad)) * baseSpeed * animSpeed;
		}

		if(con.middleMouse)
		{
			int diffx=(con.mousex-con.lastx)/2; //check the difference between the current x and the last x position
			int diffy=(con.mousey-con.lasty)/2; //check the difference between the current y and the last y position
					
			//increment the xrotation and yrotation positions
			con.xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
			con.yrot += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position
		}

		//Set last positions to current positions
		//con.lastx=con.mousex; //set lastx to the current x position
		//con.lasty=con.mousey; //set lasty to the current y position

		if(con.leftMouse)
		{
			con.increment = 0;
			con.mousemovex = 0, con.mousemovey = 0;

			int diffx=(con.mousex-con.lastx)/2; //check the difference between the current x and the last x position
			int diffy=(con.mousey-con.lasty)/2; //check the difference between the current y and the last y position

			con.incy=(con.mousex-con.lastx);
			con.incx=(con.mousey-con.lasty);
					
			//increment the xrotation and yrotation positions
			con.increment += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
			con.increment += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position

			con.mousemovey += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
			con.mousemovex += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position
		}

		//Set last positions to current positions
		con.lastx=con.mousex; //set lastx to the current x position
		con.lasty=con.mousey; //set lasty to the current y position
	
}

void CHCI::cursorPos(){
	
	SVector3 cursorPos = get3dCursorPos(con.mousex,con.mousey);

	con.cursorx = cursorPos.x;
	con.cursory = cursorPos.y;
	con.cursorz = cursorPos.z;
}





