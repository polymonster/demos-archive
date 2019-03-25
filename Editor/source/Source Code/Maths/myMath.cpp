#include "myMath.h"
#include <math.h>
#include <float.h>
#include <fstream>
#include "helpers.h"

//convert degrees to radians
double degreesToRadians(float degreeAngle)
{
	return (degreeAngle / 180 * PI);
}

//return a positive (absolute)
float absolute(float num){

	if(num < 0)
		return (0 - num);

	return num;
}

//cross the 2 vectors to get the normal
SVector3 cross(SVector3 v1,SVector3 v2){

	SVector3 normal;

	normal.x = ((v1.y * v2.z) - (v1.z * v2.y));
	normal.y = ((v1.z * v2.x) - (v1.x * v2.z));
	normal.z = ((v1.x * v2.y) - (v1.y * v2.x));

	return normal;
}

//get the scalar product using dot 
float dot(SVector3 v1,SVector3 v2){

	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));

}

SVector3 divideVectorByScalar(SVector3 vector, float scalar)
{
	vector.x /= scalar;
	vector.y /= scalar;
	vector.z /= scalar;

	return vector;
}

//find the angle between the vectors using dot product and magnitude
double angleBetweenVector(SVector3 v1,SVector3 v2){

	//get the dot product
	double dotProduct = dot(v1,v2);

	//get magnitudes
	double magnitudeProduct = magnitude(v1) * magnitude(v2);

	//get the angle between the 2 above (radians)
	double angle = acos( dotProduct / magnitudeProduct );

	//ensure angle is not indefinite
	if(_isnan(angle))
		return 0;

	//return the angle in radians
	return angle;
}

//get the magnitude of a normal
float magnitude(SVector3 normal){

	return (float) sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
}

//find the normals of a poly,
//2 vectors at right angle and cross them
SVector3 getNormal(SVector3 poly[]){

	SVector3 v1 = poly[2] - poly[0];
	SVector3 v2 = poly[1] - poly[0];

	SVector3 normal = cross(v1, v2);

	normal = normalize(normal);

	return normal;		
}

//devide by magnitude (distance)
SVector3 normalize(SVector3 normal){

	float mag = magnitude(normal);

	normal.x /= mag;
	normal.y /= mag;
	normal.z /= mag;

	return normal;
}


