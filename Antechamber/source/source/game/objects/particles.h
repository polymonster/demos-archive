#ifndef _PARTICLE_H
#define _PARTICLE_H

#include	"quad.h"


class Particle : public Quad
{
	public:

		Particle( void );
		Particle(vector2 position, char *name_p);
		~Particle( void );

		virtual void		execute					( void );
		virtual void		render					( void );
		void				spawn					(vector2 position);
		void				spawn					(vector2 position,int m_type);


		bool				active					( void ){return m_active;};

	private:

		bool m_active;
		float m_life;
		int m_type;
		float m_fade;
		vector2 m_rotation;

		vector2 m_absolute_pos;
		vector2 m_absolute_size;

		Quad *explosion;
};

#endif