// A DMA class. This should prove extremely useful, and frees you from the
// irritating task of having to count quad word counts which should mean your
// crash count goes down a lot.

// There are two types of DMA chains here. A static DMA chain, and a dynamic one.
// The static one has data added to it at initialisation time and is never changed 
// again. The dynamic one is built every frame and is double buffered so that the
// next frame's DMA packet can be built while the current frame's one is being
// processed. This means that we no longer need to send the packet, and wait right away.
// We can instead send the packet, build the new one, and then wait.
// This is exploiting parallelism and will make everything much faster, especially
// when you start doing non-trivial things. A key to speed on the PS2 is to only add
// what you need per frame, so any data that won't change per frame should go into
// the static DMA chain, and then be called from the dynamic chain using a DMA call tag.

#ifndef __DMA_H__
#define __DMA_H__

#include "sps2wrap.h"
#include "singleton.h"
#include "ps2matrix4x4.h"
#include "ps2vector4.h"
#include "PS2Defines.h"

// These macros allow you to treat VIFStaticDMA and
// VIFDynamicDMA as if they were global variables. Much easier!
#define VIFStaticDMA CVIFStaticDMA::GetSingleton()
#define VIFDynamicDMA CVIFDynamicDMA::GetSingleton()

// A base class for the static and dynamic chains. This class is never
// used alone, it just holds all of the behaviour that is common to both the other classes.
class CDMAChain
{
	public:
		// Constructor
		CDMAChain()
		{
			// Zero the array that will hold all of the SPS2 memory pages
			m_pPages = 0;
		}

		// Destructor
		virtual ~CDMAChain()
		{
			// Delete the array that holds the SPS2 memory pages (note we aren't
			// freeing up the SPS2 memory, just this array)
			if(m_pPages)
			{
				delete [] m_pPages;
				m_pPages = 0;
			}
		}

		void Begin();				// Starts a new DMA chain

		// This is the most important function of the whole class. It adds
		// 32bits (an int worth) of data to the chain, and handles stitching
		// automatically. All of the other add-data functions use this.
		void Add32(uint32 data)
		{
			if(m_pPtr >= m_pEndPtr)	// Would we be writing to the new page?
			{
				Stitch();			// If so then stitch
			}

			*m_pPtr++ = data;		// Write the data and then move the pointer on
		}

		// These are simply to make life easier, as you can see this function just calls
		// Add32 twice.
		void Add64(uint64 data)
		{ Add32((uint32)data);	Add32((uint32)(data >> 32)); }

		void Add128(uint128 data)
		{ Add64((uint64)data);	Add64((uint64)(data >> 64)); }

		void AddFloat(float data)
		{ Add32(*(uint32 *)&data); }

		void AddVectorI(int x, int y, int z, int w);
		void AddVectorF(float x, float y, float z, float w);

		void AddVector(const Vector4 & Vector)
		{
			AddFloat(Vector.x);
			AddFloat(Vector.y);
			AddFloat(Vector.z);
			AddFloat(Vector.w);
		}

		void AddMatrix(const Matrix4x4 & Matrix);

		void Align(int iAlign, int iOffset = 0);	// Aligns the write pointer to a mulitple of iAlign * 32 bits
									// The value can be offset by iOffset * 32 bits

		void PrepForDMATag();		// Makes sure that we are writing a multiple of 1 quadword (pad with No Operations
									// if we aren't) and also makes sure that the next tag isn't on a stitching boundary

		// Functions to add different DMA Tags (note that the class handles all of the QWC stuff, so you
		// don't need to worry!)
		void DMAEnd();
		// Use this function at the end of any static blocks to tell the DMAC to return to the dynamic buffer just
		// after the DMACall tag.
		void DMARet();
		// Use this to call little chunks inside the static DMA buffer. Use the physical address
		// returned by CVIFStaticDMA::GetPointer()
		void DMACall(uint32 iAddr);

		// Start a section of data that should be sent directly to the GS (i.e. GIF path 2)
		// These functions can go over a stitching boundary, so there is no need to worry about that.
		// They also handle calculating the QWC for the DIRECT VIFCode and will automatically align
		// to the last word of a quad if needed too!
		void StartDirect();
		void EndDirect();

