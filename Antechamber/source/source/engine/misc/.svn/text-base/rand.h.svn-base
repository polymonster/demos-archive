

#ifndef	_RANDOMNUM_H
#define _RANDOMNUM_H


#include <stdio.h>
#include <stdlib.h>

#include "apptimer.h"


int randnum( int lower, int upper )
{
	float frand = (   (float)rand() / ((float)(RAND_MAX)+(float)(1)) );
	int ret = (int)( frand * (float)( upper - lower ) ) + lower;
	return( ret );

};


#endif
