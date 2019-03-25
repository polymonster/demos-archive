/****************************************
/	Al (c) 2008 - quad.h
/****************************************
/	Draw 2D quad
/****************************************/

#ifndef _QUAD_H_
#define _QUAD_H_

/*************** INCLUDES ***************/

#include <windows.h>
#include <windowsx.h>

#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )

#include "link.h"
#include "d3dtex.h"
#include "paths.h"

/*************** CONSTANTS **************/



/**************** CLASSES ***************/

typedef struct
{
	float x, y, z;
	unsigned int col;
	float u, v;

}custom_vert_s;

typedef struct vector2_t
{
	float x, y;
	const vector2_t operator = ( const vector2_t &v ) { x=v.x, y=v.y; return( *this ); }
	const bool operator == ( const vector2_t &v ){ return( x==v.x && y==v.y ); }
	const bool operator != ( const vector2_t &v ){ return( x!=v.x || y!=v.y ); }
}vector2;

typedef struct texcoords4_t
{
	float u0, u1, v0, v1;
	void set( float _u0, float _u1, float _v0, float _v1 ) { u0 = _u0; u1 = _u1; v0 = _v0; v1 = _v1; }
	const texcoords4_t operator = ( const texcoords4_t &tc ) { u0=tc.u0, u1=tc.u1, v0=tc.v0, v1=tc.v1; return( *this ); }
	const bool operator == ( const texcoords4_t &tc ) { return( u0==tc.u0 && u1==tc.u1 && v0==tc.v0 && v1==tc.v1 ); }
	const bool operator != ( const texcoords4_t &tc ) { return( u0!=tc.u0 || u1!=tc.u1 || v0!=tc.v0 || v1!=tc.v1 ); }
}texcoords4;

typedef struct
{
	unsigned char r, g, b, a;
	void set( unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a ) { r = _r; g = _g; b = _b; a = _a; };

}col_info_s;

class Quad: public Link
{

public:

	Quad( char *path_p, char *name_p, float x, float y, float w, float h, unsigned int col = 0xffffffff, bool default_alphablend = true );
	~Quad();

	virtual void		execute				( void );
	virtual void		render				( void );

	void				set_texcoords		( float u0, float u1, float v0, float v1 ) { m_texcoords.u0 = u0; m_texcoords.u1 = u1, m_texcoords.v0 = v0; m_texcoords.v1 = v1; }
	texcoords4			get_texcoords		( void )						{ return m_texcoords; };

	vector2				get_pos				( void )						{ return m_pos; };
	void				set_pos				( float x, float y )			{ m_pos.x = x; m_pos.y = y; };

	vector2				get_size			( void )						{ return m_size; };
	void				set_size			( float x, float y )			{ m_size.x = x; m_size.y = y; };

	void				set_rotation		( float angle )					{ m_rotation = angle; };
	float				get_rotation		( void )						{ return( m_rotation ); };

	void				center_origin		( void );
	void				translate			( int x, int y );
	void				rotate				( float angle, float origin = 0.0f );

	void				set_texture			( tga_texture_s texture )		{ m_tex = texture; };
	tga_texture_s		get_texture			( void )						{ return( m_tex ); };

	void				set_alpha			( int a );
	int					get_alpha			( void )						{ return( m_col >> 24 ); };
	unsigned int		get_col				( void )						{ return( m_col ); };
	void				set_col				( unsigned int col )			{ m_col = col; };

	bool				m_hide;

protected:

	void				draw_textured_quad();

	LPDIRECT3DVERTEXBUFFER9	m_quad_buffer;
	D3DXMATRIX			m_matrix;

	tga_texture_s		m_tex;

	vector2				m_pos;
	vector2				m_size;
	vector2				m_last_size;
	texcoords4			m_texcoords;
	texcoords4			m_last_texcoords;
	unsigned int		m_col;
	unsigned int		m_last_col;
	float				m_rotation;

	bool				m_transform;
	bool				m_default_ablend;

	char				*m_path_p;

private:

};


/*********** GLOBAL VARIABLES ***********/



/********* FUNCTION PROTOTYPES **********/


#endif