/****************************************
/	Al (c) 2008 - EditNode.cpp
/****************************************
/	Edit Node
/****************************************/


/*************** INCLUDES ***************/

#include "editnode.h"
#include "level.h"

/*************** CONSTANTS **************/


/*********** GLOBAL VARIABLES ***********/

vector2 g_nodedims[] = 
{
	{ 8, 8 },
	{ 12, 12 },
	{ 10, 64 },
	{ 12, 12 },
	{ 16, 16 },
	{ 8, 8 },
	{ 8, 8 }
};

/********* FUNCTION DEFINITIONS *********/


EditNode::EditNode( node_type_e type, node_data_s data ) :
	Quad( PATH_GUI, "editcurs", data.pos.x, data.pos.y, g_nodedims[type].x, g_nodedims[type].y ),
	m_type( type ),
	m_data( data )
{
	set_priority( RENDER_NEAR_FRONT );

	int size = 16;
	m_pos.x = ( m_data.pos.x * size ) + ( m_size.x * 0.5 );
	m_pos.y = ( m_data.pos.y * size ) + ( m_size.x * 0.5 );

	switch( m_type )
	{
	case NODE_ENERGY_CELL:
		set_texcoords( 0.0f, 0.25f, 0.0f, 0.25f );
		break;
	case NODE_INTERCOM:
		m_pos.x -= 4;
		m_pos.y -= 4;
		set_texcoords( 0.25f, 0.5f, 0.25f, 0.5f );
		break;
	case NODE_GATE:
		m_size.x = m_data.gate_type ? 16*4 : 10;
		m_size.y = !m_data.gate_type ? 16*4 : 10;
		set_texcoords( 0.5f, 0.75f, 0.0f, 0.25f );
		break;
	case NODE_CP:
		m_pos.x -= 4;
		m_pos.y -= 4;
		set_texcoords( 0.5f, 0.75f, 0.0f, 0.25f );
		break;
	case NODE_TURRET:
		set_texcoords( 0.25f, 0.5f, 0.0f, 0.25f );
		break;
	case NODE_ELECTROBALL:
		set_texcoords( 0.0f, 0.25f, 0.25f, 0.5f );
		break;
	case NODE_SPAWN:
		set_texcoords( 0.75f, 1.0f, 0.25f, 0.5f );
	}

}


EditNode::~EditNode()
{
}


void EditNode::execute( void )
{

	Quad::execute();
}

void EditNode::render( void )
{

	if( m_type == NODE_TURRET )
		rotate( (float)(m_data.turret_direction * 90) );
	Quad::render();

	char text[32];
	switch( m_type )
	{
	case NODE_INTERCOM:
		sprintf( text, "[green]%d", m_data.intercom_id );
		g_mini_font_p->print_2d_text( text, (m_data.pos.x*16) + 8, (m_data.pos.y*16)-8 );
		break;
	case NODE_CP:
		sprintf( text, "[green]%d", m_data.gate_cp_handle );
		g_mini_font_p->print_2d_text( text, (m_data.pos.x*16) + 8, (m_data.pos.y*16)-8 );
		break;
	case NODE_GATE:
		sprintf( text, "[green]%d", m_data.gate_cp_handle );
		g_mini_font_p->print_2d_text( text, (m_data.pos.x*16) + 8, (m_data.pos.y*16)-8 );
		break;
	case NODE_TURRET:
		sprintf( text, m_data.turret_type == TURRET_TYPE_CANNON ? "C" : m_data.turret_type == TURRET_TYPE_LAZER ? "L" : "R" );
		g_mini_font_p->print_2d_text( text, (m_data.pos.x*16) + 8, (m_data.pos.y*16) );
		break;
	}
}