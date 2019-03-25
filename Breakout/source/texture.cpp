#include "dma.h"
#include "texture.h"

// Returns the base 2 logarithm of the texture size
// (also a handy way to check for illegal texture sizes)
uint32 TextureLog2(uint32 n)
{
	switch(n)
	{
	case 256:
		return 8;
	case 128:
		return 7;
	case 64:
		return 6;
	case 32:
		return 5;
	default:
		{
			printf("Unsupported texture size\n");
			exit(-1);
		}
	}
}

uint32 TSMFromColourDepth(uint32 depth)
{
	switch(depth)
	{
	case 32:
		return PSMCT32;
	case 8:
		return PSMT8;
	default:
		printf("Unsupported colour depth.\n");
		exit(-1);
	}
}

// pMemory should be enough memory for the texture 
// + the clut (if there is one)
CTexture::CTexture()
{
	m_bValid = false;
}

CTexture::~CTexture()
{
}

// This function will load 24 or 8 bit windows bmp files into
// the SPS2 memory that was given to the class constructor
bool CTexture::LoadBitmap(const char * strFilename, bool bBlackTransparent, bool bRedAsAlpha)
{
	// Open the file
	FILE * fp = NULL;
	fp = fopen(strFilename, "rb");

	if(fp == NULL)
	{
		printf("Failed to load %s\n", strFilename);
		return false;
	}

	// This struct contains the headers that we will read in.
	BitmapFile bmp;

	// Read in the bitmap header
	fread(&bmp.bitmapfileheader,sizeof(BitmapFileHeader),1,fp);

	// Check that this is a bitmap file and someone isn't trying to read
	// some weird file in.
	if(bmp.bitmapfileheader.bfType != BITMAP_ID)
	{
		printf("%s is not a valid bitmap file\n",strFilename);
		
		fclose(fp);
		return false;
	}

	// Read in the position of the bitmap header, then set the file
	// pointer to it
	fread(&bmp.bitmapinfoheader,sizeof(BitmapInfoHeader),1,fp);

	m_iWidth = bmp.bitmapinfoheader.biWidth;
	m_iHeight = bmp.bitmapinfoheader.biHeight;

	// Make sure that the user isn't trying to load an unsupported colour
	// depth
	assert(bmp.bitmapinfoheader.biBitCount == 8 ||
		   bmp.bitmapinfoheader.biBitCount == 24);

	// We will use PSMT8 for 8 bit images, and PSMCT32 for 24 bit images
	m_iImageDepth = (bmp.bitmapinfoheader.biBitCount == 8) ? 8 : 32;

	// The log2 function has an assert in it that will be fired off if
	// the texture is not a power of 2 or is an unsupported size
	m_l2Width = TextureLog2(m_iWidth);
	m_l2Height = TextureLog2(m_iHeight);

	// The image size in bytes
	int iImageSize;

	// Some image utilities don't set this field, so check first
	if(bmp.bitmapinfoheader.biSizeImage!=0)
		iImageSize = bmp.bitmapinfoheader.biSizeImage;
	else
		iImageSize = bmp.bitmapfileheader.bfSize - bmp.bitmapfileheader.bfOffBits;

	// Next, if we are loading an 8 bit texture we must load
	// a CLUT (Colour look-up table). 
	int clut[256], swizzledclut[256];

	if(bmp.bitmapinfoheader.biBitCount == 8)
	{
		fread(&clut, sizeof(int), 256, fp);

		// Loop through each CLUT entry and convert it from
		// BGR to RGB, and "swizzle" it
		for(int p=0; p<256; p++)
		{	
			if(bRedAsAlpha)
			{
				uint8 * pPtr = (uint8*)&clut[p];
				pPtr[3]= pPtr[2] / 2;
				pPtr[0] = 0xFF;
				pPtr[1] = 0xFF;
				pPtr[2] = 0xFF;
			}
			else
			{
				unsigned char * pPtr = (unsigned char *)&clut[p];
				unsigned char red = pPtr[2];
				pPtr[2] = pPtr[0];
				pPtr[0] = red;

				// Set the alpha to 0 if the colour is black, 0x80 otherwise
				if(clut[p] == 0 && bBlackTransparent)
					pPtr[3] = 0x00;
				else
					pPtr[3] = 0x80;
			}

			// If you look at page 32 of GSUSER_E.pdf you will see
			// a table called "Arrangement of CLUT in IDTEX8" if you
			// look closely you will see that the CLUT is not laid out
			// contiguously. Therefore we must "swizzle" the position so that
			// the clut is placed into memory correctly.
			// (basically what this does is swap bits 3 & 4, see the tutorial
			// doc for more info)
			((uint32 *) swizzledclut)[(p & 231) + ((p & 8) << 1) + ((p & 16) >> 1)] = clut[p];
		}
	}

	// The texture data buffer
	m_iTexAddr = VIFStaticDMA.GetPointer();
	VIFStaticDMA.StartDirect();

	// Get the total size of all of the pixel data
	m_iDataSize = m_iWidth * m_iHeight * (m_iImageDepth / 8);

	assert((m_iDataSize & 0xF) == 0);	// Make sure the texture data size is a quadword multiple.

	// Start on the last line and work our way up the image as
	// bitmaps are stored upside down
	unsigned int uLine = m_iHeight - 1;
	int iColumn = 0;
	int i = 0;

	// Seek the file to the image data
	fseek(fp,(int)(bmp.bitmapfileheader.bfOffBits) + (uLine * m_iWidth * (bmp.bitmapinfoheader.biBitCount / 8)) ,SEEK_SET);

	for(i = 0; i < (m_iWidth * m_iHeight); i++, iColumn++)
	{
		// We have finished a whole row move up one line
		// (we know we have finished a row if i is a multiple
		// of the image width
		if(i > 0 && i % m_iWidth == 0)
		{
			--uLine;
			iColumn = 0;

			fseek(fp,(int)(bmp.bitmapfileheader.bfOffBits) + (uLine * m_iWidth * (bmp.bitmapinfoheader.biBitCount / 8)) ,SEEK_SET);
		}

		if(bmp.bitmapinfoheader.biBitCount == 8)
		{
			// We have to wait until we have 4 pixels to add them to the DMA chain, because 32 bits are the smallest size
			// the DMA class accepts. (we only accept images that have widths that are a multiple of 4 so we don't need to
			// check that)
			if( i % 4 == 0 )
			{
				// Read the pixel in
				int pixel = 0;
				fread(&pixel,4,1,fp);

				VIFStaticDMA.Add32(pixel);
			}
		}
		else	// Must be 24 bit colour
		{
			// Read the pixel in
			uint32 pixel = 0;
			fread(&pixel,1,3,fp);

			if(bRedAsAlpha)
			{
				uint8 * pPtr = (uint8*)&pixel;
				pPtr[3]= pPtr[2] / 2;
				pPtr[0] = 0xFF;
				pPtr[1] = 0xFF;
				pPtr[2] = 0xFF;
			}
			else
			{
				// Swap the red and blue channels
				uint8 * pPtr = (uint8*)&pixel;
				uint8 red = pPtr[2];
				pPtr[2] = pPtr[0];
				pPtr[0] = red;

				// Set the alpha to 0 if the colour is black
				if(pixel == 0 && bBlackTransparent)
					pPtr[3] = 0;
				else
					pPtr[3] = 0x80;
			}

			VIFStaticDMA.Add32(pixel);
		}
	}

	// End the texture data buffer
	VIFStaticDMA.EndDirect();
	VIFStaticDMA.DMARet();

	// And if we have a clut...
	if( m_iImageDepth == 8 )
	{
		// Start a clut buffer
		m_iClutAddr = VIFStaticDMA.GetPointer();

		VIFStaticDMA.StartDirect();

		for(int i = 0; i < 256; i++)
		{
			VIFStaticDMA.Add32(swizzledclut[i]);
		}

		VIFStaticDMA.EndDirect();

		VIFStaticDMA.DMARet();
	}

	// Close the file
	fclose(fp);

	return true;
}

