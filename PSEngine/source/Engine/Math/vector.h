/*=========================================================*\
|	vector.h - vector classes and math
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ABleas  | ADixon
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _VECTOR_H
#define _VECTOR_H

/*======================== INCLUDES =======================*/

#include <math.h>
#include "console.h"

#include <vector>

/*======================== CONSTANTS ======================*/



/*========================= CLASSES =======================*/

/*=========================================================*\
|	Vector2i - 2D integer vector
\*=========================================================*/
class Vector2i
{
public:
	Vector2i()					: x( 0 ), y( 0 )	{ }
	Vector2i( int ax, int ay )	: x( ax ), y( ay )	{ }
	~Vector2i()					{ }

	const Vector2i	operator =	( const Vector2i &v )	{ x=v.x, y=v.y; return( *this ); }
	const bool		operator ==	( const Vector2i &v )	{ return( x==v.x && y==v.y ); }
	const bool		operator !=	( const Vector2i &v )	{ return( x!=v.x || y!=v.y ); }
	const Vector2i	operator +	( const Vector2i &v )	{ Vector2i result( x+v.x, y+v.y ); return( result ); }
	const Vector2i	operator -	( const Vector2i &v )	{ Vector2i result( x-v.x, y-v.y ); return( result ); }

	int			x, y;

};

/*=========================================================*\
|	Vector2f - 2D floating point vector
\*=========================================================*/
class Vector2f
{
public:
	Vector2f()					: x( 0 ), y( 0 )	{ }
	Vector2f( float ax, float ay )	: x( ax ), y( ay )	{ }
	~Vector2f()					{ }

	inline const Vector2f	operator =	( const Vector2f &v )	{ x=v.x, y=v.y; return( *this ); }
	inline const bool		operator ==	( const Vector2f &v )	{ return( x==v.x && y==v.y ); }
	inline const bool		operator !=	( const Vector2f &v )	{ return( x!=v.x || y!=v.y ); }
	inline const Vector2f	operator +	( const Vector2f &v )	{ Vector2f result( x+v.x, y+v.y ); return( result ); }
	inline const Vector2f	operator -	( const Vector2f &v )	{ Vector2f result( x-v.x, y-v.y ); return( result ); }
	inline const Vector2f  operator *  (const float &a    )	{ Vector2f result(x * a, y * a ); return ( result ); } 
	inline const Vector2f  operator /  (const float &a    ) 
	{ 
		if(a == 0.0f) g_console_p->dbgmsg("ps math warning : attempted Vector3f divide by 0\n"); return *this;

		float one_over_a = 1.0f / a;
		Vector2f result(x * one_over_a, y * one_over_a ); 
		return ( result ); 
	}

	inline const Vector2f &operator += (const Vector2f &v ) { x += v.x, y += v.y;  return ( *this ); } 
	inline const Vector2f &operator -= (const Vector2f &v ) { x -= v.x, y -= v.y;  return ( *this ); } 

	inline const Vector2f &operator *= (const float &a    ) { x *= a, y *= a;  return ( *this ); } 
	inline const Vector2f &operator /= (const float &a    ) 
	{ 
		if(a == 0.0f) g_console_p->dbgmsg("ps math warning : attempted Vector3f divide by 0\n"); return *this;

		float one_over_a = 1.0f / a;
		x *= one_over_a; y *= one_over_a;  
		return ( *this ); 
	} 

	float		x, y;

};

/*=========================================================*\
|	Vector3f - 3D floating point vector
\*=========================================================*/
class Vector3f
{

public:

	Vector3f()					: x( 0 ), y( 0 ), z( 0 ) { }
	Vector3f( float ax, float ay, float az ) : x( ax ), y( ay ), z( az ) { }
	~Vector3f()					{ }

	inline const Vector3f operator =  (const Vector3f &v ) {x = v.x, y = v.y, z = v.z; return (*this ); }
	inline const bool	   operator == (const Vector3f &v ) { return( x==v.x && y==v.y && z==v.z); }
	inline const bool	   operator != (const Vector3f &v ) { return( x!=v.x || y!=v.y || z!=v.z); }
	inline const Vector3f operator +  (const Vector3f &v ) { Vector3f result(x + v.x, y + v.y, z + v.z); return ( result ); } 
	inline const Vector3f operator -  (const Vector3f &v ) { Vector3f result(x - v.x, y - v.y, z - v.z); return ( result ); }
	inline const Vector3f operator *  (const float &a    ) { Vector3f result(x * a, y * a, z * a); return ( result ); } 
	inline const Vector3f operator *  (const Vector3f &v ) { Vector3f result(x * v.x, y * v.y, z * v.z); return ( result ); } 
	inline const Vector3f operator /  (const float &a    ) 
	{ 
		if(a == 0.0f) 
		{
			g_console_p->dbgmsg("ps math warning : attempted Vector3f divide by 0\n"); 
			return *this;
		}

		float one_over_a = 1.0f / a;
		Vector3f result(x * one_over_a, y * one_over_a, z * one_over_a); 
		return ( result ); 
	}

	inline const Vector3f operator / (const Vector3f &v ) 
	{
		if(v.x == 0.0f || v.y == 0.0f || v.z == 0.0f) 
		{
			g_console_p->dbgmsg("ps math warning : attempted Vector3f divide by 0\n"); return *this;
		}

		Vector3f result(x / v.x, y / v.y, z / v.z); 
		
		return ( result );
	}

	inline const Vector3f &operator += (const Vector3f &v ) { x += v.x, y += v.y, z += v.z;  return ( *this ); } 
	inline const Vector3f &operator -= (const Vector3f &v ) { x -= v.x, y -= v.y, z -= v.z ; return ( *this ); } 

	inline const Vector3f &operator *= (const float &a    ) { x *= a, y *= a, z *= a;  return ( *this ); } 
	inline const Vector3f &operator /= (const float &a    ) 
	{ 
		if(a == 0.0f) 
		{
			g_console_p->dbgmsg("ps math warning : attempted Vector3f divide by 0\n"); 
			return *this;
		}

		float one_over_a = 1.0f / a;
		x *= one_over_a; y *= one_over_a; z *= one_over_a;  return ( *this ); 
	} 

	inline void zero() { x = 0, y = 0, z = 0; }

	inline void normalise ()
	{
		float mag_sq = x * x + y * y + z * z;

		if(mag_sq > 0.0f)
		{
			float one_over_mag = 1.0f / sqrt(mag_sq);

			x *= one_over_mag;
			y *= one_over_mag;
			z *= one_over_mag;
		}
	}

	float x, y, z;
};

class Vector4f
{
public:

	float x, y, z, w;

	Vector4f()
	{

	}

	Vector4f(Vector3f vec3, float aw)
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;
		w = aw;
	}
};

/*======================= TYPEDEFS ========================*/

typedef std::vector< Vector3f > Vector3fArray;
typedef std::vector< Vector4f > Vector4fArray;
typedef std::vector< Vector2f > Vector2fArray;
typedef std::vector< Vector2i > Vector2iArray;

typedef std::vector< Vector3f* > Vector3fArray_p;
typedef std::vector< Vector2f* > Vector2fArray_p;
typedef std::vector< Vector2i* > Vector2iArray_p;

/*================== EXTERNAL VARIABLES ===================*/


#endif //_VECTOR_H