bool insidePolygon(SVector3 intersection, SVector3 poly[], long vertexCount){

	const double MATCH_FACTOR = 0.99;		// *Used to cover up the error in floating point*

	double angle = 0.0;					

	SVector3 v1, v2;					
	
	for(int i = 0; i < vertexCount; i++)		// Go in a circle to each vertex and get the angle between
	{	
		v1 = poly[i] - intersection;			// Subtract the intersection point from the current vertex
												
		v2 = poly[(i + 1) % vertexCount] - intersection; // Subtract the point from the next vertex
												
		angle += angleBetweenVector(v1, v2);	// Find the angle between the 2 vectors and add them all up as we go along
	}
											
	if(angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI, (360 degrees)
		return true;							// The point is inside of the polygon
		
	
	return false;

}

bool intersectedPolygon(SVector3 poly[], SVector3 line[], int vertexCount, SVector3 &slideTo){

	SVector3 normal;

	float originDistance = 0;

	// Make sure the line intersects the plane
	if(!intersectedPlane(poly,line,normal,originDistance))
		return false;

	// use the normal and distance from above to get the intersection point
	SVector3 intersection = intersectionPoint(normal, line, originDistance);

	slideTo = intersection;
	
	// Test if intersection point is inside the sphere.
	if(insidePolygon(intersection, poly, vertexCount))
		return true;						

	return false;		
}

SVector3 intersectionPoint(SVector3 normal, SVector3 line[], double distance){

	//points line and direction
	SVector3 p, lineDir;

	double numerator = 0.0, denominator = 0.0, dist = 0.0;

	//get the vector of the line and normalize it
	lineDir = line[1] - line[0];
	lineDir = normalize(lineDir);

	//Use the plane equation to find the distance from one of the points to the plane.
	numerator = - (normal.x * line[0].x +
				   normal.y * line[0].y +
				   normal.z * line[0].z + distance);

	//take the dot product between the line vector and the normal of the polygon,
	denominator = dot(normal, lineDir);
				  
	// Since we are using division, we need to make sure we don't get a divide by zero error
	// If we do get a 0, that means that there are INFINATE points because the the line is
	// on the plane (the normal is perpendicular to the line - (Normal.Vector = 0)).  
	// In this case, we should just return any point on the line.

	//stop division by 0
	if(denominator == 0.0)						
		return line[0];							

	// Divide to get the multiplying (percentage) factor
	dist = numerator / denominator;				
	
	// times distance by vector and add the aribtrary point
	p.x = (float)(line[0].x + (lineDir.x * dist));
	p.y = (float)(line[0].y + (lineDir.y * dist));
	p.z = (float)(line[0].z + (lineDir.z * dist));

	return p;		
}

float distance(SVector3 p1, SVector3 p2){

	//find distance between the 2 points (similar to magnitude)

	//get distance in double precision
	double d = sqrt( (p2.x - p1.x) * (p2.x - p1.x) + 
					 (p2.y - p1.y) * (p2.y - p1.y) + 
					 (p2.z - p1.z) * (p2.z - p1.z));

	//Convert to float and return
	return (float) d;

}

SVector3 closestPointOnLine(SVector3 v1, SVector3 v2, SVector3 p1){

	// Create the vector from v1 >> p1.
	SVector3 vA = p1 - v1;

	// Create a normalized direction vector from v1 >> v2
    SVector3 vB = normalize(v2 - v1);

	// Use the distance formula to find the magnitude
    float d = distance(v1, v2);

	// using dot project v1 >> v2
    float t = dot(vB, vA);

	//if the points at either end of the line (ie point is beyond the line start or end)
	//if t is less then zero, its at the v1 end
    if (t <= 0) 
		return v1;
	//if t is greater than doistance then its at the other end of the line v2
    if (t >= d) 
		return v2;

	//finding the point somewhere along the line
 
	// vector of length t and direction of vB
    SVector3 vC = vB * t;

	//to get the closest point simply add vC to the starting point of the line
    SVector3 closestPoint = v1 + vC;

	return closestPoint;
}


float planeDistance(SVector3 normal, SVector3 point){

	float distance = 0;

	// Use the plane equation to find the distance D
	//esentially negative dot product between normal vector and point (p)
	distance = - ((normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z));

	return distance;	
}

bool intersectedPlane(SVector3 poly[], SVector3 line[], SVector3 &normal, float &originDistance){

	float d1=0, d2=0; //distances to the 2 ends of the line form origin
	
	// get the normal of the polygon
	normal = getNormal(poly); 
	
	//find the distance the plane is from the origin
	originDistance = planeDistance(normal, poly[0]);

	// Get the distance from point1 from the plane using: Ax + By + Cz + D
	d1 =((normal.x * line[0].x)  +							
		 (normal.y * line[0].y)  +					
		 (normal.z * line[0].z)) + originDistance;	
	
	// Get the distance from point2 from the plane using Ax + By + Cz + D
	
	d2 = ((normal.x * line[1].x)  +					
		  (normal.y * line[1].y)  +					
		  (normal.z * line[1].z)) + originDistance;


	//if d1 * d2 is positive there is no collision otherwise there is a collision
	if(d1 * d2 >= 0)			
	   return false;						
					
	return true;	
}


//plane sphere collison returns infront, behind or intersecting
int classifySphere(SVector3 &centre, 
				   SVector3 &normal, SVector3 &point, float radius, float &distance){

	float d = (float) planeDistance(normal, point);

	// use the distance formula
	distance = (normal.x * centre.x + normal.y * centre.y + normal.z * centre.z + d);

	//if the distance is less than the radius and intersection occurs
	if(absolute(distance) < radius)
		return INTERSECTS;
	else if(distance >= radius)
		return INFRONT;
	
	//else the sphere is behind the plane
	return BEHIND;
}

//check if the sphere is colliding on its edges
bool edgeSphereCollision(SVector3 &centre, 
						 SVector3 polygon[], int vertexCount, float radius, SVector3 &ip, float distanceToPlane,SVector3 radiusVector){

	//check for the closest point on the polgons line to check
	//a collision is on the edge

	SVector3 point;

	SVector3 normal = getNormal(polygon);

	//loop through all the vertices in the poly
	for(int i = 0; i < vertexCount; i++)
	{
		//from vertex i (current) to vertex i + 1 (next)
		point = closestPointOnLine(polygon[i], polygon[(i + 1) % vertexCount], centre);

		SVector3 unitSphere = point - centre;

		unitSphere = normalize(unitSphere);

		SVector3 eRadius;

		eRadius.x = centre.x + (unitSphere.x * radiusVector.x);
		eRadius.y = centre.y + (unitSphere.y * radiusVector.y);
		eRadius.z = centre.z + (unitSphere.z * radiusVector.z);

		float ellip = distance(eRadius,centre);
		
		// Now, we want to calculate the distance between the closest point and the center
		float d = distance(point, centre);

		if(d < ellip)
		{
			float distanceOver = d - ellip;
			ip = normal * distanceOver;
			return true;
		}
			
	}
	
	//no intersection
	return false;
}
//checks if sphere is intersecting a polygon
bool spherePolygonCollision(SVector3 polygon[], 
							SVector3 centre, int vertexCount,float radius,SVector3 &ip,bool edges,SVector3 radiusVector,bool justOffset){

	SVector3 normal = getNormal(polygon);

	// This will store the distance our sphere is from the plane
	float distance = 0.0f;

	//determin if the sphere is BEHIND,INFRONT or INTERSECTING the plane
	int classification = classifySphere(centre, normal, polygon[0], radius, distance);

	//if plane is intersected make further checks
	if(classification == INTERSECTS) 
	{
		//offset the sphere so that the centre is offset from the polygon
		//by distance
		SVector3 offset = normal * distance;

		//a point that lies on the plane
		SVector3 pos = centre - offset;

		//ellipsoid radius checking ellipsoid space
		SVector3 unitSphere = pos - centre;

		unitSphere = normalize(unitSphere);

		SVector3 eRadius;

		eRadius.x = centre.x + (unitSphere.x * radiusVector.x);
		eRadius.y = centre.y + (unitSphere.y * radiusVector.y);
		eRadius.z = centre.z + (unitSphere.z * radiusVector.z);

		double ellip = sqrt((eRadius.x - centre.x) * (eRadius.x - centre.x) + 
							(eRadius.y - centre.y) * (eRadius.y - centre.y) + 
							(eRadius.z - centre.z) * (eRadius.z - centre.z));

		double intersec = sqrt((pos.x - centre.x) * (pos.x - centre.x) + 
							(pos.y - centre.y) * (pos.y - centre.y) + 
							(pos.z - centre.z) * (pos.z - centre.z));


		//check if its iniside the poly
		if(insidePolygon(pos,polygon,vertexCount))
		{
			if(intersec <= ellip) 
			{
				float distanceOver = intersec - ellip;
				ip = normal * distanceOver;
				
				if(justOffset)
				{
					eRadius.x = centre.x;
					eRadius.y = centre.y + radiusVector.y;
					eRadius.z = centre.z;

					SVector3 upLine[2];

					upLine[0].x = eRadius.x;
					upLine[0].y = eRadius.y;
					upLine[0].z = eRadius.z;

					upLine[1].x = eRadius.x;
					upLine[1].y = eRadius.y + 1000;
					upLine[1].z = eRadius.z;

					SVector3 slideTo;

					if(intersectedPolygon(polygon,upLine,vertexCount,slideTo))
					{
						ip.y = slideTo.y - eRadius.y;
					}

				}

				return true;
			}

		}
		else
		{
			//check collision with the edge of the poly if wanted
			if(edges)
			{
				if(edgeSphereCollision(centre, polygon,vertexCount,radius,ip,distance,radiusVector))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool polyIntersectingBox(SVector3 box[], SVector3 poly[], int vertexCount){

	//check if any of the vertices are inside the box
	for(int i = 0; i < vertexCount; i++)
	{
		if((poly[i].x > box[0].x) && (poly[i].x < box[1].x)
		&& (poly[i].y > box[0].y) && (poly[i].y < box[1].y)
		&& (poly[i].z > box[0].z) && (poly[i].z < box[1].z))
		{ 
			return true ;
		}
	}

	SVector3 line[12][2];

	line[0][0] = SVector3(box[0].x,box[0].y,box[0].z);
	line[0][1] = SVector3(box[0].x,box[0].y,box[1].z);

	line[1][0] = SVector3(box[0].x,box[0].y,box[0].z);
	line[1][1] = SVector3(box[0].x,box[1].y,box[0].z);

	line[2][0] = SVector3(box[0].x,box[0].y,box[0].z);
	line[2][1] = SVector3(box[1].x,box[0].y,box[0].z);

	line[3][0] = SVector3(box[1].x,box[1].y,box[1].z);
	line[3][1] = SVector3(box[0].x,box[1].y,box[1].z);

	line[4][0] = SVector3(box[1].x,box[1].y,box[1].z);
	line[4][1] = SVector3(box[1].x,box[0].y,box[1].z);

	line[5][0] = SVector3(box[1].x,box[1].y,box[1].z);
	line[5][1] = SVector3(box[1].x,box[1].y,box[0].z);

	line[6][0] = SVector3(box[0].x,box[0].y,box[1].z);
	line[6][1] = SVector3(box[0].x,box[1].y,box[1].z);
	
	line[7][0] = SVector3(box[0].x,box[0].y,box[1].z);
	line[7][1] = SVector3(box[1].x,box[0].y,box[1].z);

	line[8][0] = SVector3(box[0].x,box[1].y,box[0].z);
	line[8][1] = SVector3(box[0].x,box[1].y,box[1].z);

	line[9][0] = SVector3(box[0].x,box[1].y,box[0].z);
	line[9][1] = SVector3(box[1].x,box[1].y,box[0].z);

	line[10][0] = SVector3(box[1].x,box[1].y,box[0].z);
	line[10][1] = SVector3(box[1].x,box[0].y,box[0].z);

	line[11][0] = SVector3(box[1].x,box[0].y,box[0].z);
	line[11][1] = SVector3(box[1].x,box[0].y,box[1].z);

	float distance = 0;

	SVector3 normal = SVector3(0,0,0);

	for(int i = 0; i < 12; i ++)
	{
		if(intersectedPlane(poly,line[i],normal,distance))
		{
			SVector3 intersection = intersectionPoint(normal,line[i],distance);

			if(insidePolygon(intersection,poly,vertexCount)) return true;
		}
	}

	return false;
}

SVector3 rotateAboutY(SVector3 vertex, SVector3 scale, SVector3 pos, int angle){

	vertex.x = vertex.x * scale.x;
	vertex.y = vertex.y * scale.y;
	vertex.z = vertex.z * scale.z;

	float matrix[3][4];

	//convert the angle to radians
	float angleRad = (float) angle / 180 * PI;

	//row 1
	matrix[0][0] = cos(angleRad) * vertex.x;
	matrix[0][1] = 0;
	matrix[0][2] = -sin(angleRad) * vertex.z;
	matrix[0][3] = pos.x;

	//row 2
	matrix[1][0] = 0;
	matrix[1][1] = 1 * vertex.y;
	matrix[1][2] = 0;
	matrix[1][3] = pos.y;

	//row 3
	matrix[2][0] = sin(angleRad) * vertex.x;
	matrix[2][1] = 0;
	matrix[2][2] = cos(angleRad) * vertex.z;
	matrix[2][3] = pos.z;

	//add them up
	vertex.x = matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[0][3];
	vertex.y = matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[1][3];
	vertex.z = matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3];

	return vertex;

}

void billboardBegin(){

	float bbview[16];
	int i,j;

	//save MODELVIEW
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , bbview);

	//billboard it
	for( i=0; i<3; i+=2 ) 
	    for( j=0; j<3; j++ ) {
		if ( i==j )
		    bbview[i*4+j] = 1.0;
		else
		    bbview[i*4+j] = 0.0;
	    }
	// set the modelview matrix
	glLoadMatrixf(bbview);

}

void billboardEnd(){

	//pop back to normal matrix
	glPopMatrix();
}

void billboardSphericalBegin() {
	
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
	    for( j=0; j<3; j++ ) {
		if ( i==j )
		    modelview[i*4+j] = 1.0;
		else
		    modelview[i*4+j] = 0.0;
	    }

	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}

void transformPointByMatrix(SVector3 &point, float matrix[]){

	point.x = matrix[0] + matrix[4] + matrix[8] + matrix[12];  
	point.y = matrix[1] + matrix[5] + matrix[9] + matrix[13]; 
	point.z = matrix[2] + matrix[6] + matrix[10] + matrix[14]; 

}

void getPointFromNewMatrix(SVector3 &point,float matrix[],float x, float y, float z,float xrot,float yrot,float zrot){
	
	float xrotrad = (xrot / 180 * PI);

	matrix[12] += x;
	matrix[13] += y - (xrot/3);
	matrix[14] += z;

	point.x = matrix[0] + matrix[4] + matrix[8] + matrix[12];  
	point.y = matrix[1] + matrix[5] + matrix[9] + matrix[13]; 
	point.z = matrix[2] + matrix[6] + matrix[10] + matrix[14]; 

}

void multiplyPointByMatrix(SVector3 muzPosition,float matrix[]){

	float buffer[16];

	buffer[0] = matrix[0] * muzPosition.x;
	buffer[1] = matrix[1] * muzPosition.x;
	buffer[2] = matrix[2] * muzPosition.x;

	buffer[4] = matrix[4] * muzPosition.y;
	buffer[5] = matrix[5] * muzPosition.y;
	buffer[6] = matrix[6] * muzPosition.y;

	buffer[8] = matrix[8] * muzPosition.z;
	buffer[9] = matrix[9] * muzPosition.z;
	buffer[10] = matrix[10] * muzPosition.z;
	
	buffer[12] = matrix[12];
	buffer[13] = matrix[13];
	buffer[14] = matrix[14];

	muzPosition.x = buffer[0] + buffer[4] + buffer[8] + buffer[12];
	muzPosition.y = buffer[1] + buffer[3] + buffer[9] + buffer[13];
	muzPosition.z = buffer[2] + buffer[5] + buffer[10] + buffer[14];
}

SVector3 rotateAndScale(SVector3 vertex, SVector3 scale,int angle){

	float matrix[3][4];

	//convert the angle to radians
	float angleRad = (float) angle / 180 * PI;

	//scale
	//vertex.x = vertex.x * scale.x;
	//vertex.y = vertex.y * scale.y;
	//vertex.z = vertex.z * scale.z;

	//row 1
	matrix[0][0] = 1 * vertex.x;
	matrix[0][1] = 0;
	matrix[0][2] = 0;
	matrix[0][3] = 0;

	//row 2
	matrix[1][0] = 0;
	matrix[1][1] = cos(angleRad) * vertex.y;
	matrix[1][2] = sin(angleRad) * vertex.z;
	matrix[1][3] = 0;

	//row 3
	matrix[2][0] = 0;
	matrix[2][1] = -sin(angleRad) * vertex.y;
	matrix[2][2] = cos(angleRad) * vertex.z;
	matrix[2][3] = 0;

	//add them up
	vertex.x = matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[0][3];
	vertex.y = matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[1][3];
	vertex.z = matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3];

	return vertex;
}

bool AABBVSAABB(SVector3 colliderPos,SVector3 colliderScale,SVector3 recieverPos,SVector3 recieverScale,SVector3 *offset)
{
	*offset = SVector3(0,0,0);

	if((colliderPos.x - colliderScale.x >= recieverPos.x - recieverScale.x && 
		colliderPos.x - colliderScale.x <= recieverPos.x + recieverScale.x))
	{
		float contolPoint = colliderPos.x - colliderScale.x;
		float comparisonPoint = recieverPos.x + recieverScale.x;
		offset->x = comparisonPoint - contolPoint + 1;
	}
	else if((colliderPos.x + colliderScale.x >= recieverPos.x - recieverScale.x && 
		colliderPos.x + colliderScale.x <= recieverPos.x + recieverScale.x))
	{
		float contolPoint = colliderPos.x + colliderScale.x;
		float comparisonPoint = recieverPos.x + recieverScale.x;
		offset->x = comparisonPoint - contolPoint - 1;
	}
	else
	{
		*offset = SVector3(0,0,0);
		return true;
	}

	if((colliderPos.y - colliderScale.y >= recieverPos.y - recieverScale.y && 
		colliderPos.y - colliderScale.y <= recieverPos.y + recieverScale.y))
	{
		float contolPoint = colliderPos.y - colliderScale.y;
		float comparisonPoint = recieverPos.y + recieverScale.y;
		offset->y = comparisonPoint - contolPoint;
	}
	else if((colliderPos.y + colliderScale.y >= recieverPos.y - recieverScale.y && 
		colliderPos.y + colliderScale.y <= recieverPos.y + recieverScale.y))
	{
		float contolPoint = colliderPos.y + colliderScale.y;
		float comparisonPoint = recieverPos.y + recieverScale.y;
		offset->y = comparisonPoint - contolPoint;
	}
	else
	{
		*offset = SVector3(0,0,0);
		return true;
	}

	if((colliderPos.z - colliderScale.z >= recieverPos.z - recieverScale.z && 
		colliderPos.z - colliderScale.z <= recieverPos.z + recieverScale.z))
	{
		float contolPoint = colliderPos.z - colliderScale.z;
		float comparisonPoint = recieverPos.z + recieverScale.z;
		offset->z = comparisonPoint - contolPoint + 1;
	}
	else if((colliderPos.z + colliderScale.z >= recieverPos.z - recieverScale.z && 
		colliderPos.z + colliderScale.z <= recieverPos.z + recieverScale.z))
	{
		float contolPoint = colliderPos.z + colliderScale.z;
		float comparisonPoint = recieverPos.z + recieverScale.z;
		offset->z = comparisonPoint - contolPoint - 1;
	}
	else
	{
		*offset = SVector3(0,0,0);
		return true;
	}

	return false;
}

SMatrix4x4 matrixMultiplication(SMatrix4x4 lhs,SMatrix4x4 rhs)
{
	SMatrix4x4 result;

	result.entries[0] =
	lhs.entries[0]*rhs.entries[0]+
	lhs.entries[4]*rhs.entries[1]+
	lhs.entries[8]*rhs.entries[2]+
	lhs.entries[12]*rhs.entries[3];

	result.entries[1] =
	lhs.entries[1]*rhs.entries[0]+
	lhs.entries[5]*rhs.entries[1]+
	lhs.entries[9]*rhs.entries[2]+
	lhs.entries[13]*rhs.entries[3];

	result.entries[2] =
	lhs.entries[2]*rhs.entries[0]+
	lhs.entries[6]*rhs.entries[1]+
	lhs.entries[10]*rhs.entries[2]+
	lhs.entries[14]*rhs.entries[3];

	result.entries[3] =
	lhs.entries[3]*rhs.entries[0]+
	lhs.entries[7]*rhs.entries[1]+
	lhs.entries[11]*rhs.entries[2]+
	lhs.entries[15]*rhs.entries[3];

	result.entries[4] =
	lhs.entries[0]*rhs.entries[4]+
	lhs.entries[4]*rhs.entries[5]+
	lhs.entries[8]*rhs.entries[6]+
	lhs.entries[12]*rhs.entries[7];

	result.entries[5] =
	lhs.entries[1]*rhs.entries[4]+
	lhs.entries[5]*rhs.entries[5]+
	lhs.entries[9]*rhs.entries[6]+
	lhs.entries[13]*rhs.entries[7];

	result.entries[6] =
	lhs.entries[2]*rhs.entries[4]+
	lhs.entries[6]*rhs.entries[5]+
	lhs.entries[10]*rhs.entries[6]+
	lhs.entries[14]*rhs.entries[7];

	result.entries[7] =
	lhs.entries[3]*rhs.entries[4]+
	lhs.entries[7]*rhs.entries[5]+
	lhs.entries[11]*rhs.entries[6]+
	lhs.entries[15]*rhs.entries[7];

	result.entries[8] =
	lhs.entries[0]*rhs.entries[8]+
	lhs.entries[4]*rhs.entries[9]+
	lhs.entries[8]*rhs.entries[10]+
	lhs.entries[12]*rhs.entries[11];

	result.entries[9] =
	lhs.entries[1]*rhs.entries[8]+
	lhs.entries[5]*rhs.entries[9]+
	lhs.entries[9]*rhs.entries[10]+
	lhs.entries[13]*rhs.entries[11];

	result.entries[10] =
	lhs.entries[2]*rhs.entries[8]+
	lhs.entries[6]*rhs.entries[9]+
	lhs.entries[10]*rhs.entries[10]+
	lhs.entries[14]*rhs.entries[11];

	result.entries[11] =
	lhs.entries[3]*rhs.entries[8]+
	lhs.entries[7]*rhs.entries[9]+
	lhs.entries[11]*rhs.entries[10]+
	lhs.entries[15]*rhs.entries[11];

	result.entries[12] =
	lhs.entries[0]*rhs.entries[12]+
	lhs.entries[4]*rhs.entries[13]+
	lhs.entries[8]*rhs.entries[14]+
	lhs.entries[12]*rhs.entries[15];

	result.entries[13] =
	lhs.entries[1]*rhs.entries[12]+
	lhs.entries[5]*rhs.entries[13]+
	lhs.entries[9]*rhs.entries[14]+
	lhs.entries[13]*rhs.entries[15];

	result.entries[14] =
	lhs.entries[2]*rhs.entries[12]+
	lhs.entries[6]*rhs.entries[13]+
	lhs.entries[10]*rhs.entries[14]+
	lhs.entries[14]*rhs.entries[15];

	result.entries[15] =
	lhs.entries[3]*rhs.entries[12]+
	lhs.entries[7]*rhs.entries[13]+
	lhs.entries[11]*rhs.entries[14]+
	lhs.entries[15]*rhs.entries[15];

	return result; 
}

SMatrix4x4 matrixAddition(SMatrix4x4 lhs,SMatrix4x4 rhs)
{
	SMatrix4x4 result;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			int index = (i * 4) + j;

			result.entries[index] = lhs.entries[index] + rhs.entries[index];
		}
	}

	return result;

}


