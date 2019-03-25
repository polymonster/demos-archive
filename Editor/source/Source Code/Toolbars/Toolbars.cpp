#include "Toolbars.h"
#include "Input.h"
#include "main.h"

int xrot = 0;

BOOL CreateToolTip(int toolID, HWND hDlg, LPSTR text)
{
    // toolID:  the resource ID of the control.
    // hDlg:    the handle of the dialog box.
    // pText:   the text that appears in the ToolTip.
    // g_hInst: the global instance handle.
    
    if (!toolID || !hDlg)
    {
        return FALSE;
    }
    // Get the window of the tool.
    HWND hwndTool = GetDlgItem(g_hMainTool, toolID);
    
    // Create the ToolTip.
    HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
                              WS_POPUP | TTS_ALWAYSTIP | TTF_PARSELINKS ,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              g_hMainTool, NULL, 
                              g_hInstance, NULL);

	RECT rTooltip;

	GetClientRect(hwndTip, &rTooltip);

                              
   if (!hwndTool || !hwndTip)
   {
       return FALSE;
   }                              
                              
    // Associate the ToolTip with the tool.
    TOOLINFO toolInfo = { 0 };
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = g_hMainTool;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)hwndTool;
    toolInfo.lpszText = text;

    SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

    return TRUE;
}

void splashScreen()
{
	g_hSplash = CreateDialog(g_hInstance, (LPCTSTR)IDD_SPLASH_SCREEN, NULL, (DLGPROC)toolDlgProc);
	ShowWindow(g_hSplash, SW_SHOW);
}

void initialiseToolbars()
{
	propertiesPaneActive = false;

	//initialise UI features
	creatMainStatus();
	createMainToolbar();
	createPolygonCounter();
	createOctreeTools();
	createViewTools();

	createTextureMan();
	createModelMan();
	createTerrainTools();
	createPropTools();
	createFoliageToolbar();
	createWaterToolbar();
	createAtmosphereTools();
	
	resizeToolbars();
}

void createMainViewport()
{
	g_hMainViewport = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_MAIN_PORT), g_hWnd, (DLGPROC)viewportProc);

	//ShowWindow(g_hMainViewport, SW_SHOW);
}

void createMainToolbar()
{
	g_hMainTool = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_MAIN_BAR), g_hWnd, mainToolbarProc);

	RECT rcClient;
	GetClientRect(g_hWnd, &rcClient);
	SetWindowPos(g_hMainTool, NULL, rcClient.left,rcClient.top,rcClient.right,rcClient.top + MAIN_TOOL_HEIGHT, SWP_NOZORDER);
	
	HBITMAP hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/preview.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND buu = GetDlgItem(g_hMainTool, IDC_HOME);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/terrainmode.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_TERRAIN_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/viewsettings.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_VIEW_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/propmode.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_PROP_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/foliagemode.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_FOLIAGE_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/watermode.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_WATER_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/envmode.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_ATMOSPHERE_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/load.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_LOAD);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/save.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_SAVE2);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/undo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_UNDO);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/redo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_REDO);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/copy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_COPY);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/octreemode.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hMainTool, IDC_OPEN_OCTREE_MENU);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	/*CreateToolTip(IDC_HOME,g_hMainTool,"Home");
	CreateToolTip(IDC_OPEN_VIEW_MENU,g_hMainTool,"View - Adjust camera and guide settings");
	CreateToolTip(IDC_OPEN_TERRAIN_MENU,g_hMainTool,"Terrain mode - Sculpt, paint and edit terrain properties");
	CreateToolTip(IDC_OPEN_PROP_MENU,g_hMainTool,"Props - Add props and objects to the scene");
	CreateToolTip(IDC_OPEN_FOLIAGE_MENU,g_hMainTool,"Foliage mode - Place organic props, quickly");
	CreateToolTip(IDC_OPEN_WATER_MENU,g_hMainTool,"Water mode - Add water planes, and modify their properties");*/

	ShowWindow(g_hMainTool, SW_SHOW);

}

void createTextureMan()
{
	g_hTextureManager = CreateDialog(g_hInstance, (LPCTSTR)IDD_TEXTURE_MAN, g_hWnd, (DLGPROC)textureManagerProc);
	
	//setup the combo box to choose sizes
	HWND combo = GetDlgItem(g_hTextureManager, IDC_COMBO_SIZES);
	
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"32");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"64");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"128");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"256");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"512");

	SendMessage(combo, CB_SETCURSEL, 1, 0);

	TCITEM tabItems;
	HWND tab = GetDlgItem(g_hTextureManager,IDC_TEXTURE_TYPES);
	tabItems.mask = TCIF_TEXT | TCIF_IMAGE; 
    tabItems.iImage = -1; 
    tabItems.pszText = "Textures"; 
    TabCtrl_InsertItem(tab, 0, &tabItems); 
    tabItems.pszText = "Cube Maps"; 
    TabCtrl_InsertItem(tab, 1, &tabItems); 

	createContext(g_hTextureManager,&g_hDCSkybox,&g_hRCSkybox,IDC_SKYBOX_WINDOW);
}

