#include "main.h"
#include "toolbars.h"

#include <iostream>

//#include "SkinFeature.h"
//#pragma comment(lib, "SkinFeature.lib")

HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;
	WNDCLASS wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASS));				// Init the size of the class
	wndclass.style = CS_HREDRAW | CS_VREDRAW;			// Regular drawing capabilities
	wndclass.lpfnWndProc = WinProc;						// Pass our function pointer as the window procedure
	wndclass.hInstance = hInstance;						// Assign our hInstance
	wndclass.hIcon = LoadIcon(hInstance, "ICON2");			// General icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// An arrow for the cursor
	wndclass.lpszMenuName  = "MY_MENU";	
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	// A white window
	wndclass.lpszClassName = "OrganGrinder";			// Assign the class name

	RegisterClass(&wndclass);							// Register the class
	
	if(!dwStyle)									// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW;				//was causing flicker issue
	
	g_hInstance = hInstance;							// Assign our global hInstance to the window's hInstance

	RECT rWindow;
	rWindow.left	= 0;								// Set Left Value To 0
	rWindow.right	= width;							// Set Right Value To Requested Width
	rWindow.top	    = 0;								// Set Top Value To 0
	rWindow.bottom	= height;							// Set Bottom Value To Requested Height

	AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	hWnd = CreateWindow("OrganGrinder", strWindowName, dwStyle, 0, 0,
						rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;	// If we could get a handle, return NULL

	//wait until were fully loaded until we show the window
	return hWnd;
}


bool bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = 32;								// Here we use our #define for the color bits
    pfd.cDepthBits = 16;								// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits
	pfd.cAlphaBits = 0;									//add 8 alpha bits due to problems with nvdida and RGBA
 
	// This gets us a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// This sets the pixel format that we extracted from above
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;										// Return a success!
}

DWORD WINAPI createContext(HWND hDlg,HDC *hDc,HGLRC *hRc,int element)
{

	HWND glRegion;

	//get the thing we want to render gl in
	//if element is not null, get a child window
	//otherwise the dialog is the handle
	if(element != NULL) glRegion = GetDlgItem(hDlg,element);
	else glRegion = hDlg;

	//get the device context for this "thing"
	*hDc = GetDC(glRegion);

	//set its pixel format
	bSetupPixelFormat(*hDc);

	//finally create our context, enable it and set GL states
	*hRc = wglCreateContext(*hDc);
	wglMakeCurrent(*hDc, *hRc);
	wglSwapIntervalEXT(0);

	// OpenGL Initialization
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	glClearColor(0.0f, 1.0f, 0.0f, 0.5f);					// Black Background
	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations
	glEnable(GL_BLEND);

	return 1;
}

void SizeOpenGLScreen(int width, int height)			
{
	glViewport(0,0,width,height);														
}

void InitializeOpenGL(int width, int height) 
{  
	createContext(g_hMainViewport,&g_hDC,&g_hRC,NULL);
	SizeOpenGLScreen(width, height);					// Setup the screen translations and viewport
}

void DeInit()
{
	if (g_hRC)											
	{
		wglDeleteContext(g_hRC);
		wglDeleteContext(g_hRCTextureManager);
		wglDeleteContext(g_hRCMod);
		wglDeleteContext(g_hRCFoliage);
		wglDeleteContext(g_hRCPaintTexture);
		wglDeleteContext(g_hRCWaterTexture);
		wglDeleteContext(g_hRCProps);
		wglDeleteContext(g_hRCBloomColor);
	}

	
	if (g_hDC)
	{
		ReleaseDC(g_hWnd, g_hDC);
		ReleaseDC(g_hWnd, g_hDCTextureManager);
		ReleaseDC(g_hWnd, g_hDCMod);
		ReleaseDC(g_hWnd, g_hDCFoliage);
		ReleaseDC(g_hWnd, g_hDCPaintTexture);
		ReleaseDC(g_hWnd, g_hDCWaterTexture);
		ReleaseDC(g_hWnd, g_hDCProps);
		ReleaseDC(g_hWnd, g_hDCBloomColor);
	}
		
	UnregisterClass("OrganGrinder", g_hInstance);		// Free the window class

	PostQuitMessage (0);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{	
	//splashScreen();

	HWND hWnd;

	g_screenWidth = 1024;
	g_screenHeight = 768;

	InitCommonControls();

	//InitLicenKeys("password");

	/*if(InitializeSkin())
	{
		LoadSkinFile("data/styles/dogmax.rss"); 
	}*/

	AllocConsole();
	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);


	// Create window
	hWnd = CreateMyWindow("OrganGrinder - Level Editor - Alex Dixon 2008", g_screenWidth, g_screenHeight, 0, false, hInstance);

	if(hWnd == NULL) return TRUE;

	Init(hWnd);

	// Run our message loop and after it's done, return the result
	return MainLoop();						
}