void SMatrix4x4::output()
{
	std::ofstream fout;
	fout.open("mats.txt");

	for(int i = 0; i < 16; i++)
	{
		fout << entries[i] << " , ";

		if(i == 3 || i == 7 || i == 11) fout << "\n";
	}
}

SVector3 rotateY(SVector3 point, SVector3 center, float angle)
{
	float matrix[4][4];

	float angleRad = degreesToRadians(angle);

	SVector3 pointFromOrigin = point - center;

	//row 1
	matrix[0][0] = cos(angleRad) * pointFromOrigin.x;
	matrix[0][1] = 0;
	matrix[0][2] = -sin(angleRad) * pointFromOrigin.z;
	matrix[0][3] = center.x;

	//row 2
	matrix[1][0] = 0;
	matrix[1][1] = 1 * pointFromOrigin.y;
	matrix[1][2] = 0;
	matrix[1][3] = center.y;

	//row 3
	matrix[2][0] = sin(angleRad) * pointFromOrigin.x;
	matrix[2][1] = 0;
	matrix[2][2] = cos(angleRad) * pointFromOrigin.z;
	matrix[2][3] = center.z;

	//add them up
	point.x = matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[0][3];
	point.y = matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[1][3];
	point.z = matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3];

	return point;
}

SVector3 getPlaneFromBox(int id,SVector3 pos,SVector3 scale,SVector3 orientation)
{
	//first axis, first box;
	SVector3 plane;
	SVector3 pointOne;
	SVector3 pointTwo;

	if(id == 0)
	{
		//X PLANE (in AABB)
		pointOne = SVector3
		(
			pos.x + scale.x,
			pos.y + scale.y,
			pos.z + scale.z
		);
		
		pointTwo = SVector3
		(
			pos.x - scale.x,
			pos.y + scale.y,
			pos.z + scale.z
		);
	}
	else if(id == 1)
	{
		//Y PLANE (in AABB)
		pointOne = SVector3
		(
			pos.x + scale.x,
			pos.y + scale.y,
			pos.z + scale.z
		);
		
		pointTwo = SVector3
		(
			pos.x + scale.x,
			pos.y - scale.y,
			pos.z + scale.z
		);
	}
	else
	{
		//Z PLANE (in AABB)
		pointOne = SVector3
		(
			pos.x + scale.x,
			pos.y + scale.y,
			pos.z + scale.z
		);
		
		pointTwo = SVector3
		(
			pos.x + scale.x,
			pos.y + scale.y,
			pos.z - scale.z
		);
	}

	pointOne = rotateY(pointOne, pos, -orientation.y);
	pointTwo = rotateY(pointTwo, pos, -orientation.y);

	plane = pointOne - pointTwo;

	return plane;
}

