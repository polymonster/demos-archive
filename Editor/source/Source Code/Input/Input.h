#ifndef _INPUT_H
#define _INPUT_H

#include "Globals.h"
#include "myMath.h"
#include "VirtualCodes.h"
#include "TabletSupport.h"



typedef struct{

	//key booleans to store key states
	bool key[256];
	bool keyLocked[256];

	//mouse booleans to store mouse states
	bool leftMouse;
	bool rightMouse;
	bool middleMouse;
	int mouseOver;

	//vector 2's to store mouse coords
	SVector2 <int> mousePos;
	SVector2 <int> lastMouse;
	SVector3 mouse3dPos;
	SVector3 mouseIncrement;
	colorByte mousePixelColor;

	SVector3 mousePosOnTerrain;

	//camera related positioning variables
	SVector2 <float> rot;
	SVector3 pos;
	SVector3 lookAtpos;
	float zoom;

	//for the mini viewer
	SVector3 viewerPos;
	SVector3 viewerLookAt;
	SVector2 <float> viewerRot;
	float viewerZoom;
	SVector3 viewerMouseIncrement;
	SVector2 <int> viewerMousePos;
	SVector2 <int> viewerLastMouse;
	bool viewerMiddleMouse;
	bool viewerShift;

	int cameraMode;

	#ifdef _USE_TAB

		PACKET tabletPacket;

	#endif

}inputReader;

extern inputReader g_input;

class user{

public:

	static void create();
	static void destroy();
	static user* getInstance(){return m_instance;};
	void setStates();
	void get3dMouseCoords();
	void terrainMouseCoords();
	colorByte getPixelAtMouse();
	void updatePos();

	void setCameraPos();

	void positionCamera();
	void positionLookAtCamera();

	void mouseLook();
	bool getInput(WPARAM wParam,UINT uMsg);

	void staticCamera();

	void centerAt(SVector3 lookAtPos);

	void viewerMouseLook();
	void viewerPositionLookAt();

private:

	user(){};
	~user(){};

	static user *m_instance;

	#ifdef _USE_TAB

		PACKET localPacketBuffer[NPACKETQSIZE];

	#endif
};


void readKeys(WPARAM &wParam, bool trueOrFalse);

// This is the function where all the magic happens.  This function will take any LETTER
// (that's a - z and A - Z) and return true if it is being typed, false otherwise
bool KeyPressed(char key);

#endif