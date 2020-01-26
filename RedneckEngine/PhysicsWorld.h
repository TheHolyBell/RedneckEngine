#pragma once

#include <btBulletDynamicsCommon.h>
#include "IRigidBody.h"
#include "ICollidable.h"
#include <memory>

class PhysicsWorld
{
public:
	static void Initialize();
	static void Shutdown();

	static void AddRigidBody(std::shared_ptr<IRigidBody> entity);
	static void RemoveRigidBody(IRigidBody* entity);
	static void AddCollidableBody(ICollidable* entity);
	static void Update(float dt);
private:
	static btDynamicsWorld* m_world;
	static btBroadphaseInterface* m_broadphase;
	static btDispatcher* m_dispatcher;
	static btCollisionConfiguration* m_collisionConfig;
	static btConstraintSolver* m_solver;
};