void getOBBRegion(SVector3 pos,SVector3 scale,SVector3 orientation, SVector3 plane, float *min,float *max)
{
	float projections[8];
	SVector3 vertices[8];

	vertices[0] = SVector3
	(
		pos.x + scale.x,
		pos.y + scale.y,
		pos.z + scale.z
	);
	vertices[1] = SVector3
	(
		pos.x - scale.x,
		pos.y - scale.y,
		pos.z - scale.z
	);
	vertices[2] = SVector3
	(
		pos.x + scale.x,
		pos.y - scale.y,
		pos.z + scale.z
	);
	vertices[3] = SVector3
	(
		pos.x - scale.x,
		pos.y + scale.y,
		pos.z - scale.z
	);
	vertices[4] = SVector3
	(
		pos.x - scale.x,
		pos.y + scale.y,
		pos.z + scale.z
	);
	vertices[5] = SVector3
	(
		pos.x + scale.x,
		pos.y + scale.y,
		pos.z - scale.z
	);
	vertices[6] = SVector3
	(
		pos.x - scale.x,
		pos.y - scale.y,
		pos.z + scale.z
	);
	vertices[7] = SVector3
	(
		pos.x + scale.x,
		pos.y - scale.y,
		pos.z - scale.z
	);

	for(int i = 0; i < 8; i++)
	{
		SVector3 projectionPoint = rotateY(vertices[i],pos,-orientation.y);
		projections[i] = dot(plane,projectionPoint);
	}

	*min = projections[0];
	*max = projections[0];

	for(int i = 0; i < 8; i++)
	{
		if(projections[i] < *min) *min = projections[i];
		else if(projections[i] > *max) *max = projections[i];
	}
}


