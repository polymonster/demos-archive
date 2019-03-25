#ifndef _MAIN_H
#define _MAIN_H

#include "globals.h"
#include "HCI.h"
#include "outputText.h"

SControls hci;

//counter vars
float fps;
float lastTime;
int frame;
float currentTime;

int cameraMode;

bool displayDebug;

HANDLE threadHandle;
HANDLE glHandle;
HANDLE updateHandle;



#endif