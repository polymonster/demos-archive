/*=========================================================*\
|	render_helpers.cpp - helpers for rendering stuff.. quick
|-----------------------------------------------------------|
|				Project :	PolySpoon Helpers
|				Coder	:	ADixon
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "helpers.h"

/*======================== VARIABLES ======================*/
RenderObject *quad_mesh;
RenderObject *cube_mesh;
RenderObject *sphere_mesh;

void pshelpers::clean_up_render_helpers()
{
	delete quad_mesh;
	delete cube_mesh;
	delete sphere_mesh;
}

/*=========================================================*\
| initialise_render_helpers - 
|				creates render helper objects in video RAM
\*=========================================================*/
void pshelpers::initialise_render_helpers()
{
	//create vbos
	initialise_cube();
	initialise_quad();
	initialise_sphere();
}

/*=========================================================*\
| initialise_cube - 
|				creates a unit cube object in video RAM
\*=========================================================*/
void pshelpers::initialise_cube()
{
	cube_mesh = new RenderObject();

	//cube geometry
	Vector3fArray verts;
	verts.push_back(Vector3f(-1,-1,-1));
	verts.push_back(Vector3f(-1,-1,1));
	verts.push_back(Vector3f(1,-1,1));
	verts.push_back(Vector3f(1,-1,-1));
	verts.push_back(Vector3f(-1,1,-1));
	verts.push_back(Vector3f(-1,1,1));
	verts.push_back(Vector3f(1,1,1));
	verts.push_back(Vector3f(1,1,-1));

	//cube indices
	unsigned int inds[36] =
	{	
		0,3,2,
		2,1,0,

		4,5,6,
		6,7,4,

		0,1,5,
		5,4,0,

		2,3,7,
		7,6,2,

		1,2,6,
		6,5,1,

		0,4,7,
		7,3,0,
	};

	//build a mesh from the data
	cube_mesh->add_vertex_buffer(verts);
	cube_mesh->add_index_buffer(inds,36);
}

void pshelpers::initialise_quad()
{
	quad_mesh = new RenderObject();

	//cube geometry
	Vector3fArray verts;
	verts.push_back(Vector3f(-1, -1, 1));
	verts.push_back(Vector3f(-1,  1, 1));
	verts.push_back(Vector3f( 1,  1, 1));
	verts.push_back(Vector3f( 1, -1, 1));

	Vector2fArray texcoords;
	texcoords.push_back(Vector2f(0, 1));
	texcoords.push_back(Vector2f(0, 0));
	texcoords.push_back(Vector2f(1, 0));
	texcoords.push_back(Vector2f(1, 1));

	//cube indices
	unsigned int inds[6] =
	{	
		0,1,2,
		2,3,0,
	};

	//build a mesh from the data
	quad_mesh->add_vertex_buffer(verts);
	quad_mesh->add_texcoord_buffer(texcoords);
	quad_mesh->add_index_buffer(inds,6);
}

/*=========================================================*\
| initialise_sphere - 
|				creates a unit sphere object in video RAM
\*=========================================================*/
void pshelpers::initialise_sphere()
{
	sphere_mesh = new RenderObject(GL_STATIC_DRAW_ARB,GL_TRIANGLE_STRIP);

	Vector3f new_vert;
	Vector3fArray vbo_vertices;

	const float detail = 11.25f;
	for(float i = 0; i <= 180; i += detail)
	{
		for(float j = 360; j >= 0; j--)
		{
			Vector2f index[4];

			index[0].x = psmath::deg_to_rad(i); 
			index[0].y = psmath::deg_to_rad(j);

			index[1].x = psmath::deg_to_rad(i + detail);
			index[1].y = psmath::deg_to_rad(j); 

			index[2].x = psmath::deg_to_rad(i); 
			index[2].y = psmath::deg_to_rad(j + detail); 

			index[3].x = psmath::deg_to_rad(i + detail); 
			index[3].y = psmath::deg_to_rad(j + detail); 

			for(int k = 0; k < 4; k++)
			{
				new_vert.x = sin(index[k].y) * sin(index[k].x);
				new_vert.y = cos(index[k].y) * sin(index[k].x);
				new_vert.z = cos(index[k].x);

				vbo_vertices.push_back(new_vert);
			}
		}
	}

	sphere_mesh->add_vertex_buffer(vbo_vertices);
}

