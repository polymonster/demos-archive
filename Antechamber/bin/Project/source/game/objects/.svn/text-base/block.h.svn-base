/****************************************
/	Al (c) 2008 - block.h
/****************************************
/	A level block
/****************************************/

#ifndef _BLOCK_H_
#define _BLOCK_H_

/*************** INCLUDES ***************/

#include "quad.h"

/*************** CONSTANTS **************/



/**************** CLASSES ***************/

class Block: public Quad
{

public:

	Block();
	Block( char *texture_filename, int texture_index, float x, float y, unsigned int col, bool collidable = true );
	~Block();

	virtual void		execute				( void );
	virtual void		render				( void );

protected:

	bool				m_collidable;

private:

};


/*********** GLOBAL VARIABLES ***********/



/********* FUNCTION PROTOTYPES **********/


#endif