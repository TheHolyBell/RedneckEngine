#pragma once
#include <btBulletDynamicsCommon.h>

class IRigidBody
{
public:
	virtual ~IRigidBody() = default;

	virtual btRigidBody* GetRigidBody() const noexcept = 0;
};