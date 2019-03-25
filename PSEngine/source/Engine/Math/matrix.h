/*=========================================================*\
|	matrix.h - vector classes and math
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ADixon
|				Date	:	08/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _MATRIX_H
#define _MATRIX_H

/*======================== INCLUDES =======================*/

#include <math.h>
#include "console.h"
#include "vector.h"

/*======================== CONSTANTS ======================*/

typedef enum
{
	X_AXIS = 0,
	Y_AXIS = 1,
	Z_AXIS = 2,

}e_cardinal_axes;

/*========================= CLASSES =======================*/
class Matrix4;
class Matrix3;

/*=========================================================*\
|	Matrix4 - 4 x 4 Matrix - Row Major Format
\*=========================================================*/
class Matrix3
{
public:

	Matrix3(Matrix4 extract);
	Matrix3(){};
	void gl_compliant_matrix(float *entries);

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
};

class Matrix4
{

public:

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;

	void identity();

	void create_translation(Vector3f t);
	void create_cardinal_rotation(int axis, float theta);
	void create_arbitrary_rotation(Vector3f axis, float theta);
	void create_scale(Vector3f s);
	void create_axis_swap(Vector3f x, Vector3f y, Vector3f z);

	Vector3f get_translation();
	Matrix4 get_orientation();

	float determinant();
	void invert();
	void transpose();

	void create_bias();

	void gl_compliant_matrix(float *entries);
	void set_matrix_from_gl(float *entries);
	void set_matrix_from_gl(double *entries);
	void set_matrix_from_raw(float *entries);

	void multiply_with_gl_matrix();

	Vector3f homogenous_multiply(Vector3f v, float *w);

	const Vector3f operator *(const Vector3f &p);
	Matrix4 operator *(Matrix4 &b);
};

typedef std::vector< Matrix4 > Matrix4Array;
typedef std::vector< Matrix3 > Matrix3Array;

#endif // _MATRIX_H 

