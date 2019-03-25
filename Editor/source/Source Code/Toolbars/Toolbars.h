#ifndef _TOOLBARS_H
#define _TOOLBARS_H

#include "Globals.h"

#define MAIN_TOOL_HEIGHT 42

//call backs
BOOL CALLBACK toolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK mainToolbarProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CreateToolTip(int toolID, HWND hDlg, LPSTR text);
LRESULT CALLBACK modelManagerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK textureManagerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK viewportProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK propProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//context managers
int setupPixelFormat(HDC hdc);
DWORD WINAPI createContext(HWND hDlg,HDC *hDc,HGLRC *hRc,int element);

extern bool propertiesPaneActive;

void checkViewportSize();

//toolbar / box / dialog / creation
void createMainToolbar();
void createTerrainToolbar();
void createViewToolbar();
void creatMainStatus();
void createPropertiesPane();
void createTextureMan();
void createModelMan();
void createTerrainTools();
void createPropTools();
void resizeToolbars();
void createViewTools();
void initialiseToolbars();
void setPropertyDimensions();
void createFoliageToolbar();
void createWaterToolbar();
void splashScreen();
void createPolygonCounter();
void createMainViewport();
void createAtmosphereTools();
void createOctreeTools();


//GLOBALS
//window globals									
extern int g_propWidth;
extern RECT  g_rRect;
extern RECT g_toolboxRect;
extern RECT g_innerToolboxRect;
extern HINSTANCE g_hInstance;	

//rendering gl contexts and device globals
extern HDC   g_hDCTextureManager;
extern HGLRC g_hRCTextureManager;	
extern HDC	 g_hDCMod;
extern HGLRC g_hRCMod;
extern HDC	 g_hDCFoliage;
extern HGLRC g_hRCFoliage;
extern HDC   g_hDC;											
extern HGLRC g_hRC;
extern HDC	 g_hDCPaintTexture;
extern HGLRC g_hRCPaintTexture;
extern HDC	 g_hDCWaterTexture;
extern HGLRC g_hRCWaterTexture;
extern HDC   g_hDCProps;
extern HGLRC g_hRCProps;
extern HDC   g_hDCBloomColor;
extern HGLRC g_hRCBloomColor;
extern HDC	 g_hDCSkybox;
extern HGLRC g_hRCSkybox;
extern HDC	 g_hDCSkyboxViewer;
extern HGLRC g_hRCSkyboxViewer;


//window handles
extern HWND g_hWnd;
extern HWND g_hMod;
extern HWND g_hProp;
extern HWND g_hViewTool;
extern HWND g_hTerrain;
extern HWND g_hMainTool;
extern HWND g_hTextureManager;
extern HWND g_hTextureWindow;
extern HWND g_hTerrainPaint;
extern HWND g_hTerrainProp;
extern HWND g_hStatus;
extern HWND g_hTerrainHeight;
extern HWND g_hTerrainValues;
extern HWND g_hPropProp;
extern HWND g_hFoliageProp;
extern HWND g_hWaterProp;
extern HWND g_hWaterOptical;
extern HWND g_hWaterTexture;
extern HWND g_hWaterManagement;
extern HWND g_hPolyCounter;
extern HWND	g_hMainViewport;
extern HWND g_hAtmosphereProp;
extern HWND g_hAtmosphereSky;
extern HWND g_hAtmosphereEnv;
extern HWND g_hAtmosphereManagement;
extern HWND g_hOctree;
extern HWND g_hSplash;







#endif