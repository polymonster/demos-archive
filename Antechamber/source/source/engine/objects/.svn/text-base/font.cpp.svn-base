/****************************************
/	Al (c) 2008 - font.cpp
/****************************************
/	Hardware font class
/
/	ansi!
/	!"#$%&'()*+,-./0
/	123456789:;<=>?@
/	ABCDEFGHIJKLMNOP
/	QRSTUVWXYZ[\]^_`
/	abcdefghijklmnop
/	qrstuvwxyz{|}~€
/	‚ƒ„…†‡ˆ‰Š‹Œ
/	‘’“”•–—˜™š›œŸ 
/	¡¢£¤¥¦§¨©ª«¬­®¯°
/	±²³´µ¶·¸¹º»¼½¾¿À
/	ÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞ
/	ÑÒÓÔÕÖ×ØÙÚÛÜİŞßà
/	áâãäåæçèéêëìíîïğ
/	ñòóôõö÷øùúûüışÿ
/
/****************************************/


/*************** INCLUDES ***************/

#include "d3dapp.h"
#include "d3dtex.h"
#include "font.h"

#include "paths.h"


/*************** CONSTANTS **************/

#define				DEBUG_GEN_FONT_INFO


/*********** GLOBAL VARIABLES ***********/

unsigned int g_std_cols[] =
{
	// NA
	COL_WHITE,

	COL_WHITE,
	COL_RED,
	COL_BLUE,
	COL_GREEN,
	COL_BLACK,
	COL_YELLOW,
	COL_GREY
};


char g_col_strs[][16] =
{
	{ "[white]"		},
	{ "[red]"		},
	{ "[blue]"		},
	{ "[green]"		},
	{ "[black]"		},
	{ "[yellow]"	},
	{ "[grey]"		}
};

/********* FUNCTION PROTOTYPES **********/

Font::Font()
{
}

Font::Font( char *font_p, int chr_size, float scale, unsigned int col ) :
	m_col( col ),
	m_scale( scale ),
	m_chr_size( chr_size ),
	m_use_std_col( false ),
	m_std_col( S_COL_WHITE ),
	m_font_draw_buffer( NULL )
{

	set_priority( RENDER_FRONT + 1.0f );
	char fullpath[64];
	sprintf( fullpath, "%s%s.tga", PATH_FONTS, font_p );
		m_texture = load_d3d_texture( fullpath );

	for( int i = 0; i < MAX_CHRS; i++ )
	{
		m_chr[i].u0 = 0;
		m_chr[i].u1 = chr_size;
	}
	
	// get character spacing data
	char data_filename[64];
	strcpy( data_filename, fullpath );
	int loc = (int)( strlen(fullpath)-3 );
	data_filename[loc] = 0;
	strcat( data_filename, "dat" );

	FILE *data_handle_p = NULL;
	data_handle_p = fopen( data_filename, "rb" );
	if( data_handle_p )
	{
		for( int i = 0; i < MAX_CHRS; i++ )
		{
			fread( &m_chr[i].u0, 1, 1, data_handle_p );
			fread( &m_chr[i].u1, 1, 1, data_handle_p );
			if( m_chr[i].u0 == 8 )
				m_chr[i].u0 = 0;
			if( m_chr[i].u1 == 0 )
				m_chr[i].u1 = 8;
		}
		fclose( data_handle_p );
	}

	g_d3d_device->SetVertexShader( NULL );


}

Font::~Font()
{
	if( m_font_draw_buffer )
		m_font_draw_buffer->Release();
	m_texture.texture->Release();
}

