#ifndef _TURRET_H
#define _TURRET_H

#include	"quad.h"

class Turret : public Quad
{
	public:

		Turret( void );
		Turret(vector2 position, float fire_rate,	int type,  int projectile_type, int rotation_range, char *name_p);
		~Turret( void );

		virtual void		execute					( void );
		virtual void		render					( void );
		void				notify_player_position	( vector2 position );
		int					shoot					( vector2 *pos, vector2 *dir );

	private:

		vector2 m_aim_vector;
		float m_new_rotation;
		float m_fire_rate;
		float m_fire_timer;
		float m_cooldown_timer;

		int m_rotation_lock_max;
		int m_rotation_lock_min;

		int m_origin_rotation;

		Quad *m_cannon_quad;

		bool m_inverse;

		int m_projType;

		int m_sound_channel;

		int m_type;


};

#endif