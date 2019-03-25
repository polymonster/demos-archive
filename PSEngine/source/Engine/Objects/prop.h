/*=========================================================*\
|	prop.h - model instance scene object
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	16/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "object.h"
#include "model.h"
#include "polyspoon_math.h"
#include "camera.h"


/*======================== CONSTANTS ======================*/


/*========================= CLASSES =======================*/
class Prop : public Object
{

public:

	Prop(){};
	Prop( int handle, Matrix4* mat );
	Prop( int handle, Vector3f origin, float mass, bool add_to_physics = true);
	~Prop();

	virtual void		update();
	void				render();
	virtual void		render(bool shadow_pass);

	void		physics_world_render();

	void		set_matrix( Matrix4 *mat )				{ m_transformation = *mat; }
	Matrix4*	get_matrix( void )						{ return &m_transformation; }

	Matrix4*	get_father_matrix( void )				{ return &m_father_transformation; }
	Matrix4*	get_grandfather_matrix( void )			{ return &m_grandfather_transformation; }

	bool		animated()								{ return m_animated; }

protected:

	int			m_model_handle;
	int			m_physics_handle;
	Matrix4		m_transformation;
	Matrix4		m_father_transformation;
	Matrix4		m_grandfather_transformation;
	Vector3f	m_centre_of_mass_offset;

	bool m_animated;

	bool		first_frame;
};

class AnimatedProp : public Prop
{

public:

	AnimatedProp(){};
	AnimatedProp( int handle, Matrix4* mat );
	~AnimatedProp(){};

	virtual void update();
	void render(bool shadow_pass = false);

protected:

	int m_state;
	int m_model_handle;
};

class PlayerCharacter : public AnimatedProp
{

public:

	PlayerCharacter(){};
	PlayerCharacter( int handle, Matrix4* mat );
	~PlayerCharacter();

	void update();
	void set_camera_position(Camera *p_camera);

private:

	Vector2f m_rotation;
	Vector3f m_position;
	Vector2f m_rotation_velocity;
	Vector3f m_movement_velocity;
	Vector3f m_heading;
	float	 m_speed;
	float	 m_look_speed;
};

/*========================= TYPEDEFS =======================*/
typedef std::vector< Prop* > PropArray;