#include "input.h"
#include "Fonts.h"
#include "TimerSingleton.h"
#include "toolbars.h"

user* user::m_instance = NULL;

inputReader g_input;

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
	if(g_input.cameraMode == 0) positionLookAtCamera();
	else positionCamera();
}

void user::setStates()
{
	for(int i = 0; i < 256; i ++)
	{
		g_input.key[i] = false;
		g_input.keyLocked[i] = false;
	}


	g_input.mouse3dPos.x = 0;
	g_input.mouse3dPos.y = 0;
	g_input.mouse3dPos.z = 0;

	g_input.pos.x = 0;
	g_input.pos.y = 0;
	g_input.pos.z = 0;

	g_input.lookAtpos.x = 0;
	g_input.lookAtpos.y = 0;
	g_input.lookAtpos.z = 0;

	g_input.viewerLookAt = SVector3(0,0,0);
	g_input.viewerPos = SVector3(0,0,0);

	g_input.rot.x = 45;
	g_input.rot.y = 45;

	g_input.viewerRot.x = 45;
	g_input.viewerRot.y = 45;

	g_input.zoom = 200;
	g_input.viewerZoom = 200;

	g_input.leftMouse = false;
	g_input.rightMouse = false;
	g_input.middleMouse = false;

	g_input.cameraMode = 1;
}

bool user::getInput(WPARAM wParam,UINT uMsg)
{
	#ifdef _USE_TAB

		//WTOverlap( hTab, TRUE );
		//Firstly tablet support for any mouse message
		int nPackets;

		switch(uMsg)
		{
			//if any mouse message is processed
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDBLCLK:

			//get the packet queue from wintab
			nPackets = WTPacketsGet(hTab, NPACKETQSIZE, &localPacketBuffer);

			//loop through the packets
			for(int i = 0; i < nPackets; i++)
			{
				//set the global tablet packet to store the values
				g_input.tabletPacket = localPacketBuffer[i];
			}

			//if no packets are recieved default the pressure
			//to be static when a mouse device is used.
			if(nPackets <= 0) 
			{
				g_input.tabletPacket.pkNormalPressure = 10;
			}

			break;
		}

	#endif

	//now a tablets read in move onto everything else

	switch(uMsg)
	{
		case WM_KEYDOWN:
		
		readKeys(wParam,true);

		return 0;

		break;

		case WM_LBUTTONDOWN:

			g_input.leftMouse = true;
			SetFocus(g_hWnd);

			return 0;

		break;

		case WM_LBUTTONUP:

			g_input.leftMouse = false;

			return 0;

		break;

		case WM_RBUTTONDOWN:

			g_input.rightMouse = true;

			return 0;

		break;

		case WM_RBUTTONUP:

			g_input.rightMouse = false;

			return 0;

		break;

		case WM_MBUTTONDOWN:

			g_input.middleMouse = true;

			return 0;

		break;

		case WM_MBUTTONUP:

			g_input.middleMouse = false;

			return 0;

		break;
		
		case WM_KEYUP:

			readKeys(wParam,false);

			return 0;

		break;

	    case WM_MOUSEWHEEL:

			g_input.zoom -= (short) HIWORD(wParam);

			if(g_input.zoom < 10) g_input.zoom = 10;

        break;

	}

	return 0;
}

void readKeys(WPARAM &wParam,bool trueOrFalse)
{
	g_input.key[wParam] = trueOrFalse;
	if(!trueOrFalse) g_input.keyLocked[wParam] = false;
}

