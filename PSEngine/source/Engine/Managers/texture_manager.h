/*=========================================================*\
|	texture_manager.h - texture resources
|-----------------------------------------------------------|
|				Project :	PolySpoon Texture Manager
|				Coder	:	ABleas
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _TEXTUREMGR_H
#define _TEXTUREMGR_H

/*======================== INCLUDES =======================*/

#include <windows.h>
#include <string>

#include "window.h"
#include "console.h"

/*======================== CONSTANTS ======================*/

#define		MAX_LOADED_TEXTURES		256

/*========================= CLASSES =======================*/

class TGAAttributes
{
public:
	TGAAttributes() {}

	unsigned char attribute		:4;
	unsigned char horizontal	:1;		// horizontal texture orientation
	unsigned char vertical		:1;		// vertical texture orientation
	unsigned char inter			:2;
};

class TGAHeader
{
public:
	TGAHeader() {}

	unsigned char		image_type;		// RLE compression or BGR type
	unsigned char		image_start;	// gap after header before data
	unsigned char		bpp;			// bits per pixel
	unsigned char		*palette_p;		// pointer to the start of palette data
	unsigned char		*indices_p;		// pointer to the start of index texture data
	unsigned char		*data_p;		// pointer to the start of texture data
	unsigned short		width;
	unsigned short		height;
	TGAAttributes		attributes;		// orientation information

};

class TextureSlot
{
public:
	TextureSlot() {}

	GLuint	gl_handle;
	char	filename[128];
};

class TextureDefaults
{
public:

	static int normal_map;
	static int specular_map;
	static int parallax_map;
	static int no_texture;
	static int test_cube;
	static int materials_lookup;
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void		init				( void );

	int			load_texture		( char *filename_p );

	int			get_texture_id		( char *filename_p );
	char		*get_texture_name	( int id );

	void		bind_texture		( char *filename_p, int texture_unit = 0 );
	void		bind_texture		( int id, int texture_unit = 0 );
	void		invalidate_unit		( int unit, bool inavlidate_enabled = false );

	void		bind_cubemap( int id, int texture_unit = 0 );
	void		unbind_cubemap(int unit);

	void		disable_all_units();

	int		load_cubemap(char *front,char *back,char *left,char *right,char *top,char *bottom,char *name);
	int		load_cubemap(std::string filename);

private:

	void		preload_resources	( void );
	TGAHeader	*load_texture_data	( char *filename_p );
	void		bgr_to_rgb			( unsigned char *texture_data_p, int width, int height, int bytes_per_texel );
	void		flip_vertical		( unsigned char *texture_data_p, int width, int height, int bytes_per_texel );
	void		flip_horizontal		( unsigned char *texture_data_p, int width, int height, int bytes_per_texel );

	TextureSlot	m_texture_slot[MAX_LOADED_TEXTURES];

	unsigned int m_num_loaded_textures;
	int			 m_max_texture_units;

	int			 m_texture_unit_cache[8];
	int			 m_texture_unit_enabled[8];

};


/*================== EXTERNAL VARIABLES ===================*/


extern TextureManager g_texture_mgr;


#endif //_TEXTUREMGR_H
