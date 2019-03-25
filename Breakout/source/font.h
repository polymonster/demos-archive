#ifndef __FONT_H__
#define __FONT_H__

#include "dma.h"
#include <stdio.h>
#include <stdarg.h>

class CFont
{
public:
	CFont();
	~CFont();

	bool Load(const char * strWidths, bool DoubleHeight = false);

	// (L)eft, (R)ight, and (C)entered font rendering functions
	void RenderL(const int x, const int y, const char * strText, 
				uint8 R = 0x80, uint8 G = 0x80, uint8 B = 0x80, uint8 A = 0x80);
	void RenderR(const int x, const int y, const char * strText, 
				uint8 R = 0x80, uint8 G = 0x80, uint8 B = 0x80, uint8 A = 0x80);
	void RenderC(const int x, const int y, const char * strText, 
				uint8 R = 0x80, uint8 G = 0x80, uint8 B = 0x80, uint8 A = 0x80);

	// (L)eft, (R)ight, and (C)entered font rendering functions, with variable arguments.
	void printfL(const int x, const int y, uint8 R, uint8 G, uint8 B, uint8 A, const char * strText, ...);
	void printfR(const int x, const int y, uint8 R, uint8 G, uint8 B, uint8 A, const char * strText, ...);
	void printfC(const int x, const int y, uint8 R, uint8 G, uint8 B, uint8 A, const char * strText, ...);

	int GetStringWidth(const char * strText);

protected:
	unsigned char * m_FontWidths;
	bool m_DoubleHeight;
};

#endif