void user::mouseLook()
{
	g_input.mouseIncrement.x = (float) g_input.mousePos.x - g_input.lastMouse.x;
	g_input.mouseIncrement.y = (float) g_input.mousePos.y - g_input.lastMouse.y;

	if(g_input.middleMouse)
	{
		if(g_input.key[VK_SHIFT])
		{
			float incx = g_input.mouseIncrement.x * 0.1;
			float incy = g_input.mouseIncrement.y * 0.1;

			double xrot = degreesToRadians(g_input.rot.x);
			double yrot = degreesToRadians(g_input.rot.y);

			g_input.lookAtpos.x += float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
			g_input.lookAtpos.y -= float( cos(xrot) * incy );
			g_input.lookAtpos.z += float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );

			if(g_input.cameraMode == 1)
			{
				g_input.rot.x += g_input.mouseIncrement.y;
				g_input.rot.y += g_input.mouseIncrement.x;
			}
		}
		else
		{
			g_input.rot.x += g_input.mouseIncrement.y;
			g_input.rot.y += g_input.mouseIncrement.x;
		}
	}

	g_input.lastMouse = g_input.mousePos;
}

void user::get3dMouseCoords()
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	double posX, posY, posZ;

	//obtain current OpenGL matrices
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	//get the mouse screen coordinates (fli[[ing the y-coordinate)
	winX = (float)(g_input.mousePos.x);
	winY = (float)viewport[3] - (float)(g_input.mousePos.y);

	//read pixels to get the depth of a pixel
	glReadPixels( g_input.mousePos.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	//un project to get a 3D world position, passing in the above information
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	g_input.mouse3dPos.x = float(posX);
	g_input.mouse3dPos.y = float(posY);
	g_input.mouse3dPos.z = float(posZ);
}

void user::terrainMouseCoords()
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = -12765452;
	double posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)(g_input.mousePos.x);
	winY = (float)viewport[3] - (float)(g_input.mousePos.y);

	glReadPixels( g_input.mousePos.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		
	g_input.mousePosOnTerrain.x = float(posX);
	g_input.mousePosOnTerrain.y = float(posY);
	g_input.mousePosOnTerrain.z = float(posZ);
}



