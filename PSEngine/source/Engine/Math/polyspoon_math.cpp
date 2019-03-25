/*=========================================================*\
|	polyspoon_math.cpp - common maths operations
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ADixon
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "polyspoon_math.h"
#include "program.h"

/*=========================================================*\
|	deg_to_rad - converts degree angles into radians
\*=========================================================*/
float psmath::deg_to_rad(float degree_angle)
{
	return( degree_angle * _PI_OVER_180 );
}

/*=========================================================*\
|	rad_to_deg - converts radian angles into dagrees
\*=========================================================*/
float psmath::rad_to_deg(float radian_angle)
{
	return( radian_angle * _180_OVER_PI );
}

/*=========================================================*\
|	absolute - retruns always a positive float
\*=========================================================*/
float psmath::absolute(float value)
{
	if(value < 0.0f) value *= - 1;

	return value;
}

/*=========================================================*\
|	absolute_smallest_of - finds the smallest value of 2 params
\*=========================================================*/
float psmath::absolute_smallest_of(float value_1,float value_2)
{
	if(absolute(value_1) < absolute(value_2)) return value_1;
	else return value_2;
}

/*=========================================================*\
|	cross - retruns a Vector3f cross product result
\*=========================================================*/
Vector3f psmath::cross(Vector3f v1, Vector3f v2)
{
	Vector3f result;

	result.x = ((v1.y * v2.z) - (v1.z * v2.y));
	result.y = ((v1.z * v2.x) - (v1.x * v2.z));
	result.z = ((v1.x * v2.y) - (v1.y * v2.x));

	return result;
}

/*=========================================================*\
|	dot - retruns a float dot product result
\*=========================================================*/
float psmath::dot(Vector3f v1,Vector3f v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

/*=========================================================*\
|	perp - essentially 2d cross product to get the normal
|		 - of a 2d line well not the normal, but thats what
|		 - it looks like.
\*==========================================================*/
Vector2f psmath::perp(Vector2f v1, int hand)
{
	switch(hand)
	{
		case LEFT_HAND:
		{
			return Vector2f(v1.y,-v1.x);
		}
		break;

		case RIGHT_HAND:
		{
			return Vector2f(-v1.y,v1.x);
		}
		break;
	}

	//return left hand by default
	return Vector2f(v1.y,-v1.x); 
}

/*=========================================================*\
|	magnitude - retruns a float the length of a vector
\*=========================================================*/
float psmath::magnitude(Vector3f v)
{
	return (float) sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float psmath::magnitude(Vector2f v)
{
	return (float) sqrt((v.x * v.x) + (v.y * v.y));
}

/*=========================================================*\
|	distance - retruns a float the distance between 2 points
\*=========================================================*/
float psmath::distance(Vector3f p1, Vector3f p2)
{
	double d = sqrt( (p2.x - p1.x) * (p2.x - p1.x) + 
					 (p2.y - p1.y) * (p2.y - p1.y) + 
					 (p2.z - p1.z) * (p2.z - p1.z));

	return (float) d;
}

float psmath::distance(Vector2f p1, Vector2f p2)
{
	double d = sqrt( (p2.x - p1.x) * (p2.x - p1.x) + 
					 (p2.y - p1.y) * (p2.y - p1.y));

	return (float) d;
}

/*=========================================================*\
|	normalise - scale a vector to unit length
\*=========================================================*/
Vector3f psmath::normalise(Vector3f v){

	float mag = magnitude(v);

	if(mag != 0.0f)
	{
		v.x /= mag;
		v.y /= mag;
		v.z /= mag;
	}
	
	return v;
}

Vector2f psmath::normalise(Vector2f v){

	float mag = magnitude(v);

	if(mag != 0.0f)
	{
		v /= mag;
	}
	
	return v;
}

Vector3f psmath::project(Vector3f v)
{
	Matrix4 view, proj;

	float f_mv[16];
	float f_pm[16];

	glGetFloatv(GL_MODELVIEW_MATRIX,f_mv);
	view.set_matrix_from_gl(f_mv);

	glGetFloatv(GL_PROJECTION_MATRIX,f_pm);
	proj.set_matrix_from_gl(f_pm);

	Matrix4 res = proj * view;

	float depth = 1;

	Vector3f screen_space_coord = res.homogenous_multiply(v,&depth);
	screen_space_coord.x /= depth;
	screen_space_coord.y /= depth;
	screen_space_coord.z /= depth;

	Vector2f viewport_size = Vector2f(1024,768);

	screen_space_coord.x = (viewport_size.x * (screen_space_coord.x + 1)) * 0.5f;
	screen_space_coord.y = (viewport_size.y * (screen_space_coord.y + 1)) * 0.5f;
	screen_space_coord.z = (screen_space_coord.z + 1) * 0.5f;

	return screen_space_coord;
}

/*=========================================================*\
|	billboard_cylindrical_begin
|			- modifies x,z axis to always face the camera
\*=========================================================*/
void psmath::billboard_cylindrical_begin(){

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	//undo all rotations
	for( i=0; i<3; i+=2 ) 
	{
	    for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				 modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
	    }
	}

	// set the modelview matrix
	glLoadMatrixf(modelview);
}

/*=========================================================*\
|	billboard_spherical_begin
|		- modifies x,y and z axis to always face the camera
\*=========================================================*/
void psmath::billboard_spherical_begin() {
	
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	for( i=0; i<3; i++ ) 
	{
	    for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				 modelview[i*4+j] = 0.0;
	    }
	}

	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}

