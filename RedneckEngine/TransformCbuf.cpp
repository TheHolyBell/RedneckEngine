#include "TransformCbuf.h"

namespace Bind
{
	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::m_pVcbuf;

	TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
		:m_parent(parent)
	{
		if (m_pVcbuf == nullptr)
			m_pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}

	void TransformCbuf::Bind(Graphics& gfx) noexcept
	{
		UpdateBindImpl(gfx, GetTransforms(gfx));
	}

	void TransformCbuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		m_pVcbuf->Update(gfx, tf);
		m_pVcbuf->Bind(gfx);
	}

	TransformCbuf::Transforms TransformCbuf::GetTransforms(Graphics& gfx) noexcept
	{
		const auto model = m_parent.GetTransformXM();
		return {
			DirectX::XMMatrixTranspose(model),
			DirectX::XMMatrixTranspose(
				model *
				gfx.GetView() *
				gfx.GetProjection()
			)
		};
	}


}