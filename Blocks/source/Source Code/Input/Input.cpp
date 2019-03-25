#include "input.h"
#include "Fonts.h"
#include "TimerSingleton.h"

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

void user::setCameraPos()
{
	if(input.cameraMode == 0) positionLookAtCamera();
	else positionCamera();
}

void user::setStates()
{
	for(int i = 0; i < 256; i ++)
	{
		input.key[i] = false;
		input.functionKey[i] = false;

		input.keyLocked[i] = false;
		input.fKeyLocked[i] = false;
	}

	for(int i = 0; i < 10; i++)
	{
		input.numPad[i] = false;
		input.numPadLocked[i] = false;
	}

	input.mouse3dPos.x = 0;
	input.mouse3dPos.y = 0;
	input.mouse3dPos.z = 0;

	input.pos.x = 0;
	input.pos.y = 5;
	input.pos.z = 20;

	input.lookAtpos.x = 0;
	input.lookAtpos.y = 0;
	input.lookAtpos.z = 0;

	input.rot.x = 0;
	input.rot.y = 0;

	input.leftMouse = false;
	input.rightMouse = false;
	input.middleMouse = false;

	input.zoom = 200;

	input.cameraMode = 1;

	
}
bool user::getInput(WPARAM wParam,UINT uMsg)
{
	switch(uMsg)
	{
		case WM_KEYDOWN:
		
		readKeys(wParam,true);

		return 0;

		break;

		case WM_LBUTTONDOWN:

			user::getInstance()->input.leftMouse = true;
			SetFocus(g_hWnd);

			return 0;

		break;

		case WM_LBUTTONUP:

			user::getInstance()->input.leftMouse = false;

			return 0;

		break;

		case WM_RBUTTONDOWN:

			user::getInstance()->input.rightMouse = true;

			return 0;

		break;

		case WM_RBUTTONUP:

			user::getInstance()->input.rightMouse = false;

			return 0;

		break;

		case WM_MBUTTONDOWN:

			user::getInstance()->input.middleMouse = true;

			return 0;

		break;

		case WM_MBUTTONUP:

			user::getInstance()->input.middleMouse = false;

			return 0;

		break;
		
		case WM_KEYUP:

			readKeys(wParam,false);

			return 0;

		break;

	    case WM_MOUSEWHEEL:

			input.zoom -=(short) HIWORD(wParam) * 0.25f;

			if(input.zoom > 650) input.zoom = 650;
			else if(input.zoom < 20) input.zoom = 20;

        break;

	}

	return 0;
}

void readKeys(WPARAM &wParam,bool trueOrFalse)
{
	switch(wParam) 
	{					
		case VK_ESCAPE:

			user::getInstance()->input.functionKey[KEY_ESC] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.fKeyLocked[VK_ESCAPE] = false;

		break;

		case VK_CONTROL:
		
			user::getInstance()->input.functionKey[KEY_CTRL] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.fKeyLocked[VK_CONTROL] = false;

		break;

		case VK_UP:									

			user::getInstance()->input.functionKey[KEY_UP] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.fKeyLocked[VK_UP] = false;

		break;

		case VK_DOWN:								

			user::getInstance()->input.functionKey[KEY_DOWN] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.fKeyLocked[VK_DOWN] = false;

		break;

		case VK_LEFT:								

			user::getInstance()->input.functionKey[KEY_LEFT] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.fKeyLocked[VK_LEFT] = false;

		break;

		case VK_RIGHT:								

			user::getInstance()->input.functionKey[KEY_RIGHT] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.fKeyLocked[VK_RIGHT] = false;

		break;

		default :

			user::getInstance()->input.key[wParam] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.keyLocked[wParam] = false;

		break;
	}

	//read in number pad buttons
	int numpadstart = VK_NUMPAD0;

	for(int i = 0; i < 10; i++)
	{
		if(wParam == numpadstart + i)
		{							
			user::getInstance()->input.numPad[i] = trueOrFalse;
			if(!trueOrFalse) user::getInstance()->input.numPad[i] = false;
		}
	}
}

void user::getPadStates( )
{
	ZeroMemory( &input.gamepad.state, sizeof(XINPUT_STATE) );

    // Simply get the state of the controller from XInput.
    DWORD dwResult = XInputGetState( 0, &input.gamepad.state );	
}

