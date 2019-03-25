#include "toolbars.h"
#include "globals.h"
#include "main.h"

//hinstance (the daddy)
HINSTANCE g_hInstance;
int g_screenWidth,g_screenHeight;
bool propertiesPaneActive;

//toolbar sizing globals
RECT  g_rRect;															
int g_propWidth;
RECT g_toolboxRect;
RECT g_innerToolboxRect;

//hwnds handles
HWND g_hViewTool;
HWND g_hTerrain;
HWND g_hMainTool;
HWND g_hProp;
HWND g_hTextureManager;
HWND g_hTextureWindow;
HWND g_hWnd;
HWND g_hTerrainPaint;
HWND g_hMod;
HWND g_hTerrainProp;
HWND g_hStatus;
HWND g_hTerrainHeight;
HWND g_hTerrainValues;
HWND g_hPropProp;
HWND g_hFoliageProp;
HWND g_hWaterProp;
HWND g_hPolyCounter;
HWND g_hMainViewport;
HWND g_hWaterOptical;
HWND g_hWaterTexture;
HWND g_hWaterManagement;
HWND g_hAtmosphereProp;
HWND g_hAtmosphereSky;
HWND g_hAtmosphereEnv;
HWND g_hAtmosphereManagement;
HWND g_hOctree;
HWND g_hSplash;

//device contexts
HDC   g_hDCMod;
HGLRC g_hRCMod;
HDC   g_hDCTextureManager;
HGLRC g_hRCTextureManager;
HDC	  g_hDCPaintTexture;
HGLRC g_hRCPaintTexture;
HDC   g_hDC;
HGLRC g_hRC;
HDC	  g_hDCFoliage;
HGLRC g_hRCFoliage;
HDC	  g_hDCWaterTexture;
HGLRC g_hRCWaterTexture;
HDC   g_hDCProps;
HGLRC g_hRCProps;
HDC   g_hDCBloomColor;
HGLRC g_hRCBloomColor;
HDC	  g_hDCSkybox;
HGLRC g_hRCSkybox;
HDC	  g_hDCSkyboxViewer;
HGLRC g_hRCSkyboxViewer;

//peripherals
#ifdef _USE_TAB

	HCTX hTab = NULL;

#endif