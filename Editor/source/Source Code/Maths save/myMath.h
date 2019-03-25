#ifndef MY_MATH_H
#define MY_MATH_H

//#include "globals.h"
#include <math.h>
//openGL
#include "GLee.h"
#include <gl\gl.h>									
#include <gl\glu.h>									
#include <gl\glaux.h>

//defines for sphere collision detction
//results pi and collision types

#define PI 3.1415926535897932
#define BEHIND 0
#define INFRONT 1
#define INTERSECTS 2
#define SPHERE 3
#define ELLIPSOID 4	

//vector struct
struct SVector3
{
	SVector3() {}

	SVector3 (float new_x, float new_y, float new_z)	 
	{
		x = new_x; y = new_y; z = new_z;
	}

	//add Vector
	SVector3 operator+(SVector3 vector) {return SVector3(vector.x + x, vector.y + y, vector.z + z);}

	//subtract Vector
	SVector3 operator-(SVector3 vector) {return SVector3(x-vector.x, y-vector.y, z-vector.z);}

	//Multiply Vector
	SVector3 operator*(float t) {return SVector3(x*t,y*t,z*t);}

	bool operator<(float cmp) 
	{
		if((x > 0) && (y > 0) && (z > 0)) return true;
		else return false;
	}

	SVector3 operator*=(SVector3 v) {return SVector3(x*v.x,y*v.y,z*v.z);}

	float x, y, z;

};

template <class T>
class SVector2{

public :
	T x,y;

	SVector2 operator+(SVector2 rhs){return SVector2(rhs.x + x, rhs.y + y);}

};

typedef struct
{
	unsigned char r,g,b;

}colorByte;

typedef struct
{
	float x,y;
}NVector2;

//standard vector/math operations------------------------------------------------------------

float absolute(float num);

SVector3 cross(SVector3 v1,SVector3 v2);

float dot(SVector3 v1,SVector3 v2);

SVector3 normalize(SVector3 normal);

SVector3 getNormal(SVector3 poly[]);

double angleBetweenVector(SVector3 v1,SVector3 v2);

float magnitude(SVector3 normal);

float distance(SVector3 p1, SVector3 p2);

float planeDistance(SVector3 normal, SVector3 point);

SVector3 divideVectorByScalar(SVector3 vector, float scalar);

double degreesToRadians(float degreeAngle);

//-----------------------------------------------------------------------------------------

//collision detection maths----------------------------------------------------------------

bool insidePolygon(SVector3 intersection, SVector3 poly[], long vertexCount);

bool intersectedPolygon(SVector3 poly[], SVector3 line[], int vertexCount,SVector3 &slideTo);

SVector3 intersectionPoint(SVector3 normal, SVector3 line[], double distance);

SVector3 closestPointOnLine(SVector3 v1, SVector3 v2, SVector3 p1);

bool intersectedPlane(SVector3 poly[], SVector3 line[], SVector3 &normal, float &originDistance);

bool polyIntersectingBox(SVector3 box[], SVector3 poly[], int vertexCount);


//spherical collision test------------------------------------------------------------------

//plane sphere collison returns infront, behinf or intersecting
int classifySphere(SVector3 &centre, 
				   SVector3 &normal, SVector3 &point, float radius, float &distance);

//check if the sphere is colliding on its edges
bool edgeSphereCollision(SVector3 &centre, 
						 SVector3 polygon[], int vertexCount, float radius, SVector3 &ip,float distanceToPlane,SVector3 radiusVector);


//checks if sphere is intersecting a polygon
bool spherePolygonCollision(SVector3 polygon[], 
							SVector3 centre, int vertexCount, float radius,SVector3 &ip,bool edges,SVector3 radiusVector,bool offset);


//matrix math-------------------------------------------------------------------------------

SVector3 rotateAboutY(SVector3 vertex,SVector3 scale, SVector3 pos,int angle);

void transformPointByMatrix(SVector3 &point, float matrix[]);
void getPointFromNewMatrix(SVector3 &point,float matrix[],float x, float y, float z,float xrot,float yrot,float zrot); 
void multiplyPointByMatrix(SVector3 muzPosition,float matrix[]);

void billboardBegin();
void billboardSphericalBegin();
void billboardEnd();

SVector3 rotateAndScale(SVector3 vertex, SVector3 scale,int angle);

class CFrustum {

public:

	// Call this every time the camera moves to update the frustum
	void CalculateFrustum();

	// This takes a 3D point and returns TRUE if it's inside of the frustum
	bool PointInFrustum(float x, float y, float z);

	// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
	bool SphereInFrustum(float x, float y, float z, float radius);

	// This takes the center and half the length of the cube.
	bool CubeInFrustum( float x, float y, float z, float size );

private:

	// This holds the A B C and D values for each side of our frustum.
	float m_Frustum[6][4];
};

#endif