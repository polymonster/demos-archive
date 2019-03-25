#ifndef _GLOBALS_H
#define _GLOBALS_H

//#define _USE_TAB

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


//link libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "comctl32.lib")
//#pragma comment(lib, "cal3d_d.lib")
//#pragma comment(lib, "uxTheme.lib")

#ifdef _USE_TAB

	#pragma comment(lib, "WINTAB32.lib")
	//WINTAB (graphics tablet support)
	#include <WINTAB\wintab.h>
	#define PACKETDATA	(PK_CURSOR | PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_ORIENTATION)
	#define PACKETMODE	0
	#include <WINTAB\pktdef.h>
	#ifdef USE_X_LIB
	#include <WINTAB\wintabx.h>
	#endif
	#define NPACKETQSIZE	1
	//peripheral globals
	extern HCTX hTab;

#endif

//my project specifics
//#include "cal3d/cal3d.h"
#include "ShaderManager.h"
#include "resource.h"
#include "TabletSupport.h"
#include "myMath.h"
#include "toolbars.h"
#include "helpers.h"


//screen globals
extern int g_screenWidth,g_screenHeight;

extern int g_editMode;

extern CFrustum g_frustumCulling;

typedef struct
{
	unsigned int polyCount;
	unsigned int polyBudget;

}SPolyDetails;

extern SPolyDetails g_polyCounter;

//undo structs
class terrainHeightUndo
{
	public:

	std::vector<SVector3> heights;
	std::vector<unsigned int> idx;
	std::vector<unsigned int> idy;
	bool modified;

};

class terrainPaintUndo
{
	public:

	std::vector<SVector3> weights;
	std::vector<unsigned int> idx;
	std::vector<unsigned int> idy;
	bool modified;

};

class commandList
{
	public:

	std::vector<std::string> commandNameList;
	std::vector<terrainHeightUndo> terrainHeight;
	std::vector<terrainPaintUndo> terrainPaint;

	std::vector<std::string> commands;
};

//undo / redo stacks
extern commandList g_undoStack;
extern commandList g_redoStack;

#endif