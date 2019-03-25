/****************************************\
|	Al (c) 2008 - link.cpp
|****************************************|
|	doubly linked list
\****************************************/


/*************** INCLUDES ***************/

#include <stdio.h>
#include "link.h"


/*************** CONSTANTS **************/



/*********** GLOBAL VARIABLES ***********/

Linklist *g_object_list_p = new Linklist( "object list", MAX_LINKS );



/********* FUNCTION DEFINITIONS *********/


Link::Link() :
	m_next_p( NULL ),
	m_prev_p( NULL ),
	m_render_priority( 0.0f ),
	m_link_handle( g_object_list_p->add( this ) )
{
}

Link::Link( int &link_id ) :
	m_next_p( NULL ),
	m_prev_p( NULL ),
	m_render_priority( 0.0f ),
	m_link_handle( g_object_list_p->add( this ) )
{
	link_id = m_link_handle;
}

Link::~Link()
{
}

/************* PROTOTYPE *******************************\
| Link::set_priority
| sets priority and replaces link in the list
\*******************************************************/
void Link::set_priority( float priority )
{
	// no change required
	if( m_render_priority == priority )
		return;

	m_render_priority = priority;

	Link *link_p = g_object_list_p->m_first_p;

	// traverse the list
	do
	{
		// if link has a lower priority (ie render first)
		// note: list is rendered from last to first link
		if( link_p->get_priority() <= m_render_priority )
		{
			// already in correct position!
			if( link_p == this )
				return;

			// drop out of previous location
			if( m_prev_p )
			{
				if( m_next_p )
				{
					// somewhere in the middle - easy
					m_prev_p->m_next_p = m_next_p;
					m_next_p->m_prev_p = m_prev_p;
				}
				else
				{
					// no next link, this must be the last one
					m_prev_p->m_next_p = NULL;
					g_object_list_p->m_last_p = m_prev_p;
				}
			}
			else
			{
				// no previous link, this must be the first one
				m_next_p->m_prev_p = NULL;
				g_object_list_p->m_first_p = m_next_p;
			}

			// squeeze in!
			if( link_p->m_prev_p )
			{
				// link we are slotting in before has a previous link, budge in!
				link_p->m_prev_p->m_next_p = this;
				m_prev_p = link_p->m_prev_p;
				link_p->m_prev_p = this;
			}
			else
			{
				// this is the new first link in the list
				link_p->m_prev_p = this;
				m_prev_p = NULL;
				g_object_list_p->m_first_p = this;
			}
			m_next_p = link_p;

			// done!
			return;
		}

		link_p = link_p->m_next_p;
	}
	while( link_p != g_object_list_p->m_last_p );

}



Linklist::Linklist( char *list_name_p, short max_links ) :
	m_handle_pool( 0 ),
	m_first_p( NULL ),
	m_last_p( NULL ),
	m_num_links( max_links ),
	m_num_links_used( 0 )
{
	if( strlen( list_name_p ) >= MAX_NAME_LEN )
	{
		printf( "linklist name too long!!\n" );
		return;
	}

	strcpy( (char *)m_list_name, list_name_p );

}

Linklist::~Linklist()
{
	clear_list();
}


/************* PROTOTYPE *******************************\
| Linklist::add
| adds a link at the front of the list
\*******************************************************/
int Linklist::add( Link *link_p )
{

	// start at the beginning
	Link *temp_p = m_first_p;

	// first link in the list is empty, start the list!
	if( !m_first_p )
	{
		// lonely link
		m_first_p = link_p;
		m_last_p = link_p;
		link_p->m_next_p = NULL;
		link_p->m_prev_p = NULL;
		m_handle_pool++;
		m_num_links_used++;
	}
	else
	{
		for( int i = 0; i < m_num_links; i++ )
		{
			// found empty slot!
			if( !temp_p->m_next_p )
			{
				// doubly link the new object
				temp_p->m_next_p = link_p;
				link_p->m_prev_p = temp_p;

				// update the last link in the list
				m_last_p = link_p;
				link_p->m_next_p = NULL;

				m_num_links_used++;
				return( m_handle_pool++ );
			}
			else
			{
				// cycle through links
				temp_p = temp_p->m_next_p;
			}

		}
		// linklist full error
		dbgmsg( "Max links (%d) for linklist reached!\n", m_num_links );
		return( -1 );
	}

	// first link!
	return( 0 );

}


