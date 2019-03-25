#include "Toolbars.h"
#include "Input.h"
#include "main.h"
#include "helpers.h"
#include "textureSingleton.h"
#include "modelManager.h"
#include "SceneObject.h"
#include "props.h"
#include "water.h"

int currentToolbar;

BOOL CALLBACK mainToolbarProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	user::getInstance()->getInput(wParam,Message);

    switch(Message)
    {
        case WM_COMMAND:

			switch(LOWORD(wParam))
			{

				//MAIN MENU
				case IDC_HOME :
					
					g_editMode = NO_EDIT_MODE;

				break;

				case IDC_OPEN_VIEW_MENU :
				
					g_editMode = EDIT_VIEW;

				break;

				case IDC_OPEN_TERRAIN_MENU :
				
					g_editMode = EDIT_TERRAIN;

				break;

				case IDC_OPEN_PROP_MENU :
				
					g_editMode = EDIT_PROPS;

				break;

				case IDC_OPEN_FOLIAGE_MENU :
				
					g_editMode = EDIT_FOLIAGE;

				break;

				case IDC_OPEN_WATER_MENU :
				
					g_editMode = EDIT_WATER;

				break;

				case IDC_OPEN_ATMOSPHERE_MENU :
				
					g_editMode = EDIT_ATMOSPHERE;

				break;

				case IDC_OPEN_OCTREE_MENU :
				
					g_editMode = EDIT_OCTREE;

				break;

				case IDC_WATER_SELECT :
				
					g_waterEditMode = SELECT_WATER;

				break;

				

			default:
			break;


			}

        break;
        default:

        return FALSE;
    }

    return TRUE;
}

LRESULT CALLBACK viewportProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	user::getInstance()->getInput(wParam,Message);

	switch (Message)
	{
		case WM_MOUSEMOVE:

		g_input.mousePos.x = GET_X_LPARAM(lParam); 
		g_input.mousePos.y = GET_Y_LPARAM(lParam); 

		g_input.mousePos.y += g_toolboxRect.top + 21;

		break;
	}

    return FALSE;
}

BOOL CALLBACK toolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_COMMAND:

			switch(LOWORD(wParam))
			{
			
			case IDC_OPEN_MODEL_MANAGER:
			case IDC_OPEN_MODEL_MANAGER_P:
				
				ShowWindow(g_hMod, SW_SHOW);

			break;

			default:

				return FALSE;
				
			break;
			}

        break;

        default:

			return FALSE;

		break;
    }

    return TRUE;
}

void setPropertiesWindows(HWND *hWnd)
{
	SetWindowPos(*hWnd, NULL, 
	g_toolboxRect.left,
	g_toolboxRect.top,
	g_toolboxRect.right, 
	g_toolboxRect.bottom, 
	SWP_NOZORDER);

}

void setSubPropertiesWindows(HWND *hWnd)
{
	RECT rPrpTab;
	HWND tab = GetDlgItem(g_hTerrainProp,IDC_TAB_TERRAIN_OPTIONS);
	GetWindowRect(tab, &rPrpTab);

	g_innerToolboxRect.left = rPrpTab.left + 1;
	g_innerToolboxRect.right = (rPrpTab.right - rPrpTab.left) - 4;
	g_innerToolboxRect.top = rPrpTab.top + 21;
	g_innerToolboxRect.bottom = (rPrpTab.bottom - rPrpTab.top) - 25;

		
	//set the sub property tabs
	SetWindowPos(*hWnd, NULL, 
	g_innerToolboxRect.left,
	g_innerToolboxRect.top,
	g_innerToolboxRect.right, 
	g_innerToolboxRect.bottom, 
	SWP_NOZORDER);

}

void resizeToolbars()
{
		//some vars to get the positions of all the toolbars

		// Get the window rectangle
		GetClientRect(g_hWnd, &g_rRect);
		SetWindowPos(g_hMainTool, NULL, g_rRect.left,g_rRect.top,g_rRect.right,g_rRect.top + MAIN_TOOL_HEIGHT, SWP_NOZORDER);

		//resize status bar
		HWND statusBar;
		statusBar = GetDlgItem(g_hWnd, IDC_MAIN_STATUS);
		SendMessage(statusBar, WM_SIZE, 0, 0);

		//resize the properties box
		setPropertyDimensions();

		//reset the properties windows
		setPropertiesWindows(&g_hTerrainProp);
		setPropertiesWindows(&g_hViewTool);
		setPropertiesWindows(&g_hPropProp);
		setPropertiesWindows(&g_hFoliageProp);
		setPropertiesWindows(&g_hWaterProp);
		setPropertiesWindows(&g_hAtmosphereProp);
		setPropertiesWindows(&g_hOctree);

		//and the sub properties windows
		setSubPropertiesWindows(&g_hTerrainValues);
		setSubPropertiesWindows(&g_hTerrainHeight);
		setSubPropertiesWindows(&g_hTerrainPaint);
		setSubPropertiesWindows(&g_hWaterOptical);
		setSubPropertiesWindows(&g_hWaterTexture);
		setSubPropertiesWindows(&g_hWaterManagement);
		setSubPropertiesWindows(&g_hAtmosphereManagement);
		setSubPropertiesWindows(&g_hAtmosphereEnv);
		setSubPropertiesWindows(&g_hAtmosphereSky);

		if(propertiesPaneActive)
		{
				SetWindowPos(g_hMainViewport, NULL,
				0,
				g_toolboxRect.top,
				g_toolboxRect.left, 
				g_rRect.bottom - (g_rRect.bottom - g_toolboxRect.bottom),
				SWP_NOZORDER);

		}
		else
		{
				SetWindowPos(g_hMainViewport, NULL,
				0,
				g_toolboxRect.top,
				g_rRect.right, 
				g_rRect.bottom - (g_rRect.bottom - g_toolboxRect.bottom),
				SWP_NOZORDER);
		}
}

