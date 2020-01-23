#pragma once
#include "IDrawable.h"
#include "IUpdateable.h"
#include "IMenuViewable.h"
#include "IndexBuffer.h"
#include "IBindable.h"
#include "IUID.h"
#include <string>
#include <DirectXMath.h>

class Entity : public IDrawable, public IUpdateable, public IUID
{
public:
	Entity() = default;

	Entity(const Entity& rhs) = delete;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

	virtual void Update(float dt) override {}
	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override;
	
	virtual ~Entity() = default;
	template<typename T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : m_binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
protected:
	void AddBind(std::shared_ptr<Bind::IBindable> bind) noexcept(!IS_DEBUG);
private:
	const Bind::IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::IBindable>> m_binds;
};