bool OBBVSOBB(SVector3 colliderPos,SVector3 colliderScale,SVector3 colliderOrientation,
			  SVector3 recieverPos,SVector3 recieverScale,SVector3 recieverOrientation)
{

	SVector3 box1Axis[3];

	for(int i = 0; i < 3; i++)
	{
		box1Axis[i] = normalize(getPlaneFromBox(i,colliderPos,colliderScale,colliderOrientation));

		float min1,max1;
		float min2,max2;

		getOBBRegion(colliderPos,colliderScale,colliderOrientation,box1Axis[i],&min1,&max1);
		getOBBRegion(recieverPos,recieverScale,recieverOrientation,box1Axis[i],&min2,&max2);

		glColor3f(0,1,0);

		if
		(
			((min1 >= min2) && (min1 <= max2)) ||
			((min2 >= min1) && (min2 <= max1)) ||
			((max1 <= max2) && (max1 >= min2)) ||
			((max2 <= max1) && (max2 >= min1))
		)
		{
			glColor3f(1,0,0);
		}
		else
		{
			return false;
		}


		/*SVector3 line[2];

		line[0] = plane[i] * -1000;
		line[1] = plane[i] * 1000;

		glColor3f(1,0,1);
		glLineWidth(4);

		glBegin(GL_LINES);
			glVertex3f(line[0].x,line[0].y,line[0].z);
			glVertex3f(line[1].x,line[1].y,line[1].z);
		glEnd();

		SVector3 projPos;

		projPos = plane[i] * min2;

		glPushMatrix();
		glTranslated(projPos.x,projPos.y,projPos.z);
		cube();
		glPopMatrix();

		projPos = plane[i] * max2;

		glPushMatrix();
		glTranslated(projPos.x,projPos.y,projPos.z);
		cube();
		glPopMatrix();

		projPos = plane[i] * min1;

		glPushMatrix();
		glTranslated(projPos.x,projPos.y,projPos.z);
		cube();
		glPopMatrix();

		projPos = plane[i] * max1;

		glPushMatrix();
		glTranslated(projPos.x,projPos.y,projPos.z);
		cube();
		glPopMatrix();*/
	}

	SVector3 box2Axis[3];

	for(int i = 0; i < 3; i++)
	{
		box2Axis[i] = normalize(getPlaneFromBox(i,recieverPos,recieverScale,recieverOrientation));

		float min1,max1;
		float min2,max2;

		getOBBRegion(colliderPos,colliderScale,colliderOrientation,box2Axis[i],&min1,&max1);
		getOBBRegion(recieverPos,recieverScale,recieverOrientation,box2Axis[i],&min2,&max2);

		glColor3f(0,1,0);

		if
		(
			((min1 >= min2) && (min1 <= max2)) ||
			((min2 >= min1) && (min2 <= max1)) ||
			((max1 <= max2) && (max1 >= min2)) ||
			((max2 <= max1) && (max2 >= min1))
		)
		{
			glColor3f(1,0,0);
		}
		else
		{
			return false;
		}

	}

	SVector3 crossAxis[9];
	
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{	
			int index = (i * 3) + j;
			crossAxis[index] = cross(box2Axis[i],box1Axis[j]);
		}
	}

	for(int i = 0; i < 9; i++)
	{
		float min1,max1;
		float min2,max2;

		getOBBRegion(colliderPos,colliderScale,colliderOrientation,crossAxis[i],&min1,&max1);
		getOBBRegion(recieverPos,recieverScale,recieverOrientation,crossAxis[i],&min2,&max2);

		glColor3f(0,1,0);

		if
		(
			((min1 >= min2) && (min1 <= max2)) ||
			((min2 >= min1) && (min2 <= max1)) ||
			((max1 <= max2) && (max1 >= min2)) ||
			((max2 <= max1) && (max2 >= min1))
		)
		{
			glColor3f(1,0,0);
		}
		else
		{
			return false;
		}

	}

	return true;

}

