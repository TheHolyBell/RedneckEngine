#pragma once
#include "PhysicsEntity.h"
#include "TestPlane.h"

class PhysicsPlane : public TestPlane, public IPhysicsEntity
{
public:
	PhysicsPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f,1.0f });

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	virtual btRigidBody* GetRigidBody() const noexcept override;

	virtual ~PhysicsPlane();
private:
	btRigidBody* m_RigidBody = nullptr;
};