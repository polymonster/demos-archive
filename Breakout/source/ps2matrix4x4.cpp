#include <memory.h>
#include <stdio.h>
#include "ps2maths.h"
#include "ps2matrix4x4.h"

const Matrix4x4 Matrix4x4::IDENTITY(1.0f,0.0f,0.0f,0.0f,
									0.0f,1.0f,0.0f,0.0f,
									0.0f,0.0f,1.0f,0.0f,
									0.0f,0.0f,0.0f,1.0f);

const Matrix4x4 Matrix4x4::NULLMATRIX(0.0f,0.0f,0.0f,0.0f,
									  0.0f,0.0f,0.0f,0.0f,
									  0.0f,0.0f,0.0f,0.0f,
									  0.0f,0.0f,0.0f,0.0f);

Matrix4x4::Matrix4x4(void)
{
}

Matrix4x4::~Matrix4x4(void)
{
}

Matrix4x4::Matrix4x4(const Matrix4x4 & rhs)
{
	memcpy(elem, rhs.elem, sizeof(float) * 16);
}

Matrix4x4::Matrix4x4(float _11, float _12, float _13, float _14,
					 float _21, float _22, float _23, float _24,
					 float _31, float _32, float _33, float _34,
					 float _41, float _42, float _43, float _44)
{
	elem[0][0] = _11;	elem[0][1] = _12;	elem[0][2] = _13;	elem[0][3] = _14;
	elem[1][0] = _21;	elem[1][1] = _22;	elem[1][2] = _23;	elem[1][3] = _24;
	elem[2][0] = _31;	elem[2][1] = _32;	elem[2][2] = _33;	elem[2][3] = _34;
	elem[3][0] = _41;	elem[3][1] = _42;	elem[3][2] = _43;	elem[3][3] = _44;
}

Matrix4x4::Matrix4x4(float _elem[][4])
{
	memcpy(elem, _elem, sizeof(float) * 16);
}

Matrix4x4 operator*( const Matrix4x4 &M1,
                     const Matrix4x4 &M2)
{
    Matrix4x4 ret;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            float Value = 0;
          
            for(int k = 0; k < 4; k++)
            {
                Value += M1(i,k) * M2(k,j);
            }

            ret(i,j) = Value;
        }
    }

    return ret;
}

Vector4 operator * ( const Matrix4x4 &M,
					 const Vector4 &V )
{
    Vector4 ret;

	ret.x = M(0,0) * V.x + M(1,0) * V.y + M(2,0) * V.z + M(3,0) * V.w;
	ret.y = M(0,1) * V.x + M(1,1) * V.y + M(2,1) * V.z + M(3,1) * V.w;
	ret.z = M(0,2) * V.x + M(1,2) * V.y + M(2,2) * V.z + M(3,2) * V.w;
	ret.w = M(0,3) * V.x + M(1,3) * V.y + M(2,3) * V.z + M(3,3) * V.w;

    return ret;
}

void Matrix4x4::Translation(float X, float Y, float Z)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);

	elem[3][0] = X;
	elem[3][1] = Y;
	elem[3][2] = Z;
}

void Matrix4x4::RotationX(float fAngle)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = Cos(fAngle);
	float s = Sin(fAngle);
	elem[1][1] = c;
	elem[1][2] = s;
	elem[2][1] = -s;
	elem[2][2] = c;
}

void Matrix4x4::RotationY(float fAngle)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = Cos(fAngle);
	float s = Sin(fAngle);
	elem[0][0] = c;
	elem[2][0] = s;
	elem[0][2] = -s;
	elem[2][2] = c;
}

void Matrix4x4::RotationZ(float fAngle)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = Cos(fAngle);
	float s = Sin(fAngle);
	elem[0][0] = c;
	elem[0][1] = s;
	elem[1][0] = -s;
	elem[1][1] = c;
}

void Matrix4x4::LookAt(const Vector4 & vFrom, const Vector4 & vTo, const Vector4 & vUp)
{
	Vector4 vZ = Normalise(vFrom - vTo);
	//vZ.DumpVector4("vZ");
	Vector4 vX = Normalise(vUp.Cross(vZ));
	//vX.DumpVector4("vX");
	Vector4 vY = vZ.Cross(vX);
	//vY.DumpVector4("vY");

    elem[0][0] = vX.x;	elem[0][1] = vY.x;	elem[0][2] = vZ.x;	elem[0][3] = 0;
	elem[1][0] = vX.y;	elem[1][1] = vY.y;	elem[1][2] = vZ.y;	elem[1][3] = 0;
	elem[2][0] = vX.z;	elem[2][1] = vY.z;	elem[2][2] = vZ.z;	elem[2][3] = 0;

	elem[3][0] = -vX.Dot3(vFrom);
	elem[3][1] = -vY.Dot3(vFrom);
	elem[3][2] = -vZ.Dot3(vFrom);
	elem[3][3] = 1;
}


void Matrix4x4::DumpMatrix4x4(char * s)
{
	printf("\n%s\n");
	printf("%f %f %f %f\n",   elem[0][0], elem[0][1], elem[0][2], elem[0][3]);
	printf("%f %f %f %f\n",   elem[1][0], elem[1][1], elem[1][2], elem[1][3]);
	printf("%f %f %f %f\n",   elem[2][0], elem[2][1], elem[2][2], elem[2][3]);
	printf("%f %f %f %f\n\n", elem[3][0], elem[3][1], elem[3][2], elem[3][3]);
}