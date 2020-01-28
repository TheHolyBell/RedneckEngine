#pragma once

#include <btBulletDynamicsCommon.h>
#include "BulletPhysics\BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletPhysics\BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletPhysics\BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletPhysics\BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"
#include "IRigidBody.h"
#include "ISoftBody.h"
#include "ICollidable.h"
#include <memory>

class PhysicsWorld
{
public:
	static void Initialize();
	static void Shutdown();

	static void AddSoftBody(std::shared_ptr<ISoftBody> entity);
	static void AddRigidBody(std::shared_ptr<IRigidBody> entity);
	static void AddRigidBody(IRigidBody* entity);
	static void RemoveRigidBody(IRigidBody* entity);
	static void AddCollidableBody(ICollidable* entity);
	static void Update(float dt);
private:
	static btSoftRigidDynamicsWorld* m_world;
	static btBroadphaseInterface* m_broadphase;
	static btDispatcher* m_dispatcher;
	static btCollisionConfiguration* m_collisionConfig;
	static btConstraintSolver* m_solver;
	static btSoftBodySolver* m_softBodySolver;
};