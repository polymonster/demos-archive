
/*	=========================================================
	=== PS2 BREAKOUT == by A BLEASDALE & A DIXON === V1.0 ===
	=========================================================
	=== -using- base wrapper code by Dr. Henry Fortuna ======
	=== http://www.hsfortuna.pwp.blueyonder.co.uk/ ==========
	=========================================================
*/

// ---- Includes ----

// Standard
#include <time.h>
#include <stdlib.h>
#include <assert.h>

// Linux
#include <memory.h>
#include <signal.h>

// SPS2 Includes
#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>

// Framework Wrappers
#include "PS2Defines.h"
#include "ps2maths.h"
#include "ps2matrix4x4.h"
#include "pad.h"
#include "sps2wrap.h"
#include "dma.h"
#include "texture.h"
#include "font.h"
#include "audio.h"

// Breakout
#include "block.h"

// constants
#define ANIMRATE	4

// brick texture coordinates
#define BRICKCOORD0 0
#define BRICKCOORD1 32
#define BRICKCOORD2 64
#define BRICKCOORD3 96
#define BRICKCOORD4 128
#define BRICKCOORD5 160
#define BRICKCOORD6 192

#define ALIVE		1
#define DEAD		0

#define MAXBALLS	3

// number of brick rows and columns
#define XBRICKS		14
#define YBRICKS		8

// These two external function pointers are exported by the VU code
// and we can now use them as pointers to the compiled VU data
extern "C" void VU_vu1code_start(void);
extern "C" void VU_vu1code_end(void);

// font class static call
CFont font;

// textures class static call
CTexture textures;
CTexture textures8;
CTexture fonttex;
CTexture gamestatestex;

// dynamic instance of local classes
CBlock *ball[MAXBALLS];
CBlock *bat;
CBlock *brick[ XBRICKS ][ YBRICKS ];
CBlock *powerup;
CBlock *bg[3];
CBlock *gamestates[3];

// global vars
float ballx, bally;

int numberofballs = 1;
int g_settoball = 0;
int g_inactive_slot = 1;

float batx, baty;

int g_brick_coord[ XBRICKS ][ YBRICKS ];
int g_type[MAXBALLS] = { -1, -1, -1 };

int g_lives = 3;
int g_score = 0;

int g_powerrand = 0;
int powerchance = 0;
int g_remaining = 1;
int g_statecounter = 0;

int one_upcount = 0;
bool one_up_disp = false;
bool one_up = false;

bool g_launch = false;

bool spawnpowerup = false;
bool g_newball = true;

bool g_noncollision = false;
bool g_gameloop = true;

// GLOBAL FUNCTIONS ------------------------------------------------------

// gameloop handler
void sig_handle( int sig )
{
	g_gameloop = false;
}

// pad functions
void controlPad(){

	pad_update(PAD_0);
	pad_update(PAD_1);
		
	// bat movement
	if(pad[0].buttons & PAD_LEFT) bat->updatePos( -5 );
	if(pad[0].buttons & PAD_RIGHT) bat->updatePos( 5 );
	bat->updatePos( pad[0].axes[0] *10.0f );

/*	if(pad[1].buttons & PAD_LEFT) bat[1]->updatePos( -5 );
	if(pad[1].buttons & PAD_RIGHT) bat[1]->updatePos( 5 );
	bat[1]->updatePos( pad[0].axes[0] *10.0f );
*/
	// zoom
//	if(pad[0].buttons & PAD_L1) zoomPos += 0.2f;
//	if(pad[0].buttons & PAD_R1) zoomPos -= 0.2f;

	if(pad[0].buttons & PAD_L2) ball[0]->setZrot( -1 );
	if(pad[0].buttons & PAD_R2) ball[0]->setZrot( 1 );

}

// load game font
void getFont()
{
	// load font size data
	if( !font.Load( "font.dat", false ) )
	{
		printf("can't load font information!" );
		g_gameloop = false;
	}

	printf("loading font tex file\n");

	// load font texture
	if( !fonttex.LoadBitmap( "font.bmp", false, true ) )
	{
		printf("can't load font.bmp!" );
		g_gameloop = false;
	}
	else
		fonttex.Upload( 480 );
}

