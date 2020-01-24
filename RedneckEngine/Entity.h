#pragma once
#include "IDrawable.h"
#include "IUpdateable.h"
#include "IMenuViewable.h"
#include "IndexBuffer.h"
#include "IBindable.h"
#include "IUID.h"
#include <string>
#include <DirectXMath.h>

class Entity : public IDrawable, public IUpdateable, public IMenuViewable
{
public:
	Entity() = default;

	Entity(const Entity& rhs) = delete;

	virtual void Update(float dt) override {}
	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override = 0;
	
	virtual ~Entity() = default;

};