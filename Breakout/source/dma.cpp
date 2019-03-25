#include "dma.h"

// Singleton initialisers (note: Never make new copies of these classes anywhere else)
CVIFStaticDMA VIFStaticDMA_SingletonInit;
CVIFDynamicDMA VIFDynamicDMA_SingletonInit;

// Function to print the packet via printf
void CDMAChain::PrintPacket()
{
	SPS2Manager.FlushCache();

	unsigned int * pMem = (unsigned int *)m_pPages[0].GetCached();
	
	printf("            ___________________________________________ \n");
	printf("  Address  |127     96|95      64|63      32|31       0| Virt Addr\n");
	printf(".----------+----------+----------+----------+----------|----------.\n");

	int iWritten = ((int)m_pPtr - (int)pMem + 0xF) >> 4;

	for(int i = 0; i < iWritten; i++)
	{
		static int iPhysAddr = 0;

		if((i % 256) == 0)
		{
			iPhysAddr = m_pPages[(i / 256)].GetPhysicalAddr();
		}

		printf("| %.8X | ", iPhysAddr + ((i % 256) << 4));
	
		for(int j = 0; j < 4; j++)
		{
			printf("%.8X | ", pMem[3 - j]);
		}

		printf("%.8X | ", (int)pMem);

		printf("\n");
		pMem += 4;
	}

	printf("'-----------------------------------------------------------------'\n");
}

// Stitches a DMA packet over the 4k boundary
void CDMAChain::Stitch()
{
	VIF1_STATE VifState = m_VifState;

	// If we are in direct mode we will have to end it, stitch, and then start it again
	// in the new page
	if(VifState == VIF_DIRECT)
	{
		EndDirect();
	}

	// Finish the previous DMA tag (i.e. set its QWC) and point it to the start of the next page
	*m_pDMATag = ((uint64)NewPage() << 32) + (2 << 28) + (m_pPtr - (int *)m_pDMATag) / 4 - 1;
	// And start a new one.
	NewTag();

	if(VifState == VIF_DIRECT)
		StartDirect();
}

// Allocate space in the chain for the DMA tag that will be added there once we know how many
// quad words it will be writing.
void CDMAChain::NewTag()
{
	m_pDMATag = (uint64 *)m_pPtr;
	Add64(0);
}

// Prepare for a new DMA tag
void CDMAChain::PrepForDMATag()
{
	// Make sure we aren't in DIRECT mode
	assert(m_VifState == VIF_BASE);
	// Make sure we are aligned on a quadword boundary.
	Align(4);

	// We can't add a new tag at the very end of a packet, so
	// lets just add a NOP and let the stitching handle the new tag.
	if(m_pPtr == m_pEndPtr)
		Add64(0);
}

// Fire() will call this for you, so never call this yourself unless you know what you are doing.
void CDMAChain::DMAEnd()
{	
	// This function sets the previous open DMA tag to an end tag so that the DMA chain ends.

	assert(m_VifState == VIF_BASE);
	// Pad if we are trying to transfer less than a whole number of quadwords
	Align(4);

	// And finish the open DMA tag.
	*m_pDMATag = (7 << 28) + (m_pPtr - (int *)m_pDMATag) / 4 - 1;
}

// Call DMARet when you want to return from a DMA chain in the static buffer
void CDMAChain::DMARet()
{
	// Prepare for a new tag.
	PrepForDMATag();
	// Put the ret tag in as the most recent open DMA tag.
	*m_pDMATag = (6 << 28) + (m_pPtr - (int *)m_pDMATag) / 4 - 1;
	// And create a new tag.
	NewTag();
}

// The function will cause the DMAC to branch to the address at iAddr and run until it hits a DMARet tag,
// when it will return control back to the tag after this one.
void CDMAChain::DMACall(uint32 iAddr)
{
	PrepForDMATag();
	*m_pDMATag = ((uint64)iAddr << 32) + (5 << 28) + (m_pPtr - (int *)m_pDMATag) / 4 - 1;
	NewTag();
}

// Start Direct (GS Path 2) mode.
void CDMAChain::StartDirect()
{
	assert(m_VifState == VIF_BASE);
	// Remember that the DIRECT vif code has to go into the very last (i.e. 3rd) word of a quadword.
	Align(4, 3);
	// Set the vif state so that the stitching knows if it has to stop and start direct mode.
	m_VifState = VIF_DIRECT;
	m_pVIFCode = m_pPtr;
	// Add the space for the DIRECT vif code (it won't be written until EndDirect() is called and we
	// know how many quad words were written)
	Add32(0);
}

