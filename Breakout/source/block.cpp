#include "block.h"


CBlock::CBlock()
{
	m_x = 0;
	m_y = 0;
	m_alive = false;
	m_ballcount = 1;
}

CBlock::~CBlock(){};

void CBlock::initBat(int x, int y)
{

	m_life = 5;

	m_active = true;
	m_alive = true;
	m_bat_type = 0;
	m_bat_resize = false;

	m_x = x, m_y = y;

	m_width = 32;
	m_height = 8;

	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;
}

void CBlock::initBall(int x, int y, bool setup )
{
	m_zrot = 45;
	megacount = 0;

	m_alive = setup;
	m_goal = setup;
	m_megaball = false;
	m_sticky = false;

	m_x = x, m_y = y;

	m_width = 5;
	m_height = 5;

	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;
}

void CBlock::initBlock(int x, int y)
{
	m_alive = true;

	m_x = x, m_y = y;

	m_width = 16;
	m_height = 8;

	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;
}

void CBlock::initBlock(int x, int y, int xsize, int ysize )
{
	m_alive = true;

	m_x = x, m_y = y;

	m_width = xsize;
	m_height = ysize;

	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;
}

void CBlock::spawnPowerUp( int which, int x, int y )
{
	m_newball = false;
	m_alive = true;
	m_poweruptype = which;

	m_x = x, m_y = y;

	m_width = 14;
	m_height = 8;

	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;

	m_tex_x = 64;
	m_tex_xsize = 46;
	m_tex_ysize = 30;

	switch( which )
	{
	case 0:
		m_tex_y = 24;
		break;
	case 1:
		m_tex_y = 56;
		break;
	case 2:
		m_tex_y = 88;
		break;
	case 3:
		m_tex_y = 120;
		break;
	case 4:
		m_tex_y = 152;
		break;
	case 5:
		m_tex_y = 184;
		break;
	}
}

// == EXECUTE POWERUP FUNCTIONS =============
void CBlock::execPowerUp( CBlock *bat, CBlock *ball )
{
	switch( m_poweruptype )
	{
	case 0:
		// bat mode normal, large size
		bat->setBatMode( 0 );
		bat->setSize( 48 );
		break;
	case 1:
		// megaball mode ( time limit )
		ball->setMega( true );
		break;
	case 2:
		// bat mode normal, small size
		bat->setBatMode( 0 );
		bat->setSize( 22 );
		break;
	case 3:
		// add a ball
		m_newball = true;
		break;
	case 4:
		// bat mode crazy, variable size
		bat->setBatMode( 1 );
		break;
	case 5:
		// bat mode sticky, size medium
		bat->setBatMode( 2 );
		bat->setSize( 36 );
		break;
	}
}

void CBlock::texCoord( int xcoord, int ycoord, int xsize, int ysize )
{
	m_tex_x = xcoord;
	m_tex_y = ycoord;
	m_tex_xsize = xsize;
	m_tex_ysize = ysize;
}

void CBlock::updatePos( int amount )
{

	if( m_x < 260 && m_x > -260 )
		m_x += amount;
	else if ( m_x >= 260 )
		m_x = 259;
	else if ( m_x <= -260 )
		m_x = -259;

	// reset bounding box position and size
	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;
}

void CBlock::updatePowerPos()
{
	if( m_alive )
	{
		m_y += 3;
	}
}

void CBlock::updateBall( CBlock *bat, int ballcount, bool press ) 
{ 

	int batx = bat->getXpos( 0 );
	int baty = bat->getYpos( 0 );

	int diffx = 0, diffy = 0;


	if( m_megaball )
	{
		m_tex_x = 88;
		megacount++;
		if( megacount > 1200 )
		{
			megacount = 0;
			m_megaball = false;
		}
	}
	else
	{
		m_tex_x = 64;
	}

	if( m_goal )
	{
		m_alive = true;
		setPos( batx, baty-16 );
	}
	else
	{
		m_newx = m_x;
		m_newy = m_y;

		m_rotrad = ( m_zrot * PI / 180 ); 
		m_x += float( sin( m_rotrad ) ) * 4;
		m_y -= float( cos( m_rotrad ) ) * 4;

		if ( m_sticky )
		{
			bat->getDiff( &diffx, &diffy );
			setPos( batx + diffx/2, m_newy );
		}

		if( press )
			m_sticky = false;

		if( m_x > 290 ) 
			deflectBall( 0, 0 ); 
		if( m_x < -290 ) 
			deflectBall( 0, 0 ); 
		if( m_y > 235 ) 
		{
			// despawn ball if collision with goal
			m_alive = false;
			// but still deflect so that the angle is pointing up
			deflectBall( 1, 0 );
		}
		if( m_y < -230 ) 
			deflectBall( 1, 0 ); 
	}

	// reset bounding box position and size
	m_boundingBox[0] = m_x - m_width;
	m_boundingBox[1] = m_y - m_height;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;
 
}

