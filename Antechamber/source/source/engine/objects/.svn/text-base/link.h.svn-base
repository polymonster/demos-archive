/****************************************
/	Al (c) 2008 - link.h
/****************************************
/	a link in the chain!
/****************************************/

#ifndef _LINK_H_
#define _LINK_H_

/*************** INCLUDES ***************/

#include <windows.h>

#include "dxio.h"


/*************** CONSTANTS **************/

#define			MAX_NAME_LEN			16
#define			MAX_LINKS				2048

#define			PI						3.14159265

#define			RENDER_FRONT			100.0f
#define			RENDER_NEAR_FRONT		50.0f
#define			RENDER_MID_FRONT		10.0f
#define			RENDER_MIDDLE			5.0f
#define			RENDER_MID_BACK			2.5f
#define			RENDER_NEAR_BACK		1.0f
#define			RENDER_BACK				0.0f

/**************** CLASSES ***************/


class Linklist
{
	friend class Link;

public:

	Linklist();
	Linklist( char *list_name_p, short max_links );
	~Linklist();

	int				add				( Link *link_p );
	void			remove			( int &handle, bool deleteobj = true );
	Link			*get_obj		( int handle );
	int				get_used_links	( void )				{ return( m_num_links_used ); };

	void			execute_list	( void );
	void			render_list		( void );
	void			clear_list		( void );

	Link			*m_first_p;
	Link			*m_last_p;

private:

	int				m_handle_pool;
	short			m_num_links;
	short			m_num_links_used;
	unsigned char	m_list_name[MAX_NAME_LEN];

};


class Link
{

public:

	friend class Linklist;

	Link();
	Link( int &link_id );
	virtual ~Link();

	virtual void	execute			( void )				{};
	virtual void	render			( void )				{};
	int				get_handle		( void )				{ return( m_link_handle ); };

	void			set_priority	( float priority );
	float			get_priority	( void )				{ return m_render_priority; };

	Link			*m_next_p;
	Link			*m_prev_p;

protected:

	int				m_link_handle;
	float			m_render_priority;

private:

};


/*********** GLOBAL VARIABLES ***********/

extern Linklist	*g_object_list_p;


/********* FUNCTION PROTOTYPES **********/


#endif