// End Direct (GS Path 2) mode
void CDMAChain::EndDirect()
{
	assert(m_VifState == VIF_DIRECT);
	// We can only transfer an integer number of quadwords, so pad with NOPs if we don't haven't got that much.
	Align(4);
	int iSize = (m_pPtr - m_pVIFCode - 1) / 4;
	if(iSize != 0)	// If the size isn't 0, set write the DIRECT vifcode.
		*m_pVIFCode = (0x50 << 24) + iSize;
	m_VifState = VIF_BASE;
}

// Start an A+D mode segment (you must be DIRECT mode to use this)
void CDMAChain::StartAD()
{
	assert(m_VifState == VIF_DIRECT);

	if(m_pPtr >= m_pEndPtr)
	{
		EndDirect();
		StartDirect();
	}

	// Store the location so we can add the A+D amount later.
	m_pADGiftag = (uint64 *)m_pPtr;
	// Add the A+D giftag
	Add64((1 << 15) | (1ULL << 60));
	Add64(0xE);
}

// Add an A+D mode piece, iData is the data to add, iAddr is the register number to send it to
void CDMAChain::AddAD(uint64 iData, uint64 iAddr)
{
	Add64(iData);
	Add64(iAddr);
	// Increment the NLOOP in the giftag
	(*m_pADGiftag)++;
}

// Start uploading MPG code to VU1_MEM address iAddr
void CDMAChain::StartMPG(uint32 iAddr)
{
	assert(m_VifState == VIF_BASE);
	// MPG vifcode must be in either word 1 or 3 of a quadword
	Align(2, 1);
	m_pVIFCode = m_pPtr;
	// Add the VIF code
	Add32((0x4a << 24) + iAddr);
	m_iMPGCount = 0;
	m_iMPGAddr = iAddr;
}

// Call this once you have finished adding the microcode chunks
void CDMAChain::EndMPG()
{
	// Write the number of doubleword chunks of VU code that were written, to the MPG vifcode.
	*m_pVIFCode += (m_iMPGCount & 0xFF) << 16;
}

// Add a 64bit microcode instruction to the MPG chain
void CDMAChain::AddMPG(uint64 iInst)
{
	// We are only allowed to transfer 256 microcode instructions at once, so if we are trying to send
	// more we will need to use a new MPG vifcode.
	if(m_iMPGCount >= 256)
	{
		EndMPG();
		StartMPG(m_iMPGAddr + m_iMPGCount);
	}

	// Add the instruction and increment the microcode count.
	Add64(iInst);
	m_iMPGCount++;
}

// Make the current write pointer aligned to a certain multiple of words with an offset, by padding
// with NOPs if it isn't.
void CDMAChain::Align(int iAlign, int iOffset = 0)
{
	int p = (((int)m_pPtr >> 2) - iOffset) & (iAlign - 1);
	if(p == 0)
		return;
	for(p = iAlign - p; p > 0; p--)
		Add32(0);
}

// Add a floating point vector to the chain
void CDMAChain::AddVectorF(float x, float y, float z, float w)
{
	AddFloat(x);	
	AddFloat(y);
	AddFloat(z);
	AddFloat(w);
}

// Add an integer vector to the chain
void CDMAChain::AddVectorI(int x, int y, int z, int w)
{
	Add32(x);
	Add32(y);
	Add32(z);
	Add32(w);
}

// Add a 4x4 matrix to the chain
void CDMAChain::AddMatrix(const Matrix4x4 & Matrix)
{
	AddFloat(Matrix(0, 0));
	AddFloat(Matrix(0, 1));
	AddFloat(Matrix(0, 2));
	AddFloat(Matrix(0, 3));

	AddFloat(Matrix(1, 0));
	AddFloat(Matrix(1, 1));
	AddFloat(Matrix(1, 2));
	AddFloat(Matrix(1, 3));

	AddFloat(Matrix(2, 0));
	AddFloat(Matrix(2, 1));
	AddFloat(Matrix(2, 2));
	AddFloat(Matrix(2, 3));

	AddFloat(Matrix(3, 0));
	AddFloat(Matrix(3, 1));
	AddFloat(Matrix(3, 2));
	AddFloat(Matrix(3, 3));
}

