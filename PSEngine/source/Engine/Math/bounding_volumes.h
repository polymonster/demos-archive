/*=========================================================*\
|	bounding_volumes.h - bounding volume classes
|-----------------------------------------------------------|
|				Project :	PolySpoon Math
|				Coder	:	ADixon
|				Date	:	26/04/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef _BOUNDING_VOLUMES_H
#define _BOUNDING_VOLUMES_H
/*======================== INCLUDES =======================*/

#include "vector.h"
#include "matrix.h"

/*======================== TYPEDEFS ======================*/

typedef std::vector< unsigned int > IndexArray;

/*======================== CONSTANTS ======================*/

/*========================= CLASSES =======================*/

/*=========================================================*\
|	OBB3D - 3D Oriented Bounding Box
\*=========================================================*/
class OBB3D
{
	public:

		OBB3D();
		OBB3D(Vector3f position, Vector3f size, Vector3f rotation);
		OBB3D(Matrix4 m_orientation_matrix);

		Matrix4 m_orientation_matrix;

		Vector3f m_position;
		Vector3f m_size;
		Vector3f m_rotation;

		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();
		void build_matrix();

	private:

		Vector3f m_widget_selected_axis;
};

/*=========================================================*\
|	Convex Hull - 3D Convex shape
\*=========================================================*/
class CONVEX_HULL
{
	public:

		CONVEX_HULL(){};
		CONVEX_HULL(Vector3f position, Vector3f size, Vector3f rotation, Vector3fArray vertices, IndexArray indices);

		Matrix4 m_orientation_matrix;
		Matrix4 m_rotation_matrix;

		Vector3f m_position;
		Vector3f m_size;
		Vector3f m_rotation;

		Vector3fArray m_vertices;
		Vector3fArray m_axes;

		IndexArray m_indices;

		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();
		void build_matrix();

	private:

		Vector3f m_widget_selected_axis;
};


/*=========================================================*\
|	AABB3D - 3D Axially Aligned Bounding Box
\*=========================================================*/
class AABB3D
{
	public:

		AABB3D();
		AABB3D(Vector3f position, Vector3f size);

		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

		Vector3f m_position;
		Vector3f m_size;

	private:

		Vector3f m_widget_selected_axis;

};

/*=========================================================*\
|	SPHERE - 3D Sphere
\*=========================================================*/
class SPHERE
{
	public:

		SPHERE();
		SPHERE(Vector3f position, float radius );

		Vector3f m_position;
		float m_radius;
		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;

};

/*=========================================================*\
|	CIRCLE - 2D Circle
\*=========================================================*/
class CIRCLE
{
	public:

		CIRCLE();
		CIRCLE(Vector3f position, float radius );

		Vector3f m_position;
		float m_radius;
		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;

};

/*=========================================================*\
|	AA_ELLIPSOID - 3D Axially Aligned Ellipsoid
\*=========================================================*/
class AA_ELLIPSOID
{
	public:

		AA_ELLIPSOID();
		AA_ELLIPSOID(Vector3f position, Vector3f radii );

		Vector3f m_position;
		Vector3f m_radii;
		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;

};

/*=========================================================*\
|	TRIANGLE
\*=========================================================*/
class TRIANGLE
{
	public:

		TRIANGLE();
		TRIANGLE(Vector3fArray vertices);

		Vector3f m_vertices[3];

		bool m_under_collision;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;

};

/*=========================================================*\
|	LINE_2D - 3D Line
\*=========================================================*/
class LINE_3D
{
	public:

		LINE_3D();
		LINE_3D(Vector3f line_start, Vector3f line_end);

		Vector3f m_line_start;
		Vector3f m_line_end;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;
};

/*=========================================================*\
|	LINE_2D - 2D Line
\*=========================================================*/
class LINE_2D
{
	public:

		LINE_2D();
		LINE_2D(Vector2f line_start, Vector2f line_end);

		Vector2f m_line_start;
		Vector2f m_line_end;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;
};

/*=========================================================*\
|	RAY_3D - 3D Ray
\*=========================================================*/
class RAY_3D
{
	public:

		RAY_3D();
		RAY_3D(Vector3f point_on_ray, Vector3f direction_vector);

		Vector3f m_point_on_ray;
		Vector3f m_direction_vector;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;
};

/*=========================================================*\
|	RAY_2D - 2D Ray
\*=========================================================*/
class RAY_2D
{
	public:

		RAY_2D();
		RAY_2D(Vector2f point_on_ray, Vector2f direction_vector);

		Vector2f m_point_on_ray;
		Vector2f m_direction_vector;

		void update();
		void render();
		void aux_render();
		void back_buffer_render();

	private:

		Vector3f m_widget_selected_axis;
};

/*================== EXTERNAL VARIABLES ===================*/

#endif //_BOUNDING_VOLUMES_H