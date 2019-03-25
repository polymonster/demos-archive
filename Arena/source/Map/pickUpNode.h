#ifndef _PICKUP_NODE
#define _PICKUP_NODE

#include "globals.h"
#include "player.h"
#include "myMath.h"

typedef enum 
{
	HEALTH25 = 100,
	ARMOUR25,
	HEALTH50,
	ARMOUR50,
	HEALTH100,
	ARMOUR100,

	AMMO_SHOTGUN,
	AMMO_MACHINEGUN,
	AMMO_LAZER,
	AMMO_PLASMA,

	GUN_SHOT,
	GUN_MACHINE,
	GUN_LAZER,
	GUN_PLASMA,

}ePickups;

class CPNode {

private:

	SVector3 m_pos;
	int m_type;
	int m_radius;
	float m_timer;
	int m_timeLimit;
	float m_rotation;

public:

	CPNode();
	CPNode(int type, SVector3 pos);
	~CPNode();
	void draw(CPlayer &alj);
	void render(CWeapons *weapon[]);
	void highlight();
	void check(CPlayer &alj);
	const char *name();

	SVector3 pos(){return m_pos;};
	void setPos(SVector3 pos);
	void setType(int type);

	void saveData(ofstream &file);

	int type(){return m_type;};

	bool m_alive;
};


#endif