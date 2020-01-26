#include "PhysicsBox.h"
#include "PhysicsWorld.h"

PhysicsBox::PhysicsBox(Graphics& gfx, float size, float mass)
	: TestCube(gfx, size)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	btBoxShape* box = new btBoxShape(btVector3(size / 2.0, size / 2.0, size / 2.0));
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		box->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
	m_RigidBody = std::make_unique<btRigidBody>(info);
}

DirectX::XMMATRIX PhysicsBox::GetTransformXM() const noexcept
{
	using namespace DirectX;
	btTransform t;
	m_RigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	XMMATRIX _matrix(mat);
	PhysicsBox* _this = const_cast<PhysicsBox*>(this);
	_this->m_pos.x = XMVectorGetX(_matrix.r[3]);
	_this->m_pos.y = XMVectorGetY(_matrix.r[3]);
	_this->m_pos.z = XMVectorGetZ(_matrix.r[3]);

	return _matrix;
}

btRigidBody* PhysicsBox::GetRigidBody() const noexcept
{
	return m_RigidBody.get();
}

void PhysicsBox::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	TestCube::SetPos(pos);
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	t.setRotation(btQuaternion(yaw, pitch, roll));
	m_RigidBody->setMotionState(new btDefaultMotionState(t));
	m_RigidBody->activate(true);
}

PhysicsBox::~PhysicsBox()
{
	PhysicsWorld::RemoveRigidBody(this);
}
