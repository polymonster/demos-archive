#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include	"quad.h"


class Projectile : public Quad
{
	public:

		Projectile( void );
		Projectile(vector2 position, vector2 direction, int type, char *name_p);
		~Projectile( void );

		virtual void		execute					( void );
		virtual void		render					( void );

		void				spawn					(vector2 position, vector2 direction);
		void				spawn					(vector2 position, vector2 direction, int type);

		void				kill					( void ){m_active = false;};

		bool				emmit_particle			( void );

		bool				active					( void ){return m_active;};

		void				checkCollisions			( void );

		void				notify_player_position	(vector2 position);

		vector2				particle_pos			( void ) {return m_particle_spawn_pos;};

		int					get_type				( void ) {return m_type;};


	private:

		//general
		bool m_active;
		float m_life;
		int m_type;
		bool m_move;
		float m_speed;

		//for homing
		vector2 m_direction_vector;
		float m_rotation;
		float m_turning_circle;
		vector2 m_player_position;


		//for particle emmision
		float m_particle_timer;
		float m_emmision_rate;
		vector2 m_particle_spawn_pos;

		vector2 m_root_pos;

};

#endif