void createModelMan()
{
	g_hMod = CreateDialog(g_hInstance, (LPCTSTR)IDD_MODEL_MANAGER, g_hWnd, (DLGPROC)modelManagerProc);

	HWND combo = GetDlgItem(g_hMod, IDC_COMBO_CULLS);
	
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Back");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"Front");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"None");

	SendMessage(combo, CB_SETCURSEL, 0, 0);
}

void setPropertyDimensions()
{
	//calculate the position of the toolbox
	RECT rToolbar;
	RECT rProperty;
	RECT rClient;
	RECT rStatus;

	GetWindowRect(g_hMainTool, &rToolbar);
	GetWindowRect(g_hTerrainProp, &rProperty);
	GetWindowRect(g_hStatus, &rStatus);
	GetClientRect(g_hWnd, &rClient);
	
	g_toolboxRect.left = rClient.right - (rProperty.right - rProperty.left);
	g_toolboxRect.right = (rProperty.right - rProperty.left);
	g_toolboxRect.top = rClient.top + (rToolbar.bottom - rToolbar.top);
	g_toolboxRect.bottom = rClient.bottom - (rStatus.bottom - rStatus.top) - g_toolboxRect.top;
}

void createTerrainTools()
{
	//setup the dialog bar itself
	g_hTerrainProp = CreateDialog(g_hInstance, (LPCTSTR)IDD_TERRAIN_PROP, g_hWnd, (DLGPROC)toolDlgProc);
	
	//setup the tabs
	TCITEM tabItems;
	HWND tab = GetDlgItem(g_hTerrainProp,IDC_TAB_TERRAIN_OPTIONS);
	tabItems.mask = TCIF_TEXT | TCIF_IMAGE; 
    tabItems.iImage = -1; 
    tabItems.pszText = "Properties"; 
    TabCtrl_InsertItem(tab, 0, &tabItems); 
    tabItems.pszText = "Sculpt"; 
    TabCtrl_InsertItem(tab, 1, &tabItems); 
    tabItems.pszText = "Paint"; 
    TabCtrl_InsertItem(tab, 2, &tabItems); 

	//ShowWindow(g_hTerrainProp, SW_SHOW);

	//setup the paint dialog
	g_hTerrainPaint = CreateDialog(g_hInstance, (LPCTSTR)IDD_TERRAIN_TEXTURE, g_hWnd, (DLGPROC)toolDlgProc);
	createContext(g_hTerrainPaint,&g_hDCPaintTexture,&g_hRCPaintTexture,IDC_ACTIVE_TEXTURE);
	
	HWND combo = GetDlgItem(g_hTerrainPaint, IDC_COMBO1);
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"1");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"2");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"3");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"4");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"5");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"6");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"7");
	SendMessage(combo, CB_SETCURSEL, 0, 0);

	//ShowWindow(g_hTerrainPaint, SW_SHOW);

	//setup the paint dialog
	g_hTerrainHeight = CreateDialog(g_hInstance, (LPCTSTR)IDD_TERRAIN_HEIGHT, g_hWnd, (DLGPROC)toolDlgProc);
	
	combo = GetDlgItem(g_hTerrainHeight, IDC_SCULPT_MODES);
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"Sculpt");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"Flatten");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"Smooth");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"Noise");
	SendMessage(combo, CB_SETCURSEL, 0, 0);

	g_hTerrainValues = CreateDialog(g_hInstance, (LPCTSTR)IDD_TERRAIN_VALUES, g_hWnd, (DLGPROC)toolDlgProc);

	combo = GetDlgItem(g_hTerrainValues, IDC_TERRAIN_GRID_SIZE);
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"50");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"100");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"200");
	SendMessage(combo, CB_SETCURSEL, 0, 0);

	combo = GetDlgItem(g_hTerrainValues, IDC_TERRAIN_TILE_SIZE);
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"10");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"20");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"50");
	SendMessage(combo, CB_SETCURSEL, 0, 0);

	//ShowWindow(g_hTerrainHeight, SW_SHOW);
}

