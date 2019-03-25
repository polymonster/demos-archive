#ifndef _TRAPS_H
#define _TRAPS_H

#include	"quad.h"

class Trap : public Quad
{
	public:

		Trap( void );
		Trap(vector2 position, int type , vector2 activate_range, vector2 speed, char *name_p);
		~Trap( void );

		virtual void		execute					( void );
		virtual void		render					( void );

		void				notify_player_position	(vector2 position);
		int					type					( void ){return m_type;};

		void				stop					( void );
		void				check_collision			( void );

	private:

		int m_type;
		vector2 m_player_position;
		int m_dir;
		bool m_activate;
		vector2 m_original_pos;

		float m_up_speed;
		float m_down_speed;
		vector2 m_range;
};

#endif