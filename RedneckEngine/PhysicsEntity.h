#pragma once
#include "Entity.h"
#include "Drawable.h"

class PhysicsEntity : public Entity, public Drawable
{
public:
	PhysicsEntity(std::shared_ptr<Drawable> entity);

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
private:
	std::shared_ptr<Drawable> m_Entity;
};