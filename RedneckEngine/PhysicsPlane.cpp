#include "PhysicsPlane.h"
#include "PhysicsWorld.h"
#include "ConsoleClass.h"

PhysicsPlane::PhysicsPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color)
	: TestPlane(gfx, size, color)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	m_RigidBody = new btRigidBody(info);
}

DirectX::XMMATRIX PhysicsPlane::GetTransformXM() const noexcept
{
	using namespace DirectX;
	btTransform t;
	m_RigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	XMMATRIX _matrix(mat);

	PhysicsPlane* _this = const_cast<PhysicsPlane*>(this);
	_this->m_pos.x = XMVectorGetX(_matrix.r[3]);
	_this->m_pos.y = XMVectorGetY(_matrix.r[3]);
	_this->m_pos.z = XMVectorGetZ(_matrix.r[3]);

	return _matrix;
}

btRigidBody* PhysicsPlane::GetRigidBody() const noexcept
{
	return m_RigidBody;
}

PhysicsPlane::~PhysicsPlane()
{
	Console::WriteLine("%s been called", __FUNCTION__);
	PhysicsWorld::RemoveEntity(this);
}
