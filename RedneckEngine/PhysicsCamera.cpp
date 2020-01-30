#include "PhysicsCamera.h"
#include "ConsoleClass.h"

PhysicsCamera::PhysicsCamera() noexcept
{
	auto _position = GetPos();

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(_position.x, _position.y, _position.z));
	btSphereShape* sphere = new btSphereShape(5);

	btMotionState* motion = new btDefaultMotionState(t);
	m_RigidBody = std::make_unique<btRigidBody>(0, motion, sphere);
	m_RigidBody->setActivationState(DISABLE_DEACTIVATION);


}

void PhysicsCamera::Update(float dt)
{
	Camera::Update(dt);

	auto _position = GetPos();
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(_position.x, _position.y, _position.z));

	m_RigidBody->setWorldTransform(t);
}

btRigidBody* PhysicsCamera::GetRigidBody() const noexcept
{
	return m_RigidBody.get();
}
