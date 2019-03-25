/****************************************
/	Al (c) 2008 - guinode.h
/****************************************
/	GUI element
/****************************************/

#ifndef _GUINODE_H_
#define _GUINODE_H_

/*************** INCLUDES ***************/

#include "quad.h"
#include "font.h"

#include "paths.h"

/*************** CONSTANTS **************/



/**************** CLASSES ***************/

typedef enum
{
	SPIN_STATE_NONE		=	0,
	SPIN_STATE_INTERACTIVE,
	SPIN_STATE_SLOW,

	PSIN_STATE_EPAD		=		0xffffffff

}spin_state_e;

typedef enum
{
	BUTTON_STATE_IDLE	=	0,
	BUTTON_STATE_HIDE,
	BUTTON_STATE_MOVE,

	BUTTON_STATE_EPAD	=		0xffffffff

}button_state_e;

typedef enum
{
	BUTTON_TYPE_STANDARD = 0,
	BUTTON_TYPE_TOGGLE,
	BUTTON_TYPE_DROPLIST,
	BUTTON_TYPE_MENUGUI,

	BUTTON_TYPE_PAD		=		0xffffffff

}button_type_e;

typedef struct
{
	int l, r;
}button_handles_s;


class GUINode: public Quad
{

public:

	GUINode( char *text_p, float x, float y, int *param_p, int value, int num_options, char *list_text_p );
	GUINode( char *text_p, char *texture_name_p, float init_x, float init_y, float w, float h, int *param = NULL, int value = 0, unsigned int text_col = 0xffffffff, unsigned int col = 0xffffffff );
	GUINode( char *text_p, float x, float y, int *param_p, int value, float w = 96.0f );
	GUINode( char *text_p, float x, float y, float w, bool *param_p );
	~GUINode();

	virtual void		execute				( void );
	virtual void		render				( void );

	void				set_new_pos			( float x, float y, float rate, bool text = true );
	bool				collided			( int x, int y );
	void				set_spin_state		( int state, float speed = 25.0f )			{ m_spin_state = (spin_state_e)state; m_maxrot = speed; }

	void				activate			( bool on );

	button_handles_s	m_stdbutton_handles;
	int					m_selection;
	bool				m_active;
	bool				m_over;

protected:

	char				m_text[64];
	unsigned int		m_text_col;

	col_info_s			m_col_info;

	vector2				m_find_co;

	spin_state_e		m_spin_state;
	button_state_e		m_state;
	button_type_e		m_type;

	float				m_text_x;
	float				m_text_y;
	float				m_find_rate;

	float				m_rotspeed;
	float				m_maxrot;
	float				m_timer;

	bool				m_toggled;
	bool				m_selected;

	int					*m_parameter_p;
	bool				*m_toggle_p;
	int					m_value;
	int					*m_option_handles_p;
	int					m_num_options;


private:

};


/*********** GLOBAL VARIABLES ***********/



/********* FUNCTION PROTOTYPES **********/


#endif