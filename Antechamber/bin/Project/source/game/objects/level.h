/****************************************
/	Al (c) 2008 - level.h
/****************************************
/	Draw 2D tilemap
/****************************************/

#ifndef _LEVEL_H_
#define _LEVEL_H_

/*************** INCLUDES ***************/

#include <windows.h>
#include <windowsx.h>

#include <d3d9.h>
#pragma comment ( lib, "d3d9.lib" )

#include "level.h"
#include "editnode.h"
#include "guinode.h"

#include "actor.h"
#include "sloperules.h"
#include "blocker.h"
#include "controlpanel.h"
#include "projectile.h"
#include "turret.h"
#include "collectable.h"
#include "traps.h"
#include "particles.h"

/*************** CONSTANTS **************/

#define		MAP_WIDTH			40
#define		MAP_HEIGHT			16

#define MAP_SIZE (MAP_WIDTH*MAP_HEIGHT)

#define		MAP_SCROLL_NONE		-1
#define		MAP_SCROLL_LEFT		0
#define		MAP_SCROLL_RIGHT	1
#define		MAP_SCROLL_UP		2
#define		MAP_SCROLL_DOWN		3

#define		MAX_PROJECTILES		50
#define		MAX_PARTICLES		150

#define		MAX_INTERCOMS		4
#define		MAX_CPS				8
#define		MAX_GATES			12
#define		MAX_SMARTBALLS		6
#define		MAX_TURRETS			16
#define		MAX_COLLECTABLES	64

typedef enum
{
	TURRET_TYPE_CANNON = 0,
	TURRET_TYPE_ROCKET,
	TURRET_TYPE_LAZER
}turret_list_e;

typedef enum
{
	GATE_BRIDGE = 0,
	GATE_DOOR
}gate_list_e;

typedef enum
{
	EDIT_MODE_PLACE_ENERGY_CELL = 0,
	EDIT_MODE_PLACE_INTERCOM,
	EDIT_MODE_PLACE_GATE,
	EDIT_MODE_PLACE_CP,
	EDIT_MODE_PLACE_TURRET,
	EDIT_MODE_PLACE_ELECTROBALL,
	EDIT_MODE_PLACE_SPAWN,
	EDIT_MODE_LEVEL_BUILDER

}edit_mode_e;


/**************** CLASSES ***************/

typedef struct
{
	float x, y, z, rhw;
	unsigned int col;
	float u, v;

}level_vert_s;

class Level: public Link
{

public:

	Level();
	Level( char *tileset_p, char *map_file_p, vector2 room, int origin_y = 0, int size = 32, int tiles = 64, bool edit = false );
	~Level();

	virtual void		execute				( void );
	virtual void		render				( void );

	short				get_tile_size		( void )				{ return( m_tile_size ); }
	short				get_map_width		( void )				{ return( MAP_WIDTH ); }
	short				get_tile			( int index );			
	int					get_grid_x			( int pos );
	int					get_grid_y			( int pos );
	int					hit_collidable_tile	( int tile	);

	void				get_current_map		(vector2 *pos);

	void				set_new_map			( int scroll_dir );
	void				set_new_map			( vector2 new_map );

	void				cleanup_map			( void );
	void				cleanup_level		( void );

	void				set_edit_mode		( bool set )			{ m_edit_mode = set; };

	void				save_map			( void );

	void				obtainGates         (vector2 *position, vector2 *dimensions,int id);
	void				obtainControlPanels (vector2 *position, vector2 *dimensions,int id);
	int					useControlPanel		(int id, bool use);
	void				notify_player_position (vector2 position);
	void				obtainSmartBalls(vector2 *position, vector2 *dimensions,int id);

	//this function might be removed later on (for now it creates the static turrets, control panels and so on)
	void				init_objects		( void );

	int					number_of_gates					( void )	{return m_number_of_gates;};
	int					number_of_control_panels		( void )	{return m_number_of_control_panels+m_number_of_intercoms;};
	int					number_of_turrets				( void )	{return m_number_of_turrets;};
	int					number_of_collectables			( void )	{return m_number_of_collectables;};
	int					number_of_smartballs			( void )	{return m_number_of_smartballs;};

	bool				projectileCollision				(vector2 min_extent, vector2 max_extent,vector2 *offset);
	int					collectableCollision			(vector2 min_extent, vector2 max_extent);

	void				spawn_particle					(vector2 position);
	void				spawn_particle					(vector2 position, int type);

	tga_texture_s		m_projectile_tex[3];

	vector2				get_spawn_pos					()	{return m_spawn_pos;};

	void				animate_acid					();

protected:

	void				set_vert			( level_vert_s *fvf_struct_p, int x, int y, float u, float v, int iterator );
	void				set_vert_pos		( int &x, int &y, int tile );

	tga_texture_s		m_tileset;
	tga_texture_s		m_edit_cursors;
	char				m_map_filename[32];

	int					m_tilemap[640];

	short				m_tile_size;
	short				m_num_tex_tiles;
	short				m_origin_y;
	int					m_col;

	short				m_current_map_x;
	short				m_current_map_y;
	short				m_map_rooms;

	bool				m_edit_mode;
	edit_mode_e			m_editor_mode;

	short				m_edit_cursor;

	int					m_hard_tileset_handle;
	int					m_soft_tileset_handle;

	Quad				*m_edit_cursor_p;

	Blocker				*m_blockers[MAX_GATES];
	ControlPanel		*m_control_panels[MAX_CPS+MAX_INTERCOMS];
	Turret				*m_turrets[MAX_TURRETS];
	Collectable			*m_collectables[MAX_COLLECTABLES];
	Trap				*m_traps[8];
	Actor				*m_smartballs[MAX_SMARTBALLS];

	Projectile			*m_projectile[MAX_PROJECTILES];
	Particle			*m_particle[MAX_PARTICLES];

	int					m_backdrop_handle;

	byte				*m_mapdata_p;

	vector2				m_spawn_pos;

	float				acid_anim_timer;

	//trackers
	int m_number_of_turrets;
	int m_number_of_gates;
	int m_number_of_control_panels;
	int m_number_of_intercoms;
	int m_number_of_collectables;
	int m_number_of_traps;
	int m_number_of_smartballs;

	char				m_turret_options[3][32];
	char				m_gate_options[2][32];

	GUINode				*m_edit_buttons_p[7];
	EditNode			*m_edit_node_p[MAX_NODES];
	int					m_num_nodes;

	bool				m_save;
	bool				m_gate_type;


	//	vector2				*m_collision_list_p;
	//	bool				m_update_collision_map;

private:

};


/*********** GLOBAL VARIABLES ***********/

extern	Level			*g_level_p;

/********* FUNCTION PROTOTYPES **********/


#endif