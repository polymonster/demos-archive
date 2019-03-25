#include "main.h"										// This includes our header file
#include "timerSingleton.h"
#include "Toolbars.h"
#include "modelManager.h"
#include "textureSingleton.h"	
#include <iostream>

CScene *organGrinder;
commandList g_undoStack;
commandList g_redoStack;

bool initialised;

CFrustum g_frustumCulling;


void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	g_screenWidth = g_rRect.right;						// get screen widths
	g_screenHeight = g_rRect.bottom;

	createMainViewport();
	
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	//initialise organGrinder
	organGrinder = new CScene();

	initialiseToolbars();
	
	g_input.cameraMode = 0;

	wglShareLists(g_hRC,g_hRCTextureManager);
	wglShareLists(g_hRC,g_hRCMod);
	wglShareLists(g_hRC,g_hRCPaintTexture);
	wglShareLists(g_hRC,g_hRCFoliage);
	wglShareLists(g_hRC,g_hRCProps);
	wglShareLists(g_hRC,g_hRCWaterTexture);
	wglShareLists(g_hRC,g_hRCBloomColor);
	wglShareLists(g_hRC,g_hRCSkybox);
	wglShareLists(g_hRC,g_hRCSkyboxViewer);

	initialised = true;

	#ifdef _USE_TAB

		hTab = TabletInit(hWnd);

	#endif

	CTextureMan::getInstance()->cubeMapConfig("data\\texture\\skyboxes\\sunset.txt");

	g_shaderId.bloom = CShaderManager::getInstance()->loadShader("Shaders/gaussian.glsl");
	g_shaderId.terrain = CShaderManager::getInstance()->loadShader("Shaders/terrain/terrain.glsl");
	g_shaderId.grass = CShaderManager::getInstance()->loadShader("Shaders/punchout.glsl");
	g_shaderId.normalMap = CShaderManager::getInstance()->loadShader("Shaders/normalMap/normalMap.glsl");
	g_shaderId.water = CShaderManager::getInstance()->loadShader("Shaders/water/water.glsl");
	g_shaderId.envMap = CShaderManager::getInstance()->loadShader("Shaders/envMap/env.glsl");
	g_shaderId.skybox = CShaderManager::getInstance()->loadShader("Shaders/skybox/skybox.glsl");
	g_shaderId.brightPass = CShaderManager::getInstance()->loadShader("Shaders/brightPass/bright.glsl");
	
	//show HWND when were fully loaded
	ShowWindow(g_hWnd, SW_SHOWNORMAL);
	ShowWindow(g_hMainViewport,SW_SHOW);
	ShowWindow(g_hSplash, SW_HIDE);
	SetFocus(g_hWnd);
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
				//organGrinder->cleanUp();

				delete organGrinder;

				//WTClose(hTab);

				break;
			}

            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else
		{

			wglMakeCurrent(g_hDC, g_hRC);	

			organGrinder->render();
			organGrinder->update();

			SwapBuffers(g_hDC);

			CModelManager::getInstance()->enumerateModels();
			CTextureMan::getInstance()->textureManagerWindow();

			checkViewportSize();
		}
	}

	DeInit();											// Free everything after the program finishes

	return(msg.wParam);									// Return from the program

}

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

	user::getInstance()->getInput(wParam,uMsg);

	//int nPacks;

    switch (uMsg)
	{
		case WM_SIZE:										// If the window is resized

			wglMakeCurrent(g_hDC, g_hRC);

			g_screenWidth = LOWORD(lParam);
			g_screenHeight = HIWORD(lParam);

			SizeOpenGLScreen(g_screenWidth,g_screenHeight);// LoWord=Width, HiWord=Height

			resizeToolbars();

			return 0;

		break;

		case WM_MOVE:
			
			resizeToolbars();

		break;

		case WM_COMMAND:		
			
			// This message is sent when the user selects a command item from a menu, and some other things.
			if(LOWORD(wParam) == ID__QUIT)					// If we check the Low-bits (LOWORD) of the DWORD wParam we can see what menu item they clicked on.
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);			// If they clicked on the menu ID: IDM_QUIT, then we want to close the window so we send that message.
			}
			else if(LOWORD(wParam) == ID_TEXTURE_MANAGER)
			{
				ShowWindow(g_hTextureManager, SW_SHOW);
			}
			else if(LOWORD(wParam) ==  ID_MODEL_MANAGER)
			{
				ShowWindow(g_hMod, SW_SHOW);
			}
			else if(LOWORD(wParam) ==  ID_TOOLS_POLYGONCOUNTER)
			{
				char count[10];
				_itoa(g_polyCounter.polyCount,count,10);
				SetDlgItemText(g_hPolyCounter,IDC_POLY_NUMBERS,count);
				//ShowWindow(g_hPolyCounter, SW_SHOW);
			}

			organGrinder->sceneOptionHandler(wParam);
			CShaderManager::getInstance()->shaderOptionHandler(wParam);

		break;

	case WM_MOUSEMOVE:

		g_input.mousePos.x = GET_X_LPARAM(lParam); 
		g_input.mousePos.y = GET_Y_LPARAM(lParam); 

	break;
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct	
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
	break;

	case WM_ERASEBKGND:

		if(initialised)
		{
			wglMakeCurrent(g_hDC, g_hRC);
			organGrinder->render();
			//organGrinder->update();
			SwapBuffers(g_hDC);
		}
		return 0;

	break;

	case WM_CLOSE:									// If the window is being closed
        PostQuitMessage(0);							// Post a QUIT message to the window;										// Release memory and restore settings
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    }

    return lRet;										// Return by default
}
