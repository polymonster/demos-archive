#ifndef _BLOCKER_H
#define _BLOCKER_H

#include	"quad.h"

typedef enum
{
	DOOR = 0,
	BRIDGE,

}e_blocker_types;

class Blocker : public Quad
{
	public:

		Blocker( void );
		Blocker(vector2 position, vector2 dimensions,  int dir, int type, int cp, char *name_p);
		~Blocker( void );

		virtual void		execute					( void );
		virtual void		render					( void );
		void				activate				( void );

		unsigned int m_cp_id;

	private:

		bool m_open;
		int m_type;
		bool m_inmotion;
		int m_channelID;
		vector2 m_to_position;
		vector2 m_closed_position;
};

#endif