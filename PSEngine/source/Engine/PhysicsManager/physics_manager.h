/*=========================================================*\
|	physics_manager.h - physics_manager class
|-----------------------------------------------------------|
|				Project :	PolySpoon Physics
|				Coder	:	ADixon
|				Date	:	20/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#ifndef physics_manager_h__
#define physics_manager_h__

#include "btBulletDynamicsCommon.h"
#include "BulletMultiThreaded/Win32ThreadSupport.h"
#include "polyspoon_math.h"

const int max_proxies = 32766;

class PhysicsManager
{
public:

	PhysicsManager();
	~PhysicsManager();

	int add_collision_shape(Vector3f dimensions);
	int add_rigid_body(float mass, Vector3f origin, int collision_shape_id);

	bool extract_matrix( Matrix4 *ps_matrix, unsigned int body_id );

	void update();
	void draw_debug();

private:

	btAlignedObjectArray<btCollisionShape*>	m_collision_shapes;
	btBroadphaseInterface*	m_broadphase;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;
	btDefaultCollisionConfiguration* m_collision_configuration;
	btDynamicsWorld* m_dynamics_world;

	//multi threading
	class btThreadSupportInterface* m_collision_thread;
	class btThreadSupportInterface* m_solver_thread;
};

extern PhysicsManager *g_physics_mgr_p;

#endif // physics_manager_h__