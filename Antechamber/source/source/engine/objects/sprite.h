/****************************************
/	Al (c) 2008 - sprite.h
/****************************************
/	Sprite!
/****************************************/

#ifndef _SPRITE_H_
#define _SPRITE_H_

/*************** INCLUDES ***************/

#include	"quad.h"

/*************** CONSTANTS **************/


typedef enum
{
	ANIM_STATE_STAND = 0,
	ANIM_STATE_WALK,
	ANIM_STATE_DEATH,
	ANIM_STATE_TAKE_OFF,
	ANIM_STATE_IN_AIR,
	ANIM_STATE_LAND,
	ANIM_STATE_SAVE,
	ANIM_STATE_SPAWN,
	ANIM_STATE_DEATH2,
	ANIM_STATE_DEATH3,

	MAX_ANIM_STATES,

	ANIM_STATE_FINISHED
	
}anims_e;


/**************** CLASSES ***************/

typedef struct
{
	unsigned char	frames[MAX_ANIM_STATES];
	short			anim_states;
	vector2			texture_size;
	vector2			*frame_size;

}sprite_info_s;


class Sprite: public Quad
{

public:

	Sprite();
	Sprite( char *path_p, char *name_p, float x, float y, unsigned int col = 0xffffffff );
	Sprite( char *path_p, char *name_p, char *script_p, float x, float y, unsigned int col = 0xffffffff );
	~Sprite();

	virtual void		execute				( void );
	virtual void		render				( void );

private:

	void				update_animation	( void );
	void				read_script			( char *script_p );


protected:

	float				m_ticker;
	anims_e				m_anim_state;
	anims_e				m_last_state;
	sprite_info_s		m_sprite;

	short				m_current_frame;
	vector2				m_frame_origin;

	float				m_texel_to_uv_x;
	float				m_texel_to_uv_y;

	bool				m_dir;

	// needed for animation change
	float				m_velocity_y;
	float				m_velocity_x;

};


/*********** GLOBAL VARIABLES ***********/



/********* FUNCTION PROTOTYPES **********/


#endif