// load game textures
void getTextures()
{
	printf("loading texture file\n");

	if( !textures.LoadBitmap( "textures24bit.bmp", true ) )
	{
		printf("can't load %c", "textures24bit.bmp" );
		g_gameloop = false;
	}
	else
		textures.Upload( 480 );

	printf("loading texture file\n");

	if( !textures8.LoadBitmap( "textures8bit.bmp", true ) )
	{
		printf("can't load %c", "textures8bit.bmp" );
		g_gameloop = false;
	}
	else
		textures8.Upload( 480 );

	if( !gamestatestex.LoadBitmap( "gamestates.bmp", true ) )
	{
		printf("can't load %c", "gamestates.bmp" );
		g_gameloop = false;
	}
	else
		gamestatestex.Upload( 480 );
}

// Set brick texture and life attributes
int setBrick( int number, int idx, int idy )
{
	switch( number )
	{
	case 0:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD0;
		break;
	case 1:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD1;
		break;
	case 2:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD2;
		break;
	case 3:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD3;
		break;
	case 4:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD4;
		break;
	case 5:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD5;
		break;
	case 6:
		g_brick_coord[ idx ][ idy ] = BRICKCOORD5;
		break;
	}

	// set brick lives
	if( g_brick_coord[ idx ][ idy ] == BRICKCOORD0 )
		brick[ idx ][ idy ]->setLife( 999999 );
	else if( g_brick_coord[ idx ][ idy ] == BRICKCOORD5 )
		brick[ idx ][ idy ]->setLife( 2 );
	else
		brick[ idx ][ idy ]->setLife( 1 );

	return g_brick_coord[ idx ][ idy ];
}

// MAIN ------------------------------------------------------------------