bool SPHEREVSOBB(SVector3 boxPos,SVector3 boxScale,SVector3 boxOrientation, SVector3 spherePos, float sphereRadius,SVector3 offset)
{
	SVector3 boxAxis[3];

	SVector3 closestPointOnSphere = (spherePos + (normalize(boxPos - spherePos) * sphereRadius));

	bool collided = true;

	for(int i = 0; i < 3; i++)
	{
		boxAxis[i] = normalize(getPlaneFromBox(i,boxPos,boxScale,boxOrientation));

		float min,max,sphereProjection;

		getOBBRegion(boxPos,boxScale,boxOrientation,boxAxis[i],&min,&max);
		sphereProjection =  dot(boxAxis[i],closestPointOnSphere);

		glColor3f(0,0,1);

		if((sphereProjection >= min) && (sphereProjection <= max))
		{
		
		}
		else
		{
			collided = false;
		}

		/*glPushMatrix();
		quickTranslate(boxAxis[i] * sphereProjection);
		glColor3f(1,0.5,0);
		cube();
		glPopMatrix();

		glPushMatrix();
		quickTranslate(boxAxis[i] * min);
		glColor3f(0,0,1);
		cube();
		glPopMatrix();

		glPushMatrix();
		quickTranslate(boxAxis[i] * max);
		glColor3f(0,0,1);
		cube();
		glPopMatrix();*/
	}

	/*glColor3f(0,1,0);

	if(collided) glColor3f(1,0,1);

	glLineWidth(4);

	glBegin(GL_LINES);
		glVertex3f(spherePos.x,spherePos.y,spherePos.z);
		glVertex3f(boxPos.x,boxPos.y,boxPos.z);
	glEnd();

	glPushMatrix();
	quickTranslate(closestPointOnSphere);
	glColor3f(1,0.5,0);
	cube();
	glPopMatrix();*/

	return collided;

}

