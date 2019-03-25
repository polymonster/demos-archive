#ifndef __SPS2WRAP_H__
#define __SPS2WRAP_H__

#include <sps2lib.h>
#include <sps2util.h>
#include <sps2tags.h>
#include "singleton.h"

// This define allows us to treat SPS2Manager as a global
// instance of the CSPS2Manager class, that's all you need
// to know to use it!
#define SPS2Manager CSPS2Manager::GetSingleton()

class CDMAMem;

class CSPS2Manager : public CSingleton<CSPS2Manager>
{
	public:
		CSPS2Manager();
		~CSPS2Manager();

		// Call this function at the start of your program.
		// It allocates as many pages of SPS2 memory as you
		// want. 512 pages, i.e. 20MB is about the most you
		// can safely allocate.
		void Initialise(const int iAllocPages);

		// Call this once, after Initialise to set the colour
		// that the screen should be cleared to (note, you have
		// to call this, even if you don't care what colour the
		// screen is cleared to)
		void InitScreenClear(int R, int G, int B);

		// Call these before, and after, you do any rendering.
		void BeginScene();
		void EndScene();

		// This function will put iQuadWords amount of SPS2 memory
		// into the Memory structure. (Note, 256 QWords is the maximum
		// amount that a CDMAMem structure can hold)
		void Allocate(CDMAMem & Memory, int iQuadWords);

		// Calls sps2FlushCache
		void FlushCache();

		int GetScreenWidth()
		{ return sps2UScreenGetWidth(); }

		int GetScreenHeight()
		{ return sps2UScreenGetHeight(); }
		
	private:
		int m_iSPS2Desc;

		// The memory that is allocated by Initialise( )
		sps2Memory_t * m_pMemCached, 
					 * m_pMemUncached;
		
		// Keep an array with the memory remaining in each page
		int * m_iaFreeMemInPage;
		// How many pages worth of memory did we allocate?
		int m_iNumPages;
};

// Call this once you have finished drawing.
inline void CSPS2Manager::EndScene()
{
	while(*EE_D1_CHCR & 256);
	sps2UScreenSwap();
}

// Flushes all data through SPS2 to make sure it is written to memory before
// it is fired off to the DMAC.
inline void CSPS2Manager::FlushCache()
{
	sps2FlushCache(m_iSPS2Desc);
}

// A simple structure to look after a memory page (4096k, 256 quadwords)
class CDMAMem
{
	public:
		// Construtor (no parameters)
		CDMAMem()
		:m_iAmount(0), m_pCached(NULL), 
		 m_pUncached(NULL), m_iPhysAddr(0)
			{};
		~CDMAMem(){};

		// Constructor, specifying how many quad words to
		// use (must be less than or equal to 256).
		CDMAMem(int iQWC)
		:m_iAmount(0), m_pCached(NULL), 
		 m_pUncached(NULL), m_iPhysAddr(0)
		{
			SPS2Manager.Allocate(*this, iQWC);
		}

		// Set all of the member variables.
		void Set(int Amount, void * pCached, void * pUncached, int PhysAddr)
		{
			m_iAmount = Amount;
			m_pCached = pCached;
			m_pUncached = pUncached;
			m_iPhysAddr = PhysAddr;
		}

		// Get the cached pointer to this memory block.
		void * GetCached()
		{ return m_pCached; }

		// Get the uncached pointer to this memory block.
		void * GetUncached()
		{ return m_pUncached; }

		// Get the physical address of this memory block.
		int GetPhysicalAddr()
		{ return m_iPhysAddr; }
		
		// Get the size of this memory block.
		int GetSize()
		{ return m_iAmount; }
		
	protected:
		int m_iAmount;		// Number of quadwords in this block.
		void * m_pCached,	// Cached pointer to the start of this block.
		     * m_pUncached;	// Uncached pointer to the start of this block.
		int m_iPhysAddr;	// Physical address of this start of this block.
};

#endif
