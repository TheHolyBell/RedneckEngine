#include "PhysicsCamera.h"

PhysicsCamera::PhysicsCamera() noexcept
{
	auto _position = GetPos();

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(_position.x, _position.y, _position.z));
	btSphereShape* sphere = new btSphereShape(5);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(5, motion, sphere);
	m_RigidBody = std::make_unique<btRigidBody>(info);
	m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() |
		btCollisionObject::CF_KINEMATIC_OBJECT);
	m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
}

void PhysicsCamera::Update(float dt)
{
	Camera::Update(dt);

	auto _position = GetPos();
	_position = {};
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(_position.x, _position.y, _position.z));

	m_RigidBody->setWorldTransform(t);
}

btRigidBody* PhysicsCamera::GetRigidBody() const noexcept
{
	return m_RigidBody.get();
}
