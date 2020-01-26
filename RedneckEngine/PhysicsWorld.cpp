#include "PhysicsWorld.h"

btDynamicsWorld* PhysicsWorld::m_world = nullptr;
btDispatcher* PhysicsWorld::m_dispatcher = nullptr;
btCollisionConfiguration* PhysicsWorld::m_collisionConfig = nullptr;
btBroadphaseInterface* PhysicsWorld::m_broadphase = nullptr;
btConstraintSolver* PhysicsWorld::m_solver = nullptr;

void PhysicsWorld::Initialize()
{
	m_collisionConfig = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
	m_world->setGravity(btVector3(0, -10, 0));	//gravity on Earth
}

void PhysicsWorld::Shutdown()
{
	delete m_solver;
	delete m_collisionConfig;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_world;
}

void PhysicsWorld::AddRigidBody(std::shared_ptr<IRigidBody> entity)
{
	m_world->addRigidBody(entity->GetRigidBody());
}

void PhysicsWorld::RemoveRigidBody(IRigidBody* entity)
{
	m_world->removeRigidBody(entity->GetRigidBody());
}

void PhysicsWorld::AddCollidableBody(ICollidable* entity)
{
	m_world->addCollisionObject(entity->GetCollisionObject());
}

void PhysicsWorld::Update(float dt)
{
	auto& arr = m_world->getCollisionObjectArray();
	for (int i = 0; i < arr.size(); ++i)
		arr[i]->activate(true);
	m_world->stepSimulation(dt);
}