/**************************************************************
/	Font::gen_font_info - debug only, spits out character
/	spacing data for a given font texture.
/**************************************************************/
void Font::gen_font_info( char *font_filename )
{
	// load the tga file
	FILE *file_handle_p = fopen( font_filename , "rb" );

	tga_header_s tga_header;
	memset( &tga_header, NULL, sizeof( tga_header_s ) );

	// header data
	size_t bs = sizeof( byte );
	fread( &tga_header.header_buff, bs, bs, file_handle_p );
	fread( &tga_header.colour_map, bs, bs, file_handle_p );
	fread( &tga_header.imagetype, bs, bs, file_handle_p );
	fread( &tga_header.colourmap_start, bs*2, bs, file_handle_p );
	fread( &tga_header.bitdepth, bs*2, bs, file_handle_p );
	fread( &tga_header.colourmap_bits, bs, bs, file_handle_p );
	fread( &tga_header.xstart, bs*2, bs, file_handle_p );
	fread( &tga_header.ystart, bs*2, bs, file_handle_p );
	fread( &tga_header.width, bs*2, bs, file_handle_p );
	fread( &tga_header.height, bs*2, bs, file_handle_p );
	fread( &tga_header.bpp, bs, bs, file_handle_p );
	fread( &tga_header.description, bs, bs, file_handle_p );

	// pixel data
	size_t image_size = sizeof( unsigned int ) * ( tga_header.width * tga_header.height );
	unsigned int *tga_mem = (unsigned int *)malloc( image_size );
	memset( (void *)tga_mem, NULL, image_size );
	fread( tga_mem, image_size, sizeof( byte ), file_handle_p );

	// discard tga file
	fclose( file_handle_p );

	// set extremes for data
	for( int i = 0; i < MAX_CHRS; i++ )
	{
		m_chr[i].u0 = m_chr_size;
		m_chr[i].u1 = 0;
	}

	int a, x, l;
	a = l = 0;
	x = tga_header.width-16;
	for( int chr = 0; chr < 256-32; chr++ )
	{
		if( l >= 16 )
		{
			x -= m_chr_size;
			l = 0;
		}
		// file is upside down, so we start with the bottom
		int addr = (tga_header.height*x)+(l*m_chr_size);

		// for each row of the cell
		for( int i = 0; i < m_chr_size; i++ )
		{
			unsigned int val = *(tga_mem+addr+a);
			// scan across to find visible pixels
			while( a < m_chr_size )
			{
				// not empty!
				if( val != 0x00ffffff && val != 0xffffffff )
				{
					if( a == 7 )
					{
						// max u1
						m_chr[chr].u1 = 8;
						break;
					}
					// found u0 for character, set if lower than current
					if( a < m_chr[chr].u0 )
						m_chr[chr].u0 = a;
				}
				else
				{
					unsigned int lastval = *(tga_mem+addr+a-1);
					if( lastval != 0x00ffffff && lastval != 0xffffffff )
					{
						// found u1 for character, set if higher than current
						if( a >= m_chr[chr].u1 )
							m_chr[chr].u1 = a+1;
					}
				}
				a++;
				val = *(tga_mem+addr+a);
			}

			a = 0;
			x--;
			addr = (tga_header.height*x)+(l*m_chr_size);
		}

		x += m_chr_size;
		l++;

	}

	// write out the data!

	char data_filename[64];
	strcpy( data_filename, font_filename );
	int loc = (int)( strlen(data_filename)-3 );
	data_filename[loc] = 0;
	strcat( data_filename, "dat" );
	FILE *newdata_handle_p = fopen( data_filename, "w" );

	fwrite( (void *)m_chr, 2*MAX_CHRS-32, 1, newdata_handle_p );

	fclose( newdata_handle_p );


	//// reset to standard spacing
	//for( int i = 0; i < MAX_CHRS; i++ )
	//{
	//	m_chr[i].u0 = 0;
	//	m_chr[i].u1 = m_chr_size;
	//}

	free( tga_mem );
}



