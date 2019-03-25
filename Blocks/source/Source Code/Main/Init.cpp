#include "main.h"

HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	//handle to our window
	HWND hWnd;	
	//handle to windows class
	WNDCLASS wndclass;									
	
	// Init the size of the class
	memset(&wndclass, 0, sizeof(WNDCLASS));
	// Regular drawing capabilities
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	
	// Pass our function pointer as the window procedure
	wndclass.lpfnWndProc = WinProc;	
	// Assign our hInstance
	wndclass.hInstance = hInstance;	
	// An arrow for the cursor
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);	
	// A white window
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	// Assign the class name
	wndclass.lpszClassName = "Advanced Graphics Demo";	

	// Register the class
	RegisterClass(&wndclass);							
	
	if(!dwStyle)									// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS;// | WS_CLIPCHILDREN; //was causing flicker issue
	
	g_hInstance = hInstance;							// Assign our global hInstance to the window's hInstance

	RECT rWindow;
	rWindow.left	= 0;								// Set Left Value To 0
	rWindow.right	= width;							// Set Right Value To Requested Width
	rWindow.top	    = 0;								// Set Top Value To 0
	rWindow.bottom	= height;							// Set Bottom Value To Requested Height

	AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

	// Create the window
	hWnd = CreateWindow("Advanced Graphics Demo", strWindowName, dwStyle, 0, 0,
						rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;	// If we could get a handle, return NULL

	InitCommonControls();

	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(hWnd);									// Draw the window


	SetFocus(hWnd);										// Sets Keyboard Focus To The Window	

	return hWnd;
}


bool bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = 24;							// Here we use our #define for the color bits
    pfd.cDepthBits = 24;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 8;									// No special bitplanes needed
    pfd.cStencilBits = 8;								// We desire no stencil bits
	pfd.cAlphaBits = 8;									//add 8 alpha bits due to problems with nvdida and RGBA
 
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

void SizeOpenGLScreen(int width, int height)			
{
	//prevent a divide by 0 error
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f, 150.0f);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!bSetupPixelFormat(g_hDC))						// This sets our pixel format/information
        PostQuitMessage (0);							// If there's an error, quit

    g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
    wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use

	SizeOpenGLScreen(width, height);					// Setup the screen translations and viewport
}

void DeInit()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release our HDC from memory
		
	UnregisterClass("OrganGrinder", g_hInstance);		// Free the window class

	PostQuitMessage (0);								// Post a QUIT message to the window
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{	
	//Hwnd handle for our current window
	HWND hWnd;

	//some global variables to save our screen size
	g_screenWidth = 1024;
	g_screenHeight = 768;

	//call the create window function
	hWnd = CreateMyWindow("Advanced Graphics/Collision Demo - Alex Dixon 2008", g_screenWidth, g_screenHeight, 0, false, hInstance);

	// If we never got a valid window handle, quit the program
	if(hWnd == NULL) return TRUE;

	// INIT OpenGL
	Init(hWnd);

	// Run our message loop and after it's done, return the result
	return MainLoop();						
}



