/*=========================================================*\
|	physics_manager.cpp - physics_manager class
|-----------------------------------------------------------|
|				Project :	PolySpoon Physics
|				Coder	:	ADixon
|				Date	:	20/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

#include "physics_manager.h"
#include "window.h"
#include "timer.h"

#define MULTITHREADED_PHYSICS
//multi threading support
#ifdef MULTITHREADED_PHYSICS
	#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"
	#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
	#include "BulletMultiThreaded/PlatformDefinitions.h"
#endif

#include <iostream>

PhysicsManager *g_physics_mgr_p;

PhysicsManager::PhysicsManager()
{
	m_collision_configuration = new btDefaultCollisionConfiguration();

#ifdef MULTITHREADED_PHYSICS

	//multi threaded dispatcher
	int outstanding_tasks = 1; //what the hell is this?
	m_collision_thread = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
		"ps dispatcher thread",
		processCollisionTask,
		createCollisionLocalStoreMemory,
		outstanding_tasks));

	m_dispatcher = new	SpuGatheringCollisionDispatcher(m_collision_thread,outstanding_tasks,m_collision_configuration);

#else

	//single threaded dispatcher
	m_dispatcher = new	btCollisionDispatcher(m_collision_configuration);

#endif


	//single threaded dispatcher
	m_solver = new btSequentialImpulseConstraintSolver;

	//setup physics world

	//broadphase
	btVector3 min_AABB(-2000,-2000,-2000);
	btVector3 max_AABB(2000,2000,2000);

	m_broadphase = new btAxisSweep3(min_AABB, max_AABB, max_proxies);
	//m_broadphase = new btDbvtBroadphase();


	m_dynamics_world = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collision_configuration);
	m_dynamics_world->setGravity(btVector3(0,-100,0));

}

PhysicsManager::~PhysicsManager()
{

}

int PhysicsManager::add_collision_shape(Vector3f dimensions)
{
	btVector3 bt_dimensions = btVector3(dimensions.x, dimensions.y, dimensions.z );

	///create a few basic rigid bodies
	btCollisionShape* shape = new btBoxShape( bt_dimensions );

	m_collision_shapes.push_back(shape);

	//return a collision shape handle
	return m_collision_shapes.size() - 1;
}

int PhysicsManager::add_rigid_body(float mass, Vector3f origin, int collision_shape_id)
{
	btScalar bt_mass( mass );

	//rigid body is dynamic if and only if mass is non zero, otherwise static
	bool is_dynamic = (bt_mass != 0.f);

	//start at no local inertia
	btVector3 local_inertia(0, 0, 0);

	if (is_dynamic)
	{
		m_collision_shapes[ collision_shape_id ]->calculateLocalInertia(bt_mass, local_inertia);
	}

	//origin transform for starting position
	btTransform origin_transform;
	origin_transform.setIdentity();
	origin_transform.setOrigin( btVector3(origin.x, origin.y, origin.z) );

	//using motion state is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* motion_state = new btDefaultMotionState(origin_transform);

	btRigidBody::btRigidBodyConstructionInfo rigid_body_info(bt_mass, motion_state, m_collision_shapes[ collision_shape_id ], local_inertia);

	btRigidBody* body = new btRigidBody(rigid_body_info);

	m_dynamics_world->addRigidBody(body);

	return m_dynamics_world->getNumCollisionObjects() - 1;
}

void PhysicsManager::update()
{
	if (m_dynamics_world)
	{
		m_dynamics_world->stepSimulation(g_game_timer.get_game_time());
	}
}

void PhysicsManager::draw_debug()
{
	for (int j=m_dynamics_world->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		btCollisionObject* obj = m_dynamics_world->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			float mat[16];
			trans.getOpenGLMatrix(mat);

			Matrix4 psMat;
			psMat.set_matrix_from_gl(mat);

			AABB3D box(Vector3f(0,0,0),Vector3f(32,32,32));

			glPushMatrix();

				psMat.multiply_with_gl_matrix();
				box.render();

			glPopMatrix();
		}
	}
}

bool PhysicsManager::extract_matrix( Matrix4 *ps_matrix, unsigned int body_id )
{
	if(body_id < m_dynamics_world->getNumCollisionObjects())
	{
		btCollisionObject* object = m_dynamics_world->getCollisionObjectArray()[body_id];
		btRigidBody* body = btRigidBody::upcast(object);

		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			float mat[16];
			trans.getOpenGLMatrix(mat);

			ps_matrix->set_matrix_from_gl(mat);

			return true;
		}
	}

	ps_matrix->identity();
	return false;
}