void creatMainStatus()
{
	g_hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
        g_hWnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

	int statwidths[] = {700, 800, 900, -1};

    SendMessage(g_hStatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
    SendMessage(g_hStatus, SB_SETTEXT, 0, (LPARAM)"Editor Loaded");
}

void createViewTools()
{
	g_hViewTool = CreateDialog(g_hInstance, (LPCTSTR)IDD_VIEW_PROP, g_hWnd, (DLGPROC)toolDlgProc);

	//HWND viewGrid = GetDlgItem(g_hViewTool,IDC_TOGGLE_GRID);

	SetDlgItemText(g_hViewTool,IDC_GRID_SIZE, "20"); 
	SetDlgItemText(g_hViewTool,IDC_CELL_SIZE, "20"); 
}

void createPropTools()
{
	g_hPropProp = CreateDialog(g_hInstance, (LPCTSTR)IDD_PROP_PROP, g_hWnd, (DLGPROC)propProc);
	createContext(g_hPropProp,&g_hDCProps,&g_hRCProps,IDC_PROP_MODEL_VIEW);

	//set images
	HBITMAP hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/translate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND buu = GetDlgItem(g_hPropProp, IDC_TRANSLATE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/scale.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hPropProp, IDC_SCALE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/add.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hPropProp, IDC_ADD_PROP);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/delete.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hPropProp, IDC_DELETE_PROP);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/select.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hPropProp, IDC_SELECT_PROP);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);
}

void createFoliageToolbar()
{
	g_hFoliageProp = CreateDialog(g_hInstance, (LPCTSTR)IDD_FOLIAGE_PROP, g_hWnd, (DLGPROC)propProc);
	createContext(g_hFoliageProp,&g_hDCFoliage,&g_hRCFoliage,IDC_FOLIAGE_MODEL_VIEW);

	char text[32];
	
	strcpy(text,"1");
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_SCALE_FROM, text); 
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_SCALE_TO, text); 

	strcpy(text,"0");
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_ROTATE_FROM, text);

	strcpy(text,"360");
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_ROTATE_TO, text);

	strcpy(text,"0");
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_OFFSET_X, text);
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_OFFSET_Y, text);
	SetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_OFFSET_Z, text);
}

void createPolygonCounter()
{
	g_hPolyCounter = CreateDialog(g_hInstance, (LPCTSTR)IDD_POLY_COUNT, g_hWnd, (DLGPROC)toolDlgProc);
	ShowWindow(g_hPolyCounter, SW_HIDE);
}

void createWaterToolbar()
{
	g_hWaterProp = CreateDialog(g_hInstance, (LPCTSTR)IDD_WATER_PROP, g_hWnd, (DLGPROC)toolDlgProc);
	g_hWaterOptical = CreateDialog(g_hInstance, (LPCTSTR)IDD_WATER_OPTICAL, g_hWnd, (DLGPROC)toolDlgProc);
	g_hWaterTexture = CreateDialog(g_hInstance, (LPCTSTR)IDD_WATER_TEXTURE, g_hWnd, (DLGPROC)toolDlgProc);
	g_hWaterManagement = CreateDialog(g_hInstance, (LPCTSTR)IDD_WATER_MANAGEMENT, g_hWnd, (DLGPROC)toolDlgProc);

	createContext(g_hWaterTexture,&g_hDCWaterTexture,&g_hRCWaterTexture,IDC_ACTIVE_WATER_TEXTURE);

	HWND combo = GetDlgItem(g_hWaterOptical, IDC_COMBO_WATER_TYPES);
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"EMBM");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"EMBM + Local");
	SendMessage(combo, CB_SETCURSEL, 0, 0);

	SetDlgItemText(g_hWaterOptical, IDC_REFDIS_X,"0.0"); 
	SetDlgItemText(g_hWaterOptical, IDC_REFDIS_Y,"0.0"); 
	SetDlgItemText(g_hWaterOptical, IDC_RAFDIS_X,"0.0"); 
	SetDlgItemText(g_hWaterOptical, IDC_RAFDIS_Y,"0.0"); 
	SetDlgItemText(g_hWaterTexture, IDC_ANIM_SPEED_X,"0.0"); 
	SetDlgItemText(g_hWaterTexture, IDC_ANIM_SPEED_Y,"0.0");

	//setup the tabs
	TCITEM tabItems;
	HWND tab = GetDlgItem(g_hWaterProp,IDC_TAB_WATER_OPTIONS);
	tabItems.mask = TCIF_TEXT | TCIF_IMAGE; 
    tabItems.iImage = -1; 
    tabItems.pszText = "Planes"; 
    TabCtrl_InsertItem(tab, 0, &tabItems); 
    tabItems.pszText = "Optical"; 
    TabCtrl_InsertItem(tab, 1, &tabItems); 
    tabItems.pszText = "Texture"; 
    TabCtrl_InsertItem(tab, 2, &tabItems); 

	//set the textures
	HBITMAP hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/translate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND buu = GetDlgItem(g_hWaterManagement, IDC_WATER_TRANSLATE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/scale.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hWaterManagement, IDC_WATER_SCALE2);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/add.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hWaterManagement, IDC_WATER_ADD);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/delete.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hWaterManagement, IDC_WATER_DELETE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/select.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hWaterManagement, IDC_WATER_SELECT);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);
}