/*=========================================================*\
|	grid - draws a grid in the x/z plane with the specified 
|	properties
|
|	cell size defines the size of each cell
|	grid dimesion is the number of cells along z and x axes
|	divisions draw darker lines at constant gaps to help aid
|	referencing.
\*=========================================================*/
void pshelpers::grid(int cell_size, int grid_dimesion, int divisions)
{
	int grid_size = grid_dimesion * cell_size;

	int x = - ((grid_dimesion/2) * cell_size);
	int y = 0;
	int z = - ((grid_dimesion/2) * cell_size);

	int division_counter = divisions;

	for(int i = 0; i <= grid_dimesion; i++)
	{
		if((division_counter == 0) && (i != grid_dimesion)) 
		{
			division_counter = divisions;
			glColor3f(0.1f,0.1f,0.1f);
		}
		else glColor3f(0.7f,0.7f,0.7f);

		glBegin(GL_LINES);
			glVertex3i(x,y,z);
			glVertex3i(x + grid_size,y,z);
		glEnd();

		z += cell_size;

		division_counter--;
	}

	z = - ((grid_dimesion/2) * cell_size);
	division_counter = divisions;

	for(int i = 0; i <= grid_dimesion; i++)
	{
		if((division_counter == 0) && (i != grid_dimesion))
		{
			division_counter = divisions;
			glColor3f(0.1f,0.1f,0.1f);
		}
		else glColor3f(0.7f,0.7f,0.7f);

		glBegin(GL_LINES);
			glVertex3i(x,y,z);
			glVertex3i(x,y,z + grid_size);
		glEnd();

		x += cell_size;

		division_counter--;
	}
}

/*=========================================================*\
| cube - 
|	draws a cube on demand, defaults at origin and scale 1
\*=========================================================*/
void pshelpers::cube(Vector3f position, Vector3f scale)
{
	glPushMatrix();
	
		ps_translate(position);
		ps_scale(scale);

		cube_mesh->render();

	glPopMatrix();
}

/*=========================================================*\
| sphere / ellipsoid - 
|	draws a spherical objects on demand, 
|	defaults at origin and scale 1
\*=========================================================*/
void pshelpers::sphere(Vector3f position, float radius)
{
	glPushMatrix();
	
		ps_translate(position);
		ps_scale(Vector3f(radius,radius,radius));

		sphere_mesh->render();

	glPopMatrix();
}

void pshelpers::ellipsoid(Vector3f position, Vector3f radii)
{
	glPushMatrix();
	
		ps_translate(position);
		ps_scale(radii);

		sphere_mesh->render();

	glPopMatrix();
}

void pshelpers::screen_space_quad(Vector2i size)
{
	glMatrixMode(GL_PROJECTION);						
	glPushMatrix();									
	glLoadIdentity();								

		glOrtho(0, size.x,size.y, 0, 0, 1);	

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();	
			glLoadIdentity();

			Vector2i half_size;
			half_size.x = size.x / 2;
			half_size.y = size.y / 2;

			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
				glTexCoord2f(0,1); glVertex2f(size.x / 2 - half_size.x, size.y / 2 - half_size.y);
				glTexCoord2f(0,0); glVertex2f(size.x / 2 - half_size.x, size.y / 2 + half_size.y);
				glTexCoord2f(1,0); glVertex2f(size.x / 2 + half_size.x, size.y / 2 + half_size.y); 
				glTexCoord2f(1,1); glVertex2f(size.x / 2 + half_size.x, size.y / 2 - half_size.y); 
			glEnd();

			//glScaled(half_size.x, half_size.y, 1);
			//quad_mesh->render();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

}


/*=========================================================*\
| Translate, Rotate and Scale - 
|	Speeds up OpenGL commands by taking Vector3f's
\*=========================================================*/
void pshelpers::ps_translate(Vector3f position)
{
	glTranslatef(position.x,position.y,position.z);
}

void pshelpers::ps_rotate(Vector3f rotation)
{
	glRotatef(rotation.x,1,0,0);
	glRotatef(rotation.y,0,1,0);
	glRotatef(rotation.z,0,0,1);
}

void pshelpers::ps_scale(Vector3f scale)
{
	glScalef(scale.x,scale.y,scale.z);
}