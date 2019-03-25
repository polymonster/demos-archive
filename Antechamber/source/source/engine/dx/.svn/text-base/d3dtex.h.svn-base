
#ifndef _D3DTEX_H_
#define _D3DTEX_H_

#include <windows.h>
#include <windowsx.h>

#include <d3dx9.h>
#include <d3d9.h>

#pragma comment ( lib, "d3d9.lib" )
#pragma comment ( lib, "d3dx9.lib" )

typedef struct
{
	IDirect3DTexture9	*texture;
	unsigned short		w;
	unsigned short		h;
}tga_texture_s;

tga_texture_s load_d3d_texture( char *filename, bool get_dims = true );


#endif