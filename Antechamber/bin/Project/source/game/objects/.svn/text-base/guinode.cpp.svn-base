/****************************************
/	Al (c) 2008 - guinode.cpp
/****************************************
/	GUI element
/****************************************/


/*************** INCLUDES ***************/

#include "guinode.h"

#include "2dscene.h"


/*************** CONSTANTS **************/


/*********** GLOBAL VARIABLES ***********/


/********* FUNCTION PROTOTYPES **********/


GUINode::GUINode( char *text_p, char *texture_name_p, float init_x, float init_y, float w, float h, int *param_p, int value, unsigned int text_col, unsigned int col ) :
	Quad( PATH_GUI, texture_name_p, init_x, init_y, w, h, col ),
	m_spin_state( SPIN_STATE_INTERACTIVE ),
	m_state( BUTTON_STATE_IDLE ),
	m_type( BUTTON_TYPE_MENUGUI ),
	m_active( true ),
	m_value( value ),
	m_find_rate( 0.0f ),
	m_rotspeed( 0.05f ),
	m_maxrot( 25.0f ),
	m_timer( 0.0f ),
	m_text_col( text_col )
{
	set_priority( RENDER_FRONT );
	m_num_options = 0;
	m_option_handles_p = NULL;

	m_col_info.set( text_col >> 16, text_col >> 8, text_col, 0 );

	m_parameter_p = param_p;
	sprintf( m_text, text_p );

	m_stdbutton_handles.l = -1;
	m_stdbutton_handles.r = -1;

	m_text_x =
	m_find_co.x = init_x;

	m_text_y =
	m_find_co.y = init_y;

	m_over = false;
}

GUINode::GUINode( char *text_p, float x, float y, int *param_p, int value, float w ) :
	Quad( PATH_GUI, "stdbuttons", x, y, 16, 16, 0xffffffff ),
	m_state( BUTTON_STATE_IDLE ),
	m_type( BUTTON_TYPE_STANDARD ),
	m_active( true ),
	m_value( value ),
	m_find_rate( 0.0f ),
	m_rotspeed( 0.0f ),
	m_maxrot( 0.0f ),
	m_timer( 0.0f ),
	m_text_col( 0xffffffff ),
	m_selected( false )
{
	set_priority( RENDER_FRONT );

	m_col_info.set( 0, 0, 0, 255 );
	sprintf( m_text, text_p );

	m_option_handles_p = NULL;
	m_num_options = 0;
	m_size.x = w;
	m_size.y = 16;

	Quad *left_edge_p = new Quad( PATH_GUI, "stdbuttons", x-(m_size.x*0.5f), y-(m_size.y*0.5f), 2, 16 );
	left_edge_p->set_texcoords( 0.0f, (2.0f/m_tex.w), 0.0f, 0.25f );
	left_edge_p->set_priority( get_priority() + 0.1f );

	Quad *right_edge_p = new Quad( PATH_GUI, "stdbuttons", x+(m_size.x*0.5f), y-(m_size.y*0.5f), 2, 16 );
	right_edge_p->set_texcoords( 0.25f-(2.0f/m_tex.w), 0.25f, 0.0f, 0.25f );
	right_edge_p->set_priority( get_priority() + 0.1f );

	m_texcoords.set( (2.0f/m_tex.w), 0.25f-(2.0f/m_tex.w), 0.0f, 0.25f );

	m_stdbutton_handles.l = left_edge_p->get_handle();
	m_stdbutton_handles.r = right_edge_p->get_handle();

	m_parameter_p = param_p;

	m_text_x =
	m_text_y =
	m_find_co.x =
	m_find_co.y = 0;

	m_over = false;
}

