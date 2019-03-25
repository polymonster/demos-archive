#ifndef _MAIN_H
#define _MAIN_H

#include "Globals.h"
#include "Input.h"
#include "Helpers.h"

// This is our MAIN() for windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);
// The window proc which handles all of window's messages.
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK viewDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hWndDlg, UINT Msg,WPARAM wParam, LPARAM lParam);
// This controls our main program loop
WPARAM MainLoop();
// This is our own function that makes creating a window modular and easy
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
// This allows us to configure our window for OpenGL and backbuffered
bool bSetupPixelFormat(HDC hdc);
// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);
// This sets up OpenGL
void InitializeOpenGL(int width, int height);
// This initializes the whole program
void Init(HWND hWnd);
// This draws everything to the screen
void RenderScene();
// This frees all our memory in our program
void DeInit();


#endif 
