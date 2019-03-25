#include "font.h"

#define LETTERSPERROW 16

CFont::CFont()
{
	m_DoubleHeight = false;
	
	m_FontWidths = new unsigned char[256];

	for(int i = 0; i < 256; i++)
		m_FontWidths[i] = 16;
}

CFont::~CFont()
{
	if(m_FontWidths)
		delete [] m_FontWidths;
}

bool CFont::Load(const char * strWidths, bool DoubleHeight)
{
	m_DoubleHeight = DoubleHeight;
	
	// Load the font widths from the file
	FILE * fp = fopen(strWidths, "rb");

	if(fp == NULL)
		return false;

	if(m_FontWidths)
		delete [] m_FontWidths;

	m_FontWidths = new unsigned char[256];
	fread(m_FontWidths, sizeof(unsigned char), 256, fp);
	fclose(fp);

	return true;
}

// Get the width of the first line of the string in pixels
int CFont::GetStringWidth(const char * strText)
{
	size_t textlen = strlen(strText);
	int size = 0;

	for(uint32 letter = 0; letter < textlen; letter++)
	{
		if(strText[letter] == '\n')
			break;

		size += m_FontWidths[strText[letter]];
	}

	return size;
}

void CFont::RenderL(const int x, const int y, const char * strText, uint8 R, uint8 G, uint8 B, uint8 A)
{
	// How many sprites we have to render
	size_t textlen = strlen(strText);

	if(textlen == 0)
		return;

	VIFDynamicDMA.StartDirect();
	VIFDynamicDMA.StartAD();
	VIFDynamicDMA.AddAD(RGBAQ_SET(R,G,B,A,0), RGBAQ);
	VIFDynamicDMA.AddAD(GS_PRIM(GS_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0), PRIM);
	VIFDynamicDMA.EndAD();

	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(textlen, 1, 0, 0, GIF_FLG_REGLIST, GS_BATCH_4(GIF_REG_UV,
						 GIF_REG_XYZ2, GIF_REG_UV, GIF_REG_XYZ2)));

	uint32 lastx = x;
	uint32 line = 0;
	uint32 uvOffset = 0;
	uint32 lineHeight = 16;

	if(m_DoubleHeight)
	{
		uvOffset = 8;
		lineHeight = 32;
	}

	// Loop through all the letters
	for(uint32 letter = 0; letter < textlen; letter++)
	{
		char cLetter = strText[letter];

		// If the letter is 'Newline' move down a line
		if(cLetter == '\n')
		{
			++line;
			lastx = x;
		}

		// The width of the letter in pixels
		int letterwidth = m_FontWidths[cLetter];

		// Make the letterwidth even
		letterwidth = (letterwidth % 2) ? letterwidth + 1 : letterwidth;

		// Get the texture coordinates of the letter
		int tx = cLetter % LETTERSPERROW;
		int ty = cLetter / LETTERSPERROW;

		// The texture coordinates of the center of the letter
		int cx = (tx * 16) + 8;
	
		// Work out the texture coordinates from the center of the letter
		int u0 = ((cx - ((letterwidth) / 2)) << 4);
		int v0 = ((ty * 16) << 4) + uvOffset;

		int u1 = ((letterwidth) << 4) + u0;
		int v1 = ((16) << 4) + v0 - uvOffset;

		// Work out the positions of the corners of the sprites
		int left	= lastx;
		int right	= lastx + letterwidth;
		int top		= y + (line * lineHeight);
		int bottom	= y + (line * lineHeight) + lineHeight;

		VIFDynamicDMA.Add64(UV_SET(u0, v0));
		VIFDynamicDMA.Add64(XYZ2_SET(((left + 2048) << 4) - 8,
									 ((top  + 2048) << 4) - 8,
									 0xFFFFFF));
		VIFDynamicDMA.Add64(UV_SET(u1, v1));
		VIFDynamicDMA.Add64(XYZ2_SET(((right  + 2048) << 4) - 8,
									 ((bottom + 2048) << 4) - 8,
									 0xFFFFFF));

		// Move the sprite position forward the size of the letter we just drew
		lastx += letterwidth;
	}

	VIFDynamicDMA.EndDirect();
}

void CFont::RenderC(const int x, const int y, const char * strText, uint8 R = 0x80, uint8 G = 0x80, uint8 B = 0x80, uint8 A = 0x80)
{
	RenderL(x - (GetStringWidth(strText) >> 1), y, strText, R, G, B, A);
}

void CFont::RenderR(const int x, const int y, const char * strText, uint8 R = 0x80, uint8 G = 0x80, uint8 B = 0x80, uint8 A = 0x80)
{
	RenderL(x - GetStringWidth(strText), y, strText, R, G, B, A);
}

// A printf wrapper that uses our Render function.
void CFont::printfL(const int x, const int y, uint8 R, uint8 G, uint8 B, uint8 A, const char * strText, ...)
{
	// Unpack the extra arguments (...) using the format specificiation in 
	// strText into strBuffer
	va_list args;
	char strBuffer[4096];

	if (!strText)
		return;

	va_start(args, strText);
	vsprintf(strBuffer, strText, args);
	va_end(args);

	RenderL(x, y, strBuffer, R, G, B, A);
}

// A printf wrapper that uses our Render function.
void CFont::printfR(const int x, const int y, uint8 R, uint8 G, uint8 B, uint8 A, const char * strText, ...)
{
	// Unpack the extra arguments (...) using the format specificiation in 
	// strText into strBuffer
	va_list args;
	char strBuffer[4096];

	if (!strText)
		return;

	va_start(args, strText);
	vsprintf(strBuffer, strText, args);
	va_end(args);

	RenderL(x - GetStringWidth(strBuffer), y, strBuffer, R, G, B, A);
}

void CFont::printfC(const int x, const int y, uint8 R, uint8 G, uint8 B, uint8 A, const char * strText, ...)
{
	// Unpack the extra arguments (...) using the format specificiation in 
	// strText into strBuffer
	va_list args;
	char strBuffer[4096];

	if (!strText)
		return;

	va_start(args, strText);
	vsprintf(strBuffer, strText, args);
	va_end(args);

	RenderL(x - (GetStringWidth(strBuffer) >> 1), y, strBuffer, R, G, B, A);
}