GUINode::GUINode( char *text_p, float x, float y, int *param_p, int value, int num_options, char *list_text_p ) :
	Quad( PATH_GUI, "stdbuttons", x, y, 16, 16, 0xffffffff ),
	m_state( BUTTON_STATE_IDLE ),
	m_type( BUTTON_TYPE_DROPLIST ),
	m_active( true ),
	m_value( value ),
	m_find_rate( 0.0f ),
	m_rotspeed( 0.0f ),
	m_maxrot( 0.0f ),
	m_timer( 0.0f ),
	m_text_col( 0xffffffff ),
	m_selection( -1 ),
	m_selected( false )
{
	set_priority( RENDER_FRONT );

	m_col_info.set( 0, 0, 0, 255 );
	sprintf( m_text, text_p );

	m_size.x = 96;
	m_size.y = 16;

	Quad *left_edge_p = new Quad( PATH_GUI, "stdbuttons", x-(m_size.x*0.5f), y-(m_size.y*0.5f), 2, 16 );
	left_edge_p->set_texcoords( 0.0f, (2.0f/m_tex.w), 0.0f, 0.25f );
	left_edge_p->set_priority( get_priority() + 0.1f );

	Quad *right_edge_p = new Quad( PATH_GUI, "stdbuttons", x+(m_size.x*0.5f)-14, y-(m_size.y*0.5f), 16, 16 );
	right_edge_p->set_texcoords( 0.25f, 0.5f, 0.5f, 0.75f );
	right_edge_p->set_priority( get_priority() + 0.1f );

	m_texcoords.set( (2.0f/m_tex.w), 0.25f-(2.0f/m_tex.w), 0.0f, 0.25f );

	m_stdbutton_handles.l = left_edge_p->get_handle();
	m_stdbutton_handles.r = right_edge_p->get_handle();

	m_num_options = num_options;
	if( list_text_p )
	{
		m_option_handles_p = new int[num_options];
		for( int i = 0; i < num_options; i++ )
		{
			GUINode *item_p = new GUINode( list_text_p, x+16, y+(16*(i+1)), &m_selection, i );
			item_p->set_priority( get_priority() + 0.2f );
			m_option_handles_p[i] = item_p->get_handle();
			texcoords4 c = item_p->get_texcoords();
			item_p->set_texcoords( c.u0+0.25f, c.u1+0.25f, c.v0, c.v1 );
			list_text_p+=32;
		}
	}

	m_parameter_p = param_p;

	sprintf( m_text, text_p );
	m_text_x =
	m_text_y =
	m_find_co.x =
	m_find_co.y = 0;

	m_over = false;
}

GUINode::GUINode( char *text_p, float x, float y, float w, bool *param_p ) :
	Quad( PATH_GUI, "stdbuttons", x, y, 16, 16, 0xffffffff ),
	m_state( BUTTON_STATE_IDLE ),
	m_spin_state( SPIN_STATE_NONE ),
	m_type( BUTTON_TYPE_TOGGLE ),
	m_active( true ),
	m_find_rate( 0.0f ),
	m_rotspeed( 0.0f ),
	m_maxrot( 0.0f ),
	m_timer( 0.0f ),
	m_text_col( 0xffffffff ),
	m_toggled( false )
{
	set_priority( RENDER_FRONT );
	m_num_options = 0;
	m_option_handles_p = NULL;

	m_col_info.set( 0, 0, 0, 255 );
	sprintf( m_text, text_p );

	m_size.x = w;
	m_size.y = 16;

	Quad *left_edge_p = new Quad( PATH_GUI, "stdbuttons", x-(m_size.x*0.5f), y-(m_size.y*0.5f), 2, 16 );
	left_edge_p->set_texcoords( 0.0f, (2.0f/m_tex.w), 0.0f, 0.25f );
	left_edge_p->set_priority( get_priority() + 0.1f );

	Quad *right_edge_p = new Quad( PATH_GUI, "stdbuttons", x+(m_size.x*0.5f), y-(m_size.y*0.5f), 2, 16 );
	right_edge_p->set_texcoords( 0.25f-(2.0f/m_tex.w), 0.25f, 0.0f, 0.25f );
	right_edge_p->set_priority( get_priority() + 0.1f );

	m_texcoords.set( (2.0f/m_tex.w), 0.25f-(2.0f/m_tex.w), 0.0f, 0.25f );

	m_stdbutton_handles.l = left_edge_p->get_handle();
	m_stdbutton_handles.r = right_edge_p->get_handle();

	m_toggle_p = param_p;
	m_selected = false;

	m_text_x =
	m_text_y =
	m_find_co.x =
	m_find_co.y = 0;

	m_over = false;
}


