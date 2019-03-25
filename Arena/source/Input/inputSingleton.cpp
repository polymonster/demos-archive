#include "HCI.h"
#include "outputText.h"
#include "textureSingleton.h"
#include "timerSingleton.h"

user* user::m_instance = NULL; 

void user::create(){

	if(m_instance == NULL)
	{
		m_instance = new user;
	}
}

void user::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void user::clearStates()
{
	for(int i = 0; i < 256; i++) input.key[i] = false;
	for(i = 0; i < 9; i++) input.specialKey[i] = false;

	input.middleMouse = false;
	input.leftMouse = false;
	input.rightMouse = false;

	input.wheelDown = false;
	input.wheelUp = false;
	
	input.xpos = 0;
	input.ypos = 0;
	input.zpos = 0;
	input.xrot = 0;
	input.yrot = 0;
}

void readKeyboard(unsigned char keyID, int x, int y){

	int mod = glutGetModifiers();
	if (mod == GLUT_ACTIVE_CTRL) user::getInstance()->input.specialKey[7] = true;
	if (mod == GLUT_ACTIVE_SHIFT) user::getInstance()->input.specialKey[8] = true;

	user::getInstance()->input.key[keyID] = true;
}
void readKeyUp(unsigned char keyID, int x, int y){
	
	int mod = glutGetModifiers();
	if (mod != GLUT_ACTIVE_CTRL) user::getInstance()->input.specialKey[7] = false;
	if (mod != GLUT_ACTIVE_SHIFT) user::getInstance()->input.specialKey[8] = false;

	user::getInstance()->input.key[keyID] = false;

}
void readPassiveMouse(int xpos, int ypos){

	user::getInstance()->input.mousex = xpos;
	user::getInstance()->input.mousey = ypos;

}
void readActiveMouse(int xpos, int ypos){
	
	user::getInstance()->input.mousex = xpos;
	user::getInstance()->input.mousey = ypos;
}

void specialFunc(int key, int x, int y){

	if(key == GLUT_KEY_UP) user::getInstance()->input.specialKey[0] = true;
	if(key == GLUT_KEY_DOWN) user::getInstance()->input.specialKey[1] = true;
	if(key == GLUT_KEY_LEFT) user::getInstance()->input.specialKey[2] = true;
	if(key == GLUT_KEY_RIGHT) user::getInstance()->input.specialKey[3] = true;
	if(key == GLUT_KEY_F1) user::getInstance()->input.specialKey[4] = true;
	if(key == GLUT_KEY_F2) user::getInstance()->input.specialKey[5] = true;
	if(key == GLUT_KEY_F3) user::getInstance()->input.specialKey[6] = true;

};

void specialUpFunc(int key, int x, int y){

	if(key == GLUT_KEY_UP) user::getInstance()->input.specialKey[0] = false;
	if(key == GLUT_KEY_DOWN) user::getInstance()->input.specialKey[1] = false;
	if(key == GLUT_KEY_LEFT) user::getInstance()->input.specialKey[2] = false;
	if(key == GLUT_KEY_RIGHT) user::getInstance()->input.specialKey[3] = false;
	if(key == GLUT_KEY_F1) user::getInstance()->input.specialKey[4] = false;
	if(key == GLUT_KEY_F2) user::getInstance()->input.specialKey[5] = false;
	if(key == GLUT_KEY_F3) user::getInstance()->input.specialKey[6] = false;
};

void readMouseButton(int button, int state, int xpos, int ypos)	
{
	user::getInstance()->input.mousex = xpos;
	user::getInstance()->input.mousey = ypos;

	if((state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON))
	{
		user::getInstance()->input.leftMouse = true;
	}
	else if((state == GLUT_UP) && (button == GLUT_LEFT_BUTTON))
	{
		user::getInstance()->input.leftMouse = false;
	}

	if((state == GLUT_DOWN) && (button == GLUT_RIGHT_BUTTON))
	{
		user::getInstance()->input.rightMouse = true;
	}
	else if((state == GLUT_UP) && (button == GLUT_RIGHT_BUTTON))
	{
		user::getInstance()->input.rightMouse = false;
	}

	if((state == GLUT_DOWN) && (button == GLUT_MIDDLE_BUTTON))
	{
		user::getInstance()->input.middleMouse = true;
	}
	else if((state == GLUT_UP) && (button == GLUT_MIDDLE_BUTTON))
	{
		user::getInstance()->input.middleMouse = false;
	}

	if(button != GLUT_WHEEL_UP) user::getInstance()->input.wheelUp = false;
	if(button != GLUT_WHEEL_DOWN) user::getInstance()->input.wheelDown = false;

	if(state == GLUT_DOWN )
	{
		if(button == GLUT_WHEEL_UP)user::getInstance()->input.wheelUp = true;
		if(button == GLUT_WHEEL_DOWN)user::getInstance()->input.wheelDown = true;
	}
}