// Begin a whole new chain
void CDMAChain::Begin()
{
	m_VifState = VIF_BASE;
	m_iPhysAddr = NewPage();
	NewTag(); // Add the first tag of the chain
}

CVIFDynamicDMA::CVIFDynamicDMA()
{
	m_iCurrPage = 0;
	m_iCurrBuffer = 0;
}

CVIFDynamicDMA::~CVIFDynamicDMA()
{

}

void CVIFDynamicDMA::Initialise(int iNumPages)
{
	m_iNumPages = iNumPages;

	// We are using a double buffer so allocate 2x the memory
	m_pPages = new CDMAMem[m_iNumPages * 2];

	for(int iPage = 0; iPage < (m_iNumPages * 2); iPage++)
		SPS2Manager.Allocate(m_pPages[iPage], 256);

	*EE_VIF1_ERR = 2; // Ignore DMA mismatch error

	Begin();
}

// Move onto a new 4K page
int CVIFDynamicDMA::NewPage()
{
	assert(m_iCurrPage < m_iNumPages);

	m_pPtr = (int *)m_pPages[m_iCurrPage + (m_iCurrBuffer * m_iNumPages)].GetCached();
	m_pEndPtr = m_pPtr + 1024;
	int iPhysAddr = (int)m_pPages[m_iCurrPage + (m_iCurrBuffer * m_iNumPages)].GetPhysicalAddr();
	++m_iCurrPage;

	return iPhysAddr;
}

// Print the current VIF packet
void CVIFDynamicDMA::PrintPacket()
{
	SPS2Manager.FlushCache();

	unsigned int * pMem = (unsigned int *)m_pPages[(m_iCurrBuffer * m_iNumPages)].GetCached();
	
	printf("            ___________________________________________            \n");
	printf("  Address  |127     96|95      64|63      32|31       0| Virt Addr \n");
	printf(".----------+----------+----------+----------+----------|----------.\n");

	int iWritten = ((int)m_pPtr - (int)pMem + 0xF) >> 4;

	for(int i = 0; i < iWritten; i++)
	{
		static int iPhysAddr = 0;

		if((i % 256) == 0)
		{
			iPhysAddr = m_pPages[(i / 256) + (m_iCurrBuffer * m_iNumPages)].GetPhysicalAddr();
		}

		printf("| %.8X | ", iPhysAddr + ((i % 256) << 4));
	
		for(int j = 0; j < 4; j++)
		{
			printf("%.8X | ", pMem[3 - j]);
		}

		printf("%.8X | ", (int)pMem);

		printf("\n");
		pMem += 4;
	}

	printf("'-----------------------------------------------------------------'\n");
}

void CVIFDynamicDMA::Fire(bool bFire = true, bool bPrint = false)
{
	// End the chain before we send it.
	DMAEnd();

	if(bPrint)
		PrintPacket();

	asm("sync.l");
	// Flush all the data through to memory.
	SPS2Manager.FlushCache();
	
	// Wait for channel 1 to finish the previous packet
	while(*EE_D1_CHCR & 256);
	*EE_D1_QWC = 0;
	*EE_D1_TADR = m_iPhysAddr & 0x8ffffff0;

	// Send the packet!
	if(bFire)
		*EE_D1_CHCR = 0x145;

	asm("sync.l");
	
	// Make it so that now we write to the beginning of the other buffer.
	m_iCurrBuffer ^= 1;
	m_iCurrPage = 0;

	// Start the packet
	Begin();
}

CVIFStaticDMA::CVIFStaticDMA()
{
	m_iCurrPage = 0;
}

CVIFStaticDMA::~CVIFStaticDMA()
{

}

void CVIFStaticDMA::Initialise(int iNumPages)
{
	m_iNumPages = iNumPages;

	m_pPages = new CDMAMem[iNumPages];

	for(int iPage = 0; iPage < iNumPages; iPage++)
		SPS2Manager.Allocate(m_pPages[iPage], 256);

	*EE_VIF1_ERR = 2; // Ignore DMA mismatch error

	Begin();
}

int CVIFStaticDMA::NewPage()
{
	assert(m_iCurrPage < m_iNumPages);

	m_pPtr = (int *)m_pPages[m_iCurrPage].GetCached();

	m_pEndPtr = m_pPtr + 1024;
	int iPhysAddr = (int)m_pPages[m_iCurrPage++].GetPhysicalAddr();	
	m_iPhysicalOffset = iPhysAddr - (int)m_pPtr;

	return iPhysAddr;
}