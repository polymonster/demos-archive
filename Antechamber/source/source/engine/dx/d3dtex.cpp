
#include "d3dtex.h"
#include "d3dapp.h"


tga_texture_s load_d3d_texture( char *filename, bool get_dims )
{

	tga_texture_s tex;

	// ========================= OPEN IMAGE FILE ==========================

	if( get_dims )
	{
		FILE *image_file_p = fopen( filename, "rb" );

		if( !image_file_p )
		{
			tex.texture = NULL;
			return( tex );
		}

		fseek( image_file_p, 12, 0 );
		fread( &tex.w, sizeof( char ) * 2, 1, image_file_p );
		fread( &tex.h, sizeof( char ) * 2, 1, image_file_p );

		fclose( image_file_p );
	}
	else
	{
		tex.w = 0;
		tex.h = 0;
	}

	D3DXIMAGE_INFO src_info;

	//Use a magenta colourkey
	unsigned int color_key = 0xffffffff;

	// Load image from file using ASCII
	if ( FAILED( D3DXCreateTextureFromFileExA(	g_d3d_device,
												filename,
												0, 0, 1, 0,
												D3DFMT_A8R8G8B8,
												D3DPOOL_MANAGED,
												D3DX_FILTER_NONE,
												D3DX_DEFAULT,
												color_key,
												&src_info,
												NULL,
												&tex.texture ) ) )
	{
		tex.texture = NULL;
		return( tex );
	}

	//Return the newly made texture
	return tex;
}