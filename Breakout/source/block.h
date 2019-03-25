#ifndef _BLOCK_H
#define _BLOCK_H

#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <assert.h>
#include <memory.h>
#include <signal.h>
#include "PS2Defines.h"
#include "ps2maths.h"
#include "ps2matrix4x4.h"
#include "pad.h"
#include "sps2wrap.h"
#include "dma.h"
#include "texture.h"


class CBlock
{

private:

	float m_x, m_y;
	float m_newx, m_newy;
	float m_boundingBox[4];
	float m_rotrad;
	int m_zrot;
	int m_height,m_width;
	int m_tex_x, m_tex_y;
	int m_tex_xsize, m_tex_ysize;

	int m_poweruptype;
	int megacount;
	bool m_megaball;
	int m_ballcount;
	bool m_newball;
	bool m_sticky;
	int m_diffx;
	int m_diffy;
	bool m_stickyonce;

	int m_bat_type;
	bool m_bat_resize;

    int m_goal;
	int m_life;
	bool m_alive;
	bool m_active;

public:

	CBlock();

	void initBat(int x,int y);
	void initBall(int x, int y, bool setup );
	void initBlock(int x, int y);
	void initBlock(int x, int y, int xsize, int ysize );

	void spawnPowerUp( int which, int x, int y );
	void execPowerUp( CBlock *bat, CBlock *ball );

	void setMega( bool );
	bool getMega() { return m_megaball; };

	void resetBall();

	void setNewBall( bool );
	bool getNewBall() { return m_newball; };

	void setBatMode( int );
	bool getBatMode() { return m_bat_type; };
	void crazyBat();

	void updatePowerPos();
	void updatePos( int );
	void updateBall( CBlock *bat, int ballcount, bool press );

	void texCoord( int xcoord, int ycoord, int xsize, int ysize );

	void draw( CTexture tex );

	void checkCollision( CBlock *checkWith, int &type );
	bool batCollision( CBlock *checkWith );

	void deflectBall( int type, int bricktype );
	int batToBall( CBlock *ball );
	void setSticky( bool );
	bool isSticky() { return m_sticky; };
	
	bool isAlive(){return m_alive;};
	void kill();
	void spawn();
	void activate();
	bool isActive() { return m_active; };

	int incrementZrot( int value );

	void setZrot( float value );
	int getZrot();

	void launchBall();
	void setPos( int x, int y );
	void setLife( int );
	void setSize( int );

	int getXpos( int );
	int getYpos( int );
	int getLife() { return m_life; };

	void getPos( float *x, float *y );
	void getSize( float *x, float *y );
	void getDiff( int *x, int *y );
	void getDims( float *v0, float *v1, float *v2, float *v3 );
	
	~CBlock();

};


#endif