void CBlock::deflectBall( int type, int bricktype )
{
	if( type != -1 )
		printf("Ball Deflection\n");

	if( bricktype != 0 && m_megaball )
	{
		// do nothing! keep going
		return;
	}
	else
	{
		if( type == 0 )
		{
			m_zrot = ( 180 - ( m_zrot - 90 ) ) + 90;
		}
		else if( type == 1 )
		{
			m_zrot = 180 - m_zrot;
		}
		else if( type == 2 )
		{
			m_zrot = 135 + rand() %6;
		}
		else if( type == 3 )
		{
			m_zrot = 45 + rand() %6;
		}
		else if( type == 4 )
		{
			m_zrot = 315 + rand() %6;
		}
		else if( type == 5 )
		{
			m_zrot = 225 + rand() %6;
		}
		else
			return;
	}
}

bool CBlock::batCollision(CBlock *checkWith)
{
	float v[4];
	float bx, by;

	checkWith->getPos(&bx,&by);

	if( (bx > m_boundingBox[0]) && (by > m_boundingBox[1]) &&
		(bx < m_boundingBox[2]) && (by < m_boundingBox[3]) &&
		!checkWith->isSticky() ) 
	{
		return true;
		m_stickyonce = true;
	}
	else if( (bx > m_boundingBox[0]) && (by > m_boundingBox[1]) &&
		(bx < m_boundingBox[2]) && (by < m_boundingBox[3]) &&
		checkWith->isSticky() )
	{
		return m_stickyonce;
		m_stickyonce = false;
	}
		return false;
}

void CBlock::crazyBat()
{
	if( m_bat_type == 1 )
	{
		if( !m_bat_resize )
			m_width++;
		else if( m_bat_resize )
			m_width--;

		if( m_width > 48 )
			m_bat_resize = true;
		else if( m_width < 16 )
			m_bat_resize = false;
	}
}

int CBlock::batToBall( CBlock *ball )
{
	if( m_bat_type == 2 )
		ball->setSticky( true );
	else
		ball->setSticky( false );

	m_diffy = m_y -4;
	// ball angle based on position of collision with bat
	m_diffx = ( ball->getXpos( 0 ) - (int)m_x ) *2;
	return m_diffx;
}

void CBlock::draw( CTexture tex )
{
	// DRAW A QUAD

	if( m_alive )
	{
		int c = 2048;

		// Select Tex1
		tex.Select();

		// Start DIRECT mode transfer
		VIFDynamicDMA.StartDirect();

			VIFDynamicDMA.StartAD();

				// Add RGBAQ ( colour ) data
				VIFDynamicDMA.AddAD( RGBAQ_SET( 127,127,127,127,0 ), RGBAQ );

			VIFDynamicDMA.EndAD();

			// Add the GIFTag
			VIFDynamicDMA.Add128( GS_GIFTAG_BATCH(	1,								// NLOOP
 													1,								// EOP
 													1,								// PRE
 													GS_PRIM(	PRIM_SPRITE, 		// PRIM
																PRIM_IIP_FLAT, 
																PRIM_TME_ON, 
																PRIM_FGE_OFF, 
																PRIM_ABE_ON, 
																PRIM_AA1_OFF, 
																PRIM_FST_UV, 
																PRIM_CTXT_CONTEXT1, 
																PRIM_FIX_NOFIXDDA ), 
 													GIF_FLG_PACKED,					// FLG
 													GS_BATCH_4(	GIF_REG_UV,			// BATCH
 																GIF_REG_XYZ2, 
 																GIF_REG_UV, 
 																GIF_REG_XYZ2 ) ) );

			// Add Vertex data

			// Add UV texture coordinates
			VIFDynamicDMA.Add128( PACKED_UV( m_tex_x << 4, m_tex_y << 4 ) );

			// Add the position of bottom left vertex
			VIFDynamicDMA.Add128( PACKED_XYZ2(	( c - m_width + (int)m_x ) << 4, 
												( c - m_height + (int)m_y ) << 4, 
												  128, 
												  0 )	);

			// Add UV texture coordinates
			VIFDynamicDMA.Add128( PACKED_UV( ( m_tex_x + m_tex_xsize ) << 4,
											 ( m_tex_y + m_tex_ysize ) << 4 ) );

			// Add the position of top right vertex
			VIFDynamicDMA.Add128( PACKED_XYZ2(	( c + m_width + (int)m_x ) << 4, 
												( c + m_height + (int)m_y ) << 4, 
												  128, 
												  0 )	);

		// End the direct mode 
		VIFDynamicDMA.EndDirect();
	}

}

