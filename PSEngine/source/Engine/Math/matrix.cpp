/*=========================================================*\
|	matrix.cpp - matrix class
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ADixon
|				Date	:	08/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/


/*======================== INCLUDES =======================*/
#include "matrix.h"
#include "program.h"

/*=========================================================*\
|	Identity Matrix - Sets the current matrix to identity
\*=========================================================*/

void Matrix4::identity()
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

/*=========================================================*\
|	Operator * - Multiply 2 matrices
\*=========================================================*/
Matrix4 Matrix4::operator *(Matrix4 &b)
{
	Matrix4 result;

	result.m11 = m11 * b.m11 + m12 * b.m21 + m13 * b.m31 + m14 * b.m41;
	result.m12 = m11 * b.m12 + m12 * b.m22 + m13 * b.m32 + m14 * b.m42;
	result.m13 = m11 * b.m13 + m12 * b.m23 + m13 * b.m33 + m14 * b.m43;
	result.m14 = m11 * b.m14 + m12 * b.m24 + m13 * b.m34 + m14 * b.m44;

	result.m21 = m21 * b.m11 + m22 * b.m21 + m23 * b.m31 + m24 * b.m41;
	result.m22 = m21 * b.m12 + m22 * b.m22 + m23 * b.m32 + m24 * b.m42;
	result.m23 = m21 * b.m13 + m22 * b.m23 + m23 * b.m33 + m24 * b.m43;
	result.m24 = m21 * b.m14 + m22 * b.m24 + m23 * b.m34 + m24 * b.m44;

	result.m31 = m31 * b.m11 + m32 * b.m21 + m33 * b.m31 + m34 * b.m41;
	result.m32 = m31 * b.m12 + m32 * b.m22 + m33 * b.m32 + m34 * b.m42;
	result.m33 = m31 * b.m13 + m32 * b.m23 + m33 * b.m33 + m34 * b.m43;
	result.m34 = m31 * b.m14 + m32 * b.m24 + m33 * b.m34 + m34 * b.m44;

	result.m41 = m41 * b.m11 + m42 * b.m21 + m43 * b.m31 + m44 * b.m41;
	result.m42 = m41 * b.m12 + m42 * b.m22 + m43 * b.m32 + m44 * b.m42;
	result.m43 = m41 * b.m13 + m42 * b.m23 + m43 * b.m33 + m44 * b.m43;
	result.m44 = m41 * b.m14 + m42 * b.m24 + m43 * b.m34 + m44 * b.m44;

	return result;
}

/*=========================================================*\
|	Operator * - A Vector3 by Matrix
\*=========================================================*/
const Vector3f Matrix4::operator *(const Vector3f &v)
{	
	Vector3f result;

	result.x = v.x * m11 + v.y * m12 + v.z * m13 + m14;
	result.y = v.x * m21 + v.y * m22 + v.z * m23 + m24;
	result.z = v.x * m31 + v.y * m32 + v.z * m33 + m34;

	return result;
}

/*=========================================================*\
|	Operator * - A Vector3 by Matrix (hack to get w too!)
\*=========================================================*/
Vector3f Matrix4::homogenous_multiply(Vector3f v, float *w)
{
	Vector3f result;

	result.x = v.x * m11 + v.y * m12 + v.z * m13 + m14;
	result.y = v.x * m21 + v.y * m22 + v.z * m23 + m24;
	result.z = v.x * m31 + v.y * m32 + v.z * m33 + m34;

	float w_result = v.x * m41 + v.y * m42 + v.z * m43 + *w * m44;

	*w = w_result;

	return result;

}

/*=========================================================*\
|	create_translation - creates a translation matrix of t
\*=========================================================*/
void Matrix4::create_translation(Vector3f t)
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = t.x;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = t.y;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = t.z;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

/*=========================================================*\
|	create_cardinal_rotation 
|				- rotate around a global axis by theta
\*=========================================================*/
void Matrix4::create_cardinal_rotation(int axis, float theta)
{
	//get sin / cos theta once
	float theta_rad = psmath::deg_to_rad(theta);
	float sin_theta = sin(theta_rad);
	float cos_theta = cos(theta_rad);

	switch(axis)
	{
		case X_AXIS:
		{
			m11 = 1.0f; m12 = 0.0f;		  m13 = 0.0f;		m14 = 0.0f;
			m21 = 0.0f; m22 = cos_theta;  m23 = -sin_theta;	m24 = 0.0f;
			m31 = 0.0f; m32 = sin_theta;  m33 = cos_theta;  m34 = 0.0f;
			m41 = 0.0f; m42 = 0.0f;		  m43 = 0.0f;		m44 = 1.0f;

		}
		break;

		case Y_AXIS:
		{
			m11 = cos_theta; m12 = 0.0f; m13 = sin_theta; m14 = 0.0f;
			m21 = 0.0f;		 m22 = 1.0f; m23 = 0.0f;	   m24 = 0.0f;
			m31 = -sin_theta; m32 = 0.0f; m33 = cos_theta;  m34 = 0.0f;
			m41 = 0.0f;		 m42 = 0.0f; m43 = 0.0f;	   m44 = 1.0f;

		}
		break;

		case Z_AXIS:
		{
			m11 = cos_theta;  m12 = -sin_theta;  m13 = 0.0f; m14 = 0.0f;
			m21 = sin_theta; m22 = cos_theta;  m23 = 0.0f; m24 = 0.0f;
			m31 = 0.0f;		  m32 = 0.0f;		m33 = 1.0f; m34 = 0.0f;
			m41 = 0.0f;		  m42 = 0.0f;		m43 = 0.0f; m44 = 1.0f;

		}
		break;
	}
}


