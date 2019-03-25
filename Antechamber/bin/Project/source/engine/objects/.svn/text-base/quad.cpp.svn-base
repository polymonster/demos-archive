/****************************************
/	Al (c) 2008 - quad.cpp
/****************************************
/	Draw 2D quad
/****************************************/


/*************** INCLUDES ***************/

#include <stdio.h>
#include "d3dapp.h"
#include "quad.h"
#include "d3dtex.h"
#include "dxio.h"


/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/


/********* FUNCTION DEFINITIONS *********/

/****************************************\
|	Constructor & Destructor
\****************************************/
Quad::Quad( char *path_p, char *name_p, float x, float y, float w, float h, unsigned int col, bool default_alphablend ) :
	m_col( col ),
	m_hide( false ),
	m_transform( false ),
	m_quad_buffer( NULL )
{
	m_pos.x = x;
	m_pos.y = y;
	m_size.x = w;
	m_size.y = h;
	m_last_size.x = w;
	m_last_size.y = h;
	m_rotation = 0.0f;
	m_last_col = m_col;

	m_texcoords.set( 0.0f, 1.0f, 0.0f, 1.0f );
	m_last_texcoords.set( 0.0f, 1.0f, 0.0f, 1.0f );

	m_default_ablend = default_alphablend;

	// reset matrix
	D3DXMatrixIdentity( &m_matrix );

	// load texture
	char fullpath[64];
	sprintf( fullpath, "%s%s.tga", path_p, name_p );
	m_tex = load_d3d_texture( fullpath );
	if( !m_tex.texture )
	{
		char msg[128];
		sprintf( msg, "Quad:: texture '%s' failed to load.\n", fullpath );
		dbgmsg( msg );
	}

	g_d3d_device->SetVertexShader( NULL );

    // create a vertex buffer interface called m_draw_buffer
    g_d3d_device->CreateVertexBuffer( 4 * sizeof( custom_vert_s ), 0, CUSTOM_2DQUAD_FVF, D3DPOOL_MANAGED, &m_quad_buffer, NULL );

	custom_vert_s* vert_p;

	// lock buffer and load the vertices into it
	m_quad_buffer->Lock( 0, 0, (void**)&vert_p, 0 );

		vert_p[0].x			= 0.0f - 0.5f;
		vert_p[0].y			= m_size.y - 0.5f;
		vert_p[0].z			= 0.0f;
		vert_p[0].col		= m_col;
		vert_p[0].u			= m_texcoords.u0;
		vert_p[0].v			= m_texcoords.v1;

		vert_p[1].x			= m_size.x - 0.5f;
		vert_p[1].y			= m_size.y - 0.5f;
		vert_p[1].z			= 0.0f;
		vert_p[1].col		= m_col;
		vert_p[1].u			= m_texcoords.u1;
		vert_p[1].v			= m_texcoords.v1;

		vert_p[2].x			= 0.0f - 0.5f;
		vert_p[2].y			= 0.0f - 0.5f;
		vert_p[2].z			= 0.0f;
		vert_p[2].col		= m_col;
		vert_p[2].u			= m_texcoords.u0;
		vert_p[2].v			= m_texcoords.v0;

		vert_p[3].x			= m_size.x - 0.5f;
		vert_p[3].y			= 0.0f - 0.5f;
		vert_p[3].z			= 0.0f;
		vert_p[3].col		= m_col;
		vert_p[3].u			= m_texcoords.u1;
		vert_p[3].v			= m_texcoords.v0;

	m_quad_buffer->Unlock();

}


Quad::~Quad()
{
	// release buffers
//	if( m_tex.texture ) m_tex.texture->Release();
	if( m_quad_buffer )	m_quad_buffer->Release();
}


/****************************************\
|	execute
\****************************************/
void Quad::execute( void )
{
	// nothing to do here... yet

}

/****************************************\
|	center_origin
|	moves m_matrix to center of quad
\****************************************/
void Quad::center_origin( void )
{
	m_transform = true;

	D3DXMATRIX trans;
	D3DXMatrixTranslation( &trans, -(m_size.x * 0.5f), -(m_size.y * 0.5f), 0.0f );

	m_matrix *= trans;
}