void SPHEREVSBOXTOPS(SVector3 boxPos,SVector3 boxScale,SVector3 boxOrientation,
					float radius,SVector3 ballPos,SVector3 *offsets)
{
	SVector3 offset;
	SVector3 radiusVector(radius,radius,radius);
	SVector3 spherePosition = ballPos;

	SVector3 poly[4];

	offset = SVector3(0,0,0);

	poly[0] = SVector3(boxPos.x + boxScale.x,boxPos.y + boxScale.y,boxPos.z - boxScale.z);
	poly[1] = SVector3(boxPos.x - boxScale.x,boxPos.y + boxScale.y,boxPos.z - boxScale.z);
	poly[2] = SVector3(boxPos.x - boxScale.x,boxPos.y + boxScale.y,boxPos.z + boxScale.z);
	poly[3] = SVector3(boxPos.x + boxScale.x,boxPos.y + boxScale.y,boxPos.z + boxScale.z);

	for(int i = 0; i < 4; i++)
	{
		poly[i] = rotateY(poly[i],boxPos,-boxOrientation.y);
	}

	spherePolygonCollision(poly,spherePosition,4,radius,offset,true,radiusVector,false);

	offsets[0] = offset;

}

void SPHEREVSBOXSIDES(SVector3 boxPos,SVector3 boxScale,SVector3 boxOrientation,
					float radius,SVector3 ballPos,SVector3 *offsets)
{
	SVector3 offset;

	SVector3 radiusVector(radius,radius,radius);
	SVector3 spherePosition = ballPos;

	SVector3 poly[4];

	offset = SVector3(0,0,0);

	poly[0] = SVector3(boxPos.x - boxScale.x,boxPos.y - boxScale.y,boxPos.z + boxScale.z);
	poly[1] = SVector3(boxPos.x + boxScale.x,boxPos.y - boxScale.y,boxPos.z + boxScale.z);
	poly[2] = SVector3(boxPos.x + boxScale.x,boxPos.y + boxScale.y,boxPos.z + boxScale.z);
	poly[3] = SVector3(boxPos.x - boxScale.x,boxPos.y + boxScale.y,boxPos.z + boxScale.z);

	for(int i = 0; i < 4; i++)
	{
		poly[i] = rotateY(poly[i],boxPos,-boxOrientation.y);
	}

	spherePolygonCollision(poly,spherePosition,4,radius,offset,true,radiusVector,false);

	offsets[0] = offset;

	offset = SVector3(0,0,0);

	poly[0] = SVector3(boxPos.x - boxScale.x,boxPos.y - boxScale.y,boxPos.z + boxScale.z);
	poly[1] = SVector3(boxPos.x - boxScale.x,boxPos.y + boxScale.y,boxPos.z + boxScale.z);
	poly[2] = SVector3(boxPos.x - boxScale.x,boxPos.y + boxScale.y,boxPos.z - boxScale.z);
	poly[3] = SVector3(boxPos.x - boxScale.x,boxPos.y - boxScale.y,boxPos.z - boxScale.z);

	for(int i = 0; i < 4; i++)
	{
		poly[i] = rotateY(poly[i],boxPos,-boxOrientation.y);
	}

	spherePolygonCollision(poly,spherePosition,4,radius,offset,true,radiusVector,false);

	offsets[1] = offset;

	offset = SVector3(0,0,0);

	poly[0] = SVector3(boxPos.x - boxScale.x,boxPos.y - boxScale.y,boxPos.z - boxScale.z);
	poly[1] = SVector3(boxPos.x - boxScale.x,boxPos.y + boxScale.y,boxPos.z - boxScale.z);
	poly[2] = SVector3(boxPos.x + boxScale.x,boxPos.y + boxScale.y,boxPos.z - boxScale.z);
	poly[3] = SVector3(boxPos.x + boxScale.x,boxPos.y - boxScale.y,boxPos.z - boxScale.z);

	for(int i = 0; i < 4; i++)
	{
		poly[i] = rotateY(poly[i],boxPos,-boxOrientation.y);
	}

	spherePolygonCollision(poly,spherePosition,4,radius,offset,true,radiusVector,false);

	offsets[2] = offset;

	offset = SVector3(0,0,0);

	poly[0] = SVector3(boxPos.x + boxScale.x,boxPos.y - boxScale.y,boxPos.z - boxScale.z);
	poly[1] = SVector3(boxPos.x + boxScale.x,boxPos.y + boxScale.y,boxPos.z - boxScale.z);
	poly[2] = SVector3(boxPos.x + boxScale.x,boxPos.y + boxScale.y,boxPos.z + boxScale.z);
	poly[3] = SVector3(boxPos.x + boxScale.x,boxPos.y - boxScale.y,boxPos.z + boxScale.z);

	for(int i = 0; i < 4; i++)
	{
		poly[i] = rotateY(poly[i],boxPos,-boxOrientation.y);
	}

	spherePolygonCollision(poly,spherePosition,4,radius,offset,true,radiusVector,false);

	offsets[3] = offset;
}