/*=========================================================*\
|	billboard_end - pop matrix wrapper for readability
\*=========================================================*/
void psmath::billboard_end(){ glPopMatrix(); }

/*=========================================================*\
|	get normal - gets the normal vector of a polygon
|	NB * the normal is inverted in here, was coming out wrong
|		 in collision tests, keep an eye on this
\*=========================================================*/
Vector3f psmath::get_normal(TRIANGLE t1)
{
	Vector3f v1 = t1.m_vertices[2] - t1.m_vertices[0];
	Vector3f v2 = t1.m_vertices[1] - t1.m_vertices[0];

	Vector3f normal = cross(v1, v2);

	normal = normalise(normal) *= -1;

	return normal;		
}

/*=========================================================*\
|	classify_sphere - classify a sphere to a plane
|					- based on simple distance calculation
\*=========================================================*/
int psmath::classify_sphere(SPHERE s1, Vector3f p, Vector3f normal, float *distance)
{
	float d = plane_distance(normal, p);

	*distance = (normal.x * s1.m_position.x + normal.y * s1.m_position.y + normal.z * s1.m_position.z + d);

	//if the distance is less than the radius and intersection occurs
	if(absolute(*distance) < s1.m_radius)
	{	
		return INTERSECTS;
	}
	else if(*distance >= s1.m_radius)
	{	
		return INFRONT;
	}
	
	//else the sphere is behind the plane
	return BEHIND;

}

/*=========================================================*\
|	plane_distance - find the distance from plane to a point
\*=========================================================*/
float psmath::plane_distance(Vector3f normal, Vector3f point)
{
	float distance = 0;

	//Use the plane equation to find the distance D
	//negative dot product between normal vector and point (p)
	distance = dot(normal,point) * -1; 

	return distance;	
}

/*=========================================================*\
|	angle_between_vectors 
|			- find the angle (radians) between 2 Vectors
\*=========================================================*/
float psmath::angle_between_vectors(Vector3f v1,Vector3f v2)
{
	//get the dot product
	float dp = dot(v1,v2);

	//get magnitude product
	double mp = magnitude(v1) * magnitude(v2);

	//get the angle between the 2 above (radians)
	float angle = (float) acos( dp / mp );

	//ensure angle is a number
	if(_isnan(angle))
	{
		return 0;
	}

	//return the angle in radians
	return angle;
}

/*=========================================================*\
|	closest_point_on_line 
|		- find the closest point on a line to another point
\*=========================================================*/
Vector3f psmath::closest_point_on_line(Vector3f l1, Vector3f l2, Vector3f p)
{
	//create a vector from line start to the point.
	Vector3f v1 = p - l1;

	//get the normalised direction vector of the line
    Vector3f v2 = normalise(l2 - l1);

	//use the distance formula to get the length of the line
    float d = distance(l1, l2);

	//using dot product project v1 onto v2
    float t = dot(v2, v1);

	//if the points at either end of the line
	//the point is before the line start
    if (t <= 0) 
	{
		return l1;
	}

	//the point is after the line end
    if (t >= d) 
	{
		return l2;
	}

	//otherwise the point is on the line

	//vector of length t and direction of the line
    Vector3f v3 = v2 * t;

	//to get the closest point simply add v3 to the starting point of the line
    Vector3f closest_point = l1 + v3;

	return closest_point;
}

