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


// Define triangle vertices relative to centre of screen
int VertexData[][3] =
{
	{-200, -220, 100}, {-200,  -20, 100}, { -50, -120, 100}, // Triangle 1
	{ 200, -220, 100}, { 200,  -20, 100}, {  50, -120, 100}, // Triangle 2
	{-200,  220, 100}, {-200,   20, 100}, { -50,  120, 100}, // Triangle 3
	{ 200,  220, 100}, { 200,   20, 100}, {  50,  120, 100}, // Triangle 4
};

// Define vertex  colours
int VertexColors[][3] =
{
	{255,   0,   0}, {  0, 255,   0}, {  0,   0, 255},  // Triangle 1
	{255,   0,   0}, {  0, 255,   0}, {  0,   0, 255},  // Triangle 2
	{255,   0,   0}, {  0, 255,   0}, {  0,   0, 255},  // Triangle 3
	{255,   0,   0}, {  0, 255,   0}, {  0,   0, 255},  // Triangle 4
}; 

// The number of vertices and colours
int iVertexCount = sizeof(VertexData)/(3*sizeof(int));
int iColourCount = sizeof(VertexColors)/(3*sizeof(int));

int trinumber = 0;
int iStaticPacket[4];

bool g_bLoop = true;

void sig_handle(int sig)
{
	g_bLoop = false;
}


int main()
{
	// assert that these are the same
	assert((iVertexCount == iColourCount) && "Number of colours and vertices must be the same");
	
	// return error message if failed to initialise the pad
	if(!pad_init(PAD_0, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad\n");
		pad_cleanup(PAD_0);
		exit(0);
	}	

	// allocate pages of memory to be used
	SPS2Manager.Initialise(4096);

	// allocate memory
	VIFStaticDMA.Initialise(1536);

	// allocate memory
	VIFDynamicDMA.Initialise(256);

	// Register our signal function for every possible signal (i.e. ctrl + c)
	for(int sig=0; sig<128; sig++)
		signal(sig, sig_handle);

	// setup clear screen data
	SPS2Manager.InitScreenClear(0x20, 0, 0x20);
	
	
	// record current geometry address
	for ( int i = 0; i < 4; i++ )
	{
		iStaticPacket[i] = VIFStaticDMA.GetPointer();
	
		// begin geometry stuff
		VIFStaticDMA.StartDirect();
		
		// create giftag
		VIFStaticDMA.Add128(GS_GIFTAG_BATCH(	3,											//NLOOP
 												1,											//EOP
 												1,											//PRE
 												11,											//PRIM
 												GIF_FLG_PACKED,								//FLG
 												GS_BATCH_2(GIF_REG_RGBAQ, GIF_REG_XYZ2)));	//BATCH					
			 

		for ( int j = 0; j < 3; j++ )
		{
			// colour data
			VIFStaticDMA.Add128(PACKED_RGBA(	VertexColors[3*i + j][0],
    											VertexColors[3*i + j][1],
    											VertexColors[3*i + j][2],
    											0x80));
    				
			// vertex data  	   	
			VIFStaticDMA.Add128(PACKED_XYZ2(	(2048 + VertexData[3*i + j][0])<< 4, 
    											(2048 + VertexData[3*i + j][1])<< 4, 
    											VertexData[3*i + j][2], 
    											0));
		}
    	  		
		
		// end geometry batch
		VIFStaticDMA.EndDirect();
		
		// end DMA chain
		VIFStaticDMA.DMARet();
	}
	

	while(g_bLoop)
	{
		// Send buffer data to VIF
		VIFDynamicDMA.Fire();

		// get pad input
		pad_update(PAD_0);

		// draw scene
		SPS2Manager.BeginScene();
		
		// call geometry address to draw scene
		VIFDynamicDMA.DMACall(iStaticPacket[trinumber]);
				
		// swap buffers
		SPS2Manager.EndScene();
		
		
		// end game loop if start and select are held on the pad
		if((pad[0].buttons & PAD_START)&&(pad[0].buttons & PAD_SELECT)) g_bLoop = false;

		if(pad[0].buttons & PAD_CROSS)
			trinumber = 0;
		if(pad[0].buttons & PAD_SQUARE)
			trinumber = 1;
		if(pad[0].buttons & PAD_CIRCLE)
			trinumber = 2;
		if(pad[0].buttons & PAD_TRI)
			trinumber = 3;
	}
	
	// de-init pad
	pad_cleanup(PAD_0);
	
	return 0;
}

