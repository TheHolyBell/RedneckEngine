#include "PhysicsCamera.h"

PhysicsCamera::PhysicsCamera() noexcept
{
	auto _position = GetPos();

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(_position.x, _position.y, _position.z));
	btSphereShape* sphere = new btSphereShape(5);

	btMotionState* motion = new btDefaultMotionState(t);
	m_CollisionObject = std::make_unique<btCollisionObject>();
	m_CollisionObject->setCollisionShape(sphere);
}

void PhysicsCamera::Update(float dt)
{
	Camera::Update(dt);

	auto _position = GetPos();
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(_position.x, _position.y, _position.z));

	m_CollisionObject->setWorldTransform(t);
}

btCollisionObject* PhysicsCamera::GetCollisionObject() noexcept
{
	return m_CollisionObject.get();
}
