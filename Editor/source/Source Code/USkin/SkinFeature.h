#ifndef _SKINFEATURE_H__
#define _SKINFEATURE_H__

//Application    Interface
#ifdef __cplusplus  
extern "C"
{
#endif
#define WM_CB_MOUSEMOVE		  WM_USER + 2001
#define WM_CB_LBUTTONDOWN     WM_USER + 2002
#define WM_CB_RBUTTONDOWN	  WM_USER + 2003
#define WM_CB_LBUTTONUP		  WM_USER + 2004
#define WM_CB_RBUTTONUP		  WM_USER + 2005
#define WM_CB_LBUTTONDBLCLK   WM_USER + 2006
	
#define  ERROR_UNKNOWNFILEFORMAT  0x00001
#define  ERROR_WRONGPASSWORD	  0x00002	
	////////////////////////////////////////////////////////////////////////////
	bool __stdcall InitLicenKeys(LPCTSTR licenkey);
	
	bool __stdcall InitializeSkin();
	
	BOOL __stdcall LoadSkinFile(LPCTSTR  pszFileName, LPCTSTR pszPassWord = NULL);
	
	bool __stdcall LoadSkinFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName,
		LPCTSTR pszResourceType, LPCTSTR pszFileName, LPCTSTR pszPassWord = NULL);
	
	BOOL __stdcall RemoveSkin();
	
	bool __stdcall UnLoadSkin();

	bool __stdcall SetShapeWindowSkin(HWND hWnd,int nResID);
	
	void __stdcall SetThemeColour( COLORREF color );
	
	void __stdcall SetWindowSkinHwnd(HWND hWnd, LPCTSTR pszName = NULL);

	void __stdcall SetWindowNoSkinHwnd(HWND hWnd, BOOL bChildNoSkin = TRUE);

	void __stdcall RemoveWindowSkinHwnd(HWND hWnd);

	void __stdcall SetDialogEraseBkgnd(HWND hWnd, BOOL bErase = TRUE);

	BOOL __stdcall SetWindowSkinResID(HWND hWnd, int nResID);
	
	void __stdcall SetMenuNoSkinHwnd(HMENU hMenu);
	
	void  __stdcall DrawSkinImageRes(HDC hdc, LPRECT rect, LPCTSTR pszResName, 
	  int nLeftWidth, int nRightWidth, int nTopHeight, int nBottomHeight,
		BOOL bStretchAll = FALSE, COLORREF clrTrans = 0xff00ff, BOOL bTrans = TRUE,
		LPRECT lpRectSrc = NULL);
	
	void  __stdcall DrawSkinImage(HDC hdc, LPRECT rect, HBITMAP hbitmap, 
		int nLeftWidth, int nRightWidth, int nTopHeight, int nBottomHeight,
		BOOL bStretchAll = FALSE, COLORREF clrTrans = 0xff00ff, BOOL bTrans = TRUE,
		LPRECT lpRectSrc = NULL);
	
	void __stdcall SetDialogBkClipRgn(HWND hWnd,BOOL bClip,BOOL bAllChild);

	BOOL __stdcall SetCaptionButtonVisible(UINT uID, BOOL bVisible);
	
	void __stdcall SetButtonTooltip(HWND hWnd,LPCTSTR pszTooltip);

	HBITMAP __stdcall GetBitmapFromRes(LPCTSTR pszResName);
	
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif






