GUINode::~GUINode()
{
	g_object_list_p->remove( m_stdbutton_handles.l );
	g_object_list_p->remove( m_stdbutton_handles.r );

	if( m_option_handles_p )
	{
		for( int i = 0; i < m_num_options; i++ )
			g_object_list_p->remove( m_option_handles_p[i] );
		delete [] m_option_handles_p;
	}

}


void GUINode::execute( void )
{
	if( !m_hide )
	{
		switch( m_state )
		{
		case BUTTON_STATE_IDLE:
			break;

		case BUTTON_STATE_MOVE:
			{
				// tween position
				vector2 dist;
				dist.x = fabsf( m_pos.x - m_find_co.x );
				dist.y = fabsf( m_pos.y - m_find_co.y );

				int found = 0;

				if( dist.x > 2.0f )
					m_pos.x += ( ( m_find_co.x - m_pos.x ) * ( m_find_rate * get_game_time( dist.x ) ) );
				else
				{
					m_pos.x = m_find_co.x;
					found++;
				}

				if( dist.y > 2.0f )
					m_pos.y += ( ( m_find_co.y - m_pos.y ) * ( m_find_rate * get_game_time( dist.y ) ) );
				else
				{
					m_pos.y = m_find_co.y;
					found++;
				}

				if( found >= 2 )
					m_state = BUTTON_STATE_IDLE;

			}
			break;

		}

		int mx, my;
		get_mouse_pos( mx, my );

		switch( m_type )
		{
			case BUTTON_TYPE_TOGGLE:
			{
				if( mouse_button_down( 0 ) && collided( mx, my ) )
				{
					if( m_parameter_p && mouse_button_click( 0 ) )
					{
						*m_toggle_p = !(*m_toggle_p);
					}
				}
				if( *m_toggle_p )
				{
					m_texcoords.v0 = 0.25f;
					m_texcoords.v1 = 0.5f;
				}
				else
				{
					m_texcoords.v0 = 0.0f;
					m_texcoords.v1 = 0.25f;
				}
			}
			break;

			case BUTTON_TYPE_STANDARD:
			{
				if( mouse_button_down( 0 ) && collided( mx, my ) )
				{
					m_texcoords.v0 = 0.25f;
					m_texcoords.v1 = 0.5f;
					if( m_parameter_p && mouse_button_click( 0 ) )
						*m_parameter_p = m_value;
				}
				else
				{
					m_texcoords.v0 = 0.0f;
					m_texcoords.v1 = 0.25f;
				}
			}
			break;

			case BUTTON_TYPE_DROPLIST:
			{
				if( m_selected )
				{
					for( int i = 0; i < m_num_options; i++ )
					{
						GUINode *item_p = (GUINode *)g_object_list_p->get_obj( m_option_handles_p[i] );
						item_p->m_hide = false;
						Quad *border_p = (Quad *)g_object_list_p->get_obj( item_p->m_stdbutton_handles.l );
						border_p->m_hide = false;
						border_p = (Quad *)g_object_list_p->get_obj( item_p->m_stdbutton_handles.r );
						border_p->m_hide = false;
					}
					Quad *dropdown_p = (Quad *)g_object_list_p->get_obj( m_stdbutton_handles.r );
					dropdown_p->set_texcoords( 0.25f, 0.5f, 0.75f, 1.0f );
				}
				else
				{
					for( int i = 0; i < m_num_options; i++ )
					{
						GUINode *item_p = (GUINode *)g_object_list_p->get_obj( m_option_handles_p[i] );
						item_p->m_hide = true;
						Quad *border_p = (Quad *)g_object_list_p->get_obj( item_p->m_stdbutton_handles.l );
						border_p->m_hide = true;
						border_p = (Quad *)g_object_list_p->get_obj( item_p->m_stdbutton_handles.r );
						border_p->m_hide = true;
					}
					Quad *dropdown_p = (Quad *)g_object_list_p->get_obj( m_stdbutton_handles.r );
					dropdown_p->set_texcoords( 0.25f, 0.5f, 0.5f, 0.75f );
				}

				if( collided( mx, my ) && mouse_button_click( 0 ) )
				{
					m_selected = !m_selected;
					if( m_selected ) m_selection = -1;
				}

				if( m_selected && m_selection != -1 )
				{
					m_selected = false;
					*m_parameter_p = m_value;
				}
			}
			break;

			case BUTTON_TYPE_MENUGUI:
			{
				if( m_rotspeed > 0.6f )
					m_col_info.a = 255 * ( m_rotspeed / 25.0f );
				else
					m_col_info.a = 0;

				m_text_col = ( m_col_info.r << 16 ) | ( m_col_info.g << 8 ) | m_col_info.b | ( m_col_info.a << 24 );

				float minspeed = 0.1f;
				switch( m_spin_state )
				{
					case SPIN_STATE_NONE:
						m_rotspeed = 0.0f;
						break;

					case SPIN_STATE_SLOW:
						m_rotspeed = minspeed;
						break;

					case SPIN_STATE_INTERACTIVE:
					{
						// minimum rotation speed is between m_maxrot and 0.5f
						minspeed = m_maxrot > 0.5f ? 0.5f : m_maxrot;

						if( collided( mx, my ) )
						{
							if(!m_over) 
							{
								m_over = true;
								g_sound_mgr_p->play(g_sound_id.activate);
							}

							// acelerate rotation to maxrot
							if( m_rotspeed < m_maxrot )
								m_rotspeed += g_scene_p->m_game_time * 25.0f;
							else
								m_rotspeed = m_maxrot;

							// set the button's command parameter to the preset value
							// in other words, what the button is for!
							if( m_parameter_p && mouse_button_down( 0 ) )
							{
								*m_parameter_p = m_value;
							}

						}
						else
						{
							m_over = false;
							// decelerate rotation to minspeed
							if( m_rotspeed > minspeed )
								m_rotspeed -= g_scene_p->m_game_time * 6.0f;
							else
								m_rotspeed = minspeed;
						}
					}
					break;
				}
			}
			break;

			
		}

		//if( m_spin_state != SPIN_STATE_NONE )
		{
			// if the quad is rotating, increase timer
			m_rotspeed ? m_timer += g_scene_p->m_game_time * m_rotspeed : m_timer = 0.0f;

			if( m_timer >= 2.0f )
				m_timer = 0.0f;

			// rotate matrix
			rotate( 180.0f * m_timer );
		}

		Quad::execute();
	}
}


