#ifndef _INPUT_H
#define _INPUT_H

#include "Globals.h"
#include "myMath.h"

#pragma comment ( lib, "XInput.lib" )

typedef enum
{
	KEY_ESC = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_SHIFT,
	KEY_CTRL,
	MOUSE_MIDDLE,
	MOUSE_LEFT,
	MOUSE_RIGHT,

}eFunctionKeys;

typedef struct
{
	XINPUT_STATE	state;
	bool connected;

}SGamepad;

typedef struct{

	//key booleans to store key states
	bool key[256];
	bool keyLocked[256];
	bool functionKey[256];
	bool fKeyLocked[256];
	bool numPad[10];
	bool numPadLocked[10];

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

	//camera related positioning variables
	SVector2 <float> rot;
	SVector3 pos;
	SVector3 lookAtpos;
	int zoom;

	SGamepad gamepad;

	int cameraMode;

}inputReader;

class user{

public:

	static void create();
	static void destroy();
	inputReader input;
	static user* getInstance(){return m_instance;};
	void setStates();
	void get3dMouseCoords();
	colorByte getPixelAtMouse();
	void updatePos();

	void setCameraPos();

	void positionCamera();
	void positionLookAtCamera();

	void mouseLook();
	bool getInput(WPARAM wParam,UINT uMsg);

	void staticCamera();

	void getPadStates();


private:

	user(){};
	~user(){};

	static user *m_instance;
};


void readKeys(WPARAM &wParam, bool trueOrFalse);

#endif