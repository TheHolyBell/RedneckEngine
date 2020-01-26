#pragma once
#include <btBulletDynamicsCommon.h>

class IPhysicsEntity
{
public:
	virtual ~IPhysicsEntity() = default;

	virtual btRigidBody* GetRigidBody() const noexcept = 0;
};