#pragma once
#include <btBulletCollisionCommon.h>

class ICollidable
{
public:

	virtual btCollisionObject* GetCollisionObject() noexcept = 0;

	virtual ~ICollidable() = default;
};