void GUINode::render( void )
{

	if( !m_hide )
	{
		switch( m_type )
		{
			case BUTTON_TYPE_STANDARD:
			case BUTTON_TYPE_TOGGLE:
			{
				Quad::draw_textured_quad();
				g_mini_font_p->print_2d_text( m_text, (m_pos.x+8-(m_size.x*0.5f)), (m_pos.y-4), 1.0f, m_text_col );
			}
			break;

			case BUTTON_TYPE_DROPLIST:
			{
				Quad::draw_textured_quad();
				g_mini_font_p->print_2d_text( m_text, (m_pos.x+8-(m_size.x*0.5f)), (m_pos.y-4), 1.0f, m_text_col );
			}
			break;

			case BUTTON_TYPE_MENUGUI:
			{
				Quad::draw_textured_quad();
				g_mini_font_p->print_2d_text( m_text, (m_pos.x+18), (m_pos.y-4), 1.0f, m_text_col );
			}
			break;

		}
	}

}

bool GUINode::collided( int x, int y )
{
	// button bb
	int w = (int)( m_size.x *0.5f );
	int h = (int)( m_size.y *0.5f );
	if( m_active && x >= m_pos.x - w && x <= m_pos.x + w && y >= m_pos.y - h && y <= m_pos.y + h )
		return( true );

	return( false );
}

void GUINode::activate( bool on )
{
	m_hide = !on;
	m_active = on;
	Quad *edge_p = (Quad*)g_object_list_p->get_obj( m_stdbutton_handles.l );
	edge_p->m_hide = !on;
	edge_p = (Quad*)g_object_list_p->get_obj( m_stdbutton_handles.r );
	edge_p->m_hide = !on;
}

void GUINode::set_new_pos( float x, float y, float rate, bool text )
{
	// set tween to new position params
	m_find_rate = rate;
	m_find_co.x = x;
	m_find_co.y = y;

	if( text )
	{
		m_text_x = x;
		m_text_y = y;
	}

	m_state = BUTTON_STATE_MOVE;
}