void user::mouseLook()
{
	input.mouseIncrement.x = (float) input.mousePos.x - input.lastMouse.x;
	input.mouseIncrement.y = (float) input.mousePos.y - input.lastMouse.y;

	if(input.middleMouse)
	{
		if(input.functionKey[KEY_CTRL])
		{
			float incx = input.mouseIncrement.x * 0.001;
			float incy = input.mouseIncrement.y * 0.001;

			double xrot = degreesToRadians(input.rot.x);
			double yrot = degreesToRadians(input.rot.y);

			input.lookAtpos.x += float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
			input.lookAtpos.y -= float( cos(xrot) * incy );
			input.lookAtpos.z += float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );
		}
		else
		{
			input.rot.x += input.mouseIncrement.y;
			input.rot.y += input.mouseIncrement.x;
		}
	}

	if(input.rot.x < 5) input.rot.x = 5;
	else if(input.rot.x > 175) input.rot.x = 175;

	if(input.key['H'])
	{
		std::ofstream fout;
		fout.open("flie.txt");

		fout << input.zoom;

		fout.close();
	}

	input.lastMouse = input.mousePos;
}

void user::get3dMouseCoords()
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	double posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)(input.mousePos.x);
	winY = (float)viewport[3] - (float)(input.mousePos.y);
	glReadPixels( input.mousePos.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	input.mouse3dPos.x = float(posX);
	input.mouse3dPos.y = float(posY);
	input.mouse3dPos.z = float(posZ);
}

