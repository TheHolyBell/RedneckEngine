#include "PhysicsWorld.h"
#include "PyScriptManager.h"
#include <thread>

btSoftRigidDynamicsWorld* PhysicsWorld::m_world = nullptr;
btDispatcher* PhysicsWorld::m_dispatcher = nullptr;
btCollisionConfiguration* PhysicsWorld::m_collisionConfig = nullptr;
btBroadphaseInterface* PhysicsWorld::m_broadphase = nullptr;
btConstraintSolver* PhysicsWorld::m_solver = nullptr;
btSoftBodySolver* PhysicsWorld::m_softBodySolver = nullptr;

static bool _s_callback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	//PyScriptManager::Execute("Collision");

	return false;
}

void PhysicsWorld::Initialize()
{
	m_collisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_softBodySolver = new btDefaultSoftBodySolver();
	m_world = new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig, m_softBodySolver);
	m_world->setGravity(btVector3(0, -10, 0));	//gravity on Earth

	gContactAddedCallback = _s_callback;
}

void PhysicsWorld::Shutdown()
{
	delete m_solver;
	delete m_collisionConfig;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_world;
}

void PhysicsWorld::AddSoftBody(std::shared_ptr<ISoftBody> entity)
{
	m_world->addSoftBody(entity->GetSoftBody());
}

void PhysicsWorld::AddRigidBody(std::shared_ptr<IRigidBody> entity)
{
	m_world->addRigidBody(entity->GetRigidBody());
}

void PhysicsWorld::AddRigidBody(IRigidBody* entity)
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
	m_world->stepSimulation(dt);
}