void Font::print_2d_text( char *string_p, int str_x, int str_y, float scale, unsigned int col )
{
	if( !*string_p )
		return;

	int len = (int)strlen( string_p );

	char *found_p = NULL;
	do
	{
		for( int i4 = 0; i4 < S_NUM_COLS-1; i4++ )
		{
			found_p = strstr( string_p, (char *)g_col_strs[i4] );
			if( found_p )
			{
				int cslen = strlen( (char *)g_col_strs[i4] );
				found_p[0] = i4+1;
				found_p[1] = 0;
				found_p += ( cslen );
				strcat( string_p, found_p );

				found_p = strstr( string_p, "[]" );
				if( found_p )
				{
					found_p[0] = 1;
					found_p[1] = 0;
					found_p += 2;
					strcat( string_p, found_p );
				}
			}
		}
	}while( found_p );

	m_scale = scale;
	m_col = col;

	// number of verts for printing the string
	int num_verts = ( (int)len * 4 );

	// create a vertex buffer interface
	g_d3d_device->CreateVertexBuffer( num_verts * sizeof( chr_vert_s ), 0, CUSTOM_STRIP_FVF, D3DPOOL_MANAGED, &m_font_draw_buffer, NULL );

	g_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_d3d_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_d3d_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	chr_vert_s *vert_p;

    // lock buffer and load the vertices into it
	m_font_draw_buffer->Lock( 0, 0, (void**)&vert_p, 0 );

	float chr_v = 1.0f / 16.0f;
	float texel_w = chr_v * ( 1.0f / (float)m_chr_size );
	float img_w = m_chr_size * 16.0f;
	float chr_u = chr_v;
	float x, y, u, v;
	x = (float)str_x;
	y = (float)str_y;

	//	u0,v0 --- u1,v0
	//	  |			|
	//	  |			|
	//	u0,v1 --- u1,v1

	//// goodbye spaces
	//int sk = 0;
	//while( (unsigned char)string_p[sk] < 32 )
	//	string_p++;

	// check for special colour chrs
	int k = 0;
	unsigned char chr = (unsigned char)( string_p[k] );
	m_use_std_col =  chr < ' ' ? !m_use_std_col : m_use_std_col;
	if( chr < ' ' )
	{
		m_std_col = (cols_e)chr;
		k++;
		chr = (unsigned char)( string_p[k] );
	}

	// caculate uv's
	float pos = ( (float)(chr-1) / 16.0f );
	int n = (int)pos;
	u = ( pos - (float)n ) + ( m_chr[(unsigned char)( string_p[k] )-33].u0 * texel_w );
	v = (float)n / 16.0f;

	// find character width
	int chr_w = (int)( m_chr[(unsigned char)( string_p[k] )-33].u1 - m_chr[(unsigned char)( string_p[k] )-33].u0 );
	chr_u = chr_w * texel_w;

	// first 2 verts of the strip
	set_vert( vert_p, x, y, u, v, 0 );
	vert_p++;
	set_vert( vert_p, x, y + m_chr_size, u, v + chr_v, 0 );
	vert_p++;

	int i, j;
	i = 0;
	j = k;

	// ======================= fill the strip! =======================

	while( i < num_verts )
	{

		// last 2 verts
		set_vert( vert_p, x + chr_w, y, u + chr_u, v, i );
		i++;
		set_vert( vert_p, x + chr_w, y + m_chr_size, u + chr_u, v + chr_v, i );
		i++;

		// ----------- update some variables ------------

		// move forward by length of last chr
		x += chr_w;
		// increment chr
		j++;
		// stop at the end!
		if( j >= (int)strlen( string_p ) )
			break;
		chr = (unsigned char)( string_p[j] );

		// set colour mode
		while( chr < ' ' )
		{
			m_use_std_col = !m_use_std_col;
			m_std_col = (cols_e)chr;
			j++;
			if( j >= (int)strlen( string_p ) )
				break;
			chr = (unsigned char)( string_p[j] );
		}

		// ---------- recalculate chr u0 & v0 -----------

		pos = ( (float)(chr-1) / 16.0f );
		n = (int)pos;
		u = ( pos - (float)n ) + ( m_chr[(unsigned char)( string_p[j] )-33].u0 * texel_w );
		v = (float)n / 16.0f;

		// ---------- recalculate chr w and u1 ----------
		if( string_p[j] <= ' ' )
			chr_w = 3;
		else
			chr_w = (int)( m_chr[(unsigned char)( string_p[j] )-33].u1 - m_chr[(unsigned char)( string_p[j] )-33].u0 );
		chr_u = chr_w * texel_w;

		// ----------------------------------------------

		// first 2 verts
		set_vert( vert_p, x, y, u, v, i );
		i++;
		set_vert( vert_p, x, y + m_chr_size, u, v + chr_v, i );
		i++;

	}

	m_use_std_col = false;

	// ===============================================================


	m_font_draw_buffer->Unlock();


	g_d3d_device->SetTexture( 0, m_texture.texture );

	g_d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	g_d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

	g_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

	g_d3d_device->SetStreamSource( 0, m_font_draw_buffer, 0, sizeof( chr_vert_s ) );
	g_d3d_device->SetFVF( CUSTOM_STRIP_FVF );

	// copy the vertex buffer to the back buffer
	g_d3d_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, num_verts-2 );

	if( m_font_draw_buffer )
	{
		m_font_draw_buffer->Release();
		m_font_draw_buffer = NULL;

	}

}


void Font::set_vert( chr_vert_s *fvf_struct_p, float x, float y, float u, float v, int iterator )
{
	fvf_struct_p[iterator].x		= x;
	fvf_struct_p[iterator].y		= y;
	fvf_struct_p[iterator].z		= 0.0f;
	fvf_struct_p[iterator].rhw		= 1.0f;
	fvf_struct_p[iterator].col		= m_use_std_col ? g_std_cols[m_std_col] : m_col;
	fvf_struct_p[iterator].u		= u;
	fvf_struct_p[iterator].v		= v;
}


int Font::get_chr_width( unsigned char chr )
{
	return( (int)( m_chr[chr].u1 - m_chr[chr].u0 ) );
}

int Font::get_text_width( char *string_p )
{
	int len = strlen( string_p );
	unsigned char chr;
	int width = 0;
	for( int i  = 0; i < len; i++ )
	{
		chr = (unsigned char)string_p[i];
		if( chr <= ' ' )
			width += 3;
		else
			width += (int)( m_chr[chr-33].u1 - m_chr[chr-33].u0 );
	}
	return( width );
}
