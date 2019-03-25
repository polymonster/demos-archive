/****************************************
/	Al (c) 2008 - dxio.cpp
/****************************************
/	print stuff
/****************************************/


/*************** INCLUDES ***************/

#include "dxio.h"

/*************** CONSTANTS **************/


/*********** GLOBAL VARIABLES ***********/


/********* FUNCTION PROTOTYPES **********/


void dbgmsg( char *string_p, ... )
{
	va_list	valist;
	char 	outstr[2048];

   	va_start(valist,string_p);
   	int ret = vsprintf(outstr,string_p,valist);

	OutputDebugStringA( outstr );
}