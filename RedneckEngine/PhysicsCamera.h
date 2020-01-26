#pragma once
#include "ICollidable.h"
#include "Camera.h"
#include <memory>

class PhysicsCamera : public Camera, public ICollidable
{
public:
	PhysicsCamera() noexcept;

	virtual void Update(float dt) override;
	virtual btCollisionObject* GetCollisionObject() noexcept override;
private:
	std::unique_ptr<btCollisionObject> m_CollisionObject;
};