void buildShadowTextureMatrix(SMatrix4x4 projection, SMatrix4x4 view,SMatrix4x4 *textureDefault)
{
	//bias matrix transforms from world space
	//(-1 - 1) (four quadrants) to texture space (0 - 1) one quadrant
	SMatrix4x4 biasMatrix;

	biasMatrix.entries[0] = 0.5f;
	biasMatrix.entries[1] = 0.0f;
	biasMatrix.entries[2] = 0.0f;
	biasMatrix.entries[3] = 0.0f;

	biasMatrix.entries[4] = 0.0f;
	biasMatrix.entries[5] = 0.5f;
	biasMatrix.entries[6] = 0.0f;
	biasMatrix.entries[7] = 0.0f;

	biasMatrix.entries[8] = 0.0f;
	biasMatrix.entries[9] = 0.0f;
	biasMatrix.entries[10] = 0.5f;
	biasMatrix.entries[11] = 0.0f;

	biasMatrix.entries[12] = 0.5f;
	biasMatrix.entries[13] = 0.5f;
	biasMatrix.entries[14] = 0.5f;
	biasMatrix.entries[15] = 1.0f;

	//perform matrix multiplication with view, projection and bias to get a texture matrix
	//SMatrix4x4 lightViewProj = matrixMultiplication(biasMatrix,projection);
	//SMatrix4x4 textureMatrix = matrixMultiplication(lightViewProj,view);
	//Set up texture coordinate generation. using our texture matrix

	glMatrixMode(GL_TEXTURE);
	glGetFloatv(GL_TEXTURE_MATRIX,*textureDefault);
	glMultMatrixf(biasMatrix);
	glMultMatrixf(projection);
	glMultMatrixf(view);
	glMatrixMode(GL_MODELVIEW);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
}