#ifndef _GLOBALS_H
#define _GLOBALS_H

//openGL
#include "GLee.h"
#include <gl\gl.h>									
#include <gl\glu.h>									

//windows
#include <windows.h>
#include <windowsx.h>
#include <Commdlg.h>
#include <commctrl.h>
#include <uxTheme.h>
#include <zmouse.h>

//c++ libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <crtdbg.h>
#include <string>
#include "XInput.h"

//my project specifics
#include "ShaderManager.h"
#include "myMath.h"

//link libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "cal3d_d.lib")
#pragma comment(lib, "uxTheme.lib")
#pragma comment(lib, "WINTAB32.lib")

//window handle globals
extern HWND g_hWnd;
extern HDC g_hDC;
extern HGLRC g_hRC;
extern RECT g_rRect;
extern HINSTANCE g_hInstance;

extern CFrustum g_frustumCulling;
extern int g_objectCounter;

//screen globals
extern int g_screenWidth,g_screenHeight;

#endif