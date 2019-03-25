/****************************************
/	Al (c) 2008 - d3dapp.h
/****************************************
/	Direct3D handler
/****************************************/

#ifndef _D3DAPP_H_
#define _D3DAPP_H_

/*************** INCLUDES ***************/

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "2dscene.h"



/*************** CONSTANTS **************/

#define		FRAMERATE			16.666667f


const unsigned int	CUSTOM_2DQUAD_FVF	=	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
const unsigned int	CUSTOM_STRIP_FVF	=	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2 );

/*********** GLOBAL VARIABLES ***********/

typedef enum
{

	WINDOWED_WIDTH	=	640,
	WINDOWED_HEIGHT	=	480,
	FULL_WIDTH		=	640,
	FULL_HEIGHT		=	480

}window_dims_e;

extern bool					g_widescreen;
extern bool					g_fullscreen;
extern bool					g_editor_mode;

extern int					g_window_x;
extern int					g_window_y;
extern int					g_window_w;
extern int					g_window_h;

extern unsigned int			g_clear_col;

extern LPDIRECT3D9			g_d3d;				// the pointer to our Direct3D interface
extern LPDIRECT3DDEVICE9	g_d3d_device;		// the pointer to the device class

extern HWND					g_hWnd;



//#ifndef FULLSCREEN
//#define		WINDOW_X			200
//#define		WINDOW_Y			200
//#define		WINDOW_MODE			WS_EX_TOPMOST | WS_POPUP
//#define		FULL_MODE			1
//#else
//#define		WINDOW_X			0
//#define		WINDOW_Y			0
//#define		WINDOW_MODE			WS_EX_TOPMOST | WS_POPUP
//#define		FULL_MODE			0
//#endif

//#ifndef WIDESCREEN
//#define		WINDOW_W			640
//#define		WINDOW_H			256
//#else
//#define		WINDOW_W			640
//#define		WINDOW_H			480
//#endif


/********* FUNCTION PROTOTYPES **********/

void				init_d3d		( HWND hWnd );		// sets up and initializes Direct3D
void				render_frame	( void );			// renders a single frame
void				cleanup_d3d		( void );			// closes Direct3D and releases memory


#endif