colorByte user::getPixelAtMouse()
{
	unsigned char pixel[3];

	int mx = input.mousePos.x;
	int my = input.mousePos.y;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(mx, viewport[3] - my, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	input.mousePixelColor.r = pixel[0];
	input.mousePixelColor.g = pixel[1];
	input.mousePixelColor.b = pixel[2];

	return input.mousePixelColor;
}

void user::positionCamera()
{
	glRotated(input.rot.x,1,0,0);
	glRotated(input.rot.y,0,1,0);
	glTranslated(-input.pos.x,-input.pos.y,-input.pos.z);
}

void user::staticCamera()
{
	glTranslated(0,0,-20);
}

void user::positionLookAtCamera()
{
	//vector to store the calculated camera pos
	SVector3 cameraPos = input.lookAtpos;

	//get the rotations in radians
	double x = degreesToRadians(user::getInstance()->input.rot.x);
	double y = degreesToRadians(user::getInstance()->input.rot.y);

	//calculate the unit sphere position
	cameraPos.x += float(-cos(x) * sin(y));
	cameraPos.y += float(sin(x));
	cameraPos.z += float(cos(x) * cos(y));

	//scale the unit vector by zoom
	cameraPos =  cameraPos * float(input.zoom);

	//now translate the scaled vector to focus on the look at point'
	//cameraPos = input.lookAtpos - cameraPos;

	//finally set up the openGL matrix stack for
	//this view angle
	glRotated(input.rot.x,1,0,0);
	glRotated(input.rot.y,0,1,0);
	glTranslated(-cameraPos.x,-cameraPos.y,-cameraPos.z);

	input.pos = cameraPos; //store in pos var for use in other camera modes
}

void user::updatePos()
{
	float timeStep = CTimer::getInstance()->animBaseSpeed();

	float xrotrad, yrotrad;
	yrotrad = (input.rot.y / 180 * 3.141592654f);
	xrotrad = (input.rot.x / 180 * 3.141592654f); 

	int speed = 8;
	if(input.functionKey[KEY_CTRL]) speed = 20;

		//Forward movement
		if(input.key['W']) 
		{
			input.pos.x += float(sin(yrotrad)) * timeStep * speed;
			input.pos.z -= float(cos(yrotrad)) * timeStep * speed;
			input.pos.y -= float(sin(xrotrad)) * timeStep * speed;
		}

		//Backwards Movement
		if(input.key['S'])
		{
			input.pos.x -= float(sin(yrotrad))* timeStep * speed;
			input.pos.z += float(cos(yrotrad))* timeStep * speed;
			input.pos.y += float(sin(xrotrad))* timeStep * speed;
		}

		//strafe right
		if (input.key['D'])
		{
			input.pos.x += float(cos(yrotrad)) * timeStep * speed;
			input.pos.z += float(sin(yrotrad)) * timeStep * speed;
		}

		//strafe left
		if (input.key['A'])
		{
			input.pos.x -= float(cos(yrotrad)) * timeStep * speed;
			input.pos.z -= float(sin(yrotrad)) * timeStep * speed;
		}

		int invertDirection = 1;
		if(input.functionKey[KEY_CTRL]) invertDirection = -1;

		//set the camera orientation to an axis 
		//FRONT
		if(input.numPad[2])
		{
			input.rot.x = 0;
			input.rot.y = 0;
		}
		//TOP
		if(input.numPad[5])
		{
			input.rot.x = 90 * invertDirection;
			input.rot.y = 0;
		}
		//LEFT
		if(input.numPad[4])
		{
			input.rot.x = 0;
			input.rot.y = 90 * invertDirection;
		}

}
// And a list of the important virtual codes just for you
// "Missing codes" are most likely OEM (original equipment manufacture) specific so check
// out the website of the keyboard in question or MSDN for a full virtual code list

/*	
	Virtual Code	Hex Value			"Key"
	============	=========			=====

	 VK_LBUTTON			01			left mouse button  
	 VK_RBUTTON			02			right mouse button  
	 VK_CANCEL			03			control-break processing  
	 VK_MBUTTON			04			middle mouse button (three-button mouse)  
 
	 VK_BACK			08				backspace  
	 VK_TAB				09				tab  
	
	 VK_CLEAR			0C				clear  
	 VK_RETURN			0D				enter  

	 VK_SHIFT			10				shift  
	 VK_CONTROL			11				ctrl   
	 VK_MENU			12				alt 
	 VK_PAUSE			13				pause   
	 VK_CAPITAL			14				caps lock  

	 VK_ESCAPE			1B				esc  
	 VK_SPACE			20				spacebar  
	 VK_PRIOR			21				page up   
	 VK_NEXT			22				page down  
	 VK_END				23				end key  
	 VK_HOME			24				home key  
	 VK_LEFT			25				left arrow  
	 VK_UP				26				up arrow  
	 VK_RIGHT			27				right arrow  
	 VK_DOWN			28				down arrow  
	 VK_SELECT			29				select key  

	 VK_EXECUTE			2B				execute key  
	 VK_SNAPSHOT		2C				print screen key  
	 VK_INSERT			2D				insert key  
	 VK_DELETE			2E				delete key  
	 VK_HELP			2F				help key  
	 VK_0				30					0   
	 VK_1				31					1   
	 VK_2				32					2   
	 VK_3				33					3   
	 VK_4				34					4   
	 VK_5				35					5   
	 VK_6				36					6   
	 VK_7				37					7   
	 VK_8				38					8   
	 VK_9				39					9   
											
	 VK_A				41					a   
	 VK_B				42					b   
	 VK_C				43					c   
	 VK_D				44					d   
	 VK_E				45					e   
	 VK_F				46					f   
	 VK_G				47					g   
	 VK_H				48					h   
	 VK_I				49					i   
	 VK_J				4A					j   
	 VK_K				4B					k   
	 VK_L				4C					l   
	 VK_M				4D					m   
	 VK_N				4E					n   
	 VK_O				4F					o   
	 VK_P				50					p   
	 VK_Q				51					q   
	 VK_R				52					r   
	 VK_S				53					s   
	 VK_T				54					t   
	 VK_U				55					u   
	 VK_V				56					v   
	 VK_W				57					w   
	 VK_X				58					x   
	 VK_Y				59					y   
	 VK_Z				5A					z   
 	 VK_NUMPAD0			60			Numeric keypad 0 key  
	 VK_NUMPAD1			61			Numeric keypad 1 key  
	 VK_NUMPAD2			62			Numeric keypad 2 key  
	 VK_NUMPAD3			63			Numeric keypad 3 key  
	 VK_NUMPAD4			64			Numeric keypad 4 key  
	 VK_NUMPAD5			65			Numeric keypad 5 key  
	 VK_NUMPAD6			66			Numeric keypad 6 key  
	 VK_NUMPAD7			67			Numeric keypad 7 key  
	 VK_NUMPAD8			68			Numeric keypad 8 key  
	 VK_NUMPAD9			69			Numeric keypad 9 key  
	 VK_MULTIPLY		6A				Multiply key  
	 VK_ADD				6B				Add key  
	 VK_SEPARATOR		6C				Separator key  
	 VK_SUBTRACT		6D				Subtract key  
	 VK_DECIMAL			6E				Decimal key  
	 VK_DIVIDE			6F				Divide key  
	 VK_F1				70				f1 key  
	 VK_F2				71				f2 key  
	 VK_F3				72				f3 key  
	 VK_F4				73				f4 key  
	 VK_F5				74				f5 key  
	 VK_F6				75				f6 key  
	 VK_F7				76				f7 key  
	 VK_F8				77				f8 key  
	 VK_F9				78				f9 key  
	 VK_F10				79				f10 key  
	 VK_F11				7A				f11 key  
	 VK_F12				7B				f12 key  
	 	   
	 VK_NUMLOCK			90			num lock key  
	 VK_SCROLL			91			scroll lock key  
*/
