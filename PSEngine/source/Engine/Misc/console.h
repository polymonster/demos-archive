/*=========================================================*\
|	console.h - console output and in-game console
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _CONSOLE_H
#define _CONSOLE_H

/*======================== INCLUDES =======================*/

#include <windows.h>
#include <stdio.h>


/*========================= CLASSES =======================*/


class Console
{
public:

	Console();
	~Console();

	void		dbgmsg		( char *string_p, ... );
	void		process_gl_errors();

private:

	char			*m_text_pool;
	unsigned char	*m_last_gl_error;

};



/*================== EXTERNAL VARIABLES ===================*/

extern Console		*g_console_p;

#endif //_CONSOLE_H