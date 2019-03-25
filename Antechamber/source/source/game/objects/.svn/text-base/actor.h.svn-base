/****************************************
/	Al (c) 2008 - actor.h
/****************************************
/	Sprite based character
/****************************************/

#ifndef _ACTOR_H_
#define _ACTOR_H_

/*************** INCLUDES ***************/

#include	"sprite.h"

/*************** CONSTANTS **************/

#define RESPAWN_RATE 2.0f

typedef enum
{
	ACTOR_STATE_INIT	= 0,
	ACTOR_STATE_IDLE,
	ACTOR_STATE_WALK,

	ACTOR_STATE_PAD		= 0xffffffff
}actor_state_e;

/**************** CLASSES ***************/
typedef struct
{
	int id;
	vector2 start;
	vector2 end;

}potential_collider_s;	

typedef struct
{
	vector2 top;
	vector2 bottom;
	vector2 front;
	vector2 back;

}player_bb_s;


class Actor: public Sprite
{

public:

	Actor();
	Actor( char *name_p, float x, float y, bool user_control = false, int player_number = 0, unsigned int col = 0xffffffff);
	~Actor();

	virtual void		execute					( void );
	virtual void		render					( void );

	void				check_level_collision	( void );
	void				resolve_collision		( int &x, int &y );

	void				new_collision_check		( void );
	void				wall_collision			( player_bb_s player_boundry , float additional_offset );
	void				floor_collision			( player_bb_s player_boundry);
	void				slope_collision			( player_bb_s player_boundry);
	void				underside_slope_collision( player_bb_s player_boundry);
	player_bb_s			build_bounding_box		( void );	
	void				gate_collision			( player_bb_s player_boundry );
	void				set_pos					( vector2 position );
	void				check_for_ledge			( player_bb_s player_boundry );
	void				check_control_panels	( player_bb_s player_boundry );
	void				pool_collision			( player_bb_s player_boundry );
	void				actor_collision			();
	void				save_game				();
	void				load_save_data			( void );

	void				emmitParticle			( vector2 *position );

	void				respawn();
	void				control();
	void				land();
	void				jump();




protected:

	float				m_gravity;
	int					m_last_x;
	int					m_last_y;
	int					m_origin;

	float				m_direction_timer;

	actor_state_e		m_state;

	vector2				m_center_pos;

	unsigned short		m_jumps			:3;
	unsigned short		m_user_control	:1;

	unsigned short		pad				:12;

	bool				m_pressed_jump;

	bool				m_pressed_interact;

	bool				m_emmit_particle;

	float				m_respawn_timer;

	bool				m_jumping;
	bool				m_in_air;
	bool				m_landing;
	bool				m_saving;

	int m_lives;
	int m_score;
	bool m_dead;

	int m_control_scheme;

	bool m_display_tooltip;


};


/*********** GLOBAL VARIABLES ***********/



/********* FUNCTION PROTOTYPES **********/


#endif