#include "PhysicsSphere.h"
#include "ConsoleClass.h"
#include "PhysicsWorld.h"

PhysicsSphere::PhysicsSphere(Graphics& gfx, float radius, float mass)
	: SolidSphere(gfx, radius)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	btSphereShape* sphere = new btSphereShape(radius);
	btVector3 inertia(0, 0, 0);
	if (mass != 0)
		sphere->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
	m_RigidBody = std::make_unique<btRigidBody>(info);

	m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
}

void PhysicsSphere::Update(float dt)
{
	if (m_pos.y < -500)
		PhysicsWorld::RemoveRigidBody(this);
}

DirectX::XMMATRIX PhysicsSphere::GetTransformXM() const noexcept
{
	using namespace DirectX;
	btTransform t;
	m_RigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	XMMATRIX _matrix(mat);
	PhysicsSphere* _this = const_cast<PhysicsSphere*>(this);
	_this->m_pos.x = XMVectorGetX(_matrix.r[3]);
	_this->m_pos.y = XMVectorGetY(_matrix.r[3]);
	_this->m_pos.z = XMVectorGetZ(_matrix.r[3]);

	return _matrix;
}

btRigidBody* PhysicsSphere::GetRigidBody() const noexcept
{
	return m_RigidBody.get();
}

void PhysicsSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	SolidSphere::SetPos(pos);
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	m_RigidBody->setMotionState(new btDefaultMotionState(t));
	m_RigidBody->activate(true);
}

PhysicsSphere::~PhysicsSphere()
{
	Console::WriteLine("%s been called", __FUNCTION__);
	PhysicsWorld::RemoveRigidBody(this);
}
