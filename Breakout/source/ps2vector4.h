#ifndef __PS2VECTOR4_H__
#define __PS2VECTOR4_H__

#include "ps2maths.h"

class Vector4
{
public:
	// Constructors
	Vector4(void);
	Vector4(const float _x, const float _y, const float _z, const float _w);
	Vector4(const Vector4 & rhs);

	~Vector4(void);

	// Operations with other vectors
	Vector4 & operator+=(const Vector4 & rhs);
	Vector4 & operator-=(const Vector4 & rhs);

	// Special arithmetic
	float Dot3(const Vector4 & rhs) const;
	float Dot4(const Vector4 & rhs) const;
	Vector4 Cross(const Vector4 & rhs) const;

	Vector4 & operator*=(const float s);
	Vector4 & operator/=(const float s);

	bool operator==(const Vector4 & rhs) const;

	// Miscellaneous
	float Length() const;
	float LengthSqr() const;
	Vector4 Normalise();
	void NormaliseSelf();
	void DumpVector4(char * s);
	
	// Member data
	float x,y,z,w; 
};

inline Vector4 operator + (const Vector4 &v1,
						   const Vector4 &v2)
{
	return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, 1.0f);
}

inline Vector4 operator - (const Vector4 &v1,
						   const Vector4 &v2)
{
	return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, 1.0f);
}

inline Vector4 operator - (const Vector4 &v1)
{
	return Vector4(-v1.x, -v1.y, -v1.z, 1.0f);
}

inline Vector4 operator * (const Vector4 &v,
						   const float &s)
{
	return Vector4(v.x * s, v.y * s, v.z * s, 1.0f);
}

inline Vector4 operator * (const float & s,
						   const Vector4 &v)
{
	return Vector4(v.x * s, v.y * s, v.z * s, 1.0f);
}

inline Vector4 operator / (const Vector4 &v,
						   const float & s)
{
	return Vector4(v.x / s, v.y / s, v.z / s, 1.0f);
}

inline Vector4 CrossProduct (const Vector4 &v1,
							 const Vector4 &v2)
{
	return Vector4(v1.y * v2.z - v1.z * v2.y,
				   v1.z * v2.x - v1.x * v2.z,
				   v1.x * v2.y - v1.y * v2.x,
				   1.0f);
}

inline float DotProduct4(const Vector4 &v1,
						 const Vector4 &v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

inline float DotProduct3(const Vector4 &v1,
						 const Vector4 &v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline Vector4 Normalise (const Vector4 &v)
{
	return v / v.Length();
}

#endif
