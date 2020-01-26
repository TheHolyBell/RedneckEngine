#pragma once
#include "TestCube.h"
#include "IRigidBody.h"
#include <memory>


class PhysicsBox : public TestCube, public IRigidBody
{
public:
	PhysicsBox(Graphics& gfx, float size, float mass = 2.0f);

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	virtual btRigidBody* GetRigidBody() const noexcept override;
	virtual void SetPos(DirectX::XMFLOAT3 pos) noexcept override;

	virtual ~PhysicsBox();

private:
	std::unique_ptr<btRigidBody> m_RigidBody;
};