/*=========================================================*\
|	create_scale 
|				- creates a scale matrix of vector s
\*=========================================================*/
void Matrix4::create_scale(Vector3f s)
{
	m11 = s.x ; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f; m22 = s.y ; m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = s.z ; m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

/*=========================================================*\
|	gl_compliant_matrix 
|				- returns a matrix that is suitible for use
|				- in OpenGL calls, i.e glMultMatrixf()
\*=========================================================*/
void Matrix4::gl_compliant_matrix(float *entries)
{
	entries[0]  = m11; entries[4]  = m12; entries[8]  = m13; entries[12] = m14;
	entries[1]  = m21; entries[5]  = m22; entries[9]  = m23; entries[13] = m24;
	entries[2]  = m31; entries[6]  = m32; entries[10] = m33; entries[14] = m34;
	entries[3]  = m41; entries[7]  = m42; entries[11] = m43; entries[15] = m44;
}

/*=========================================================*\
|	set_matrix_from_gl 
|				- sets the matrix from one obtained using
|				- OpenGL i.e glGetFloatv(GL_MODELVIEW)
\*=========================================================*/
void Matrix4::set_matrix_from_gl(float *entries)
{
	m11 = entries[0]; m12 = entries[4]; m13 = entries[8];  m14 = entries[12];
	m21 = entries[1]; m22 = entries[5]; m23 = entries[9];  m24 = entries[13];
	m31 = entries[2]; m32 = entries[6]; m33 = entries[10]; m34 = entries[14];
	m41 = entries[3]; m42 = entries[7]; m43 = entries[11]; m44 = entries[15];
}

void Matrix4::set_matrix_from_gl(double *entries)
{
	m11 = entries[0]; m12 = entries[4]; m13 = entries[8];  m14 = entries[12];
	m21 = entries[1]; m22 = entries[5]; m23 = entries[9];  m24 = entries[13];
	m31 = entries[2]; m32 = entries[6]; m33 = entries[10]; m34 = entries[14];
	m41 = entries[3]; m42 = entries[7]; m43 = entries[11]; m44 = entries[15];
}

void Matrix4::set_matrix_from_raw(float *entries)
{
	m11 = entries[0]; m21 = entries[4]; m31 = entries[8];  m41 = entries[12];
	m12 = entries[1]; m22 = entries[5]; m32 = entries[9];  m42 = entries[13];
	m13 = entries[2]; m23 = entries[6]; m33 = entries[10]; m43 = entries[14];
	m14 = entries[3]; m24 = entries[7]; m34 = entries[11]; m44 = entries[15];
}


/*=========================================================*\
|	multiply_with_gl_matrix 
|		- multiplies the matrix with current openGL matrix
|		- this mereley updates OpenGL's current matrix
|		- and the result is not stored in the Matrix4 class
|		- that outcome could be added with another function
\*=========================================================*/
void Matrix4::multiply_with_gl_matrix()
{
	float entries[16];

	entries[0] = m11; entries[4] = m12; entries[8]  = m13; entries[12] = m14;
	entries[1] = m21; entries[5] = m22; entries[9]  = m23; entries[13] = m24;
	entries[2] = m31; entries[6] = m32; entries[10] = m33; entries[14] = m34;
	entries[3] = m41; entries[7] = m42; entries[11] = m43; entries[15] = m44;

	glMultMatrixf(entries);
}

/*=========================================================*\
|	determinat 
|		- gets the determinant of a matrix |M|
\*=========================================================*/
float Matrix4::determinant()
{
	float det =	m11 * (m22 * m33 - m23 * m32)
			+	m12 * (m23 * m31 - m21 * m33)
			+	m13 * (m21 * m32 - m22 * m31);
	return det;

	//this is a 4x4 matrix determinant
	/*float det =   
	  m11 * ( m22 * (m33 * m44 - m34 * m43) - m32 * (m23 * m44 + m24 * m43) + m42 * (m23 * m34 - m24 * m33))
	- m21 * ( m12 * (m33 * m44 - m34 * m43) - m32 * (m13 * m44 + m14 * m43) + m42 * (m13 * m34 - m14 * m33))
	+ m31 * ( m12 * (m23 * m44 - m24 * m43) - m22 * (m13 * m44 + m14 * m43) + m42 * (m13 * m24 - m14 * m23))
	- m14 * (+m12 * (m23 * m34 - m24 * m33) - m22 * (m13 * m34 + m14 * m33) + m32 * (m13 * m24 - m14 * m23));
	return det;*/
}

/*=========================================================*\
|	invert 
|		- Inverts the current matrix so when multiplied by 
|		- the non inverse the result is an identity matrix
|		- Matrix is auto transposed, no need for transpose call
\*=========================================================*/
void Matrix4::invert()
{
	float det = determinant();

	if(psmath::absolute(det) == 0.0f) return;

	float one_over_det = 1.0f / det;

	//calculate new matrix in a temporary variable
	Matrix4 result;

	result.m11 = (m22 * m33 - m23 * m32) * one_over_det;
	result.m12 = (m13 * m32 - m12 * m33) * one_over_det;
	result.m13 = (m12 * m23 - m13 * m22) * one_over_det;

	result.m21 = (m23 * m31 - m21 * m33) * one_over_det;
	result.m22 = (m11 * m33 - m13 * m31) * one_over_det;
	result.m23 = (m13 * m21 - m11 * m23) * one_over_det;

	result.m31 = (m21 * m32 - m22 * m31) * one_over_det;
	result.m32 = (m12 * m31 - m11 * m32) * one_over_det;
	result.m33 = (m11 * m22 - m12 * m21) * one_over_det;

	result.m14 = -(m14 * result.m11 + m24 * result.m21 + m34 * result.m31);
	result.m24 = -(m14 * result.m12 + m24 * result.m22 + m34 * result.m32);
	result.m34 = -(m14 * result.m13 + m24 * result.m23 + m34 * result.m33);

	result.m41 = 0.0f;
	result.m42 = 0.0f;
	result.m43 = 0.0f;
	result.m44 = 1.0f;

	//copy the result over
	m11 = result.m11; m12 = result.m12; m13 = result.m13; m14 = result.m14;
	m21 = result.m21; m22 = result.m22; m23 = result.m23; m24 = result.m24;
	m31 = result.m31; m32 = result.m32; m33 = result.m33; m34 = result.m34;
	m41 = result.m41; m42 = result.m42; m43 = result.m43; m44 = result.m44;
}

/*=========================================================*\
|	get_translation 
|		- gets the translation portion of the matrix
\*=========================================================*/
Vector3f Matrix4::get_translation()
{
	return Vector3f(m14,m24,m34);
}

/*=========================================================*\
|	get_orientation 
|		- returns a rotation matrix
\*=========================================================*/
Matrix4 Matrix4::get_orientation()
{
	Matrix4 om = *this;
	// zero pos
	om.m14 = 0.f; om.m24 = 0.f; om.m34 = 0.f;

	// one scale
	//om.m11 = 1.f; om.m22 = 1.f; om.m33 = 1.f; om.m44 = 1.f;
	return om;
}

void Matrix4::create_bias()
{
	m11 = 0.5f;
	m21 = 0.0f;
	m31 = 0.0f;
	m41 = 0.0f;

	m12 = 0.0f;
	m22 = 0.5f;
	m32 = 0.0f;
	m42 = 0.0f;

	m13 = 0.0f;
	m23 = 0.0f;
	m33 = 0.5f;
	m43 = 0.0f;

	m14 = 0.5f;
	m24 = 0.5f;
	m34 = 0.5f;
	m44 = 1.0f;
}

void Matrix4::create_axis_swap( Vector3f x, Vector3f y, Vector3f z )
{
	m11 = x.x;
	m21 = x.y;
	m31 = x.z;
	m41 = 0.0f;

	m12 = y.x;
	m22 = y.y;
	m32 = y.z;
	m42 = 0.0f;

	m13 = z.x;
	m23 = z.y;
	m33 = z.z;
	m43 = 0.0f;

	m14 = 0.0f;
	m24 = 0.0f;
	m34 = 0.0f;
	m44 = 1.0f;
}

Matrix3::Matrix3( Matrix4 extract )
{
	m11 = extract.m11; m12 = extract.m12; m13 = extract.m13;
	m21 = extract.m21; m22 = extract.m22; m23 = extract.m23;
	m31 = extract.m31; m32 = extract.m23; m33 = extract.m33; 
}

void Matrix3::gl_compliant_matrix( float *entries )
{
	entries[0] = m11; entries[3] = m12; entries[6] = m13;
	entries[1] = m21; entries[4] = m22; entries[7] = m23;
	entries[2] = m31; entries[5] = m32; entries[8] = m33;
}