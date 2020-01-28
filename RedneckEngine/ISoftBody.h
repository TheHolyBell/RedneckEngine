#pragma once
#include <btBulletDynamicsCommon.h>
#include "BulletPhysics\BulletSoftBody\btSoftRigidDynamicsWorld.h"

class ISoftBody
{
public:
	virtual btSoftBody* GetSoftBody() noexcept = 0;

	virtual ~ISoftBody() = default;
};