void CTexture::Select(uint64 Tex0 = 0)
{
	VIFDynamicDMA.StartDirect();
	VIFDynamicDMA.StartAD();
	VIFDynamicDMA.AddAD(0, TEXFLUSH);
	VIFDynamicDMA.AddAD(ALPHA_SET(0, 1, 0, 1, 128), ALPHA_1);
	VIFDynamicDMA.AddAD(TEX1_SET(1, 0, 1, 1, 0, 0, 0), TEX1_1);

	// If the user passed their own TEX0 value, use it.
	// Otherwise just use the default.
	if(Tex0 != 0)
		VIFDynamicDMA.AddAD(Tex0, TEX0_1);
	else
		VIFDynamicDMA.AddAD(m_iTex0, TEX0_1);

	VIFDynamicDMA.EndAD();
	VIFDynamicDMA.EndDirect();
}

uint64 CTexture::Upload(uint32 uGSPage)
{
	VIFDynamicDMA.Add32(FLUSH);

	uint32 iDstPtr = (uGSPage << 5);
	uint32 iClutPtr = ((uGSPage + 15) << 5);
	uint32 uQSizeClut = (256 >> 2);

	if( m_iImageDepth == 8 )
	{
		// Add the clut
		VIFDynamicDMA.StartDirect();

		VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(4, 0, 0, 0, 
							 GIF_FLG_PACKED, GS_BATCH_1(GIF_REG_A_D)));

		VIFDynamicDMA.Add64(BITBLTBUF_SET(0, 0, 0, iClutPtr, 1, 0));
		VIFDynamicDMA.Add64(BITBLTBUF);

		VIFDynamicDMA.Add64(TRXPOS_SET(0, 0, 0, 0, 0));
		VIFDynamicDMA.Add64(TRXPOS);

		VIFDynamicDMA.Add64(TRXREG_SET(16, 16));
		VIFDynamicDMA.Add64(TRXREG);

		VIFDynamicDMA.Add64(TRXDIR_SET(0));
		VIFDynamicDMA.Add64(TRXDIR);

		VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(uQSizeClut, 0, 0, 0, 
							 GIF_FLG_IMAGE, 0));

		VIFDynamicDMA.EndDirect();

		VIFDynamicDMA.DMACall(m_iClutAddr);
	}

	// Add the texture data itself.
	VIFDynamicDMA.StartDirect();
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(4, 0, 0, 0,
						 GIF_FLG_PACKED, GS_BATCH_1(GIF_REG_A_D)));

	int TBW = m_iWidth / 64;

	VIFDynamicDMA.Add64(BITBLTBUF_SET(0, 0, 0, iDstPtr, (TBW == 0) ? 1 : TBW, m_iImageDepth == 8 ? PSMT8 : PSMCT32));
	VIFDynamicDMA.Add64(BITBLTBUF);

	VIFDynamicDMA.Add64(TRXPOS_SET(0, 0, 0, 0, 0));
	VIFDynamicDMA.Add64(TRXPOS);

	VIFDynamicDMA.Add64(TRXREG_SET(m_iWidth, m_iHeight));
	VIFDynamicDMA.Add64(TRXREG);

	VIFDynamicDMA.Add64(TRXDIR_SET(0));
	VIFDynamicDMA.Add64(TRXDIR);

	uint32 iQWC = (m_iDataSize) >> 4;

	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(iQWC, 1, 0, 0, 
						 GIF_FLG_IMAGE, 0));

	VIFDynamicDMA.EndDirect();

	VIFDynamicDMA.DMACall(m_iTexAddr);

	// Generate a default TEX0.
	m_iTex0 = TEX0_SET(iDstPtr, (TBW == 0) ? 1 : TBW, m_iImageDepth == 8 ? PSMT8 : PSMCT32, m_l2Width, m_l2Height, 1, 0, iClutPtr, 0, 0, 0, 1);

	return m_iTex0;
}