int main()
{
	// seed random function from time
	srand((unsigned)time(NULL));

	AudioDevice DSP0(0), DSP1(1);
	SoundSample powerUp("powerUp", &DSP1);
	SoundSample gameOver("gameOver", &DSP1);
	SoundSample hitBlock("hitBlock", &DSP0);
	SoundSample hitInvinc("hitInvinc", &DSP0);
	SoundSample win("win", &DSP1);
	SoundSample hitBat("hitBat", &DSP0);
	SoundSample loseLife("loseLife", &DSP1);
	SoundSample start("start", &DSP1);

	// INITIALISE OBJECTS ------------------

	bat = new CBlock();
//	bat[1] = new CBlock();
	powerup = new CBlock();
	
	for( int i = 0; i < 3; i++ )
	{
		ball[i] = new CBlock();
		bg[i] = new CBlock();
		gamestates[i] = new CBlock();
	}

	ball[0]->initBall( 0, 80, ALIVE );
	ball[1]->initBall( 20, 80, DEAD );
	ball[2]->initBall( 30, 80, DEAD );

	bat->initBat( 0, 200 );
//	bat[1]->initBat( 100, 200 );
//	bat[1]->kill();

	gamestates[0]->initBlock(-5,0, 80, 20 );
	gamestates[1]->initBlock(-5,0, 80, 20 );
	gamestates[2]->initBlock(-5,0, 80, 20 );

	// kills last 2 gamestates
	for( int i = 1; i < 3; i++ ) gamestates[i]->kill();

	// initialise backgrounds
	bg[0]->initBlock(-290,-245, 600, 15 );
	bg[1]->initBlock( 315,-230,  15, 450 );
	bg[2]->initBlock(-315,-230,  15, 450 );

	int blockx = -216, blocky = -180;

	// initialise and set positions for bricks
	for( int i = 0; i < YBRICKS; i++ )
	{
		for( int j = 0; j < XBRICKS; j++ )
		{
			brick[j][i] = new CBlock();
			brick[j][i]->initBlock( blockx, blocky );
			blockx += 32;
		}
		blocky += 16;
		blockx = -216;
	}
	// -------------------------------------

	// Initialise control pad 0
	if(!pad_init(PAD_0, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad\n");
		pad_cleanup(PAD_0);
		exit(0);
	}

	// Set up PS2 memory
	SPS2Manager.Initialise(4096);	// 4096 * 4K Pages = 16MB Total
	VIFStaticDMA.Initialise(1536);	// 1536 * 4K Pages = 6MB Static DMA
	VIFDynamicDMA.Initialise(256);	// 256 * 4K Pages * 2 Buffers = 2MB Dynamic DMA
	
	// Set up the DMA packet to clear the screen
	SPS2Manager.InitScreenClear(0x00, 0x00, 0x00);

	// get external game files
	getFont();
	getTextures();

	// Set texture coord for each object

	bat->texCoord( 0, 224, 128, 32 );

	for( int i = 0; i < 3; i++ )
	{
		ball[i]->texCoord( 64, 0, 24, 24 );
		bg[i]->texCoord( 10, 10, 30, 15 );
	}

	gamestates[0]->texCoord( 0, 0, 256, 33 );
	gamestates[1]->texCoord( 0, 33, 256, 33 );
	gamestates[2]->texCoord( 0, 66, 256, 33 );

	int blocktype = 0; // used for random(ish) level generation

	for( int i = 0; i < YBRICKS; i++ )
	{
		for( int j = 0; j < XBRICKS; j++ )
		{
			// set coords for all bricks
			brick[j][i]->texCoord(	0,
									setBrick( blocktype+rand()%2, j, i ),
									64,
									32	);
		}
		blocktype++;
		if( blocktype > 5 )
			blocktype = 0;
	}

	// Main Loop -------------------------------------------------------------
	while( g_gameloop )
	{
		if( gamestates[0]->isAlive() )
		{
			if( pad[0].buttons & PAD_CROSS )
			{
				if( gamestates[0]->isAlive() )
				{
					start.Play();
					gamestates[0]->kill();
				}
			}
		}

		if( g_remaining < 1 )
		{
			g_statecounter++;
			gamestates[2]->spawn();

			if( g_statecounter < 2 )
			{
				win.Play();
			}
			else if( g_statecounter > 300 )
			{
				gamestates[2]->kill();
				g_statecounter = 0;

				int blockx = -216, blocky = -180;
				// initialise and set positions for bricks
				for( int i = 0; i < YBRICKS; i++ )
				{
					for( int j = 0; j < XBRICKS; j++ )
					{
						brick[j][i] = new CBlock();
						brick[j][i]->initBlock( blockx, blocky );
						blockx += 32;
					}
					blocky += 16;
					blockx = -216;
				}

				int blocktype = 5; // used for random(ish) level generation

				for( int i = 0; i < YBRICKS; i++ )
				{
					for( int j = 0; j < XBRICKS; j++ )
					{
						// set coords for all bricks
						brick[j][i]->texCoord(	0,
												setBrick( blocktype+rand()%2, j, i ),
												64,
												32	);
					}
					blocktype--;
					if( blocktype < 0 )
						blocktype = 5;
				}

				g_remaining = 1;
			}
		}
		// set player lives ( determined by bat! )
		g_lives = bat->getLife();

		// get ball 0 position info ( debug dump )
		ball[0]->getPos( &ballx, &bally );
		int ballxloc = (int)( ballx + 240 ) / 30;
		int ballyloc = (int)( bally + 180 ) / 15;

		if( g_score > 1000 )
		{
			one_up = true;
			one_up_disp = true;
			g_score = 0;
		}

		if( one_up )
		{
			bat->setLife( bat->getLife() +1 );
			one_up = false;
		}

		// DRAWING FUNCTIONS =======================

		// Fire off the previous frame's buffer (or init data if it's the first time)
		VIFDynamicDMA.Fire();

		// Clear the screen
		SPS2Manager.BeginScene();

		gamestatestex.Upload( 480 );

		for( int i = 0; i < 3; i++ )
			gamestates[i]->draw( gamestatestex );

		textures8.Upload( 480 );

		for( int i = 0; i < 3; i++ )
			bg[i]->draw( textures8 );
		
		textures.Upload( 480 );

		for( int i = 0; i < MAXBALLS; i++ )
		{
			if( ball[i]->isAlive() )
				ball[i]->draw( textures );
		}

		powerup->draw( textures );

		bat->draw( textures );

		for( int i = 0; i < YBRICKS; i++ )
			for( int j = 0; j < XBRICKS; j++ )
				brick[j][i]->draw( textures );

		fonttex.Upload( 480 );
		fonttex.Select();

		font.printfC( -180, -230, 127, 127, 127, 127, "Lives: %i",g_lives );
		font.printfC( 100, -230, 127, 127, 127, 127, "Score: %i", g_score );
		font.printfC( 180, 205, 127, 127, 127, 127, "ball Y: %i", ballyloc );
		font.printfC( 180, 225, 127, 127, 127, 127, "ball X: %i", ballxloc );
		font.printfC( -180, 225, 127, 127, 127, 127, "Bricks left: %i", g_remaining );

		if( one_up_disp )
		{
			one_upcount ++;
			font.printfC( 0, 0, 127, 127, 127, 127, "1UP!" );
			if( one_upcount > 100 )
			{
				one_upcount = 0;
				one_up_disp = false;
			}
		}

		// Swap the buffers
		SPS2Manager.EndScene();
		// =========================================

		// Control Pad function call
		controlPad();
		// X launches balls
		if( pad[0].buttons & PAD_CROSS )
		{
			ball[0]->launchBall();
			ball[1]->launchBall();
			ball[2]->launchBall();
			g_launch = true;
		}
		else
			g_launch = false;

		if( ( pad[0].buttons & PAD_SQUARE ) && numberofballs < MAXBALLS )
			ball[g_inactive_slot]->spawn();


		// State update and Collisions

		// BALL - BAT AND POWERUP INTERACTION ========================

		// reset ball count and THEN...
		numberofballs = 0;
		// recount number of spawned balls, and SET...
		for( int i = 2; i >= 0; i-- )
		{
			if( ball[i]->isAlive() )
			{
				// increase for every ball thats alive to get active ball total
				numberofballs++;
				// flag the FIRST ball in the array that is ACTIVE as safe
				// to use for the spawn POSITION of a new ball
				g_settoball = i;
			}
			else
			{
				// flag the FIRST ball in the array that is INACTIVE as
				// safe to use for the spawn SLOT of a new ball
				g_inactive_slot = i;
			}
		}

		// if there are no balls left
		if( numberofballs < 1 )
		{
			loseLife.Play();
			// life lost
			bat->setLife( bat->getLife() -1 );
			// spawn and reset ball 0
			ball[0]->spawn();
			ball[0]->resetBall();
		}
	
		// get bad pos
		bat->getPos( &batx, &baty );

		// if a new ball has been asked for
		if( powerup->getNewBall() )
		{
			// if max balls reached
			if( numberofballs >= MAXBALLS )
			{
				// dont change anything
				numberofballs = MAXBALLS;
			}
			else
			{
				// increase number of balls
				numberofballs++;
				// spawn a new ball in an inactive slot, and set it's position to that of an active ball
				ball[g_inactive_slot]->spawn();
				ball[g_inactive_slot]->setPos( ball[g_settoball]->getXpos( 0 ), ball[g_settoball]->getYpos( 0 ) );
			}

			// set bat to it's correct location ( unexplained error handling )
			bat->setPos( batx, baty );
			
			// dont set a new ball again until flagged by the powerup instance
			powerup->setNewBall( false );
		}

		// for all balls
		for( int i = 0; i < MAXBALLS; i++ )
		{
			// if ball is active
			if( ball[i]->isAlive() )
			{
				// update ball position
				ball[i]->updateBall( bat, numberofballs, g_launch );

				// if bat collides with a ball
				if( bat->batCollision( ball[i] ) )
				{
					hitBat.Play();
					// set ball angle (direction) to new angle + bat curve differential
					ball[i]->setZrot( bat->batToBall( ball[i] ) );
				}
			}
		}

		// execute crazy bat function ( will only apply if crazybat set in class )
		bat->crazyBat();

		// update powerup position
		powerup->updatePowerPos();

		for( int i = 0; i < MAXBALLS; i++ )
		{
			// if ball is active
			if( ball[i]->isAlive() )
			{
				// if spawn powerup is true
				if( spawnpowerup )
				{
					// spawn a powerup at ball position and reset
					powerup->spawnPowerUp( g_powerrand, ball[i]->getXpos( 0 ), ball[i]->getYpos( 0 ) );
					spawnpowerup = false;
				}
			}
		}

		// if bat collides with powerup
		if( bat->batCollision( powerup ) )
		{
			if( powerup->isAlive() )
				powerUp.Play();

			// execute powerup function!
			powerup->execPowerUp( bat, ball[g_settoball] );
			// kill powerup
			powerup->kill();
		}
		// else if powerup drops below bat before collision, just kill
		else if ( powerup->getYpos( 0 ) > 235 )
			powerup->kill();

		// ==========================================================

		g_remaining = 0;
		// FOR ALL BRICKS
		for( int i = 0; i < YBRICKS; i++ )
		{
			for( int j = 0; j < XBRICKS; j++ )
			{
				if( g_brick_coord[j][i] != BRICKCOORD0 )
					g_remaining++;
				// if the block is still there
				if( brick[j][i]->isAlive() )
				{
					// for all balls
					for( int k = 0; k < MAXBALLS; k++ )
					{
						// if ball is active
						if( ball[k]->isAlive() )
						{
							// check for collision with ball, get collision type
							brick[j][i]->checkCollision( ball[k], g_type[k] );

							// if collision type is not negative ie a hit
							if( g_type[k] != -1 )
							{
								if( g_brick_coord[j][i] == BRICKCOORD0 )
									hitInvinc.Play();
								else
									hitBlock.Play();

								// generate random percentage
								powerchance = rand() % 100;

								// if top 5% and
								// brick type is not invulnerable and
								// powerup is not already spawned
								if( ( powerchance > 50 ) &&
									( g_brick_coord[j][i] != BRICKCOORD0 ) &&
									( !powerup->isAlive() ) )
								{
									// spawn a powerup!
									spawnpowerup = true;
									g_powerrand = rand()% 6;
								}

								// if brick is not type 0 and not type 6
								if( g_brick_coord[j][i] != BRICKCOORD5 &&
									g_brick_coord[j][i] != BRICKCOORD0 )
								{
									// ADD TO SCORE ----------
									g_score += 25;
									// -----------------------
								}

								// deflect ball when colliding with blocks
								ball[k]->deflectBall( g_type[k], g_brick_coord[j][i] );

								// collided brick loses a life point
								brick[j][i]->setLife( brick[j][i]->getLife() - 1 );

								// If block is type 5 and life is 1, then it's a hard block that has been hit
								if( g_brick_coord[j][i] = BRICKCOORD5 && brick[j][i]->getLife() == 1 )
								{
									// therefore, set to type 6, the broken block
									brick[j][i]->texCoord( 0, BRICKCOORD6, 64, 32 );
								}

								// If block life is less than 1
								if( brick[j][i]->getLife() < 1 )
								{
									// destory block
									brick[j][i]->kill();
								}
							}
						}
					}
				}
			}
		}

		// if player lives drops below zero
		if( g_lives < 0 )
		{
			g_statecounter++;
			gamestates[1]->spawn();

			if( g_statecounter < 2 )
				gameOver.Play();
			else if ( g_statecounter > 300 )
			{
				g_statecounter = 0;
				g_gameloop = false;
				gamestates[1]->kill();
			}
		//	g_gameloop = false;
		}
		// ------------------------------------------------
		
		DSP0.HandleAudio();
		DSP1.HandleAudio();

		// Check for exit condition
		if( (pad[0].buttons & PAD_START) && (pad[0].buttons & PAD_SELECT) ) g_gameloop = false;
	}

	// end loop ------------------------------------------------------------------
	
	// Cleanup the Pads
	pad_cleanup(PAD_0);

	return 0;
}

