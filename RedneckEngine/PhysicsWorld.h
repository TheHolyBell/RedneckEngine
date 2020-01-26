#pragma once

#include <btBulletDynamicsCommon.h>
#include "PhysicsEntity.h"
#include <memory>

class PhysicsWorld
{
public:
	static void Initialize();
	static void AddEntity(std::shared_ptr<IPhysicsEntity> entity);
	static void RemoveEntity(IPhysicsEntity* entity);
	static void Update(float dt);

private:
	static btDynamicsWorld* m_world;
	static btDispatcher* m_dispatcher;
	static btCollisionConfiguration* m_collisionConfig;
	static btBroadphaseInterface* m_broadphase;
	static btConstraintSolver* m_solver;
};