// A nice manager to wrap up all the SPS2 memory handling etc. into a nice easy to use
// class.
#include "sps2wrap.h"
#include "PS2Defines.h"
#include "dma.h"

// We have to create this to use the singleton. (it calls the constructor
// which sets up the ms_Singleton member).
CSPS2Manager SPS2Manager_SingletonInit;

static int iScreenClear = 0;

// Constructor
CSPS2Manager::CSPS2Manager()
{
	// The array to keep track of how much memory is allocated in each page
	m_iaFreeMemInPage = 0;
}

// Destructor
CSPS2Manager::~CSPS2Manager()
{
	sps2UScreenShutdown();
	sps2Release(m_iSPS2Desc);

	// Free up all of the memory that was allocated in Initialise
	if(m_iaFreeMemInPage)
	{
		delete [] m_iaFreeMemInPage;
		m_iaFreeMemInPage = 0;
	}
}

// Set up SPS2, and allocate all of the SPS2 memory that will be
// used
void CSPS2Manager::Initialise(const int iAllocPages)
{
	// Keep track of how many 4K pages will be allocated
	m_iNumPages = iAllocPages;

	// Initialise SPS2
	m_iSPS2Desc = sps2Init();

	if(m_iSPS2Desc < 0)
	{
		fprintf(stderr, "Failed to initialise SPS2 library." \
				"Please check that the module is loaded.\n");
		exit(-1);
	}

	// Open the vector units
	int iVPU0; iVPU0 = open("/dev/ps2vpu0", O_RDWR);
	int iVPU1; iVPU1 = open("/dev/ps2vpu1", O_RDWR);

	// Allocate the memory that we will be using, and get the cached and
	// cached pointers.
	m_pMemCached = sps2Allocate(iAllocPages * 4096,
			SPS2_MAP_BLOCK_4K | SPS2_MAP_CACHED, m_iSPS2Desc);
	m_pMemUncached = sps2Remap(m_pMemCached, SPS2_MAP_UNCACHED, m_iSPS2Desc);

	// Set up an array to keep track of how much memory is free in each page
	m_iaFreeMemInPage = new int[m_iNumPages];

	for(int iPage = 0; iPage < m_iNumPages; iPage++)
	{
		m_iaFreeMemInPage[iPage] = 256;	// 4K == 256 Quad words
	}

	// Set up the screen
	sps2UScreenInit(0);
}

// Allocate some of the memory that we created in Initialise.
void CSPS2Manager::Allocate(CDMAMem & Memory, int iQuadWords)
{
	assert(iQuadWords <= 256 && "A CDMAMem class can only handle a maximum of 256 quadwords");

	for(int iPage = 0; iPage < m_iNumPages; iPage++)
	{
		// Find the first page with enough free memory in it.
		if(m_iaFreeMemInPage[iPage] >= iQuadWords)
		{
			// Set up the CDMAMem structure.
			// Get the first free quad word in the page.
			int FirstQW = 256 - m_iaFreeMemInPage[iPage];
			m_iaFreeMemInPage[iPage] -= iQuadWords;
			// Get the pointers.
			void * pCached = &((char *)m_pMemCached->pvStart)[iPage * 4096 + (FirstQW * 16)];
			void * pUncached = &((char *)m_pMemUncached->pvStart)[iPage * 4096 + (FirstQW * 16)];
			// Set the CDMAMem's member variables
			Memory.Set(iQuadWords, pCached, pUncached, sps2GetPhysicalAddress(pCached, m_pMemCached));
			return;	// No need to search anymore
		}
	}

	assert(!"Not enough free memory!");
}

// Initialise the screen clear. With this class we clear the screen much faster than SPS2 would
// usually. This puts all of the screen clear DMA data into our static DMA buffer. This means it is
// only ever added once (very fast), and is then simply CALLed from the dynamic DMA buffer.
// Notice that instead of just using one big sprite to clear the screen we use 20 long tall strips.
// This is because the GS is optimised for polygons less than or equal to 32 pixels wide, and it
// "chokes" on ones that are larger.
void CSPS2Manager::InitScreenClear(int R, int G, int B)
{
	int x0 = (2048 - (sps2UScreenGetWidth() >> 1)) << 4;
	int y0 = (2048 - (sps2UScreenGetHeight() >> 1)) << 4;
	int x1 = (2048 + (sps2UScreenGetWidth() >> 1)) << 4;
	int y1 = (2048 + (sps2UScreenGetHeight() >> 1)) << 4;

	// Get the address of the screen clear packet so we can CALL it
	// from the dynamic packet.
	iScreenClear = VIFStaticDMA.GetPointer();

	// Start the VIF direct mode.
	VIFStaticDMA.StartDirect();

	VIFStaticDMA.Add128(GS_GIFTAG_BATCH(4 + (20 * 2), 1, 0, 0, 
						GIF_FLG_PACKED, GS_BATCH_1(GIF_REG_A_D)));

	VIFStaticDMA.Add64(TEST_SET(0, 0, 0, 0, 0, 0, 1, 1));
	VIFStaticDMA.Add64(TEST_1);

	VIFStaticDMA.Add64(PRIM_SET(0x6, 0, 0, 0, 0, 0, 0, 0, 0));
	VIFStaticDMA.Add64(PRIM);

	VIFStaticDMA.Add64(RGBAQ_SET(R, G, B, 0x80, 0x3f800000));
	VIFStaticDMA.Add64(RGBAQ);

	for(int i = 0; i < 20; i++)
	{
		VIFStaticDMA.Add64(XYZ2_SET(x0, y0, 0));
		VIFStaticDMA.Add64(XYZ2);

		VIFStaticDMA.Add64(XYZ2_SET(x0 + (32 << 4), y1, 0));
		VIFStaticDMA.Add64(XYZ2);

		x0 += (32 << 4);
	}

	VIFStaticDMA.Add64(TEST_SET(0, 0, 0, 0, 0, 0, 1, 3));
	VIFStaticDMA.Add64(TEST_1);

	VIFStaticDMA.EndDirect();

	VIFStaticDMA.DMARet();
}

// This just DMACall's the screen clear packet that was set up in InitScreenClear
// (I'm sure you can see why this is so much faster than the sps2 screen clear, as the
//  CPU hardly has to do any work at all here).
void CSPS2Manager::BeginScene()
{
	VIFDynamicDMA.DMACall(iScreenClear);
}