/*=========================================================*\
|	closest_point_on_AABB3D 
|		- find the closest point on an AABB to another point
\*=========================================================*/
Vector3f psmath::closest_point_on_AABB3D(AABB3D b1, Vector3f p)
{
	//this could be optimised out
	Vector3f b1_extent_min = b1.m_position - b1.m_size;
	Vector3f b1_extent_max = b1.m_position + b1.m_size;

	if(p.x < b1_extent_min.x) p.x = b1_extent_min.x;
	else if(p.x > b1_extent_max.x) p.x = b1_extent_max.x;

	if(p.y < b1_extent_min.y) p.y = b1_extent_min.y;
	else if(p.y > b1_extent_max.y) p.y = b1_extent_max.y;

	if(p.z < b1_extent_min.z) p.z = b1_extent_min.z;
	else if(p.z > b1_extent_max.z) p.z = b1_extent_max.z;

	return p;
}

/*=========================================================*\
|	get_axes_from_OBB
|		- finds the 3 oriented axes of an OBB 
\*=========================================================*/
void psmath::get_axes_from_OBB(OBB3D b1, Vector3f *axes)
{
	//make some vertices for each axis
	Vector3f x_axis[2];
	Vector3f y_axis[2];
	Vector3f z_axis[2];

	x_axis[0] = Vector3f(-1,1,1);
	x_axis[1] = Vector3f( 1,1,1);

	y_axis[0] = Vector3f( 1,-1,1);
	y_axis[1] = Vector3f( 1, 1,1);

	z_axis[0] = Vector3f( 1,1,-1);
	z_axis[1] = Vector3f( 1,1, 1);

	//orient the verts to the obb orientation
	for(int i = 0; i < 2; i++)
	{
		x_axis[i] = b1.m_orientation_matrix * x_axis[i];
		y_axis[i] = b1.m_orientation_matrix * y_axis[i];
		z_axis[i] = b1.m_orientation_matrix * z_axis[i];
	}

	//now take a vector between each axis verts to get the final
	//normalised axis
	axes[0] = normalise(x_axis[1] - x_axis[0]);
	axes[1] = normalise(y_axis[1] - y_axis[0]);
	axes[2] = normalise(z_axis[1] - z_axis[0]);
}

/*=========================================================*\
|	find_extents
|		- finds the min and max extents of a group of vertices
|		- on a given axis (return by pointers min and max)
|		- extent is returned as a float value from the
|		- dot product (the distance along an axis)
\*=========================================================*/
void psmath::find_extents(Vector3f axis, Vector3f *vertices, unsigned int vertex_count, float *min, float *max)
{
	float *projections = new float[vertex_count];

	for(unsigned int i = 0; i < vertex_count; i++)
	{
		projections[i] = dot(axis,vertices[i]);
	}

	*min = projections[0];
	*max = projections[0];

	for(unsigned int i = 0; i < vertex_count; i++)
	{
		if(projections[i] < *min) *min = projections[i];
		else if(projections[i] > *max) *max = projections[i];
	}

	delete projections;
}

void psmath::find_extents(Vector3f axis, Vector3fArray vertices, float *min, float *max)
{
	float *projections = new float[vertices.size()];

	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		projections[i] = dot(axis,vertices[i]);
	}

	*min = projections[0];
	*max = projections[0];

	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		if(projections[i] < *min) *min = projections[i];
		else if(projections[i] > *max) *max = projections[i];
	}

	delete projections;
}

/*=========================================================*\
|	find_extents
|		- finds the min and max extents of a group of vertices
|		- on a given axis (return by pointers min and max)
|		- extent is returned as a vector3f value of the position
|		- the extent is on the axis
\*=========================================================*/
void psmath::find_extents(Vector3f axis, Vector3f *vertices, unsigned int vertex_count, Vector3f *min_position, Vector3f *max_position)
{
	float min, max;

	find_extents(axis,vertices,vertex_count,&min,&max);

	*min_position = axis * min;
	*max_position = axis * max;
}

