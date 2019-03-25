#include "main.h"										// This includes our header file
#include "timerSingleton.h"
#include "modelManager.h"
#include "textureSingleton.h"
#include "scene.h"
#include "helpers.h"
#include "fonts.h"
#include "shaderManager.h"

bool initialised;

HWND g_hWnd;
HDC g_hDC;
HGLRC g_hRC;
RECT g_rRect;
HINSTANCE g_hInstance;
int g_screenWidth,g_screenHeight;
CScene *scene;

void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	g_screenWidth = g_rRect.right;						// get screen widths
	g_screenHeight = g_rRect.bottom;

	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	CText::getInstance()->create();
	CText::getInstance()->createFont("Raster Fonts",30,g_hDC);

	user::getInstance()->create();
	user::getInstance()->setStates();
	user::getInstance()->input.cameraMode = 0;

	CTextureMan::create();

	CTextureMan::getInstance()->load("data/texture/WOOD_1.tga");
	CTextureMan::getInstance()->load("data/texture/WOOD_1N.tga");
	CTextureMan::getInstance()->load("data/texture/floor.tga");
	CTextureMan::getInstance()->load("data/texture/3dhelp.tga");

	CTextureMan::getInstance()->loadCubeMap
	(
		"data/texture/alps_north.tga",
		"data/texture/alps_south.tga",
		"data/texture/alps_west.tga",
		"data/texture/alps_east.tga",
		"data/texture/alps_up.tga",
		"data/texture/alps_down.tga"
	);

	CTimer::getInstance()->create();
	CTimer::getInstance()->init();

	CModelManager::getInstance()->create();
	CModelManager::getInstance()->load("data/models/toyroom4.3ds");

	CShaderManager::create();
	g_shaderId.celShader = CShaderManager::getInstance()->loadShader("Shaders/CelShader/CelShader.glsl");
	g_shaderId.normalMap = CShaderManager::getInstance()->loadShader("Shaders/normalMap/normalMap.glsl");
	g_shaderId.skyBox = CShaderManager::getInstance()->loadShader("Shaders/skyBox/skyBox.glsl");

	initialised = true;

	scene = new CScene();

	createCubeVBO();

	glEnable (GL_DEPTH_TEST); //enable the depth testing
	glDepthFunc(GL_LEQUAL); //set the depth function
	glEnable (GL_LIGHTING); //enable the lighting
	glEnable(GL_NORMALIZE); //enable normalise for lighting
	glEnable(GL_BLEND); //enable blend for belnding colours
	glShadeModel (GL_SMOOTH); //set the shader to smooth shader
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);			
}

WPARAM MainLoop()
{
	MSG msg;

	while(1)											
	{	
			
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
			{
				break;
			}

            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else
		{
			//render and update our scene in here
			scene->buildOctree();

			scene->render();

			SwapBuffers(g_hDC);

			scene->update();

			user::getInstance()->getPadStates();
		}
	}

	DeInit();											// Free everything after the program finishes

	return(msg.wParam);									// Return from the program
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

	//HERE WE GET ANOTHER FUNCTION TO DEAL WITH KEY PRESSES
	user::getInstance()->getInput(wParam,uMsg);

    switch (uMsg)
	{
		case WM_SIZE:										

			//WINDOW HAS BEEN RESIZED
			wglMakeCurrent(g_hDC, g_hRC);
			g_screenWidth = LOWORD(lParam);
			g_screenHeight = HIWORD(lParam);
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));

			return 0;

		break; 

	case WM_MOUSEMOVE:

		//OBTAIN MOUSE COORDINATE
		user::getInstance()->input.mousePos.x = GET_X_LPARAM(lParam); 
		user::getInstance()->input.mousePos.y = GET_Y_LPARAM(lParam); 

	break;
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct	
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_CLOSE:				
		// If the window is being closed
        PostQuitMessage(0);	
        break; 
     
    default:									
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
      break; 
    }

    return lRet;
}
