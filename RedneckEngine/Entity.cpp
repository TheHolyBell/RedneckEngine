#include "Entity.h"
#include <cassert>

using Bind::IndexBuffer;

void Entity::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	for (auto& b : m_binds)
		b->Bind(gfx);
	gfx.DrawIndexed(m_pIndexBuffer->GetCount());
}

void Entity::AddBind(std::shared_ptr<Bind::IBindable> bind) noexcept(!IS_DEBUG)
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && m_pIndexBuffer == nullptr);
		m_pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	m_binds.push_back(std::move(bind));
}
