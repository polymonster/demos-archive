#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <sps2lib.h>
#include <sps2tags.h>
#include "PS2Defines.h"

// These are global in main.cpp, extern them here so we can use them
extern sps2Memory_t * pMemCached;
extern int iSPS2Desc;

// All the following is the layout of a bitmap file header (the bit before
// the actual image data). The __attribute__((packed)) tells the compiler
// not to insert any annoying padding. (If it was padded we wouldn't be
// able to read it in in one go easily)
#define BITMAP_ID      0x4D42

struct BitmapFileHeader
{
	uint16  bfType;
    uint32  bfSize;
    uint16  bfReserved1;
    uint16  bfReserved2;
    uint32  bfOffBits;
} __attribute__((packed));

struct BitmapInfoHeader
{
	uint32	biSize;
	int32	biWidth;
	int32	biHeight;
	uint16	biPlanes;
	uint16	biBitCount;
	uint32	biCompression;
	uint32	biSizeImage;
	int32	biXPelsPerMeter;
	int32	biYPelsPerMeter;
	uint32	biClrUsed;
	uint32	biClrImportant;
} __attribute__((packed));

struct BitmapFile
{
	BitmapFileHeader  bitmapfileheader;
	BitmapInfoHeader  bitmapinfoheader;
} __attribute__((packed));

class CTexture
{
public:
	CTexture();
	~CTexture();

	bool LoadBitmap(const char * strFilename, bool bBlackTransparent = false, bool bRedAsAlpha = false);
	uint64 Upload(uint32 uGSPage);	// This function returns tex0, incase you want to modify it.
	void Select(uint64 Tex0 = 0);	// If you modified tex0, you can pass it back here. Otherwise the
									// function will use its own values for tex0.

private:
	int m_iTexAddr;
	int m_iClutAddr;

	uint64 m_iTex0;

	int m_iDataSize;

	unsigned char m_pOriginalPalette[256*4];
	unsigned int m_pPalette32[256];

	int m_iImageDepth;
	int m_iPalBytesPerPixel;

	int m_iWidth, m_iHeight;
	int m_l2Width, m_l2Height;

	bool m_bValid;
};

#endif