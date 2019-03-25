/*=========================================================*\
|	polyspoon_math.h - main maths class
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ADixon
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _POLYSPOON_MATH_H
#define _POLYSPOON_MATH_H

/*======================== INCLUDES =======================*/

#include <math.h>
#include <float.h>
#include "vector.h"
#include "matrix.h"
#include "bounding_volumes.h"

/*======================== CONSTANTS ======================*/

#define	PI				3.1415926535897932f

#define	_PI_OVER_180	0.0174532925199433f
#define _180_OVER_PI	57.295779513082322f

typedef enum
{
	INTERSECTS = 0,
	BEHIND = 1,
	INFRONT = 2,

}e_classifications;

typedef enum
{
	LEFT_HAND = 0,
	RIGHT_HAND = 1,

}e_handedness;


const Vector3f unit_cube_vertices[8] =
{
	Vector3f(1,1,1),
	Vector3f(-1,-1,-1),
	Vector3f(-1,1,-1),
	Vector3f(1,-1,-1),
	Vector3f(-1,-1,1),
	Vector3f(1,1,-1),
	Vector3f(1,-1,1),
	Vector3f(-1,1,1)
};

/*========================= NAMESPACE =======================*/
namespace psmath
{
	//generic
	float absolute(float value);
	float absolute_smallest_of(float value_1, float value_2);

	//angles
	float deg_to_rad(float degree_angle);
	float rad_to_deg(float radian_angle);
	float angle_between_vectors(Vector3f v1, Vector3f v2);

	//vectors
	Vector3f cross(Vector3f v1, Vector3f v2);
	Vector2f perp(Vector2f v1, int hand);
	Vector3f normalise(Vector3f v);
	Vector2f normalise(Vector2f v);
	Vector3f project(Vector3f v);
	void get_axes_from_OBB(OBB3D b1, Vector3f *axes);
	
	float dot(Vector3f v1, Vector3f v2);
	float magnitude(Vector3f v);
	float magnitude(Vector2f v);
	float distance(Vector3f p1, Vector3f p2);
	float distance(Vector2f p1, Vector2f p2);

	//billboards
	void billboard_spherical_begin();
	void billboard_cylindrical_begin();
	void billboard_end();

	//polygons / triangles
	Vector3f get_normal(TRIANGLE t1);

	//planes
	int classify_sphere(SPHERE s1, Vector3f p, Vector3f normal, float *distance);
	float plane_distance(Vector3f normal, Vector3f point);

	//closest point
	Vector3f closest_point_on_line(Vector3f l1, Vector3f l2, Vector3f p);
	Vector3f closest_point_on_AABB3D(AABB3D b1, Vector3f p);
	void find_extents(Vector3f axis, Vector3f *vertices, unsigned int vertex_count, float *min, float *max);
	void find_extents(Vector3f axis, Vector3fArray vertices, float *min, float *max);
	void find_extents(Vector3f axis, Vector3f *vertices, unsigned int vertex_count, Vector3f *min_position, Vector3f *max_position);

	//bounding volume vs bounding volume tests
	bool SPHERE_vs_SPHERE(SPHERE *s1, SPHERE *s2);
	bool AA_ELLIPSOID_vs_SPHERE(AA_ELLIPSOID *e1, SPHERE *s1);
	bool AA_ELLIPSOID_vs_AA_ELLIPSOID(AA_ELLIPSOID *e1, AA_ELLIPSOID *e2);
	bool SPHERE_vs_TRIANGLE(SPHERE *s1, TRIANGLE *t1);
	bool AA_ELLIPSOID_vs_TRIANGLE(AA_ELLIPSOID *e1, TRIANGLE *t1);
	bool AABB3D_vs_AABB3D(AABB3D *b1, AABB3D *b2);
	bool AABB3D_vs_SPHERE(AABB3D *b1, SPHERE *s1);
	bool AABB3D_vs_AA_ELLIPSOID(AABB3D *b1, AA_ELLIPSOID *e1);
	bool OBB3D_vs_OBB3D(OBB3D *b1, OBB3D *b2);
	bool CONVEX_HULL_vs_CONVEX_HULL(CONVEX_HULL *h1, CONVEX_HULL *h2);

	//point tests
	bool POINT_inside_TRIANGLE(TRIANGLE t1, Vector3f p);
	bool POINT_inside_AA_ELLIPSOID(AA_ELLIPSOID e1, Vector3f p);
	bool POINT_inside_SPHERE(SPHERE s1, Vector3f p);
	bool POINT_inside_AABB3D(AABB3D b1, Vector3f p);

	//ray / line tests tests
	bool SPHERE_vs_LINE();
	bool SPHERE_vs_RAY();
	bool AA_ELLIPSOID_vs_LINE();
	bool AA_ELLIPSOID_vs_RAY();
};


/*================== EXTERNAL VARIABLES ===================*/


#endif //_POLYSPOON_MATH_H