void Linklist::execute_list( void )
{
	// obj list execute
	if( m_first_p )
	{
		Link *link_p = m_first_p;
		for( int i = 0; i < MAX_LINKS; i++ )
		{
			link_p->execute();
			if( link_p->m_next_p )
				link_p = link_p->m_next_p;
			else
				break;
		}
	}
}

void Linklist::render_list( void )
{
	// lets render this junk

	if( m_last_p )
	{
		float priority = 0.0f;
		Link *link_p = m_last_p;
		for( int i = 0; i < MAX_LINKS; i++ )
		{
			link_p->render();
			if( link_p->m_prev_p )
				link_p = link_p->m_prev_p;
			else
				break;
		}
	}
}


void Linklist::clear_list()
{
	// obj list clear
	if( m_first_p )
	{
		Link *link_p = m_first_p;
		for( int i = 0; i < MAX_LINKS; i++ )
		{
			Link *next_p;
			if( link_p->m_next_p )
			{

				next_p = link_p->m_next_p;
				int handle = link_p->get_handle();
				remove( handle );
				link_p = next_p;
			}
			else
			{
				int handle = link_p->get_handle();
				remove( handle );
				break;
			}
		}
	}

	m_num_links_used = 0;
}


Link *Linklist::get_obj( int link_handle )
{
	Link *link_p = m_first_p;
	while( link_p && link_handle != link_p->get_handle() )
	{
		link_p = link_p->m_next_p ? link_p->m_next_p : NULL;
	}

	if( !link_p )
	{
		dbgmsg( "Find link error! Link handle not found or invalid.\n" );
		return( NULL );
	}
	else
	{
		return( link_p );
	}
}


void Linklist::remove( int &link_handle, bool deleteobj )
{
	Link *link_p = m_first_p;
	while( link_p && link_handle != link_p->get_handle() )
	{
		if( link_handle < 0 )
		{
			link_p = NULL;
			break;
		}
		link_p = link_p->m_next_p ? link_p->m_next_p : NULL;
	}

	if( !link_p )
	{
		dbgmsg( "Delete link error! Link handle not found or invalid.\n" );
		return;
	}

	// this link is first in the list
	if( link_p == m_first_p )
	{
		// make next link the first, unless this is the last (only) link!
		if( link_p->m_next_p )
		{
			m_first_p = link_p->m_next_p;
			// sever our connection with the linklist
			m_first_p->m_prev_p = NULL;
		}
		else
		{
			m_first_p = NULL;
			m_last_p = NULL;
		}

	}
	else
	{
		// if the link is not the last
		if( link_p->m_next_p )
		{
			// cut out the middle man!
			link_p->m_next_p->m_prev_p = link_p->m_prev_p;
			link_p->m_prev_p->m_next_p = link_p->m_next_p;
		}
		else
		{
			// must be the last in the list
			m_last_p = link_p->m_prev_p;
			// sever our connection with the linklist
			m_last_p->m_next_p = NULL;
		}
	}

	// owned
	if( deleteobj )
	{
		dbgmsg( "Deleting link, handle %d.\n", link_handle );
		delete( link_p );
	}
	else
		dbgmsg( "Link not deleted as specified. PLEASE DELETE MANUALLY IMMEDIATELY!!! handle %d.\n", link_handle );

	link_p = NULL;

	m_num_links_used--;
	link_handle = -1;

}