/****************************************
/	Al (c) 2008 - editnode.h
/****************************************
/	Draw 2D quad
/****************************************/

#ifndef _EDITNODE_H_
#define _EDITNODE_H_

/*************** INCLUDES ***************/

#include <windows.h>

#include "quad.h"
#include "paths.h"

/*************** CONSTANTS **************/

#define			MAX_NODES		64

typedef enum
{
	NODE_ENERGY_CELL = 0,
	NODE_INTERCOM,
	NODE_GATE,
	NODE_CP,
	NODE_TURRET,
	NODE_ELECTROBALL,
	NODE_SPAWN,

	NUM_TYPES

}node_type_e;

typedef struct
{
	vector2		pos;
	int			gate_type;
	int			gate_cp_handle;
	int			turret_direction;
	int			turret_type;
	int			intercom_id;
}node_data_s;

/**************** CLASSES ***************/


class EditNode : public Quad
{

public:

	EditNode( node_type_e type, node_data_s data );
	~EditNode();

	void				execute			( void );
	void				render			( void );

	node_type_e			m_type;
	node_data_s			m_data;

protected:

private:

};


/*********** GLOBAL VARIABLES ***********/

extern	vector2 g_nodedims[];

/********* FUNCTION PROTOTYPES **********/


#endif