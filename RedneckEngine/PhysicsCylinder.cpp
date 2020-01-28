#include "PhysicsCylinder.h"
#include "PhysicsWorld.h"

PhysicsCylinder::PhysicsCylinder(Graphics& gfx, float bottomRadius, float topRadius, float height, float mass)
	: TestCylinder(gfx, bottomRadius, topRadius, height)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	
	// Cylinder part
	if (topRadius > 0.1 && bottomRadius > 0.1)
	{
		btCylinderShape* cylinder = new btCylinderShape(btVector3(bottomRadius, height / 2, topRadius));
		btVector3 inertia(0, 0, 0);
		if (mass != 0)
			cylinder->calculateLocalInertia(mass, inertia);

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
		m_RigidBody = std::make_unique<btRigidBody>(info);
	}
	else
	{
		btConeShape* cone = new btConeShape(bottomRadius, height);
		btVector3 inertia(0, 0, 0);
		if (mass != 0)
			cone->calculateLocalInertia(mass, inertia);

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cone, inertia);
		m_RigidBody = std::make_unique<btRigidBody>(info);
	}
	m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
}

DirectX::XMMATRIX PhysicsCylinder::GetTransformXM() const noexcept
{
	using namespace DirectX;
	btTransform t;
	m_RigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	XMMATRIX _matrix(mat);
	PhysicsCylinder* _this = const_cast<PhysicsCylinder*>(this);
	_this->m_pos.x = XMVectorGetX(_matrix.r[3]);
	_this->m_pos.y = XMVectorGetY(_matrix.r[3]);
	_this->m_pos.z = XMVectorGetZ(_matrix.r[3]);

	return _matrix;
}

void PhysicsCylinder::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	TestCylinder::SetPos(pos);

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	t.setRotation(btQuaternion(yaw, pitch, roll));
	m_RigidBody->getWorldTransform() = t;
}

btRigidBody* PhysicsCylinder::GetRigidBody() const noexcept
{
	return m_RigidBody.get();
}

PhysicsCylinder::~PhysicsCylinder()
{
	PhysicsWorld::RemoveRigidBody(this);
}