void CBlock::checkCollision( CBlock *checkWith, int &type )
{

	// get dimensions of ball
	float v[4];
	checkWith->getDims( &v[0], &v[1], &v[2], &v[3] );

	// get size of ball
	float sx, sy;
	checkWith->getSize(&sx,&sy);
	// get position of ball
	float bx, by;
	checkWith->getPos(&bx,&by);

	// Check ball dimensions against block dimensions

	// check NON-COLLISION flag FIRST, for efficiency
	if( (bx < m_boundingBox[0] -sx/2) && (by < m_boundingBox[1]-sy/2) 
		&& (bx > m_boundingBox[2] +sx/2) && (by > m_boundingBox[3]+sy/2) ) 
		type = -1;

	// type 0 - side collision
	else if( (v[2] > m_boundingBox[0]) && (v[1] > m_boundingBox[1]) 
		&& (v[2] < m_boundingBox[2]) && (v[1] < m_boundingBox[3])
		&& (v[2] > m_boundingBox[0]) && (v[3] > m_boundingBox[1]) 
		&& (v[2] < m_boundingBox[2]) && (v[3] < m_boundingBox[3]) ) 
		type = 0;

	else if( (v[0] > m_boundingBox[0]) && (v[3] > m_boundingBox[1]) 
		&& (v[0] < m_boundingBox[2]) && (v[3] < m_boundingBox[3])
		&& (v[0] > m_boundingBox[0]) && (v[1] > m_boundingBox[1]) 
		&& (v[0] < m_boundingBox[2]) && (v[1] < m_boundingBox[3]) ) 
		type = 0;

	// type 1 - top or bottom collision
	else if( (v[2] > m_boundingBox[0]) && (v[1] > m_boundingBox[1]) 
		&& (v[2] < m_boundingBox[2]) && (v[1] < m_boundingBox[3])
		&& (v[0] > m_boundingBox[0]) && (v[1] > m_boundingBox[1]) 
		&& (v[0] < m_boundingBox[2]) && (v[1] < m_boundingBox[3]) ) 
		type = 1;

	else if( (v[0] > m_boundingBox[0]) && (v[3] > m_boundingBox[1]) 
		&& (v[0] < m_boundingBox[2]) && (v[3] < m_boundingBox[3])
		&& (v[2] > m_boundingBox[0]) && (v[3] > m_boundingBox[1]) 
		&& (v[2] < m_boundingBox[2]) && (v[3] < m_boundingBox[3]) ) 
		type = 1;

	// all else - corner collisions
	else if( (v[0] > m_boundingBox[0]) && (v[1] > m_boundingBox[1]) 
		&& (v[0] < m_boundingBox[2]) && (v[1] < m_boundingBox[3]) ) 
		type = 2;
	else if( (v[0] > m_boundingBox[0]) && (v[3] > m_boundingBox[1]) 
		&& (v[0] < m_boundingBox[2]) && (v[3] < m_boundingBox[3]) ) 
		type = 3;
	else if( (v[2] > m_boundingBox[0]) && (v[3] > m_boundingBox[1]) 
		&& (v[2] < m_boundingBox[2]) && (v[3] < m_boundingBox[3]) ) 
		type = 4;
	else if( (v[2] > m_boundingBox[0]) && (v[1] > m_boundingBox[1]) 
		&& (v[2] < m_boundingBox[2]) && (v[1] < m_boundingBox[3]) ) 
		type = 5;
	else
		// if none of these set a collision type, something strange
		// has happened, but set non-collision anyway!
		type = -1;
}


int CBlock::getXpos( int type )
{
	switch( type )
	{
	case 0:
		return (int)m_x;
	case 1:
		return (int)m_newx;
	}
}

int CBlock::getYpos( int type )
{
	switch( type )
	{
	case 0:
		return (int)m_y;
	case 1:
		return (int)m_newy;
	}
}


int CBlock::incrementZrot( int value )
{
	m_zrot += value;
}

void CBlock::setZrot(float value)
{
	m_zrot = (int) value;
}

int CBlock::getZrot()
{
	return m_zrot;
}

void CBlock::setSize( int newsize )
{
	m_width = newsize;
}

void CBlock::getDims( float *v0, float *v1, float *v2, float *v3 )
{
	*v0 = m_boundingBox[0];
	*v1 = m_boundingBox[1];
	*v2 = m_boundingBox[2];
	*v3 = m_boundingBox[3];
}

void CBlock::getPos(float *x, float *y)
{
	*x = m_x;
	*y = m_y;
}

void CBlock::getSize(float *x, float *y)
{
	*x = m_width;
	*y = m_height;
}

void CBlock::getDiff( int *x, int *y )
{
	*x = m_diffx;
	*y = m_diffy;
}

void CBlock::setPos( int x, int y )
{
	m_x = x;
	m_y = y;
}

void CBlock::launchBall()
{
	m_goal = false;
}

void CBlock::setLife( int lives )
{
	m_life = lives;
}

void CBlock::setMega( bool to )
{
	m_megaball = to;
}

void CBlock::setNewBall( bool to )
{
	m_newball = to;
}

void CBlock::setSticky( bool to )
{
	m_sticky = to;
}

void CBlock::setBatMode( int to )
{
	m_bat_type = to;
}

void CBlock::resetBall()
{
	m_goal = true;
}

void CBlock::kill()
{
	m_alive = false;
}

void CBlock::spawn()
{
	m_alive = true;
}

void CBlock::activate()
{
	m_active = true;
}