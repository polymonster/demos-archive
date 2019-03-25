#ifndef __PS2MATRIX4x4_H__
#define __PS2MATRIX4x4_H__

#include "ps2vector4.h"

class Matrix4x4
{
public:
	Matrix4x4(void);
	Matrix4x4(const Matrix4x4 & rhs);
	Matrix4x4(float _elem[][4]);
	Matrix4x4(float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44);

	~Matrix4x4(void);

	void Translation(float X, float Y, float Z);
	void RotationX(float fAngle);
	void RotationY(float fAngle);
	void RotationZ(float fAngle);
	
	void DumpMatrix4x4(char * s);

	void LookAt(const Vector4 & vFrom,
				const Vector4 & vTo,
				const Vector4 & vUp);

	inline float &operator()( unsigned int Row, unsigned int Column )
	{ return elem[Row][Column]; }
	inline float const &operator()( unsigned int Row, unsigned int Column ) const
	{ return elem[Row][Column]; }

	float elem[4][4];

	static const Matrix4x4 IDENTITY;
	static const Matrix4x4 NULLMATRIX;
};

Matrix4x4 operator * ( const Matrix4x4 & M1,
					   const Matrix4x4 & M2 );

Vector4 operator * ( const Matrix4x4 & M,
					 const Vector4 & V );

inline Matrix4x4 operator - ( const Matrix4x4 & M )
{
	return Matrix4x4(-M(0,0),-M(0,1),-M(0,2),-M(0,3),
					 -M(1,0),-M(1,1),-M(1,2),-M(1,3),
					 -M(2,0),-M(2,1),-M(2,2),-M(2,3),
					 -M(3,0),-M(3,1),-M(3,2),-M(3,3));
}

inline Matrix4x4 operator - ( const Matrix4x4 & M1,
							  const Matrix4x4 & M2 )
{
	return Matrix4x4(M1(0,0)-M2(0,0),M1(0,1)-M2(0,1),M1(0,2)-M2(0,2),M1(0,3)-M2(0,3),
					 M1(1,0)-M2(1,0),M1(1,1)-M2(1,1),M1(1,2)-M2(1,2),M1(1,3)-M2(1,3),
					 M1(2,0)-M2(2,0),M1(2,1)-M2(2,1),M1(2,2)-M2(2,2),M1(2,3)-M2(2,3),
					 M1(3,0)-M2(3,0),M1(3,1)-M2(3,1),M1(3,2)-M2(3,2),M1(3,3)-M2(3,3));
}

inline Matrix4x4 operator + ( const Matrix4x4 & M1,
							  const Matrix4x4 & M2 )
{
	return Matrix4x4(M1(0,0)+M2(0,0),M1(0,1)+M2(0,1),M1(0,2)+M2(0,2),M1(0,3)+M2(0,3),
					 M1(1,0)+M2(1,0),M1(1,1)+M2(1,1),M1(1,2)+M2(1,2),M1(1,3)+M2(1,3),
					 M1(2,0)+M2(2,0),M1(2,1)+M2(2,1),M1(2,2)+M2(2,2),M1(2,3)+M2(2,3),
					 M1(3,0)+M2(3,0),M1(3,1)+M2(3,1),M1(3,2)+M2(3,2),M1(3,3)+M2(3,3));
}

inline Matrix4x4 operator * ( const Matrix4x4 & M,
							  const float & s )
{
	return Matrix4x4(M(0,0) * s,M(0,1) * s,M(0,2) * s,M(0,3) * s,
					 M(1,0) * s,M(1,1) * s,M(1,2) * s,M(1,3) * s,
					 M(2,0) * s,M(2,1) * s,M(2,2) * s,M(2,3) * s,
					 M(3,0) * s,M(3,1) * s,M(3,2) * s,M(3,3) * s);
}

inline Matrix4x4 operator * ( const float & s,
							  const Matrix4x4 & M )
{
	return Matrix4x4(M(0,0) * s,M(0,1) * s,M(0,2) * s,M(0,3) * s,
					 M(1,0) * s,M(1,1) * s,M(1,2) * s,M(1,3) * s,
					 M(2,0) * s,M(2,1) * s,M(2,2) * s,M(2,3) * s,
					 M(3,0) * s,M(3,1) * s,M(3,2) * s,M(3,3) * s);
}

inline Matrix4x4 Transpose( Matrix4x4 const & M )
{
	Matrix4x4 ret;

	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			ret(i,j) = M(j,i);
		}
	}

	return ret;
}

#endif
