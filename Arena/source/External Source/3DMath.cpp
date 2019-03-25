/************************************************************
*	APRON TUTORIAL PRESENTED BY MORROWLAND					*
*************************************************************
*	Author					: Ronny André Reierstad			*
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
************************************************************/

#include "3DMath.h"
#include <math.h>	


/////////////////////////////////////////////////////////////////////////////////////////////////
//										 ABSOLUTE		returnes the absolute value
/////////////////////////////////////////////////////////////////////////////////////////////////
float Absolute(float num)
{
	if(num < 0) return (0 - num);
	return num;	
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										 CROSS			returnes the cross product
/////////////////////////////////////////////////////////////////////////////////////////////////
tVector3 Cross(tVector3 vVector1, tVector3 vVector2)
{
	tVector3 vNormal;									
	
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
	
	return vNormal;										
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										 DOT			calculates the dot product
/////////////////////////////////////////////////////////////////////////////////////////////////
float Dot(tVector3 vVector1, tVector3 vVector2) 
{
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										 NORMALIZE		returnes the vector normalized
/////////////////////////////////////////////////////////////////////////////////////////////////
tVector3 Normalize(tVector3 vNormal)
{
	float magnitude = Magnitude(vNormal);				
	
	vNormal.x /= magnitude;								
	vNormal.y /= magnitude;								
	vNormal.z /= magnitude;	
	
	return vNormal;							
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										 NORMAL			returnes the normal of a triangle
/////////////////////////////////////////////////////////////////////////////////////////////////
tVector3 Normal(tVector3 vTriangle[])					
{	
	tVector3 vVector1 = vTriangle[2] - vTriangle[0];
	tVector3 vVector2 = vTriangle[1] - vTriangle[0];
	tVector3 vNormal = Cross(vVector1, vVector2);		
	vNormal = Normalize(vNormal);						
	
	return vNormal;										
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										 MAGNITUDE		magnitude of a normal or vector
/////////////////////////////////////////////////////////////////////////////////////////////////
float Magnitude(tVector3 vNormal)
{
	return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}