		// Use these functions to add data that will used in A+D mode, and these functions
		// will calculate the QWC for you. (You must be in DIRECT mode to use these)
		void StartAD();
		void EndAD(){};
		void AddAD(uint64 iData, uint64 iAddr);

		// Use these functions to transfer microcode across to VU1 via the DMAC
		void StartMPG(uint32 iAddr);
		void EndMPG();
		void AddMPG(uint64 iInst);

		// Add a VIF unpack code. You must know the number of quadwords you are adding for this function.
		void AddUnpack(int format, int addr, int num, int usetops = 0, int nosign = 1, int masking = 0)
		{
			Add32((0x60 << 24) + (format << 24) + (masking << 28) + (usetops << 15) +
				(nosign << 14) + (num << 16) + addr);
		}

		// A nice function for debugging that will print the contents of the packet
		// out via printf
		virtual void PrintPacket();
		
	protected:
		virtual int NewPage() = 0;	// Starts writing to a new memory page
		virtual void Stitch();		// Stitches across the page boundary with a next tag
		virtual void NewTag();		// Reserves space in the chain for a DMA tag
		
		int m_iNumPages;			// How many pages we have allocated for DMAing
		CDMAMem * m_pPages;			// All the pages that we have allocated

		int * m_pPtr;				// The data write pointer
		int * m_pEndPtr;			// The next page starts here
		int m_iPhysAddr;			// The physical address of the first page (for TADR)

		uint64 * m_pDMATag;			// The currently open DMA tag
		int * m_pVIFCode;			// The currently open VIF code

		int m_iMPGCount;			// How many MPG instructions we have written
		int m_iMPGAddr;				// The VU_MICRO_MEM address we are writing to

		uint64 * m_pADGiftag;		// The currently open AD GIFtag.
		
		typedef enum
		{
			VIF_BASE,
			VIF_DIRECT				// Direct + TTE has alignment issues, MPG, and unpack do not
		} VIF1_STATE;				// The different states the DMA chain can be in

		VIF1_STATE m_VifState;		// The VIF state at this point in the chain
};

// The static DMA chain. You should use this by first getting the physical address via GetPointer
// You should then save this address to use with a DMACall tag in the dynamic chain. Then add your
// data in the standard CDMAChain way. Finally add a DMARet tag to the static DMA chain so that
// the DMAC will return control to the dynamic chain.
class CVIFStaticDMA : public CDMAChain, public CSingleton<CVIFStaticDMA>
{
	public:
		CVIFStaticDMA();
		virtual ~CVIFStaticDMA();

		void Initialise(int iNumPages);

		int GetPointer();			// Retrieve the current pointer's physical address

	protected:
		virtual int NewPage();

		int m_iCurrPage;
		int m_iPhysicalOffset;
};

inline int CVIFStaticDMA::GetPointer()
{
	return (int)m_pDMATag + m_iPhysicalOffset;
}

// The dynamic DMA chain is the only DMA chain that you should fire. You should fire this at
// the start of your main loop, then add all of your data to the class. That way while the previous
// frame's data is being transferred, you can be building the next packet in parallel.
// This DMA chain is double buffered to allow this, so you should remember that when you call
// Initialise, this class will actually use double that, so make sure to allocate enough memory!
class CVIFDynamicDMA : public CDMAChain, public CSingleton<CVIFDynamicDMA>
{
	public:
		CVIFDynamicDMA();
		virtual ~CVIFDynamicDMA();

		// See comment above
		void Initialise(int iNumPages);
		// This fires the packet via path 1. It takes two parameters, one that allows you
		// to build but not actually fire the packet (good for debugging), and the second which
		// will print the packet out via printf.
		void Fire(bool bFire = true, bool bPrint = false);

		// Prints the packet via printf
		virtual void PrintPacket();

		// Retrieve a temporary piece of SPS2 memory. Note that this returns the
		// First page from the buffer that is currently being sent, so don't call this
		// Directly after calling Fire or you could experience problems.
		CDMAMem & getTempPage()
		{
			return m_pPages[(m_iCurrBuffer ^ 1) * m_iNumPages];
		}

	protected:
		virtual int NewPage();
		
		int m_iCurrPage;	// The current page we are writing to.
		int m_iCurrBuffer;	// The current buffer we are writing to (0 or 1)
};

#endif
