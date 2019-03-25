#ifndef _PARTICLES_H
#define _PARTICLES_H

#include "globals.h"
#include "myMath.h"
#include "textureSingleton.h"

#define MAX_PARTICLES 50
#define MAX_RICOCHET_PARTICLES 10
#define MAX_BLOOD_PARTICLES 20

typedef struct						
{
	bool	active;					
	float	life;					// Particle Life
	float	fade;					// Fade Speed
	float	r,g,b;					// Colour Value
	float	x,y,z;					// Position
	float	xdir,ydir,zdir;			// Direction /angle
	float	xg,yg,zg;				// Gravity
	SVector3 poly[4];
	SVector3 particlePoly[4];
}
particles;

static float colors[12][3]=	
{
	{0.2f,0.1f,0.5f},{1.0f,0.75f,0.63f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.6f},{0.5f,1.0f,0.55f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
	{0.5f,0.5f,1.7f},{0.75f,0.5f,0.9f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};	

class CParticles {

public:

	CParticles();

	void initRicochet(SVector3 pos,int type);
	void ricochetParticles();
	void updateRicochet();

	void initBlood(SVector3 pos);
	void bloodParticles();
	void updateBlood();

	void initBulletHole(SVector3 pos, SVector3 poly[]);
	void bulletHole();
	void update();

	void initMuzzle(SVector3 pos);
	void muzzleFlash();

	bool inUse;

private:

	
	particles particle[MAX_PARTICLES];
	float slowdown;
	int xspeed, yspeed;
	GLuint	texture[1];
	SVector3 m_poly[4];
	int m_activeParticles;

	int texId[5];

	int m_type;
};


#endif	