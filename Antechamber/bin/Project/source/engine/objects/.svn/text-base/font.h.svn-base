/**************************************************************
/	Al (c) 2008 - font.h
/**************************************************************
/	Hardware font class
/
/	Notes:	All characters dealt with in unsigned char, its just
/			easier that way...
/
/**************************************************************/

#ifndef _FONT_H_
#define _FONT_H_

/*************** INCLUDES ***************/

#include <windows.h>
#include <windowsx.h>

#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )

#include "link.h"


/*************** CONSTANTS **************/

#define		MAX_CHRS				256 - 32

#define		COL_WHITE				0xffffffff
#define		COL_RED					0xffff0000
#define		COL_BLUE				0xff0000ff
#define		COL_GREEN				0xff00ff00
#define		COL_BLACK				0xff000000
#define		COL_YELLOW				0xffffff00
#define		COL_GREY				0xffa0a0a0


/**************** CLASSES ***************/

typedef enum
{
	S_COL_WHITE		= 1,
	S_COL_RED,
	S_COL_BLUE,
	S_COL_GREEN,
	S_COL_BLACK,
	S_COL_YELLOW,
	S_COL_GREY,

	S_NUM_COLS,

	S_COL_PAD		= 0xffffffff

}cols_e;

typedef struct
{
	// font texture UVs
	byte u0, u1;

}chr_info_s;

typedef struct
{
	float x, y, z, rhw;
	unsigned int col;
	float u, v;

}chr_vert_s;

typedef struct
{

    byte  header_buff;          // size of ID field that follows 18 byte header (0 usually)
    byte  colour_map;			// type of colour map 0=none, 1=has palette
    byte  imagetype;			// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourmap_start;		// first colour map entry in palette
    short bitdepth;				// number of colours in palette
    byte  colourmap_bits;		// number of bits per palette entry 15,16,24,32

    short xstart;				// image x origin
    short ystart;				// image y origin
    short width;				// image width in pixels
    short height;				// image height in pixels
    byte  bpp;					// image bits per pixel 8,16,24,32
    byte  description;			// image descriptor bits (vh flip bits)
    
    // pixel data follows header
    
}tga_header_s;


class Font: public Link
{

public:

	Font();
	Font( char *font_p, int chr_size, float scale = 1.0f, unsigned int col = 0xffffffff );
	~Font();

//	void				set_params			( float scale = 1.0f, unsigned int col = 0xffffffff );

	void				print_2d_text		( char *string_p, int str_x, int str_y, float scale = 1.0f, unsigned int col = 0xffffffff );

	int					get_chr_width		( unsigned char chr );
	int					get_text_width		( char *string_p );

	void				gen_font_info		( char *font_filename );

	
private:

	void				set_vert			( chr_vert_s *fvf_struct_p, float x, float y, float u, float v, int iterator );

	chr_info_s			m_chr[MAX_CHRS];

	tga_texture_s		m_texture;
	LPDIRECT3DVERTEXBUFFER9 m_font_draw_buffer;

	cols_e				m_std_col;
	bool				m_use_std_col;
	
	float				m_scale;
	int					m_chr_size;
	unsigned int		m_col;

};

/*********** GLOBAL VARIABLES ***********/

extern Font				*g_mini_font_p;
extern unsigned int		g_std_cols[];

/********* FUNCTION PROTOTYPES **********/



#endif