colorByte user::getPixelAtMouse()
{
	//create an unsigned char to store the pixel
	unsigned char pixel[3];

	//obtain the mouse x and y coodinate from the input globals
	int mx = g_input.mousePos.x;
	int my = g_input.mousePos.y;

	//get the viewport dimensions from OpenGL
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	//read the pixels and store the result in the pixel unsigned char
	glReadPixels(mx, (viewport[3]) - my, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	//store the pixel colour for later use
	g_input.mousePixelColor.r = pixel[0];
	g_input.mousePixelColor.g = pixel[1];
	g_input.mousePixelColor.b = pixel[2];

	return g_input.mousePixelColor;
}

void user::positionCamera()
{
	glRotated(g_input.rot.x,1,0,0);
	glRotated(g_input.rot.y,0,1,0);
	glTranslated(-g_input.pos.x,-g_input.pos.y,-g_input.pos.z);
}

void user::staticCamera()
{
	glTranslated(0,0,-20);
}

void user::positionLookAtCamera()
{
	//vector to store the calculated camera pos
	SVector3 cameraPos = g_input.lookAtpos;

	//get the rotations in radians
	double x = degreesToRadians(g_input.rot.x);
	double y = degreesToRadians(g_input.rot.y);

	SVector3 unitVector = SVector3(0,0,0);
	//calculate the unit sphere position
	unitVector.x += float(-cos(x) * sin(y));
	unitVector.y += float(sin(x));
	unitVector.z += float(cos(x) * cos(y));

	//scale the unit vector by zoom
	cameraPos +=  (unitVector * float(g_input.zoom));

	//now translate the scaled vector to focus on the look at point'
	//cameraPos = input.lookAtpos - cameraPos;

	//finally set up the openGL matrix stack for
	//this view angle
	glRotated(g_input.rot.x,1,0,0);
	glRotated(g_input.rot.y,0,1,0);
	glTranslated(-cameraPos.x,-cameraPos.y,-cameraPos.z);

	g_input.pos = cameraPos; //store in pos var for use in other camera modes
}

void user::updatePos()
{
	float timeStep = CTimer::getInstance()->animBaseSpeed();

	float xrotrad, yrotrad;
	yrotrad = (g_input.rot.y / 180 * 3.141592654f);
	xrotrad = (g_input.rot.x / 180 * 3.141592654f); 

	int speed = 20;
	if(g_input.key[VK_CONTROL]) speed = 50;

		//Forward movement
		if(g_input.key[VK_UP]) 
		{
			g_input.pos.x += float(sin(yrotrad)) * timeStep * speed;
			g_input.pos.z -= float(cos(yrotrad)) * timeStep * speed;
			g_input.pos.y -= float(sin(xrotrad)) * timeStep * speed;
		}

		//Backwards Movement
		if(g_input.key[VK_DOWN])
		{
			g_input.pos.x -= float(sin(yrotrad))* timeStep * speed;
			g_input.pos.z += float(cos(yrotrad))* timeStep * speed;
			g_input.pos.y += float(sin(xrotrad))* timeStep * speed;
		}

		//strafe right
		if (g_input.key[VK_RIGHT])
		{
			g_input.pos.x += float(cos(yrotrad)) * timeStep * speed;
			g_input.pos.z += float(sin(yrotrad)) * timeStep * speed;
		}

		//strafe left
		if (g_input.key[VK_LEFT])
		{
			g_input.pos.x -= float(cos(yrotrad)) * timeStep * speed;
			g_input.pos.z -= float(sin(yrotrad)) * timeStep * speed;
		}

		int invertDirection = 1;
		if(g_input.key[VK_CONTROL]) invertDirection = -1;

		//set the camera orientation to an axis 
		//FRONT
		if(g_input.key[VK_NUMPAD2])
		{
			g_input.rot.x = 0;
			g_input.rot.y = 0;
		}
		//TOP
		if(g_input.key[VK_NUMPAD5])
		{
			g_input.rot.x = 90 * invertDirection;
			g_input.rot.y = 0;
		}
		//LEFT
		if(g_input.key[VK_NUMPAD4])
		{
			g_input.rot.x = 0;
			g_input.rot.y = 90 * invertDirection;
		}

}

void user::centerAt(SVector3 lookAtPos)
{
	g_input.lookAtpos = lookAtPos;
}

void user::viewerMouseLook()
{
	//for the mini viewer
	g_input.viewerMouseIncrement.x = (float) g_input.viewerMousePos.x - g_input.viewerLastMouse.x;
	g_input.viewerMouseIncrement.y = (float) g_input.viewerMousePos.y - g_input.viewerLastMouse.y;

	if(g_input.viewerMiddleMouse)
	{
		if(g_input.key[VK_SHIFT])
		{
			float incx = g_input.viewerMouseIncrement.x * g_input.zoom;
			float incy = g_input.viewerMouseIncrement.y * g_input.zoom;

			double xrot = degreesToRadians(g_input.viewerRot.x);
			double yrot = degreesToRadians(g_input.viewerRot.y);

			g_input.viewerLookAt.x += float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
			g_input.viewerLookAt.y -= float( cos(xrot) * incy );
			g_input.viewerLookAt.z += float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );
		}
		else
		{
			g_input.viewerRot.x += g_input.viewerMouseIncrement.y;
			g_input.viewerRot.y += g_input.viewerMouseIncrement.x;
		}
	}

	g_input.viewerLastMouse = g_input.viewerMousePos;
}

void user::viewerPositionLookAt()
{
	//vector to store the calculated camera pos
	SVector3 cameraPos = g_input.viewerLookAt;

	//get the rotations in radians
	float x = degreesToRadians(g_input.viewerRot.x);
	float y = degreesToRadians(g_input.viewerRot.y);

	SVector3 unitVector = SVector3(0,0,0);
	//calculate the unit sphere position
	unitVector.x += float(-cos(x) * sin(y));
	unitVector.y += float(sin(x));
	unitVector.z += float(cos(x) * cos(y));

	//scale the unit vector by zoom
	cameraPos +=  (unitVector * float(g_input.viewerZoom));

	//finally set up the openGL matrix stack for
	//this view angle
	glRotated(g_input.viewerRot.x,1,0,0);
	glRotated(g_input.viewerRot.y,0,1,0);
	glTranslated(-cameraPos.x,-cameraPos.y,-cameraPos.z);

	g_input.viewerPos = cameraPos; //store in pos var for use in other camera modes
}
