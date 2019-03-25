/*=========================================================*\
|	console.cpp - console output and in-game console
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "console.h"
#include "common.h"
#include <iostream>

/*======================== CONSTANTS ======================*/


/*======================== VARIABLES ======================*/

Console *g_console_p = NULL;

/*================= PROTOTYPE DEFINITIONS =================*/

Console::Console()
{
	//#ifdef _DEBUG

		m_text_pool = new char[4096];
		memset( m_text_pool, NULL, 4096 );

		//strcpy((char *)m_last_gl_error,"no error processed yet\0");

		//allocate a console window
		AllocConsole();
		freopen("conin$","r",stdin);
		freopen("conout$","w",stdout);
		freopen("conout$","w",stderr);

	//#endif
}

Console::~Console()
{
	//#ifdef _DEBUG

		delete [] m_text_pool;

	//#endif
}

void Console::dbgmsg( char *string_p, ... )
{
	//#ifdef _DEBUG

		// variable argument list
		va_list	valist;

   		va_start( valist, string_p );
   		vsprintf_s( m_text_pool, 4096, string_p, valist );

		char *outstr_p = m_text_pool;

		// output to console
		//OutputDebugStringA( outstr_p );
		std::cout << outstr_p;

	//#endif
}

void Console::process_gl_errors()
{
	GLenum e = glGetError();
	unsigned char *c = (unsigned char*) gluErrorString(e);
	
	const char *conc = (const char *)c;

	if(strcmp(conc, "no error"))
	{
		dbgmsg("GL Error : %s\n", c);
	}
}