void checkViewportSize()
{
	if(propertiesPaneActive)
	{
		if(g_editMode == NO_EDIT_MODE)
		{
			SetWindowPos(g_hMainViewport, NULL,
			0,
			g_toolboxRect.top,
			g_rRect.right, 
			g_rRect.bottom - (g_rRect.bottom - g_toolboxRect.bottom),
			SWP_NOZORDER);

			propertiesPaneActive = false;

		}
	}
	else
	{
		if(g_editMode != NO_EDIT_MODE)
		{
			SetWindowPos(g_hMainViewport, NULL,
			0,
			g_toolboxRect.top,
			g_toolboxRect.left, 
			g_rRect.bottom - (g_rRect.bottom - g_toolboxRect.bottom),
			SWP_NOZORDER);

			propertiesPaneActive = true;
		}
	}
}


LRESULT CALLBACK textureManagerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hOpenGL;
	static DWORD lpThreadId;

	switch (message)
	{
		case WM_INITDIALOG:

				createContext(hDlg,&g_hDCTextureManager,&g_hRCTextureManager,IDC_TEXTURE_WINDOW);

			return TRUE;

		case WM_COMMAND:


		switch(LOWORD(wParam))
		{
			case IDC_CLOSE:

				ShowWindow(g_hTextureManager, SW_HIDE);

			break;
		}

		break;

		case WM_MOUSEMOVE:

		CTextureMan::getInstance()->input.mousePos.x = GET_X_LPARAM(lParam); 
		CTextureMan::getInstance()->input.mousePos.y = GET_Y_LPARAM(lParam); 
		
		g_input.viewerMousePos.x = GET_X_LPARAM(lParam); 
		g_input.viewerMousePos.y = GET_Y_LPARAM(lParam); 

		break;

		case WM_LBUTTONDOWN:

		CTextureMan::getInstance()->input.leftMouse = true; 

		break;

		case WM_LBUTTONUP:

		CTextureMan::getInstance()->input.leftMouse = false; 

		break;

		case WM_MBUTTONDOWN:

			g_input.viewerMiddleMouse = true;
			SetFocus(hDlg);

		return 0;

		break;

		case WM_MBUTTONUP:

			g_input.viewerMiddleMouse = false;

			return 0;

		break;

		case WM_MOUSEWHEEL:

			g_input.viewerZoom -= (short) HIWORD(wParam);

			if(g_input.viewerZoom < 10) g_input.viewerZoom = 10;

        break;

		case WM_KEYDOWN:

		if(wParam == VK_SHIFT) g_input.viewerShift = true;

		break;

		case WM_KEYUP:

		if(wParam == VK_SHIFT) g_input.viewerShift = false;

		break;
	}
    return FALSE;
}

LRESULT CALLBACK modelManagerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				
				createContext(hDlg,&g_hDCMod,&g_hRCMod,IDC_MODEL_WINDOW);

			return TRUE;

		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				case ID_CLOSE:

				ShowWindow(hDlg, SW_HIDE);

				break;

				case ID_ADD_MODEL:

				CModelManager::getInstance()->load(openFile("3D Model","3ds"));

				break;
			}

		break;

		case WM_MBUTTONDOWN:

			g_input.viewerMiddleMouse = true;
			SetFocus(hDlg);

			return 0;

		break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:

			SetFocus(hDlg);

			return 0;

		break;

		case WM_MBUTTONUP:

			g_input.viewerMiddleMouse = false;

			return 0;

		break;

		case WM_MOUSEWHEEL:

			g_input.viewerZoom -= (short) HIWORD(wParam);

			if(g_input.viewerZoom < 10) g_input.viewerZoom = 10;

        break;

		case WM_MOUSEMOVE:

		g_input.viewerMousePos.x = GET_X_LPARAM(lParam); 
		g_input.viewerMousePos.y = GET_Y_LPARAM(lParam); 

		break;

		case WM_KEYDOWN:

		if(wParam == VK_SHIFT) g_input.viewerShift = true;

		break;

		case WM_KEYUP:

		if(wParam == VK_SHIFT) g_input.viewerShift = false;

		break;
	}
    return FALSE;
}

LRESULT CALLBACK propProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MBUTTONDOWN:

			g_input.viewerMiddleMouse = true;
			SetFocus(hDlg);

			return 0;

		break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:

			SetFocus(hDlg);

			return 0;

		break;

		case WM_MBUTTONUP:

			g_input.viewerMiddleMouse = false;

			return 0;

		break;

		case WM_MOUSEWHEEL:

			g_input.viewerZoom -= (short) HIWORD(wParam) * 0.3;

        break;

		case WM_MOUSEMOVE:

		g_input.viewerMousePos.x = GET_X_LPARAM(lParam); 
		g_input.viewerMousePos.y = GET_Y_LPARAM(lParam); 

		break;

		case WM_KEYDOWN:

		if(wParam == VK_SHIFT) g_input.viewerShift = true;

		break;

		case WM_KEYUP:

		if(wParam == VK_SHIFT) g_input.viewerShift = false;

		break;
	}
    return FALSE;
}




