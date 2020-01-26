#pragma once
#include "SolidSphere.h"
#include "PhysicsEntity.h"

class PhysicsSphere : public SolidSphere, public IPhysicsEntity
{
public:
	PhysicsSphere(Graphics& gfx, float radius, float mass = 2);

	virtual void Update(float dt) override;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	virtual btRigidBody* GetRigidBody() const noexcept override;
	virtual void SetPos(DirectX::XMFLOAT3 pos) noexcept override;

	virtual ~PhysicsSphere();

private:
	btRigidBody* m_RigidBody = nullptr;
};