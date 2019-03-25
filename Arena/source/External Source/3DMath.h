#ifndef _3DMATH_H
#define _3DMATH_H

#include "globals.h"


#define PI 3.1415926535897932

#define BEHIND		0	
#define INTERSECTS	1	
#define FRONT		2	


// This returns the absolute value of num - a simple if/else check
float absolute(float num);

//	This returns a perpendicular vector from 2 given vectors by taking the cross product.
tVector3 Cross(tVector3 vVector1, tVector3 vVector2);

// This returns the dot product between 2 vectors
float Dot(tVector3 vVector1, tVector3 vVector2);

//	This returns a normalize vector (A vector exactly of length 1)
tVector3 Normalize(tVector3 vNormal);

//	This returns the normal of a polygon (The direction the polygon is facing)
tVector3 Normal(tVector3 vTriangle[]);

//	This returns the magnitude of a normal (or any other vector)
float Magnitude(tVector3 vNormal);




#endif

//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com