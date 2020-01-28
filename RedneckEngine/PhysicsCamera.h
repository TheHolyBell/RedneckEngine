#pragma once
#include "IRigidBody.h"
#include "Camera.h"
#include <memory>

class PhysicsCamera : public Camera, public IRigidBody
{
public:
	PhysicsCamera() noexcept;

	virtual void Update(float dt) override;
	virtual btRigidBody* GetRigidBody() const noexcept override;
private:
	std::unique_ptr<btRigidBody> m_RigidBody;
};