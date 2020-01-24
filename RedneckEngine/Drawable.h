#pragma once

#include "IDrawable.h"
#include "IUpdateable.h"
#include "IMenuViewable.h"
#include "IndexBuffer.h"
#include "IBindable.h"
#include "IUID.h"
#include <string>
#include <DirectXMath.h>

namespace Bind
{
	class IBindable;
	class IndexBuffer;
}

using namespace Bind;

class Drawable
{
public:
	Drawable() = default;

	Drawable(const Drawable& rhs) = delete;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	virtual ~Drawable() = default;
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