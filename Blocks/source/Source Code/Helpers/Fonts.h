#ifndef _FONTS_H
#define _FONTS_H

#include "Globals.h"

#define MAX_CHARS 255
#define FONT_HEIGHT 10

class CText{

public:

	static void create();
	static void destroy();

	static CText* getInstance(){return m_instance;};

	//Font Creation
	void createFont(LPSTR strFontName, int height, HDC &hdc);

	//display
	void output(int x, int y, const char *strString, ...);
	void positionText(int x, int y);

private:

	CText(){};
	~CText(){};

	static CText *m_instance;

	unsigned int m_font[6];

	HFONT m_oldFont;
};

#endif