void user::gameMode(){

		user::getInstance()->input.increment = 0;
		user::getInstance()->input.mousemovex = 0, user::getInstance()->input.mousemovey = 0;

		int diffx=(user::getInstance()->input.mousex-user::getInstance()->input.lastx)*20; //check the difference between the current x and the last x position
		int diffy=(user::getInstance()->input.mousey-user::getInstance()->input.lasty)*20; //check the difference between the current y and the last y position

		user::getInstance()->input.incy=(user::getInstance()->input.mousex-user::getInstance()->input.lastx);
		user::getInstance()->input.incx=(user::getInstance()->input.mousey-user::getInstance()->input.lasty);
					
			//increment the xrotation and yrotation positions
		user::getInstance()->input.increment += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
		user::getInstance()->input.increment += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position

		user::getInstance()->input.mousemovex += (float) diffy * CTimer::getInstance()->animBaseSpeed(); //set the xrot to xrot with the addition of the difference in the y position
		user::getInstance()->input.mousemovey += (float) diffx * CTimer::getInstance()->animBaseSpeed();	//set the xrot to yrot with the addition of the difference in the x position

		if((user::getInstance()->input.mousex != xScreenSize / 2) || (user::getInstance()->input.mousey != yScreenSize / 2))
		{
			glutWarpPointer(xScreenSize / 2,yScreenSize / 2);
			user::getInstance()->input.mousex = xScreenSize / 2;
			user::getInstance()->input.mousey = yScreenSize / 2;
		}

		user::getInstance()->input.lastx=user::getInstance()->input.mousex; //set lastx to the current x position
		user::getInstance()->input.lasty=user::getInstance()->input.mousey; //set lasty to the current y position

}

void user::viewMode(){

	//FREE FLY CAMERA MODE-----------------------
		//Allow movement in any direction on any axis
		//-------------------------------------------

	float timeSpeed = CTimer::getInstance()->animBaseSpeed();
	timeSpeed*=100;

	int boost = 1;
	if(user::getInstance()->input.key['e']) boost = 5;

	float xrotrad, yrotrad;
	yrotrad = (user::getInstance()->input.yrot / 180 * PI);
	xrotrad = (user::getInstance()->input.xrot / 180 * PI); 

		//Forward movement
		if (user::getInstance()->input.key['w']) 
		{
			user::getInstance()->input.xpos += float(sin(yrotrad)) * boost * timeSpeed;
			user::getInstance()->input.zpos -= float(cos(yrotrad)) * boost * timeSpeed ;
			user::getInstance()->input.ypos -= float(sin(xrotrad)) * boost * timeSpeed;
		}

		//Backwards Movement
		if (user::getInstance()->input.key['s'])
		{
			user::getInstance()->input.xpos -= float(sin(yrotrad))* boost * timeSpeed;
			user::getInstance()->input.zpos += float(cos(yrotrad))* boost * timeSpeed;
			user::getInstance()->input.ypos += float(sin(xrotrad))* boost * timeSpeed;
		}

		//Strafe controls are the same regardless of camera mode
		//strafe right
		if (user::getInstance()->input.key['d'])
		{
			user::getInstance()->input.xpos += float(cos(yrotrad)) * boost * timeSpeed;
			user::getInstance()->input.zpos += float(sin(yrotrad)) * boost * timeSpeed;
		}

		//strafe left
		if (user::getInstance()->input.key['a'])
		{
			user::getInstance()->input.xpos -= float(cos(yrotrad)) * boost * timeSpeed;
			user::getInstance()->input.zpos -= float(sin(yrotrad)) * boost * timeSpeed;
		}

	//MOUSE MOVEMENT
	int diffx=(user::getInstance()->input.mousex-user::getInstance()->input.lastx)/2; //check the difference between the current x and the last x position
	int diffy=(user::getInstance()->input.mousey-user::getInstance()->input.lasty)/2; //check the difference between the current y and the last y position

	user::getInstance()->input.incy=(user::getInstance()->input.mousex-user::getInstance()->input.lastx);
	user::getInstance()->input.incx=(user::getInstance()->input.mousey-user::getInstance()->input.lasty);
	
	if(user::getInstance()->input.middleMouse)
	{
		//increment the xrotation and yrotation positions
		user::getInstance()->input.xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
		user::getInstance()->input.yrot += (float) diffx;	//set the xrot to yrot with the addition of the difference in the x position
	}

	//Set last positions to current positions
	user::getInstance()->input.lastx = user::getInstance()->input.mousex; //set lastx to the current x position
	user::getInstance()->input.lasty = user::getInstance()->input.mousey; //set lasty to the current y position

	//Prevent the user looking beyond 90 degrees, so mouse movements and viewing is not inverted
	if(user::getInstance()->input.xrot > 55) user::getInstance()->input.xrot = 55;
	if(user::getInstance()->input.xrot < -55) user::getInstance()->input.xrot = -55;
}

void user::cursorPos(){
	
	SVector3 cursorPos = get3dCursorPos(user::getInstance()->input.mousex,user::getInstance()->input.mousey);

	user::getInstance()->input.cursorx = cursorPos.x;
	user::getInstance()->input.cursory = cursorPos.y;
	user::getInstance()->input.cursorz = cursorPos.z;
}