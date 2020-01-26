#pragma once
#include "TestCylinder.h"
#include "IRigidBody.h"
#include <memory>

class PhysicsCylinder : public TestCylinder, public IRigidBody
{
public:
	PhysicsCylinder(Graphics& gfx, float bottomRadius, float topRadius, float height, float mass = 5.0);

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	virtual void SetPos(DirectX::XMFLOAT3 pos) noexcept override;
	virtual btRigidBody* GetRigidBody() const noexcept override;

	virtual ~PhysicsCylinder();
private:
	std::unique_ptr<btRigidBody> m_RigidBody;
};