#ifndef _COLLECTABLE_H
#define _COLLECTABLE_H

#include	"quad.h"


class Collectable : public Quad
{
	public:

		Collectable( void );
		Collectable(vector2 position, int type ,char *name_p);
		~Collectable( void );

		virtual void		execute					( void );
		virtual void		render					( void );
		int					collect					( void );

		bool				active					( void ){return m_active;};

		void				checkCollisions			( void );

		int					type					( void ){return m_type;};

	private:

		bool m_active;
		int m_type;
};

#endif