void createAtmosphereTools()
{
	g_hAtmosphereProp = CreateDialog(g_hInstance, (LPCTSTR)IDD_ATMOSPHERE_PROP, g_hWnd, (DLGPROC)toolDlgProc);
	g_hAtmosphereEnv = CreateDialog(g_hInstance, (LPCTSTR)IDD_ATMOSPHERE_ENV, g_hWnd, (DLGPROC)toolDlgProc);
	g_hAtmosphereSky = CreateDialog(g_hInstance, (LPCTSTR)IDD_ATMOSPHERE_SKY, g_hWnd, (DLGPROC)toolDlgProc);
	g_hAtmosphereManagement = CreateDialog(g_hInstance, (LPCTSTR)IDD_ATMOSPHERE_MANAGEMENT, g_hWnd, (DLGPROC)toolDlgProc);
	createContext(g_hAtmosphereEnv,&g_hDCBloomColor,&g_hRCBloomColor,IDC_BLOOM_COLOR);
	createContext(g_hAtmosphereSky,&g_hDCSkyboxViewer,&g_hRCSkyboxViewer,IDC_SKYBOX_VIEWER);


	TCITEM tabItems;
	HWND tab = GetDlgItem(g_hAtmosphereProp,IDC_TAB_ATMOSPHERE_OPTIONS);
	tabItems.mask = TCIF_TEXT | TCIF_IMAGE; 
    tabItems.iImage = -1; 
    tabItems.pszText = "Add"; 
    TabCtrl_InsertItem(tab, 0, &tabItems); 
    tabItems.pszText = "Environment"; 
    TabCtrl_InsertItem(tab, 1, &tabItems); 
    tabItems.pszText = "Sky Box"; 
    TabCtrl_InsertItem(tab, 2, &tabItems); 

	SetDlgItemText(g_hAtmosphereEnv,IDC_BLOOM_PASSES,"1");

	HWND combo = GetDlgItem(g_hAtmosphereEnv, IDC_SHADOW_SIZE);
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"4096");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"512");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"1024");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"2048");
	
	SendMessage(combo, CB_SETCURSEL, 1, 0);

	combo = GetDlgItem(g_hAtmosphereEnv, IDC_BLOOM_SIZE);
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"128");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"256");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"512");
	SendMessage(combo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)"1024");
	SendMessage(combo, CB_SETCURSEL, 1, 0);

	combo = GetDlgItem(g_hAtmosphereEnv, IDC_SHADOW_REAL_TIME);
	SendMessage(combo, CB_INSERTSTRING, 0, (LPARAM)(LPCTSTR)"Yes");
	SendMessage(combo, CB_INSERTSTRING, 0, (LPARAM)(LPCTSTR)"No");
	SendMessage(combo, CB_SETCURSEL, 1, 0);

	//set images
	HBITMAP hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/add.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND buu = GetDlgItem(g_hAtmosphereManagement, IDC_ATMOSPHERE_ADD);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/delete.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hAtmosphereManagement, IDC_ATMOSPHERE_DELETE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/translate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hAtmosphereSky, IDC_ATMOSPHERE_TRANSLATE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/scale.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hAtmosphereSky, IDC_ATMOSPHERE_SCALE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	


}

void createOctreeTools()
{
	g_hOctree = CreateDialog(g_hInstance, (LPCTSTR)IDD_OCTREE_PROP, g_hWnd, (DLGPROC)toolDlgProc);

	SetDlgItemText(g_hOctree,IDC_OCTREE_NODE_SIZE,"100");
	SetDlgItemText(g_hOctree,IDC_OCTREE_OBJECTS,"10");

	//set the images
	HBITMAP hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/translate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND buu = GetDlgItem(g_hOctree, IDC_OCTREE_TRANSLATE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);

	hbmp = (HBITMAP)LoadImage(0, "data/interface/icons/scale.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	buu = GetDlgItem(g_hOctree, IDC_OCTREE_SCALE);
	SendMessage(buu,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)hbmp);
}