/****************************************\
|	translate
|	moves the quad for this frame only!
|	for permanent move use 'set_pos'
\****************************************/
void Quad::translate( int x, int y )
{
	m_transform = true;
	D3DXMATRIX trans;
	D3DXMatrixTranslation( &trans, (float)x, (float)y, 0.0f );

	m_matrix *= trans;
}

/****************************************\
|	rotate
|	rotates the quad for this frame
\****************************************/
void Quad::rotate( float angle, float origin )
{
	m_transform = true;

	if( !origin )
		center_origin();
	else
		// sort out this crap
		translate( m_size.x * origin, m_size.y * origin );

	D3DXMATRIX rot;
	D3DXMatrixRotationZ( &rot, angle * ( PI / 180.0f ) );

	m_matrix *= rot;
}


/****************************************\
|	render
\****************************************/
void Quad::render( void )
{

	// screen origin adjustment (nightmare)
//	translate( 0.0f, !g_editor_mode ? (int)( ( (float)g_window_h - 256.0f ) * 0.5f ) : 0.0f );

	// draw the quad
	draw_textured_quad();

}

/****************************************\
|	draw_textured_quad
\****************************************/
void Quad::draw_textured_quad( void )
{

	if( !m_hide )
	{
		translate( (int)m_pos.x, (int)m_pos.y );

		// the quad has changed - rewrite to the buffer
		// for the record, not locking every frame saved absolutely no time...
		if( m_last_col != m_col || m_last_texcoords != m_texcoords || m_last_size != m_size )
		{
			custom_vert_s* vert_p;
			m_quad_buffer->Lock( 0, 0, (void**)&vert_p, 0 );

			vert_p[0].x			= 0.0f - 0.5f;
			vert_p[0].y			= m_size.y - 0.5f;
			vert_p[0].z			= 0.0f;
			vert_p[0].col		= m_col;
			vert_p[0].u			= m_texcoords.u0;
			vert_p[0].v			= m_texcoords.v1;

			vert_p[1].x			= m_size.x - 0.5f;
			vert_p[1].y			= m_size.y - 0.5f;
			vert_p[1].z			= 0.0f;
			vert_p[1].col		= m_col;
			vert_p[1].u			= m_texcoords.u1;
			vert_p[1].v			= m_texcoords.v1;

			vert_p[2].x			= 0.0f - 0.5f;
			vert_p[2].y			= 0.0f - 0.5f;
			vert_p[2].z			= 0.0f;
			vert_p[2].col		= m_col;
			vert_p[2].u			= m_texcoords.u0;
			vert_p[2].v			= m_texcoords.v0;

			vert_p[3].x			= m_size.x - 0.5f;
			vert_p[3].y			= 0.0f - 0.5f;
			vert_p[3].z			= 0.0f;
			vert_p[3].col		= m_col;
			vert_p[3].u			= m_texcoords.u1;
			vert_p[3].v			= m_texcoords.v0;

			m_quad_buffer->Unlock();
		}

		m_last_texcoords = m_texcoords;
		m_last_size = m_size;
		m_last_col = m_col;

		g_d3d_device->SetTransform( D3DTS_WORLD, &m_matrix );

		g_d3d_device->SetTexture( 0, m_tex.texture );
		g_d3d_device->SetFVF( CUSTOM_2DQUAD_FVF );

		g_d3d_device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		g_d3d_device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

		g_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );
		g_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		if( m_default_ablend )
		{
			g_d3d_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			g_d3d_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			g_d3d_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			g_d3d_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		}
		g_d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		g_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

		g_d3d_device->SetStreamSource( 0, m_quad_buffer, 0, sizeof( custom_vert_s ) );

		// copy the vertex buffer to the back buffer
		g_d3d_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		D3DXMATRIX reset;
		D3DXMatrixTranslation( &reset, 0.0f, 0.0f, 0.0f );
		g_d3d_device->SetTransform( D3DTS_WORLD, &reset );

		D3DXMatrixIdentity( &m_matrix );
	}
}


void Quad::set_alpha( int a )
{
	col_info_s cols;
	cols.r = m_col >> 16;
	cols.g = m_col >> 8;
	cols.b = m_col;
	m_col = ( cols.r << 16 ) | ( cols.g << 8 ) | cols.b | ( a << 24 );
}