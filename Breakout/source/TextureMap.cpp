// Acknowledgement
// Original code is by Dr. Henry Fortuna
// http://www.hsfortuna.pwp.blueyonder.co.uk/
// -------------------------------------------

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

#define ANIMRATE 4
#define MAX_PACKETS 10

bool g_bLoop = true;
int animCounter = 0;
int animFrame = 0;

class CBlock {

public:

	CBlock();
	void initBat(int x,int y, int DMALocation);
	void initBlock(int x,int y, int DMALocation);
	void updatePos();
	void draw();
	int getDMA(){return m_DMAPos;};
	bool checkCollision(int x, int y);
	bool isAlive(){return m_alive;};
	void kill();
	~CBlock();

private:

	int m_x, m_y;
	int m_height,m_width;
	int m_DMAPos;
	int m_boundingBox[4];
	int m_vertexArray[4][2];
	bool m_alive;
};

CBlock::CBlock(){};
CBlock::~CBlock(){};

void CBlock::initBat(int x, int y, int DMALocation){

	m_x = x, m_y = y;
	m_DMAPos = DMALocation;

	m_width = 40;
	m_height = 15;

	m_alive = true;

	m_boundingBox[0] = m_x;
	m_boundingBox[1] = m_y;
	m_boundingBox[2] = m_x + m_width;
	m_boundingBox[3] = m_y + m_height;

	m_vertexArray[0][0] = -50;
	m_vertexArray[0][1] = -50;

	m_vertexArray[1][0] = -50;
	m_vertexArray[1][1] =  50;

	m_vertexArray[2][0] =  50;
	m_vertexArray[2][1] =  50;

	m_vertexArray[3][0] =  50;
	m_vertexArray[3][1] = -50;

}

void CBlock::draw(){

		// begin geometry stuff
		VIFStaticDMA.StartDirect();
		// create giftag
		VIFStaticDMA.Add128(GS_GIFTAG_BATCH(	4,											//NLOOP
 												1,											//EOP
 												1,											//PRE
 												11,											//PRIM
 												GIF_FLG_PACKED,								//FLG
 												GS_BATCH_2(GIF_REG_RGBAQ, GIF_REG_XYZ2)));	//BATCH					

		for ( int i = 0; i < 4; i++ )
		{
			// colour data
			VIFStaticDMA.Add128(PACKED_RGBA(	255,
    											0,
    											0,
    											0x80));
    				
			// vertex data  	   	
			VIFStaticDMA.Add128(PACKED_XYZ2(	(2048 + m_vertexArray[i][0])<< 4, 
    											(2048 + m_vertexArray[i][1])<< 4, 
    											100, 
    											0));
		}
    	  		
		
		// end geometry batch
		VIFStaticDMA.EndDirect();
		// end DMA chain
		VIFStaticDMA.DMARet();
}




int main()
{
	// Initialise control pad 0
	if(!pad_init(PAD_0, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad\n");
		pad_cleanup(PAD_0);
		exit(0);
	}	

	SPS2Manager.Initialise(4096);	// 4096 * 4K Pages = 16MB Total

	VIFStaticDMA.Initialise(1536);	// 1536 * 4K Pages = 6MB Static DMA
	VIFDynamicDMA.Initialise(256);	// 256 * 4K Pages * 2 Buffers =
									// 2MB Dynamic DMA
	
	// Set up the DMA packet to clear the screen.
	SPS2Manager.InitScreenClear(0x20, 0, 0x20);	

	int packet[MAX_PACKETS];

	//init classes
	CBlock *bat;
	bat = new CBlock();
	bat->initBat(0,0,0);

	while(g_bLoop)
	{
		packet[bat->getDMA()] = VIFStaticDMA.GetPointer();
		bat->draw();

		// Send buffer data to VIF
		VIFDynamicDMA.Fire();

		// get pad input
		pad_update(PAD_0);

		// draw scene
		SPS2Manager.BeginScene();
		
		// call geometry address to draw scene
		VIFDynamicDMA.DMACall(packet[0]);

		// Swap the buffers
		SPS2Manager.EndScene();
		
		// Check for exit condition
		if((pad[0].buttons & PAD_START)&&(pad[0].buttons & PAD_SELECT)) g_bLoop = false;

		animCounter ++;
		if (animCounter > ANIMRATE )
		{
			animFrame += 32;
			if(animFrame > 255) animFrame = 0;
			animCounter = 0;
		}
	}
	
	// Cleanup the Pads
	pad_cleanup(PAD_0);

	return 0;
}

